#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spn/SpnInteractable.h"

#include "SpnTestDoor.generated.h"

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

	/** Hinge yaw when open, in degrees (placeholder motion). */
	UPROPERTY(EditAnywhere, Category = "Spn|Test Door|Placeholder Motion", meta = (ClampMin = "-170", ClampMax = "170"))
	float OpenAngle = 90.f;

	/** Swing speed in degrees per second (placeholder motion). */
	UPROPERTY(EditAnywhere, Category = "Spn|Test Door|Placeholder Motion", meta = (ClampMin = "1"))
	float SwingSpeed = 180.f;

	//~ ISpnInteractable
	virtual FSpnInteractStatus SpnUse(AActor* User) override;
	virtual FString SpnPromptText(const AActor* User) const override;

	/** Open according to the world state (or bAuthoredOpen while no fact exists). */
	bool IsOpen() const;

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
	TMap<FString, bool> AuthoredDefaults() const;

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

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UTextRenderComponent> Label;

	float CurrentAngle = 0.f;
	float TargetAngle = 0.f;
	FDelegateHandle WorldStateReplacedHandle;
};
