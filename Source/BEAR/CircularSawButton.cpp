// Fill out your copyright notice in the Description page of Project Settings.


#include "CircularSawButton.h"

#include "Particles/ParticleSystemComponent.h"

ACircularSawButton::ACircularSawButton()
{
	PrimaryActorTick.bCanEverTick = true;

	IsActiveFlag = true;
}

void ACircularSawButton::Deactivate()
{
	const auto SmokeEmitter = Cast<UParticleSystemComponent>(SmokeEffectActor->GetComponentByClass(UParticleSystemComponent::StaticClass()));
	SmokeEmitter->SetActive(true);

	IsActiveFlag = false;
}

void ACircularSawButton::BeginPlay()
{
	Super::BeginPlay();

}

void ACircularSawButton::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if(IsActiveFlag && ActivatorActors.Contains(OtherActor))
	{
		CircularSaw->Activate(true);
	}
}

void ACircularSawButton::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	if(IsActiveFlag && ActivatorActors.Contains(OtherActor))
	{
		CircularSaw->Activate(false);
	}
}

void ACircularSawButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

