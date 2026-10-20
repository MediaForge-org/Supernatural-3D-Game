#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/EngineTypes.h"

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

	/** The interactable in front of the owner's eyes, or nullptr. */
	AActor* FindTarget() const;

	/** The first actor hit from Start along Direction within InteractRange, if it is interactable. */
	AActor* FindTargetAlong(const FVector& Start, const FVector& Direction) const;

	/** Uses the target in front of the owner. Authority only. Returns true if a target was used. */
	UFUNCTION(BlueprintCallable, Category = "Spn|Interaction")
	bool TryInteract();
};
