// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SphereComponent.h"

#include "HeroPlayerController.h"
#include "BaseCharacter.h"
#include "HealthComponent.h"
#include "HeroPlayerState.h"
#include "CustomMacros.h"

AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	SetReplicatingMovement(true);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile movement component"));
	ProjectileMovementComponent->ProjectileGravityScale = 0;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Capsule Component"));
	SetRootComponent(SphereComponent);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (validate(IsValid(SphereComponent))) {
		AActor* Owner = GetOwner();
		if (validate(IsValid(Owner)) == false) { return; }

		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);
		SphereComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
		SphereComponent->IgnoreActorWhenMoving(Owner, true);
	}
}

void AProjectile::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComp, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult)
{
	if (validate(IsValid(OtherActor)) == false) { return; }

	if (HasAuthority()) {
		ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(OtherActor);
		if (IsValid(BaseCharacter)) {
			AController* OtherController = BaseCharacter->GetController();
			if (IsValid(OtherController) == false) { return; }

			AHeroPlayerState* OtherHeroPlayerState = OtherController->GetPlayerState<AHeroPlayerState>();
			if (validate(IsValid(OtherHeroPlayerState)) == false) { return; }
			if (OtherHeroPlayerState->GetTeamIndex() == TeamIndex) { return; }
		}

		UHealthComponent* HealthComponent = OtherActor->FindComponentByClass<UHealthComponent>();
		if (IsValid(HealthComponent)) {
			HealthComponent->TakeDamage(Damage, GetInstigatorController());
		}
	}

	Destroy();
}


void AProjectile::OnHit(
	UPrimitiveComponent* HitComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit) 
{
	Destroy();
}

void AProjectile::SetTeamIndex(int Index) {
	TeamIndex = Index;
}