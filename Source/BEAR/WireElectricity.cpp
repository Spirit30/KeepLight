// Fill out your copyright notice in the Description page of Project Settings.


#include "WireElectricity.h"
#include "CableComponent.h"
#include "Util.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

AWireElectricity::AWireElectricity()
{
	PrimaryActorTick.bCanEverTick = true;

}

TArray<ACableActor*> AWireElectricity::GetCables()
{
	return Cables;
}

void AWireElectricity::ClearPath()
{
	Path.Empty();
	PathZonesNum.Empty();
}

void AWireElectricity::SetPath(TArray<FVector> Points)
{
	Path = Points;
}

void AWireElectricity::AddPath(TArray<FVector> Points)
{
	Path.Append(Points);
}

void AWireElectricity::SetDirection(int32 Direction)
{
	DirectionSign = Direction;
}

void AWireElectricity::Init(FVector StartLocation)
{
	SetActorLocation(StartLocation);
	
	DirectionSign = 1;
	CurrentZoneIndex = 0;
	TargetPointIndex = 0;

	UpdatePath();

	OnThisEnterZoneIndex0();

	Activate();
}

void AWireElectricity::OnBearEnterZoneIndex1()
{
	if(IsActorTickEnabled() && CurrentZoneIndex == 0)
	{
		MinPointIndex = TargetPointIndex;
		MaxPointIndex = PathZonesNum[0] + PathZonesNum[1];
		SetDirection(1);
	}
}

void AWireElectricity::OnBearExitZoneIndex1()
{
	if(IsActorTickEnabled() && CurrentZoneIndex == 1)
	{
		MinPointIndex = 0;
		MaxPointIndex = CurrentZoneIndex;
		SetDirection(-1);
	}
}

void AWireElectricity::OnBearEnterZoneIndex2()
{
	if(IsActorTickEnabled() && CurrentZoneIndex == 1)
	{
		MinPointIndex = CurrentZoneIndex;
		MaxPointIndex = PathZonesNum[0] + PathZonesNum[1] + PathZonesNum[2];
		SetDirection(1);
	}
}

void AWireElectricity::OnBearExitZoneIndex2()
{
	if(IsActorTickEnabled() && CurrentZoneIndex == 1)
	{
		MinPointIndex = 0;
		MaxPointIndex = CurrentZoneIndex;
		SetDirection(-1);
	}
}

void AWireElectricity::Deactivate()
{
	SetActorTickEnabled(false);
	Emitter->Deactivate();
	AudioComponent->SetPaused(true);
	LightOfLantern->SetHiddenInGame(true);
}

void AWireElectricity::BeginPlay()
{
	Super::BeginPlay();

	AudioComponent = UGameplayStatics::SpawnSoundAttached(Sound, GetRootComponent());
	LightOfLantern = Cast<UStaticMeshComponent>(Util::GetComponentByName(Lantern, "Light"));
	
	Deactivate();
}

void AWireElectricity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdatePath();

	if(Path.Num() > 0)
	{
		const auto CurrentLocation = GetActorLocation();
		const auto TargetPoint = GetTargetPoint();
		const float Dist = FVector::Dist(CurrentLocation, TargetPoint);

		if(Dist > ReachDestinationDist)
		{
			const float Progress = Speed * DeltaTime / Dist;
			const auto Location = FMath::Lerp(CurrentLocation, TargetPoint, Progress);
			SetActorLocation(Location);
		}
		else
		{
			SetNextTargetPoint();
		}
	}

	//Logger::ToScreen("ZONE: ", (float)CurrentZoneIndex, DeltaTime);
}

void AWireElectricity::Activate()
{
	SetActorTickEnabled(true);
	Emitter->Activate();
	AudioComponent->SetPaused(false);
	LightOfLantern->SetHiddenInGame(false);
}

void AWireElectricity::UpdatePath()
{
	ClearPath();

	for(const auto CableActor : GetCables())
	{
		TArray<FVector> Points;
		CableActor->CableComponent->GetCableParticleLocations(Points);
		AddPath(Points);
		PathZonesNum.Add(Points.Num());
	}
}

FVector AWireElectricity::GetTargetPoint() const
{
	return Path[TargetPointIndex];
}

void AWireElectricity::SetNextTargetPoint()
{
	if(TargetPointIndex >= MaxPointIndex - 1)
	{
		DirectionSign = -1;
	}
	else if(TargetPointIndex <= MinPointIndex)
	{
		DirectionSign = 1;
	}

	TargetPointIndex += DirectionSign;

	int32 ZoneIndex = FindCurrentZoneIndex();

	if(ZoneIndex != CurrentZoneIndex)
	{
		switch (ZoneIndex)
		{
			case 0:
				OnThisEnterZoneIndex0();
				break;

			case 1:
				OnThisEnterZoneIndex1();
				break;

			default:
				break;
		}
		
		CurrentZoneIndex = ZoneIndex;
	}
}

int32 AWireElectricity::FindCurrentZoneIndex()
{
	for(int32 z = 0; z < Cables.Num(); ++z)
	{
		TArray<FVector> ZonePath;
		Cables[z]->CableComponent->GetCableParticleLocations(ZonePath);

		if(ZonePath.Contains(GetTargetPoint()))
		{
			return  z;
		}
	}

	return -1;
}

void AWireElectricity::OnThisEnterZoneIndex0()
{
	MinPointIndex = 0;
	MaxPointIndex = PathZonesNum[0];
}

void AWireElectricity::OnThisEnterZoneIndex1()
{
	MinPointIndex = PathZonesNum[0];
	MaxPointIndex = PathZonesNum[0] + PathZonesNum[1];
}