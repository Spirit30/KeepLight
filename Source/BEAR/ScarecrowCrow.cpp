// Fill out your copyright notice in the Description page of Project Settings.

#include "ScarecrowCrow.h"

#include "Util.h"
#include "Kismet/GameplayStatics.h"

AScarecrowCrow::AScarecrowCrow()
{
	PrimaryActorTick.bCanEverTick = true;

	State = Calm;
}

void AScarecrowCrow::BeginPlay()
{
	Super::BeginPlay();

	BearCollision = Cast<USphereComponent>(Util::GetComponentByName(this, BearCollisionName));

	InitialLocation = GetActorLocation();
	InitialBearCollisionRadius = BearCollision->GetUnscaledSphereRadius();
}

void AScarecrowCrow::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	switch (State)
	{
		case Calm:
			OnCalm(OtherActor);
			break;

		case Chase:
			OnChase(OtherActor);
			break;

		case Back:
			OnCalm(OtherActor);
			OnCalm(0.0f);
			break;

		default: ;
	}
}

void AScarecrowCrow::Tick(float DeltaSeconds)
{
	if(Bear->IsKilled())
	{
		OnStop();
	}
	
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
		
		case Leave:
			OnLeave(DeltaSeconds);
			break;
		
		default: ;
	}
	
	IsAttackAnimation = State > Calm;
}

void AScarecrowCrow::RemoveDeath()
{
	if(Death)
	{
		Death->Destroy();
	}
}

inline void AScarecrowCrow::OnCalm(AActor* OtherActor)
{
	if(OtherActor == Bear)
	{
		if(Hat->IsAttached())
		{
			RemoveDeath();
			Bear->DisableInput(GetWorld()->GetFirstPlayerController());
		}
		
		AttackTimer = AttackDelay;
		State = Angry;
		
		EnableControlPoint(false);

		//Sound
		UGameplayStatics::PlaySoundAtLocation(this, CrowSound, GetActorLocation(), FRotator::ZeroRotator, SoundVolume);
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
	MoveSmoothTo(Bear->GetHeadLocation(), DeltaSeconds);
}

void AScarecrowCrow::OnBack(float DeltaSeconds)
{
	MoveSmoothTo(InitialLocation, DeltaSeconds);

	const float Dist = FVector::Dist(GetActorLocation(), InitialLocation);

	if(Dist < BackDistance)
	{
		OnReturnBack();
	}
}

void AScarecrowCrow::OnLeave(float DeltaSeconds)
{
	const auto Location = GetActorLocation() + LeaveDirection + HatOffset;
	MoveSmoothTo(Location, DeltaSeconds);
	
	const auto HatLocation = GetActorLocation() + HatOffset;
	Hat->SetActorLocation(HatLocation);
}

void AScarecrowCrow::OnAttack()
{
	State = Chase;
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
		if(Hat->IsAttached())
		{
			OnLeave();
		}
		else
		{
			//Play to End
			HatSecuence->GetSequencePlayer()->Play();
		}
	}

	//Sound
	UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation(), FRotator::ZeroRotator, SoundVolume);

	//Reduce collision radius to prevent Bear trigger Chase again.
	BearCollision->SetSphereRadius(InitialBearCollisionRadius * 0.5f);
}

void AScarecrowCrow::OnReturnBack()
{
	SetActorLocation(InitialLocation);
	State = Calm;

	BearCollision->SetSphereRadius(InitialBearCollisionRadius);
}

void AScarecrowCrow::OnLeave()
{
	Hat->GrabByCrow();
	State = Leave;
	
	EnableControlPoint(true);

	Bear->EnableInput(GetWorld()->GetFirstPlayerController());
}

void AScarecrowCrow::OnStop()
{
	GetComponentByClass(USkeletalMeshComponent::StaticClass())->SetComponentTickEnabled(false);
	SetActorTickEnabled(false);
}

void AScarecrowCrow::MoveSmoothTo(FVector TargetLocation, float DeltaSeconds)
{
	const float Dist = FVector::Dist(GetActorLocation(), TargetLocation);
	const auto Location = FMath::Lerp(GetActorLocation(), TargetLocation, DeltaSeconds * FMath::Max(MinSpeed, SpeedDistCoef * Dist));
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

void AScarecrowCrow::EnableControlPoint(bool Flag)
{
	const auto NextControlPointBox = Cast<UBoxComponent>(Util::GetComponentByName(NextControlPoint, "Box"));
	NextControlPointBox->SetCollisionEnabled(Flag ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
}