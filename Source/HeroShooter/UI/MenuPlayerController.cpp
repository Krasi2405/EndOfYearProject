// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuPlayerController.h"


AMenuPlayerController::AMenuPlayerController() {
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}

void AMenuPlayerController::BeginPlay() {
	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputMode);
}