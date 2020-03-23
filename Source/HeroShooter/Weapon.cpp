// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "TimerManager.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Components/SceneComponent.h"

#include "CustomMacros.h"
#include "HealthComponent.h"
#include "BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/AudioComponent.h"


AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(FName("RootComponent")));

	FiringPoint = CreateDefaultSubobject<USceneComponent>(FName("FiringPoint"));

	FireParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(FName("FiringParticles"));


	FireAudioComponent = CreateDefaultSubobject<UAudioComponent>(FName("FireAudioComponent"));
	FireAudioComponent->bAutoActivate = false;

	ReloadAudioComponent = CreateDefaultSubobject<UAudioComponent>(FName("ReloadAudioComponent"));
	ReloadAudioComponent->bAutoActivate = false;

	OutOfAmmoAudioComponent = CreateDefaultSubobject<UAudioComponent>(FName("OutOfAmmoAudioComponent"));
	OutOfAmmoAudioComponent->bAutoActivate = false;
}


void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentAmmo = MaxAmmo;
	if (validate(IsValid(FireParticleSystemComponent)) == false) { return; }
	if (validate(IsValid(FireParticleSystemTemplate)) == false) { return; }
	FireParticleSystemComponent->SetTemplate(FireParticleSystemTemplate);

}


void AWeapon::PullTrigger() {
	bFiring = true;
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
		return;
	}
	if (HasAuthority() == false) {
		CurrentAmmo--; // ServerFire will take care of ammo if player is server as well.
	}

	LocalFire();
	ServerFire(GetAimingReticleDirection());

	OnAmmoChanged.Broadcast(CurrentAmmo);
}


void AWeapon::LocalFire() {
	UWorld* World = GetWorld();
	if (validate(IsValid(World)) == false) { return; }
	if (validate(IsValid(FireParticleSystemTemplate)) == false) { return; }

	FTransform FiringPointTransform = GetFiringPointGlobalTransform();
	UParticleSystemComponent* ParticleComponent = UGameplayStatics::SpawnEmitterAtLocation(World, FireParticleSystemTemplate, FiringPointTransform);
	if (validate(IsValid(ParticleComponent)) == false) { return; }
	ParticleComponent->SetWorldRotation(FiringPointTransform.GetRotation());

	if (validate(IsValid(FireAudioComponent)) == false) { return; }
	FireAudioComponent->Play();
}


void AWeapon::ServerFire_Implementation(FVector Direction) {
	CurrentAmmo--;
}

bool AWeapon::ServerFire_Validation(FVector Direction) {
	return CurrentAmmo > 0;
}


void AWeapon::ReleaseTrigger() {
	bFiring = false;
	GetWorldTimerManager().ClearTimer(TriggerTimerHandle);
}


void AWeapon::Reload() {
	CurrentAmmo = MaxAmmo;
	OnAmmoChanged.Broadcast(CurrentAmmo);
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

		FTransform FiringTransform = GetFiringPointGlobalTransform();

		if (validate(IsValid(FiringPoint)) == false) { return FVector(0, 0, 0); }
		FVector Start = FiringTransform.GetLocation();
		FVector DirectionUnit;
		float Length;
		(HitLocation - Start).ToDirectionAndLength(DirectionUnit, Length);

		return DirectionUnit;
	}
	else
	{
		return Owner->GetControlRotation().Vector();
	}
}

#if WITH_EDITOR
void AWeapon::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);

	CurrentAmmo = MaxAmmo;
}
#endif

FTransform AWeapon::GetFiringPointGlobalTransform() {
	FTransform Transform = GetActorTransform();
	
	if (validate(IsValid(FiringPoint)) == false) { return Transform; }

	FVector RotatedVector = GetActorRotation().RotateVector(FiringPoint->GetComponentLocation());
	Transform.SetLocation(RotatedVector + GetActorLocation());
	Transform.SetRotation(GetActorRotation().Quaternion());

	return Transform;
}


int AWeapon::GetMaxAmmo() {
	return MaxAmmo;
}


int AWeapon::GetCurrentAmmo() {
	return CurrentAmmo;
}



bool AWeapon::IsFiring() {
	return bFiring;
}