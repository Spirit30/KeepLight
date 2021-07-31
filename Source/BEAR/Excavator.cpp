// Fill out your copyright notice in the Description page of Project Settings.


#include "Excavator.h"

// Sets default values
AExcavator::AExcavator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AExcavator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExcavator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

