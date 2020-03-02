// Fill out your copyright notice in the Description page of Project Settings.


#include "IngameHUD.h"
#include "Blueprint/UserWidget.h"

#include "IngameMenu.h"
#include "ChatBox.h"
#include "HeroPickerMenu.h"
#include "CustomMacros.h"
#include "GameModes/GameModeInfoWidget.h"
#include "GameModes/HeroShooterGameState.h"
#include "GameFramework/PlayerController.h"
#include "HeroInfoWidget.h"

void AIngameHUD::SetupWidgets(AHeroShooterGameState* HeroShooterGameState) {
	APlayerController* PlayerController = GetOwningPlayerController();
	if (validate(IsValid(PlayerController)) == false) { return; }

	TSubclassOf<UGameModeInfoWidget> InfoWidgetTemplate = HeroShooterGameState->GetInfoWidgetTemplate();
	if (validate(IsValid(InfoWidgetTemplate)) == false) { return; }
	GameModeInfoWidget = CreateWidget<UGameModeInfoWidget>(PlayerController, InfoWidgetTemplate);
	if (validate(IsValid(GameModeInfoWidget)) == false) { return; }
	GameModeInfoWidget->AddToViewport();
	GameModeInfoWidget->Setup(HeroShooterGameState);


	if (validate(IsValid(ChatBoxClass)) == false) { return; }
	ChatBox = CreateWidget<UChatBox>(PlayerController, ChatBoxClass);
	if (validate(IsValid(ChatBox)) == false) { return; }
	ChatBox->AddToViewport();
	CloseChat();

	if (validate(IsValid(HeroPickerClass)) == false) { return; }
	HeroPicker = CreateWidget<UHeroPickerMenu>(PlayerController, HeroPickerClass);
	if (validate(IsValid(HeroPicker)) == false) { return; }
	HeroPicker->AddToViewport();


	if (validate(IsValid(HeroInfoTemplate)) == false) { return; }
	HeroInfo = CreateWidget<UHeroInfoWidget>(PlayerController, HeroInfoTemplate);
	if (validate(IsValid(HeroInfo)) == false) { return; }
	HeroInfo->AddToViewport();
}


UHeroInfoWidget* AIngameHUD::GetHeroInfoWidget() {
	return HeroInfo;
}


UGameModeInfoWidget* AIngameHUD::GetGameModeInfoWidget() {
	return GameModeInfoWidget;
}


UHeroPickerMenu* AIngameHUD::GetHeroPicker() {
	return HeroPicker;
}


void AIngameHUD::ActivateHeroPicker() {
	if (validate(IsValid(HeroPicker)) == false) { return; }
	HeroPicker->SetVisibility(ESlateVisibility::Visible);

	SetInputSettings(FInputModeUIOnly(), true);

	if (validate(IsValid(HeroInfo)) == false) { return; }
	HeroInfo->SetVisibility(ESlateVisibility::Hidden);
}


void AIngameHUD::DeactivateHeroPicker() {
	if (validate(IsValid(HeroPicker)) == false) { return; }
	HeroPicker->SetVisibility(ESlateVisibility::Hidden);

	SetInputSettings(FInputModeGameOnly(), false);


	if (validate(IsValid(HeroInfo)) == false) { return; }
	HeroInfo->SetVisibility(ESlateVisibility::Visible);
}


void AIngameHUD::AddChatMessage(const FString& PlayerName, const FString& Message) {
	if (validate(IsValid(ChatBox)) == false) { return; }
	ChatBox->AddMessage(PlayerName, Message);
}


void AIngameHUD::OpenChat() {
	if (validate(IsValid(ChatBox)) == false) { return; }
	ChatBox->Open();

	SetInputSettings(FInputModeGameAndUI(), false);
}


void AIngameHUD::CloseChat() {
	if (validate(IsValid(ChatBox)) == false) { return; }
	ChatBox->Close();

	SetInputSettings(FInputModeGameOnly(), false);
}


void AIngameHUD::ActivateIngameMenu() {
	if (validate(IsValid(IngameMenuClass)) == false) { return; }
	if (validate(IsValid(IngameMenu) == false) == false) { return; }


	APlayerController* PlayerController = GetOwningPlayerController();
	if (validate(IsValid(PlayerController)) == false) { return; }

	IngameMenu = CreateWidget<UIngameMenu>(PlayerController, IngameMenuClass);
	IngameMenu->AddToViewport();

	SetInputSettings(FInputModeGameOnly(), true);
}


void AIngameHUD::DeactivateIngameMenu() {
	if (validate(IsValid(IngameMenu)) == false) { return; }

	IngameMenu->RemoveFromViewport();
	IngameMenu = nullptr;

	SetInputSettings(FInputModeGameOnly(), false);
}


void AIngameHUD::ShowWinningDisplay() {
	APlayerController* PlayerController = GetOwningPlayerController();
	if (validate(IsValid(PlayerController)) == false) { return; }

	if (validate(IsValid(WinningTeamMessageWidget)) == false) { return; }
	UUserWidget* WinningDisplay = CreateWidget(PlayerController, WinningTeamMessageWidget);
	if (validate(IsValid(WinningDisplay)) == false) { return; }
	WinningDisplay->AddToViewport();
}


void AIngameHUD::ShowLosingDisplay() {
	APlayerController* PlayerController = GetOwningPlayerController();
	if (validate(IsValid(PlayerController)) == false) { return; }

	if (validate(IsValid(LosingTeamMessageWidget)) == false) { return; }
	UUserWidget* LosingDisplay = CreateWidget(PlayerController, LosingTeamMessageWidget);
	if (validate(IsValid(LosingDisplay)) == false) { return; }
	LosingDisplay->AddToViewport();
}



void AIngameHUD::SetInputSettings(const FInputModeDataBase& InputModeSettings, bool bShowMouse) {
	APlayerController* PlayerController = GetOwningPlayerController();
	if (validate(IsValid(PlayerController)) == false) { return; }

	PlayerController->bShowMouseCursor = bShowMouse;
	PlayerController->SetInputMode(InputModeSettings);
}