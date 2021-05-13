// Copyright Epic Games, Inc. All Rights Reserved.

#include "BEARCharacter.h"

#include "DrawDebugHelpers.h"
#include "Logger.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ABEARCharacter::ABEARCharacter()
{
	//Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	//Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//Init values
	MinWalkSpeed = 120.0f;
	MaxWalkSpeed = 600.0f;

	//Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.0f; // Default = 2
	GetCharacterMovement()->AirControl = 0.8f;
	GetCharacterMovement()->JumpZVelocity = 1000.0f;
	GetCharacterMovement()->GroundFriction = 3.0f;
	GetCharacterMovement()->MaxWalkSpeed = 	MaxWalkSpeed;
	GetCharacterMovement()->MaxFlySpeed = 600.0f;

	//Tick Group
	SetTickGroup(TG_PrePhysics);
}

FVector ABEARCharacter::GetHeadLocation() const
{
	return GetMesh()->GetSocketLocation("HeadSocket");
}

bool ABEARCharacter::IsKilled() const
{
	return  IsKilledFlag;
}

void ABEARCharacter::Kill(bool HasEffect, FVector Point)
{
	IsKilledFlag = true;
	
	GetMesh()->SetComponentTickEnabled(false);
	SetActorTickEnabled(false);
	GetCharacterMovement()->Deactivate();
	DestroyPlayerInputComponent();

	if(HasEffect)
	{
		const auto DeathEffect = GetWorld()->SpawnActor(DeathEffectClass);
		DeathEffect->SetActorLocation(Point);
	}
}

bool ABEARCharacter::IsPush() const
{
	return IsDrag && MoveDirection > 0;
}

bool ABEARCharacter::IsPull() const
{
	return IsDrag && MoveDirection <= 0;
}

void ABEARCharacter::BeginPlay()
{
	Super::BeginPlay();

	ResetFallingTimer();
}

void ABEARCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	//Set up gameplay key bindings
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABEARCharacter::TryJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABEARCharacter::MoveRight);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ABEARCharacter::Interact);
	PlayerInputComponent->BindAction("Interact", IE_Released, this, &ABEARCharacter::StopInteract);
}

void ABEARCharacter::NotifyActorBeginOverlap(AActor* OtherActor)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, OtherActor->GetName());

	const auto CloseDragObject = Cast<ADraggableObject>(OtherActor);

	if(CloseDragObject)
	{
		RightHandRotation = CloseDragObject->RightHandDragRotation;
		LeftHandRotation = CloseDragObject->LeftHandDragRotation;

		CloseDragObjects.Add(CloseDragObject);
	}

	const auto CloseInteractable = Cast<AInteractable>(OtherActor);

	if(CloseInteractable)
	{
		CloseInteractables.Add(CloseInteractable);
	}
}

void ABEARCharacter::NotifyActorEndOverlap(AActor* OtherActor)
{
	if(OtherActor == ActiveDragObject)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, OtherActor->GetName());

		StopInteract();
	}

	if(CloseDragObjects.Contains(OtherActor))
	{
		const auto CloseDragObject = Cast<ADraggableObject>(OtherActor);
		
		CloseDragObjects.Remove(CloseDragObject);
	}
}

void ABEARCharacter::Tick(float DeltaSeconds)
{
	if(IsDrag)
	{
		//GEngine->AddOnScreenDebugMessage(-1, DeltaSeconds * 2, FColor::Magenta, FString::Printf(TEXT("Velocity: %f"), GetVelocity().Size()));

		if(CanDrag())
		{
			ActiveDragObject->Drag(DeltaSeconds);

			RightHandLocation = ActiveDragObject->GetRightHandLocation();
			LeftHandLocation = ActiveDragObject->GetLeftHandLocation();
			IsIK = true;
		}
		else
		{
			StopInteract();
		}
	}
	else if(ActiveInteractable)
	{
		if(ActiveInteractable->IsInteracting())
		{
			RightHandLocation = ActiveInteractable->GetRightHandLocation();
			LeftHandLocation = ActiveInteractable->GetLeftHandLocation();
			IsIK = true;
		}
		else
		{
			StopInteract();
			ActiveInteractable= nullptr;
		}
	}
	else
	{
		IsIK = false;
	}

	//Fix stack falling
	if(GetCharacterMovement()->IsFalling())
	{
		IsStack = FallingTimer <= 0;	
		FallingTimer -= DeltaSeconds;
	}
}

void ABEARCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	
	ResetFallingTimer();
}

void ABEARCharacter::MoveRight(float Value)
{
	if(!ActiveInteractable || ActiveInteractable->CanMoveCharacter())
	{
		InputDirection = Value;
		
		const auto MoveDirectionVector = FVector(0.0f,-1.0f,0.0f);
	
		// add movement in that direction
		AddMovementInput(MoveDirectionVector, Value);

		//GEngine->AddOnScreenDebugMessage(-1, GetWorld()->GetDeltaSeconds() * 2, FColor::Magenta, FString::Printf(TEXT("DIR: %f, %f, %f"), GetActorForwardVector().X, GetActorForwardVector().Y, GetActorForwardVector().Z));
		const float CharacterDirectionCoef = GetActorForwardVector().Y <= 0 ? 1 : -1;
		MoveDirection = FMath::Sign(Value) * CharacterDirectionCoef;
	}
}

void ABEARCharacter::Interact()
{
	if(CanDrag())
	{
		GetCharacterMovement()->MaxWalkSpeed = 	MinWalkSpeed;
		GetCharacterMovement()->bOrientRotationToMovement = false;

		IsDrag = true;
	}
	else if(CanInteract())
	{
		ActiveInteractable->Interact();
	}
}

void ABEARCharacter::StopInteract()
{
	if(IsDrag)
	{
		ActiveDragObject->StopDrag();

		GetCharacterMovement()->MaxWalkSpeed = 	MaxWalkSpeed;
		GetCharacterMovement()->bOrientRotationToMovement = true;

		IsDrag = false;
		ActiveDragObject = nullptr;
	}
}

void ABEARCharacter::TryJump()
{
	if(IsStack)
	{
		GetCharacterMovement()->AddImpulse(DeStackForce, true);
			
		IsStack = false;

		ResetFallingTimer();
	}
	else if(!IsDrag)
	{
		Jump();
	}
}

bool ABEARCharacter::CanDrag()
{
	for(auto PotentialDragObject : CloseDragObjects)
	{
		if(PotentialDragObject->CanDrag())
		{
			ActiveDragObject = PotentialDragObject;
			return true;
		}
	}

	return false;
}

bool ABEARCharacter::CanInteract()
{
	for(auto PotentialInteractable : CloseInteractables)
	{
		if(PotentialInteractable->CanInteract())
		{
			const auto Direction = (PotentialInteractable->GetActorLocation() - GetActorLocation()).GetUnsafeNormal();
		
			if(FVector::DotProduct(GetActorForwardVector(), Direction) > PotentialInteractable->GetDragDot())
			{
				ActiveInteractable = PotentialInteractable;
				return true;
			}
		}
	}

	return false;
}

void ABEARCharacter::ResetFallingTimer()
{
	FallingTimer = MaxFallingTime;
}