// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Excavator.generated.h"

UCLASS()
class BEAR_API AExcavator : public AActor
{
	GENERATED_BODY()
	
	public:

	UPROPERTY(EditAnywhere)
	float Speed = 10.0f;
	
	UFUNCTION(BlueprintCallable)
	void SetBearInCabine(bool Flag);
	
	UFUNCTION(BlueprintCallable)
	void SetCanMove(bool Flag);
	
	UFUNCTION(BlueprintCallable)
	void SetAnimatingLader(bool Flag);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayWheelsMoveAnimation();

	UFUNCTION(BlueprintImplementableEvent)
	void StopWheelsMoveAnimation();
	
	AExcavator();

	protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	private:

	bool IsBearInCabine;
	bool IsCanMove;
	bool IsAnimatingLader;

	bool IsMove() const;
	void UpdateWheelsMoveAnimation();
};
