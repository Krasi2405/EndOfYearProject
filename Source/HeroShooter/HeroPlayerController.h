// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HeroPlayerController.generated.h"

class UIngameMenu;
class UChatBox;
class UHeroPickerMenu;
class UGameModeInfoWidget;
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
	void ServerSendMessageRequest(const FString& Message);

	UFUNCTION(Client, Reliable)
	void ClientReceiveMessage(APlayerState* SendingPlayer, const FString& Message);

	void SetTeamIndex(int NewTeamIndex);

	int GetTeamIndex();

	void TeleportSpectatorToHeroPicker();

protected:

	
	virtual void BeginPlay() override;

	// Team Index may be updated before or after begin play.
	// If updated after begin play the responsibility is given to begin play for calling TeamSetup function.
	bool bBeginPlayExecuted = false;

	// Only called on client
	virtual void AcknowledgePossession(class APawn* Pawn) override;

	// Only called on server.
	virtual void OnPossess(class APawn* Pawn) override;

	virtual void SetupInputComponent() override;


	UFUNCTION()
	void ServerHandleDeath();

	UPROPERTY()
	AHeroSpawner* TeamSpawner;


	

	UFUNCTION()
	void ChooseHero(TSubclassOf<ABaseCharacter> Hero);

	UFUNCTION(Server, Reliable)
	void ServerSpawnHero(TSubclassOf<ABaseCharacter> Hero);

	void ServerSpawnHero_Implementation(TSubclassOf<ABaseCharacter> Hero);


	void ActivateHeroPicker();

	void DeactivateHeroPicker();


	void ToggleChat();

	bool bChatOpen = false;

	void ServerSendMessageRequest_Implementation(const FString& Message);

	void ClientReceiveMessage_Implementation(APlayerState* SendingPlayer, const FString& Message);


	void ToggleIngameMenu();

	bool bIngameMenuActive = false;


	UPROPERTY(ReplicatedUsing = OnRep_TeamIndex, VisibleAnywhere)
	int TeamIndex = -1;

	int LastTeamSetupIndex = -1;

	// Called after team index is changed on client and right away on server.
	void TeamSetup();

	UFUNCTION()
	void OnRep_TeamIndex();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UFUNCTION()
	void HandleWinCondition(int WinningTeamIndex);

};
