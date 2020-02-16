// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "HeroShooterGameState.generated.h"


class UDataTable;
class AHeroSpawner;

/**
 * 
 */
UCLASS()
class HEROSHOOTER_API AHeroShooterGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	UDataTable* GetHeroList();

protected:

	UPROPERTY(EditDefaultsOnly)
	UDataTable* HeroListTable = nullptr;

};