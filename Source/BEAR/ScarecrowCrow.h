// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScarecrowCrow.generated.h"

UCLASS()
class BEAR_API AScarecrowCrow : public AActor
{
	GENERATED_BODY()
	
	public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool IsAttackAnimation;
	
	AScarecrowCrow();
};
