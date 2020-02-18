// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DeathmatchTeamScore.generated.h"

class UProgressBar;
class UTextBlock;

/**
 * 
 */
UCLASS()
class HEROSHOOTER_API UDeathmatchTeamScore : public UUserWidget
{
	GENERATED_BODY()

public:
	
	void Setup(int MaxScore);

	void UpdateScore(int NewScore);

protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ScoreProgressBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ScoreText;

	int MaxScore;

};
