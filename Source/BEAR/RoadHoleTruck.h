// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "BEARCharacter.h"
#include "DraggableObject.h"
#include "LevelSequenceActor.h"
#include "Components/ActorComponent.h"
#include "RoadHoleTruck.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BEAR_API URoadHoleTruck : public UActorComponent
{
	GENERATED_BODY()

	public:

	UPROPERTY(EditAnywhere)
	ABEARCharacter* Bear;

	UPROPERTY(EditAnywhere)
	TArray<ADraggableObject*> Rocks;

	UPROPERTY(EditAnywhere)
	ALevelSequenceActor* RoadHoleTruckSequence;

	UPROPERTY(EditAnywhere)
	TArray<float> SequenceTimePositions;

	UPROPERTY(EditAnywhere)
	float InRangeDist;

	UPROPERTY(EditAnywhere)
	FVector RangeDistOffsetPerItem;

	UPROPERTY(EditAnywhere)
	USoundWave* Creaking;

	UPROPERTY(EditAnywhere)
	float CreakingVolume;

	UPROPERTY(EditAnywhere)
	FVector CreakingLocation;

	UPROPERTY(EditAnywhere)
	float PassY;
	
	UPROPERTY(EditAnywhere)
	float SetPassedInitialDelay;
	
	URoadHoleTruck();

	protected:

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	private:

	int32 InRangeCount;
	bool IsDone;
	FTimerHandle SetPassedTimerHandle;

	FVector GetOriginForCurrentItem() const;
	void TrySetPassed();
};
