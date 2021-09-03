// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "BEARCharacter.h"
#include "DraggableObject.h"
#include "LevelSequenceActor.h"
#include "GameFramework/Actor.h"

#include "Catapult.generated.h"

UCLASS()
class BEAR_API ACatapult : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	ABEARCharacter* BEAR;
	
	UPROPERTY(EditAnywhere)
	ADraggableObject* Rock;

	UPROPERTY(EditAnywhere)
	AActor* Target;

	UPROPERTY(EditAnywhere)
	ALevelSequenceActor* CatapultSequence;
	
	UPROPERTY(EditAnywhere)
	float ConstructSpeed = 10.0f;

	UPROPERTY(EditAnywhere)
	float ConstructDist = 10.0f;
	
	UPROPERTY(EditAnywhere)
	float HitForce = 1000000.0f;

	UPROPERTY(EditAnywhere)
	float HitForceWithoutRockMesh = 100000.0f;

	UPROPERTY(EditAnywhere)
	float HitDot = 0.65f;

	UPROPERTY(EditAnywhere)
	float ShootSpeed = 100.0f;

	UPROPERTY(EditAnywhere)
	float ShootDist = 10.0f;

	UPROPERTY(EditAnywhere)
	bool AutoHide = true;

	UPROPERTY(EditAnywhere)
	USoundWave* ShootSound;

	UPROPERTY(EditAnywhere)
	USoundWave* FallingSound;

	UPROPERTY(EditAnywhere)
	float FallingSoundVolume = 0.25f;
	
	public:	

	ACatapult();

	UFUNCTION(BlueprintCallable)
	void Shoot();
	void Construct();

	protected:

	virtual void BeginPlay() override;
	//virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	virtual void Tick(float DeltaTime) override;

	private:

	UStaticMeshComponent* WoodenPlankMesh;
	UStaticMeshComponent* RockMesh;

	enum CatapultState
	{
		NotReady,
		Constructing,
		Ready,
		Shooting,
		Destroyed
	};

	CatapultState State;

	FTransform InitialWoodenPlankMeshTransform;
	FTransform InitialRockMeshTransform;

	void OnConstructing(float DeltaTime);
	void OnShooting(float DeltaTime);
	
};
