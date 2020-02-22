
#include "MultiplayerGameInstance.h"
#include "Engine/World.h"
#include "OnlineSessionSettings.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "GameModes/HeroShooterGameState.h"
#include "Containers/UnrealString.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"

#include "UI/MainMenu.h"
#include "UI/ServerInfo.h"
#include "CustomMacros.h"
#include "GameModes/LobbyGameMode.h"

// Doing this so I can test locally in one instance of the engine.
const static FName HOST_SESSION_NAME = TEXT("SessionGame");
const static FName JOIN_SESSION_NAME = TEXT("JoinSessionGame");

const static FName SERVER_SETTINGS_NAME_KEY = TEXT("ServerName");
const static FName SERVER_SETTINGS_PASSWORD_KEY = TEXT("ServerPassword");


const static FString USER_STATS_FILENAME = "USERSTATS";


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

	UserCloudInterface = OnlineSubsystem->GetUserCloudInterface();
	if (validate(UserCloudInterface != nullptr) == false) { return; }
	UserCloudInterface->OnReadUserFileCompleteDelegates.AddUObject(this, &UMultiplayerGameInstance::OnReadUserFileComplete);
	UserCloudInterface->OnWriteUserFileCompleteDelegates.AddUObject(this, &UMultiplayerGameInstance::OnWriteUserFileComplete);

	OnlineIdentityInterface = OnlineSubsystem->GetIdentityInterface();
	if (validate(OnlineIdentityInterface != nullptr) == false) { return; }
}



void UMultiplayerGameInstance::RequestUserInfo(const TSharedPtr<const FUniqueNetId> NetId) {
	ReadUserFile(NetId, USER_STATS_FILENAME);
}

void UMultiplayerGameInstance::UpdateUserInfo(const TSharedPtr<const FUniqueNetId> NetId, FUserInfo UserInfo) {
	FString Json = UserInfoToJSON(UserInfo);
	WriteUserFile(NetId, USER_STATS_FILENAME, Json);
}


void UMultiplayerGameInstance::UpdateLocalUserInfo(int Rating, int Wins, int Losses) {
	FUserInfo UserInfo;
	UserInfo.Rating = Rating;
	UserInfo.Wins = Wins;
	UserInfo.Losses = Losses;


	UWorld* World = GetWorld();
	if (validate(IsValid(World)) == false) { return; }

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (validate(IsValid(PlayerController)) == false) { return; }

	APlayerState* PlayerState = PlayerController->GetPlayerState<APlayerState>();
	if (validate(IsValid(PlayerState)) == false) { return; }

	UpdateUserInfo(GetUniqueID(PlayerState), UserInfo);
}

void UMultiplayerGameInstance::ReadUserFile(const TSharedPtr<const FUniqueNetId> NetId, const FString& Filename) {
	if (validate(UserCloudInterface != nullptr) == false) { return; }
	if (validate(NetId.IsValid()) == false) { return; }
	UserCloudInterface->ReadUserFile(NetId.ToSharedRef().Get(), Filename);
}


void UMultiplayerGameInstance::OnReadUserFileComplete(bool bSuccess, const FUniqueNetId& UserOwner, const FString& Filename) {
	if (validate(bSuccess) == false) {
		UE_LOG(LogTemp, Error, TEXT("Reading user file failed for %s"), *UserOwner.ToString());
		return;
	}

	if (validate(UserCloudInterface != nullptr) == false) { return; }

	TArray<uint8> Bytes = TArray<uint8>();
	bool bReadSuccess = UserCloudInterface->GetFileContents(UserOwner, Filename, Bytes);
	if (validate(bReadSuccess) == false) { return; }

	uint8* BytesPtr = Bytes.GetData();
	FString Contents = BytesToString(BytesPtr, Bytes.Num());
	
	UE_LOG(LogTemp, Warning, TEXT("Read from cloud: %s"), *Contents);

	if (Filename == USER_STATS_FILENAME) {
		FUserInfo UserInfo = JSONToUserInfo(Contents);

		UserInfo.Username = OnlineIdentityInterface->GetPlayerNickname(UserOwner);;

		OnUserInfoRequestCompleted.Broadcast(UserOwner, UserInfo);
	}
	else
	{
		validate(false);
	}
}


void UMultiplayerGameInstance::WriteUserFile(const TSharedPtr<const FUniqueNetId> NetId, const FString& Filename, FString Contents) {
	if (validate(UserCloudInterface != nullptr) == false) { return; }
	if (validate(NetId.IsValid()) == false) { return; }

	int BufferSize = Contents.Len();
	uint8* ByteArray = reinterpret_cast<uint8*>(FMemory::Malloc(BufferSize));
	int NumberOfBytesCopied = StringToBytes(Contents, ByteArray, BufferSize);

	TArray<uint8> FileContents = TArray<uint8>();
	for (int i = 0; i < BufferSize; i++) {
		FileContents.Add(ByteArray[i]);
	}

	UserCloudInterface->WriteUserFile(NetId.ToSharedRef().Get(), Filename, FileContents);
}


