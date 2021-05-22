// Fill out your copyright notice in the Description page of Project Settings.


#include "TrafficLightSwing.h"

#include "Logger.h"

ATrafficLightSwing::ATrafficLightSwing()
{
	PrimaryActorTick.bCanEverTick = true;
}

bool ATrafficLightSwing::GetIsBearSwing()
{
	return Bear->GetIsSwing();
}

void ATrafficLightSwing::BeginPlay()
{
	Super::BeginPlay();

	PhysicsConstraint = Cast<UPhysicsConstraintComponent>(PhysicsConstraintActor->GetComponentByClass(UPhysicsConstraintComponent::StaticClass()));
	
	SetPhysicsConstraintEnabled(false);
	SetActorTickEnabled(false);

	TrafficLightDraggable->SetActorLocationAndRotation(SwingAnchor->GetActorLocation(), UpsideDown);
}

void ATrafficLightSwing::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if(IsPhysicsActive && OtherActor == Bear)
	{
		Bear->SetIsSwing(true);
		AcumulativeSwingForce = FVector::ZeroVector;
	}
	else if(OtherActor == TrafficLightDraggable)
	{
		SetPhysicsConstraintEnabled(true);	
		SetActorTickEnabled(true);
	}
}

void ATrafficLightSwing::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	if(IsPhysicsActive && OtherActor == Bear)
	{
		Bear->SetIsSwing(false);
		AcumulativeSwingForce = FVector::ZeroVector;
	}
}

void ATrafficLightSwing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(TrafficLightDraggable->GetActorLocation());

	if(Bear->GetIsSwing())
	{
		AcumulativeSwingForce += SwingForce * AcumulateSwingForceCoef;
		TrafficLightDraggable->GetStaticMeshComponent()->AddForce(AcumulativeSwingForce * Bear->MoveRightInputValue);
	}
}

void ATrafficLightSwing::SetPhysicsConstraintEnabled(bool flag)
{
	IsPhysicsActive = flag;
	PhysicsConstraint->SetLinearXLimit(flag ? LCM_Locked : LCM_Free, 0);
	PhysicsConstraint->SetLinearYLimit(flag ? LCM_Locked : LCM_Free, 0);
	PhysicsConstraint->SetLinearZLimit(flag ? LCM_Locked : LCM_Free, 0);
}

