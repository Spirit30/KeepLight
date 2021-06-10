// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaterPump.generated.h"

UCLASS()
class BEAR_API AWaterPump : public AActor
{
	GENERATED_BODY()
	
public:	

	AWaterPump();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;
};
