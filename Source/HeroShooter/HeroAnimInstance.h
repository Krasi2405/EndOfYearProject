// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HeroAnimInstance.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReloadFinished);

/**
 * 
 */
UCLASS()
class HEROSHOOTER_API UHeroAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	void StartReload(float StartTime = 0.0f);

	void CancelReload();

	FOnReloadFinished OnReloadFinished;

protected:

	void NativeInitializeAnimation() override;

	void NativeUpdateAnimation(float DeltaTimeX) override;

	UFUNCTION(BlueprintCallable)
	void FinishReload();

	UFUNCTION(BlueprintImplementableEvent)
	void OnStartReload(float StartTime = 0.0f);

	UFUNCTION(BlueprintImplementableEvent)
	void OnCancelReload();

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool bReloading = false;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float Speed = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float MovementDirection = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float AimOffsetVertical;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float AimOffsetHorizontal;
};
