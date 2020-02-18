// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MapNameRow.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FMapNameRow : public FTableRowBase
{
	GENERATED_BODY()

public:

	FMapNameRow()
	{

	}


	UPROPERTY(EditAnywhere)
	FString MapName;
};
