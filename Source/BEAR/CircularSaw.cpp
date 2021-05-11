// Fill out your copyright notice in the Description page of Project Settings.


#include "CircularSaw.h"

#include "Util.h"

ACircularSaw::ACircularSaw()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACircularSaw::Activate(bool flag)
{
	IsActive = flag;

	SetActorRelativeLocation(flag ? ActiveOffset : FVector::ZeroVector);
	CollisionOfChildActor->SetCollisionResponseToAllChannels(flag ? ECR_Block : ECR_Ignore);

	if(IsActive)
	{
		WoodenDeckRemover->StartRemove();
	}
}

void ACircularSaw::BeginPlay()
{
	Super::BeginPlay();

	CollisionOfChildActor = Cast<UStaticMeshComponent>(CollisionOfChildActorHolder->GetComponentByClass(UStaticMeshComponent::StaticClass()));
}

void ACircularSaw::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(IsActive)
	{
		AddActorLocalRotation(RotationSpeed * DeltaTime);
	}
}

