// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayAbility.h"
#include "ShootProjectileGameplayAbility.generated.h"

class AProjectile;

/**
 * 
 */
UCLASS()
class HEROSHOOTER_API UShootProjectileGameplayAbility : public UActiveGameplayAbility
{
	GENERATED_BODY()


	virtual void ActivateAbility
	(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;


	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AProjectile> ProjectileTemplate;

	UPROPERTY(EditDefaultsOnly)
	float ForwardOffset = 50;
};
