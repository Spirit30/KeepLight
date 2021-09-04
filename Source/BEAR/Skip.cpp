// Fill out your copyright notice in the Description page of Project Settings.

#include "Skip.h"

#include "Blueprint/UserWidget.h"

int32 ASkip::Deaths = 0;

ASkip::ASkip()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ASkip::BeginPlay()
{
	Super::BeginPlay();

	if(Deaths >= DeathsToSkip)
	{
		ResetTimeAndDeaths();
		ShowConfirmPopup();
	}
}

void ASkip::ShowConfirmPopup()
{
	const auto ConfirmPopup = CreateWidget(GetWorld(), ConfirmPopupClass);

	const auto Player = GetWorld()->GetFirstPlayerController();
	FInputModeUIOnly Mode;
	Mode.SetLockMouseToViewportBehavior(EMouseLockMode::LockOnCapture);
	Mode.SetWidgetToFocus(ConfirmPopup->TakeWidget());
	Player->SetInputMode(Mode);
	
	ConfirmPopup->AddToViewport();
}

void ASkip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeSec += DeltaTime;

	if(TimeSec >= TimeSecToSkip)
	{
		ResetTimeAndDeaths();
		ShowConfirmPopup();
	}
}

void ASkip::ResetTimeAndDeaths()
{
	Deaths = 0;
	TimeSec = 0.0f;
}

void ASkip::OnDeath()
{
	++Deaths;
}

