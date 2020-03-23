// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class APlayerState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDie);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealthChange, int, Health);


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HEROSHOOTER_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealthComponent();

	APlayerController* GetLastDamagedBy();

	void SetHealth(float Health);

	void TakeDamage(float Damage, APlayerController* Damager);

	void Heal(float HealingPower);

	void SetDamager(APlayerController* Damager);

	float GetHealth();

	float GetMaxHealth();

	FDie OnDeath;

	FHealthChange OnHealthChanged;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	int MaxHealth;

	UPROPERTY(VisibleInstanceOnly, ReplicatedUsing=OnRep_Health)
	int Health;

	bool bDead = false;

	UFUNCTION()
	void OnRep_Health();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	UPROPERTY()
	APlayerController* LastDamagedByPlayer = nullptr;

	void ClearLastDamagedBy();

	UPROPERTY(EditDefaultsOnly)
	float LastDamagedByExpireTime = 5.0f;

	FTimerHandle LastDamagedByTimerHandle;
};
