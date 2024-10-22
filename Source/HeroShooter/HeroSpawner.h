// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HeroSpawner.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerEnterSpawn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerExitSpawn);


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

	void DestroyPreview();

	void SpawnHero(AController* Player, TSubclassOf<ABaseCharacter> Hero);

	void SpawnLastSelectedHero(AController* Player);

	FVector GetCameraSpotLocation();

	FRotator GetCameraSpotLookAtRotation();

	FOnPlayerEnterSpawn OnPlayerEnter;

	FOnPlayerExitSpawn OnPlayerExit;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnEnterSpawnArea(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnExitSpawnArea(AActor* OverlappedActor, AActor* OtherActor);

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

	TMap<AController*, TSubclassOf<ABaseCharacter>> SelectedHeroMap;
};
