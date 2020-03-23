// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EffectArea.generated.h"

class UGameplayEffect;
class ABaseCharacter;

UCLASS()
class HEROSHOOTER_API AEffectArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEffectArea();

	UFUNCTION(BlueprintCallable)
	void SetAreaOfEffectCollision(UPrimitiveComponent* CollisionComponent);

	void SetDuration(float Duration);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayEffect>> AreaEffects;

	UPROPERTY()
	UPrimitiveComponent* AreaOfEffect;

	UPROPERTY()
	TArray<ABaseCharacter*> AffectedCharacters;

	UFUNCTION()
	virtual void OnEnterArea(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, 
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	virtual void OnExitArea(
		UPrimitiveComponent* OverlappedComp, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex
	);

	void DestroyArea();
};
