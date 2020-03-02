// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UI/MenuInterface.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/OnlineUserCloudInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "MultiplayerGameInstance.generated.h"

class UMainMenu;
class ALobbyGameMode;
class APlayerState;

USTRUCT()
struct FUserInfo {
	GENERATED_BODY()

	FString Username;

	UPROPERTY()
	int Rating;

	UPROPERTY()
	int Wins;

	UPROPERTY()
	int Losses;
};


DECLARE_MULTICAST_DELEGATE_TwoParams(
	FOnUserInfoRequestCompleted, const FUniqueNetId&, FUserInfo
);

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

	const TSharedPtr<const FUniqueNetId> GetUniqueID(APlayerState* PlayerState);

	UFUNCTION(Exec)
	void PrintLocalUniqueID();

	FString GetPlayerUsername(APlayerState* PlayerState);

protected:

	virtual void Init() override;

	IOnlineSubsystem* OnlineSubsystem;

	IOnlineIdentityPtr OnlineIdentityInterface;


/* ----------------------
		USER CLOUD
-------------------------*/

public:

	void RequestUserInfo(const TSharedPtr<const FUniqueNetId> NetId);

	void UpdateUserInfo(const TSharedPtr<const FUniqueNetId> NetId, FUserInfo UserInfo);

	FOnUserInfoRequestCompleted OnUserInfoRequestCompleted;


protected:

	IOnlineUserCloudPtr UserCloudInterface;

	void ReadUserFile(const TSharedPtr<const FUniqueNetId> NetId, const FString& Filename);

	void OnReadUserFileComplete(bool bSuccess, const FUniqueNetId& UserOwner, const FString& Filename);


	void WriteUserFile(const TSharedPtr<const FUniqueNetId> NetId, const FString& Filename, FString Contents);

	void OnWriteUserFileComplete(bool bSuccess, const FUniqueNetId& UserOwner, const FString& Filename);


	UFUNCTION(Exec)
	void WriteToFileTest(FString Contents);

	UFUNCTION(Exec)
	void PrintUserFile();

	UFUNCTION(Exec)
	void UpdateLocalUserInfo(int Rating, int Wins, int Losses);
	
	FString UserInfoToJSON(FUserInfo UserInfo);

	FUserInfo JSONToUserInfo(FString JSON);

/* ----------------------
	   SESSIONS
-------------------------*/

public:

	UFUNCTION()
	void FindSessions();

protected:

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

	IOnlineSessionPtr SessionInterface;

	UPROPERTY(EditDefaultsOnly)
	FString LobbyGamePath;

};
