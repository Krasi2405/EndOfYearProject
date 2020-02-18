// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroPlayerState.h"
#include "UnrealNetwork.h"

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


void AHeroPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHeroPlayerState, KillCount);

	DOREPLIFETIME(AHeroPlayerState, DeathCount);
}