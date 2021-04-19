// Fill out your copyright notice in the Description page of Project Settings.


#include "ScarecrowHat.h"

#include "Util.h"

bool AScarecrowHat::IsAttached()
{
	return IsAttachedFlag;
}

void AScarecrowHat::GrabByCrow()
{
	BearCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	CrowCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	
	IsGrabbedByCrow = true;
}

void AScarecrowHat::Delete()
{
	HatMeshActor->Destroy();
	Destroy();
}

void AScarecrowHat::BeginPlay()
{
	Super::BeginPlay();

	HatMesh = Cast<UStaticMeshComponent>(HatMeshActor->FindComponentByClass(UStaticMeshComponent::StaticClass()));
	BearCollision = Cast<USphereComponent>(Util::GetComponentByName(this, USphereComponent::StaticClass(), BearTriggerName));
	CrowCollision = Cast<UBoxComponent>(Util::GetComponentByName(this, UBoxComponent::StaticClass(), CrowTriggerName));

	//Tick Group
	SetTickGroup(TG_PostUpdateWork);
}

void AScarecrowHat::Interact()
{
	Super::Interact();

	HatMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	IsActivated = true;
}

void AScarecrowHat::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(IsActivated && !IsGrabbedByCrow)
	{
		const auto HeadAnchorLocation = Bear->GetMesh()->GetSocketLocation(TargetSocketName) + HeadOffset;

		if(IsAttached())
		{
			SetActorLocation(HeadAnchorLocation);
		}
		else
		{
			const float AttachProgress = DeltaSeconds * AttachSpeed;
			
			const auto Location = FMath::Lerp(GetActorLocation(), HeadAnchorLocation, AttachProgress);
			SetActorLocation(Location);

			const auto Rotation = FMath::Lerp(GetActorRotation(), FRotator::ZeroRotator, AttachProgress);
			SetActorRotation(Rotation);

			const float Dist = FVector::Dist(GetActorLocation(), HeadAnchorLocation);
			IsAttachedFlag = Dist < AttachDistance;

			if(IsAttachedFlag)
			{
				StopInteract();
			}
		}
	}
}
