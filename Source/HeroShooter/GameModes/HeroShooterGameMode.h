// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "HeroShooterGameMode.generated.h"

class AController;
class AHeroPlayerController;
class AHeroPlayerState;
class UDataTable;
class AEnemyAIController;

/**
 * 
 */
UCLASS()
class HEROSHOOTER_API AHeroShooterGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	
	AHeroShooterGameMode();

	TArray<AController*> GetTeamPlayerList(int TeamIndex);

	virtual void HandleDeath(AController* PlayerController, AController* Killer);

protected:

	UFUNCTION(Exec)
	void TeamWin(int TeamIndex);

	TOptional<int> WinningTeamIndex;

	UPROPERTY(EditDefaultsOnly, meta=(Tooltip="Time to wait after win condition before switching map"))
	int GameEndTimeSeconds = 10;

	UPROPERTY(EditDefaultsOnly)
	UDataTable* MapPoolDataTable;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AEnemyAIController> EnemyAIControllerTemplate;

	UPROPERTY(EditDefaultsOnly)
	bool bSpawnBots = false;

	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

	virtual void InitGameState() override;

	void OnGameOverUpdateUserInfo(const FUniqueNetId& NetId, struct FUserInfo UserInfo);

	void TravelToMapInMapPool();

	TArray<TArray<AController*>*> Teams = TArray<TArray<AController*>*>();
	
	int GetTeamIndexWithLeastPlayers();

	int GetPlayerCountInTeam(int TeamIndex);

	UFUNCTION(Exec)
	void ChangePlayerTeam(int TeamIndex, int PlayerIndex, int NewTeam);

	virtual int GetDeltaRating(AHeroPlayerState* PlayerController);

	UFUNCTION(Exec)
	void AddBot(int TeamIndex);

	UFUNCTION(Exec)
	void RemoveBot(int TeamIndex);

	int BaseDeltaRating = 20;
};
