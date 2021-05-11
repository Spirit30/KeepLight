// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "StackObjectRemover.generated.h"

UCLASS()
class BEAR_API AStackObjectRemover : public AActor
{
	GENERATED_BODY()
	
	public:

	UPROPERTY(EditAnywhere)
	AActor* ActorToRemove;

	UPROPERTY(EditAnywhere)
	TArray<FVector> ActorRemovePath;

	UPROPERTY(EditAnywhere)
	float ReachDestinationDist = 2.0f;

	UPROPERTY(EditAnywhere)
	float RemoveSpeed = 20.0f;
	
	AStackObjectRemover();
	void StartRemove();

	protected:

	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void Tick(float DeltaTime) override;

	private:

	UStaticMeshComponent* Hand;

	int32 PathPointIndex;
	
	void OnNextPoint();
	void OnEndRemove() const;

	void SetTargetActorPhysicsEnabled(bool flag) const;
};
