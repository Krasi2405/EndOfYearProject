// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "MenuInterface.h"
#include "MainMenu.generated.h"


class UButton;
class UWidgetSwitcher;
class UScrollBox;
class UServerInfo;
class UEditableTextBox;
class USpinBox;
class UCheckBox;

/**
 * 
 */
UCLASS()
class HEROSHOOTER_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetMenuInterface(IMenuInterface* MenuInterface);

	void AddServer(struct FServerInfoSettings ServerSettings);

	void ClearServerList();

	void SelectServer(UServerInfo* ServerInfo);

	UFUNCTION()
	void JoinSelectedServerListSession();

protected:

	virtual bool Initialize() override;

	UPROPERTY()
	UServerInfo* SelectedServerInfo = nullptr;

	UPROPERTY(meta = (BindWidget))
	UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
	UWidget* HostMenu;

	UPROPERTY(meta = (BindWidget))
	UButton* HostMenuButton;

	UPROPERTY(meta = (BindWidget))
	UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	UButton* HostMenuBackButton;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinMenuButton;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinMenuBackButton;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinServerButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;

	UPROPERTY(meta = (BindWidget))
	UButton* RefreshServerListButton;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* IPTextBox;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* ServerList;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* ServerNameTextBox;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* PasswordTextBox;

	UPROPERTY(meta = (BindWidget))
	USpinBox* PlayerCountSpinBox;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* SpawnBotsCheckBox;

	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void JoinIPServer();

	UFUNCTION()
	void EnterJoinMenu();

	UFUNCTION()
	void EnterHostMenu();

	UFUNCTION()
	void EnterMainMenu();

	UFUNCTION()
	void QuitGame();

	UFUNCTION()
	void FindSessions();

	IMenuInterface* MenuInterface;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UServerInfo> ServerInfoTemplate;
};
