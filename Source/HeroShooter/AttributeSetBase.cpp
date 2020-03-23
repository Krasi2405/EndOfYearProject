// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSetBase.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffectTypes.h"
#include "GameplayEffect.h"	
#include "GameFramework/CharacterMovementComponent.h"

#include "HealthComponent.h"
#include "CustomMacros.h"
#include "BaseCharacter.h"


void UAttributeSetBase::Setup() {
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(GetOwningActor());
	if (validate(IsValid(BaseCharacter)) == false) { return; }

	UCharacterMovementComponent* MovementComponent = BaseCharacter->GetCharacterMovement();
	if (validate(IsValid(MovementComponent)) == false) { return; }

	MovementSpeed.SetBaseValue(MovementComponent->MaxWalkSpeed);
	MovementSpeed.SetCurrentValue(MovementSpeed.GetBaseValue());

	UHealthComponent* HealthComponent = BaseCharacter->FindComponentByClass<UHealthComponent>();
	if (validate(IsValid(HealthComponent)) == false) { return; }
	Health.SetBaseValue(HealthComponent->GetMaxHealth());
	Health.SetCurrentValue(Health.GetBaseValue());
}


void UAttributeSetBase::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) {
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(GetOwningActor());
	if (validate(IsValid(BaseCharacter)) == false) { return; }

	if (Attribute.AttributeName == "MovementSpeed") {
		UCharacterMovementComponent* MovementComponent = BaseCharacter->GetCharacterMovement();
		if (validate(IsValid(MovementComponent)) == false) { return; }
		MovementComponent->MaxWalkSpeed = NewValue;
		MovementComponent->MaxFlySpeed = NewValue;
		MovementComponent->MaxCustomMovementSpeed = NewValue;
	}
	else if (Attribute.AttributeName == "Health") 
	{
		UHealthComponent* HealthComponent = BaseCharacter->FindComponentByClass<UHealthComponent>();
		if (validate(IsValid(HealthComponent)) == false) { return; }
		HealthComponent->SetHealth(NewValue);
	}
}

void UAttributeSetBase::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) {
	Super::PostGameplayEffectExecute(Data);

	
}