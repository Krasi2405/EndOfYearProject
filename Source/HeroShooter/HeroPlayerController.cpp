// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroPlayerController.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/GameState.h"
#include "Engine/World.h"
#include "UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

#include "UI/IngameMenu.h"
#include "UI/ChatBox.h"
#include "UI/HeroPickerMenu.h"
#include "CustomMacros.h"
#include "BaseCharacter.h"
#include "HeroSpawner.h"
#include "BaseCharacter.h"
#include "GameFramework/SpectatorPawn.h"
#include "HealthComponent.h"
#include "GameModes/HeroShooterGameMode.h"
#include "GameModes/HeroShooterGameState.h"
#include "GameModes/GameModeInfoWidget.h"

AHeroPlayerController::AHeroPlayerController() {
	bReplicates = true;
	SetReplicatingMovement(true); // Needed so linetrace works on server.

	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}



void AHeroPlayerController::AcknowledgePossession(APawn* Pawn) {
	Super::AcknowledgePossession(Pawn);

	ABaseCharacter* PossessedCharacter = Cast<ABaseCharacter>(Pawn);
	if (validate(IsValid(PossessedCharacter)) == false) { return; }

	UHealthComponent* HealthComponent = PossessedCharacter->FindComponentByClass<UHealthComponent>();
	if (validate(IsValid(HealthComponent)) == false) { return; }

	// TODO: Respawn timer.
}



void AHeroPlayerController::OnPossess(APawn* Pawn) {
	Super::OnPossess(Pawn);

	if (validate(IsValid(Pawn)) == false) { return; }

	ABaseCharacter* PossessedCharacter = Cast<ABaseCharacter>(Pawn);
	if (validate(IsValid(PossessedCharacter)) == false) { return; }
	
	UHealthComponent* HealthComponent = PossessedCharacter->FindComponentByClass<UHealthComponent>();
	if (validate(IsValid(HealthComponent)) == false) { return; }
	HealthComponent->OnDeath.AddDynamic(this, &AHeroPlayerController::ServerHandleDeath);
}


void AHeroPlayerController::ServerHandleDeath() {
	if (validate(HasAuthority()) == false) { return; }
	UE_LOG(LogTemp, Warning, TEXT("%s has died!"), *GetName());

	UWorld* World = GetWorld();
	if (validate(IsValid(World)) == false) { return; }
	AHeroShooterGameMode* GameMode = Cast<AHeroShooterGameMode>(World->GetAuthGameMode());
	if (validate(IsValid(GameMode)) == false) { return; }
	GameMode->HandleDeath(this);

	// TODO: Wait a bit and respawn player
}


