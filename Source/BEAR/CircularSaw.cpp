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

	DeathCollision->SetCollisionResponseToAllChannels(IsActive ? ECR_Overlap : ECR_Ignore);

	Audio->SetActive(flag, true);

	if(IsActive)
	{
		const auto Remover = BearOrRock ? WoodenDeckRightMover : WoodenDeckLeftMover;
		Remover->StartRemove();
	}
}

void ACircularSaw::BeginPlay()
{
	Super::BeginPlay();

	DeathCollision = Cast<USphereComponent>(Death->GetComponentByClass(USphereComponent::StaticClass()));
	Audio = Cast<UAudioComponent>(GetComponentByClass(UAudioComponent::StaticClass()));
}

void ACircularSaw::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if(IsActive && OtherActor == WoodenDeckRightMover->ActorToRemove)
	{
		const auto WoodenDeck = Cast<ADraggableObject>(OtherActor);

		if(WoodenDeck->IsDrag())
		{
			RockMover->StartRemove();
			WoodenDeckRightMover->StartRemove();
			
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
}

void ACircularSaw::DisapearWoodenDeck()
{
	WoodenDeckRightMover->ActorToRemove->Destroy();

	ApearWoodenPlanks();
}

