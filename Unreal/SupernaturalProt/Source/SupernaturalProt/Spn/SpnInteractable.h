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

	/**
	 * Short verb phrase for the on-screen prompt ("Open door"), empty for no prompt. PLACEHOLDER text, not localized; the real UI maps
	 * feedback/prompt ids later. Called every frame for the object in front of the local player, so it must be cheap.
	 */
	virtual FString SpnPromptText(const AActor* User) const { return FString(); }

	/**
	 * The deliberate, violent alternative to Use (NightBuild04: breaking a locked door, F / DualSense Triangle). Default: not offered.
	 * Call on the authority only.
	 */
	virtual FSpnInteractStatus SpnForce(AActor* User)
	{
		FSpnInteractStatus Status;
		Status.Result = ESpnInteractResult::Refused;
		Status.Feedback = TEXT("placeholder:feedback.verb.unsupported");
		return Status;
	}

	/** Prompt of the Force verb, empty = not offered right now (so no second prompt is shown). */
	virtual FString SpnForcePromptText(const AActor* User) const { return FString(); }

	/** Tie-break when two usable objects score alike in the player's view (higher wins), e.g. a lever over the door it opens. */
	virtual int32 SpnInteractionPriority() const { return 0; }
};
