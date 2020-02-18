// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathmatchGameState.h"
#include "Engine/World.h"
#include "UnrealNetwork.h"

#include "GameModes/Deathmatch/DeathmatchGameMode.h"
#include "CustomMacros.h"

void ADeathmatchGameState::BeginPlay() {
	Super::BeginPlay();

	Kills.AddZeroed(TeamCount);
}


void ADeathmatchGameState::AddKill(int Team) {
	if (validate(Kills.Num() > Team) == false) { return; }
	Kills[Team] += 1;

	UE_LOG(LogTemp, Warning, TEXT("AddKill"))
	OnKillCountUpdated.Broadcast();

	UE_LOG(LogTemp, Warning, TEXT("Team %d now has %d kills!"), Team, Kills[Team]);
}


int ADeathmatchGameState::GetKillCount(int Team) {
	if (validate(Kills.Num() > Team) == false) { return -1; }
	return Kills[Team];
}


int ADeathmatchGameState::GetVictoryScore() {
	return KillsForVictory;
}

void ADeathmatchGameState::Rep_KillsUpdated() {

	UE_LOG(LogTemp, Warning, TEXT("Rep_KillsUpdated"))
	OnKillCountUpdated.Broadcast();
}

void ADeathmatchGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADeathmatchGameState, Kills);
}


