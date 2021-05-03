// Fill out your copyright notice in the Description page of Project Settings.


#include "Logger.h"

#include "DrawDebugHelpers.h"
#include "EnvironmentQuery/EnvQueryTypes.h"

void Logger::ToScreen(FString Message, float Duration, FColor Color, int32 Key)
{
	GEngine->AddOnScreenDebugMessage(Key, Duration, Color, Message);
}

void Logger::ToScreen(FString Prefix, float Value, float Duration, FColor Color, int32 Key)
{
	GEngine->AddOnScreenDebugMessage(Key, Duration, Color, FString::Printf(TEXT("%s %f"), *Prefix, Value));
}

void Logger::ToScreen(FString Prefix, bool Value, float Duration, FColor Color, int32 Key)
{
	GEngine->AddOnScreenDebugMessage(Key, Duration, Color, FString::Printf(TEXT("%s %s"), *Prefix, *FString(Value ? "True" : "False")));
}

void Logger::DrawLine(const UWorld* InWorld, FVector const& LineStart, FVector const& LineEnd, float Duration, FColor const& Color)
{
	DrawDebugLine(InWorld, LineStart, LineEnd, Color, false, Duration);
}
