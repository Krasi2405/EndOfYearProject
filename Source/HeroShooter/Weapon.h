// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOutOfAmmo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAmmoChanged, int, Ammo);

class UParticleSystem;
class UParticleSystemComponent;
class USceneComponent;
class UAudioComponent;
class AIngameHUD;


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

	int GetMaxAmmo();

	int GetCurrentAmmo();

	bool IsFiring();

	FOnAmmoChanged OnAmmoChanged;

	FOutOfAmmo OnOutOfAmmo;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTransform GetFiringPointGlobalTransform();

	UFUNCTION()
	void PreFire();

	UFUNCTION(meta=(ToolTip="Locally simulate firing."))
	virtual void LocalFire();

	// Sent from original client to server.
	UFUNCTION(Server, Reliable)
	void ServerFire(FVector Direction);

	virtual void ServerFire_Implementation(FVector Direction);
	virtual bool ServerFire_Validation(FVector Direction);

	FVector GetAimingReticleDirection();

	AIngameHUD* IngameHUD;

	UPROPERTY(VisibleDefaultsOnly)
	USceneComponent* FiringPoint;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* FireParticleSystemTemplate;

	UPROPERTY(VisibleDefaultsOnly)
	UParticleSystemComponent* FireParticleSystemComponent;

	UPROPERTY(VisibleDefaultsOnly)
	UAudioComponent* FireAudioComponent;

	UPROPERTY(VisibleDefaultsOnly)
	UAudioComponent* ReloadAudioComponent;

	UPROPERTY(VisibleDefaultsOnly)
	UAudioComponent* OutOfAmmoAudioComponent;

	UPROPERTY(EditDefaultsOnly)
	bool bHasMesh = true; 
	
	UPROPERTY(EditDefaultsOnly)
	float FireRate = 1;

	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Max ammo in clip"))
	int MaxAmmo = 30;

	UPROPERTY(VisibleInstanceOnly)
	int CurrentAmmo = 30;

	FTimerHandle TriggerTimerHandle;

	bool bFiring = false;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
