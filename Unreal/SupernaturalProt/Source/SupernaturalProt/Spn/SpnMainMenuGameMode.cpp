#include "Spn/SpnMainMenuGameMode.h"

#include "Camera/CameraActor.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpectatorPawn.h"

ASpnMainMenuGameMode::ASpnMainMenuGameMode()
{
	DefaultPawnClass = nullptr;
	PlayerControllerClass = APlayerController::StaticClass();
	HUDClass = nullptr;
}

void ASpnMainMenuGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	ACameraActor* Chosen = nullptr;
	for (TActorIterator<ACameraActor> It(GetWorld()); It; ++It)
	{
		if (Chosen == nullptr || It->ActorHasTag(TEXT("SpnMenuCamera")))
		{
			Chosen = *It;
		}
	}
	if (Chosen != nullptr && NewPlayer != nullptr)
	{
		NewPlayer->SetViewTarget(Chosen);
	}
}
