// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatBox.generated.h"

class UScrollBox;
class UEditableTextBox;
class UChatText;
class PlayerController;

/**
 * 
 */
UCLASS()
class HEROSHOOTER_API UChatBox : public UUserWidget
{
	GENERATED_BODY()

public:
	
	virtual bool Initialize() override;

	void SendMessage();

	void AddMessage(FString PlayerName, FString Message);

	void Open();

	void Close();

	UFUNCTION(BlueprintCallable) // Native cpp delegate doesn't work so using blueprint events.
	void OnTextCommited(const FText& Text, ETextCommit::Type CommitMethod);

protected:

	UPROPERTY(meta = (BindWidget))
	UScrollBox* MessageList;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* MessageBox;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UChatText> ChatTextTemplate;
};
