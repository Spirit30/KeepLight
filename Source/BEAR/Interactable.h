// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

UCLASS()
class BEAR_API AInteractable : public AActor
{
	GENERATED_BODY()
	
	public:	

	AInteractable();
	
	bool CanInteract() const;
	bool IsInteracting() const;
	virtual void Interact();
	virtual void StopInteract();
	
	private:

	bool IsInteractingFlag;
};
