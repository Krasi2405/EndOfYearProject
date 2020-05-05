// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatisticsTabItem.generated.h"


class AHeroPlayerState;
class UTextBlock;
class UImage;

/**
 * 
 */
UCLASS()
class HEROSHOOTER_API UStatisticsTabItem : public UUserWidget
{
	GENERATED_BODY()

public:

	void Setup(AHeroPlayerState* HeroPlayerState);


protected:

	UFUNCTION()
	void UpdateStats(int Kills, int Deaths, int Assists);

	UFUNCTION()
	void UpdateUsername(FString Username);

	UFUNCTION()
	void UpdatePlayerPortrait(UTexture2D* Portrait);

	UPROPERTY()
	AHeroPlayerState* ConnectedPlayerState;

	UPROPERTY(meta = (BindWidget))
	UImage* Portrait;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* KillText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DeathText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AssistText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* UsernameText;

};
