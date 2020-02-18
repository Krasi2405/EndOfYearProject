// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeInfoWidget.h"
#include "CustomMacros.h"

void UGameModeInfoWidget::Setup(AHeroShooterGameState* GameState) {
	validate(false); // Should be overriden in child.
	RemoveFromViewport();
}