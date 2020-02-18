// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileWeapon.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

#include "BaseCharacter.h"
#include "Projectile.h"
#include "CustomMacros.h"
#include "HeroPlayerController.h"

void AProjectileWeapon::Fire() {
	
}


void AProjectileWeapon::ServerFire_Implementation(FVector Direction) {
	Super::ServerFire_Implementation(Direction);

	UWorld* World = GetWorld();
	if (validate(IsValid(World)) == false) { return; }

	ABaseCharacter* Owner = Cast<ABaseCharacter>(GetOwner());
	if (validate(IsValid(Owner)) == false) { return; }
	if (validate(IsValid(Owner->Instigator)) == false) { return; }

	FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
	SpawnParameters.Owner = Owner;
	SpawnParameters.Instigator = Owner->Instigator;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	UE_LOG(LogTemp, Warning, TEXT("Instigator: %s"), *Owner->Instigator->GetName());

	FVector ProjectileSpawnLocation = GetActorLocation() + FiringPoint;
	FRotator ProjectileSpawnRotation = Direction.Rotation();

	if (validate(IsValid(ProjectileTemplate)) == false) { return; }
	AProjectile* Projectile = World->SpawnActor<AProjectile>(
		ProjectileTemplate.Get(),
		ProjectileSpawnLocation,
		ProjectileSpawnRotation,
		SpawnParameters
	);

	if (validate(IsValid(Projectile)) == false) { return; }

	AHeroPlayerController* OwnerController = Cast<AHeroPlayerController>(Owner->GetController());
	if (validate(IsValid(OwnerController)) == false) { return; }
	Projectile->SetTeamIndex(OwnerController->GetTeamIndex());
}