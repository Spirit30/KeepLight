// Fill out your copyright notice in the Description page of Project Settings.


#include "CircularSawButton.h"

ACircularSawButton::ACircularSawButton()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACircularSawButton::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACircularSawButton::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if(ActivatorActors.Contains(OtherActor))
	{
		CircularSaw->Activate(true);
	}
}

void ACircularSawButton::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	if(ActivatorActors.Contains(OtherActor))
	{
		CircularSaw->Activate(false);
	}
}

void ACircularSawButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

