// Copyright Epic Games, Inc. All Rights Reserved.

#include "SupernaturalProtGameMode.h"
#include "SpnWorldFactsSmokeTest.h"

ASupernaturalProtGameMode::ASupernaturalProtGameMode()
{
	// stub
}

void ASupernaturalProtGameMode::BeginPlay()
{
	Super::BeginPlay();

	// PLACEHOLDER: proves the SPN core runs inside Unreal; result goes to LogSpnCore.
	SpnCore::RunWorldFactsSmokeTest();
}
