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

	void SetTeamIndex(int NewTeamIndex);

	int GetTeamIndex();

protected:

	UPROPERTY(Replicated)
	int KillCount = 0;

	UPROPERTY(Replicated)
	int DeathCount = 0;

	UPROPERTY(ReplicatedUsing = OnRep_TeamIndex, VisibleAnywhere)
	int TeamIndex = -1;

	UFUNCTION()
	void OnRep_TeamIndex();


	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
};
