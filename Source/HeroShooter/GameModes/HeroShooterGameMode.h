// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "HeroShooterGameMode.generated.h"

class AHeroPlayerController;
class UDataTable;

/**
 * 
 */
UCLASS()
class HEROSHOOTER_API AHeroShooterGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	AHeroShooterGameMode();

	TArray<AHeroPlayerController*> GetTeamPlayerList(int TeamIndex);

	virtual void HandleDeath(AHeroPlayerController* PlayerController);

protected:

	UPROPERTY(EditDefaultsOnly)
	int TeamCount = 3;

	UPROPERTY(EditDefaultsOnly)
	int MaxPlayersInTeam = 2;

	UFUNCTION(Exec)
	void TeamWin(int TeamIndex);

	TOptional<int> WinningTeamIndex;

	UPROPERTY(EditDefaultsOnly, meta=(Tooltip="Time to wait after win condition before switching map"))
	int GameEndTimeSeconds = 10;

	UPROPERTY(EditDefaultsOnly)
	UDataTable* MapPoolDataTable;

	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

	virtual void InitGameState() override;

	void OnGameOverUpdateUserInfo(const FUniqueNetId& NetId, struct FUserInfo UserInfo);

	void TravelToMapInMapPool();

	TArray<TArray<AHeroPlayerController*>*> Teams = TArray<TArray<AHeroPlayerController*>*>();
	
	int GetTeamIndexWithLeastPlayers();

	UFUNCTION(Exec)
	void ChangePlayerTeam(int TeamIndex, int PlayerIndex, int NewTeam);

	virtual int GetDeltaRating(AHeroPlayerController* PlayerController);

	int BaseDeltaRating = 20;
};
