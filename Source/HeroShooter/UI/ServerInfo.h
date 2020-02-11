// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerInfo.generated.h"

USTRUCT()
struct FServerInfoSettings {
	GENERATED_BODY()

	int ServerIndex = -1;
	FString Name = TEXT("Undefined");
	FString Owner = TEXT("Undefined");
	int CurrentPlayerCount = 0;
	int MaxPlayerCount = 0;
	float Latency = 999;
};


class UTextBlock;
class UButton;
class UMainMenu;

/**
 * 
 */
UCLASS()
class HEROSHOOTER_API UServerInfo : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateServerInfo(FServerInfoSettings Info);

	void SetMainMenu(UMainMenu* MainMenu);

	void UnSelect();

	int GetRowIndex();

protected:

	virtual bool Initialize() override;

	void Select();

	UPROPERTY(EditDefaultsOnly)
	FLinearColor SelectedBackgroundColor = FLinearColor::Blue;

	FButtonStyle NormalButtonStyle;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* NameBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* OwnerBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerCountBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MaxPlayersBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* LatencyBlock;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;

	UFUNCTION()
	void OnClicked();

	UPROPERTY(EditDefaultsOnly)
	int MaxCharacterLimit = 20;

	UMainMenu* MainMenu;

	int ServerRowIndex = -1;

	// TODO: Separate in different class for re-use.
	UPROPERTY()
	float DoubleClickDelay = 0.5f;
	
	FTimerHandle DoubleClickHandle;
	
};