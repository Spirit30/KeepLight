// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CircularSaw.h"
#include "GameFramework/Actor.h"
#include "CircularSawButton.generated.h"

UCLASS()
class BEAR_API ACircularSawButton : public AActor
{
	GENERATED_BODY()
	
	public:

	UPROPERTY(EditAnywhere)
	ACircularSaw* CircularSaw;

	UPROPERTY(EditAnywhere)
	AActor* SmokeEffectActor;
	
	UPROPERTY(EditAnywhere)
	TArray<AActor*> ActivatorActors;

	ACircularSawButton();

	void Deactivate();

	protected:

	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	virtual void Tick(float DeltaTime) override;

	private:

	bool IsActiveFlag;
};
