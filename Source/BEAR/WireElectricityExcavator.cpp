// Fill out your copyright notice in the Description page of Project Settings.


#include "WireElectricityExcavator.h"

#include "CableComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWireElectricityExcavator::AWireElectricityExcavator()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AWireElectricityExcavator::Init(FVector StartLocation)
{
	SetActorLocation(StartLocation);
	
	DirectionSign = 1;
	TargetPointIndex = 0;

	UpdatePath();

	Activate();
}

void AWireElectricityExcavator::BeginPlay()
{
	Super::BeginPlay();

	AudioComponent = UGameplayStatics::SpawnSoundAttached(Sound, GetRootComponent());

	Deactivate();
}

void AWireElectricityExcavator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdatePath();

	const auto CurrentLocation = GetActorLocation();
	const auto TargetPoint = GetTargetPoint();
	const float Dist = FVector::Dist(CurrentLocation, TargetPoint);

	if(Dist > ReachDestinationDist)
	{
		const float Progress = Speed * DeltaTime / Dist;
		const auto Location = FMath::Lerp(CurrentLocation, TargetPoint, Progress);
		SetActorLocation(Location);
	}
	else
	{
		SetNextTargetPoint();
	}
}

void AWireElectricityExcavator::Activate()
{
	SetActorTickEnabled(true);
	Emitter->Activate();
	AudioComponent->SetPaused(false);
}

void AWireElectricityExcavator::Deactivate()
{
	SetActorTickEnabled(false);
	Emitter->Deactivate();
	AudioComponent->SetPaused(true);
}

void AWireElectricityExcavator::UpdatePath()
{
	Cable->CableComponent->GetCableParticleLocations(Path);
}

FVector AWireElectricityExcavator::GetTargetPoint() const
{
	return Path[TargetPointIndex];
}

void AWireElectricityExcavator::SetNextTargetPoint()
{
	if(TargetPointIndex >= Path.Num() - 1)
	{
		DirectionSign = -1;
	}
	else if(TargetPointIndex <= 0)
	{
		DirectionSign = 1;
	}

	TargetPointIndex += DirectionSign;
}

