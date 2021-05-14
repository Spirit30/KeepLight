// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class BEAR_API Util
{
	public:
	
	static TArray<UActorComponent*> GetComponentsByName(AActor* Actor, FString Name);
	static UActorComponent* GetComponentByName(AActor* Actor, FString Name);
	static UStaticMeshComponent* GetStaticMeshComponentByName(AActor* Actor, FString Name);
	static bool IsFloor(AActor* Actor);
};
