// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "HeroPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class HEROSHOOTER_API AHeroPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	void AddKill();

	void AddDeath();

	int GetKillCount();

	int GetDeathCount();

protected:

	UPROPERTY(Replicated)
	int KillCount = 0;

	UPROPERTY(Replicated)
	int DeathCount = 0;


	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
};
