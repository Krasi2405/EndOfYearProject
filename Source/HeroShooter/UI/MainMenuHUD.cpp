// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuHUD.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "MainMenu.h"
#include "UserProfileInfo.h"
#include "CustomMacros.h"
#include "MultiplayerGameInstance.h"


void AMainMenuHUD::BeginPlay() {
	Super::BeginPlay();
	
	APlayerController* PlayerController = GetOwningPlayerController();
	if (validate(IsValid(PlayerController)) == false) { return; }

	UMultiplayerGameInstance* GameInstance = GetGameInstance<UMultiplayerGameInstance>();
	if (validate(IsValid(GameInstance)) == false) { return; }

	if (validate(IsValid(MainMenuTemplate)) == false) { return; }
	MainMenu = CreateWidget<UMainMenu>(PlayerController, MainMenuTemplate);
	if (validate(IsValid(MainMenu)) == false) { return; }
	MainMenu->AddToViewport();
	MainMenu->SetMenuInterface(GameInstance);

	if (validate(IsValid(UserProfileTemplate)) == false) { return; }
	UserProfile = CreateWidget<UUserProfileInfo>(PlayerController, UserProfileTemplate);
	if (validate(IsValid(UserProfile)) == false) { return; }
	UserProfile->AddToViewport();
	
}