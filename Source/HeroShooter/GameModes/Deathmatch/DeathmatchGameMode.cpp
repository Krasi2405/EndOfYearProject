// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathmatchGameMode.h"

#include "GameModes/Deathmatch/DeathmatchGameState.h"
#include "HeroPlayerController.h"
#include "CustomMacros.h"

void ADeathmatchGameMode::HandleDeath(AHeroPlayerController* PlayerController, AHeroPlayerController* KillerController) {
	Super::HandleDeath(PlayerController, KillerController);

	ADeathmatchGameState* GameState = GetGameState<ADeathmatchGameState>();
	if (validate(IsValid(GameState)) == false) { return; }

	if (IsValid(KillerController)) {
		int KillerTeamIndex = KillerController->GetTeamIndex();
		if (validate(KillerTeamIndex != -1) == false) { return; }

		GameState->AddKill(KillerTeamIndex);
		if (GameState->GetKillCount(KillerTeamIndex) >= GameState->GetVictoryScore()) {
			TeamWin(KillerTeamIndex);
		}
	}
	else
	{
		if (validate(IsValid(PlayerController)) == false) { return; }
		GameState->RemoveKill(PlayerController->GetTeamIndex());
	}
}
