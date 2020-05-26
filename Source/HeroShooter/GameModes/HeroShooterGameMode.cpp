// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroShooterGameMode.h"
#include "Engine/World.h"
#include "Engine/DataTable.h"	
#include "TimerManager.h"

#include "MapNameRow.h"
#include "HeroShooterGameState.h"
#include "HeroPlayerController.h"
#include "HeroSpawner.h"
#include "CustomMacros.h"
#include "HeroPlayerState.h"
#include "MultiplayerGameInstance.h"
#include "GameFramework/GameSession.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "AI/Controllers/EnemyAIController.h"


AHeroShooterGameMode::AHeroShooterGameMode() {
	bUseSeamlessTravel = true;
	bStartPlayersAsSpectators = true;
}



void AHeroShooterGameMode::PostLogin(APlayerController* NewPlayer) {
	Super::PostLogin(NewPlayer);

	AHeroPlayerController* HeroController = Cast<AHeroPlayerController>(NewPlayer);
	if (validate(IsValid(HeroController)) == false) { return; }

	AHeroPlayerState* HeroPlayerState = HeroController->GetPlayerState<AHeroPlayerState>();
	if (validate(IsValid(HeroPlayerState)) == false) { return; }

	int TeamIndex = GetTeamIndexWithLeastPlayers();
	if (validate(TeamIndex != -1) == false) { return; }
	if (validate(Teams.Num() > TeamIndex) == false) { return; }

	TArray<AController*>* Team = Teams[TeamIndex];
	if (validate(Team != nullptr) == false) { return; }

	UE_LOG(LogTemp, Warning, TEXT("Call Remove Bot With Team Index %d"), TeamIndex);
	RemoveBot(TeamIndex);
	Teams[TeamIndex]->Add(HeroController);
	HeroPlayerState->SetTeamIndex(TeamIndex);

}


void AHeroShooterGameMode::Logout(AController* Exiting) {
	Super::Logout(Exiting);

	AHeroPlayerState* PlayerState = Exiting->GetPlayerState<AHeroPlayerState>();
	if (validate(IsValid(PlayerState)) == false) { return; }

	int TeamIndex = PlayerState->GetTeamIndex();
	if (validate(TeamIndex != -1) == false) { return; }

	TArray<AController*>* Team = Teams[TeamIndex];
	if (validate(Team != nullptr) == false) { return; }

	Team->Remove(Exiting);

	if (PlayerState->bIsABot == false) {
		AddBot(TeamIndex);
	}
}


void AHeroShooterGameMode::BeginPlay() {
	Super::BeginPlay();


	UMultiplayerGameInstance* GameInstance = GetGameInstance<UMultiplayerGameInstance>();
	if (validate(IsValid(GameInstance)) == false) { return; }
	GameInstance->OnUserInfoRequestCompleted.AddUObject(this, &AHeroShooterGameMode::OnGameOverUpdateUserInfo);
}


void AHeroShooterGameMode::OnGameOverUpdateUserInfo(const FUniqueNetId& NetId, FUserInfo UserInfo) {
	UWorld* World = GetWorld();
	if (validate(IsValid(World)) == false) { return; }

	AHeroPlayerController* PlayerController = Cast<AHeroPlayerController>(GetPlayerControllerFromNetId(World, NetId));
	if (validate(IsValid(PlayerController)) == false) { return; }
	if (validate(WinningTeamIndex.IsSet()) == false) { return; }

	AHeroPlayerState* PlayerState = PlayerController->GetPlayerState<AHeroPlayerState>();
	if (validate(IsValid(PlayerState)) == false) { return; }

	PlayerState->GetTeamIndex() == WinningTeamIndex.GetValue() ? UserInfo.Wins++ : UserInfo.Losses++;
	UserInfo.Rating += GetDeltaRating(PlayerState);


	UMultiplayerGameInstance* GameInstance = GetGameInstance<UMultiplayerGameInstance>();
	if (validate(IsValid(GameInstance)) == false) { return; }

	GameInstance->UpdateUserInfo(
		PlayerController->GetPlayerState<APlayerState>()->UniqueId.GetUniqueNetId(), UserInfo);
}


