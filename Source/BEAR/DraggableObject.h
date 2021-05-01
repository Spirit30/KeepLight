// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "DynamicObject.h"
#include "Components/BoxComponent.h"

#include "DraggableObject.generated.h"

/**
 * 
 */
UCLASS()
class BEAR_API ADraggableObject : public ADynamicObject
{
	GENERATED_BODY()

	public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PushForce = 8000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PullForce = 12000;
	
	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// float PushDistance;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// float PullDistance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector RightHandDragOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator RightHandDragRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector LeftHandDragOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator LeftHandDragRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DragDot = 0.75f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DragDistanceLerpSpeed = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundWave* DragSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DragSoundVolume = 0.1f;
	
	// UPROPERTY(EditAnywhere)
	// TArray<AActor*> Obstacles;

	ADraggableObject();

	void Drag(float DeltaSeconds);
	void StopDrag();
	FVector GetRightHandLocation() const;
	FVector GetLeftHandLocation() const;
	bool CanDrag() const;

	protected:

	virtual void BeginPlay() override;
	// virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	// virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	//virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	virtual void Tick(float DeltaTime) override;
	
	private:

	AActor* BEAR;
	// UBoxComponent* BoxCollision;
	// FVector BoxCollisionInitialOffset;
	UAudioComponent* AudioComponent;
	
	FVector DragPivot;
	FVector PreviousLocation;
	bool IsDrag;
	//TArray<AActor*> FloorActors;

	FVector CalculateDragPivot() const;
	void SetLocation(FVector Location);
	void MoveBack();
	//void UpdateBoxCollision();
};
