// Copyright Epic Games, Inc. All Rights Reserved.


#include "ActionRPGGameModeBase.h"
#include "MainCharacter.h"
#include "MainPlayerController.h"

AActionRPGGameModeBase::AActionRPGGameModeBase()
{
	DefaultPawnClass = AMainCharacter::StaticClass();
	
}