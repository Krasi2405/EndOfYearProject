// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_BaseNode.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "CustomMacros.h"
#include "BaseCharacter.h"

void UBTTask_BaseNode::OnGameplayTaskDeactivated(UGameplayTask& Task)
{

}

void UBTTask_BaseNode::OnGameplayTaskActivated(UGameplayTask& Task)
{

}

EBTNodeResult::Type UBTTask_BaseNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (validate(IsValid(BlackboardComponent)) == false) { return EBTNodeResult::Aborted; }
	AIController = OwnerComp.GetAIOwner();
	if (validate(IsValid(AIController)) == false) { return EBTNodeResult::Aborted; }

	AICharacter = Cast<ABaseCharacter>(AIController->GetPawn());
	if (validate(IsValid(AICharacter)) == false) { return EBTNodeResult::Aborted; }
	
	return EBTNodeResult::Succeeded;
}
