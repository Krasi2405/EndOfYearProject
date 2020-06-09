// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootProjectileGameplayAbility.h"

#include "BaseCharacter.h"
#include "Projectile.h"
#include "CustomMacros.h"
#include "HeroPlayerController.h"
#include "HeroPlayerState.h"

void UShootProjectileGameplayAbility::ActivateAbility
(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
) {
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UWorld* World = GetWorld();
	if (validate(IsValid(World)) == false) { return; }

	ABaseCharacter* Owner = Cast<ABaseCharacter>(ActorInfo->OwnerActor);
	if (validate(IsValid(Owner)) == false) { return; }
	if (validate(IsValid(Owner->Instigator)) == false) { return; }

	AHeroPlayerController* OwnerController = Cast<AHeroPlayerController>(ActorInfo->PlayerController);
	if (validate(IsValid(OwnerController)) == false) { return; }

	FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
	SpawnParameters.Owner = Owner;
	SpawnParameters.Instigator = Owner->Instigator;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;



	FRotator ProjectileSpawnRotation = OwnerController->GetControlRotation();
	FVector ProjectileSpawnLocation = Owner->GetActorLocation() + FVector(0, 0, Owner->BaseEyeHeight) + ProjectileSpawnRotation.Vector() * ForwardOffset;

	if (validate(IsValid(ProjectileTemplate)) == false) { return; }
	AProjectile* Projectile = World->SpawnActor<AProjectile>(
		ProjectileTemplate.Get(),
		ProjectileSpawnLocation,
		ProjectileSpawnRotation,
		SpawnParameters
	);

	if (validate(IsValid(Projectile)) == false) { return; }

	AHeroPlayerState* HeroPlayerState = OwnerController->GetPlayerState<AHeroPlayerState>();
	if (validate(IsValid(HeroPlayerState)) == false) { return; }

	Projectile->SetTeamIndex(HeroPlayerState->GetTeamIndex());
}