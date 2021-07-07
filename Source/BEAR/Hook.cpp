// Fill out your copyright notice in the Description page of Project Settings.


#include "Hook.h"

#include "DraggableObject.h"
#include "Logger.h"
#include "Util.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

AHook::AHook()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AHook::OnHookButtonBegin()
{
	if(State == Calm)
	{
		SetState(MoveHorizontal);
	}
}

void AHook::OnHookButtonEnd()
{
	if(State == MoveHorizontal)
	{
		SetState(MoveVertical);
	}
}

void AHook::SetPaused(bool Flag)
{
	IsPausedFlag = Flag;
}

void AHook::OnTakeTrigger(UPrimitiveComponent* ThisTrigger, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Logger::ToScreen("CONTACT: " + OtherActor->GetName(), 2.0f, FColor::Red);

	if(TakeActors.Contains(OtherActor))
	{
		if(!TakenActor)
		{
			// const int32 Index = TakeActors.IndexOfByKey(OtherActor);
			// if(!Movers[Index]->IsMove())
			// {
				TakenActor = OtherActor;
				IsReadyToDrop = false;

				TryEnablePhysics(TakenActor, false);

				Logger::ToScreen("TAKE: " + TakenActor->GetName(), 2.0f, FColor::Green);
			//}
		}
	}
	else if(OtherActor == DestinationArea)
	{
		if(TakenActor)
		{
			const int32 Index = TakeActors.IndexOfByKey(TakenActor);
			const auto Mover = Movers[Index];
			if(Mover)
			{
				Movers[Index]->StartMove();
			}

			TryEnablePhysics(TakenActor, true);

			Logger::ToScreen("DESTANATION: " + TakenActor->GetName(), 2.0f, FColor::Yellow);
			TakenActor = nullptr;
		}
	}
	else if(DropActors.Contains(OtherActor))
	{
		RedirectVertical();

		if(IsReadyToDrop)
		{
			if(TakenActor)
			{
				const int32 Index = TakeActors.IndexOfByKey(TakenActor);
				TakenActor->SetActorRotation(DropRotations[Index]);
				
				TryEnablePhysics(TakenActor, true);

				Logger::ToScreen("DROP: " + TakenActor->GetName(), 2.0f, FColor::Yellow);
				TakenActor = nullptr;
			}
		}
		else
		{
			IsReadyToDrop = true;
		}
	}
}

void AHook::BeginPlay()
{
	Super::BeginPlay();

	TakeTrigger = Cast<USphereComponent>(Util::GetComponentByName(this, "TakeTrigger"));
	TakeTrigger->OnComponentBeginOverlap.AddDynamic(this, &AHook::OnTakeTrigger);
	
	const auto MoveLimit = Cast<UBoxComponent>(Util::GetComponentByName(this, "MoveLimit"));
	MoveLimit->UpdateBounds();
	const auto MoveBoundsBox = MoveLimit->Bounds.GetBox();
	
	TopLimit = GetActorLocation().Z;
	BottomLimit = MoveBoundsBox.Min.Z;
	LeftLimit = MoveBoundsBox.Max.Y;
	RightLimit = MoveBoundsBox.Min.Y;
	
	VerticalDestination = TopLimit;
	HorizontalDestination = LeftLimit;
	SetState(Calm);
}

void AHook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(IsPausedFlag)
	{
		return;
	}

	auto Location = GetActorLocation();

	if(State == MoveHorizontal)
	{
		TakeTrigger->UpdateBounds();
		const auto TakeTriggerSphere = TakeTrigger->Bounds.GetSphere();

		//IF out of Horizontal Bounds
		if(Location.Y + TakeTriggerSphere.W > LeftLimit || Location.Y - TakeTriggerSphere.W < RightLimit)
		{
			RedirectHorizontal();
		}
	}

	if(State == MoveVertical)
	{
		//IF is going Up
		if(VerticalDestination == TopLimit)
		{
			TakeTrigger->UpdateBounds();
			const auto TakeTriggerSphere = TakeTrigger->Bounds.GetSphere();
			
			if(Location.Z + TakeTriggerSphere.W > TopLimit)
			{
				SetState(Calm);
			}
		}
	}

	const float Dist = FVector::Dist(Location, Destination);
	Location = FMath::Lerp(Location, Destination, Speed * DeltaTime / Dist);
	SetActorLocation(Location);

	if(TakenActor && IsReadyToDrop)
	{
		Location = FMath::Lerp(TakenActor->GetActorLocation(), TakeTrigger->GetComponentLocation(), TakenActorSpeed * DeltaTime);
		TakenActor->SetActorLocation(Location);
	}
}

void AHook::SetState(HookState NewState)
{
	State = NewState;

	if(State == Calm)
	{
		SetActorTickEnabled(false);

		Logger::ToScreen("New State - Calm");
	}
	else
	{
		Destination = GetActorLocation();

		if(State == MoveHorizontal)
		{
			RedirectHorizontal();

			Logger::ToScreen("New State - Move Horizontal");
		}
	
		if(State == MoveVertical)
		{
			RedirectVertical();

			Logger::ToScreen("New State - Move Vertical");
		}

		SetActorTickEnabled(true);
	}
}

void AHook::RedirectVertical()
{
	VerticalDestination = VerticalDestination == BottomLimit ? TopLimit : BottomLimit;
	Destination.Z = VerticalDestination;
	
	Logger::ToScreen("RedirectVertical - ", VerticalDestination, 2.0f);
}

void AHook::RedirectHorizontal()
{
	HorizontalDestination = HorizontalDestination == LeftLimit ? RightLimit : LeftLimit;
	Destination.Y = HorizontalDestination;

	Logger::ToScreen("RedirectHorizontal - ", HorizontalDestination, 2.0f);
}

void AHook::TryEnablePhysics(AActor* Actor, bool Flag)
{
	const auto Draggable = Cast<ADraggableObject>(Actor);

	if(Draggable)
	{
		const auto StaticMesh = Cast<UStaticMeshComponent>(Util::GetComponentByName(Draggable, "View"));
		StaticMesh->SetCollisionEnabled(Flag ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
		StaticMesh->SetSimulatePhysics(Flag);
		StaticMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
		StaticMesh->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
	}
}
