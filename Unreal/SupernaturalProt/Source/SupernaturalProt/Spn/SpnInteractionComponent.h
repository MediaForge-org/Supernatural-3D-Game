#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/EngineTypes.h"
#include "InputCoreTypes.h"
#include "Widgets/SWidget.h"

#include "SpnInteractionComponent.generated.h"

/**
 * Finds the ISpnInteractable the owner is looking at and uses it. Reusable on any
 * actor; on a pawn the trace starts at the eyes and follows the view (control) rotation.
 * PLACEHOLDER targeting (short sphere trace, no prompt UI); range values are not tuned.
 */
UCLASS(ClassGroup = (Spn), meta = (BlueprintSpawnableComponent))
class USpnInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USpnInteractionComponent();

	/** How far from the eyes an object can be used, in cm (placeholder). */
	UPROPERTY(EditAnywhere, Category = "Spn|Interaction", meta = (ClampMin = "0"))
	float InteractRange = 250.f;

	/** Radius of the sphere trace, in cm; makes aiming forgiving (placeholder). */
	UPROPERTY(EditAnywhere, Category = "Spn|Interaction", meta = (ClampMin = "0"))
	float TraceRadius = 15.f;

	UPROPERTY(EditAnywhere, Category = "Spn|Interaction")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;

	/** Shows the prompt ("E – Tür öffnen") in a small Slate widget in the lower third of the view (PLACEHOLDER look, not the final HUD) while a local player looks at something usable. */
	UPROPERTY(EditAnywhere, Category = "Spn|Interaction")
	bool bShowPrompt = true;

	/** Key named in the prompt; the character copies its interact key here. */
	UPROPERTY(EditAnywhere, Category = "Spn|Interaction")
	FKey PromptKey = EKeys::E;

	/** Widest angle in degrees between the view direction and the direction to an object's centre (widened by its size) that still counts as "looking at it" (placeholder). */
	UPROPERTY(EditAnywhere, Category = "Spn|Interaction", meta = (ClampMin = "1", ClampMax = "90"))
	float MaxViewAngle = 35.f;

	/** Largest height difference between the eyes and an object's centre, in cm (placeholder). */
	UPROPERTY(EditAnywhere, Category = "Spn|Interaction", meta = (ClampMin = "1"))
	float MaxHeightDifference = 150.f;

	/** Objects count as at most this big (cm radius) when judging how easy they are to aim at; keeps a huge door from being "hit" from everywhere. */
	UPROPERTY(EditAnywhere, Category = "Spn|Interaction", meta = (ClampMin = "0"))
	float MaxTargetRadius = 60.f;

	/** Key named in the second prompt (Force, e.g. breaking a door); the character copies its force key here. */
	UPROPERTY(EditAnywhere, Category = "Spn|Interaction")
	FKey ForceKey = EKeys::F;

	/** The prompt text shown right now ("[E] Open door"), empty when nothing usable is in range. Read by the QA tests. */
	UFUNCTION(BlueprintPure, Category = "Spn|Interaction")
	const FString& GetCurrentPrompt() const { return CurrentPrompt; }

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** The interactable in front of the owner's eyes, or nullptr. */
	AActor* FindTarget() const;

	/** The interactable the player means when looking from Start along Direction: in reach, in view, line of sight clear (no wall), then the most central and closest (spn::core::PickInteractionCandidate). */
	AActor* FindTargetAlong(const FVector& Start, const FVector& Direction) const;

	/** Force key pressed: starts the timed action on the target (hold for SpnForceDuration seconds), or forces at once when the target needs no time. Authority only. */
	UFUNCTION(BlueprintCallable, Category = "Spn|Interaction")
	bool BeginForce();

	/** Force key released: cancels a running timed action. */
	UFUNCTION(BlueprintCallable, Category = "Spn|Interaction")
	void EndForce();

	UFUNCTION(BlueprintPure, Category = "Spn|Interaction")
	bool IsForcing() const { return bForceHeld; }

	/** The target in front offers the Force verb right now (a locked, breakable door). The gamepad hold (Square) only forces then. */
	bool HasForceTarget() const;

	/** Id of the building-facade door the last unanswered Interact / Force pointed at, and when (world seconds); empty if none yet. QA. */
	const FString& GetLastDecorDoorId() const { return LastDecorDoorId; }
	double GetLastDecorDoorTime() const { return LastDecorDoorTime; }

	/** 0..1 progress of the running timed action. */
	UFUNCTION(BlueprintPure, Category = "Spn|Interaction")
	float GetForceProgress() const { return ForceProgress; }

	/** Uses the target in front of the owner. Authority only. Returns true if a target was used. */
	UFUNCTION(BlueprintCallable, Category = "Spn|Interaction")
	bool TryInteract();

	/** The Force verb (break a door open, F / DualSense Triangle) on the target in front of the owner. Authority only. Returns true if a target was addressed. */
	UFUNCTION(BlueprintCallable, Category = "Spn|Interaction")
	bool TryForce();

private:
	bool TryVerb(bool bForce);
	/** Nothing usable in front: if the player tried a door that is only part of a building facade, say so (no permanent marker on those doors). */
	void AnswerDecorDoor(bool bForce);
	FString LastDecorDoorId;
	double LastDecorDoorTime = -1.0;
	void TickForce(float DeltaTime);
	void AbortForce();

	TWeakObjectPtr<AActor> ForceTarget;
	float ForceElapsed = 0.f;
	float ForceDuration = 0.f;
	float ForceProgress = 0.f;
	bool bForceHeld = false;
	void ShowPromptWidget(const FString& Text);
	void HidePromptWidget();

	FString CurrentPrompt;
	TSharedPtr<SWidget> PromptWidget;
	TSharedPtr<class STextBlock> PromptTextBlock;
};