void AHeroPlayerController::BeginPlay() {
	if (IsLocalController()) {
		UWorld* World = GetWorld();
		if (validate(IsValid(World)) == false) { return; }
		AHeroShooterGameState* GameState = World->GetGameState<AHeroShooterGameState>();
		if (validate(IsValid(GameState)) == false) { return; }
		GameState->OnWinConditionSent.AddDynamic(this, &AHeroPlayerController::HandleWinCondition);

		TSubclassOf<UGameModeInfoWidget> InfoWidgetTemplate = GameState->GetInfoWidgetTemplate();
		if (validate(IsValid(InfoWidgetTemplate)) == false) { return; }
		GameModeInfoWidget = CreateWidget<UGameModeInfoWidget>(this, InfoWidgetTemplate);
		if (validate(IsValid(GameModeInfoWidget)) == false) { return; }
		GameModeInfoWidget->AddToViewport();
		GameModeInfoWidget->Setup(GameState);

		if (validate(IsValid(ChatBoxClass)) == false) { return; }
		ChatBox = CreateWidget<UChatBox>(this, ChatBoxClass);
		if (validate(IsValid(ChatBox)) == false) { return; }
		CloseChat();
		ChatBox->AddToViewport();

		if (validate(IsValid(HeroPickerClass)) == false) { return; }
		HeroPicker = CreateWidget<UHeroPickerMenu>(this, HeroPickerClass);
		if (validate(IsValid(HeroPicker)) == false) { return; }
		HeroPicker->AddToViewport();
		HeroPicker->OnHeroSelected.AddDynamic(this, &AHeroPlayerController::ChooseHero);

		FInputModeUIOnly InputMode;
		SetInputMode(InputMode);
	}
	
	if (LastTeamSetupIndex != TeamIndex) {
		TeamSetup();
	}

	bBeginPlayExecuted = true;
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


void AHeroPlayerController::SetTeamIndex(int NewTeamIndex) {
	TeamIndex = NewTeamIndex;

	if (LastTeamSetupIndex != TeamIndex && bBeginPlayExecuted) {
		TeamSetup();
	}
}


int AHeroPlayerController::GetTeamIndex() {
	return TeamIndex;
}


void AHeroPlayerController::OnRep_TeamIndex() {
	SetTeamIndex(TeamIndex);

	if (LastTeamSetupIndex != TeamIndex && bBeginPlayExecuted) {
		TeamSetup();
	}
}


void AHeroPlayerController::TeleportSpectatorToHeroPicker() {
	if (validate(IsValid(TeamSpawner)) == false) { return; }
	FVector Location = TeamSpawner->GetCameraSpotLocation();
	FRotator Rotation = TeamSpawner->GetCameraSpotLookAtRotation();

	ASpectatorPawn* SpectatorPawn = GetSpectatorPawn();
	if (validate(IsValid(SpectatorPawn)) == false) { return; }
	SpectatorPawn->TeleportTo(Location, Rotation);
}


void AHeroPlayerController::TeamSetup() {
	UE_LOG(LogTemp, Warning, TEXT("TeamSetup"))

	if (validate(TeamIndex != -1) == false) { return; }
	
	UWorld* World = GetWorld();
	if (validate(IsValid(World)) == false) { return; }

	TArray<AActor*> Spawners;
	UGameplayStatics::GetAllActorsOfClass(World, AHeroSpawner::StaticClass(), Spawners);
	if (validate(Spawners.Num() > 0) == false) { return; }

	for (AActor* SpawnerActor : Spawners) {
		AHeroSpawner* Spawner = Cast<AHeroSpawner>(SpawnerActor);
		if (validate(IsValid(Spawner)) == false) { return; }
		if (Spawner->GetTeamIndex() == TeamIndex) {
			UE_LOG(LogTemp, Warning, TEXT("%s got Spawner %s"), *GetName(), *Spawner->GetName());
			TeamSpawner = Spawner;
			break;
		}
	}
	
	if (IsLocalController()) {
		if (validate(IsValid(HeroPicker)) == false) { return; }
		if (validate(IsValid(TeamSpawner)) == false) { return; }
		TeamSpawner->Setup(this);
		HeroPicker->Setup(TeamSpawner);
		TeleportSpectatorToHeroPicker();
	}
	LastTeamSetupIndex = TeamIndex;
}

void AHeroPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHeroPlayerController, TeamIndex);
}

void AHeroPlayerController::ChooseHero(TSubclassOf<ABaseCharacter> Hero) {
	DeactivateHeroPicker();
	ServerSpawnHero(Hero);
}

void AHeroPlayerController::ServerSpawnHero_Implementation(TSubclassOf<ABaseCharacter> Hero) {
	if (validate(IsValid(TeamSpawner)) == false) { return; }
	if (validate(IsValid(Hero)) == false) { return; }
	TeamSpawner->SpawnHero(this, Hero);
}


void AHeroPlayerController::ActivateHeroPicker() {
	bShowMouseCursor = true;
	SetInputMode(FInputModeUIOnly());

	if (validate(IsValid(HeroPicker)) == false) { return; }
	HeroPicker->SetVisibility(ESlateVisibility::Visible);
}


void AHeroPlayerController::DeactivateHeroPicker() {
	bShowMouseCursor = false;
	SetInputMode(FInputModeGameOnly());

	if (validate(IsValid(HeroPicker)) == false) { return; }
	HeroPicker->SetVisibility(ESlateVisibility::Hidden);
}



void AHeroPlayerController::HandleWinCondition(int WinningTeamIndex) {
	WinningTeamIndex == TeamIndex ? ShowWinningDisplay() : ShowLosingDisplay();
}


void AHeroPlayerController::ShowWinningDisplay() {
	if (validate(IsValid(WinningTeamMessageWidget)) == false) { return; }
	UUserWidget* WinningDisplay = CreateWidget(this, WinningTeamMessageWidget);
	if (validate(IsValid(WinningDisplay)) == false) { return; }
	WinningDisplay->AddToViewport();
}

void AHeroPlayerController::ShowLosingDisplay() {
	if (validate(IsValid(LosingTeamMessageWidget)) == false) { return; }
	UUserWidget* LosingDisplay = CreateWidget(this, LosingTeamMessageWidget);
	if (validate(IsValid(LosingDisplay)) == false) { return; }
	LosingDisplay->AddToViewport();
}