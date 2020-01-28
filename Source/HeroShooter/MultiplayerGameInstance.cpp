// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerGameInstance.h"
#include "GameFramework/PlayerController.h"
#include "UI/MainMenu.h"
#include "Engine/Engine.h"
#include "CustomMacros.h"



void UMultiplayerGameInstance::SetMainMenu(UMainMenu* MainMenu) {
	this->MainMenu = MainMenu;
	MainMenu->SetMenuInterface(this);
}


void UMultiplayerGameInstance::Host() {
	UEngine* Engine = GetEngine();
	if (validate(IsValid(Engine)) == false) { return; }
	Engine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Hosting!"));

	UWorld* World = GetWorld();
	if (validate(IsValid(World)) == false) { return; }
	if (validate(GameMapName != "") == false) { return; }
	World->ServerTravel(GameMapName + "?listen", true, true);
}


void UMultiplayerGameInstance::Join(FString IPAddress) {
	UEngine* Engine = GetEngine();
	if (validate(IsValid(Engine)) == false) { return; }
	Engine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Joining ") + IPAddress);
	
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (validate(IsValid(PlayerController)) == false) { return; }
	PlayerController->ClientTravel(IPAddress, ETravelType::TRAVEL_Absolute);
}


