// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplodingProjectile.h"
#include "Engine/World.h"
#include "BaseCharacter.h"
#include "GameFramework/PlayerController.h"
#include "HealthComponent.h"
#include "CustomMacros.h"


void AExplodingProjectile::Destroyed() {
	Super::Destroyed();

	ABaseCharacter* Owner = Cast<ABaseCharacter>(GetOwner());
	if (validate(IsValid(Owner)) == false) { return; }

	UWorld* World = GetWorld();
	if (validate(IsValid(World)) == false) { return; }

	TArray<struct FHitResult> OutHits;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(Owner);
	World->SweepMultiByChannel
	(
		OutHits,
		GetActorLocation(),
		GetActorLocation(),
		GetActorRotation().Quaternion(),
		ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(Range),
		QueryParams
	);

	for (FHitResult HitResult : OutHits) {
		AActor* HitActor = HitResult.Actor.Get();
		if (validate(IsValid(HitActor)) == false) { return; }
		UE_LOG(LogTemp, Warning, TEXT("Hit %s with exploding projectile"), *HitActor->GetName());

		UHealthComponent* HealthComponent = HitActor->FindComponentByClass<UHealthComponent>();
		if (IsValid(HealthComponent) == false) { continue; }

		ABaseCharacter* HitCharacter = Cast<ABaseCharacter>(HitActor);
		if (IsValid(HitCharacter) && HitCharacter->GetTeamIndex() == Owner->GetTeamIndex()) {
			continue;
		}

		AController* OwnerController = Owner->GetController();
		if (validate(IsValid(OwnerController)) == false) { return; }

		HealthComponent->TakeDamage((float)Damage, OwnerController);
		return;
	}
}