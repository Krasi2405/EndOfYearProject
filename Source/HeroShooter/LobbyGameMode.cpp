// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "Engine/World.h"
#include "CustomMacros.h"

ALobbyGameMode::ALobbyGameMode() {
	bUseSeamlessTravel = true;
}


void ALobbyGameMode::PostLogin(APlayerController* NewPlayer) {
	Super::PostLogin(NewPlayer);

	PlayerCount++;
	if (PlayerCount >= MinPlayersForGame) {
		UWorld* World = GetWorld();
		if (validate(IsValid(World)) == false) { return; }

		FString MapPath = GameMapPaths[FMath::RandRange(0, GameMapPaths.Num() - 1)];

		World->ServerTravel(MapPath + "?listen", true);
	}
}

void ALobbyGameMode::Logout(AController* Exiting) {
	Super::Logout(Exiting);

	PlayerCount--;
}