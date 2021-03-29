// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "DynamicObject.generated.h"

UCLASS()
class BEAR_API ADynamicObject : public AActor
{
	GENERATED_BODY()
	
	public:

	// UPROPERTY(EditAnywhere)
	// FVector Gravity;
	//
	// UPROPERTY(EditAnywhere)
	// float CalmDownSquaredVelocity;
	//
	ADynamicObject();

	UStaticMeshComponent* GetStaticMeshComponent();

	protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UStaticMeshComponent* StaticMeshComponent;
};
