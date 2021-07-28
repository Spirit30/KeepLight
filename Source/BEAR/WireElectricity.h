// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CableActor.h"
#include "Particles/Emitter.h"

#include "WireElectricity.generated.h"

UCLASS()
class BEAR_API AWireElectricity : public AActor
{
	GENERATED_BODY()
	
	public:

	UPROPERTY(EditAnywhere)
	TArray<ACableActor*> Cables;

	UPROPERTY(EditAnywhere)
	float Speed = 100.0f;

	UPROPERTY(EditAnywhere)
	float ReachDestinationDist = 10.0f;

	UPROPERTY(EditAnywhere)
	AEmitter* Emitter;

	UPROPERTY(EditAnywhere)
	USoundWave* Sound;
	
	UPROPERTY(EditAnywhere)
	AActor* Lantern;

	AWireElectricity();

	UFUNCTION(BlueprintCallable)
	TArray<ACableActor*> GetCables();
	
	UFUNCTION(BlueprintCallable)
	void ClearPath();

	UFUNCTION(BlueprintCallable)
	void SetPath(TArray<FVector> Points);

	UFUNCTION(BlueprintCallable)
	void AddPath(TArray<FVector> Points);

	UFUNCTION(BlueprintCallable)
	void SetDirection(int32 Direction);

	UFUNCTION(BlueprintCallable)
	void Init(FVector StartLocation);

	//STATE EVENTS

	UFUNCTION(BlueprintCallable)
	void OnBearEnterZoneIndex1();

	UFUNCTION(BlueprintCallable)
	void OnBearExitZoneIndex1();

	UFUNCTION(BlueprintCallable)
	void OnBearEnterZoneIndex2();

	UFUNCTION(BlueprintCallable)
	void OnBearExitZoneIndex2();

	UFUNCTION(BlueprintCallable)
	void Deactivate();

	protected:
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	private:

	UAudioComponent* AudioComponent;
	UStaticMeshComponent* LightOfLantern;

	TArray<FVector> Path;
	TArray<int32> PathZonesNum;
	int32 TargetPointIndex;
	int32 DirectionSign;
	int32 CurrentZoneIndex;

	int32 MinPointIndex;
	int32 MaxPointIndex;

	void Activate();
	void UpdatePath();
	FVector GetTargetPoint() const;
	void SetNextTargetPoint();
	int32 FindCurrentZoneIndex();
	
	//STATE EVENTS
	
	void OnThisEnterZoneIndex0();
	void OnThisEnterZoneIndex1();
};
