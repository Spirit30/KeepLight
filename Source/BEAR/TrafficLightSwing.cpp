// Fill out your copyright notice in the Description page of Project Settings.


#include "TrafficLightSwing.h"

#include "Logger.h"
#include "Kismet/GameplayStatics.h"

ATrafficLightSwing::ATrafficLightSwing()
{
	PrimaryActorTick.bCanEverTick = true;
}

bool ATrafficLightSwing::GetIsBearSwing()
{
	return Bear->GetIsSwing();
}

void ATrafficLightSwing::ResetAcumulativeSwingForce()
{
	TrafficLightDraggable->GetStaticMeshComponent()->SetAllPhysicsLinearVelocity(FVector::ZeroVector);
	TrafficLightDraggable->GetStaticMeshComponent()->SetAllPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
}

void ATrafficLightSwing::BeginPlay()
{
	Super::BeginPlay();

	InitialLocation = TrafficLightDraggable->GetActorLocation();
	InitialRotation = TrafficLightDraggable->GetActorRotation();

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
		CanSwing = !CanSwing;
		
		if(CanSwing)
		{
			Bear->SetIsSwing(TrafficLightDraggable);
			ResetAcumulativeSwingForce();
		}
	}
	else if(OtherActor == TrafficLightDraggable)
	{
		TrafficLightDraggable->SetActorLocationAndRotation(InitialLocation, InitialRotation);
		UGameplayStatics::PlaySound2D(this, FallSound);
		
		SetPhysicsConstraintEnabled(true);	
		SetActorTickEnabled(true);
	}
}

void ATrafficLightSwing::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	if(IsPhysicsActive && OtherActor == Bear)
	{
		ResetAcumulativeSwingForce();
	}
}

void ATrafficLightSwing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const auto Location = TrafficLightDraggable->GetActorLocation();

	SetActorLocation(Location);

	if(GetIsBearSwing())
	{
		//On Input Start Or End
		if(Bear->MoveRightInputValue != PreviousInputValue)
		{
			InputTime = 0.0f;		
			PreviousInputValue = Bear->MoveRightInputValue;
		}
		//On Input
		else
		{
			if(Bear->MoveRightInputValue)
			{
				InputTime += DeltaTime;
			}
			else
			{
				InputTime -= DeltaTime;
				InputTime = FMath::Max(0.0f, InputTime);
			}
		}

		const float Force = -Bear->MoveRightInputValue * BaseForce * AccelerateCurve->GetFloatValue(InputTime) * DeltaTime;
		const auto FinalForce = FVector(0.0f, Force, 0);
		TrafficLightDraggable->GetStaticMeshComponent()->AddForce(FinalForce);
	}
}

void ATrafficLightSwing::SetPhysicsConstraintEnabled(bool flag)
{
	IsPhysicsActive = flag;
	PhysicsConstraint->SetLinearXLimit(flag ? LCM_Locked : LCM_Free, 0);
	PhysicsConstraint->SetLinearYLimit(flag ? LCM_Locked : LCM_Free, 0);
	PhysicsConstraint->SetLinearZLimit(flag ? LCM_Locked : LCM_Free, 0);
}

