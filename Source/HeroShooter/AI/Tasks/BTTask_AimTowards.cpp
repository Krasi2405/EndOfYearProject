// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_AimTowards.h"

#include "Kismet/KismetMathLibrary.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Classes/AIController.h"

#include "CustomMacros.h"
#include "BaseCharacter.h"


EBTNodeResult::Type UBTTask_AimTowards::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Super::ExecuteTask(OwnerComp, NodeMemory) != EBTNodeResult::Succeeded) {
		return EBTNodeResult::Failed;
	}

	AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(GetSelectedBlackboardKey()));
	if (validate(IsValid(TargetActor)) == false) return EBTNodeResult::Failed;
	
	FVector TargetLocation = TargetActor->GetActorLocation();

	ABaseCharacter* TargetCharacter = Cast<ABaseCharacter>(TargetActor);
	if (IsValid(TargetCharacter)) {
		TargetLocation += FVector(0, 0, TargetCharacter->BaseEyeHeight);
	}

	FVector StartLocation = AICharacter->GetActorLocation();

	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(
		StartLocation,
		TargetLocation
	);

	AIController->SetControlRotation(LookAtRotation);
	AICharacter->SetActorRotation(FRotator(0, LookAtRotation.Yaw, 0));

	return EBTNodeResult::Succeeded;
}
