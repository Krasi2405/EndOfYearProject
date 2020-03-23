// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_IsFiring.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Classes/AIController.h"

#include "CustomMacros.h"
#include "BaseCharacter.h"

void UBTDecorator_IsFiring::OnGameplayTaskDeactivated(UGameplayTask& Task)
{

}

void UBTDecorator_IsFiring::OnGameplayTaskActivated(UGameplayTask& Task)
{

}

bool UBTDecorator_IsFiring::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (validate(IsValid(BlackboardComponent)) == false) return false;

	AAIController* Controller = OwnerComp.GetAIOwner();
	if (validate(IsValid(Controller)) == false) return false;

	ABaseCharacter* ControlledPawn = Cast<ABaseCharacter>(Controller->GetPawn());
	if (validate(IsValid(ControlledPawn)) == false) return false;


	// TODO Return whether gun is firing;
	return true;
}
