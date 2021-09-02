// Fill out your copyright notice in the Description page of Project Settings.


#include "Catapult.h"

#include "Util.h"
#include "Kismet/KismetMathLibrary.h"

ACatapult::ACatapult()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACatapult::Shoot()
{
	if(State < Destroyed)
	{
		State = Shooting;
	}
}

void ACatapult::BeginPlay()
{
	Super::BeginPlay();

	WoodenPlankMesh = Util::GetStaticMeshComponentByName(this, "WoodenPlankMesh");
	RockMesh = Util::GetStaticMeshComponentByName(this, "RockMesh");

	InitialWoodenPlankMeshTransform = WoodenPlankMesh->GetComponentTransform();
	InitialRockMeshTransform = RockMesh->GetComponentTransform();

	if(AutoHide)
	{
		//Hide
		WoodenPlankMesh->SetHiddenInGame(true);
		RockMesh->SetHiddenInGame(true);

		//Disable Physics
		WoodenPlankMesh->SetSimulatePhysics(false);
		WoodenPlankMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		RockMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		State = Ready;
	}
}

void ACatapult::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if(State == NotReady)
	{
		// if(OtherActor == WoodenPlank && WoodenPlank->IsDrag())
		// {
		// 	//Drop Draggable
		// 	BEAR->StopInteract();
		// 	
		// 	//Set Graphics as Draggables are
		// 	WoodenPlankMesh->SetWorldLocation(WoodenPlank->GetStaticMeshComponent()->GetComponentLocation());
		// 	RockMesh->SetWorldLocation(Rock->GetStaticMeshComponent()->GetComponentLocation());
		//
		// 	//Destroy Draggables
		// 	WoodenPlank->Destroy();
		// 	Rock->Destroy();
		//
		// 	//Show
		// 	WoodenPlankMesh->SetHiddenInGame(false);
		// 	RockMesh->SetHiddenInGame(false);
		//
		// 	State = Constructing;
		// }
	}
}

void ACatapult::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if(State >= Ready && Other == BEAR)
	{
		const auto Dot = FVector::DotProduct(HitNormal, FVector::DownVector);

		if(Dot > HitDot)
		{
			const auto Mesh = Cast<UStaticMeshComponent>(MyComp);
			const float FinalHitForce = RockMesh->IsBeingDestroyed() ? HitForceWithoutRockMesh : HitForce;
			Mesh->AddForceAtLocation(HitNormal * FinalHitForce, HitLocation, NAME_None);
		}
	}
}

void ACatapult::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (State)
	{
		case Constructing:
			OnConstructing(DeltaTime);
			break;

		case Shooting:
			OnShooting(DeltaTime);
			break;

		default: 
			break;
	}
}

void ACatapult::OnConstructing(float DeltaTime)
{
	const auto WoodenPlankMeshTransform = UKismetMathLibrary::TLerp(WoodenPlankMesh->GetComponentTransform(), InitialWoodenPlankMeshTransform, DeltaTime * ConstructSpeed);
	const auto RockMeshTransform = UKismetMathLibrary::TLerp(RockMesh->GetComponentTransform(), InitialRockMeshTransform, DeltaTime * ConstructSpeed);

	const float WoodenPlankDist = FVector::Dist(WoodenPlankMeshTransform.GetLocation(), InitialWoodenPlankMeshTransform.GetLocation());
	const float RockDist = FVector::Dist(RockMeshTransform.GetLocation(), InitialRockMeshTransform.GetLocation());

	//Constructing
	if(WoodenPlankDist > ConstructDist || RockDist > ConstructDist)
	{
		WoodenPlankMesh->SetWorldTransform(WoodenPlankMeshTransform);
		RockMesh->SetWorldTransform(RockMeshTransform);
	}
	//Constructed
	else
	{
		WoodenPlankMesh->SetWorldTransform(InitialWoodenPlankMeshTransform);
		RockMesh->SetWorldTransform(InitialRockMeshTransform);
			
		//Enable Physics
		WoodenPlankMesh->SetSimulatePhysics(true);
		WoodenPlankMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		RockMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			
		State = Ready;
	}
}

void ACatapult::OnShooting(float DeltaTime)
{
	const auto TargetLocation = Cable->GetActorLocation();
	
	const auto Location = FMath::Lerp(RockMesh->GetComponentLocation(), TargetLocation, DeltaTime * ShootSpeed);

	const float Dist = FVector::Dist(Location, TargetLocation);

	if(Dist > ShootDist)
	{
		RockMesh->SetWorldLocation(Location);
	}
	else
	{
		RockMesh->DestroyComponent();
		
		CatapultSequence->GetSequencePlayer()->Play();
		
		State = Destroyed;
	}
}

