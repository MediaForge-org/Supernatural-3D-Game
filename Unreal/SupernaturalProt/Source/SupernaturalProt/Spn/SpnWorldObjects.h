#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spn/SpnInteractable.h"
#include "SpnInventoryTypes.h"

#include "SpnWorldObjects.generated.h"

class UStaticMeshComponent;
class UTextRenderComponent;
class USpnInventorySubsystem;
class USpnWorldStateSubsystem;

// Real world interactables of Night Build 01 (work packages J, K, L). ENGINEERING PROPOSAL, not master canon. Mesh and text are PLACEHOLDERS.
//
// Unlike ASpnTestPickup (a WorldFacts counter), these objects move real items through USpnInventorySubsystem, i.e. through the Core inventory
// transactions: capacity, weight, volume and category rules are the Core's. No WorldFacts counter ever stands in for an item.
//
// PERSISTENCE (what is and is not saved): the CONTENT of the player's inventory and of every container is saved by "player.inventory". The
// "taken" flag of a pickup is a bool fact in "world.facts". There is NO actor/world-object persistence yet (Core ObjectPersistence is not
// integrated in Unreal): a container actor exists because its level places it; only its contents persist, never its transform or lid state.

namespace SpnWorld
{
	/** The player's own container in the single-player test setup (created by the session on a new game). */
	inline constexpr uint64 PlayerContainerId = 1;

	/** Stable container id of a world object: a fixed offset plus a 32-bit hash of its StableId (same id on every run, never reused by the player). */
	SUPERNATURALPROT_API uint64 ContainerIdFor(const FString& StableId);

	/** Display name of an item definition (localized test names, else the id). */
	SUPERNATURALPROT_API FText ItemDisplayName(const FString& DefinitionId);

	/** Why an inventory transaction failed, as a short localized sentence for the HUD. */
	SUPERNATURALPROT_API FText ErrorSentence(ESpnInventoryError Error);

	/** The player's own container: PlayerContainerId if it exists, else the lowest container id that is not a world container. 0 if none. */
	SUPERNATURALPROT_API uint64 FindPlayerContainer(const USpnInventorySubsystem& Inventory);
}

/** A pickup that adds a real item to the player's inventory. All-or-nothing: if the stack does not fit, nothing moves and the object stays. */
UCLASS(meta = (PrioritizeCategories = "Spn"))
class SUPERNATURALPROT_API ASpnWorldPickup : public AActor, public ISpnInteractable
{
	GENERATED_BODY()

public:
	ASpnWorldPickup();

	/** D2 stable id of the object; keys the saved "taken" flag. Unique per level. */
	UPROPERTY(EditAnywhere, Category = "Spn|Pickup")
	FString StableId = TEXT("pickup");

	/** Item definition id from the item catalog (for example spn_test:item.ration). Validated by the Core when taken. */
	UPROPERTY(EditAnywhere, Category = "Spn|Pickup")
	FString DefinitionId = TEXT("spn_test:item.ration");

	UPROPERTY(EditAnywhere, Category = "Spn|Pickup", meta = (ClampMin = "1"))
	int32 Quantity = 1;

	virtual FSpnInteractStatus SpnUse(AActor* User) override;
	virtual FString SpnPromptText(const AActor* User) const override;

	bool IsTaken() const;
	/** Key of the saved flag (world.facts). */
	FString TakenKey() const;

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;

private:
	void Refresh();
	USpnWorldStateSubsystem* GetWorldState() const;
	USpnInventorySubsystem* GetInventory() const;

	UPROPERTY(VisibleAnywhere, Category = "Components") TObjectPtr<USceneComponent> Root;
	UPROPERTY(VisibleAnywhere, Category = "Components") TObjectPtr<UStaticMeshComponent> Body;
	UPROPERTY(VisibleAnywhere, Category = "Components") TObjectPtr<UTextRenderComponent> Label;
	FDelegateHandle ReplacedHandle;
	FDelegateHandle InventoryHandle;
};

/** An item a container starts with. */
USTRUCT()
struct FSpnContainerSeed
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Spn|Container") FString DefinitionId;
	UPROPERTY(EditAnywhere, Category = "Spn|Container", meta = (ClampMin = "1")) int32 Quantity = 1;
};

/**
 * A world container backed by a real inventory container (stable id from StableId). Using it opens the inventory screen as a container view:
 * items are moved with the normal inventory actions ("Move to ..."), which are Core transfers (capacity-checked, atomic, no duplication).
 */
UCLASS(meta = (PrioritizeCategories = "Spn"))
class SUPERNATURALPROT_API ASpnWorldContainer : public AActor, public ISpnInteractable
{
	GENERATED_BODY()

public:
	ASpnWorldContainer();

	UPROPERTY(EditAnywhere, Category = "Spn|Container")
	FString StableId = TEXT("container");

	/** Container preset of the catalog (capacity rules), for example spn_test:preset.small_box. */
	UPROPERTY(EditAnywhere, Category = "Spn|Container")
	FString PresetId = TEXT("spn_test:preset.small_box");

	UPROPERTY(EditAnywhere, Category = "Spn|Container")
	FString Title = TEXT("Box");

	/** Contents of a container that does not exist yet (a new game). An existing container (restored from a save) is never re-seeded. */
	UPROPERTY(EditAnywhere, Category = "Spn|Container")
	TArray<FSpnContainerSeed> InitialItems;

	virtual FSpnInteractStatus SpnUse(AActor* User) override;
	virtual FString SpnPromptText(const AActor* User) const override;

	uint64 GetContainerId() const;
	/** Creates the inventory container with its initial items if it does not exist and a catalog is installed. True if the container exists afterwards. */
	bool EnsureContainer();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;

private:
	USpnInventorySubsystem* GetInventory() const;

	UPROPERTY(VisibleAnywhere, Category = "Components") TObjectPtr<USceneComponent> Root;
	UPROPERTY(VisibleAnywhere, Category = "Components") TObjectPtr<UStaticMeshComponent> Body;
	UPROPERTY(VisibleAnywhere, Category = "Components") TObjectPtr<UTextRenderComponent> Label;
	FDelegateHandle ReplacedHandle;
};
