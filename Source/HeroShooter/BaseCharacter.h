// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"


class AWeapon;
class UHealthComponent;

UCLASS()
class HEROSHOOTER_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	void SetTeamIndex(int Index);

	int GetTeamIndex();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly)
	float MinControlPitch = -80.0f;
	
	UPROPERTY(EditDefaultsOnly)
	float MaxControlPitch = 90.0f;

	// Used to hide the head from the camera
	UPROPERTY(EditDefaultsOnly)
	FName HeadBoneName = FName("Head");

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWeapon> StartingWeaponTemplate = nullptr;

	UPROPERTY(Replicated, VisibleInstanceOnly)
	AWeapon* Weapon = nullptr;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void StartFiring();

	void StopFiring();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Instanced, Category = "Base Character")
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere)
	UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere)
	int TeamIndex = 0; // Used to identify team player is on.

};
