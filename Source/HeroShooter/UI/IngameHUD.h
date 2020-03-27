// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameFramework/PlayerController.h"
#include "IngameHUD.generated.h"


class UIngameMenu;
class UChatBox;
class UHeroPickerMenu;
class UGameModeInfoWidget;
class UUserWidget;
class UHeroInfoWidget;
class UHintWidget;
class AHeroShooterGameState;

/**
 * 
 */
UCLASS()
class HEROSHOOTER_API AIngameHUD : public AHUD
{
	GENERATED_BODY()

public:

	void SetupWidgets(AHeroShooterGameState* GameState);

	UGameModeInfoWidget* GetGameModeInfoWidget();


	UHeroInfoWidget* GetHeroInfoWidget();


	UHeroPickerMenu* GetHeroPicker();

	void ActivateHeroPicker();

	void DeactivateHeroPicker();



	void AddChatMessage(const FString& PlayerName, const FString& Message);

	void OpenChat();

	void CloseChat();


	void DeactivateIngameMenu();

	void ActivateIngameMenu();


	void ShowWinningDisplay();

	void ShowLosingDisplay();


	void ShowHint(FString Hint);

	void HideHint();

protected:

	UPROPERTY()
	UGameModeInfoWidget* GameModeInfoWidget = nullptr;


	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHeroInfoWidget> HeroInfoTemplate;

	UPROPERTY()
	UHeroInfoWidget* HeroInfo = nullptr;


	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UIngameMenu> IngameMenuClass;

	UPROPERTY()
	UIngameMenu* IngameMenu = nullptr;


	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UChatBox> ChatBoxClass;

	UPROPERTY()
	UChatBox* ChatBox = nullptr;


	UPROPERTY(EditDefaultsONly)
	TSubclassOf<UHeroPickerMenu> HeroPickerClass;

	UPROPERTY()
	UHeroPickerMenu* HeroPicker = nullptr;


	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> WinningTeamMessageWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> LosingTeamMessageWidget;


	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHintWidget> HintTemplate;


	UPROPERTY()
	UHintWidget* HintWidget = nullptr;


	void SetInputSettings(const FInputModeDataBase& InputModeSettings, bool bShowMouse);
};
