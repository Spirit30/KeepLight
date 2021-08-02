// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CableActor.h"
#include "GameFramework/Actor.h"
#include "Particles/Emitter.h"

#include "WireElectricityExcavator.generated.h"

UCLASS()
class BEAR_API AWireElectricityExcavator : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	ACableActor* Cable;

	UPROPERTY(EditAnywhere)
	float Speed = 100.0f;

	UPROPERTY(EditAnywhere)
	float ReachDestinationDist = 10.0f;

	UPROPERTY(EditAnywhere)
	AEmitter* Emitter;

	UPROPERTY(EditAnywhere)
	USoundWave* Sound;
	
	public:	

	AWireElectricityExcavator();

	UFUNCTION(BlueprintCallable)
	void Init(FVector StartLocation);

	UFUNCTION(BlueprintCallable)
	void Deactivate();

	protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	private:

	UAudioComponent* AudioComponent;

	TArray<FVector> Path;
	int32 TargetPointIndex;
	int32 DirectionSign;

	void Activate();
	void UpdatePath();
	FVector GetTargetPoint() const;
	void SetNextTargetPoint();
};
