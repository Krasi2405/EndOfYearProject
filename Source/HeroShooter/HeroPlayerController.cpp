// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroPlayerController.h"
#include "UI/IngameMenu.h"

#include "CustomMacros.h"

AHeroPlayerController::AHeroPlayerController() {
	SetReplicatingMovement(true); // Needed so linetrace works on server.

	bShowMouseCursor = false;
	bEnableClickEvents = false;
	bEnableMouseOverEvents = true;
}


void AHeroPlayerController::BeginPlay() {
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}

void AHeroPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	if (validate(IsValid(InputComponent)) == false) { return; }
	UE_LOG(LogTemp, Warning, TEXT("SetupInputComponent"))
	InputComponent->BindAction(FName("SwitchIngameMenu"), IE_Pressed, this, &AHeroPlayerController::SwitchIngameMenu);
}


void AHeroPlayerController::SwitchIngameMenu() {
	UE_LOG(LogTemp, Warning, TEXT("Switch Ingame Menu"));
	bIngameMenuActive ? DeactivateIngameMenu() : ActivateIngameMenu();
}


void AHeroPlayerController::DeactivateIngameMenu() {
	if (validate(IsValid(IngameMenu)) == false) { return; }

	IngameMenu->RemoveFromViewport();
	IngameMenu = nullptr;

	FInputModeGameOnly InputModeData;
	SetInputMode(InputModeData);

	bShowMouseCursor = false;
	bEnableTouchEvents = false;
	bEnableMouseOverEvents = true;
	bIngameMenuActive = false;
}


void AHeroPlayerController::ActivateIngameMenu() {
	if (validate(IsValid(IngameMenuClass)) == false) { return; }
	if (validate(IsValid(IngameMenu) == false) == false) { return; }

	IngameMenu = CreateWidget<UIngameMenu>(this, IngameMenuClass);
	IngameMenu->AddToViewport();

	FInputModeGameAndUI InputModeData;
	InputModeData.SetWidgetToFocus(IngameMenu->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputModeData);

	bShowMouseCursor = true;
	bEnableTouchEvents = true;
	bEnableMouseOverEvents = true;
	bIngameMenuActive = true;
}