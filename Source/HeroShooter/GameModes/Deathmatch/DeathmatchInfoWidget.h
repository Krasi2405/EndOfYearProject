// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModes/GameModeInfoWidget.h"
#include "DeathmatchInfoWidget.generated.h"

class UDeathmatchTeamScore;
class ADeathmatchGameState;

/**
 * 
 */
UCLASS()
class HEROSHOOTER_API UDeathmatchInfoWidget : public UGameModeInfoWidget
{
	GENERATED_BODY()
	
public:

	void Setup(AHeroShooterGameState* GameState) override;

protected:

	void UpdateScore(int TeamIndex, int Score);

	UFUNCTION()
	void UpdateKillCount();

	ADeathmatchGameState* DeathmatchGameState;

	UPROPERTY(meta = (BindWidget))
	UDeathmatchTeamScore* TeamScoreOne;

	UPROPERTY(meta = (BindWidget))
	UDeathmatchTeamScore* TeamScoreTwo;
};
