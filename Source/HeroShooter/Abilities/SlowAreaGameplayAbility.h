// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayAbility.h"
#include "SlowAreaGameplayAbility.generated.h"


class AEffectArea;

/**
 * 
 */
UCLASS()
class HEROSHOOTER_API USlowAreaGameplayAbility : public UActiveGameplayAbility
{
	GENERATED_BODY()

protected:

	virtual void ActivateAbility
    (
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData
    ) override;
    

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<AEffectArea> AreaEffectTemplate;

    UPROPERTY(EditDefaultsOnly)
    int MaxRange = 500;

    UPROPERTY(EditDefaultsOnly)
    float AreaDuration = 5.0f;
};
