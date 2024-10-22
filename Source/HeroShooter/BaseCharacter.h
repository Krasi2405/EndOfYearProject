// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Abilities/GameplayAbility.h"
#include "BaseCharacter.generated.h"


class AWeapon;
class UHealthComponent;
class UHeroAnimInstance;
class UWidgetComponent;
class UHealthbar;
class UCustomGameplayAbility;
class UActiveGameplayAbility;
class UPassiveGameplayAbility;
class UAttributeSetBase;
class UBehaviorTree;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponChange, AWeapon*, Weapon);

UCLASS()
class HEROSHOOTER_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	AWeapon* GetEquippedWeapon();

	int GetTeamIndex();

	void HideHead();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION()
	void ActivateAbility(int AbilityIndex);

	UFUNCTION(Server, Reliable)
	void ServerActivateAbility(int AbilityIndex);

	void ServerActivateAbility_Implementation(int AbilityIndex);

	UFUNCTION(BlueprintCallable)
	void AddGameplayTag(FGameplayTag Tag);

	UFUNCTION(BlueprintCallable)
	void RemoveGameplayTag(FGameplayTag Tag);

	UBehaviorTree* GetAIBehaviorTreeForCurrentGamemode();

	void StartFiring();

	void StopFiring();

	UFUNCTION()
	void AttemptReload();

	UFUNCTION()
	void CancelReload();

	FOnWeaponChange OnWeaponChange;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Destroyed() override;

	virtual void PossessedBy(AController* Controller) override;

	virtual void UnPossessed() override;

	// Send reload command to server 
	UFUNCTION(Server, Reliable)
	void ServerReload();

	void ServerReload_Implementation();

	// Simulate animation in other clients.
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastReload();

	void NetMulticastReload_Implementation();


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

	bool bReloading = false;

	UFUNCTION()
	void OnRep_WeaponSpawn();

	void MoveForward(float Value);

	void MoveRight(float Value);

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Instanced, Category = "Base Character")
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere)
	UHealthComponent* HealthComponent;

	UPROPERTY()
	UHeroAnimInstance* HeroAnimInstance;

	UFUNCTION()
	void Die();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UAbilitySystemComponent* AbilitySystemComponent;

	UFUNCTION(BlueprintCallable)
	void AcquireAbility(TSubclassOf<UCustomGameplayAbility> AbilityToAcquire);

	UFUNCTION(Client, Reliable)
	void ClientBindAbility(TSubclassOf<UCustomGameplayAbility> AbilityToAcquire, int AbilityIndex);

	void ClientBindAbility_Implementation(TSubclassOf<UCustomGameplayAbility> AbilityToAcquire, int AbilityIndex);

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UCustomGameplayAbility>> Abilities;

	UPROPERTY(VisibleAnywhere)
	TArray<TSubclassOf<UActiveGameplayAbility>> ActiveAbilities;

	UPROPERTY(VisibleAnywhere)
	TMap<TSubclassOf<UGameplayAbility>, bool> ActiveAbilitiesCooldownMap;

	UFUNCTION()
	void ClearCooldown(TSubclassOf<UActiveGameplayAbility> Ability);

	UPROPERTY(VisibleAnywhere)
	TArray<TSubclassOf<UPassiveGameplayAbility>> PassiveAbilities;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UAttributeSetBase* AttributeSet;

	UPROPERTY(EditDefaultsOnly)
	UBehaviorTree* BehaviourTree;

	UFUNCTION()
	void Reload();

	void SetupEquippedWeapon();
};
