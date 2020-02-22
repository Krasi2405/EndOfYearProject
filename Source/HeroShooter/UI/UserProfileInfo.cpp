// Fill out your copyright notice in the Description page of Project Settings.


#include "UserProfileInfo.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"

#include "MultiplayerGameInstance.h"
#include "CustomMacros.h"

bool UUserProfileInfo::Initialize() {
	bool bSuccess = Super::Initialize();
	if (validate(bSuccess) == false) { return false; }

	HideProfileMenu();

	if (validate(IsValid(ProfileMenuButton)) == false) { return false; }
	ProfileMenuButton->OnClicked.AddDynamic(this, &UUserProfileInfo::ShowProfileMenu);

	if (validate(IsValid(CancelButton)) == false) { return false; }
	CancelButton->OnClicked.AddDynamic(this, &UUserProfileInfo::HideProfileMenu);

	UMultiplayerGameInstance* GameInstance = GetGameInstance<UMultiplayerGameInstance>();
	if (validate(IsValid(GameInstance)) == false) { return false; }
	
	APlayerState* OwningPlayerState = GetOwningPlayerState();
	if (validate(IsValid(OwningPlayerState)) == false) { return false; }

	const TSharedPtr<const FUniqueNetId> NetId = GameInstance->GetUniqueID(OwningPlayerState);
	GameInstance->RequestUserInfo(NetId);
	GameInstance->OnUserInfoRequestCompleted.AddUObject(this, &UUserProfileInfo::OnUpdateUserInfo);

	return true;
}


void UUserProfileInfo::OnUpdateUserInfo(const FUniqueNetId& NetId, FUserInfo UserInfo) {
	UE_LOG(LogTemp, Warning, TEXT("OnUpdateUserInfo"));
	Update(UserInfo);
}


void UUserProfileInfo::Update(FUserInfo UserInfo) {
	if (validate(IsValid(Rating)) == false) { return; }
	if (validate(IsValid(WinsCount)) == false) { return; }
	if (validate(IsValid(LossesCount)) == false) { return; }
	if (validate(IsValid(Username)) == false) { return; }

	Username->SetText(FText::FromString(UserInfo.Username));
	Rating->SetText(FText::FromString(FString::FromInt(UserInfo.Rating)));
	WinsCount->SetText(FText::FromString(FString::FromInt(UserInfo.Wins)));
	LossesCount->SetText(FText::FromString(FString::FromInt(UserInfo.Losses)));
}


void UUserProfileInfo::HideProfileMenu() {
	ProfileMenu->SetVisibility(ESlateVisibility::Hidden);
}


void UUserProfileInfo::ShowProfileMenu() {
	ProfileMenu->SetVisibility(ESlateVisibility::Visible);
}