int AHeroShooterGameMode::GetDeltaRating(AHeroPlayerState* PlayerController) {
	// TODO: calculate based on enemy and player rating.
	if (validate(WinningTeamIndex.IsSet()) == false) { return 0; }

	return PlayerController->GetTeamIndex() == WinningTeamIndex.GetValue() ? BaseDeltaRating : -BaseDeltaRating;
}


void AHeroShooterGameMode::InitGameState() {
	Super::InitGameState();

	AHeroShooterGameState* GameState = GetGameState<AHeroShooterGameState>();
	if (validate(IsValid(GameState)) == false) { return; }

	validate(GameState->GetTeamCount() > 0);
	if (validate(IsValid(EnemyAIControllerTemplate)) == false) { return; }

	for (int i = 0; i < GameState->GetTeamCount(); i++) {
		TArray<AController*>* Team = new TArray<AController*>();
		int MaxPlayers = GameState->GetMaxPlayersInTeam();
		Teams.Add(Team);
		for (int j = GetPlayerCountInTeam(i); j < MaxPlayers; j++) {
			AddBot(i);
		}
	}
}



int AHeroShooterGameMode::GetTeamIndexWithLeastPlayers() {
	AHeroShooterGameState* GameState = GetGameState<AHeroShooterGameState>();
	if (validate(IsValid(GameState)) == false) { return -1; }

	if (validate(GameState->GetTeamCount() > 0) == false) { return -1; }
	if (validate(Teams.Num() >= 0) == false) { return -1; }
	if (validate(Teams[0] != nullptr) == false) { return -1; }

	int LeastTeamIndex = 0;
	int LeastTeamCount = GetPlayerCountInTeam(0);

	for (int i = 1; i < Teams.Num(); i++) {
		TArray<AController*>* Team = Teams[i];
		if (validate(Team != nullptr) == false) { return -1; }

		int PlayerCount = GetPlayerCountInTeam(i);
		if (LeastTeamCount > PlayerCount) {
			LeastTeamIndex = i;
			LeastTeamCount = PlayerCount;
		}
	}

	if (validate(GetPlayerCountInTeam(LeastTeamIndex) < GameState->GetMaxPlayersInTeam()) == false) { return -1; }

	return LeastTeamIndex;
}


int AHeroShooterGameMode::GetPlayerCountInTeam(int TeamIndex) {
	if (validate(TeamIndex < Teams.Num()) == false) { return -1; }

	TArray<AController*>* Team = Teams[TeamIndex];
	if (validate(Team != nullptr) == false) { return -1; }
	int PlayerCount = 0;
	for (int i = 0; i < Team->Num(); i++) {
		APlayerController* PlayerController = Cast<APlayerController>(*(Team->GetData() + i));
		if (IsValid(PlayerController)) {
			PlayerCount++;
		}
	}

	return PlayerCount;
}


void AHeroShooterGameMode::TeamWin(int TeamIndex) {
	WinningTeamIndex = TeamIndex;

	AHeroShooterGameState* GameState = GetGameState<AHeroShooterGameState>();
	if (validate(IsValid(GameState)) == false) { return; }
	GameState->NetMulticastSendWinningTeam(TeamIndex);

	UMultiplayerGameInstance* GameInstance = GetGameInstance<UMultiplayerGameInstance>();
	if (validate(IsValid(GameInstance)) == false) { return; }

	for (APlayerState* PlayerState : GameState->PlayerArray) {
		TSharedPtr<const FUniqueNetId> NetId = GameInstance->GetUniqueID(PlayerState);
		GameInstance->RequestUserInfo(NetId);
	}

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(
		TimerHandle, 
		this, 
		&AHeroShooterGameMode::TravelToMapInMapPool,
		GameEndTimeSeconds,
		false
	);
}


