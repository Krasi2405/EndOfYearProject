// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HeroPlayerController.generated.h"

class UIngameMenu;
class UChatBox;
class UHeroPickerMenu;
class AHeroSpawner;
class ABaseCharacter;

/**
 * 
 */
UCLASS()
class HEROSHOOTER_API AHeroPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AHeroPlayerController();

	UFUNCTION(Server, Reliable)
	void SendMessageRequest(const FString& Message);

	UFUNCTION(NetMulticast, Reliable)
	void ReceiveMessage(const FString& PlayerName, const FString& Message);

	void SetTeamIndex(int NewTeamIndex);

	int GetTeamIndex();

	void TeleportSpectatorToHeroPicker();

	AHeroSpawner* GetTeamSpawner();

protected:

	virtual void BeginPlay() override;

	// Team Index may be updated before or after begin play.
	// If updated after begin play the responsibility is given to begin play for calling TeamSetup function.
	bool bBeginPlayExecuted = false;


	UPROPERTY()
	AHeroSpawner* TeamSpawner;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UIngameMenu> IngameMenuClass;

	UPROPERTY()
	UIngameMenu* IngameMenu;


	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UChatBox> ChatBoxClass;

	UPROPERTY()
	UChatBox* ChatBox;


	UPROPERTY(EditDefaultsONly)
	TSubclassOf<UHeroPickerMenu> HeroPickerClass;

	UHeroPickerMenu* HeroPicker;

	UFUNCTION()
	void ChooseHero(TSubclassOf<ABaseCharacter> Hero);

	UFUNCTION(Server, Reliable)
	void ServerSpawnHero(TSubclassOf<ABaseCharacter> Hero);

	void ServerSpawnHero_Implementation(TSubclassOf<ABaseCharacter> Hero);


	void ActivateHeroPicker();

	void DeactivateHeroPicker();


	bool bOpenChat = false;

	
	void SendMessageRequest_Implementation(const FString& Message);

	void ReceiveMessage_Implementation(const FString& PlayerName, const FString& Message);


	virtual void SetupInputComponent() override;

	void SwitchIngameMenu();

	void DeactivateIngameMenu();

	void ActivateIngameMenu();

	bool bIngameMenuActive = false;

	void ToggleChat();

	void OpenChat();

	void CloseChat();

	UPROPERTY(ReplicatedUsing = OnRep_TeamIndex)
	int TeamIndex = -1;

	int LastTeamSetupIndex = -1;

	// Called after team index is changed on client and right away on server.
	void TeamSetup();

	UFUNCTION()
	void OnRep_TeamIndex();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	
};
