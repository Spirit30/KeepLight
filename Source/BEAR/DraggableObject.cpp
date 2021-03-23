// Fill out your copyright notice in the Description page of Project Settings.


#include "DraggableObject.h"

// void ADraggableObject::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
// {
// 	++EnvironmentsCount;
// 	
// 	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan,  FString::Printf(TEXT("NAME: %s, COUNT: %d"), *OtherActor->GetName(), EnvironmentsCount));
// }
//
// void ADraggableObject::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
// {
// 	--EnvironmentsCount;
// 	
// 	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Magenta,  FString::Printf(TEXT("NAME: %s, COUNT: %d"), *OtherActor->GetName(), EnvironmentsCount));
// }
//
// bool ADraggableObject::IsGround() const
// {
// 	return EnvironmentsCount > 0;
// }
//
// void ADraggableObject::BeginPlay()
// {
// 	Super::BeginPlay();
//
// 	auto Sphere = FindGroundCollision();
// 	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ADraggableObject::OnOverlapBegin);
// 	Sphere->OnComponentEndOverlap.AddDynamic(this, &ADraggableObject::OnOverlapEnd);
// }
//
// USphereComponent* ADraggableObject::FindGroundCollision() const
// {
// 	TArray<USphereComponent*> Components;
// 	GetComponents<USphereComponent>(Components);
//
// 	for(auto Component : Components)
// 	{
// 		if(Component->GetName() == "GroundCollision")
// 		{
// 			return Cast<USphereComponent>(Component);
// 		}
// 	}
//
// 	return nullptr;
// }
