// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroSpawner.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "Engine/TriggerVolume.h"

#include "HeroPlayerController.h"
#include "BaseCharacter.h"
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


void AHeroSpawner::SpawnHeroPreview(TSubclassOf<ABaseCharacter> HeroTemplate) {
	if (validate(IsValid(HeroTemplate)) == false) { return; }

	if (IsValid(HeroPreview)) {
		HeroPreview->Destroy();
	}

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
	if (validate(IsValid(Controller)) == false) { return; }

	Controller->Possess(Hero);
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
	UE_LOG(LogTemp, Warning, TEXT("Enter Spawn Area. Overlapped Actor: %s, Other Actor: %s"), 
		*OverlappedActor->GetName(), *OtherActor->GetName());
	
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