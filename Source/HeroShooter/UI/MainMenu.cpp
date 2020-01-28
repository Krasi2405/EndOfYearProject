// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "GameFramework/PlayerController.h"

#include "CustomMacros.h"

bool UMainMenu::Initialize() {
	bool HasInitialized = Super::Initialize();
	if (HasInitialized == false) { return false; }

	if (validate(IsValid(HostButton)) == false) { return false; }
	if (validate(IsValid(JoinServerButton)) == false) { return false; }
	if (validate(IsValid(JoinMenuButton)) == false) { return false; }
	if (validate(IsValid(JoinMenuBackButton)) == false) { return false; }
	if (validate(IsValid(QuitButton)) == false) { return false; }

	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
	JoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::EnterJoinMenu);
	JoinServerButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);
	JoinMenuBackButton->OnClicked.AddDynamic(this, &UMainMenu::EnterMainMenu);
	QuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);
	return true;
}


void UMainMenu::HostServer() {
	if (validate(MenuInterface != nullptr) == false) { return; }
	MenuInterface->Host();
}


void UMainMenu::JoinServer() {
	if (validate(MenuInterface != nullptr) == false) { return; }
	if (validate(IsValid(IPTextBox)) == false) { return; }

	FString IPAddress = IPTextBox->GetText().ToString();
	MenuInterface->Join(IPAddress);
}


void UMainMenu::EnterJoinMenu() {
	if (validate(IsValid(MenuSwitcher)) == false) { return; }
	if (validate(IsValid(JoinMenu)) == false) { return; }
	MenuSwitcher->SetActiveWidget(JoinMenu);
}


void UMainMenu::EnterMainMenu() {
	if (validate(IsValid(MenuSwitcher)) == false) { return; }
	if (validate(IsValid(JoinMenu)) == false) { return; }
	MenuSwitcher->SetActiveWidget(MainMenu);
}


void UMainMenu::SetMenuInterface(IMenuInterface* MenuInterface) {
	this->MenuInterface = MenuInterface;
}


void UMainMenu::QuitGame() {
	APlayerController* PlayerController = GetOwningPlayer();
	if (validate(IsValid(PlayerController)) == false) { return; }
	PlayerController->ConsoleCommand("quit");
}