void AHeroShooterGameMode::ChangePlayerTeam(int TeamIndex, int PlayerIndex, int NewTeamIndex) {
	if (validate(TeamIndex != NewTeamIndex) == false) { return; }

	if (validate(Teams.Num() > TeamIndex) == false) { return; }
	TArray<AController*>* Team = Teams[TeamIndex];

	if (validate(Team->Num() > PlayerIndex) == false) { return; }
	AController* PlayerController = (*Team)[PlayerIndex];

	if (validate(IsValid(PlayerController)) == false) { return; }
	if (validate(Teams.Num() > NewTeamIndex) == false) { return; }
	TArray<AController*>* NewTeam = Teams[NewTeamIndex];

	AHeroShooterGameState* GameState = GetGameState<AHeroShooterGameState>();
	if (validate(IsValid(GameState)) == false) { return; }
	if (validate(NewTeam->Num() < GameState->GetMaxPlayersInTeam()) == false) { return; }

	NewTeam->Add(PlayerController);
	Team->RemoveAt(PlayerIndex);

	AHeroPlayerState* PlayerState = PlayerController->GetPlayerState<AHeroPlayerState>();
	if (validate(IsValid(PlayerState)) == false) { return; }
	PlayerState->SetTeamIndex(NewTeamIndex);
}


void AHeroShooterGameMode::TravelToMapInMapPool() {
	if (validate(IsValid(MapPoolDataTable)) == false) { return; }
	TArray<FMapNameRow*> Maps;
	MapPoolDataTable->GetAllRows<FMapNameRow>(TEXT("GENERAL"), Maps);
	int MapCount = Maps.Num();
	if (validate(MapCount > 0) == false) { return; }
	FMapNameRow* Map = Maps[FMath::RandRange(0, MapCount)];
	if (validate(Map != nullptr) == false) { return; }
	FString MapName = Map->MapName;

	UWorld* World = GetWorld();
	if (validate(IsValid(World)) == false) { return; }
	UE_LOG(LogTemp, Warning, TEXT("Server Travelling to %s"), *MapName);
	World->ServerTravel(MapName + "?listen", true);
}


void AHeroShooterGameMode::HandleDeath(AController* PlayerController, AController* Killer) {
	if (validate(IsValid(PlayerController)) == false) { return; }
	AHeroPlayerState* PlayerState = PlayerController->GetPlayerState<AHeroPlayerState>();
	if (validate(IsValid(PlayerState)) == false) { return; }
	PlayerState->AddDeath();

	if (IsValid(Killer)) {
		AHeroPlayerState* KillerPlayerState = Killer->GetPlayerState<AHeroPlayerState>();
		if (validate(IsValid(KillerPlayerState)) == false) { return; }
		KillerPlayerState->AddKill();
	}
}



void AHeroShooterGameMode::AddBot(int TeamIndex) {
	if (bSpawnBots == false) { return; }

	if (validate(Teams.IsValidIndex(TeamIndex)) == false) { return; }
	TArray<AController*>* Team = Teams[TeamIndex];
	if (validate(Team != nullptr) == false) { return; }

	UWorld* World = GetWorld();
	if (validate(IsValid(World)) == false) { return; }
	AHeroShooterGameState* GameState = Cast<AHeroShooterGameState>(World->GetGameState());
	if (validate(IsValid(GameState)) == false) { return; }
	if (validate(Team->Num() < GameState->GetMaxPlayersInTeam()) == false) { return; }
	UE_LOG(LogTemp, Warning, TEXT("Add Bot to team %d with %d players"), TeamIndex, Team->Num());

	AEnemyAIController* EnemyAI = World->SpawnActor<AEnemyAIController>(EnemyAIControllerTemplate);
	if (validate(IsValid(EnemyAI)) == false) { return; }
	AHeroPlayerState* AIPlayerState = EnemyAI->GetPlayerState<AHeroPlayerState>();
	if (validate(IsValid(AIPlayerState)) == false) { return; }
	AIPlayerState->SetTeamIndex(TeamIndex);

	Team->Add(EnemyAI);
}

void AHeroShooterGameMode::RemoveBot(int TeamIndex) {
	if (bSpawnBots == false) { return; }

	if (validate(Teams.IsValidIndex(TeamIndex)) == false) { return; }

	TArray<AController*>* Team = Teams[TeamIndex];
	for (int i = 0; i < Team->Num(); i++) {
		AAIController* AIController = Cast<AAIController>(*(Team->GetData() + i));
		if (IsValid(AIController)) {
			Team->Remove(AIController);
			AIController->Destroy();
			return;
		}
	}
	validate(false);
}