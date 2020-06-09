// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroPlayerController.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/GameState.h"
#include "Engine/World.h"
#include "UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Player.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

#include "UI/IngameHUD.h"
#include "UI/IngameMenu.h"
#include "UI/ChatBox.h"
#include "UI/HeroPickerMenu.h"
#include "UI/HeroInfoWidget.h"
#include "UI/StatisticsTab.h"
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
#include "HeroPlayerState.h"

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
	IngameHUD->HideHint();
	UHeroInfoWidget* HeroInfo = IngameHUD->GetHeroInfoWidget();
	if (validate(IsValid(HeroInfo)) == false) { return; }
	UHealthComponent* HealthComponent = PossessedCharacter->FindComponentByClass<UHealthComponent>();
	if (validate(IsValid(HealthComponent)) == false) { return; }
	HealthComponent->OnDeath.AddDynamic(this, &AHeroPlayerController::ClientHandleDeath);
	HeroInfo->SetupHealthBar(HealthComponent);

	PossessedCharacter->OnWeaponChange.AddDynamic(this, &AHeroPlayerController::ChangeWeapon);
	if (IsValid(PossessedCharacter->GetEquippedWeapon())) {
		ChangeWeapon(PossessedCharacter->GetEquippedWeapon());
	}

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


void AHeroPlayerController::ServerHandleDeath() {
	if (validate(HasAuthority()) == false) { return; }
	UE_LOG(LogTemp, Warning, TEXT("%s has died!"), *GetName());

	UWorld* World = GetWorld();
	if (validate(IsValid(World)) == false) { return; }

	AHeroShooterGameMode* GameMode = Cast<AHeroShooterGameMode>(World->GetAuthGameMode());
	if (validate(IsValid(GameMode)) == false) { return; }

	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(GetPawn());
	if (validate(IsValid(BaseCharacter)) == false) { return; }
	UHealthComponent* HealthComponent = BaseCharacter->FindComponentByClass<UHealthComponent>();
	if (validate(IsValid(HealthComponent)) == false) { return; }

	AController* KillerController = Cast<AController>(HealthComponent->GetLastDamagedBy());
	if (IsValid(KillerController)) {
		UE_LOG(LogTemp, Warning, TEXT("Last Damaged By In ServerHandleDeath %s"), *KillerController->GetName());
	}
	GameMode->HandleDeath(this, KillerController);
	
	FTimerManager& TimerManager = GetWorldTimerManager();
	TimerManager.SetTimer(RespawnTimerHandle, this, &AHeroPlayerController::Respawn, RespawnDelay, false, RespawnDelay);

	BaseCharacter->SetActorEnableCollision(false);
	BaseCharacter->GetCharacterMovement()->GravityScale = 0;
}


void AHeroPlayerController::ClientHandleDeath() {
	AIngameHUD* HUD = GetHUD<AIngameHUD>();
	if (validate(IsValid(HUD)) == false) { return; }

	HUD->SetTimer(RespawnDelay);
	HUD->ShowHint("Respawning in");


	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(GetPawn());
	BaseCharacter->DisableInput(this);
}


void AHeroPlayerController::Respawn() {
	UWorld* World = GetWorld();
	if (validate(IsValid(World)) == false) { return; }

	AHeroShooterGameState* GameState = Cast<AHeroShooterGameState>(World->GetGameState());
	if (validate(IsValid(GameState)) == false) { return; }

	AHeroPlayerState* PlayerState = GetPlayerState<AHeroPlayerState>();
	if (validate(IsValid(PlayerState)) == false) { return; }

	AHeroSpawner* HeroSpawner = GameState->GetTeamSpawner(PlayerState->GetTeamIndex());
	HeroSpawner->SpawnLastSelectedHero(this);
}


