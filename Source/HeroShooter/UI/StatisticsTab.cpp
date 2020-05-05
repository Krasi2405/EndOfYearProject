// Fill out your copyright notice in the Description page of Project Settings.


#include "StatisticsTab.h"
#include "Components/PanelWidget.h"
#include "HeroPlayerState.h"
#include "HeroPlayerController.h"
#include "GameModes/HeroShooterGameState.h"
#include "UI/StatisticsTabItem.h"
#include "CustomMacros.h"


bool UStatisticsTab::Initialize() {
	if(Super::Initialize() == false) {
		return false;
	}

	AHeroPlayerController* HeroPlayerController = Cast<AHeroPlayerController>(GetOwningPlayer());
	if (validate(IsValid(HeroPlayerController)) == false) { return false; }

	UWorld* World = GetWorld();
	if (validate(IsValid(World)) == false) { return false; }
	
	AHeroShooterGameState* GameState = Cast<AHeroShooterGameState>(World->GetGameState());
	if (validate(IsValid(GameState)) == false) { return false; }

	TArray<APlayerState*> PlayerArray = GameState->PlayerArray;
	for (APlayerState* PlayerState : PlayerArray) {
		AHeroPlayerState* HeroPlayerState = Cast<AHeroPlayerState>(PlayerState);
		if (validate(IsValid(HeroPlayerState)) == false) { return false; }
		AddPlayer(HeroPlayerState);
	}
	GameState->OnAddPlayer.AddDynamic(this, &UStatisticsTab::AddPlayer);
	GameState->OnRemovePlayer.AddDynamic(this, &UStatisticsTab::RemovePlayer);

	return true;
}

void UStatisticsTab::AddPlayer(AHeroPlayerState* HeroPlayerState) {
	if (validate(IsValid(HeroPlayerState)) == false) { return; }
	if (StateParentMap.Contains(HeroPlayerState)) { return; }
	HeroPlayerState->OnTeamChange.AddDynamic(this, &UStatisticsTab::HandleTeamChange);
	if (validate(HeroPlayerState->GetTeamIndex() != -1) == false) { return; }

	UStatisticsTabItem* TabItem = CreateWidget<UStatisticsTabItem>(GetOwningPlayer(), StatisticsTabItemTemplate);
	if (validate(IsValid(TabItem)) == false) { return; }
	StateParentMap.Add(HeroPlayerState, TabItem);

	TabItem->Setup(HeroPlayerState);
	if (HeroPlayerState->GetTeamIndex() == 0) {
		Team1TabItemPanel->AddChild(TabItem);
	}
	else if (HeroPlayerState->GetTeamIndex() == 1) 
	{
		Team2TabItemPanel->AddChild(TabItem);
	}
	else
	{
		validate(false);
	}

}



void UStatisticsTab::RemovePlayer(AHeroPlayerState* HeroPlayerState) {
	if (validate(IsValid(HeroPlayerState)) == false) { return; }
	UStatisticsTabItem* InfoWidget = nullptr;
	StateParentMap.RemoveAndCopyValue(HeroPlayerState, InfoWidget);
	if (validate(IsValid(InfoWidget)) == false) { return; }

	InfoWidget->RemoveFromParent();
}


void UStatisticsTab::HandleTeamChange(AHeroPlayerState* HeroPlayerState) {
	if (validate(IsValid(HeroPlayerState)) == false) { return; }
	UE_LOG(LogTemp, Warning, TEXT("TeamChange!"))
	RemovePlayer(HeroPlayerState);
	AddPlayer(HeroPlayerState);
}