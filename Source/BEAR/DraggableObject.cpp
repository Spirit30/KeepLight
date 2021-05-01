// Fill out your copyright notice in the Description page of Project Settings.

#include "DraggableObject.h"

#include "BEARCharacter.h"
#include "Components/AudioComponent.h"
#include "Engine/StaticMeshSocket.h"
#include "Kismet/GameplayStatics.h"

ADraggableObject::ADraggableObject()
{
	PrimaryActorTick.bCanEverTick = true;

	SetTickGroup(TG_PostUpdateWork);
}

void ADraggableObject::Drag(float DeltaSeconds)
{
	if(!IsDrag)
	{
		if(DragSound)
		{
			AudioComponent = UGameplayStatics::SpawnSoundAtLocation(this, DragSound, GetActorLocation());
			AudioComponent->SetVolumeMultiplier(DragSoundVolume);
			AudioComponent->SetActive(true);
			AudioComponent->Play();
		}
	}
	
	IsDrag = true;

	//DragPivot Calculated once per Tick for optimization
	DragPivot = CalculateDragPivot();

	const auto Bear = Cast<ABEARCharacter>(BEAR);
	
	auto Force = 0;
	
	if(Bear->IsPush())
	{
		Force = PushForce;
	}
	else if(Bear->IsPull())
	{
		Force = PullForce;
	}
	
	GetStaticMeshComponent()->AddForce(BEAR->GetActorForwardVector() * DeltaSeconds * Force * Bear->MoveDirection, NAME_None, true);

	// const auto DragPivotOffset = GetActorLocation() - DragPivot;
	// const float DragDist = Cast<ABEARCharacter>(BEAR)->IsPush() ? PushDistance : PullDistance;
	// 			
	// auto TargetDragLocation = BEAR->GetActorLocation() + DragPivotOffset + BEAR->GetActorForwardVector() * DragDist;
	// //Locked 2D
	// TargetDragLocation.X = 0;
	// //Controlled by Physics
	// TargetDragLocation.Z = GetActorLocation().Z;
	// 		
	// auto DragLocation = FMath::Lerp(GetActorLocation(), TargetDragLocation, DeltaSeconds * DragDistanceLerpSpeed);
	// //Locked 2D
	// DragLocation.X = 0;
	// //Controlled by Physics
	// DragLocation.Z = GetActorLocation().Z;
	//
	// SetLocation(DragLocation);
}

void ADraggableObject::StopDrag()
{
	IsDrag = false;

	if(AudioComponent)
	{
		AudioComponent->DestroyComponent();
	}
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

	//Alternative method
	// FVector OutPointOnBody;
	// GetStaticMeshComponent()->GetClosestPointOnCollision(BEAR->GetActorLocation(), OutPointOnBody);
	// return OutPointOnBody;
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

// void ADraggableObject::UpdateBoxCollision()
// {
// 	BoxCollision->SetWorldLocation(GetActorLocation() + BoxCollisionInitialOffset);
// 	BoxCollision->SetWorldRotation(FRotator::ZeroRotator);
// }

void ADraggableObject::BeginPlay()
{
	Super::BeginPlay();

	BEAR = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	// BoxCollision = Cast<UBoxComponent>(Util::GetComponentByName(this, UBoxComponent::StaticClass(), "BoxCollision"));
	// BoxCollisionInitialOffset = BoxCollision->GetRelativeLocation();

	//UpdateBoxCollision();
}

// void ADraggableObject::NotifyActorBeginOverlap(AActor* OtherActor)
// {
// 	Super::NotifyActorBeginOverlap(OtherActor);
//
// 	if(Util::IsFloor(OtherActor))
// 	{
// 		FloorActors.Add(OtherActor);
// 	}
// }
//
// void ADraggableObject::NotifyActorEndOverlap(AActor* OtherActor)
// {
// 	Super::NotifyActorEndOverlap(OtherActor);
//
// 	if(Util::IsFloor(OtherActor))
// 	{
// 		FloorActors.Remove(OtherActor);
// 	}
// }

// void ADraggableObject::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
// {
// 	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
//
// 	if(IsDrag)
// 	{
// 		const auto Bear = Cast<ABEARCharacter>(BEAR);
// 		
// 		//Character
// 		if(Other == BEAR && Bear->IsPull())
// 		{
// 			MoveBack();
// 		}
// 		//Obstacle
// 		else if(Obstacles.Contains(Other) && Bear->IsPush())
// 		{
// 			MoveBack();
// 		}
// 	}
// }

void ADraggableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// UpdateBoxCollision();
	//
	// if(IsDrag && FloorActors.Num() == 0)
	// {
	// 	Cast<ABEARCharacter>(BEAR)->StopInteract();
	// }
}
