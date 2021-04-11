// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "WindSystem.generated.h"

UCLASS()
class BEAR_API AWindSystem : public AActor
{
	GENERATED_BODY()
	
	public:

	UPROPERTY(EditAnywhere)
	USoundWave* Audio;

	UPROPERTY(EditAnywhere)
	float Volume;
	
	UPROPERTY(EditAnywhere)
	bool IsApplyWindForce;

	UPROPERTY(EditAnywhere)
	UMaterialParameterCollection * MaterialParameterCollection;
	
	UPROPERTY(EditAnywhere)
	FName WindForceParameterName;
	
	UPROPERTY(EditAnywhere)
	float MinWindForce;

	UPROPERTY(EditAnywhere)
	float MaxWindForce;

	UPROPERTY(EditAnywhere)
	float SmoothWindForceSpeed;

	UPROPERTY(EditAnywhere)
	FString AudioDescriptorData;
	
	AWindSystem();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	private:

	UMaterialParameterCollectionInstance* MaterialParameterCollectionInstance;
	TArray<float> AudioDescriptor;
	float CurrentPlayTime;
	float CurrentWindForce;

	void Play() const;
};
