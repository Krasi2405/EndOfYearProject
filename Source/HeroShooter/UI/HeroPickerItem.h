// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HeroPickerItem.generated.h"

class ABaseCharacter;
class UHeroPickerMenu;
class UButton;
class UImage;
/**
 * 
 */
UCLASS()
class HEROSHOOTER_API UHeroPickerItem : public UUserWidget
{
	GENERATED_BODY()

public:

	void Setup(UHeroPickerMenu* HeroPickerMenu, TSubclassOf<ABaseCharacter> HeroAsset, UTexture2D* HeroImage);

protected:
	virtual bool Initialize() override;

	UFUNCTION()
	void Select();

	UHeroPickerMenu* HeroPickerMenu;

	TSubclassOf<ABaseCharacter> HeroTemplate;

	UPROPERTY(meta = (BindWidget))
	UButton* SelectButton;

	UPROPERTY(meta = (BindWidget))
	UImage* Icon;
};
