// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroPlayerState.h"
#include "Engine/World.h"
#include "GameFramework/GameSession.h"
#include "HeroPlayerController.h"
#include "CustomMacros.h"
#include "UnrealNetwork.h"
#include "GameFramework/GameStateBase.h"
#include "UI/IngameHUD.h"
#include "UI/StatisticsTab.h"

void AHeroPlayerState::AddKill() {
	KillCount += 1;
}

void AHeroPlayerState::AddDeath() {
	DeathCount += 1;
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

	if (bIsABot == false) {
		UWorld* World = GetWorld();
		if (validate(IsValid(World)) == false) { return; }

		AHeroPlayerController* HeroPlayerController = Cast<AHeroPlayerController>(
			GetPlayerControllerFromNetId(World, UniqueId.GetUniqueNetId().ToSharedRef().Get())
		);

		if (IsValid(HeroPlayerController) && HeroPlayerController->IsLocalController()) {
			HeroPlayerController->SetTeamIndex(NewTeamIndex);
		}

		OnTeamChange.Broadcast(this);
	}
}


int AHeroPlayerState::GetTeamIndex() {
	return TeamIndex;
}


void AHeroPlayerState::OnRep_TeamIndex() {
	SetTeamIndex(TeamIndex);
}


void AHeroPlayerState::OnRep_Stats() {

}


void AHeroPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHeroPlayerState, KillCount);
	DOREPLIFETIME(AHeroPlayerState, DeathCount);
	DOREPLIFETIME(AHeroPlayerState, TeamIndex);
}