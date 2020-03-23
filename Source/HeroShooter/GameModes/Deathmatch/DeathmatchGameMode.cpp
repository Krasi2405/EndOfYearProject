// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathmatchGameMode.h"

#include "GameModes/Deathmatch/DeathmatchGameState.h"
#include "HeroPlayerController.h"
#include "HeroPlayerState.h"
#include "CustomMacros.h"

void ADeathmatchGameMode::HandleDeath(AController* PlayerController, AController* KillerController) {
	Super::HandleDeath(PlayerController, KillerController);

	ADeathmatchGameState* GameState = GetGameState<ADeathmatchGameState>();
	if (validate(IsValid(GameState)) == false) { return; }

	if (IsValid(KillerController)) {
		AHeroPlayerState* KillerPlayerState = KillerController->GetPlayerState<AHeroPlayerState>();
		if (validate(IsValid(KillerPlayerState)) == false) { return; }

		int KillerTeamIndex = KillerPlayerState->GetTeamIndex();
		if (validate(KillerTeamIndex != -1) == false) { return; }

		GameState->AddKill(KillerTeamIndex);
		if (GameState->GetKillCount(KillerTeamIndex) >= GameState->GetVictoryScore()) {
			TeamWin(KillerTeamIndex);
		}
	}
	else
	{
		if (validate(IsValid(PlayerController)) == false) { return; }

		AHeroPlayerState* PlayerState = PlayerController->GetPlayerState<AHeroPlayerState>();
		if (validate(IsValid(PlayerState)) == false) { return; }

		int TeamIndex = PlayerState->GetTeamIndex();

		GameState->RemoveKill(TeamIndex);
	}
}
