
#include "MultiplayerGameInstance.h"
#include "Engine/Engine.h"
#include "OnlineSessionSettings.h"
#include "GameFramework/PlayerController.h"

#include "UI/MainMenu.h"
#include "UI/ServerInfo.h"
#include "CustomMacros.h"
#include "GameModes/LobbyGameMode.h"

// Doing this so I can test locally in one instance of the engine.
const static FName HOST_SESSION_NAME = TEXT("SessionGame");
const static FName JOIN_SESSION_NAME = TEXT("JoinSessionGame");

const static FName SERVER_SETTINGS_NAME_KEY = TEXT("ServerName");
const static FName SERVER_SETTINGS_PASSWORD_KEY = TEXT("ServerPassword");



void UMultiplayerGameInstance::Init() {
	OnlineSubsystem = IOnlineSubsystem::Get();
	if (validate(OnlineSubsystem != nullptr) == false) { return; };
	UE_LOG(LogTemp, Warning, TEXT("Using subsystem: %s"), *OnlineSubsystem->GetSubsystemName().ToString());
	SessionInterface = OnlineSubsystem->GetSessionInterface();
	if (validate(SessionInterface != nullptr) == false) { return; }
	SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMultiplayerGameInstance::OnCreateSession);
	SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UMultiplayerGameInstance::OnDestroySession);
	SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UMultiplayerGameInstance::OnFindSessionsComplete);
	SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UMultiplayerGameInstance::OnJoinSessionComplete);
}



void UMultiplayerGameInstance::OnCreateSession(FName SessionName, bool bSuccessfull) {
	if (validate(bSuccessfull) == false) { return; }

	UEngine* Engine = GetEngine();
	if (validate(Engine != nullptr) == false) { return; }
	Engine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Hosting!"));
	UWorld* World = GetWorld();
	if (validate(World != nullptr) == false) { return; }

	World->ServerTravel(LobbyGamePath + "?listen", true, true);
}


void UMultiplayerGameInstance::OnDestroySession(FName SessionName, bool bSuccessfull) {
	if (validate(bSuccessfull) == false) { return; }

	if (SessionName == HOST_SESSION_NAME) {
		if (validate(CurrentServerSettings.IsSet()) == false) { return; }
		FCustomServerSettings ServerSettings = CurrentServerSettings.Get(FCustomServerSettings());
		Host(ServerSettings);
	}
}


void UMultiplayerGameInstance::Host(FCustomServerSettings ServerSettings) {
	if (validate(SessionInterface != nullptr) == false) { return; }
	CurrentServerSettings = ServerSettings;
	FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(HOST_SESSION_NAME);
	if (ExistingSession != nullptr) {
		SessionInterface->DestroySession(HOST_SESSION_NAME);
		return;
	}

	FOnlineSessionSettings SessionSettings;
#if WITH_EDITOR
	SessionSettings.bIsLANMatch = true;
#else
	SessionSettings.bIsLANMatch = false;
#endif
	SessionSettings.NumPublicConnections = ServerSettings.MaxPlayerCount;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true; // Needed for Steam sessions.
	SessionSettings.Set(SERVER_SETTINGS_NAME_KEY, ServerSettings.ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionSettings.Set(SERVER_SETTINGS_PASSWORD_KEY, ServerSettings.Password, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	SessionInterface->CreateSession(0, HOST_SESSION_NAME, SessionSettings);
}


void UMultiplayerGameInstance::JoinIP(FString IPAddress) {
	UEngine* Engine = GetEngine();
	if (validate(Engine != nullptr) == false) { return; }
	Engine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Joining ") + IPAddress);

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (validate(PlayerController != nullptr) == false) { return; }
	PlayerController->ClientTravel(IPAddress, ETravelType::TRAVEL_Absolute);
}


void UMultiplayerGameInstance::JoinSession(uint32 Index) {
	if (validate(SessionInterface != nullptr) == false) { return; }
	if (validate(SearchSettingsPtr.IsValid()) == false) { return; }
	if (validate(SearchSettingsPtr->SearchResults.IsValidIndex(Index)) == false) { return; }

	FOnlineSessionSearchResult Session = SearchSettingsPtr->SearchResults[Index];
	SessionInterface->JoinSession(0, JOIN_SESSION_NAME, Session);
}


void UMultiplayerGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result) {
	if (validate(SessionInterface != nullptr) == false) { return; }

	if (Result == EOnJoinSessionCompleteResult::Success) {

		FString Address;
		bool bGotAddress = SessionInterface->GetResolvedConnectString(SessionName, Address);
		if (bGotAddress == false) {
			UE_LOG(LogTemp, Error, TEXT("Could not resolve address!"));
			return;
		}
		UE_LOG(LogTemp, Warning, TEXT("Got Address: %s"), *Address);
		JoinIP(Address);
	}
	else if (Result == EOnJoinSessionCompleteResult::AlreadyInSession)
	{
		UE_LOG(LogTemp, Error, TEXT("Error Could Not Join Session: Already In Session"))
	}
	else if (Result == EOnJoinSessionCompleteResult::SessionDoesNotExist) {
		UE_LOG(LogTemp, Error, TEXT("Error Could Not Join Session: Session Does Not Exist"))
	}
	else if (Result == EOnJoinSessionCompleteResult::SessionIsFull) {
		UE_LOG(LogTemp, Error, TEXT("Error Could Not Join Session: Session Is Full"))
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Unknown Error Could Not Join Session"))
	}
}


void UMultiplayerGameInstance::FindSessions() {
	if (validate(SessionInterface != nullptr) == false) { return; }

	SearchSettingsPtr = (MakeShareable(new FOnlineSessionSearch()));

#if WITH_EDITOR
	SearchSettingsPtr->bIsLanQuery = true;
#else
	SearchSettingsPtr->bIsLanQuery = false;
#endif
	SearchSettingsPtr->TimeoutInSeconds = 3;
	SearchSettingsPtr->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	SearchSettingsPtr->MaxSearchResults = 500; // Used because sharing app id with other games.

	SessionInterface->FindSessions(0, SearchSettingsPtr.ToSharedRef());
}


void UMultiplayerGameInstance::OnFindSessionsComplete(bool bSuccess) {
	if (validate(bSuccess) == false) { return; }

	TArray<FOnlineSessionSearchResult> SessionsFound = SearchSettingsPtr->SearchResults;
	if (SessionsFound.Num() == 0) {
		UE_LOG(LogTemp, Error, TEXT("No Sessions Found!"));
		return;
	}

	if (validate(IsValid(MainMenu)) == false) { return; }
	MainMenu->ClearServerList();

	for (int i = 0; i < SessionsFound.Num(); i++) {
		auto& SessionResult = SessionsFound[i];
		FOnlineSession Session = SessionResult.Session;
		FServerInfoSettings ServerInfo;
		ServerInfo.ServerIndex = i;
		FString ServerName;
		validate(Session.SessionSettings.Get<FString>(SERVER_SETTINGS_NAME_KEY, ServerName));
		ServerInfo.Name = ServerName;
		ServerInfo.MaxPlayerCount = Session.SessionSettings.NumPublicConnections;
		ServerInfo.CurrentPlayerCount = ServerInfo.MaxPlayerCount - Session.NumOpenPublicConnections;
		ServerInfo.Owner = Session.OwningUserName;
		ServerInfo.Latency = SessionResult.PingInMs;

		MainMenu->AddServer(ServerInfo);
	}
}


void UMultiplayerGameInstance::SetMainMenu(UMainMenu* MainMenu) {
	this->MainMenu = MainMenu;
	MainMenu->SetMenuInterface(this);
}