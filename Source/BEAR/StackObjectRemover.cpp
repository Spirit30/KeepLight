// Fill out your copyright notice in the Description page of Project Settings.


#include "StackObjectRemover.h"

AStackObjectRemover::AStackObjectRemover()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AStackObjectRemover::StartRemove()
{
	PathPointIndex = 0;
	Hand->SetHiddenInGame(false);
	SetTargetActorPhysicsEnabled(false);
}

bool AStackObjectRemover::IsActive() const
{
	return PathPointIndex < ActorRemovePath.Num();
}

void AStackObjectRemover::BeginPlay()
{
	Super::BeginPlay();

	Hand = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));

	PathPointIndex = ActorRemovePath.Num();
}

void AStackObjectRemover::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if(ActorToRemove && OtherActor == ActorToRemove)
	{
		StartRemove();
	}
}

void AStackObjectRemover::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(ActorToRemove && IsActive())
	{
		const auto PathPoint = ActorRemovePath[PathPointIndex];
		const auto Location = FMath::Lerp(ActorToRemove->GetActorLocation(), PathPoint, DeltaTime * RemoveSpeed);
		const float Dist = FVector::Dist(ActorToRemove->GetActorLocation(), Location);
		
		if(Dist > ReachDestinationDist)
		{
			ActorToRemove->SetActorLocation(Location);
			Hand->SetWorldLocation(Location);
		}
		else
		{
			OnNextPoint();
		}
	}
}

void AStackObjectRemover::OnNextPoint()
{
	++PathPointIndex;

	if(PathPointIndex == ActorRemovePath.Num())
	{
		OnEndRemove();
	}
}

void AStackObjectRemover::OnEndRemove() const
{
	Hand->SetHiddenInGame(true);
	SetTargetActorPhysicsEnabled(true);
}

void AStackObjectRemover::SetTargetActorPhysicsEnabled(bool flag) const
{
	for (UActorComponent* Component : ActorToRemove->GetComponents())
	{
		if (UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(Component))
		{
			Mesh->SetSimulatePhysics(flag);
			Mesh->SetCollisionEnabled(flag ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
		}
	}
}

