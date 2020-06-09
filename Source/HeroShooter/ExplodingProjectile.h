// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "ExplodingProjectile.generated.h"

/**
 * 
 */
UCLASS()
class HEROSHOOTER_API AExplodingProjectile : public AProjectile
{
	GENERATED_BODY()


	virtual void Destroyed() override;

	UPROPERTY(EditDefaultsOnly)
	float SplashDamage = 80;


	UPROPERTY(EditDefaultsOnly)
	float Range = 50;
};
