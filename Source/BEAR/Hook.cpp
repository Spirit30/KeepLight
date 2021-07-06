// Fill out your copyright notice in the Description page of Project Settings.


#include "Hook.h"

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

void AHook::OnTakeTrigger(UPrimitiveComponent* ThisTrigger, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Logger::ToScreen("CONTACT: " + OtherActor->GetName(), 2.0f, FColor::Red);
	
	if(DropActors.Contains(OtherActor))
	{
		RedirectVertical();
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
