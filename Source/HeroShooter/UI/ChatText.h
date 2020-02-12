// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatText.generated.h"

class UTextBlock;
class APlayerController;

/**
 * 
 */
UCLASS()
class HEROSHOOTER_API UChatText : public UUserWidget
{
	GENERATED_BODY()

public:

	void Update(FString& Name, FString& Message);


protected:

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerNameBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MessageBlock;
};
