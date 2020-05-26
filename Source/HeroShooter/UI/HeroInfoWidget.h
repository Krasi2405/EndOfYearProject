// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HeroInfoWidget.generated.h"

class UProgressBar;
class UTextBlock;
class UHealthbar;


/**
 * 
 */
UCLASS()
class HEROSHOOTER_API UHeroInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetupAmmoBar(class AWeapon* Weapon);

	void SetupHealthBar(class UHealthComponent* Healthbar);

protected:
	
	UPROPERTY(meta = (BindWidget))
	UHealthbar* Healthbar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MaxAmmoText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentAmmoText;


	UFUNCTION()
	void UpdateHealthbar(int CurrentHealth);

	UFUNCTION()
	void UpdateAmmoBar(int CurrentAmmo);

};
