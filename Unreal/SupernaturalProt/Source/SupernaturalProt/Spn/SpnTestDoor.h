#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spn/SpnInteractable.h"

#include "SpnTestDoor.generated.h"

class UMaterialInterface;
class UStaticMeshComponent;
class UTextRenderComponent;
class USpnWorldStateSubsystem;

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

	/** Key id ([a-z][a-z0-9_.]*) that unlocks the door with Use while fact placeholder.key.<id>.held is true. Empty = no key opens it. */
	UPROPERTY(EditAnywhere, Category = "Spn|Test Door")
	FString RequiredKeyId;

	/** Hinge yaw when open, in degrees (placeholder motion). */
	UPROPERTY(EditAnywhere, Category = "Spn|Test Door|Placeholder Motion", meta = (ClampMin = "-170", ClampMax = "170"))
	float OpenAngle = 90.f;

	/** Swing speed in degrees per second (placeholder motion). */
	UPROPERTY(EditAnywhere, Category = "Spn|Test Door|Placeholder Motion", meta = (ClampMin = "1"))
	float SwingSpeed = 180.f;

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

	float CurrentAngle = 0.f;
	float TargetAngle = 0.f;
	FDelegateHandle WorldStateReplacedHandle;
};
