// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable.h"

FVector AInteractable::GetLeftHandLocation() const
{
	return  GetActorLocation();
}

FVector AInteractable::GetRightHandLocation() const
{
	return  GetActorLocation();
}

bool AInteractable::CanMoveCharacter() const
{
	return CanMoveCharacterFlag;
}

AInteractable::AInteractable()
{
	PrimaryActorTick.bCanEverTick = true;
}

bool AInteractable::CanInteract() const
{
	return !IsInteracting();
}

bool AInteractable::IsInteracting() const
{
	return IsInteractingFlag;
}

float AInteractable::GetDragDot() const
{
	return DragDot;
}

void AInteractable::Interact()
{
	IsInteractingFlag = true;
}

void AInteractable::StopInteract()
{
	IsInteractingFlag = false;
}
