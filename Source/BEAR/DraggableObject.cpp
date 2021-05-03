// Fill out your copyright notice in the Description page of Project Settings.

#include "DraggableObject.h"

#include "BEARCharacter.h"
#include "Logger.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/StaticMeshSocket.h"
#include "Kismet/GameplayStatics.h"

ADraggableObject::ADraggableObject()
{
	PrimaryActorTick.bCanEverTick = true;

	SetTickGroup(TG_PrePhysics);
}

void ADraggableObject::Drag(float DeltaSeconds)
{
	IsDrag = true;

	//DragPivot Calculated once per Tick for optimization
	DragPivot = CalculateDragPivot();

	const auto Bear = Cast<ABEARCharacter>(BEAR);
	
	//Find Force depending on Dist
	Logger::DrawLine(GetWorld(), DragPivot, Bear->GetActorLocation(), DeltaSeconds);
	const float Dist = FVector::Dist(DragPivot, Bear->GetActorLocation());
	FVector Force;

	//Push
	if(Bear->IsPush())
	{
		const float ForceAlpha = Dist / MaxDragDist;
		const float ForceAlphaClamped = ForceAlpha > 1.0f ? 1.0f : ForceAlpha;
		const float ForceAmount = FMath::Lerp(MaxDragForce, 0.0f, ForceAlphaClamped);

		const FVector ForceDirection = (DragPivot - Bear->GetActorLocation()).GetUnsafeNormal();
		const int32 ForceDirectionY = FMath::Sign(ForceDirection.Y);
		Force = FVector(0, ForceAmount * ForceDirectionY, 0) + Bear->GetCapsuleComponent()->GetComponentVelocity() * CharacterAffectCoef;

		const FString Output = FString::Printf(TEXT("PUSH Dist: %f Dist Alpha: %f  Force.Y: %f"), Dist, ForceAlphaClamped, Force.Y);
		Logger::ToScreen(Output, DeltaSeconds, FColor::Green);
	}
	//Pull
	else
	{
		const float ForceAlpha = Dist / MaxDragDist;
		const float ForceAlphaClamped = ForceAlpha > MaxDragDist ? MaxDragDist : ForceAlpha;
		const float ForceAmount = FMath::Lerp(0.0f, MaxDragForce, ForceAlphaClamped);

		const FVector ForceDirection = (Bear->GetActorLocation() - DragPivot).GetUnsafeNormal();
		const int32 ForceDirectionY = FMath::Sign(ForceDirection.Y);
		Force = FVector(0, ForceAmount * ForceDirectionY, 0) - Bear->GetCapsuleComponent()->GetComponentVelocity() * CharacterAffectCoef;
		
		const FString Output = FString::Printf(TEXT("PULL Dist: %f Dist Alpha: %f  Force.Y: %f"), Dist, ForceAlphaClamped, Force.Y);
		Logger::ToScreen(Output, DeltaSeconds, FColor::Green);
	}

	GetStaticMeshComponent()->AddForce(Force, NAME_None, true);
}

void ADraggableObject::StopDrag()
{
	IsDrag = false;
}

FVector ADraggableObject::GetRightHandLocation() const
{
	return  DragPivot + RightHandDragOffset;
}

FVector ADraggableObject::GetLeftHandLocation() const
{
	return  DragPivot + LeftHandDragOffset;
}

bool ADraggableObject::CanDrag() const
{
	const auto DirectionFromBearToThis = (GetActorLocation() - BEAR->GetActorLocation()).GetUnsafeNormal();
	return  FVector::DotProduct(BEAR->GetActorForwardVector(), DirectionFromBearToThis) > DragDot;
}

FVector ADraggableObject::CalculateDragPivot() const
{
	const auto StaticMesh = GetStaticMeshComponent();
	const auto SocketNames = StaticMesh->GetAllSocketNames();
	
	if(SocketNames.Num() > 0)
	{
		FVector ClosestSocketLocation;
		float MinDist = TNumericLimits<float>::Max();
		
		for(const auto SocketName : SocketNames)
		{
			const auto Socket = StaticMesh->GetSocketByName(SocketName);
			FTransform SocketTransform;

			if(Socket->GetSocketTransform(SocketTransform, GetStaticMeshComponent()))
			{
				const auto SocketWorldLocation = SocketTransform.GetLocation();
		
				const float Dist = FVector::Dist(SocketWorldLocation, BEAR->GetActorLocation());
			
				if(Dist < MinDist)
				{
					MinDist = Dist;
					ClosestSocketLocation = SocketWorldLocation;
				}
			}
		}
		
		return ClosestSocketLocation;
	}
	
	return StaticMesh->GetComponentLocation();
}

void ADraggableObject::SetLocation(FVector Location)
{
	PreviousLocation = GetActorLocation();
	SetActorLocation(Location);
}

void ADraggableObject::MoveBack()
{
	if(PreviousLocation != FVector::ZeroVector)
	{
		SetActorLocation(PreviousLocation);
	}
}

void ADraggableObject::BeginPlay()
{
	Super::BeginPlay();

	BEAR = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if(DragSound)
	{
		AudioComponent = UGameplayStatics::SpawnSoundAtLocation(this, DragSound, GetActorLocation());
		const FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
		AudioComponent->AttachToComponent(GetStaticMeshComponent(), AttachmentTransformRules);
	}
}

void ADraggableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(AudioComponent)
	{
		const float Velocity = GetStaticMeshComponent()->GetComponentVelocity().SizeSquared();
		AudioComponent->SetVolumeMultiplier(Velocity > DragSoundVelocity ? DragSoundVolume : 0.0f);
	}
}
