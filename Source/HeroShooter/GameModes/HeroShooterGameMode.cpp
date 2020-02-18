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


AHeroShooterGameMode::AHeroShooterGameMode() {
	bUseSeamlessTravel = true;
	bStartPlayersAsSpectators = true;
}



void AHeroShooterGameMode::PostLogin(APlayerController* NewPlayer) {
	Super::PostLogin(NewPlayer);

	UE_LOG(LogTemp, Warning, TEXT("PostLogin"));
	AHeroPlayerController* HeroController = Cast<AHeroPlayerController>(NewPlayer);
	if (validate(IsValid(HeroController)) == false) { return; }

	int TeamIndex = GetTeamIndexWithLeastPlayers();
	if (validate(TeamIndex != -1) == false) { return; }
	TArray<AHeroPlayerController*>* Team = Teams[TeamIndex];
	if (validate(Team != nullptr) == false) { return; }

	Teams[TeamIndex]->Add(HeroController);
	HeroController->SetTeamIndex(TeamIndex);

	UE_LOG(LogTemp, Warning, TEXT("Assign %s to %d Team"), *HeroController->GetName(), TeamIndex);
}


void AHeroShooterGameMode::Logout(AController* Exiting) {
	Super::Logout(Exiting);

	AHeroPlayerController* PlayerController = Cast<AHeroPlayerController>(Exiting);
	if (validate(IsValid(PlayerController)) == false) { return; }

	int TeamIndex = PlayerController->GetTeamIndex();
	if (validate(TeamIndex != -1) == false) { return; }

	TArray<AHeroPlayerController*>* Team = Teams[TeamIndex];
	if (validate(Team != nullptr) == false) { return; }

	Team->Remove(PlayerController);
}

void AHeroShooterGameMode::BeginPlay() {
	Super::BeginPlay();

	
}

void AHeroShooterGameMode::InitGameState() {
	Super::InitGameState();

	AHeroShooterGameState* GameState = GetGameState<AHeroShooterGameState>();
	if (validate(IsValid(GameState)) == false) {
		UE_LOG(LogTemp, Error, TEXT("GameState should inherit from AHeroShooterGameState!"))
		return; 
	}

	validate(GameState->GetTeamCount() > 0);
	for (int i = 0; i < GameState->GetTeamCount(); i++) {
		Teams.Add(new TArray<AHeroPlayerController*>());
	}
}



int AHeroShooterGameMode::GetTeamIndexWithLeastPlayers() {
	AHeroShooterGameState* GameState = GetGameState<AHeroShooterGameState>();
	if (validate(IsValid(GameState)) == false) { return -1; }

	if (validate(GameState->GetTeamCount() > 0) == false) { return -1; }
	if (validate(Teams[0] != nullptr) == false) { return -1; }

	int LeastTeamIndex = 0;
	int LeastTeamCount = Teams[0]->Num();

	for (int i = 1; i < Teams.Num(); i++) {
		TArray<AHeroPlayerController*>* Team = Teams[i];
		if (validate(Team != nullptr) == false) { return -1; }

		int PlayerCount = Team->Num();

		if (LeastTeamCount > PlayerCount) {
			LeastTeamIndex = i;
			LeastTeamCount = PlayerCount;
		}
	}

	if (validate(Teams[LeastTeamIndex]->Num() < GameState->GetTeamCount()) == false) { return -1; }

	return LeastTeamIndex;
}


void AHeroShooterGameMode::TeamWin(int TeamIndex) {
	AHeroShooterGameState* GameState = GetGameState<AHeroShooterGameState>();
	if (validate(IsValid(GameState)) == false) { return; }
	GameState->NetMulticastSendWinningTeam(TeamIndex);

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
	TArray<AHeroPlayerController*>* Team = Teams[TeamIndex];

	if (validate(Team->Num() > PlayerIndex) == false) { return; }
	AHeroPlayerController* PlayerController = (*Team)[PlayerIndex];

	if (validate(IsValid(PlayerController)) == false) { return; }
	if (validate(Teams.Num() > NewTeamIndex) == false) { return; }
	TArray<AHeroPlayerController*>* NewTeam = Teams[NewTeamIndex];


	AHeroShooterGameState* GameState = GetGameState<AHeroShooterGameState>();
	if (validate(IsValid(GameState)) == false) { return; }
	if (validate(NewTeam->Num() < GameState->GetTeamCount()) == false) { return; }

	NewTeam->Add(PlayerController);
	PlayerController->SetTeamIndex(NewTeamIndex);
	Team->RemoveAt(PlayerIndex);

	UE_LOG(LogTemp, Log, TEXT("Successfully changed teams!"));
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


void AHeroShooterGameMode::HandleDeath(AHeroPlayerController* PlayerController) {
	if (validate(IsValid(PlayerController)) == false) { return; }
	AHeroPlayerState* PlayerState = PlayerController->GetPlayerState<AHeroPlayerState>();
	if (validate(IsValid(PlayerState)) == false) { return; }
	PlayerState->AddDeath();
}