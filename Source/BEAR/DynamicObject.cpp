// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicObject.h"

ADynamicObject::ADynamicObject()
{
	PrimaryActorTick.bCanEverTick = true;
	SetTickGroup(TG_DuringPhysics);
}

UStaticMeshComponent* ADynamicObject::GetStaticMeshComponent()
{
	return StaticMeshComponent;
}

void ADynamicObject::BeginPlay()
{
	Super::BeginPlay();

	StaticMeshComponent = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));
}

void ADynamicObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// if(StaticMeshComponent->GetComponentVelocity().SizeSquared() > CalmDownSquaredVelocity)
	// {
	// 	StaticMeshComponent->AddForce(Gravity);
	// }
}

