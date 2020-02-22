// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserProfileInfo.generated.h"

class UMultiplayerGameInstance;
class UButton;
class UImage;
class UUserWidget;
class UTextBlock;

/**
 * 
 */
UCLASS()
class HEROSHOOTER_API UUserProfileInfo : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void Update(struct FUserInfo UserInfo);

protected:

	virtual bool Initialize() override;

	void OnUpdateUserInfo(const FUniqueNetId& NetId, FUserInfo UserInfo);

	UPROPERTY(meta = (BindWidget))
	UWidget* ProfileMenu;

	UPROPERTY(meta = (BindWidget))
	UButton* ProfileMenuButton;

	UPROPERTY(meta = (BindWidget))
	UButton* CancelButton;

	UPROPERTY(meta = (BindWidget))
	UImage* ProfileImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Username;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Rating;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* WinsCount;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* LossesCount;

	UFUNCTION()
	void HideProfileMenu();

	UFUNCTION()
	void ShowProfileMenu();

};
