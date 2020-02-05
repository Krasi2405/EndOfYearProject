// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class HEROSHOOTER_API AProjectile : public AActor
{
	GENERATED_BODY()
	

public:
	// Sets default values for this actor's properties
	AProjectile();

	void SetTeamIndex(int TeamIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	UPROPERTY(VisibleDefaultsOnly)
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(VisibleDefaultsOnly)
	USphereComponent* SphereComponent;

	//Damage to deal on hit
	UPROPERTY(EditDefaultsOnly)
	float Damage = 30.0f;

	UPROPERTY(VisibleAnywhere)
	int TeamIndex;
};
