// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroShooterGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

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