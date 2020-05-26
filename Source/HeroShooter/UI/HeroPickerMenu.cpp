// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroPickerMenu.h"
#include "Engine/DataTable.h"
#include "Engine/World.h"
#include "Components/HorizontalBox.h"
#include "Components/Button.h"

#include "HeroTableRow.h"
#include "HeroPickerItem.h"
#include "CustomMacros.h"
#include "BaseCharacter.h"
#include "HeroPlayerController.h"
#include "HeroSpawner.h"
#include "HeroPlayerState.h"
#include "GameModes/HeroShooterGameState.h"


bool UHeroPickerMenu::Initialize() {
	if (Super::Initialize() == false) { return false; }

	if (validate(IsValid(SelectButton)) == false) { return false; }
	SelectButton->OnClicked.AddDynamic(this, &UHeroPickerMenu::SpawnSelected);
	
	if (validate(InitializeHeroList()) == false) { return false; }

	return true;
}


void UHeroPickerMenu::SpawnSelected() {
	// Player controller spawns the hero on the server.

	if (validate(IsValid(SelectedHero)) == false) { return; }
	OnHeroSelected.Broadcast(SelectedHero);
}


bool UHeroPickerMenu::InitializeHeroList() {
	UWorld* World = GetWorld();
	if (validate(IsValid(World)) == false) { return false; }
	AHeroShooterGameState* GameState = Cast<AHeroShooterGameState>(World->GetGameState());
	if (validate(IsValid(GameState)) == false) { return false; }
	UDataTable* HeroListTable = GameState->GetHeroList();
	if (validate(IsValid(HeroListTable)) == false) { return false; }

	TArray<FHeroListTableRow*> Heroes;
	HeroListTable->GetAllRows<FHeroListTableRow>(TEXT("GENERAL"), Heroes);

	if (validate(IsValid(HeroListBox)) == false) { return false; }
	if (validate(IsValid(HeroPickerItemTemplate)) == false) { return false; }
	for (FHeroListTableRow* HeroTableRow : Heroes) {
		UHeroPickerItem* HeroItem = CreateWidget<UHeroPickerItem>(this, HeroPickerItemTemplate);
		if (validate(IsValid(HeroItem)) == false) { return false; }
		HeroItem->Setup(this, HeroTableRow->Asset, HeroTableRow->Texture);
		HeroListBox->AddChild(HeroItem);
	}
	return true;
}


void UHeroPickerMenu::SetSelectedHero(TSubclassOf<ABaseCharacter> HeroTemplate) {
	SelectedHero = HeroTemplate;

	AHeroPlayerController* HeroController = Cast<AHeroPlayerController>(GetOwningPlayer());
	if (validate(IsValid(HeroController)) == false) { return; }

	AHeroSpawner* TeamSpawner = HeroController->GetAssociatedHeroSpawner();
	if (validate(IsValid(TeamSpawner)) == false) { return; }
	TeamSpawner->SpawnHeroPreview(HeroTemplate);
}