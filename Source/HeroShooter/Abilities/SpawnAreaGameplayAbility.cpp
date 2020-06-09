// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnAreaGameplayAbility.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

#include "EffectArea.h"
#include "CustomMacros.h"
#include "BaseCharacter.h"

void USpawnAreaGameplayAbility::ActivateAbility
(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData
) 
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	

	bool bCommit = CommitAbility(Handle, ActorInfo, ActivationInfo);
	if (bCommit == false) { return; }

    ABaseCharacter* Character = Cast<ABaseCharacter>(ActorInfo->OwnerActor);
	if (validate(IsValid(Character)) == false) { return; }

    FRotator ControlRotation = Character->GetControlRotation();

	FVector Start = Character->GetActorLocation() + FVector(0, 0, Character->BaseEyeHeight);
	FVector End = Start + (ControlRotation.Vector() * MaxRange);

	UWorld* World = GetWorld();
	if (validate(IsValid(World)) == false) { return; }

	DrawDebugLine(
		World,
		Start,
		End,
		FColor::Red,
		false,
		5.0f
	);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Character);

	FHitResult OutHit;
	bool bHit = World->LineTraceSingleByChannel(
		OutHit,
		Start,
		End,
		ECollisionChannel::ECC_WorldStatic,
		QueryParams
	);

	FVector AreaSpawnLocation = End;
	if (bHit) {
		AreaSpawnLocation = OutHit.Location;
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	if (validate(IsValid(AreaEffectTemplate)) == false) { return; }
	AEffectArea* EffectArea = World->SpawnActor<AEffectArea>(
		AreaEffectTemplate,
		AreaSpawnLocation,
		ControlRotation,
		SpawnParameters
	);
	if (validate(IsValid(EffectArea)) == false) { return; };
	EffectArea->SetDuration(AreaDuration);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}