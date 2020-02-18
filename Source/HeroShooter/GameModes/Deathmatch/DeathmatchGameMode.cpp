// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathmatchGameMode.h"

#include "GameModes/Deathmatch/DeathmatchGameState.h"
#include "HeroPlayerController.h"
#include "CustomMacros.h"

void ADeathmatchGameMode::HandleDeath(AHeroPlayerController* PlayerController) {
	Super::HandleDeath(PlayerController);

	if (validate(IsValid(PlayerController)) == false) { return; }
	int TeamIndex = PlayerController->GetTeamIndex();

	ADeathmatchGameState* GameState = GetGameState<ADeathmatchGameState>();
	if (validate(IsValid(GameState)) == false) { return; }

	// Handle only for 2 teams
	if (TeamIndex == 0) {
		GameState->AddKill(1);
		if (GameState->GetKillCount(1) >= GameState->GetVictoryScore()) {
			TeamWin(1);
		}
	}
	else if (TeamIndex == 1) {
		GameState->AddKill(0);
		if (GameState->GetKillCount(0) >= GameState->GetVictoryScore()) {
			TeamWin(0);
		}
	}
	else
	{
		validate(false);
	}

}
