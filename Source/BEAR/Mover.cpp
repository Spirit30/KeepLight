// Fill out your copyright notice in the Description page of Project Settings.


#include "Mover.h"

//#include "Logger.h"

AMover::AMover()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMover::StartMove()
{
	StartDistance = FVector::Dist(Target->GetActorLocation(), GetDestination());

	Target->GetComponents<UStaticMeshComponent>(TargetViews);
	
	SetActorTickEnabled(true);

	IsMoveFlag = true;
}

void AMover::BeginPlay()
{
	Super::BeginPlay();

	//Initialized from outside
	if(IsMove())
	{
		return;
	}
	
	if(AutoMove)
	{
		StartMove();
	}
	else
	{
		StopMove();
	}
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

	if(!Target)
	{
		return;
	}

	for(auto TargetView : TargetViews)
	{
		TargetView->SetAllPhysicsLinearVelocity(FVector::ZeroVector);
		TargetView->SetAllPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
	}
	
	const auto CurrentLocation = Target->GetActorLocation();
	const auto Destination = GetDestination();
	const float Dist = FVector::Dist(CurrentLocation, Destination);

	//Logger::ToScreen("Moving " + Target->GetName() + " Dist ", Dist, DeltaTime, FColor::Yellow);

	if(!Interpolate)
	{
		Target->SetActorLocation(Destination);
		MovementEnd();
		return;
	}

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

		//Logger::ToScreen("Moving - " + Target->GetName(), DeltaTime, FColor::Orange);
		//Logger::DrawLine(GetWorld(), Location, Destination, DeltaTime, FColor::Orange);
	}
	else
	{
		MovementEnd();
	}
}

void AMover::StopMove()
{
	SetActorTickEnabled(false);
	IsMoveFlag = false;
}

bool AMover::IsMove() const
{
	return IsMoveFlag;
}

FVector AMover::GetDestination()
{
	return DestinationActor ? DestinationActor->GetActorLocation() : GetActorLocation() + LocalDestination;
}

void AMover::MovementEnd()
{
	StopMove();

	if(ShouldRotate)
	{
		Target->SetActorRotation(RotationDestination);
	}

	if(ShouldDestroyOnEnd && Target)
	{
		Target->Destroy();
	}
}

