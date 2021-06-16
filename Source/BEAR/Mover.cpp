// Fill out your copyright notice in the Description page of Project Settings.


#include "Mover.h"

AMover::AMover()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AMover::StartMove()
{
	StartDistance = FVector::Dist(Target->GetActorLocation(), GetDestination());
	
	SetActorTickEnabled(true);
}

void AMover::BeginPlay()
{
	Super::BeginPlay();

	StopMove();
}

void AMover::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if(OtherActor == Target)
	{
		StartMove();
	}
}

void AMover::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const auto CurrentLocation = Target->GetActorLocation();
	const auto Destination = GetDestination();
	const float Dist = FVector::Dist(CurrentLocation, Destination);

	if(Dist > ReachDestinationDist)
	{
		const auto Location = FMath::Lerp(Target->GetActorLocation(), Destination, Speed * DeltaTime);
		Target->SetActorLocation(Location);

		if(ShouldRotate)
		{
			const float Progress = Dist / StartDistance;
			const auto Rotation = FMath::Lerp(Target->GetActorRotation(), RotationDestination, Progress);
			Target->SetActorRotation(Rotation);
		}
	}
	else
	{
		StopMove();

		if(ShouldRotate)
		{
			Target->SetActorRotation(RotationDestination);
		}
	}
}

void AMover::StopMove()
{
	SetActorTickEnabled(false);
}

FVector AMover::GetDestination()
{
	return DestinationActor ? DestinationActor->GetActorLocation() : GetActorLocation() + LocalDestination;
}

