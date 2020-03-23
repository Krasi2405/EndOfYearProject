// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AttributeSetBase.generated.h"

/**
 * 
 */
UCLASS()
class HEROSHOOTER_API UAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()

public:

	void Setup();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeSetBase")
	FGameplayAttributeData Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeSetBase")
	FGameplayAttributeData MovementSpeed;

	void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	
};
