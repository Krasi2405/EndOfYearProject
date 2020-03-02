// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroInfoWidget.h"
#include "Components/TextBlock.h"
#include "Healthbar.h"

#include "CustomMacros.h"


void UHeroInfoWidget::Setup(int MaxAmmo, int MaxHealth) {
	if (validate(IsValid(Healthbar)) == false) { return; }
	Healthbar->Setup(MaxHealth);

	this->MaxAmmo = MaxAmmo;
	if (validate(IsValid(MaxAmmoText)) == false) { return; }
	MaxAmmoText->SetText(FText::FromString(FString::FromInt(MaxAmmo)));
	UpdateAmmoBar(MaxAmmo);
}


void UHeroInfoWidget::UpdateHealthbar(int CurrentHealth) {
	if (validate(IsValid(Healthbar)) == false) { return; }
	Healthbar->Update(CurrentHealth);
}

void UHeroInfoWidget::UpdateAmmoBar(int CurrentAmmo) {
	if (validate(IsValid(CurrentAmmoText)) == false) { return; }
	CurrentAmmoText->SetText(FText::FromString(FString::FromInt(CurrentAmmo)));
}