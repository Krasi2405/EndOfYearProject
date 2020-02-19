// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UI/MenuInterface.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MultiplayerGameInstance.generated.h"

class UMainMenu;
class ALobbyGameMode;

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

	UFUNCTION()
	void FindSessions();

protected:

	virtual void Init() override;


	UFUNCTION(Exec)
	void Host(FCustomServerSettings ServerSettings) override;

	TOptional<FCustomServerSettings> CurrentServerSettings;

	UFUNCTION(Exec)
	void JoinIP(FString IPAddress) override;

	void JoinSession(uint32 Index) override;


	UMainMenu* MainMenu;

	UFUNCTION()
	void OnCreateSession(FName SessionName, bool bSuccessfull);

	UFUNCTION()
	void OnDestroySession(FName SessionName, bool bSuccessfull);

	UFUNCTION()
	void OnFindSessionsComplete(bool bSuccessfull);

	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	TSharedPtr<class FOnlineSessionSearch> SearchSettings;

	IOnlineSubsystem* OnlineSubsystem;

	IOnlineSessionPtr SessionInterface;

	UPROPERTY(EditDefaultsOnly)
	FString LobbyGamePath;

};
