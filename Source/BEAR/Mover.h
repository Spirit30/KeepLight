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

	protected:

	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void Tick(float DeltaTime) override;

	private:

	float StartDistance;
	UStaticMeshComponent* TargetView;

	FVector GetDestination();
};
