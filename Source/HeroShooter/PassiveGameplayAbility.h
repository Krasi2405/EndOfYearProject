// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CustomGameplayAbility.h"
#include "PassiveGameplayAbility.generated.h"


class UPassiveAbilityComponent;

/**
 * 
 */
UCLASS()
class HEROSHOOTER_API UPassiveGameplayAbility : public UCustomGameplayAbility
{
	GENERATED_BODY()

public:

	TSubclassOf<UPassiveAbilityComponent> GetPassiveAbilityComponentTemplate();

protected:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPassiveAbilityComponent> PassiveAbilityComponentTemplate;
	
};
