// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatisticsTab.generated.h"

class UStatisticsTabItem;
class AHeroPlayerState;
class UPanelWidget;

/**
 * 
 */
UCLASS()
class HEROSHOOTER_API UStatisticsTab : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION()
	void AddPlayer(AHeroPlayerState* HeroPlayerState);

	UFUNCTION()
	void RemovePlayer(AHeroPlayerState* HeroPlayerState);

protected:

	virtual bool Initialize() override;

	UFUNCTION()
	void HandleTeamChange(AHeroPlayerState* HeroPlayerState);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UStatisticsTabItem> StatisticsTabItemTemplate;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* Team1TabItemPanel;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* Team2TabItemPanel;

	TMap<AHeroPlayerState*, UStatisticsTabItem*> StateParentMap;

};