void AHeroPlayerController::BeginPlay() {
	Super::BeginPlay();

	AHeroPlayerState* PlayerState = GetPlayerState<AHeroPlayerState>();
	if (validate(IsValid(PlayerState)) == false) { return; }
	SetTeamIndex(PlayerState->GetTeamIndex());
	PlayerState->OnTeamChange.AddUniqueDynamic(this, &AHeroPlayerController::OnTeamChange);


	UWorld* World = GetWorld();
	if (validate(IsValid(World)) == false) { return; }

	AHeroShooterGameState* GameState = World->GetGameState<AHeroShooterGameState>();
	if (validate(IsValid(GameState)) == false) { return; }
	GameState->OnWinConditionSent.AddDynamic(this, &AHeroPlayerController::HandleWinCondition);

	AIngameHUD* IngameHUD = GetHUD<AIngameHUD>();
	if (IsValid(IngameHUD)) {
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

	InputComponent->BindAction(FName("ShowStats"), IE_Pressed, this, &AHeroPlayerController::ShowStats);
	InputComponent->BindAction(FName("ShowStats"), IE_Released, this, &AHeroPlayerController::HideStats);
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
	UE_LOG(LogTemp, Warning, TEXT("Change team on %s to %d"), *GetName(), NewTeamIndex);
	TeamIndex = NewTeamIndex;

	if (LastTeamSetupIndex != TeamIndex && bBeginPlayExecuted) {
		TeamSetup();
	}
}



void AHeroPlayerController::TeleportSpectatorToHeroPicker() {
	if (validate(IsValid(TeamSpawner)) == false) { return; }
	FVector Location = TeamSpawner->GetCameraSpotLocation();
	FRotator Rotation = TeamSpawner->GetCameraSpotLookAtRotation();

	SetControlRotation(Rotation);
	ClientSetLocation(Location, Rotation);
}


void AHeroPlayerController::TeamSetup() {
	if (validate(TeamIndex != -1) == false) { return; }

	TeamSpawner = GetAssociatedHeroSpawner();
	if (IsLocalController()) {
		if (IsValid(TeamSpawner)) {
			TeamSpawner->OnPlayerEnter.RemoveAll(this);
			TeamSpawner->OnPlayerExit.RemoveAll(this);
		}

		if (validate(IsValid(TeamSpawner)) == false) { return; }
		
		TeamSpawner->Setup(this);
		TeamSpawner->OnPlayerEnter.AddDynamic(this, &AHeroPlayerController::OnEnterSpawnArea);
		TeamSpawner->OnPlayerExit.AddDynamic(this, &AHeroPlayerController::OnExitSpawnArea);
	}
	TeleportSpectatorToHeroPicker();
	LastTeamSetupIndex = TeamIndex;
}


void AHeroPlayerController::ChooseHero(TSubclassOf<ABaseCharacter> Hero) {
	APawn* Pawn = GetPawn();
	if (IsValid(Pawn)) {
		Pawn->Destroy();
	}
	ServerSpawnHero(Hero);
}


void AHeroPlayerController::ServerSpawnHero_Implementation(TSubclassOf<ABaseCharacter> Hero) {
	if (validate(IsValid(Hero)) == false) { return; }

	AHeroSpawner* TeamSpawner = GetAssociatedHeroSpawner();
	if (validate(IsValid(TeamSpawner)) == false) { return; }
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

	TeleportSpectatorToHeroPicker();
	IngameHUD->HideHint();
	UnPossess();
	ActivateHeroPicker();
}


void AHeroPlayerController::ShowStats() {
	AIngameHUD* HUD = GetHUD<AIngameHUD>();
	if (validate(IsValid(HUD)) == false) { return; }
	HUD->ShowStatisticsTab();
}


void AHeroPlayerController::HideStats() {
	AIngameHUD* HUD = GetHUD<AIngameHUD>();
	if (validate(IsValid(HUD)) == false) { return; }
	HUD->HideStatisticsTab();
}

AHeroSpawner* AHeroPlayerController::GetAssociatedHeroSpawner() {
	if (TeamIndex == -1) {
		UE_LOG(LogTemp, Warning, TEXT("No spawner for team -1"));
		return nullptr; 
	}

	UWorld* World = GetWorld();
	if (validate(IsValid(World)) == false) { return nullptr; }
	AHeroShooterGameState* GameState = Cast<AHeroShooterGameState>(World->GetGameState<AHeroShooterGameState>());
	if (validate(IsValid(GameState)) == false) { return nullptr; }

	AHeroSpawner* Spawner = GameState->GetTeamSpawner(TeamIndex);
	if (validate(IsValid(Spawner)) == false) { return nullptr; }

	return Spawner;
}



void AHeroPlayerController::OnTeamChange(AHeroPlayerState* HeroPlayerState) {
	SetTeamIndex(HeroPlayerState->GetTeamIndex());
}



void AHeroPlayerController::ChangeWeapon(AWeapon* Weapon) {
	AIngameHUD* IngameHUD = GetHUD<AIngameHUD>();
	if (validate(IsValid(IngameHUD)) == false) { return; }
	UHeroInfoWidget* HeroInfo = IngameHUD->GetHeroInfoWidget();
	if (validate(IsValid(HeroInfo)) == false) { return; }

	HeroInfo->SetupAmmoBar(Weapon);
}