// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroPlayerController.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/GameState.h"
#include "Engine/World.h"
#include "UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Player.h"

#include "UI/IngameHUD.h"
#include "UI/IngameMenu.h"
#include "UI/ChatBox.h"
#include "UI/HeroPickerMenu.h"
#include "UI/HeroInfoWidget.h"
#include "CustomMacros.h"
#include "BaseCharacter.h"
#include "HeroSpawner.h"
#include "BaseCharacter.h"
#include "Weapon.h"
#include "GameFramework/SpectatorPawn.h"
#include "HealthComponent.h"
#include "GameModes/HeroShooterGameMode.h"
#include "GameModes/HeroShooterGameState.h"
#include "GameModes/GameModeInfoWidget.h"
#include "MultiplayerGameInstance.h"


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
	PossessedCharacter->HideHead();

	AIngameHUD* IngameHUD = GetHUD<AIngameHUD>();
	if (validate(IsValid(IngameHUD)) == false) { return; }
	UHeroInfoWidget* HeroInfo = IngameHUD->GetHeroInfoWidget();
	if (validate(IsValid(HeroInfo)) == false) { return; }

	AWeapon* Weapon = PossessedCharacter->GetEquippedWeapon();
	if (validate(IsValid(Weapon)) == false) { return; }

	UHealthComponent* HealthComponent = PossessedCharacter->FindComponentByClass<UHealthComponent>();
	if (validate(IsValid(HealthComponent)) == false) { return; }

	HeroInfo->Setup(Weapon->GetMaxAmmo(), HealthComponent->GetMaxHealth());

	HealthComponent->OnHealthChanged.AddDynamic(HeroInfo, &UHeroInfoWidget::UpdateHealthbar);
	Weapon->OnAmmoChanged.AddDynamic(HeroInfo, &UHeroInfoWidget::UpdateAmmoBar);

	DeactivateHeroPicker();
}



void AHeroPlayerController::OnPossess(APawn* Pawn) {
	Super::OnPossess(Pawn);

	ABaseCharacter* PossessedCharacter = Cast<ABaseCharacter>(Pawn);
	if (validate(IsValid(PossessedCharacter)) == false) { return; }
	
	UHealthComponent* HealthComponent = PossessedCharacter->FindComponentByClass<UHealthComponent>();
	if (validate(IsValid(HealthComponent)) == false) { return; }

	HealthComponent->OnDeath.AddDynamic(this, &AHeroPlayerController::ServerHandleDeath);
}


void AHeroPlayerController::OnUnPossess() {
	Super::OnUnPossess();
}


void AHeroPlayerController::ServerHandleDeath() {
	if (validate(HasAuthority()) == false) { return; }
	UE_LOG(LogTemp, Warning, TEXT("%s has died!"), *GetName());

	UWorld* World = GetWorld();
	if (validate(IsValid(World)) == false) { return; }

	AHeroShooterGameMode* GameMode = Cast<AHeroShooterGameMode>(World->GetAuthGameMode());
	if (validate(IsValid(GameMode)) == false) { return; }

	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(GetPawn());
	BaseCharacter->DisableInput(this);
	if (validate(IsValid(BaseCharacter)) == false) { return; }
	UHealthComponent* HealthComponent = BaseCharacter->FindComponentByClass<UHealthComponent>();
	if (validate(IsValid(HealthComponent)) == false) { return; }

	AHeroPlayerController* KillerHeroController = Cast<AHeroPlayerController>(HealthComponent->GetLastDamagedBy());
	GameMode->HandleDeath(this, KillerHeroController);
	// TODO: Wait a bit and respawn player
}


void AHeroPlayerController::BeginPlay() {
	if (IsLocalController()) {
		UWorld* World = GetWorld();
		if (validate(IsValid(World)) == false) { return; }
		AHeroShooterGameState* GameState = World->GetGameState<AHeroShooterGameState>();
		if (validate(IsValid(GameState)) == false) { return; }
		GameState->OnWinConditionSent.AddDynamic(this, &AHeroPlayerController::HandleWinCondition);

		AIngameHUD* IngameHUD = GetHUD<AIngameHUD>();
		if (validate(IsValid(IngameHUD)) == false) { return; }
		IngameHUD->SetupWidgets(GameState);

		UHeroPickerMenu* HeroPicker = IngameHUD->GetHeroPicker();
		if (validate(IsValid(HeroPicker)) == false) { return; }
		HeroPicker->OnHeroSelected.AddDynamic(this, &AHeroPlayerController::ChooseHero);

		IngameHUD->ActivateHeroPicker();
	}
	
	if (LastTeamSetupIndex != TeamIndex) {
		TeamSetup();
	}

	bBeginPlayExecuted = true;
}


void AHeroPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	if (validate(IsValid(InputComponent)) == false) { return; }

	InputComponent->BindAction(FName("ToggleIngameMenu"), IE_Pressed, this, &AHeroPlayerController::ToggleIngameMenu);
	InputComponent->BindAction(FName("ToggleChat"), IE_Pressed, this, &AHeroPlayerController::ToggleChat);

	InputComponent->BindAction(FName("SwitchHero"), IE_Pressed, this, &AHeroPlayerController::SwitchHero);
}


void AHeroPlayerController::ToggleIngameMenu() {
	AIngameHUD* IngameHUD = GetHUD<AIngameHUD>();
	if (validate(IsValid(IngameHUD)) == false) { return; }

	bIngameMenuActive ? IngameHUD->DeactivateIngameMenu() : IngameHUD->ActivateIngameMenu();
	bIngameMenuActive = !bIngameMenuActive;
}


