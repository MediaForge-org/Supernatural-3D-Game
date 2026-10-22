#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spn/SpnInteractable.h"

#include "SpnTestPickup.generated.h"

class UStaticMeshComponent;
class UTextRenderComponent;
class USpnWorldStateSubsystem;

/**
 * PLACEHOLDER test pickup (Docs/Proposals/0007): proves "take an object once, a per-holder
 * count grows" on top of the interaction system and WorldFacts. Not a design for items,
 * inventory, capacity, weight or stacking: cube mesh, no sounds, UI or keys.
 *
 * The pickup keeps no state of its own. Taken or not lives only in USpnWorldStateSubsystem
 * (fact placeholder.pickup.<StableId>.taken, absent = not taken); visibility and collision
 * are just its presentation, re-read after every use and every load.
 */
UCLASS(meta = (PrioritizeCategories = "Spn"))
class ASpnTestPickup : public AActor, public ISpnInteractable
{
	GENERATED_BODY()

public:
	ASpnTestPickup();

	/** Stable object id (D2: 1..96 chars, [a-z][a-z0-9_.]*). Must be unique per level; it keys the saved state. */
	UPROPERTY(EditAnywhere, Category = "Spn|Test Pickup")
	FString StableId = TEXT("test_pickup");

	/** Item id (prototype rule: 1..48 chars, [a-z][a-z0-9_]*, no dots). */
	UPROPERTY(EditAnywhere, Category = "Spn|Test Pickup")
	FString ItemId = TEXT("test_item");

	/** Amount added to the holder's count when taken (at least 1). */
	UPROPERTY(EditAnywhere, Category = "Spn|Test Pickup", meta = (ClampMin = "1"))
	int32 Amount = 1;

	//~ ISpnInteractable
	virtual FSpnInteractStatus SpnUse(AActor* User) override;

	/** Taken according to the world state (not taken while no fact exists). */
	bool IsTaken() const;

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	USpnWorldStateSubsystem* GetWorldState() const;
	FSpnInteractDefaults AuthoredDefaults() const;

	/** Shows or hides the object according to the world state. */
	void RefreshFromWorldState();
	void HandleWorldStateReplaced();
	void WarnAboutDuplicateIds() const;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Body;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UTextRenderComponent> Label;

	FDelegateHandle WorldStateReplacedHandle;
};
