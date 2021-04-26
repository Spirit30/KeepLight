// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "BEARCharacter.h"
#include "LevelSequenceActor.h"
#include "ScarecrowHat.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "ScarecrowCrow.generated.h"

UCLASS()
class BEAR_API AScarecrowCrow : public AActor
{
	GENERATED_BODY()
	
	public:

	UPROPERTY(EditAnywhere)
	ABEARCharacter* Bear;

	UPROPERTY(EditAnywhere)
	float MinSpeed;
	
	UPROPERTY(EditAnywhere)
	float SpeedDistCoef;

	UPROPERTY(EditAnywhere)
	float AttackDelay;

	UPROPERTY(EditAnywhere)
	AScarecrowHat* Hat;

	UPROPERTY(EditAnywhere)
	FVector HatOffset = FVector(0, 0, -75);

	UPROPERTY(EditAnywhere)
	TArray<float> SequenceTimePositions;

	UPROPERTY(EditAnywhere)
	float BackDistance = 10;

	UPROPERTY(EditAnywhere)
	USoundWave* CrowSound;

	UPROPERTY(EditAnywhere)
	USoundWave* HitSound;
	
	UPROPERTY(EditAnywhere)
	float SoundVolume = 1.0f;

	UPROPERTY(EditAnywhere)
	ALevelSequenceActor* HatSecuence;
	
	UPROPERTY(EditAnywhere)
	FString BearCollisionName;

	UPROPERTY(EditAnywhere)
	AActor* Death;

	UPROPERTY(EditAnywhere)
	FVector LeaveDirection = FVector(0, 1000, 1000);

	UPROPERTY(EditAnywhere)
	float DestroyZ = 2000;

	UPROPERTY(EditAnywhere)
	AActor* NextControlPoint;
	
	//Used by ScarecrowCrow Animation Blueprint
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool IsAttackAnimation;
	
	AScarecrowCrow();

	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void Tick(float DeltaSeconds) override;

	private:

	USphereComponent* BearCollision;
	
	FVector InitialLocation;
	int32 InitialBearCollisionRadius;
	
	float AttackTimer;
	int32 CurrentHatHit;

	enum State
	{
		Calm,
		Angry,
        Chase,
        Back,
        Leave
    };
	
	State State;

	void OnCalm(AActor* OtherActor);
	void OnChase(AActor* OtherActor);

	void OnCalm(float DeltaSeconds);
	void OnAngry(float DeltaSeconds);
	void OnChase(float DeltaSeconds);
	void OnBack(float DeltaSeconds);
	void OnLeave(float DeltaSeconds);

	void OnAttack();
	void OnHitHat();
	void OnReturnBack();
	void OnLeave();
	void OnStop();
	void OnDelete();
	void TrySetPassed();

	void MoveSmoothTo(FVector TargetLocation, float DeltaSeconds);
	void UpdateRotation(FVector TargetLocation);
};