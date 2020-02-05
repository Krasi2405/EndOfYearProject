// Fill out your copyright notice in the Description page of Project Settings.


#include "HitscanWeapon.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

#include "CustomMacros.h"
#include "HealthComponent.h"
#include "BaseCharacter.h"

void AHitscanWeapon::Fire() {
}


void AHitscanWeapon::ServerFire_Implementation(FVector Direction) {
	Super::ServerFire_Implementation(Direction);

	UE_LOG(LogTemp, Warning, TEXT("%s: Hitscan ServerFire"), *GetName())

	UWorld* World = GetWorld();
	if (validate(IsValid(World)) == false) { return; }

	ABaseCharacter* Owner = Cast<ABaseCharacter>(GetOwner());
	if (validate(IsValid(Owner)) == false) { return; }

	APlayerController* PlayerController = Cast<APlayerController>(Owner->GetController());
	if (validate(IsValid(PlayerController)) == false) { return; }


	FVector Start = GetActorLocation() + FiringPoint;
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
		ECollisionChannel::ECC_GameTraceChannel1,
		QueryParams
	);

	int TeamIndex = Owner->GetTeamIndex();
	for (FHitResult HitResult : OutHits) {
		AActor* HitActor = HitResult.Actor.Get();
		if (validate(IsValid(HitActor)) == false) { return; }

		UHealthComponent* HealthComponent = HitActor->FindComponentByClass<UHealthComponent>();
		if (IsValid(HealthComponent) == false) { continue; }

		ABaseCharacter* Character = Cast<ABaseCharacter>(HitActor);
		if (IsValid(Character) && Character->GetTeamIndex() == TeamIndex) {
			continue;
		}
		HealthComponent->TakeDamage((float) Damage);
		return;
	}
}