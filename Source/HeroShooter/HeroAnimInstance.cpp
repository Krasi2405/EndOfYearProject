// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroAnimInstance.h"

#include "CustomUtilities.h"
#include "CustomMacros.h"
#include "BaseCharacter.h"



void UHeroAnimInstance::NativeInitializeAnimation() {
	Super::NativeInitializeAnimation();

}


void UHeroAnimInstance::NativeUpdateAnimation(float DeltaTimeX) {
	Super::NativeUpdateAnimation(DeltaTimeX);

	ABaseCharacter* Character = Cast<ABaseCharacter>(TryGetPawnOwner());
	if (validate(IsValid(Character)) == false) { return; }
	FVector Velocity = Character->GetVelocity();

	Speed = Velocity.Size();
	MovementDirection = (Velocity.Rotation() - Character->GetControlRotation()).Yaw;
	MovementDirection = FCustomUtilities::NormalizeFloatAsRotationValue(MovementDirection);


	FRotator RotationDelta = Character->GetControlRotation() - Character->GetActorRotation();
	RotationDelta = FCustomUtilities::GetStandardUnrealRotation(RotationDelta);

	AimOffsetVertical = RotationDelta.Pitch;
	// AimOffsetHorizontal = RotationDelta.Yaw;
}


void UHeroAnimInstance::FinishReload() {
	OnReloadFinished.Broadcast();
	bReloading = false;
}


void UHeroAnimInstance::StartReload(float StartTime) {
	if (bReloading == false) {
		OnStartReload(StartTime);
		bReloading = true;
	}
	else
	{
		OnCancelReload();
		OnStartReload(StartTime);
	}
}


void UHeroAnimInstance::CancelReload() {
	if (bReloading) {
		OnCancelReload();
		bReloading = false;
	}
}
