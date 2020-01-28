// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "MenuInterface.h"
#include "MainMenu.generated.h"

class UButton;
class UWidgetSwitcher;
class UEditableTextBox;
/**
 * 
 */
UCLASS()
class HEROSHOOTER_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetMenuInterface(IMenuInterface* MenuInterface);

protected:

	virtual bool Initialize() override;


private:
	UPROPERTY(meta = (BindWidget))
	UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
	UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinMenuButton;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinMenuBackButton;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinServerButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* IPTextBox;
	
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* MenuSwitcher;

	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void EnterJoinMenu();

	UFUNCTION()
	void EnterMainMenu();

	UFUNCTION()
	void QuitGame();

	IMenuInterface* MenuInterface;
};
