// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroPickerItem.h"
#include "Engine/DataTable.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Engine/Texture2D.h"

#include "UI/HeroPickerMenu.h"
#include "BaseCharacter.h"
#include "CustomMacros.h"

bool UHeroPickerItem::Initialize() {
	bool bSuccess = Super::Initialize();
	if (bSuccess == false) { return false; }

	if (validate(IsValid(Icon)) == false) { return false; }
	if (validate(IsValid(SelectButton)) == false) { return false; }
	
	SelectButton->OnClicked.AddDynamic(this, &UHeroPickerItem::Select);

	return true;
}


void UHeroPickerItem::Setup(UHeroPickerMenu* HeroPickerMenu, TSubclassOf<ABaseCharacter> HeroAsset, UTexture2D* HeroImage) {
	if (validate(IsValid(HeroPickerMenu)) == false) { return; }
	this->HeroPickerMenu = HeroPickerMenu;
	if (validate(IsValid(HeroAsset)) == false) { return; }
	HeroTemplate = HeroAsset;
	if (validate(IsValid(HeroImage)) == false) { return; }
	Icon->SetBrushFromTexture(HeroImage);
}


void UHeroPickerItem::Select() {
	if (validate(IsValid(HeroPickerMenu)) == false) { return; }
	UE_LOG(LogTemp, Warning, TEXT("SelectItem"))
	HeroPickerMenu->SetSelectedHero(HeroTemplate);
}