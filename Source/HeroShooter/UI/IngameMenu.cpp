// Fill out your copyright notice in the Description page of Project Settings.


#include "IngameMenu.h"
#include "Button.h"
#include "CustomMacros.h"

bool UIngameMenu::Initialize() {
	bool HasInitialized = Super::Initialize();
	if (validate(HasInitialized) == false) { return false; }

	if (validate(IsValid(LeaveButton)) == false) { return false; }
	LeaveButton->OnClicked.AddDynamic(this, &UIngameMenu::LeaveServer);

	return true;
}

void UIngameMenu::LeaveServer() {
	if (validate(MainMenuName != "") == false) { return; }

	APlayerController* PlayerController = GetOwningPlayer();
	if (validate(IsValid(PlayerController)) == false) return;

	PlayerController->ClientTravel(MainMenuName, ETravelType::TRAVEL_Absolute);
}