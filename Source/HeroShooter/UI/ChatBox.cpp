// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatBox.h"
#include "UI/ChatText.h"
#include "Components/ScrollBox.h"
#include "Components/EditableTextBox.h"

#include "HeroPlayerController.h"
#include "CustomMacros.h"


bool UChatBox::Initialize() {
	bIsFocusable = true;

	return Super::Initialize();
}


void UChatBox::SendMessage() {
	FString Message = MessageBox->GetText().ToString();
	if (Message == "") { return; }
	AHeroPlayerController* PlayerController = Cast<AHeroPlayerController>(GetOwningPlayer());
	if (validate(IsValid(PlayerController)) == false) { return; }
	PlayerController->ServerSendMessageRequest(Message);
	MessageBox->SetText(FText::FromString(""));
}


void UChatBox::AddMessage(FString PlayerName, FString Message) {
	UChatText* ChatText = CreateWidget<UChatText>(this, ChatTextTemplate);
	ChatText->Update(PlayerName, Message);
	MessageList->AddChild(ChatText);
}


void UChatBox::Open() {
	MessageBox->SetFocus();
	MessageBox->SetKeyboardFocus(); 
	
	SetVisibility(ESlateVisibility::Visible);
}


void UChatBox::Close() {
	SetVisibility(ESlateVisibility::Hidden);
}



void UChatBox::OnTextCommited(const FText& Text, ETextCommit::Type CommitMethod) {
	if (CommitMethod == ETextCommit::OnEnter) {
		SendMessage();
		Close();
		APlayerController* Owner = GetOwningPlayer();
		if (validate(IsValid(Owner)) == false) { return; }
		Owner->SetInputMode(FInputModeGameOnly());
	}
}
