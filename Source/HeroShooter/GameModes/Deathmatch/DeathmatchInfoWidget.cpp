// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathmatchInfoWidget.h"
#include "DeathmatchTeamScore.h"
#include "DeathmatchGameState.h"
#include "CustomMacros.h"

void UDeathmatchInfoWidget::Setup(AHeroShooterGameState* GameState) {
	DeathmatchGameState = Cast<ADeathmatchGameState>(GameState);
	if (validate(IsValid(DeathmatchGameState)) == false) { return; }
	if (validate(DeathmatchGameState->GetTeamCount() == 2) == false) { return; }


	if (validate(IsValid(TeamScoreOne)) == false) { return; }
	if (validate(IsValid(TeamScoreTwo)) == false) { return; }
	TeamScoreOne->Setup(DeathmatchGameState->GetVictoryScore());
	TeamScoreTwo->Setup(DeathmatchGameState->GetVictoryScore());

	DeathmatchGameState->OnKillCountUpdated.AddDynamic(this, &UDeathmatchInfoWidget::UpdateKillCount);
}

void UDeathmatchInfoWidget::UpdateKillCount() {
	UE_LOG(LogTemp, Warning, TEXT("UpdateKillCount"))

	if (validate(IsValid(DeathmatchGameState)) == false) { return; }
	UpdateScore(0, DeathmatchGameState->GetKillCount(0));
	UpdateScore(1, DeathmatchGameState->GetKillCount(1));
}


void UDeathmatchInfoWidget::UpdateScore(int TeamIndex, int Score) {
	if (validate(IsValid(TeamScoreOne)) == false) { return; }
	if (validate(IsValid(TeamScoreTwo)) == false) { return; }

	UDeathmatchTeamScore* TeamScore;
	if (TeamIndex == 0) {
		TeamScore = TeamScoreOne;
	}
	else if (TeamIndex == 1)
	{
		TeamScore = TeamScoreTwo;
	}
	
	if (validate(IsValid(TeamScore)) == false) { return; }
	TeamScore->UpdateScore(Score);
}