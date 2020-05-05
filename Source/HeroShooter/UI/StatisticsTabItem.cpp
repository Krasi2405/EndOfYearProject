// Fill out your copyright notice in the Description page of Project Settings.


#include "StatisticsTabItem.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "HeroPlayerState.h"



void UStatisticsTabItem::Setup(AHeroPlayerState* HeroPlayerState) {
	ConnectedPlayerState = HeroPlayerState;

	UpdateStats(HeroPlayerState->GetKillCount(), HeroPlayerState->GetDeathCount(), 0);
	UpdateUsername(HeroPlayerState->GetPlayerName());

	HeroPlayerState->OnStatsChange.AddDynamic(this, &UStatisticsTabItem::UpdateStats);
	HeroPlayerState->OnUsernameChange.AddDynamic(this, &UStatisticsTabItem::UpdateUsername);
	HeroPlayerState->OnPortraitChange.AddDynamic(this, &UStatisticsTabItem::UpdatePlayerPortrait);
}


void UStatisticsTabItem::UpdateStats(int Kills, int Deaths, int Assists) {
	KillText->SetText(FText::FromString(FString::FromInt(Kills)));
	DeathText->SetText(FText::FromString(FString::FromInt(Deaths)));
	AssistText->SetText(FText::FromString(FString::FromInt(Assists)));
}


void UStatisticsTabItem::UpdateUsername(FString Username) {
	UsernameText->SetText(FText::FromString(Username));
}


void UStatisticsTabItem::UpdatePlayerPortrait(UTexture2D* PortraitTexture) {
	Portrait->SetBrushFromTexture(PortraitTexture);
}