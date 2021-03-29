// Fill out your copyright notice in the Description page of Project Settings.


#include "RoadHoleTruck.h"

//#include "DrawDebugHelpers.h"

URoadHoleTruck::URoadHoleTruck()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URoadHoleTruck::BeginPlay()
{
	Super::BeginPlay();

	// const auto Player = RoadHoleTruckSequence->GetSequencePlayer();
	// Player->Play();
	// Player->Pause();
}

// Called every frame
void URoadHoleTruck::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(!IsDone)
	{
		int32 NewInRangeCount = 0;
		const auto Player = RoadHoleTruckSequence->GetSequencePlayer();
		const auto OriginLocation = GetOriginForCurrentItem();

		//DrawDebugSphere(GetWorld(), OriginLocation, InRangeDist, 4, FColor::Red, false, DeltaTime);

		for(const auto Rock : Rocks)
		{
			const float RockDist = FVector::Distance(OriginLocation, Rock->GetActorLocation());

			if(RockDist < InRangeDist)
			{
				++NewInRangeCount;
			}
		}

		const float BearDist = FVector::Distance(OriginLocation, Bear->GetActorLocation());

		if(BearDist < InRangeDist)
		{
			++NewInRangeCount;
		}

		if(NewInRangeCount != InRangeCount)
		{		
			if(NewInRangeCount > 0)
			{
				const int InRangeIndex = NewInRangeCount - 1;
			
				if(InRangeIndex < SequenceTimePositions.Num())
				{
					const float PositionTime = SequenceTimePositions[InRangeIndex];
					const auto Position = FMovieSceneSequencePlaybackParams(PositionTime,EUpdatePositionMethod::Play);
					Player->PlayTo(Position);
				}
				else
				{
					Player->Play();
					IsDone = true;
				}
			}

			InRangeCount = NewInRangeCount;

			//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, FString::Printf(TEXT("InRangeCount: %d"), InRangeCount));
		}
	}
}

FVector URoadHoleTruck::GetOriginForCurrentItem() const
{
	return GetOwner()->GetActorLocation() + RangeDistOffsetPerItem * InRangeCount;
}

