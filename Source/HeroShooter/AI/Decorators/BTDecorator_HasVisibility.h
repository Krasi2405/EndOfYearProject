// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_HasVisibility.generated.h"

/**
 * 
 */
UCLASS()
class HEROSHOOTER_API UBTDecorator_HasVisibility : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
protected:
	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) override;

	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
