// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class FString;
template <typename RetType = void, class InnerType>
RetType GetLambdaValue(InnerType&& Inner, FString CallingFunc)
{
	return Inner(CallingFunc);
}


#define validate(Expression)                                                  \
	GetLambdaValue<bool>([&] (FString FuncName) {                             \
		if((Expression) == false) {                                           \
			UE_LOG(LogTemp, Error,                                            \
				TEXT("%s::%s:%d  ("#Expression") == false"),                  \
				*FString(StaticClass()->GetName()),                           \
				*FuncName,                                                    \
				__LINE__                                                      \
			);                                                                \
			return false;                                                     \
		}                                                                     \
		return true;                                                          \
	}, FString(__func__))
