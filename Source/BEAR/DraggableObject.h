// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DynamicObject.h"

#include "DraggableObject.generated.h"

/**
 * 
 */
UCLASS()
class BEAR_API ADraggableObject : public ADynamicObject
{
	GENERATED_BODY()

	public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinPushForce;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxPushForce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinPushVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxPushVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PullDistance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector RightHandDragOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator RightHandDragRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector LeftHandDragOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator LeftHandDragRotation;

	ADraggableObject();

	protected:
	
	virtual void Tick(float DeltaTime) override;
};
