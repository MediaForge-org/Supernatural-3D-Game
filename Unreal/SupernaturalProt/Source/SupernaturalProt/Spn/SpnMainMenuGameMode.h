#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "SpnMainMenuGameMode.generated.h"

/**
 * Game mode of the main-menu level (Night Build 01): no pawn, no HUD. The player controller looks through the camera actor tagged SpnMenuCamera
 * (or the first camera actor of the level); the menu itself is shown by USpnUiSubsystem when the level has loaded.
 */
UCLASS()
class SUPERNATURALPROT_API ASpnMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASpnMainMenuGameMode();
	virtual void RestartPlayer(AController* NewPlayer) override {}
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
};
