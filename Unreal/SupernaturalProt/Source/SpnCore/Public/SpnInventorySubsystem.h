#pragma once

#include "CoreMinimal.h"
#include "SpnInventoryTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Templates/PimplPtr.h"

#include "SpnInventorySubsystem.generated.h"

class USpnSaveSubsystem;

// Unreal integration of the portable inventory (Core/include/spn/core/Inventory.h, M2B1/M2B2) -> M2B3.3. ENGINEERING PROPOSAL, not master canon.
//
// The Core is the only implementation of inventory rules. This subsystem owns one ItemCatalog and one InventoryState per game instance,
// translates Unreal-typed commands to Core commands, and reports the Core's errors as ESpnInventoryError. Core types stay in private
// headers (Decision 0006). Game thread only; no balance values anywhere: limits come from the catalog data.
//
// TRANSACTION GUARANTEE: Apply() is all-or-nothing (Core ApplyAll): if any command fails, the containers, stacks, id counter and revision
// are exactly as before, FailedCommandIndex names the command, and no listener is called. Arguments that cannot even be translated
// (bad ids) are rejected before the Core is touched.
//
// INACTIVE UNTIL A CATALOG IS LOADED: without a catalog every mutation returns NoCatalog and the save subsystem treats the inventory as not
// integrated (it neither writes nor restores player.inventory).

/**
 * Owns the item catalog and the inventory state of one game instance.
 * The test catalog (Content/SpnTest/Inventory/TestItemCatalog.json) is explicitly temporary test data.
 */
UCLASS()
class SPNCORE_API USpnInventorySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	USpnInventorySubsystem();
	virtual ~USpnInventorySubsystem() override;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// --- catalog ------------------------------------------------------------------------------------------------------------------

	/**
	 * Validates the catalog with the Core (definitions, container presets) and installs it with an EMPTY inventory.
	 * Refused with CatalogLocked while containers exist (ResetInventory first). On any error the previous catalog stays.
	 */
	FSpnInventoryStatus SetCatalog(const FSpnItemCatalogData& Catalog);

	/** Parses catalog JSON (see Content/SpnTest/Inventory/TestItemCatalog.json for the schema) and calls SetCatalog. */
	FSpnInventoryStatus SetCatalogFromJson(const FString& JsonText);
	FSpnInventoryStatus LoadCatalogFromFile(const FString& AbsolutePath);

	/** Absolute path of the temporary test catalog. */
	static FString GetTestCatalogPath();

	/** True if a catalog is installed (the inventory is active). */
	bool HasCatalog() const;
	const FSpnItemCatalogData& GetCatalogData() const;
	bool FindDefinition(const FString& DefinitionId, FSpnItemDefinitionData& OutDefinition) const;
	bool FindContainerPreset(const FString& PresetId, FSpnContainerRulesData& OutRules) const;

	/** Empties the inventory (no containers, revision 0, ids restart at 1) and keeps the catalog. */
	void ResetInventory();
	/** Removes catalog and inventory. */
	void ClearCatalog();

	// --- transactions --------------------------------------------------------------------------------------------------------------

	/** One transaction: all commands succeed in order, or nothing changes. An empty list is a successful no-op. */
	FSpnInventoryStatus Apply(const TArray<FSpnInventoryCommand>& Commands);

	// single-command conveniences (each is one transaction)
	FSpnInventoryStatus CreateContainer(uint64 ContainerId, const FSpnContainerRulesData& Rules);
	FSpnInventoryStatus CreateContainerFromPreset(uint64 ContainerId, const FString& PresetId);
	FSpnInventoryStatus DestroyContainer(uint64 ContainerId);
	FSpnInventoryStatus AddItem(uint64 ContainerId, const FString& DefinitionId, uint32 Quantity = 1, const TArray<FSpnItemStateField>& State = {});
	FSpnInventoryStatus RemoveItem(uint64 ContainerId, uint64 StackId, uint32 Quantity = 1);
	FSpnInventoryStatus TransferItem(uint64 FromContainer, uint64 ToContainer, uint64 StackId, uint32 Quantity = 1);
	FSpnInventoryStatus SplitStack(uint64 ContainerId, uint64 StackId, uint32 Quantity);
	FSpnInventoryStatus MergeStacks(uint64 ContainerId, uint64 SourceStack, uint64 TargetStack, uint32 Quantity = 0);
	FSpnInventoryStatus Equip(uint64 ContainerId, uint64 StackId, const FString& Slot);
	FSpnInventoryStatus Unequip(uint64 ContainerId, const FString& Slot);

	/** The most recent failed transaction or catalog change (for inspection / debug commands); Error == None if there was none. */
	const FSpnInventoryStatus& GetLastFailure() const;

	// --- read-only queries ---------------------------------------------------------------------------------------------------------

	uint64 GetRevision() const;
	TArray<uint64> ListContainers() const;
	bool GetContainer(uint64 ContainerId, FSpnContainerView& OutView) const;
	/** Total quantity of a definition in a container (0 for an unknown container or definition). */
	uint64 CountOf(uint64 ContainerId, const FString& DefinitionId) const;
	/** Container holding the stack, 0 if none. */
	uint64 LocateStack(uint64 StackId) const;
	/** The next stack id that will be allocated (ids are never reused). */
	uint64 GetNextStackId() const;
	/** The Core's own invariant check; empty string = consistent. For tests and diagnostics. */
	FString CheckInvariants() const;
	/** One line per container and stack, for logs. */
	FString Describe() const;

	// --- notifications ---------------------------------------------------------------------------------------------------------------

	/** After every successful, non-empty transaction. */
	FSimpleMulticastDelegate OnInventoryChanged;
	/** After the save subsystem replaced the whole inventory by a load (all other loaded state is in place by then). */
	FSimpleMulticastDelegate OnInventoryReplaced;

private:
	/** USpnSaveSubsystem captures and replaces the Core state directly (Private/SpnInventoryInternal.h). */
	friend class USpnSaveSubsystem;

	struct FState;
	TPimplPtr<FState> State;
};