void AHeroPlayerController::ServerSendMessageRequest_Implementation(const FString& Message) {
	UWorld* World = GetWorld();
	if (validate(IsValid(World)) == false) { return; }
	AHeroShooterGameState* GameState = Cast<AHeroShooterGameState>(World->GetGameState());

	APlayerState* SendingPlayer = GetPlayerState<APlayerState>();
	if (validate(IsValid(SendingPlayer)) == false) { return; }

	if (validate(IsValid(GameState)) == false) { return; }
	TArray<APlayerState*>& PlayerStates = GameState->PlayerArray;

	for (APlayerState* PlayerState : PlayerStates) {
		if (validate(IsValid(PlayerState)) == false) { return; }
		AHeroPlayerController* PlayerController = Cast<AHeroPlayerController>(PlayerState->GetOwner());
		if (validate(IsValid(PlayerController)) == false) { continue; }
		
		PlayerController->ClientReceiveMessage(SendingPlayer, Message);
	}
}


void AHeroPlayerController::ClientReceiveMessage_Implementation(APlayerState* SendingPlayer, const FString& Message) {
	AIngameHUD* IngameHUD = GetHUD<AIngameHUD>();
	if (validate(IsValid(IngameHUD)) == false) { return; }

	UMultiplayerGameInstance* GameInstance = Cast<UMultiplayerGameInstance>(GetGameInstance());
	if (validate(IsValid(GameInstance)) == false) { return; }

	FString PlayerName = GameInstance->GetPlayerUsername(SendingPlayer);
	IngameHUD->AddChatMessage(PlayerName, Message);
}


void AHeroPlayerController::ToggleChat() {
	AIngameHUD* IngameHUD = GetHUD<AIngameHUD>();
	if (validate(IsValid(IngameHUD)) == false) { return; }

	bChatOpen ? IngameHUD->CloseChat() : IngameHUD->OpenChat();
	bChatOpen = !bChatOpen;
}


void AHeroPlayerController::SetTeamIndex(int NewTeamIndex) {
	TeamIndex = NewTeamIndex;

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
			TeamSpawner = Spawner;
			break;
		}
	}
	
	if (IsLocalController()) {
		AIngameHUD* IngameHUD = GetHUD<AIngameHUD>();
		if (validate(IsValid(IngameHUD)) == false) { return; }

		UHeroPickerMenu* HeroPicker = IngameHUD->GetHeroPicker();
		if (validate(IsValid(HeroPicker)) == false) { return; }

		if (validate(IsValid(HeroPicker)) == false) { return; }
		if (validate(IsValid(TeamSpawner)) == false) { return; }
		TeamSpawner->Setup(this);
		TeamSpawner->OnPlayerEnter.AddDynamic(this, &AHeroPlayerController::OnEnterSpawnArea);
		TeamSpawner->OnPlayerExit.AddDynamic(this, &AHeroPlayerController::OnExitSpawnArea);
		HeroPicker->Setup(TeamSpawner);
		TeleportSpectatorToHeroPicker();
	}
	LastTeamSetupIndex = TeamIndex;
}


void AHeroPlayerController::ChooseHero(TSubclassOf<ABaseCharacter> Hero) {
	ServerSpawnHero(Hero);
}


void AHeroPlayerController::ServerSpawnHero_Implementation(TSubclassOf<ABaseCharacter> Hero) {
	if (validate(IsValid(TeamSpawner)) == false) { return; }
	if (validate(IsValid(Hero)) == false) { return; }
	TeamSpawner->SpawnHero(this, Hero);
}


void AHeroPlayerController::ActivateHeroPicker() {
	AIngameHUD* IngameHUD = GetHUD<AIngameHUD>();
	if (validate(IsValid(IngameHUD)) == false) { return; }

	IngameHUD->ActivateHeroPicker();
}


void AHeroPlayerController::DeactivateHeroPicker() {
	AIngameHUD* IngameHUD = GetHUD<AIngameHUD>();
	if (validate(IsValid(IngameHUD)) == false) { return; }

	IngameHUD->DeactivateHeroPicker();
}


void AHeroPlayerController::HandleWinCondition(int WinningTeamIndex) {
	AIngameHUD* IngameHUD = GetHUD<AIngameHUD>();
	if (validate(IsValid(IngameHUD)) == false) { return; }

	WinningTeamIndex == TeamIndex ? IngameHUD->ShowWinningDisplay() : IngameHUD->ShowLosingDisplay();
}



void AHeroPlayerController::OnEnterSpawnArea() {
	AIngameHUD* IngameHUD = GetHUD<AIngameHUD>();
	if (validate(IsValid(IngameHUD)) == false) { return; }

	IngameHUD->ShowHint("Press H to switch hero");
	bCanSwitchHero = true;
}

void AHeroPlayerController::OnExitSpawnArea() {
	AIngameHUD* IngameHUD = GetHUD<AIngameHUD>();
	if (validate(IsValid(IngameHUD)) == false) { return; }

	IngameHUD->HideHint();
	bCanSwitchHero = false;
}


void AHeroPlayerController::SwitchHero() {
	if (bCanSwitchHero == false) { return; }
	AIngameHUD* IngameHUD = GetHUD<AIngameHUD>();
	if (validate(IsValid(IngameHUD)) == false) { return; }

	IngameHUD->HideHint();

	ABaseCharacter* Character = Cast<ABaseCharacter>(GetCharacter());
	if (validate(IsValid(Character)) == false) { return; }

	Character->Destroy();

	UnPossess();
	ActivateHeroPicker();
}