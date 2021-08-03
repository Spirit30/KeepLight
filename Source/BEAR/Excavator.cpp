// Fill out your copyright notice in the Description page of Project Settings.


#include "Excavator.h"

void AExcavator::SetBearInCabine(bool Flag)
{
	IsBearInCabine = Flag;

	UpdateWheelsMoveAnimation();
}

void AExcavator::SetCanMove(bool Flag)
{
	IsCanMove = Flag;

	UpdateWheelsMoveAnimation();
}

void AExcavator::SetAnimatingLader(bool Flag)
{
	IsAnimatingLader = Flag;

	UpdateWheelsMoveAnimation();
}

AExcavator::AExcavator()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AExcavator::BeginPlay()
{
	Super::BeginPlay();

	SetCanMove(true);
}

void AExcavator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(IsMove())
	{
		auto Location = GetActorLocation();
		Location.Y -= DeltaTime * Speed;
		SetActorLocation(Location);
	}
}

bool AExcavator::IsMove() const
{
	return  IsBearInCabine && IsCanMove && !IsAnimatingLader;
}

void AExcavator::UpdateWheelsMoveAnimation()
{
	if(IsMove())
	{
		PlayWheelsMoveAnimation();
	}
	else
	{
		StopWheelsMoveAnimation();
	}
}

