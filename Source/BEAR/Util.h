// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class BEAR_API Util
{
	public:
	
	static TArray<UActorComponent*> GetComponentsByName(AActor* Actor, TSubclassOf<UActorComponent> ComponentClass, FString Name);
	static UActorComponent* GetComponentByName(AActor* Actor, TSubclassOf<UActorComponent> ComponentClass, FString Name);
	static bool IsFloor(AActor* Actor);
};
