#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spn/SpnInteractable.h"

#include "SpnTestDoor.generated.h"

class UMaterialInterface;
class UStaticMeshComponent;
class UTextRenderComponent;
class USpnWorldStateSubsystem;
class APawn;

/**
 * PLACEHOLDER test door (Docs/Proposals/0004): proves Use, WorldFacts persistence and
 * save/load in a level. Not a design for doors in the game: cube mesh, no keys, sounds or UI.
 *
 * The door keeps no state of its own. Open/closed lives only in USpnWorldStateSubsystem
 * (fact placeholder.door.<StableId>.open, absent = bAuthoredOpen); the hinge angle is just
 * its presentation, re-read after every use and every load.
 */
UCLASS(meta = (PrioritizeCategories = "Spn"))
class ASpnTestDoor : public AActor, public ISpnInteractable
{
	GENERATED_BODY()

public:
	ASpnTestDoor();

	/** Stable object id (D2: 1..96 chars, [a-z][a-z0-9_.]*). Must be unique per level; it keys the saved state. */
	UPROPERTY(EditAnywhere, Category = "Spn|Test Door")
	FString StableId = TEXT("test_door");

	/** Authored state, used while the player has never changed this door. */
	UPROPERTY(EditAnywhere, Category = "Spn|Test Door")
	bool bAuthoredOpen = false;

	/** Authored lock; a locked door refuses Use (no key model yet). */
	UPROPERTY(EditAnywhere, Category = "Spn|Test Door")
	bool bAuthoredLocked = false;

	/** Authored block (barricaded / jammed): both verbs are refused while the `.disabled` fact says so. Mechanisms clear the fact later. */
	UPROPERTY(EditAnywhere, Category = "Spn|Test Door")
	bool bAuthoredBlocked = false;

	/** A locked, closed door of this kind can be broken open with Force (F / Triangle). Breaking is permanent and saved. */
	UPROPERTY(EditAnywhere, Category = "Spn|Test Door")
	bool bBreakable = false;

	/** How long F must be held to break a locked, breakable door (seconds, placeholder). 0 = at once. */
	UPROPERTY(EditAnywhere, Category = "Spn|Test Door", meta = (ClampMin = "0"))
	float BreakSeconds = 1.6f;

	/** Audible radius in cm of a door being broken / used (placeholder values). */
	UPROPERTY(EditAnywhere, Category = "Spn|Test Door|Noise", meta = (ClampMin = "0"))
	float DoorBreakNoiseRangeCm = 2000.f;

	UPROPERTY(EditAnywhere, Category = "Spn|Test Door|Noise", meta = (ClampMin = "0"))
	float DoorUseNoiseRangeCm = 600.f;

	/** Key id ([a-z][a-z0-9_.]*) that unlocks the door with Use while fact placeholder.key.<id>.held is true. Empty = no key opens it. */
	UPROPERTY(EditAnywhere, Category = "Spn|Test Door")
	FString RequiredKeyId;

	/** Hinge yaw when open, in degrees (placeholder motion). */
	UPROPERTY(EditAnywhere, Category = "Spn|Test Door|Placeholder Motion", meta = (ClampMin = "-170", ClampMax = "170"))
	float OpenAngle = 90.f;

	/** Swing speed in degrees per second (placeholder motion). */
	UPROPERTY(EditAnywhere, Category = "Spn|Test Door|Placeholder Motion", meta = (ClampMin = "1"))
	float SwingSpeed = 180.f;

	/** The character that opened the door and stands in its swing steps back to this many cm beyond the leaf's reach (PLACEHOLDER). */
	UPROPERTY(EditAnywhere, Category = "Spn|Test Door|Placeholder Motion", meta = (ClampMin = "0"))
	float StepAsideMargin = 12.f;

	/** Longest automatic step out of the swing in cm; a longer one is not taken and the door stays blocked (PLACEHOLDER). */
	UPROPERTY(EditAnywhere, Category = "Spn|Test Door|Placeholder Motion", meta = (ClampMin = "0"))
	float StepAsideMaxLength = 90.f;

	/**
	 * Optional look (M2A3 V3 level): leaf material, handle and hinge details and the leaf size. The defaults reproduce the original placeholder door
	 * (100 x 10 x 200 cm cube, no handle). The leaf is never wider than the clear doorway of the building kit (110 cm); a pawn can pass the opening
	 * whatever the style. Collision is the leaf box only; handles and hinges have none.
	 */
	UPROPERTY(EditAnywhere, Category = "Spn|Test Door|Placeholder Look")
	TObjectPtr<UMaterialInterface> LeafMaterial = nullptr;

	/** Material of the handle bars and hinge barrels (metal/brass look). Empty = engine default. */
	UPROPERTY(EditAnywhere, Category = "Spn|Test Door|Placeholder Look")
	TObjectPtr<UMaterialInterface> HardwareMaterial = nullptr;

	UPROPERTY(EditAnywhere, Category = "Spn|Test Door|Placeholder Look")
	bool bShowHandle = false;

	/** Three hinge barrels on the hinge edge of the leaf. */
	UPROPERTY(EditAnywhere, Category = "Spn|Test Door|Placeholder Look")
	bool bShowHinges = false;

