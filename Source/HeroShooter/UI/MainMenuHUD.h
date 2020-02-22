// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainMenuHUD.generated.h"

class UMainMenu;
class UUserProfileInfo;
/**
 * 
 */
UCLASS()
class HEROSHOOTER_API AMainMenuHUD : public AHUD
{
	GENERATED_BODY()
	

protected:
	
	void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMainMenu> MainMenuTemplate;

	UPROPERTY()
	UMainMenu* MainMenu;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserProfileInfo> UserProfileTemplate;

	UPROPERTY()
	UUserProfileInfo* UserProfile;


};
