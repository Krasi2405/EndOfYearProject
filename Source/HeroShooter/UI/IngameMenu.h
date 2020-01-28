// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IngameMenu.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class HEROSHOOTER_API UIngameMenu : public UUserWidget
{
	GENERATED_BODY()


protected:

	virtual bool Initialize() override;

	UFUNCTION()
	void LeaveServer();

	UPROPERTY(meta = (BindWidget))
	UButton* LeaveButton;

	UPROPERTY(EditDefaultsOnly)
	FString MainMenuName;
};
