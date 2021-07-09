// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"

#include "BEARCharacter.h"
#include "Mover.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Hook.generated.h"

UCLASS()
class BEAR_API AHook : public AActor
{
	GENERATED_BODY()
	
	public:

	UPROPERTY(EditAnywhere)
	TArray<AActor*> DropActors;

	UPROPERTY(EditAnywhere)
	TArray<FRotator> DropRotations;
	
	UPROPERTY(EditAnywhere)
	TArray<AActor*> TakeActors;

	UPROPERTY(EditAnywhere)
	AActor* DestinationArea;
	
	UPROPERTY(EditAnywhere)
	TArray<AMover*> Movers;
	
	UPROPERTY(EditAnywhere)
	float Speed = 100.0f;

	UPROPERTY(EditAnywhere)
	float TakenActorSpeed = 10.0f;

	UPROPERTY(EditAnywhere)
	USoundWave* MoveSound;

	AHook();

	UFUNCTION(BlueprintCallable)
	void OnHookButtonBegin();
	
	UFUNCTION(BlueprintCallable)
	void OnHookButtonEnd();

	UFUNCTION(BlueprintCallable)
	void SetPaused(bool Flag);

	UFUNCTION()
	void OnTakeTrigger(UPrimitiveComponent* ThisTrigger, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	private:

	USphereComponent* TakeTrigger;

	float TopLimit;
	float BottomLimit;
	float LeftLimit;
	float RightLimit;

	FVector Destination;
	float VerticalDestination;
	float HorizontalDestination;

	enum HookState
	{
		Calm,
		MoveHorizontal,
		MoveVertical
	};
	
	HookState State;

	UAudioComponent* MoveAudioComponent;
	AActor* TakenActor;
	bool IsReadyToDrop;
	bool IsPausedFlag;

	void SetState(HookState NewState);
	void RedirectVertical();
	void RedirectHorizontal();
	void TryEnablePhysics(AActor* Actor, bool Flag);
};
