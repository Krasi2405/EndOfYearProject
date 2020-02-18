// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModes/GameModeInfoWidget.h"
#include "EscortInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class HEROSHOOTER_API UEscortInfoWidget : public UGameModeInfoWidget
{
	GENERATED_BODY()


public:
	void Setup(AHeroShooterGameState* GameState) override;

};
