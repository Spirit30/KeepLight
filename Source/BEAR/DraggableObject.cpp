// Fill out your copyright notice in the Description page of Project Settings.


#include "DraggableObject.h"

ADraggableObject::ADraggableObject()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADraggableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
