// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroInfoWidget.h"
#include "Components/TextBlock.h"
#include "Healthbar.h"
#include "Weapon.h"
#include "HealthComponent.h"

#include "CustomMacros.h"


void UHeroInfoWidget::SetupAmmoBar(AWeapon* Weapon) {
	if (validate(IsValid(Weapon)) == false) { return; }

	int MaxAmmo = Weapon->GetMaxAmmo();
	MaxAmmoText->SetText(FText::FromString(FString::FromInt(MaxAmmo)));
	UpdateAmmoBar(MaxAmmo);

	Weapon->OnAmmoChanged.AddDynamic(this, &UHeroInfoWidget::UpdateAmmoBar);
}


void UHeroInfoWidget::SetupHealthBar(UHealthComponent* HealthComponent) {
	if (validate(IsValid(HealthComponent)) == false) { return; }

	int MaxHealth = HealthComponent->GetMaxHealth();
	Healthbar->Setup(MaxHealth);
	UpdateHealthbar(MaxHealth);

	HealthComponent->OnHealthChanged.AddDynamic(this, &UHeroInfoWidget::UpdateHealthbar);
}


void UHeroInfoWidget::UpdateHealthbar(int CurrentHealth) {
	Healthbar->Update(CurrentHealth);
}

void UHeroInfoWidget::UpdateAmmoBar(int CurrentAmmo) {
	CurrentAmmoText->SetText(FText::FromString(FString::FromInt(CurrentAmmo)));
}