// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DynamicObject.h"

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
	float MinPushForce;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxPushForce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinPushDist;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxPushDist;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PullDistance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector RightHandDragOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator RightHandDragRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector LeftHandDragOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator LeftHandDragRotation;

	// /** called when something enters the sphere component */
	// UFUNCTION()
 //    void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
 //
	// /** called when something leaves the sphere component */
	// UFUNCTION()
 //    void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
 //
	// bool IsGround() const;

	protected:

	//virtual void BeginPlay() override;

	private:

	// int32 EnvironmentsCount;
	// USphereComponent* GroundCollision;
	//
	// USphereComponent* FindGroundCollision() const;
};
