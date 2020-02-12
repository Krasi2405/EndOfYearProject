// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HeroPlayerController.generated.h"

class UIngameMenu;
class UChatBox;

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

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UIngameMenu> IngameMenuClass;

	UPROPERTY()
	UIngameMenu* IngameMenu;


	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UChatBox> ChatBoxClass;

	UPROPERTY()
	UChatBox* ChatBox;

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
	
};
