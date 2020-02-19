// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/SpinBox.h"
#include "GameFramework/PlayerController.h"

#include "MultiplayerGameInstance.h"
#include "ServerInfo.h"
#include "CustomMacros.h"

bool UMainMenu::Initialize() {
	bool HasInitialized = Super::Initialize();
	if (HasInitialized == false) { return false; }

	if (validate(IsValid(HostButton)) == false) { return false; }
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	if (validate(IsValid(HostMenuButton)) == false) { return false; }
	HostMenuButton->OnClicked.AddDynamic(this, &UMainMenu::EnterHostMenu);

	if (validate(IsValid(HostMenuBackButton)) == false) { return false; }
	HostMenuBackButton->OnClicked.AddDynamic(this, &UMainMenu::EnterMainMenu);

	if (validate(IsValid(JoinMenuButton)) == false) { return false; }
	JoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::EnterJoinMenu);

	if (validate(IsValid(JoinServerButton)) == false) { return false; }
	JoinServerButton->OnClicked.AddDynamic(this, &UMainMenu::JoinIPServer);

	if (validate(IsValid(JoinMenuBackButton)) == false) { return false; }
	JoinMenuBackButton->OnClicked.AddDynamic(this, &UMainMenu::EnterMainMenu);

	if (validate(IsValid(QuitButton)) == false) { return false; }
	QuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);

	RefreshServerListButton->OnClicked.AddDynamic(this, &UMainMenu::FindSessions);

	return true;
}


void UMainMenu::HostServer() {
	if (validate(MenuInterface != nullptr) == false) { return; }
	if (validate(IsValid(ServerNameTextBox)) == false) { return; }
	if (validate(IsValid(PasswordTextBox)) == false) { return; }
	if (validate(IsValid(PlayerCountSpinBox)) == false) { return; }

	FCustomServerSettings ServerSettings;
	ServerSettings.ServerName = ServerNameTextBox->GetText().ToString();
	ServerSettings.Password = PasswordTextBox->GetText().ToString();
	ServerSettings.MaxPlayerCount = (int) PlayerCountSpinBox->GetValue();

	MenuInterface->Host(ServerSettings);
}


void UMainMenu::FindSessions() {
	if (validate(MenuInterface != nullptr) == false) { return; }
	MenuInterface->FindSessions();
}


void UMainMenu::JoinIPServer() {
	if (validate(MenuInterface != nullptr) == false) { return; }
	if (validate(IsValid(IPTextBox)) == false) { return; }
	FString IPAddress = IPTextBox->GetText().ToString();
	MenuInterface->JoinIP(IPAddress);
}


void UMainMenu::JoinSelectedServerListSession() {
	if (validate(MenuInterface != nullptr) == false) { return; }
	if (validate(IsValid(SelectedServerInfo)) == false) { return; }

	MenuInterface->JoinSession(SelectedServerInfo->GetRowIndex());
}

void UMainMenu::EnterJoinMenu() {
	if (validate(IsValid(MenuSwitcher)) == false) { return; }
	if (validate(IsValid(JoinMenu)) == false) { return; }
	MenuSwitcher->SetActiveWidget(JoinMenu);

	MenuInterface->FindSessions();
}


void UMainMenu::EnterMainMenu() {
	if (validate(IsValid(MenuSwitcher)) && validate(IsValid(MainMenu)) == false) { return; }
	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::EnterHostMenu() {
	if (validate(IsValid(MenuSwitcher)) && validate(IsValid(HostMenu)) == false) { return; }
	MenuSwitcher->SetActiveWidget(HostMenu);
}


void UMainMenu::SetMenuInterface(IMenuInterface* MenuInterface) {
	this->MenuInterface = MenuInterface;
}


void UMainMenu::QuitGame() {
	APlayerController* PlayerController = GetOwningPlayer();
	if (validate(IsValid(PlayerController)) == false) { return; }
	PlayerController->ConsoleCommand("quit");
}


void UMainMenu::AddServer(FServerInfoSettings ServerSettings) {
	if (validate(ServerInfoTemplate != nullptr) == false) { return; }
	UServerInfo* ServerInfo = CreateWidget<UServerInfo>(this, ServerInfoTemplate);
	ServerInfo->UpdateServerInfo(ServerSettings);
	ServerInfo->SetMainMenu(this);
	if (validate(IsValid(ServerInfo)) == false) { return; }
	ServerList->AddChild(ServerInfo);
}


void UMainMenu::ClearServerList() {
	if (validate(IsValid(ServerList)) == false) { return; }
	ServerList->ClearChildren();
}


void UMainMenu::SelectServer(UServerInfo* ServerInfo) {
	if (SelectedServerInfo == ServerInfo) { return; }

	if (IsValid(SelectedServerInfo)) {
		SelectedServerInfo->UnSelect();
	}
	SelectedServerInfo = ServerInfo;
}