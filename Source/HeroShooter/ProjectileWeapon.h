// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "ProjectileWeapon.generated.h"


class AProjectile;

/**
 * 
 */
UCLASS()
class HEROSHOOTER_API AProjectileWeapon : public AWeapon
{
	GENERATED_BODY()

protected:
	virtual void LocalFire() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AProjectile> ProjectileTemplate;


	virtual void ServerFire_Implementation(FVector Direction) override;
};
