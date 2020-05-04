// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroShooterGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

#include "HeroPlayerState.h"
#include "HeroSpawner.h"
#include "CustomMacros.h"


UDataTable* AHeroShooterGameState::GetHeroList() {
	return HeroListTable;
}


void AHeroShooterGameState::NetMulticastSendWinningTeam_Implementation(int TeamIndex) {
	OnWinConditionSent.Broadcast(TeamIndex);
}


TSubclassOf<UGameModeInfoWidget> AHeroShooterGameState::GetInfoWidgetTemplate() {
	return InfoWidgetTemplate;
}


int AHeroShooterGameState::GetTeamCount() {
	return TeamCount;
}

int AHeroShooterGameState::GetMaxPlayersInTeam() {
	return MaxPlayersInTeam;
}


void AHeroShooterGameState::AddPlayerState(APlayerState* PlayerState) {
	Super::AddPlayerState(PlayerState);

	AHeroPlayerState* HeroPlayerState = Cast<AHeroPlayerState>(PlayerState);
	if (validate(IsValid(HeroPlayerState)) == false) { return; }
	OnAddPlayer.Broadcast(HeroPlayerState);
}

void AHeroShooterGameState::RemovePlayerState(APlayerState* PlayerState) {
	Super::RemovePlayerState(PlayerState);

	AHeroPlayerState* HeroPlayerState = Cast<AHeroPlayerState>(PlayerState);
	if (validate(IsValid(HeroPlayerState)) == false) { return; }
	OnRemovePlayer.Broadcast(HeroPlayerState);
}