// Fill out your copyright notice in the Description page of Project Settings.


#include "Healthbar.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

#include "CustomMacros.h"

void UHealthbar::Setup(int MaxHealth) {
	this->MaxHealth = MaxHealth;

	if (validate(IsValid(MaxHealthText)) == false) { return; }
	MaxHealthText->SetText(FText::FromString(FString::FromInt(MaxHealth)));
	Update(MaxHealth);
}


void UHealthbar::Update(int CurrentHealth) {
	if (validate(MaxHealth != 0) == false) { return; }
	if (validate(IsValid(HealthProgressBar)) == false) { return; }
	if (validate(IsValid(CurrentHealthText)) == false) { return; }

	HealthProgressBar->SetPercent(CurrentHealth / (float)MaxHealth);;
	CurrentHealthText->SetText(FText::FromString(FString::FromInt(CurrentHealth)));
}