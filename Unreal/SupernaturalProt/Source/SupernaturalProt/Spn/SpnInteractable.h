#pragma once

#include "CoreMinimal.h"
#include "SpnWorldStateSubsystem.h"
#include "UObject/Interface.h"

#include "SpnInteractable.generated.h"

// Something a player can use (Docs/Proposals/0004, section 4). ENGINEERING PROPOSAL,
// not master canon. D1: only the verb Use exists, so the interface has one function.

UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class USpnInteractable : public UInterface
{
	GENERATED_BODY()
};

class ISpnInteractable
{
	GENERATED_BODY()

public:
	/**
	 * Uses the object. Call on the authority only: the state change goes through
	 * USpnWorldStateSubsystem, which the host owns (co-op direction, no replication yet).
	 */
	virtual FSpnInteractStatus SpnUse(AActor* User) = 0;
};
