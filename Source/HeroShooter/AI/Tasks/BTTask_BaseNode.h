// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_BaseNode.generated.h"

class ABaseCharacter;
class UBlackboardComponent;

/**
 * 
 */
UCLASS()
class HEROSHOOTER_API UBTTask_BaseNode : public UBTTaskNode
{
	GENERATED_BODY()
	
protected:
	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) override;

	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UBlackboardComponent* BlackboardComponent = nullptr;

	ABaseCharacter* AICharacter = nullptr;
	
	AAIController* AIController = nullptr;
};
