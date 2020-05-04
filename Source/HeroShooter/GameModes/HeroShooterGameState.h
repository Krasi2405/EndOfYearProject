// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "HeroShooterGameState.generated.h"

class AHeroPlayerState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAddPlayer, AHeroPlayerState*, Player);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRemovePlayer, AHeroPlayerState*, Player);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWinConditionSent, int, TeamIndex);

class UDataTable;
class AHeroSpawner;
class UGameModeInfoWidget;

/**
 * 
 */
UCLASS()
class HEROSHOOTER_API AHeroShooterGameState : public AGameState
{
	GENERATED_BODY()

public:

	UDataTable* GetHeroList();

	FOnWinConditionSent OnWinConditionSent;

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastSendWinningTeam(int TeamIndex);

	void NetMulticastSendWinningTeam_Implementation(int TeamIndex);

	TSubclassOf<UGameModeInfoWidget> GetInfoWidgetTemplate();

	int GetTeamCount();

	int GetMaxPlayersInTeam();

	FOnAddPlayer OnAddPlayer;

	FOnRemovePlayer OnRemovePlayer;

protected:

	virtual void AddPlayerState(APlayerState* PlayerState) override;

	virtual void RemovePlayerState(APlayerState* PlayerState) override;

	UPROPERTY(EditDefaultsOnly)
	int TeamCount = 2;

	UPROPERTY(EditDefaultsOnly)
	int MaxPlayersInTeam = 3;

	UPROPERTY(EditDefaultsOnly)
	UDataTable* HeroListTable = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameModeInfoWidget> InfoWidgetTemplate;

};