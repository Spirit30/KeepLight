// Fill out your copyright notice in the Description page of Project Settings.


#include "CircularSaw.h"

#include "CircularSawButton.h"
#include "DraggableObject.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

ACircularSaw::ACircularSaw()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACircularSaw::Activate(bool flag, bool BearOrRock)
{
	IsActive = flag;

	SetActorRelativeLocation(IsActive ? ActiveOffset : FVector::ZeroVector);
	CollisionOfChildActor->SetCollisionResponseToAllChannels(IsActive ? ECR_Block : ECR_Ignore);
	//DeathCollision->SetCollisionResponseToAllChannels(IsActive ? ECR_Overlap : ECR_Ignore);

	Audio->SetActive(flag, true);

	if(IsActive)
	{
		const auto Remover = BearOrRock ? WoodenDeckMover : WoodenDeckRemover;
		
		const auto WoodenDeck = Cast<ADraggableObject>(Remover->ActorToRemove);
		
		const float Dist = FVector::Dist(GetActorLocation(), WoodenDeck->GetStaticMeshComponent()->GetCenterOfMass());

		if(Dist < RemoveWoodenDeckDistance)
		{
			Remover->StartRemove();
		}
	}
}

void ACircularSaw::BeginPlay()
{
	Super::BeginPlay();

	CollisionOfChildActor = Cast<UStaticMeshComponent>(CollisionOfChildActorHolder->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	DeathCollision = Cast<USphereComponent>(Death->GetComponentByClass(USphereComponent::StaticClass()));
	Audio = Cast<UAudioComponent>(GetComponentByClass(UAudioComponent::StaticClass()));
}

void ACircularSaw::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if(IsActive && OtherActor == WoodenDeckMover->ActorToRemove)
	{
		const auto WoodenDeck = Cast<ADraggableObject>(OtherActor);

		if(WoodenDeck->IsDrag())
		{
			RockMover->StartRemove();
			WoodenDeckMover->StartRemove();
			
			GetWorld()->GetTimerManager().SetTimer(RemoveWoodenDeckTimerHandle, this, &ACircularSaw::DisapearWoodenDeck, RemoveWoodenDeckTimer, false);
			
			UGameplayStatics::PlaySound2D(GetWorld(), WoodDeckDoneSound, WoodDeckDoneVolume);

			const auto Button = Cast<ACircularSawButton>(ButtonActor);
			Button->Deactivate();
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

	ApearWoodenPlanks();
}

