// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuPlayerController.h"


AMenuPlayerController::AMenuPlayerController() {
	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputMode);

	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}