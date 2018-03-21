// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ema_vr_rehabGameMode.h"
#include "ema_vr_rehabCharacter.h"
#include "UObject/ConstructorHelpers.h"

Aema_vr_rehabGameMode::Aema_vr_rehabGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
