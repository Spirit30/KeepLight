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

	UPROPERTY(EditAnywhere)
	USoundWave* HitSound;

	UPROPERTY(EditAnywhere)
	float HitSoundImpulseThreshold;
	
	ADynamicObject();

	UStaticMeshComponent* GetStaticMeshComponent();

	protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyHit(UPrimitiveComponent* Comp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	UStaticMeshComponent* StaticMeshComponent;
};
