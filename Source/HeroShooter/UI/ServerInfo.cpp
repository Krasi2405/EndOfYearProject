// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerInfo.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "CustomMacros.h"
#include "MainMenu.h"


bool UServerInfo::Initialize() {
	bool HasInitialized = Super::Initialize();
	if (validate(HasInitialized) == false) { return false; }

	if (validate(IsValid(JoinButton)) == false) { return false; }
	JoinButton->OnClicked.AddDynamic(this, &UServerInfo::OnClicked);
	
	JoinButton->WidgetStyle.Pressed.TintColor = SelectedBackgroundColor * 1.5;
	NormalButtonStyle = JoinButton->WidgetStyle;

	return true;
}


void UServerInfo::OnClicked() {
	UWorld* World = GetWorld();
	if (validate(IsValid(World)) == false) { return; };
	if (validate(IsValid(MainMenu)) == false) { return; }

	bool bDoubleClick = World->GetTimerManager().IsTimerActive(DoubleClickHandle);

	if (bDoubleClick) {
		MainMenu->JoinSelectedServerListSession();
	}
	else
	{
		Select();
		MainMenu->SelectServer(this);
		World->GetTimerManager().SetTimer(DoubleClickHandle, DoubleClickDelay, false);
	}
}


int UServerInfo::GetRowIndex() {
	return ServerRowIndex;
}


void UServerInfo::UpdateServerInfo(FServerInfoSettings Info) {
	NameBlock->Text = FText::FromString(Info.Name.LeftChop(Info.Name.Len() - MaxCharacterLimit));
	OwnerBlock->Text = FText::FromString(Info.Owner.LeftChop(Info.Owner.Len() - MaxCharacterLimit));
	PlayerCountBlock->Text = FText::FromString(FString::FromInt(Info.CurrentPlayerCount));
	MaxPlayersBlock->Text = FText::FromString(FString::FromInt(Info.MaxPlayerCount));
	LatencyBlock->Text = FText::FromString(FString::FromInt((int)Info.Latency));
	ServerRowIndex = Info.ServerIndex;
}


void UServerInfo::SetMainMenu(UMainMenu* MainMenu) {
	this->MainMenu = MainMenu;
}


void UServerInfo::Select() {
	// TArray<UWidget*> ButtonChildren = JoinButton->GetAllChildren();
	if (validate(IsValid(JoinButton)) == false) { return; }

	FButtonStyle ButtonStyle = JoinButton->WidgetStyle;
	ButtonStyle.Normal.TintColor = SelectedBackgroundColor;
	ButtonStyle.Hovered.TintColor = SelectedBackgroundColor * 1.2;
	ButtonStyle.Pressed.TintColor = SelectedBackgroundColor * 1.5;
	JoinButton->SetStyle(ButtonStyle);
}


void UServerInfo::UnSelect() {
	JoinButton->SetStyle(NormalButtonStyle);
}