// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "HeroController.h"
#include "TeamInterface.h"
#include "EnemyAIController.generated.h"

class AEnemyCharacter;
class ANexus;
class ACoverPoint;
class UCrowdFollowingComponent;
class UDataTable;
class AHeroSpawner;
class AHeroPlayerState;
/**
 * 
 */


UCLASS()
class HEROSHOOTER_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	

public:
	AEnemyAIController();


protected:

	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnTeamChange(AHeroPlayerState* HeroPlayerState);

	void SetupAI();

	virtual void OnPossess(class APawn* Pawn) override;

	virtual void Destroyed() override;

	UFUNCTION()
	void ServerHandleDeath();

	UPROPERTY(EditDefaultsOnly)
	UDataTable* HeroListTable;

	UPROPERTY()
	AHeroSpawner* HeroSpawner;


	UPROPERTY(EditDefaultsOnly)
	int RespawnDelay = 5;

	FTimerHandle RespawnTimerHandle;

	UFUNCTION()
	void Respawn();

	void SpawnPawn();

};