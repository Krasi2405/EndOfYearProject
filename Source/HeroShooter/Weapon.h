// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"


DECLARE_MULTICAST_DELEGATE(FOutOfAmmo);

UCLASS(Abstract)
class HEROSHOOTER_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	bool HasMesh();

	UFUNCTION()
	virtual void PullTrigger();

	UFUNCTION()
	virtual void ReleaseTrigger();

	UFUNCTION()
	virtual void Reload();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void PreFire();

	
	UFUNCTION(meta=(ToolTip="Locally simulate firing."))
	virtual void Fire();

	// Sent from original client to server.
	UFUNCTION(Server, Reliable)
	void ServerFire(FVector Direction);

	virtual void ServerFire_Implementation(FVector Direction);
	virtual bool ServerFire_Validation(FVector Direction);

	FVector GetAimingReticleDirection();

	UPROPERTY(EditDefaultsOnly, Meta = (MakeEditWidget = true))
	FVector FiringPoint;

	UPROPERTY(EditDefaultsOnly)
	bool bHasMesh = true; 
	
	UPROPERTY(EditDefaultsOnly)
	float FireRate = 1;

	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Max ammo in clip"))
	int MaxAmmo = 30;

	UPROPERTY(VisibleInstanceOnly)
	int CurrentAmmo = 30;

	FTimerHandle TriggerTimerHandle;

	FOutOfAmmo OnOutOfAmmo;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
