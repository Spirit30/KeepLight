// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Catapult.h"
#include "Mover.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "CircularSaw.generated.h"

UCLASS()
class BEAR_API ACircularSaw : public AActor
{
	GENERATED_BODY()
	
	public:

	UPROPERTY(EditAnywhere)
	AActor* ButtonActor;

	UPROPERTY(EditAnywhere)
	USoundWave* WoodDeckDoneSound;

	UPROPERTY(EditAnywhere)
	float WoodDeckDoneVolume = 1.0f;

	UPROPERTY(EditAnywhere)
	AMover* Mover;
	
	UPROPERTY(EditAnywhere)
	AActor* CollisionOfChildActorHolder;

	UPROPERTY(EditAnywhere)
	FVector ActiveOffset = FVector(0.0f, 0.0f, 90.0f);
	
	UPROPERTY(EditAnywhere)
	FRotator RotationSpeed = FRotator(300.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere)
	TArray<AActor*> WoodenPlankStaticActors;

	UPROPERTY(EditAnywhere)
	float RemoveWoodenDeckTimer = 2.0f;

	UPROPERTY(EditAnywhere)
	AActor* Death;

	UPROPERTY(EditAnywhere)
	ACatapult* Catapult;
	
	FTimerHandle RemoveWoodenDeckTimerHandle;
	
	ACircularSaw();

	void Activate(bool flag, bool BearOrRock);

	protected:

	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void Tick(float DeltaTime) override;

	private:

	UAudioComponent* Audio;
	
	bool IsActive;
	USphereComponent* DeathCollision;

	void ApearWoodenPlanks();

	UFUNCTION()
	void DisapearWoodenDeck();
};
