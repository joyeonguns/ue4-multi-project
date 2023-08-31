// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiFPSGameMode.h"
#include "MultiFPSCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMultiFPSGameMode::AMultiFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
