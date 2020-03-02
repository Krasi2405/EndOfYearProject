// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"


class AWeapon;
class UHealthComponent;
class UHeroAnimInstance;
class UWidgetComponent;
class UHealthbar;

UCLASS()
class HEROSHOOTER_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	AWeapon* GetEquippedWeapon();

	void HideHead();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Destroyed() override;

	UFUNCTION()
	void AttemptReload();

	// Send reload command to server 
	UFUNCTION(Server, Reliable)
	void ServerReload();

	void ServerReload_Implementation();

	// Simulate animation in other clients.
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastReload();

	void NetMulticastReload_Implementation();

	UFUNCTION()
	void CancelReload();

	// Cancel reload command to server 
	UFUNCTION(Server, Reliable)
	void ServerCancelReload();

	void ServerCancelReload_Implementation();

	// Cancel reload animation in other clients.
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastCancelReload();

	void NetMulticastCancelReload_Implementation();

	UFUNCTION(BlueprintCallable)
	void SetHealthWidgetComponent(UWidgetComponent* HealthWidgetComponent);

	UPROPERTY(VisibleDefaultsOnly)
	UWidgetComponent* HealthbarWidgetComponent;

	UPROPERTY()
	UHealthbar* ExternalHealthbar;

	UFUNCTION()
	void UpdateExternalHealthbar(int NewHealth);

	UPROPERTY(EditDefaultsOnly)
	float MinControlPitch = -80.0f;
	
	UPROPERTY(EditDefaultsOnly)
	float MaxControlPitch = 90.0f;

	// Used to hide the head from the camera
	UPROPERTY(EditDefaultsOnly)
	FName HeadBoneName = FName("Head");

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWeapon> StartingWeaponTemplate = nullptr;

	UPROPERTY(ReplicatedUsing = OnRep_WeaponSpawn, VisibleInstanceOnly)
	AWeapon* Weapon = nullptr;

	UFUNCTION()
	void OnRep_WeaponSpawn();

	void MoveForward(float Value);

	void MoveRight(float Value);

	void StartFiring();

	void StopFiring();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Instanced, Category = "Base Character")
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere)
	UHealthComponent* HealthComponent;

	UPROPERTY()
	UHeroAnimInstance* HeroAnimInstance;
};
