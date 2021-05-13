// Fill out your copyright notice in the Description page of Project Settings.


#include "Catapult.h"

ACatapult::ACatapult()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACatapult::BeginPlay()
{
	Super::BeginPlay();	
}

void ACatapult::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if(Other == BEAR)
	{
		const auto Dot = FVector::DotProduct(HitNormal, FVector::DownVector);

		if(Dot > HitDot)
		{
			const auto Mesh = Cast<UStaticMeshComponent>(MyComp);
			Mesh->AddForceAtLocation(HitNormal * HitForce, HitLocation, NAME_None);
		}
	}
}

void ACatapult::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

