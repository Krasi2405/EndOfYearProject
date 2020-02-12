// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatText.h"
#include "Components/TextBlock.h"


void UChatText::Update(FString& Name, FString& Message) {
	PlayerNameBlock->SetText(FText::FromString(Name + ": "));
	MessageBlock->SetText(FText::FromString(Message));
}