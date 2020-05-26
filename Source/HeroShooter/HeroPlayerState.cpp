// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroPlayerState.h"
#include "Engine/World.h"
#include "GameFramework/GameSession.h"
#include "HeroPlayerController.h"
#include "CustomMacros.h"
#include "UnrealNetwork.h"
#include "GameFramework/GameStateBase.h"
#include "GameModes/HeroShooterGameState.h"
#include "HeroSpawner.h"
#include "UI/IngameHUD.h"
#include "UI/StatisticsTab.h"
#include "HeroTableRow.h"

void AHeroPlayerState::AddKill() {
	KillCount += 1;
	OnStatsChange.Broadcast(KillCount, DeathCount, 0);
}

void AHeroPlayerState::AddDeath() {
	DeathCount += 1;
	OnStatsChange.Broadcast(KillCount, DeathCount, 0);
}

int AHeroPlayerState::GetKillCount() {
	return KillCount;
}

int AHeroPlayerState::GetDeathCount() {
	return DeathCount;
}


void AHeroPlayerState::SetTeamIndex(int NewTeamIndex) {
	if (TeamIndex == NewTeamIndex) { return; }

	TeamIndex = NewTeamIndex;
	OnTeamChange.Broadcast(this);
}


int AHeroPlayerState::GetTeamIndex() {
	return TeamIndex;
}


void AHeroPlayerState::OnRep_TeamIndex() {
	SetTeamIndex(TeamIndex);
}


void AHeroPlayerState::OnRep_Stats() {
	OnStatsChange.Broadcast(KillCount, DeathCount, 0);
}


void AHeroPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHeroPlayerState, KillCount);
	DOREPLIFETIME(AHeroPlayerState, DeathCount);
	DOREPLIFETIME(AHeroPlayerState, TeamIndex);
}



void AHeroPlayerState::SetPortrait_Implementation(int TextureIndex) {
	UWorld* World = GetWorld();
	if (validate(IsValid(World)) == false) { return; }
	
	AHeroShooterGameState* GameState = Cast<AHeroShooterGameState>(World->GetGameState());
	if (validate(IsValid(GameState)) == false) { return; }
	
	UDataTable* HeroListTable = GameState->GetHeroList();
	if (validate(IsValid(HeroListTable)) == false) { return; }

	TArray<FHeroListTableRow*> Heroes;
	HeroListTable->GetAllRows<FHeroListTableRow>(TEXT("GENERAL"), Heroes);

	if (validate(Heroes.IsValidIndex(TextureIndex)) == false) { return; }
	FHeroListTableRow* HeroTableRow = Heroes[TextureIndex];

	OnPortraitChange.Broadcast(HeroTableRow->Texture);
}