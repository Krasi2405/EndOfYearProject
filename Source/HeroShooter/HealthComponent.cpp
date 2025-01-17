// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "Engine/World.h"
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

void UHealthComponent::TakeDamage(float Damage, AController* LastDamagedByPlayer) {
	if (bDead) { return; }
	SetDamager(LastDamagedByPlayer);

	Health -= Damage;
	if (Health <= 0) {
		bDead = true;
		OnDeath.Broadcast();
	}
	OnHealthChanged.Broadcast(Health);
}


void UHealthComponent::SetHealth(float NewHealth) {
	if (Health > NewHealth) {
		TakeDamage(Health - NewHealth, LastDamagedByPlayer);
	}
	else if (Health < NewHealth) {
		Heal(NewHealth - Health);
	}
}


void UHealthComponent::SetDamager(AController* Damager) {
	this->LastDamagedByPlayer = Damager;

	UWorld* World = GetWorld();
	if (validate(IsValid(World)) == false) { return; }


	FTimerManager& TimerManager = World->GetTimerManager();
	TimerManager.ClearTimer(LastDamagedByTimerHandle);
	TimerManager.SetTimer(
		LastDamagedByTimerHandle,					// Handle associated with timer
		this,										// Object to call the method on
		&UHealthComponent::ClearLastDamagedBy,		// call method after timer expires
		LastDamagedByExpireTime,					// Time to wait before calling the method.
		false										// don't loop
	);

	UE_LOG(LogTemp, Warning, TEXT("Set Damager"));
}

void UHealthComponent::ClearLastDamagedBy() {
	UE_LOG(LogTemp, Warning, TEXT("Clear Damager"))
	LastDamagedByPlayer = nullptr;
}


void UHealthComponent::Heal(float HealingPower) {
	if (bDead) { return; }

	Health += HealingPower;
	if (Health > MaxHealth) {
		Health = MaxHealth;
	}
	OnHealthChanged.Broadcast(Health);
}


AController* UHealthComponent::GetLastDamagedBy() {
	return LastDamagedByPlayer;
}


float UHealthComponent::GetHealth() {
	return Health;
}


float UHealthComponent::GetMaxHealth() {
	return MaxHealth;
}


void UHealthComponent::OnRep_Health() {
	OnHealthChanged.Broadcast(Health);
	if (Health <= 0) {
		bDead = true;
		OnDeath.Broadcast();
	}
}


void UHealthComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, Health);
}