// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SphereComponent.h"

#include "BaseCharacter.h"
#include "HealthComponent.h"
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

		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::AProjectile::OnOverlapBegin);
		SphereComponent->IgnoreActorWhenMoving(Owner, true);
		SphereComponent->IgnoreActorWhenMoving(Owner, true);
	}
}

void AProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (validate(IsValid(OtherActor)) == false) { return; }

	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(OtherActor);
	if (IsValid(BaseCharacter) && BaseCharacter->GetTeamIndex() == TeamIndex) { return; } // Don't hit people with same team.

	AProjectile* Projectile = Cast<AProjectile>(OtherActor);
	if (IsValid(Projectile)) { return; }
	// TODO: Actually fix using proper projectile collision channel.

	UHealthComponent* HealthComponent = OtherActor->FindComponentByClass<UHealthComponent>();
	if (IsValid(HealthComponent)) {
		HealthComponent->TakeDamage(Damage);
	}

	Destroy();
}


void AProjectile::SetTeamIndex(int Index) {
	TeamIndex = Index;
}