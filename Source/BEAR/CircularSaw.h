// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StackObjectRemover.h"
#include "GameFramework/Actor.h"
#include "CircularSaw.generated.h"

UCLASS()
class BEAR_API ACircularSaw : public AActor
{
	GENERATED_BODY()
	
	public:

	UPROPERTY(EditAnywhere)
	AStackObjectRemover* WoodenDeckRemover;

	UPROPERTY(EditAnywhere)
	AActor* CollisionOfChildActorHolder;

	UPROPERTY(EditAnywhere)
	FVector ActiveOffset = FVector(0.0f, 0.0f, 90.0f);
	
	UPROPERTY(EditAnywhere)
	FRotator RotationSpeed = FRotator(300.0f, 0.0f, 0.0f);

	ACircularSaw();

	void Activate(bool flag);

	protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	private:

	UStaticMeshComponent* CollisionOfChildActor;
	
	bool IsActive;
};
