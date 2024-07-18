// Copyright Epic Games, Inc. All Rights Reserved.

#include "PuzzlePlatformGameMode.h"
#include "PuzzlePlatformCharacter.h"
#include "UObject/ConstructorHelpers.h"

APuzzlePlatformGameMode::APuzzlePlatformGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
