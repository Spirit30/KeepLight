// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "BEARCharacter.h"
#include "GameFramework/Actor.h"
#include "ScarecrowCrow.generated.h"

UCLASS()
class BEAR_API AScarecrowCrow : public AActor
{
	GENERATED_BODY()
	
	public:

	UPROPERTY(EditAnywhere)
	FName TargetSocketName;

	UPROPERTY(EditAnywhere)
	float Speed;

	UPROPERTY(EditAnywhere)
	float AttackDelay;

	//Used by ScarecrowCrow Animation Blueprint
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool IsAttackAnimation;
	
	AScarecrowCrow();
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void Tick(float DeltaSeconds) override;

	private:

	float AttackTimer;

	ABEARCharacter* Bear;
};
