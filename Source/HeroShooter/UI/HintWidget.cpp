// Fill out your copyright notice in the Description page of Project Settings.


#include "HintWidget.h"
#include "Components/TextBlock.h"
#include "CustomMacros.h"

void UHintWidget::SetHint(FString HintText) {
	if (validate(IsValid(Hint)) == false) { return; }
	Hint->Text = FText::FromString(HintText);
}