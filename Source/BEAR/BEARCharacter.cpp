// Copyright Epic Games, Inc. All Rights Reserved.

#include "BEARCharacter.h"

#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ABEARCharacter::ABEARCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//Init values
	MinWalkSpeed = 120.0f;
	MaxWalkSpeed = 600.0f;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.0f; // Default = 2
	GetCharacterMovement()->AirControl = 0.8f;
	GetCharacterMovement()->JumpZVelocity = 1000.0f;
	GetCharacterMovement()->GroundFriction = 3.0f;
	GetCharacterMovement()->MaxWalkSpeed = 	MaxWalkSpeed;
	GetCharacterMovement()->MaxFlySpeed = 600.0f;

	//Tick Group
	SetTickGroup(TG_DuringPhysics);
}

//////////////////////////////////////////////////////////////////////////
// Input

void ABEARCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABEARCharacter::TryJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABEARCharacter::MoveRight);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ABEARCharacter::Interact);
	PlayerInputComponent->BindAction("Interact", IE_Released, this, &ABEARCharacter::StopInteract);
}

void ABEARCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	auto Capsule = GetCapsuleComponent();
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &ABEARCharacter::OnOverlapBegin);
	Capsule->OnComponentEndOverlap.AddDynamic(this, &ABEARCharacter::OnOverlapEnd);
}

void ABEARCharacter::Tick(float DeltaSeconds)
{
	if(IsDrag)
	{
		//GEngine->AddOnScreenDebugMessage(-1, DeltaSeconds * 2, FColor::Magenta, FString::Printf(TEXT("Velocity: %f"), GetVelocity().Size()));

		//Push
		if(MoveDirection > 0)
		{
			ActiveDragComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
			
			const float PushVelocity = ActiveDragComponent->GetComponentVelocity().SizeSquared();

			//GEngine->AddOnScreenDebugMessage(-1, DeltaSeconds * 2, FColor::Magenta, FString::Printf(TEXT("Velocity: %f"), PushVelocity));
		
			const float DragForce = 
                    //Bigger Dist means smaller Push Forces
                    FMath::Lerp(
                    	ActiveDragObject->MaxPushForce,
                    	ActiveDragObject->MinPushForce,
                    	FMath::Clamp(PushVelocity, ActiveDragObject->MinPushVelocity, ActiveDragObject->MaxPushVelocity) / ActiveDragObject->MaxPushVelocity);

			const auto Force = GetActorForwardVector() * DragForce * DeltaSeconds;
			ActiveDragComponent->AddForceAtLocation(Force, ActiveDragObject->GetActorLocation());
		}
		//Pull
		else
		{
			ActiveDragComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
			
			auto TargetDragLocation = GetActorLocation() + GetActorForwardVector() * ActiveDragObject->PullDistance;
			//Locked 2D
			TargetDragLocation.X = 0;
			//Controlled by Physics
			TargetDragLocation.Z = ActiveDragObject->GetActorLocation().Z;
			
			auto DragLocation = FMath::Lerp(ActiveDragObject->GetActorLocation(), TargetDragLocation, DeltaSeconds * DragDistanceLerpSpeed);
			//Locked 2D
			DragLocation.X = 0;
			
			ActiveDragObject->SetActorLocation(DragLocation);
		}
		
		RightHandLocation = ActiveDragComponent->GetComponentLocation() + ActiveDragObject->RightHandDragOffset;
		LeftHandLocation = ActiveDragComponent->GetComponentLocation() + ActiveDragObject->LeftHandDragOffset;
	}
}

void ABEARCharacter::MoveRight(float Value)
{
	const auto MoveDirectionVector = FVector(0.0f,-1.0f,0.0f);
	
	// add movement in that direction
	AddMovementInput(MoveDirectionVector, Value);

	//GEngine->AddOnScreenDebugMessage(-1, GetWorld()->GetDeltaSeconds() * 2, FColor::Magenta, FString::Printf(TEXT("DIR: %f, %f, %f"), GetActorForwardVector().X, GetActorForwardVector().Y, GetActorForwardVector().Z));
	const float CharacterDirectionCoef = GetActorForwardVector().Y <= 0 ? 1 : -1;
	MoveDirection = FMath::Sign(Value) * CharacterDirectionCoef;
}

void ABEARCharacter::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, OtherActor->GetName());

	const auto CloseDragObject = Cast<ADraggableObject>(OtherActor);

	if(CloseDragObject)
	{
		const auto CloseDragComponent = OtherComp; //Cast<UPrimitiveComponent>(DragObject->FindComponentByClass(UStaticMeshComponent::StaticClass()));

		RightHandRotation = CloseDragObject->RightHandDragRotation;
		LeftHandRotation = CloseDragObject->LeftHandDragRotation;

		CloseDragObjects.Add(CloseDragObject);
		CloseDragComponents.Add(CloseDragComponent);
	}
}

void ABEARCharacter::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherComp == ActiveDragComponent)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, OtherActor->GetName());

		StopInteract();
	}

	if(CloseDragComponents.Contains(OtherComp))
	{
		const auto CloseDragObject = Cast<ADraggableObject>(OtherActor);
		const auto CloseDragComponent = OtherComp; //Cast<UPrimitiveComponent>(DragObject->FindComponentByClass(UStaticMeshComponent::StaticClass()));
		
		CloseDragObjects.Remove(CloseDragObject);
		CloseDragComponents.Remove(CloseDragComponent);
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
}

void ABEARCharacter::StopInteract()
{
	if(IsDrag)
	{
		GetCharacterMovement()->MaxWalkSpeed = 	MaxWalkSpeed;
		GetCharacterMovement()->bOrientRotationToMovement = true;

		ActiveDragComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

		IsDrag = false;
		ActiveDragObject = nullptr;
		ActiveDragComponent = nullptr;
	}
}

void ABEARCharacter::TryJump()
{
	if(!IsDrag)
	{
		Jump();
	}
}

bool ABEARCharacter::CanDrag()
{
	for(auto PotentialDragObject : CloseDragObjects)
	{
		const auto Direction = (PotentialDragObject->GetActorLocation() - GetActorLocation()).GetUnsafeNormal();
		
		if(FVector::DotProduct(GetActorForwardVector(), Direction) > 0.75f)
		{
			ActiveDragObject = PotentialDragObject;
			ActiveDragComponent = Cast<UPrimitiveComponent>(ActiveDragObject->FindComponentByClass(UStaticMeshComponent::StaticClass()));
			return true;
		}
	}

	return false;
}