void UMultiplayerGameInstance::OnWriteUserFileComplete(bool bSuccess, const FUniqueNetId& UserOwner, const FString& Filename) {
	if (validate(bSuccess) == false) { 
		UE_LOG(LogTemp, Error, TEXT("Write to user file %s failed for %s"), *Filename, *UserOwner.ToString());
		return;
	}
}


void UMultiplayerGameInstance::WriteToFileTest(FString Contents) {
	UWorld* World = GetWorld();
	if (validate(IsValid(World)) == false) { return; }

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (validate(IsValid(PlayerController)) == false) { return; }

	APlayerState* PlayerState = PlayerController->GetPlayerState<APlayerState>();
	if (validate(IsValid(PlayerState)) == false) { return; }

	WriteUserFile(GetUniqueID(PlayerState), USER_STATS_FILENAME, Contents);
}


void UMultiplayerGameInstance::PrintUserFile() {
	UWorld* World = GetWorld();
	if (validate(IsValid(World)) == false) { return; }

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (validate(IsValid(PlayerController)) == false) { return; }

	APlayerState* PlayerState = PlayerController->GetPlayerState<APlayerState>();
	if (validate(IsValid(PlayerState)) == false) { return; }
	const TSharedPtr<const FUniqueNetId> NetId = GetUniqueID(PlayerState);
	if (validate(NetId != nullptr) == false) { return; }
	ReadUserFile(NetId, USER_STATS_FILENAME);
}


FString UMultiplayerGameInstance::UserInfoToJSON(FUserInfo UserInfo) {
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetNumberField("Rating", UserInfo.Rating);
	JsonObject->SetNumberField("Wins", UserInfo.Wins);
	JsonObject->SetNumberField("Losses", UserInfo.Losses);
	
	FString OutJson;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&OutJson);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);
	
	return OutJson;
}


FUserInfo UMultiplayerGameInstance::JSONToUserInfo(FString JSON) {
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JSON);
	FJsonSerializer::Deserialize(JsonReader, JsonObject);

	FUserInfo UserInfo;
	UserInfo.Rating = JsonObject->GetIntegerField("Rating");
	UserInfo.Wins = JsonObject->GetIntegerField("Wins");
	UserInfo.Losses = JsonObject->GetIntegerField("Losses");
	
	return UserInfo;
}


const TSharedPtr<const FUniqueNetId> UMultiplayerGameInstance::GetUniqueID(APlayerState* PlayerState) {
	if (validate(IsValid(PlayerState)) == false) { return nullptr; }
	return PlayerState->UniqueId.GetUniqueNetId();
}


void UMultiplayerGameInstance::PrintLocalUniqueID() {
	UWorld* World = GetWorld();
	if (validate(IsValid(World)) == false) { return; }

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (validate(IsValid(PlayerController)) == false) { return; }

	APlayerState* PlayerState = PlayerController->GetPlayerState<APlayerState>();
	if (validate(IsValid(PlayerState)) == false) { return; }
	const TSharedPtr<const FUniqueNetId> NetId = GetUniqueID(PlayerState);
	if (validate(NetId.IsValid()) == false) { return; }
	UE_LOG(LogTemp, Warning, TEXT("Local Controller Unique Id: %s"), *NetId->ToString());
}


void UMultiplayerGameInstance::OnCreateSession(FName SessionName, bool bSuccessfull) {
	if (validate(bSuccessfull) == false) { return; }

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
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (validate(PlayerController != nullptr) == false) { return; }

	PlayerController->ClientTravel(IPAddress, ETravelType::TRAVEL_Absolute);
}


void UMultiplayerGameInstance::JoinSession(uint32 Index) {
	if (validate(SessionInterface != nullptr) == false) { return; }
	if (validate(SearchSettings.IsValid()) == false) { return; }
	if (validate(SearchSettings->SearchResults.IsValidIndex(Index)) == false) { return; }

	FOnlineSessionSearchResult Session = SearchSettings->SearchResults[Index];
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

	SearchSettings = (MakeShareable(new FOnlineSessionSearch()));

#if WITH_EDITOR
	SearchSettings->bIsLanQuery = true;
#else
	SearchSettingsPtr->bIsLanQuery = false;
#endif
	SearchSettings->TimeoutInSeconds = 3;
	SearchSettings->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	SearchSettings->MaxSearchResults = 500; // Used because sharing app id with other games.

	SessionInterface->FindSessions(0, SearchSettings.ToSharedRef());
}


void UMultiplayerGameInstance::OnFindSessionsComplete(bool bSuccess) {
	if (validate(bSuccess) == false) { return; }

	TArray<FOnlineSessionSearchResult> SessionsFound = SearchSettings->SearchResults;
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