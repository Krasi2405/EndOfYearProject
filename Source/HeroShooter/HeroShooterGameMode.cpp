// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroShooterGameMode.h"
#include "Engine/World.h"

#include "HeroShooterGameState.h"
#include "HeroPlayerController.h"
#include "HeroSpawner.h"
#include "CustomMacros.h"

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


void AHeroShooterGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) {
	Super::InitGame(MapName, Options, ErrorMessage);

	if (validate(TeamCount > 0) == false) { 
		ErrorMessage = "No team count in gamemode!";
		return;
	}


	for (int i = 0; i < TeamCount; i++) {
		Teams.Add(new TArray<AHeroPlayerController*>());
	}
}

void AHeroShooterGameMode::InitGameState() {
	Super::InitGameState();

	UE_LOG(LogTemp, Warning, TEXT("InitGameState"))
	AHeroShooterGameState* GameState = GetGameState<AHeroShooterGameState>();
	if (validate(IsValid(GameState)) == false) {
		UE_LOG(LogTemp, Error, TEXT("GameState should inherit from AHeroShooterGameState!"))
		return; 
	}
}



int AHeroShooterGameMode::GetTeamIndexWithLeastPlayers() {
	if (validate(TeamCount > 0) == false) { return -1; }
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

	if (validate(Teams[LeastTeamIndex]->Num() < MaxPlayersInTeam) == false) { return -1; }

	return LeastTeamIndex;
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

	if (validate(NewTeam->Num() < MaxPlayersInTeam) == false) { return; }

	NewTeam->Add(PlayerController);
	PlayerController->SetTeamIndex(NewTeamIndex);
	Team->RemoveAt(PlayerIndex);

	UE_LOG(LogTemp, Log, TEXT("Successfully changed teams!"));
}