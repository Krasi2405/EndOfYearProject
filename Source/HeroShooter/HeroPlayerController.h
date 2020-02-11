// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HeroPlayerController.generated.h"

class UIngameMenu;

/**
 * 
 */
UCLASS()
class HEROSHOOTER_API AHeroPlayerController : public APlayerController
{
	GENERATED_BODY()

	AHeroPlayerController();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UIngameMenu> IngameMenuClass;

	UPROPERTY()
	UIngameMenu* IngameMenu;

	virtual void SetupInputComponent() override;

	void SwitchIngameMenu();

	void DeactivateIngameMenu();

	void ActivateIngameMenu();

	bool bIngameMenuActive = false;
	
};