	/** Leaf width in cm (hinge edge to free edge) and thickness; the hinge stays at the actor origin. */
	UPROPERTY(EditAnywhere, Category = "Spn|Test Door|Placeholder Look", meta = (ClampMin = "60", ClampMax = "108"))
	float LeafWidth = 100.f;

	UPROPERTY(EditAnywhere, Category = "Spn|Test Door|Placeholder Look", meta = (ClampMin = "3", ClampMax = "10"))
	float LeafThickness = 10.f;

	//~ ISpnInteractable
	virtual FSpnInteractStatus SpnUse(AActor* User) override;
	virtual FString SpnPromptText(const AActor* User) const override;

	virtual FSpnInteractStatus SpnForce(AActor* User) override;
	virtual FString SpnForcePromptText(const AActor* User) const override;
	virtual float SpnForceDuration() const override;
	virtual void SpnForceStrain(float Progress01) override;

	/** Broken open for good (fact `.broken`). A broken door has no working lock but still swings. */
	bool IsBroken() const;

	/** Blocked: the `.disabled` fact or the authored bAuthoredBlocked. */
	bool IsBlocked() const;

	/** True if the key this door needs is held. */
	bool HasRequiredKey() const;

	/** Open according to the world state (or bAuthoredOpen while no fact exists). */
	bool IsOpen() const;

	/** Lock state as the game sees it: the world-state fact if one exists, else the authored default (bAuthoredLocked). */
	bool IsLocked() const;

	/**
	 * True if a pawn is inside the leaf's current volume or anywhere on its way to the closed position (sampled every 10 degrees,
	 * object-type query ECC_Pawn). Closing is refused while this is true; opening is never refused by it.
	 * UNVERIFIED: written without an Unreal build; the overlap result must be checked on Windows (door closing onto a standing player).
	 */
	bool IsClosingObstructed() const;

	/**
	 * True if a pawn is inside the leaf volume anywhere between the two hinge angles (sampled every 4 degrees). The swing waits while this is
	 * true. IgnoredPawn (optional) is left out of the test: it tells whether the opener is the only pawn in the way. Any-hit queries only,
	 * no result arrays: this runs every frame of a swing.
	 */
	bool IsSweepBlockedByPawn(float FromAngle, float ToAngle, const APawn* IgnoredPawn = nullptr) const;

	/** Distance in cm from the hinge axis to the farthest point of the leaf (constant; the whole swing stays within it). */
	float GetLeafReach() const;

	/** World position of the hinge axis at the leaf's height. */
	FVector GetHingeLocation() const;

	/** The pawn that opened the door with its last Use (null when nobody did, e.g. a script). */
	APawn* GetOpener() const { return Opener.Get(); }

	/** How the last request to let the opener step out of the swing ended: none, stepping, no room. For tests and diagnostics. */
	enum class EStepAside : uint8 { None, Requested, NoRoom };
	EStepAside GetStepAsideResult() const { return StepAsideResult; }
	/** Where the last requested step went (world). */
	FVector GetLastStepAsideTarget() const { return LastStepAsideTarget; }

	/** True while the opening or closing leaf is held back by a pawn. */
	bool IsWaitingForPawn() const { return bWaitingForPawn; }

	/** Hinge yaw shown right now, and the yaw it is swinging to. */
	float GetCurrentAngle() const { return CurrentAngle; }
	float GetTargetAngle() const { return TargetAngle; }

	/** Advances the swing; enabled only while the door moves. */
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	USpnWorldStateSubsystem* GetWorldState() const;
	FSpnInteractDefaults AuthoredDefaults() const;
	FSpnInteractStatus RunVerb(ESpnInteractionVerb Verb, AActor* User);
	void ReportOutcome(const FSpnInteractStatus& Status) const;
	void EmitDoorNoise(ESpnInteractionVerb Verb, const FSpnInteractStatus& Status, const AActor* User) const;

	/** Points the hinge at the world state: swinging there, or jumping there (after a load). */
	void RefreshFromWorldState(bool bAnimate);
	void HandleWorldStateReplaced();
	void SetCurrentAngle(float Angle);
	void WarnAboutDuplicateIds() const;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USceneComponent> Hinge;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Panel;

	/** Handle bars on the two faces of the leaf, visible only with bShowHandle; no collision. */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> HandleFront;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> HandleBack;

	/** Hinge barrels (cylinders) on the hinge edge, visible only with bShowHinges; no collision. */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TArray<TObjectPtr<UStaticMeshComponent>> HingeBarrels;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UTextRenderComponent> Label;

	/** The opener stands in the opening swing: asks it to take a short step out of the swing if there is room (once per swing). */
	void LetOpenerStepAside(APawn& Pawn);

	float CurrentAngle = 0.f;
	float TargetAngle = 0.f;
	float StrainJitter = 0.f;
	bool bWaitingForPawn = false;
	bool bStepAsideAsked = false;
	EStepAside StepAsideResult = EStepAside::None;
	FVector LastStepAsideTarget = FVector::ZeroVector;
	TWeakObjectPtr<APawn> Opener;
	FDelegateHandle WorldStateReplacedHandle;
};
