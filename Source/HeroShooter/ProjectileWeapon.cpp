// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileWeapon.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

#include "BaseCharacter.h"
#include "Projectile.h"
#include "CustomMacros.h"

void AProjectileWeapon::Fire() {
	
}


void AProjectileWeapon::ServerFire_Implementation(FVector Direction) {
	Super::ServerFire_Implementation(Direction);

	UE_LOG(LogTemp, Warning, TEXT("%s: Projectile ServerFire"), *GetName())

	if (validate(IsValid(ProjectileTemplate)) == false) { return; }

	UWorld* World = GetWorld();
	if (validate(IsValid(World)) == false) { return; }

	ABaseCharacter* Owner = Cast<ABaseCharacter>(GetOwner());
	if (validate(IsValid(Owner)) == false) { return; }

	FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
	SpawnParameters.Owner = Owner;
	SpawnParameters.Instigator = Owner->Instigator;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector ProjectileSpawnLocation = GetActorLocation() + FiringPoint;;
	FRotator ProjectileSpawnRotation = Direction.Rotation();

	AProjectile* Projectile = World->SpawnActor<AProjectile>(
		ProjectileTemplate.Get(),
		ProjectileSpawnLocation,
		ProjectileSpawnRotation,
		SpawnParameters
	);

	validate(IsValid(Projectile));
}