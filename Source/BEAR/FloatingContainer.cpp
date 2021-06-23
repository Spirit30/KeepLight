// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingContainer.h"

#include "Logger.h"

AFloatingContainer::AFloatingContainer()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AFloatingContainer::StartFloating()
{
	PickRandomDestinationLocation();
	SetActorTickEnabled(true);
}

void AFloatingContainer::StopFloating()
{
	SetActorTickEnabled(false);
}

void AFloatingContainer::BeginPlay()
{
	Super::BeginPlay();

	InitialLocation = GetActorLocation();
	Sphere = Cast<USphereComponent>(GetComponentByClass(USphereComponent::StaticClass()));

	if(AutoPlay)
	{
		StartFloating();
	}
	else
	{
		StopFloating();
	}
}

void AFloatingContainer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const auto CurrentLocation = GetActorLocation();
	const float Dist = FVector::Dist(CurrentLocation, DestinationLocation);

	if(Dist > ReachDistance)
	{
		const auto Location = FMath::Lerp(CurrentLocation, DestinationLocation, Speed / Dist * DeltaTime);
		SetActorLocation(Location);

		Logger::DrawLine(GetWorld(), CurrentLocation, DestinationLocation, DeltaTime, FColor::Orange);
	}
	else
	{
		PickRandomDestinationLocation();
	}
}

void AFloatingContainer::PickRandomDestinationLocation()
{
	DestinationLocation = InitialLocation + FMath::VRand().GetUnsafeNormal() * Sphere->GetUnscaledSphereRadius();
}

