// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroPlayerController.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/GameState.h"
#include "Engine/World.h"

#include "UI/IngameMenu.h"
#include "UI/ChatBox.h"
#include "CustomMacros.h"

AHeroPlayerController::AHeroPlayerController() {
	bReplicates = true;
	SetReplicatingMovement(true); // Needed so linetrace works on server.

	bShowMouseCursor = false;
	bEnableClickEvents = false;
	bEnableMouseOverEvents = true;
}


void AHeroPlayerController::BeginPlay() {
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	if (validate(IsValid(ChatBoxClass)) == false) { return; }
	ChatBox = CreateWidget<UChatBox>(this, ChatBoxClass);
	if (validate(IsValid(ChatBox)) == false) { return; }
	CloseChat();
	ChatBox->AddToViewport();
}

void AHeroPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	if (validate(IsValid(InputComponent)) == false) { return; }

	InputComponent->BindAction(FName("SwitchIngameMenu"), IE_Pressed, this, &AHeroPlayerController::SwitchIngameMenu);
	InputComponent->BindAction(FName("ToggleChat"), IE_Pressed, this, &AHeroPlayerController::ToggleChat);

}

void AHeroPlayerController::SwitchIngameMenu() {
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


void AHeroPlayerController::SendMessageRequest_Implementation(const FString& Message) {
	UWorld* World = GetWorld();
	if (validate(IsValid(World)) == false) { return; }
	AGameState* GameState = Cast<AGameState>(World->GetGameState());

	FString PlayerName = GetName();

	if (validate(IsValid(GameState)) == false) { return; }
	TArray<APlayerState*>& PlayerStates = GameState->PlayerArray;
	for (APlayerState* PlayerState : PlayerStates) {
		if (validate(IsValid(PlayerState)) == false) { continue; }
		APawn* ControlledPawn = PlayerState->GetPawn();
		if (validate(IsValid(ControlledPawn)) == false) { continue; }
		AHeroPlayerController* PlayerController = Cast<AHeroPlayerController>(ControlledPawn->GetController());
		if (validate(IsValid(PlayerController)) == false) { continue; }
		
		PlayerController->ReceiveMessage(PlayerName, Message);
	}

}


void AHeroPlayerController::ReceiveMessage_Implementation(const FString& PlayerName, const FString& Message) {
	if (validate(IsValid(ChatBox)) == false) { return; }
	ChatBox->AddMessage(PlayerName, Message);
}


void AHeroPlayerController::ToggleChat() {
	ChatBox->GetVisibility() == ESlateVisibility::Visible ? CloseChat() : OpenChat();
}


void AHeroPlayerController::OpenChat() {
	if (validate(IsValid(ChatBox)) == false) { return; }
	ChatBox->Open();

	SetInputMode(FInputModeGameAndUI());
}


void AHeroPlayerController::CloseChat() {
	if (validate(IsValid(ChatBox)) == false) { return; }
	ChatBox->Close();

	SetInputMode(FInputModeGameOnly());
}