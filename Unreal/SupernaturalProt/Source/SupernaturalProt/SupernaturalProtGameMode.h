// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SupernaturalProtGameMode.generated.h"

/**
 *  Simple GameMode for a third person game
 */
UCLASS(abstract)
class ASupernaturalProtGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	/** Constructor */
	ASupernaturalProtGameMode();

protected:

	/** Runs the placeholder SPN core integration check (see SpnWorldFactsSmokeTest.h) */
	virtual void BeginPlay() override;
};



