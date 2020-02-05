// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "HitscanWeapon.generated.h"

/**
 * 
 */
UCLASS()
class HEROSHOOTER_API AHitscanWeapon : public AWeapon
{
	GENERATED_BODY()

protected:
	virtual void Fire() override;

	virtual void ServerFire_Implementation(FVector Direction) override;

	UPROPERTY(EditDefaultsOnly)
	int Damage = 20;

	UPROPERTY(EditDefaultsOnly)
	int MaxRange = 1500;
};
