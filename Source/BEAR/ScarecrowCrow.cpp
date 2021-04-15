// Fill out your copyright notice in the Description page of Project Settings.

#include "ScarecrowCrow.h"

AScarecrowCrow::AScarecrowCrow()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AScarecrowCrow::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Bear = Cast<ABEARCharacter>(OtherActor);

	if(Bear)
	{
		IsAttackAnimation = true;
		AttackTimer = AttackDelay;
	}
}

void AScarecrowCrow::Tick(float DeltaSeconds)
{
	if(Bear)
	{
		if(AttackTimer <= 0)
		{
			const auto TargetLocation = Bear->GetMesh()->GetSocketLocation(TargetSocketName);
			const auto Location = FMath::Lerp(GetActorLocation(), TargetLocation, DeltaSeconds * Speed);
			SetActorLocation(Location);
		}
		else
		{
			AttackTimer -= DeltaSeconds;
		}
	}
}
