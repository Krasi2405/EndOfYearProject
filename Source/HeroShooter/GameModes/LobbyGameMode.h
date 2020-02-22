// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class HEROSHOOTER_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALobbyGameMode();

protected:

	virtual void Logout(AController* Exiting) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FString> GameMapPaths;

	UFUNCTION(Exec)
	void TravelToGame();

	UPROPERTY(EditDefaultsOnly)
	int MinPlayersForGame = 4;

	int PlayerCount = 0;

};
