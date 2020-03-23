// Fill out your copyright notice in the Description page of Project Settings.


#include "HitscanWeapon.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

#include "HeroPlayerController.h"
#include "HeroPlayerState.h"
#include "CustomMacros.h"
#include "HealthComponent.h"
#include "BaseCharacter.h"


void AHitscanWeapon::ServerFire_Implementation(FVector Direction) {
	Super::ServerFire_Implementation(Direction);

	UWorld* World = GetWorld();
	if (validate(IsValid(World)) == false) { return; }

	ABaseCharacter* Owner = Cast<ABaseCharacter>(GetOwner());
	if (validate(IsValid(Owner)) == false) { return; }

	FTransform FiringTransform = GetFiringPointGlobalTransform();

	FVector Start = FiringTransform.GetLocation();
	FVector End = Start + Direction * MaxRange;
	DrawDebugLine(
		World,
		Start,
		End,
		FColor::Red,
		false,
		5.0f
	);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(Owner);

	TArray<FHitResult> OutHits;
	World->LineTraceMultiByChannel(
		OutHits, 
		Start,
		End,
		ECollisionChannel::ECC_GameTraceChannel1, // Custom WeaponTrace channel
		QueryParams
	);

	AController* OwnerController = Owner->GetController();
	if (validate(IsValid(OwnerController)) == false) { return; }

	AHeroPlayerState* HeroPlayerState = OwnerController->GetPlayerState<AHeroPlayerState>();
	if (validate(IsValid(HeroPlayerState)) == false) { return; }

	int TeamIndex = HeroPlayerState->GetTeamIndex();
	for (FHitResult HitResult : OutHits) {
		AActor* HitActor = HitResult.Actor.Get();
		if (validate(IsValid(HitActor)) == false) { return; }

		UHealthComponent* HealthComponent = HitActor->FindComponentByClass<UHealthComponent>();
		if (IsValid(HealthComponent) == false) { continue; }

		ABaseCharacter* Character = Cast<ABaseCharacter>(HitActor);
		if (IsValid(Character)) {
			AController* OtherController = Character->GetController();
			if (IsValid(OtherController) == false) { continue; }

			AHeroPlayerState* OtherHeroPlayerState = OwnerController->GetPlayerState<AHeroPlayerState>();
			if (validate(IsValid(OtherHeroPlayerState)) == false) { return; }

			if (OtherHeroPlayerState->GetTeamIndex() == TeamIndex) { continue; }
		}

		HealthComponent->TakeDamage((float) Damage, OwnerController);
		return;
	}
}