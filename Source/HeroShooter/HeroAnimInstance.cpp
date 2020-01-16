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