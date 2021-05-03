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
	float MaxDragForce = 10000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxDragDist = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CharacterAffectCoef = 10.0f;
	
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
	USoundWave* DragSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DragSoundVolume = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DragSoundVelocity = 100.0f;
	
	ADraggableObject();

	void Drag(float DeltaSeconds);
	void StopDrag();
	FVector GetRightHandLocation() const;
	FVector GetLeftHandLocation() const;
	bool CanDrag() const;

	protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	private:

	AActor* BEAR;
	UAudioComponent* AudioComponent;
	
	FVector DragPivot;
	FVector PreviousLocation;
	bool IsDrag;

	FVector CalculateDragPivot() const;
	void SetLocation(FVector Location);
	void MoveBack();
};
