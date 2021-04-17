// Fill out your copyright notice in the Description page of Project Settings.


#include "Logger.h"

void Logger::ToScreen(FString Message, float Duration, FColor Color, int32 Key)
{
	GEngine->AddOnScreenDebugMessage(Key, Duration, Color, Message);
}

void Logger::ToScreen(FString Prefix, float Value, float Duration, FColor Color, int32 Key)
{
	GEngine->AddOnScreenDebugMessage(Key, Duration, Color, FString::Printf(TEXT("%s %f"), *Prefix, Value));
}
