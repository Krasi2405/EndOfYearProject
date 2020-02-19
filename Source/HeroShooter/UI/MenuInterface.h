// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MenuInterface.generated.h"


USTRUCT()
struct FCustomServerSettings {
	GENERATED_BODY()

	FString ServerName = "Undefined";
	int MaxPlayerCount = 6;
	FString Password;
};


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMenuInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class HEROSHOOTER_API IMenuInterface
{
	GENERATED_BODY()

public:
	virtual void Host(FCustomServerSettings ServerSettings) = 0;

	virtual void FindSessions() = 0;

	virtual void JoinIP(FString IPAddress) = 0;

	virtual void JoinSession(uint32 Index) = 0;
};