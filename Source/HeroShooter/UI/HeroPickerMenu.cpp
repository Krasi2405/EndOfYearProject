// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroPickerMenu.h"
#include "Engine/DataTable.h"
#include "Engine/World.h"
#include "Components/HorizontalBox.h"
#include "Components/Button.h"

#include "HeroTableRow.h"
#include "HeroPickerItem.h"
#include "CustomMacros.h"
#include "HeroShooterGameState.h"
#include "BaseCharacter.h"
#include "HeroPlayerController.h"
#include "HeroSpawner.h"


bool UHeroPickerMenu::Initialize() {
	bool bSuccess = Super::Initialize();
	if (validate(bSuccess) == false) { return false; }

	if (validate(IsValid(SelectButton)) == false) { return false; }
	SelectButton->OnClicked.AddDynamic(this, &UHeroPickerMenu::SpawnSelected);

	return true;
}


void UHeroPickerMenu::SpawnSelected() {
	if (validate(IsValid(SelectedHero)) == false) { return; }
	OnHeroSelected.Broadcast(SelectedHero);
}


void UHeroPickerMenu::Setup(AHeroSpawner* ConnectedSpawner) {
	if (validate(IsValid(ConnectedSpawner)) == false) { return; }
	TeamSpawner = ConnectedSpawner;

	validate(InitializeHeroList());
}


bool UHeroPickerMenu::InitializeHeroList() {
	UE_LOG(LogTemp, Warning, TEXT("InitializeHeroList!"))
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
	if (validate(IsValid(TeamSpawner)) == false) { return; }
	TeamSpawner->SpawnHeroPreview(HeroTemplate);
}