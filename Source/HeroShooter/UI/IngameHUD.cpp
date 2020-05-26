// Fill out your copyright notice in the Description page of Project Settings.


#include "IngameHUD.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"

#include "IngameMenu.h"
#include "ChatBox.h"
#include "HeroPickerMenu.h"
#include "CustomMacros.h"
#include "GameModes/GameModeInfoWidget.h"
#include "GameModes/HeroShooterGameState.h"
#include "HintWidget.h"
#include "StatisticsTab.h"
#include "HeroInfoWidget.h"


void AIngameHUD::SetupWidgets(AHeroShooterGameState* HeroShooterGameState) {
	UE_LOG(LogTemp, Warning, TEXT("Setup widgets for %s"), *GetOwningPlayerController()->GetName());
	TSubclassOf<UGameModeInfoWidget> InfoWidgetTemplate = HeroShooterGameState->GetInfoWidgetTemplate();
	GameModeInfoWidget = SpawnWidget<UGameModeInfoWidget>(InfoWidgetTemplate);

	ChatBox = SpawnWidget<UChatBox>(ChatBoxTemplate);
	if (validate(IsValid(ChatBox))) {
		CloseChat();
	}

	HeroPicker = SpawnWidget<UHeroPickerMenu>(HeroPickerTemplate);
	validate(IsValid(HeroPicker));

	HeroInfo = SpawnWidget<UHeroInfoWidget>(HeroInfoTemplate);
	validate(IsValid(HeroInfo));

	HintWidget = SpawnWidget<UHintWidget>(HintTemplate);
	if (validate(IsValid(HintWidget))) {
		HideHint();
	}

	StatisticsTab = SpawnWidget<UStatisticsTab>(StatisticsTabTemplate);
	if (validate(IsValid(StatisticsTab))) {
		HideStatisticsTab();
	}
}


UHeroInfoWidget* AIngameHUD::GetHeroInfoWidget() {
	return HeroInfo;
}


UGameModeInfoWidget* AIngameHUD::GetGameModeInfoWidget() {
	return GameModeInfoWidget;
}


UStatisticsTab* AIngameHUD::GetStatisticsTab() {
	return StatisticsTab;
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
	validate(SpawnWidget<UUserWidget>(WinningTeamMessageWidget) != nullptr);
}


void AIngameHUD::ShowLosingDisplay() {
	validate(SpawnWidget<UUserWidget>(LosingTeamMessageWidget) != nullptr);
}


void AIngameHUD::SetInputSettings(const FInputModeDataBase& InputModeSettings, bool bShowMouse) {
	APlayerController* PlayerController = GetOwningPlayerController();
	if (validate(IsValid(PlayerController)) == false) { return; }

	PlayerController->bShowMouseCursor = bShowMouse;
	PlayerController->SetInputMode(InputModeSettings);
}


void AIngameHUD::ShowHint(FString Hint) {
	if (IsValid(HintWidget) == false) { return;  }

	HintWidget->SetVisibility(ESlateVisibility::Visible);
	HintWidget->SetHint(Hint);
}


void AIngameHUD::HideHint() {
	HideWidget(HintWidget);
}


void AIngameHUD::ShowStatisticsTab() {
	ShowWidget(StatisticsTab);
}


void AIngameHUD::HideStatisticsTab() {
	HideWidget(StatisticsTab);
}


void AIngameHUD::ShowWidget(UUserWidget* UserWidget) {
	if (ensure(IsValid(UserWidget)) == false) { return; }
	UserWidget->SetVisibility(ESlateVisibility::Visible);
}


void AIngameHUD::HideWidget(UUserWidget* UserWidget) {
	if (ensure(IsValid(HintWidget)) == false) { return; }
	UserWidget->SetVisibility(ESlateVisibility::Collapsed);
}

template <typename WidgetClass>
WidgetClass* AIngameHUD::SpawnWidget(TSubclassOf<UUserWidget> WidgetTemplate) {
	APlayerController* PlayerController = GetOwningPlayerController();
	if (validate(IsValid(PlayerController)) == false) { return nullptr; }

	UUserWidget* UserWidget = nullptr;
	if (validate(IsValid(WidgetTemplate)) == false) { return nullptr; }
	UserWidget = CreateWidget(PlayerController, WidgetTemplate);

	if (validate(IsValid(UserWidget)) == false) { return nullptr; }
	UserWidget->AddToViewport();

	WidgetClass* Widget = Cast<WidgetClass>(UserWidget);
	if (validate(IsValid(Widget)) == false) { return nullptr; }

	return Widget;
}


