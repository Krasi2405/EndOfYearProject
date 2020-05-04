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

	UE_LOG(LogTemp, Warning, TEXT("Enemy AI BeginPlay"))

	if (validate(IsValid(HeroListTable)) == false) { return; }

	TArray<FHeroListTableRow*> Heroes;
	HeroListTable->GetAllRows<FHeroListTableRow>(TEXT("GENERAL"), Heroes);
	FHeroListTableRow* HeroRow = Heroes[FMath::RandRange(0, Heroes.Num() - 1)];
	TSubclassOf<ABaseCharacter> BaseCharacterTemplate = HeroRow->Asset;
	if (validate(IsValid(BaseCharacterTemplate)) == false) { return; }

	// TODO: Dont copy paste and do actual architecture.
	AHeroPlayerState* HeroPlayerState = GetPlayerState<AHeroPlayerState>();
	if (validate(IsValid(HeroPlayerState)) == false) { return; }
	if (validate(HeroPlayerState->GetTeamIndex() != -1) == false) { return; }

	UWorld* World = GetWorld();
	if (validate(IsValid(World)) == false) { return; }

	TArray<AActor*> Spawners;
	UGameplayStatics::GetAllActorsOfClass(World, AHeroSpawner::StaticClass(), Spawners);
	if (validate(Spawners.Num() > 0) == false) { return; }

	for (AActor* SpawnerActor : Spawners) {
		AHeroSpawner* Spawner = Cast<AHeroSpawner>(SpawnerActor);
		if (validate(IsValid(Spawner)) == false) { return; }
		if (Spawner->GetTeamIndex() == HeroPlayerState->GetTeamIndex()) {
			UE_LOG(LogTemp, Warning, TEXT("%s got Spawner %s"), *GetName(), *Spawner->GetName());
			HeroSpawner = Spawner;
			break;
		}
	}

	HeroSpawner->SpawnHero(this, BaseCharacterTemplate);
}


void AEnemyAIController::OnPossess(APawn* Pawn) {
	Super::OnPossess(Pawn);

	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Pawn);
	if (validate(IsValid(BaseCharacter)) == false) { return; }

	UBehaviorTree* BehaviorTree = BaseCharacter->GetAIBehaviorTreeForCurrentGamemode();
	if (validate(IsValid(BehaviorTree)) == false) { return; }

	bool bSuccess = RunBehaviorTree(BehaviorTree);
	if (validate(bSuccess) == false) { return; }
}


void AEnemyAIController::Destroyed() {
	Super::Destroyed();

	ABaseCharacter* Character = Cast<ABaseCharacter>(GetPawn());
	if (IsValid(Character)) {
		Character->Destroy();
	}
}