// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Catapult.generated.h"

UCLASS()
class BEAR_API ACatapult : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	AActor* BEAR;

	UPROPERTY(EditAnywhere)
	float HitForce = 1000000.0f;

	UPROPERTY(EditAnywhere)
	float HitDot = 0.65f;
	
public:	

	ACatapult();

protected:

	virtual void BeginPlay() override;
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

public:	

	virtual void Tick(float DeltaTime) override;
};
