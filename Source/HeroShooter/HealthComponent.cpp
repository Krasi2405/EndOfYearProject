// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "CustomMacros.h"


UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bReplicates = true;
}

#if WITH_EDITOR
void UHealthComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);
	Health = MaxHealth;
}
#endif

void UHealthComponent::BeginPlay() {
	Super::BeginPlay();

	validate(MaxHealth > 0);
}

void UHealthComponent::TakeDamage(float Damage) {
	if (bDead) { return; }

	Health -= Damage;

	if (Health <= 0) {
		bDead = true;
		OnDeath.Broadcast();
	}
	OnHealthChanged.Broadcast(Health);
}


void UHealthComponent::Heal(float HealingPower) {
	if (bDead) { return; }

	Health += HealingPower;
	if (Health > MaxHealth) {
		Health = MaxHealth;
	}
	OnHealthChanged.Broadcast(Health);
}


float UHealthComponent::GetHealth() {
	return Health;
}

float UHealthComponent::GetMaxHealth() {
	return MaxHealth;
}


void UHealthComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, Health);
}