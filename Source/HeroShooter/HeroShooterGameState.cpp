// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroShooterGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

#include "HeroSpawner.h"
#include "CustomMacros.h"


UDataTable* AHeroShooterGameState::GetHeroList() {
	return HeroListTable;
}