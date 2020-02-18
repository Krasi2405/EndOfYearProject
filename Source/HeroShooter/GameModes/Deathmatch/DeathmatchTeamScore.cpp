// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathmatchTeamScore.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

#include "CustomMacros.h"

void UDeathmatchTeamScore::Setup(int MaxScore) {
	this->MaxScore = MaxScore;
}


void UDeathmatchTeamScore::UpdateScore(int NewScore) {
	ScoreText->SetText(FText::FromString(FString::FromInt(NewScore)));
	if (validate(MaxScore != 0) == false) { return; }
	ScoreProgressBar->SetPercent(NewScore / MaxScore);
}