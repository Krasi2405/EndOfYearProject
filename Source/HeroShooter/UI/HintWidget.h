// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HintWidget.generated.h"


class UTextBlock;

/**
 * 
 */
UCLASS()
class HEROSHOOTER_API UHintWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void SetHint(FString HintText);


protected:

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Hint;


};
