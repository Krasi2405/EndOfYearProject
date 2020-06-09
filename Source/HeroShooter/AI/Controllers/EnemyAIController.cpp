// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "Engine/EngineTypes.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Pawn.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

#include "GameModes/HeroShooterGameState.h"
#include "GameModes/HeroShooterGameMode.h"
#include "HealthComponent.h"
#include "HeroTableRow.h"
#include "BaseCharacter.h"
#include "HeroPlayerState.h"
#include "HeroSpawner.h"
#include "CustomMacros.h"


AEnemyAIController::AEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = false;
	bWantsPlayerState = true;
}


void AEnemyAIController::BeginPlay() {
	Super::BeginPlay();

	AHeroPlayerState* HeroPlayerState = GetPlayerState<AHeroPlayerState>();
	if (validate(IsValid(HeroPlayerState)) == false) { return; }
	HeroPlayerState->OnTeamChange.AddDynamic(this, &AEnemyAIController::OnTeamChange);

	
}


void AEnemyAIController::OnTeamChange(AHeroPlayerState* HeroPlayerState) {
	SetupAI();
}


void AEnemyAIController::SetupAI() {
	SpawnPawn();
}



void AEnemyAIController::OnPossess(APawn* Pawn) {
	Super::OnPossess(Pawn);

	UE_LOG(LogTemp, Warning, TEXT("OnPossess"))
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Pawn);
	if (validate(IsValid(BaseCharacter)) == false) { return; }

	UBehaviorTree* BehaviorTree = BaseCharacter->GetAIBehaviorTreeForCurrentGamemode();
	if (validate(IsValid(BehaviorTree)) == false) { return; }

	bool bSuccess = RunBehaviorTree(BehaviorTree);
	if (validate(bSuccess) == false) { return; }


	UHealthComponent* HealthComponent = BaseCharacter->FindComponentByClass<UHealthComponent>();
	if (validate(IsValid(HealthComponent)) == false) { return; }

	HealthComponent->OnDeath.AddDynamic(this, &AEnemyAIController::ServerHandleDeath);
}


void AEnemyAIController::ServerHandleDeath() {
	UE_LOG(LogTemp, Warning, TEXT("Server Handle Death"))
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
	TimerManager.SetTimer(RespawnTimerHandle, this, &AEnemyAIController::Respawn, RespawnDelay, false, RespawnDelay);

	BaseCharacter->SetActorEnableCollision(false);
	BaseCharacter->GetCharacterMovement()->GravityScale = 0;
}


void AEnemyAIController::Respawn() {
	SpawnPawn();
}


void AEnemyAIController::Destroyed() {
	Super::Destroyed();

	ABaseCharacter* Character = Cast<ABaseCharacter>(GetPawn());
	if (IsValid(Character)) {
		UE_LOG(LogTemp, Warning, TEXT("Destroy AI Character"))
		Character->Destroy();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Destroying AIController with no character"))
	}
}



void AEnemyAIController::SpawnPawn() {
	AHeroPlayerState* HeroPlayerState = GetPlayerState<AHeroPlayerState>();
	if (validate(IsValid(HeroPlayerState)) == false) { return; }
	if (validate(IsValid(HeroListTable)) == false) { return; }

	TArray<FHeroListTableRow*> Heroes;
	HeroListTable->GetAllRows<FHeroListTableRow>(TEXT("GENERAL"), Heroes);
	FHeroListTableRow* HeroRow = Heroes[FMath::RandRange(0, Heroes.Num() - 1)];
	TSubclassOf<ABaseCharacter> BaseCharacterTemplate = HeroRow->Asset;
	if (validate(IsValid(BaseCharacterTemplate)) == false) { return; }
	if (validate(HeroPlayerState->GetTeamIndex() != -1) == false) { return; }

	UWorld* World = GetWorld();
	if (validate(IsValid(World)) == false) { return; }
	AHeroShooterGameState* GameState = World->GetGameState<AHeroShooterGameState>();
	if (validate(IsValid(GameState)) == false) { return; }
	HeroSpawner = GameState->GetTeamSpawner(HeroPlayerState->GetTeamIndex());
	if (validate(IsValid(HeroSpawner)) == false) { return; }
	HeroSpawner->SpawnHero(this, BaseCharacterTemplate);
}