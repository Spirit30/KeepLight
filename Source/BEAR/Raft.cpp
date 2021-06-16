// Fill out your copyright notice in the Description page of Project Settings.


#include "Raft.h"

ARaft::ARaft()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ARaft::ConstructRaft()
{
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ARaft::BeginPlay()
{
	Super::BeginPlay();

	BoxCollision = Cast<UBoxComponent>(GetComponentByClass(UBoxComponent::StaticClass()));
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ARaft::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

