// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "HeroShooterGameMode.generated.h"

class AHeroPlayerController;

/**
 * 
 */
UCLASS()
class HEROSHOOTER_API AHeroShooterGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	TArray<AHeroPlayerController*> GetTeamPlayerList(int TeamIndex);

protected:
	
	UPROPERTY(EditDefaultsOnly)
	int TeamCount = 2;

	UPROPERTY(EditDefaultsOnly)
	int MaxPlayersInTeam = 3;

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

	TArray<TArray<AHeroPlayerController*>*> Teams = TArray<TArray<AHeroPlayerController*>*>();
	
	int GetTeamIndexWithLeastPlayers();

	UFUNCTION(Exec)
	void ChangePlayerTeam(int TeamIndex, int PlayerIndex, int NewTeam);
};
