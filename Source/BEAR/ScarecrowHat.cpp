// Fill out your copyright notice in the Description page of Project Settings.


#include "ScarecrowHat.h"

void AScarecrowHat::BeginPlay()
{
	Super::BeginPlay();

	HatMesh = Cast<UStaticMeshComponent>(HatMeshActor->FindComponentByClass(UStaticMeshComponent::StaticClass()));

	//Tick Group
	SetTickGroup(TG_PostUpdateWork);
}

void AScarecrowHat::Interact()
{
	Super::Interact();

	HatMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
}

void AScarecrowHat::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(IsInteracting())
	{
		const auto HeadAnchorLocation = Bear->GetMesh()->GetSocketLocation(TargetSocketName) + HeadOffset;
		SetActorLocation(HeadAnchorLocation);
	}
}
