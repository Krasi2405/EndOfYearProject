// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CustomGameplayAbility.h"
#include "ActiveGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class HEROSHOOTER_API UActiveGameplayAbility : public UCustomGameplayAbility
{
	GENERATED_BODY()
	
public:

	

	FName GetInputActionName() { return InputActionName; }

	EInputEvent GetActivationInputEvent() { return ActivationInputEvent; }

	UTexture2D* GetAbilityImage() { return AbilityImage; }

	float GetCooldownDuration() { return CooldownDuration; }

protected:

	UPROPERTY(EditDefaultsOnly)
	FName InputActionName;

	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<EInputEvent> ActivationInputEvent;
		
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* AbilityImage;

	UPROPERTY(EditDefaultsOnly)
	float CooldownDuration = 5;
};
