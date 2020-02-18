// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModes/HeroShooterGameState.h"
#include "DeathmatchGameState.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnKillCountUpdated);

/**
 * 
 */
UCLASS()
class HEROSHOOTER_API ADeathmatchGameState : public AHeroShooterGameState
{
	GENERATED_BODY()
	
public:


	void AddKill(int Team);

	int GetKillCount(int Team);

	FOnKillCountUpdated OnKillCountUpdated;

	int GetVictoryScore();

protected:

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	virtual void BeginPlay() override;


	UPROPERTY(EditDefaultsOnly)
	int KillsForVictory = 1;

	UPROPERTY(ReplicatedUsing=Rep_KillsUpdated)
	TArray<int> Kills;

	UFUNCTION()
	void Rep_KillsUpdated();
};
