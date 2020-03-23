// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_HasVisibility.h"


#include "BehaviorTree/BlackboardComponent.h"
#include "Classes/AIController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

#include "BaseCharacter.h"
#include "CustomMacros.h"

void UBTDecorator_HasVisibility::OnGameplayTaskDeactivated(UGameplayTask& Task)
{

}

void UBTDecorator_HasVisibility::OnGameplayTaskActivated(UGameplayTask& Task)
{

}

bool UBTDecorator_HasVisibility::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (validate(IsValid(BlackboardComponent)) == false) return EBTNodeResult::Failed;

	AAIController* Controller = OwnerComp.GetAIOwner();
	if (validate(IsValid(Controller)) == false) return EBTNodeResult::Failed;

	AActor* ControlledPawn = Controller->GetPawn();
	if (validate(IsValid(ControlledPawn)) == false) return EBTNodeResult::Failed;
	FVector StartLocation = ControlledPawn->GetActorLocation();

	AActor* Target = Cast<AActor>(BlackboardComponent->GetValueAsObject(GetSelectedBlackboardKey()));
	if(validate(IsValid(Target)) == false) return EBTNodeResult::Failed;
	FVector EndLocation = Target->GetActorLocation();
	ABaseCharacter* TargetCharacter = Cast<ABaseCharacter>(Target);
	if (IsValid(TargetCharacter)) {
		EndLocation += FVector(0, 0, TargetCharacter->BaseEyeHeight);
	}

	UWorld* World = GetWorld();
	if (validate(IsValid(World)) == false) return EBTNodeResult::Failed;

	DrawDebugLine(World, StartLocation, EndLocation, FColor::Red, false, 1.0f);
	
	FHitResult HitResult;
	if (World->LineTraceSingleByChannel(HitResult,StartLocation, EndLocation, ECC_Visibility)) {
		return EBTNodeResult::Failed;
	}
	return EBTNodeResult::Succeeded;
}
