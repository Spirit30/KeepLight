// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "BEARCharacter.h"
#include "DraggableObject.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "TrafficLightSwing.generated.h"

UCLASS()
class BEAR_API ATrafficLightSwing : public AActor
{
	GENERATED_BODY()
	
	public:

	UPROPERTY(EditAnywhere)
	ABEARCharacter* Bear;

	UPROPERTY(EditAnywhere)
	ADraggableObject* TrafficLightDraggable;

	UPROPERTY(EditAnywhere)
	AActor* SwingAnchor;
	
	UPROPERTY(EditAnywhere)
	AActor* PhysicsConstraintActor;

	UPROPERTY(EditAnywhere)
	FRotator UpsideDown;

	UPROPERTY(EditAnywhere)
	float BaseForce = 10000.0f;
	
	UPROPERTY(EditAnywhere)
	UCurveFloat* AccelerateCurve;
	
	UPROPERTY(EditAnywhere)
	USoundWave* FallSound;
	
	ATrafficLightSwing();

	UFUNCTION(BlueprintCallable)
	bool GetIsBearSwing();

	UFUNCTION(BlueprintCallable)
	void ResetAcumulativeSwingForce();

	protected:

	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	virtual void Tick(float DeltaTime) override;

	private:

	UPhysicsConstraintComponent* PhysicsConstraint;

	FVector InitialLocation;
	FRotator InitialRotation;
	bool IsPhysicsActive;
	bool CanSwing;
	float InputTime;
	float PreviousInputValue;

	void SetPhysicsConstraintEnabled(bool flag);
};