// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "BEARCharacter.h"
#include "Interactable.h"
#include "ScarecrowHat.generated.h"

/**
 * 
 */
UCLASS()
class BEAR_API AScarecrowHat : public AInteractable
{
	GENERATED_BODY()

	public:

	UPROPERTY(EditAnywhere)
	ABEARCharacter* Bear;

	UPROPERTY(EditAnywhere)
	FName TargetSocketName;

	UPROPERTY(EditAnywhere)
	FVector HeadOffset = FVector(0, 0, 250);

	UPROPERTY(EditAnywhere)
	AActor* HatMeshActor;

	protected:

	virtual void BeginPlay() override;
	virtual void Interact() override;
	virtual void Tick(float DeltaSeconds) override;

	private:

	UStaticMeshComponent* HatMesh;
};
