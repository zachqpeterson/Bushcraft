// Copyright Epic Games, Inc. All Rights Reserved.

#include "BushcraftGameMode.h"
#include "BushcraftCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABushcraftGameMode::ABushcraftGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
