// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Healthbar.generated.h"

class UTextBlock;
class UProgressBar;

/**
 * 
 */
UCLASS()
class HEROSHOOTER_API UHealthbar : public UUserWidget
{
	GENERATED_BODY()

public:

	void Setup(int MaxHealth);

	void Update(int CurrentHealth);

protected:

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthProgressBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MaxHealthText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentHealthText;

	int MaxHealth = 0;
};
