// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "DraggableObject.h"
#include "Interactable.h"
#include "GameFramework/Character.h"

#include "BEARCharacter.generated.h"

UCLASS(config=Game)
class ABEARCharacter : public ACharacter
{
	GENERATED_BODY()

	public:

	//Used by Animation Blueprint.
	//------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MoveDirection;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsIK;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector RightHandLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator RightHandRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector LeftHandLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator LeftHandRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsPullUp;
	
	//------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> DeathEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxFallingTime = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector DeStackForce = FVector(0, 0, 100000);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JumpHitDot = 0.75f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JumpHitForce = 10000.0f;
	
	float MoveRightInputValue;
	
	ABEARCharacter();

	FVector GetHeadLocation() const;
	bool IsKilled() const;
	UFUNCTION(BlueprintCallable)
	void Kill(bool HasEffect, FVector Point);
	bool IsPush() const;
	bool IsPull() const;
	void StopInteract();
	UFUNCTION(BlueprintCallable)
	void SetPullUp(bool flag);
	void SetIsSwing(bool flag);
	bool GetIsSwing();
	
	protected:

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void Landed(const FHitResult& Hit) override;

	private:

	float MinWalkSpeed;
	float MaxWalkSpeed;
	float FallingTimer;
	bool IsStack;

	bool IsDrag;
	bool IsKilledFlag;

	bool IsSwing;
	
	TArray<ADraggableObject*> CloseDragObjects;
	TArray<AInteractable*> CloseInteractables;

	//Input interface
	//------------------------------
	void MoveRight(float Val);
	void Interact();
	void TryJump();
	//------------------------------

	bool CanDrag();
	bool CanInteract();
	void ResetFallingTimer();
	
	UInputComponent* InputComponent;
	ADraggableObject* ActiveDragObject;
	AInteractable* ActiveInteractable;
	UStaticMeshComponent* PotentialTopObstacle;

	FHitResult PotentialTopObstacleHit;
};