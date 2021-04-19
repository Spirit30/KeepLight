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

	//------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DragDistanceLerpSpeed;

	ABEARCharacter();
	
	//Called for side to side input
	void MoveRight(float Val);
	void Interact();
	void StopInteract();
	void TryJump();
	
	protected:

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual  void NotifyActorEndOverlap(AActor* OtherActor) override;
	virtual void Tick(float DeltaSeconds) override;

	private:

	float MinWalkSpeed;
	float MaxWalkSpeed;

	bool IsDrag;
	
	TArray<ADraggableObject*> CloseDragObjects;
	TArray<AInteractable*> CloseInteractables;

	bool CanDrag();
	bool CanInteract();

	ADraggableObject* ActiveDragObject;
	AInteractable* ActiveInteractable;
};