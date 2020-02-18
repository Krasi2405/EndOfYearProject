// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Blueprint/UserWidget.h"
#include "InfoWidgetInterface.generated.h"

class AHeroShooterGameState;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInfoWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HEROSHOOTER_API IInfoWidgetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void Setup(AHeroShooterGameState* GameState) = 0;
};
