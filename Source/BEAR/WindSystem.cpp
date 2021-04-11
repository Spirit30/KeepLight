// Fill out your copyright notice in the Description page of Project Settings.

#include "WindSystem.h"

#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "Misc/DefaultValueHelper.h"

AWindSystem::AWindSystem()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWindSystem::BeginPlay()
{
	Super::BeginPlay();

	MaterialParameterCollectionInstance = GetWorld()->GetParameterCollectionInstance(MaterialParameterCollection);
	
	TArray<FString> AudioDescriptorLines;
	AudioDescriptorData.ParseIntoArray(AudioDescriptorLines, TEXT(";"), true);

	for(auto Line : AudioDescriptorLines)
	{
		float OutValue;
		
		if(FDefaultValueHelper::ParseFloat(Line, OutValue))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, FString::Printf(TEXT("Value: %f"), OutValue));
			AudioDescriptor.Add(OutValue);
		}
	}

	Play();
}

void AWindSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float AudioDuration = Audio->GetDuration();
	CurrentPlayTime += DeltaTime;

	if(CurrentPlayTime >= AudioDuration)
	{
		CurrentPlayTime -= AudioDuration;
		Play();
	}

	//GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, FString::Printf(TEXT("%f - %f"), CurrentPlayTime, Audio->GetDuration()));
	
	if(IsApplyWindForce)
	{
		const float CurrentSamplePlayTime = CurrentPlayTime / SmoothWindForceSpeed;
		const float SampleDuration = AudioDuration / SmoothWindForceSpeed;
		
		const int32 CurrentSampleIndex = FMath::Floor(CurrentSamplePlayTime / SampleDuration * AudioDescriptor.Num());
		const float CurrentSampleValue = AudioDescriptor[CurrentSampleIndex];

		//GEngine->AddOnScreenDebugMessage(-1, 0.25f, FColor::Green, FString::Printf(TEXT("VALUE: %f"), CurrentSampleValue));

		const float WindForce = FMath::Lerp(MinWindForce, MaxWindForce, CurrentSampleValue);
		CurrentWindForce = FMath::Lerp(CurrentWindForce, WindForce, DeltaTime * SmoothWindForceSpeed);

		//GEngine->AddOnScreenDebugMessage(-1, 0.25f, FColor::Green, FString::Printf(TEXT("CurrentWindForce: %f"), CurrentWindForce));

		MaterialParameterCollectionInstance->SetScalarParameterValue(WindForceParameterName, CurrentWindForce);
	}
}

void AWindSystem::Play() const
{
	UGameplayStatics::PlaySound2D(GetWorld(), Audio, Volume);
}

