// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroSpawner.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "Engine/TriggerVolume.h"

#include "GameModes/HeroShooterGameState.h"
#include "HeroPlayerController.h"
#include "BaseCharacter.h"
#include "HeroTableRow.h"
#include "HeroPlayerState.h"
#include "CustomMacros.h"


// Sets default values for this component's properties
AHeroSpawner::AHeroSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
}


// Called when the game starts
void AHeroSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (TeamIndex == -1) {
		UE_LOG(LogTemp, Warning, TEXT("%s has no team index set!"), *GetName());
		return; 
	}

	SpawnLocation->OnActorBeginOverlap.AddDynamic(this, &AHeroSpawner::OnEnterSpawnArea);
	SpawnLocation->OnActorEndOverlap.AddDynamic(this, &AHeroSpawner::OnExitSpawnArea);
}


void AHeroSpawner::Setup(APlayerController* PlayerController) {
	if (validate(IsValid(PlayerController)) == false) { return; }
	LocalPlayerController = PlayerController;
}


void AHeroSpawner::DestroyPreview() {
	if (IsValid(HeroPreview)) {
		HeroPreview->Destroy();
	}
}


void AHeroSpawner::SpawnHeroPreview(TSubclassOf<ABaseCharacter> HeroTemplate) {
	if (validate(IsValid(HeroTemplate)) == false) { return; }

	DestroyPreview();

	if (validate(IsValid(LocalPlayerController)) == false) { return; }
	FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
	SpawnParameters.Owner = LocalPlayerController;
	SpawnParameters.Instigator = LocalPlayerController->GetInstigator();
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector HeroSpawnLocation = GetActorLocation() + HeroPreviewLocation;
	FRotator HeroSpawnRotation = UKismetMathLibrary::FindLookAtRotation(HeroPreviewLocation, CameraSpot);

	UWorld* World = GetWorld();
	if (validate(IsValid(World)) == false) { return; }

	ABaseCharacter* Hero = World->SpawnActor<ABaseCharacter>(
		HeroTemplate.Get(),
		HeroSpawnLocation,
		HeroSpawnRotation,
		SpawnParameters
	);

	if (validate(IsValid(Hero)) == false) { return; };
	HeroPreview = Hero;
}


void AHeroSpawner::SpawnHero(AController* Controller, TSubclassOf<ABaseCharacter> HeroTemplate) {
	if (validate(IsValid(SpawnLocation)) == false) { return; }
	if (validate(HasAuthority()) == false) { return; }

	FVector Location = SpawnLocation->GetActorLocation();
	FVector BoxSize = SpawnLocation->GetBounds().GetBox().GetSize();
	FTransform SpawnLocation = FTransform(
		UKismetMathLibrary::RandomPointInBoundingBox(
			Location,
			BoxSize
		)
	);

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	UWorld* World = GetWorld();
	if (validate(IsValid(World)) == false) { return; }

	ABaseCharacter* Hero = World->SpawnActor<ABaseCharacter>(
		HeroTemplate,
		SpawnLocation,
		SpawnParameters
	);

	if (validate(IsValid(Hero)) == false) { return; }
	Hero->SetReplicates(false);
	if (validate(IsValid(Controller)) == false) { return; }

	SelectedHeroMap.Add(Controller, HeroTemplate);
	Controller->Possess(Hero);
	


	//
	AHeroPlayerState* PlayerState = Controller->GetPlayerState<AHeroPlayerState>();
	if (validate(IsValid(PlayerState)) == false) { return; }

	AHeroShooterGameState* GameState = Cast<AHeroShooterGameState>(World->GetGameState());
	if (validate(IsValid(GameState)) == false) { return; }
	
	UDataTable* HeroListTable = GameState->GetHeroList();
	if (validate(IsValid(HeroListTable)) == false) { return; }

	TArray<FHeroListTableRow*> Heroes;
	HeroListTable->GetAllRows<FHeroListTableRow>(TEXT("GENERAL"), Heroes);

	for (int i = 0; i < Heroes.Num(); i++) {
		if (HeroTemplate == Heroes[i]->Asset) {
			PlayerState->SetPortrait(i);
			return;
		}
	}
	validate(false);
	// TODO: FIx with actual architecture of getting heroes.
}

void AHeroSpawner::SpawnLastSelectedHero(AController* Controller) {
	if (validate(SelectedHeroMap.Contains(Controller)) == false) { return; }
	SpawnHero(Controller, SelectedHeroMap[Controller]);
}

FVector AHeroSpawner::GetCameraSpotLocation() {
	return CameraSpot + GetActorLocation();
}


FRotator AHeroSpawner::GetCameraSpotLookAtRotation() {
	return UKismetMathLibrary::FindLookAtRotation(CameraSpot, HeroPreviewLocation);
}


int AHeroSpawner::GetTeamIndex() {
	return TeamIndex;
}


void AHeroSpawner::OnEnterSpawnArea(AActor* OverlappedActor, AActor* OtherActor) {
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(OtherActor);
	if (IsValid(BaseCharacter) == false) { return; }
	
	APlayerController* OtherController = Cast<APlayerController>(BaseCharacter->GetController());
	if (IsValid(OtherController) == false) { return; }

	if (OtherController == LocalPlayerController) {
		OnPlayerEnter.Broadcast();
	}
}


void AHeroSpawner::OnExitSpawnArea(AActor* OverlappedActor, AActor* OtherActor) {
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(OtherActor);
	if (IsValid(BaseCharacter) == false) { return; }

	APlayerController* OtherController = Cast<APlayerController>(BaseCharacter->GetController());
	if (IsValid(OtherController) == false) { return; }

	if (OtherController == LocalPlayerController) {
		OnPlayerExit.Broadcast();
	}
}