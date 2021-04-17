// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable.h"

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

void AInteractable::Interact()
{
	IsInteractingFlag = true;
}

void AInteractable::StopInteract()
{
	IsInteractingFlag = false;
}
