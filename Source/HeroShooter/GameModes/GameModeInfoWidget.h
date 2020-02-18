// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameModes/InfoWidgetInterface.h"
#include "GameModeInfoWidget.generated.h"

class AHeroShooterGameState;

/**
 * 
 */
UCLASS()
class HEROSHOOTER_API UGameModeInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void Setup(AHeroShooterGameState* GameState);

};
