// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "FloatingContainer.generated.h"

UCLASS()
class BEAR_API AFloatingContainer : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	bool AutoPlay;
	
	UPROPERTY(EditAnywhere)
	float Speed = 1.0f;

	UPROPERTY(EditAnywhere)
	float ReachDistance = 10.0f;
	
	public:	

	AFloatingContainer();
	UFUNCTION(BlueprintCallable)
	void StartFloating();
	UFUNCTION(BlueprintCallable)
	void StopFloating();

	protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	private:

	USphereComponent* Sphere;
	
	FVector InitialLocation;
	FVector DestinationLocation;

	void PickRandomDestinationLocation();
};
