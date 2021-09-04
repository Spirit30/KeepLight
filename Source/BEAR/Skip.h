// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Skip.generated.h"

UCLASS()
class BEAR_API ASkip : public AActor
{
	GENERATED_BODY()
	
	public:

	UPROPERTY(EditAnywhere)
	float TimeSecToSkip = 600.0f;

	UPROPERTY(EditAnywhere)
	int32 DeathsToSkip = 5;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> ConfirmPopupClass;
	
	ASkip();
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void ResetTimeAndDeaths();
	UFUNCTION(BlueprintCallable)
	void OnDeath();

	protected:

	virtual void BeginPlay() override;

	private:

	static int32 Deaths;
	float TimeSec;

	void ShowConfirmPopup();
};
