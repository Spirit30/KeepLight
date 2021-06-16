// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Raft.generated.h"

UCLASS()
class BEAR_API ARaft : public AActor
{
	GENERATED_BODY()
	
	public:	

	ARaft();
	UFUNCTION(BlueprintCallable)
	void ConstructRaft();

	protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	private:

	UBoxComponent* BoxCollision;
};
