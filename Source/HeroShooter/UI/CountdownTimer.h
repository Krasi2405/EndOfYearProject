// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CountdownTimer.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCountdownOver);


class UTextBlock;

/**
 * 
 */
UCLASS()
class HEROSHOOTER_API UCountdownTimer : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetTimer(int Seconds);

	FCountdownOver OnCountdownOver;

protected:

	void CountDownByOne();
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TimerText;

	FTimerHandle TimerHandle;

	int Timer;

};
