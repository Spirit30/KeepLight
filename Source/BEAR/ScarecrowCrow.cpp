// Fill out your copyright notice in the Description page of Project Settings.

#include "ScarecrowCrow.h"
#include "Kismet/GameplayStatics.h"

AScarecrowCrow::AScarecrowCrow()
{
	PrimaryActorTick.bCanEverTick = true;

	State = Calm;
}

void AScarecrowCrow::BeginPlay()
{
	Super::BeginPlay();

	InitialLocation = GetActorLocation();
}

void AScarecrowCrow::NotifyActorBeginOverlap(AActor* OtherActor)
{
	switch (State)
	{
		case Calm:
			OnCalm(OtherActor);
			break;

		case Chase:
			OnChase(OtherActor);
			break;

		default: ;
	}
}

void AScarecrowCrow::Tick(float DeltaSeconds)
{
	switch (State)
	{
		case Calm:
			OnCalm(DeltaSeconds);
			break;
		
		case Angry:
			OnAngry(DeltaSeconds);
			break;

		case Chase:
			OnChase(DeltaSeconds);
			break;
			
		case Back:
			OnBack(DeltaSeconds);
			break;
		
		// case Leave:
		// 	OnLeave(DeltaSeconds);
		// 	break;
		
		default: ;
	}
	
	IsAttackAnimation = State > Calm;
}

inline void AScarecrowCrow::OnCalm(AActor* OtherActor)
{
	if(OtherActor == Bear)
	{
		AttackTimer = AttackDelay;
		State = Angry;
	}
}

inline void AScarecrowCrow::OnChase(AActor* OtherActor)
{
	if(OtherActor == Hat)
	{
		OnHitHat();	
	}
}

void AScarecrowCrow::OnCalm(float DeltaSeconds)
{
	UpdateRotation(Bear->GetActorLocation());
}

//Wait a delay before attack
void AScarecrowCrow::OnAngry(float DeltaSeconds)
{
	if(AttackTimer > 0)
	{
		AttackTimer -= DeltaSeconds;
	}
	else
	{
		OnAttack();
	}
}

//Follow Bear
void AScarecrowCrow::OnChase(float DeltaSeconds)
{
	const auto TargetLocation = Bear->GetMesh()->GetSocketLocation(TargetSocketName);
	MoveSmoothTo(TargetLocation, DeltaSeconds);
}

void AScarecrowCrow::OnBack(float DeltaSeconds)
{
	MoveSmoothTo(InitialLocation, DeltaSeconds);

	const float Dist = FVector::Dist(GetActorLocation(), InitialLocation);

	if(Dist < BackDistance)
	{
		SetActorLocation(InitialLocation);
		State = Calm;
	}
}

void AScarecrowCrow::OnLeave(float DeltaSeconds)
{
}

void AScarecrowCrow::OnAttack()
{
	State = Chase;

	//Sound
	UGameplayStatics::PlaySoundAtLocation(this, CrowSound, GetActorLocation(), FRotator::ZeroRotator, SoundVolume);
}

void AScarecrowCrow::OnHitHat()
{
	State = Back;

	if(CurrentHatHit < SequenceTimePositions.Num())
	{
		//Play to Position
		const float PositionTime = SequenceTimePositions[CurrentHatHit];
		const auto Position = FMovieSceneSequencePlaybackParams(PositionTime, EUpdatePositionMethod::Play);
		HatSecuence->GetSequencePlayer()->PlayTo(Position);
		++CurrentHatHit;
	}
	else
	{
		//Play to End
		HatSecuence->GetSequencePlayer()->Play();
	}

	//Sound
	UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation(), FRotator::ZeroRotator, SoundVolume);
}

void AScarecrowCrow::MoveSmoothTo(FVector TargetLocation, float DeltaSeconds)
{
	const float Dist = FVector::Dist(GetActorLocation(), TargetLocation);
	const auto Location = FMath::Lerp(GetActorLocation(), TargetLocation, DeltaSeconds * Speed * Dist);
	SetActorLocation(Location);

	UpdateRotation(TargetLocation);
}

//Update side rotation for situation when player jump over crow
void AScarecrowCrow::UpdateRotation(FVector TargetLocation)
{
	if(TargetLocation.Y > GetActorLocation().Y)
	{
		const auto LookLeft = FRotator::ZeroRotator;
		SetActorRotation(LookLeft);
	}
	else
	{
		const auto LookRight = FRotator(0, 180, 0);
		SetActorRotation(LookRight);
	}
}