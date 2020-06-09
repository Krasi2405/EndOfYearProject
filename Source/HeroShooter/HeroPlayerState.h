// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "HeroPlayerState.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnStatsChange, int, Kills, int, Deaths, int, Assists);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUsernameChange, FString, Username);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPortraitChange, UTexture2D*, PortraitTexture);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTeamChange, AHeroPlayerState*, HeroPlayerState);

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

	UFUNCTION(BlueprintCallable)
	int GetTeamIndex();

	UFUNCTION(NetMulticast, Reliable)
	void SetPortrait(int TextureIndex);

	void SetPortrait_Implementation(int TextureIndex);

	FOnStatsChange OnStatsChange;

	FOnUsernameChange OnUsernameChange;

	FOnPortraitChange OnPortraitChange;

	FOnTeamChange OnTeamChange;

protected:

	UPROPERTY(ReplicatedUsing = OnRep_Stats)
	int KillCount = 0;

	UPROPERTY(ReplicatedUsing = OnRep_Stats)
	int DeathCount = 0;

	UPROPERTY(ReplicatedUsing = OnRep_TeamIndex, VisibleAnywhere)
	int TeamIndex = -1;

	UFUNCTION()
	void OnRep_TeamIndex();

	UFUNCTION()
	void OnRep_Stats();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

};
