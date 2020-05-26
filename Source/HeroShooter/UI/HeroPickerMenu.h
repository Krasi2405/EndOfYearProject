// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HeroPickerMenu.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHeroSelected, TSubclassOf<ABaseCharacter>, Hero);

class UDataTable;
class UHeroPickerItem;
class UHorizontalBox;
class UButton;
class ABaseCharacter;
class AHeroSpawner;

/**
 * 
 */
UCLASS()
class HEROSHOOTER_API UHeroPickerMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetSelectedHero(TSubclassOf<ABaseCharacter> HeroTemplate);

	FOnHeroSelected OnHeroSelected;

protected:

	bool Initialize() override;

	bool InitializeHeroList();


	UFUNCTION()
	void SpawnSelected();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHeroPickerItem> HeroPickerItemTemplate;

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<ABaseCharacter> SelectedHero;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* HeroListBox;

	UPROPERTY()
	AHeroSpawner* TeamSpawner;

	UPROPERTY(meta = (BindWidget))
	UButton* SelectButton;
};
