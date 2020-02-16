// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "HeroTableRow.generated.h"

class ABaseCharacter;
class UTexture2D;

USTRUCT(BlueprintType)
struct FHeroListTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:

	FHeroListTableRow()
	{
		
	}


	UPROPERTY(EditAnywhere)
	TSubclassOf<ABaseCharacter> Asset;

	UPROPERTY(EditAnywhere)
	UTexture2D* Texture;
};
