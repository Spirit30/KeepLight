// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class BEAR_API Logger
{
	public:

	static void ToScreen(FString Message, float Duration = 2, FColor Color = FColor::White, int32 Key = -1);
	static void ToScreen(FString Prefix, float Value, float Duration = 2, FColor Color = FColor::White, int32 Key = -1);
	static void ToScreen(FString Prefix, bool Value, float Duration = 2, FColor Color = FColor::White, int32 Key = -1);
	static void DrawLine(const UWorld* InWorld, FVector const& LineStart, FVector const& LineEnd, float Duration = 2, FColor const& Color = FColor::White);
};
