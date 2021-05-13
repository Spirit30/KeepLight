// Fill out your copyright notice in the Description page of Project Settings.


#include "CircularSaw.h"

#include "DraggableObject.h"

ACircularSaw::ACircularSaw()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACircularSaw::Activate(bool flag)
{
	IsActive = flag;

	SetActorRelativeLocation(IsActive ? ActiveOffset : FVector::ZeroVector);
	CollisionOfChildActor->SetCollisionResponseToAllChannels(IsActive ? ECR_Block : ECR_Ignore);
	DeathCollision->SetCollisionResponseToAllChannels(IsActive ? ECR_Overlap : ECR_Ignore);

	if(IsActive)
	{
		const auto WoodenDeck = Cast<ADraggableObject>(WoodenDeckRemover->ActorToRemove);
		
		const float Dist = FVector::Dist(GetActorLocation(), WoodenDeck->GetStaticMeshComponent()->GetCenterOfMass());

		if(Dist < RemoveWoodenDeckDistance)
		{
			WoodenDeckRemover->StartRemove();
		}
	}
}

void ACircularSaw::BeginPlay()
{
	Super::BeginPlay();

	CollisionOfChildActor = Cast<UStaticMeshComponent>(CollisionOfChildActorHolder->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	DeathCollision = Cast<USphereComponent>(Death->GetComponentByClass(USphereComponent::StaticClass()));
}

void ACircularSaw::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if(IsActive && OtherActor == WoodenDeckMover->ActorToRemove)
	{
		const auto WoodenDeck = Cast<ADraggableObject>(OtherActor);

		if(WoodenDeck->IsDrag())
		{
			ApearWoodenPlanks();
			RockMover->StartRemove();
			WoodenDeckMover->StartRemove();
			GetWorld()->GetTimerManager().SetTimer(RemoveWoodenDeckTimerHandle, this, &ACircularSaw::DisapearWoodenDeck, RemoveWoodenDeckTimer, false);
		}
	}
}

void ACircularSaw::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(IsActive)
	{
		AddActorLocalRotation(RotationSpeed * DeltaTime);
	}
}

void ACircularSaw::ApearWoodenPlanks()
{
	for(const auto WoodenPlankActor : WoodenPlankStaticActors)
	{
		const auto WoodenPlankMesh = Cast<UStaticMeshComponent>(WoodenPlankActor->GetComponentByClass(UStaticMeshComponent::StaticClass()));
		WoodenPlankMesh->SetHiddenInGame(false);
	}

	const auto WoodenPlankMesh = WoodenPlankDraggable->GetStaticMeshComponent();
	WoodenPlankMesh->SetHiddenInGame(false);
	WoodenPlankMesh->SetSimulatePhysics(true);
}

void ACircularSaw::DisapearWoodenDeck()
{
	WoodenDeckMover->ActorToRemove->Destroy();
}

