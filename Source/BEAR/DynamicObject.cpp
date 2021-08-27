// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicObject.h"

#include "Util.h"
#include "Kismet/GameplayStatics.h"

ADynamicObject::ADynamicObject()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SetTickGroup(TG_PrePhysics);
}

UStaticMeshComponent* ADynamicObject::GetStaticMeshComponent() const
{
	return StaticMeshComponent;
}

void ADynamicObject::BeginPlay()
{
	Super::BeginPlay();

	StaticMeshComponent = Cast<UStaticMeshComponent>(Util::GetStaticMeshComponentByName(this, "View"));

	if(!StaticMeshComponent)
	{
		StaticMeshComponent = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));
	}
}

void ADynamicObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(IsFalling())
	{		
		StaticMeshComponent->AddForce(ExtraGravity * DeltaTime);
	}
}

void ADynamicObject::NotifyHit(UPrimitiveComponent* Comp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(Comp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	
	const float CurrentTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
	const float IntervalSinceLastHitSound = CurrentTime - LastHitSoundTime;
	
	if(HitSound && NormalImpulse.SizeSquared() > HitSoundImpulseThreshold && IntervalSinceLastHitSound > MinHitSoundInteval)
	{
		LastHitSoundTime = CurrentTime;
	
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, Comp->GetComponentLocation(), FRotator::ZeroRotator, HitSoundVolume);
	}
}

bool ADynamicObject::IsFalling() const
{
	return StaticMeshComponent->GetComponentVelocity().Z < IsFallingThresholdZ;
}
