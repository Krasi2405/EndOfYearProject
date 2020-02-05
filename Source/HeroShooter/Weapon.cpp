// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "TimerManager.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

#include "CustomMacros.h"
#include "HealthComponent.h"
#include "BaseCharacter.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}


void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentAmmo = MaxAmmo;
	ENetRole NetRole = GetRemoteRole();
	ENetMode NetMode = GetNetMode();
	UE_LOG(LogTemp, Warning, TEXT("%s: NetMode: %d"), *GetName(), NetMode);
	UE_LOG(LogTemp, Warning, TEXT("%s: NetRole: %d"), *GetName(), NetRole);
}


void AWeapon::PullTrigger() {
	GetWorldTimerManager().SetTimer(
		TriggerTimerHandle,
		this,
		&AWeapon::PreFire,
		1 / FireRate,
		true,
		0.0f
	);
}


void AWeapon::PreFire() {
	if (CurrentAmmo <= 0) {
		OnOutOfAmmo.Broadcast();
		ReleaseTrigger();
		// TODO: Out of ammo sound.
		return;
	}
	// TODO: Screen shake, flash, sound, etc.
	if (HasAuthority() == false) {
		CurrentAmmo--; // ServerFire will take care of ammo if player is server as well.
	}
	Fire();

	
	ServerFire(GetAimingReticleDirection());
}


void AWeapon::Fire() {
	validate(false);
	// Workaround because pure virtual functions are not allowed in AActors.
}


void AWeapon::ServerFire_Implementation(FVector Direction) {
	CurrentAmmo--;
}

bool AWeapon::ServerFire_Validation(FVector Direction) {
	return CurrentAmmo > 0;
}


void AWeapon::ReleaseTrigger() {
	GetWorldTimerManager().ClearTimer(TriggerTimerHandle);
}


void AWeapon::Reload() {
	CurrentAmmo = MaxAmmo;
}


bool AWeapon::HasMesh() {
	return bHasMesh;
}


FVector AWeapon::GetAimingReticleDirection() {
	ABaseCharacter* Owner = Cast<ABaseCharacter>(GetOwner());
	if (validate(IsValid(Owner)) == false) { return FVector(0, 0, 0); }

	APlayerController* PlayerController = Cast<APlayerController>(Owner->GetController());
	if (validate(IsValid(PlayerController)) == false) { return FVector(0, 0, 0); }

	int Width;
	int Height;
	PlayerController->GetViewportSize(Width, Height);
	FHitResult HitResult;
	FVector HitLocation;
	if (PlayerController->GetHitResultAtScreenPosition(FVector2D(Width / 2, Height / 2), ECC_Visibility, false, HitResult)) {
		HitLocation = HitResult.Location;
	}
	else
	{
		validate(false);
		return FVector(0, 0, 0);
	}


	FVector Start = GetActorLocation() + FiringPoint;
	FVector DirectionUnit;
	float Length;
	(HitLocation - Start).ToDirectionAndLength(DirectionUnit, Length);

	return DirectionUnit;
}

#if WITH_EDITOR
void AWeapon::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);

	CurrentAmmo = MaxAmmo;
}
#endif