// Fill out your copyright notice in the Description page of Project Settings.


#include "CountdownTimer.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Components/TextBlock.h"

#include "CustomMacros.h"


void UCountdownTimer::SetTimer(int Seconds) {
	UWorld* World = GetWorld();
	if (validate(IsValid(World)) == false) { return; }

	FTimerManager& TimerManager = World->GetTimerManager();
	TimerManager.SetTimer(TimerHandle, this, &UCountdownTimer::CountDownByOne, 1.0f, true, 1.0f);

	Timer = Seconds;
	TimerText->Text = FText::FromString(FString::FromInt(Timer));
}


void UCountdownTimer::CountDownByOne() {
	if (Timer <= 0) {
		OnCountdownOver.Broadcast();
		TimerHandle.Invalidate();
	}
	Timer--;
	TimerText->Text = FText::FromString(FString::FromInt(Timer));
}

