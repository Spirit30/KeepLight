// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Mover.generated.h"

UCLASS()
class BEAR_API AMover : public AActor
{
	GENERATED_BODY()
	
	public:

	UPROPERTY(EditAnywhere)
	bool AutoMove;

	UPROPERTY(EditAnywhere)
	bool Interpolate = true;

	UPROPERTY(EditAnywhere)
	AActor* Target;

	UPROPERTY(EditAnywhere)
	AActor* DestinationActor;

	UPROPERTY(EditAnywhere)
	FVector LocalDestination;
	
	UPROPERTY(EditAnywhere)
	float Speed = 1.0f;

	UPROPERTY(EditAnywhere)
	float ReachDestinationDist = 10.0f;

	UPROPERTY(EditAnywhere)
	bool ShouldRotate;

	UPROPERTY(EditAnywhere)
	FRotator RotationDestination;
	
	AMover();

	UFUNCTION(BlueprintCallable)
	void StartMove();
	UFUNCTION(BlueprintCallable)
	void StopMove();
	bool IsMove() const;

	protected:

	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void Tick(float DeltaTime) override;

	private:

	float StartDistance;
	TArray<UStaticMeshComponent*> TargetViews;
	bool IsMoveFlag;

	FVector GetDestination();
	void MovementEnd();
};
