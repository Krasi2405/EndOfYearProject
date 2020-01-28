// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UI/MenuInterface.h"
#include "MultiplayerGameInstance.generated.h"


class UMainMenu;
/**
 * 
 */
UCLASS()
class HEROSHOOTER_API UMultiplayerGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetMainMenu(UMainMenu* MainMenu);

protected:

	UFUNCTION(Exec)
	void Host() override;
	

	UFUNCTION(Exec)
	void Join(FString IPAddress) override;

	UPROPERTY(EditDefaultsOnly)
	FString GameMapName;


	UMainMenu* MainMenu;

};
