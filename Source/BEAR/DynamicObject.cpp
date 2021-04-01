// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicObject.h"

#include "Kismet/GameplayStatics.h"

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
	//StaticMeshComponent->OnComponentHit.AddDynamic(this, &ADynamicObject::NotifyHit)
}

void ADynamicObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADynamicObject::NotifyHit(UPrimitiveComponent* Comp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	// if(Other)
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Cyan, FString::Printf(TEXT("HIT: %s, IMPULSE: %f"), *Other->GetName(), NormalImpulse.SizeSquared()));
	// }
	// else
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, "Unknown HIT");
	// }
	
	if(HitSound && NormalImpulse.SizeSquared() > HitSoundImpulseThreshold)
	{
		//Sound
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, Comp->GetComponentLocation(), FRotator::ZeroRotator);
	}
}

