// Copyright Epic Games, Inc. All Rights Reserved.

#include "BEARGameMode.h"
#include "BEARCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABEARGameMode::ABEARGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SideScrollerCPP/Blueprints/BEAR/BEAR_BP"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
