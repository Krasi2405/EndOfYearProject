// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HeroSpawner.generated.h"

class UDataTable;
class APlayerController;
class ABaseCharacter;
class ATriggerVolume;

// Meant to be locally used to 
UCLASS()
class HEROSHOOTER_API AHeroSpawner : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	AHeroSpawner();

	int GetTeamIndex();

	void Setup(APlayerController* PlayerController);

	void SpawnHeroPreview(TSubclassOf<ABaseCharacter> Hero);

	void SpawnHero(APlayerController* Player, TSubclassOf<ABaseCharacter> Hero);

	FVector GetCameraSpotLocation();

	FRotator GetCameraSpotLookAtRotation();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditInstanceOnly)
	int TeamIndex = -1;

	UPROPERTY(EditInstanceOnly, Meta = (MakeEditWidget = true))
	ATriggerVolume* SpawnLocation;

	UPROPERTY(EditInstanceOnly, Meta = (MakeEditWidget = true))
	FVector CameraSpot;

	UPROPERTY(EditInstanceOnly, Meta = (MakeEditWidget = true))
	FVector HeroPreviewLocation;

	UPROPERTY()
	APlayerController* LocalPlayerController;

	UPROPERTY()
	ABaseCharacter* HeroPreview;
		
};
