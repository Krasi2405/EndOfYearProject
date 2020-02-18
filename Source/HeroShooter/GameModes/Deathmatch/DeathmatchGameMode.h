// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModes/HeroShooterGameMode.h"
#include "DeathmatchGameMode.generated.h"

/**
 * 
 */
UCLASS()
class HEROSHOOTER_API ADeathmatchGameMode : public AHeroShooterGameMode
{
	GENERATED_BODY()
	
public:

	virtual void HandleDeath(AHeroPlayerController* PlayerController) override;

};
