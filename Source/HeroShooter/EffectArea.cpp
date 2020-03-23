// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectArea.h"
#include "BaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "BaseCharacter.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "CustomMacros.h"

// Sets default values
AEffectArea::AEffectArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AEffectArea::BeginPlay()
{
	Super::BeginPlay();
	
	if (validate(IsValid(AreaOfEffect))) {
		AreaOfEffect->OnComponentBeginOverlap.AddDynamic(this, &AEffectArea::OnEnterArea);
		AreaOfEffect->OnComponentEndOverlap.AddDynamic(this, &AEffectArea::OnExitArea);
	}
}


void AEffectArea::OnEnterArea(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
) {
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(OtherActor);
	if (IsValid(BaseCharacter)) {
		UAbilitySystemComponent* AbilitySystemComponent = BaseCharacter->GetAbilitySystemComponent();
		if (validate(IsValid(AbilitySystemComponent)) == false) { return; }
		for (TSubclassOf<UGameplayEffect> Effect : AreaEffects) {
			if (validate(IsValid(Effect)) == false) { return; }
			AbilitySystemComponent->ApplyGameplayEffectToTarget(Effect.GetDefaultObject(), AbilitySystemComponent);
		}
		AffectedCharacters.AddUnique(BaseCharacter);
	}
}


void AEffectArea::OnExitArea(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
) {
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(OtherActor);
	if (IsValid(BaseCharacter)) {
		UAbilitySystemComponent* AbilitySystemComponent = BaseCharacter->GetAbilitySystemComponent();
		if (validate(IsValid(AbilitySystemComponent)) == false) { return; }
		for (TSubclassOf<UGameplayEffect> Effect : AreaEffects) {
			if (validate(IsValid(Effect)) == false) { return; }
			AbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(Effect, AbilitySystemComponent);
		}
	}
	AffectedCharacters.Remove(BaseCharacter);
}


void AEffectArea::SetAreaOfEffectCollision(UPrimitiveComponent* CollisionComponent) {
	AreaOfEffect = CollisionComponent;
}


void AEffectArea::SetDuration(float Duration) {

	FTimerManager& TimerManager = GetWorldTimerManager();
	FTimerHandle TimerHandle;
	TimerManager.SetTimer(
		TimerHandle,
		this,
		&AEffectArea::DestroyArea,
		Duration,
		false
	);
}


void AEffectArea::DestroyArea() {
	for (ABaseCharacter* Character : AffectedCharacters) {
		if (validate(IsValid(Character)) == false) { continue; }
		UAbilitySystemComponent* AbilitySystemComponent = Character->GetAbilitySystemComponent();
		if (validate(IsValid(AbilitySystemComponent)) == false) { continue; }

		for (TSubclassOf<UGameplayEffect> Effect : AreaEffects) {
			if (validate(IsValid(Effect)) == false) { return; }
			AbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(Effect, AbilitySystemComponent);
		}
	}
	Destroy();
}