// Fill out your copyright notice in the Description page of Project Settings.


#include "TrafficLightSwing.h"

ATrafficLightSwing::ATrafficLightSwing()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATrafficLightSwing::BeginPlay()
{
	Super::BeginPlay();

	PhysicsConstraint = Cast<UPhysicsConstraintComponent>(PhysicsConstraintActor->GetComponentByClass(UPhysicsConstraintComponent::StaticClass()));
	
	SetPhysicsConstraintEnabled(false);
	
	TrafficLightDraggable->SetActorLocationAndRotation(Location, Rotation);
	
	SetActorTickEnabled(false);
}

void ATrafficLightSwing::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if(OtherActor == Bear)
	{
		IsBearSwing = true;
	}
	else if(OtherActor == TrafficLightDraggable)
	{
		SetPhysicsConstraintEnabled(true);
		
		PhysicsConstraint->SetConstrainedComponents(
			Cast<UPrimitiveComponent>(SwingAnchor->GetRootComponent()),
			NAME_None,
			TrafficLightDraggable->GetStaticMeshComponent(),
			NAME_None);
		
		SetActorTickEnabled(true);
	}
}

void ATrafficLightSwing::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	if(OtherActor == Bear)
	{
		IsBearSwing = false;
	}
}

void ATrafficLightSwing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(IsBearSwing)
	{
		SetActorLocation(Bear->GetActorLocation());
	}
}

void ATrafficLightSwing::SetPhysicsConstraintEnabled(bool flag)
{
	PhysicsConstraint->SetLinearXLimit(flag ? LCM_Locked : LCM_Free, 0);
	PhysicsConstraint->SetLinearYLimit(flag ? LCM_Locked : LCM_Free, 0);
	PhysicsConstraint->SetLinearZLimit(flag ? LCM_Locked : LCM_Free, 0);
}

