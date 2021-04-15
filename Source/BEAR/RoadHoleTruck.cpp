// Fill out your copyright notice in the Description page of Project Settings.


#include "RoadHoleTruck.h"

//#include "DrawDebugHelpers.h"
#include "MovieSceneSequencePlayer.h"
#include "Kismet/GameplayStatics.h"

URoadHoleTruck::URoadHoleTruck()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URoadHoleTruck::BeginPlay()
{
	Super::BeginPlay();
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
			if(NewInRangeCount < SequenceTimePositions.Num())
			{
				//Play to Position
				const float PositionTime = SequenceTimePositions[NewInRangeCount];
				const auto Position = FMovieSceneSequencePlaybackParams(PositionTime, EUpdatePositionMethod::Play);
				Player->PlayTo(Position);
			}
			else
			{
				//Play to End
				Player->Play();
				IsDone = true;
			}

			InRangeCount = NewInRangeCount;

			//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, FString::Printf(TEXT("InRangeCount: %d"), InRangeCount));

			//Sound
			UGameplayStatics::PlaySoundAtLocation(this, Creaking, CreakingLocation, FRotator::ZeroRotator, CreakingVolume);
		}

		TrySetPassed();
	}
}


FVector URoadHoleTruck::GetOriginForCurrentItem() const
{
	return GetOwner()->GetActorLocation() + RangeDistOffsetPerItem * InRangeCount;
}

void URoadHoleTruck::TrySetPassed()
{
	const auto Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if(Character->GetActorLocation().Y < PassY)
	{
		const auto Player = RoadHoleTruckSequence->GetSequencePlayer();
		const auto Position = FMovieSceneSequencePlaybackParams(Player->GetDuration().AsSeconds(), EUpdatePositionMethod::Jump);
		Player->SetPlaybackPosition(Position);
		IsDone = true;
	}
}
