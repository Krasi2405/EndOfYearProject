// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_StopFiring.h"

#include "CustomMacros.h"
#include "BaseCharacter.h"



EBTNodeResult::Type UBTTask_StopFiring::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Super::ExecuteTask(OwnerComp, NodeMemory) != EBTNodeResult::Succeeded) {
		return EBTNodeResult::Failed;
	}

	AICharacter->StopFiring();

	return EBTNodeResult::Succeeded;
}
