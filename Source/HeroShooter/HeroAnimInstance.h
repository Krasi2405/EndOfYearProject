// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HeroAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class HEROSHOOTER_API UHeroAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:

	void NativeInitializeAnimation() override;

	void NativeUpdateAnimation(float DeltaTimeX) override;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float Speed = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float MovementDirection = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float AimOffsetVertical;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float AimOffsetHorizontal;
};
