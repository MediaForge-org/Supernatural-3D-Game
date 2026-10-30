#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"

#include "SpnInventoryTypes.generated.h"

// Unreal-side value types of the inventory (M2B3.3). ENGINEERING PROPOSAL, not master canon.
//
// These mirror the portable Core (Core/include/spn/core/Inventory.h) with Unreal types so that gameplay code, tools and tests never
// see Core types (Decision 0006). Ids are the Core's stable ContentId strings ("namespace:path.segments", lower case) for definitions,
// categories, slots, container kinds and presets, and 64-bit numbers for containers and item stacks (0 = none).
// NOTHING here is item data: the test items live in Content/SpnTest/Inventory/TestItemCatalog.json and are marked as test data.

/** Outcome of a catalog change or an inventory transaction. Mirrors spn::core::InventoryErrorCode for commands; appended to, never reordered. */
UENUM(BlueprintType)
enum class ESpnInventoryError : uint8
{
	None,
	/** No item catalog is loaded: the inventory is inactive. */
	NoCatalog,
	/** An argument is malformed (id grammar, zero id, bad JSON field). Nothing was changed. */
	InvalidArgument,
	/** SetCatalog was refused because containers exist; call ResetInventory first. */
	CatalogLocked,
	/** An unexpected exception was caught at the boundary. Nothing was changed. */
	InternalError,
	// --- the Core's command errors ---
	InvalidId,
	UnknownDefinition,
	UnknownContainer,
	DuplicateContainer,
	UnknownItem,
	InvalidQuantity,
	NotStackable,
	StackMismatch,
	SameContainer,
	ContainerNotEmpty,
	CategoryNotAllowed,
	StackLimitReached,
	WeightLimitReached,
	VolumeLimitReached,
	ItemEquipped,
	UnknownSlot,
	NotEquippable,
	AlreadyEquipped,
	SlotOccupied,
	SlotEmpty,
	InvalidState,
	ArithmeticOverflow,
	LimitExceeded,
	// --- catalog ---
	InvalidDefinition,
	DuplicateDefinition,
};

SPNCORE_API const TCHAR* LexToString(ESpnInventoryError Error);

/** Item definition as data (what an item IS). Limits of 0 mean "no contribution". */
struct FSpnItemDefinitionData
{
	FString Id;
	FString Category;
	/** 1 = non-stackable unique instance, > 1 = stackable. */
	int32 MaxStack = 1;
	uint64 WeightGrams = 0;
	uint64 VolumeMilliliters = 0;
	/** Slots this item can occupy; non-empty requires MaxStack == 1. */
	TArray<FString> EquipSlots;
};

/** Capacity rules of a container. A negative limit means "unlimited". */
struct FSpnContainerRulesData
{
	FString Kind;
	int64 MaxStacks = -1;
	int64 MaxWeightGrams = -1;
	int64 MaxVolumeMilliliters = -1;
	/** Empty = every category. */
	TArray<FString> AllowedCategories;
	/** Slots this container offers. */
	TArray<FString> EquipSlots;
};

struct FSpnContainerPresetData
{
	FString Id;
	FSpnContainerRulesData Rules;
};

/** A complete catalog: definitions plus named container presets. */
struct FSpnItemCatalogData
{
	/** True for the temporary test catalog; shown in logs so test items are never mistaken for game content. */
	bool bIsTestData = false;
	FString Description;
	TArray<FSpnItemDefinitionData> Items;
	TArray<FSpnContainerPresetData> ContainerPresets;
};

UENUM(BlueprintType)
enum class ESpnStateValueType : uint8
{
	Bool,
	Int,
	String,
};

/** One named per-instance field of a non-stackable item (durability, charge, notes ...). Field names: [a-z][a-z0-9_]*, at most 16 per item. */
USTRUCT(BlueprintType)
struct FSpnItemStateField
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	FString Name;
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	ESpnStateValueType Type = ESpnStateValueType::Int;
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	bool BoolValue = false;
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	int64 IntValue = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	FString StringValue;

	static FSpnItemStateField MakeBool(const FString& InName, bool bValue) { FSpnItemStateField F; F.Name = InName; F.Type = ESpnStateValueType::Bool; F.BoolValue = bValue; return F; }
	static FSpnItemStateField MakeInt(const FString& InName, int64 Value) { FSpnItemStateField F; F.Name = InName; F.Type = ESpnStateValueType::Int; F.IntValue = Value; return F; }
	static FSpnItemStateField MakeString(const FString& InName, const FString& Value) { FSpnItemStateField F; F.Name = InName; F.Type = ESpnStateValueType::String; F.StringValue = Value; return F; }
};

enum class ESpnInventoryCommandType : uint8
{
	CreateContainer,
	DestroyContainer,
	AddItem,
	RemoveItem,
	TransferItem,
	SplitStack,
	MergeStacks,
	EquipItem,
	UnequipSlot,
};

/** One command of a transaction. Use the factory functions; unused fields stay default. */
struct FSpnInventoryCommand
{
	ESpnInventoryCommandType Type = ESpnInventoryCommandType::AddItem;
	uint64 Container = 0;
	/** TransferItem: target container. */
	uint64 OtherContainer = 0;
	/** AddItem: definition id. */
	FString Definition;
	/** RemoveItem / TransferItem / SplitStack / EquipItem: the stack; MergeStacks: the source. */
	uint64 Item = 0;
	/** MergeStacks: the target stack. */
	uint64 OtherItem = 0;
	/** 0 is only meaningful for MergeStacks ("as much as fits"). */
	uint32 Quantity = 1;
	/** EquipItem / UnequipSlot. */
	FString Slot;
	/** CreateContainer. */
	FSpnContainerRulesData Rules;
	/** AddItem of a non-stackable item. */
	TArray<FSpnItemStateField> State;

	static FSpnInventoryCommand CreateContainer(uint64 Id, const FSpnContainerRulesData& InRules) { FSpnInventoryCommand C; C.Type = ESpnInventoryCommandType::CreateContainer; C.Container = Id; C.Rules = InRules; return C; }
	static FSpnInventoryCommand DestroyContainer(uint64 Id) { FSpnInventoryCommand C; C.Type = ESpnInventoryCommandType::DestroyContainer; C.Container = Id; return C; }
	static FSpnInventoryCommand AddItem(uint64 InContainer, const FString& InDefinition, uint32 InQuantity = 1, const TArray<FSpnItemStateField>& InState = {}) { FSpnInventoryCommand C; C.Type = ESpnInventoryCommandType::AddItem; C.Container = InContainer; C.Definition = InDefinition; C.Quantity = InQuantity; C.State = InState; return C; }
	static FSpnInventoryCommand RemoveItem(uint64 InContainer, uint64 Stack, uint32 InQuantity = 1) { FSpnInventoryCommand C; C.Type = ESpnInventoryCommandType::RemoveItem; C.Container = InContainer; C.Item = Stack; C.Quantity = InQuantity; return C; }
	static FSpnInventoryCommand TransferItem(uint64 From, uint64 To, uint64 Stack, uint32 InQuantity = 1) { FSpnInventoryCommand C; C.Type = ESpnInventoryCommandType::TransferItem; C.Container = From; C.OtherContainer = To; C.Item = Stack; C.Quantity = InQuantity; return C; }
	static FSpnInventoryCommand SplitStack(uint64 InContainer, uint64 Stack, uint32 InQuantity) { FSpnInventoryCommand C; C.Type = ESpnInventoryCommandType::SplitStack; C.Container = InContainer; C.Item = Stack; C.Quantity = InQuantity; return C; }
	static FSpnInventoryCommand MergeStacks(uint64 InContainer, uint64 Source, uint64 Target, uint32 InQuantity = 0) { FSpnInventoryCommand C; C.Type = ESpnInventoryCommandType::MergeStacks; C.Container = InContainer; C.Item = Source; C.OtherItem = Target; C.Quantity = InQuantity; return C; }
	static FSpnInventoryCommand EquipItem(uint64 InContainer, uint64 Stack, const FString& InSlot) { FSpnInventoryCommand C; C.Type = ESpnInventoryCommandType::EquipItem; C.Container = InContainer; C.Item = Stack; C.Slot = InSlot; return C; }
	static FSpnInventoryCommand UnequipSlot(uint64 InContainer, const FString& InSlot) { FSpnInventoryCommand C; C.Type = ESpnInventoryCommandType::UnequipSlot; C.Container = InContainer; C.Slot = InSlot; return C; }
};

/** Result of a catalog change or a transaction. */
struct FSpnInventoryStatus
{
	ESpnInventoryError Error = ESpnInventoryError::None;
	/** Transactions: index of the failing command, -1 if none / not command related. */
	int32 FailedCommandIndex = -1;
	/** Human-readable reason for logs and debug output. Not for program logic. */
	FString Detail;
	/** Inventory revision after the call (unchanged on failure). */
	uint64 Revision = 0;
	/** Stack ids allocated by the transaction, in allocation order. */
	TArray<uint64> CreatedStacks;

	bool IsOk() const { return Error == ESpnInventoryError::None; }
};

/** Read-only copy of one stack. */
struct FSpnStackView
{
	uint64 StackId = 0;
	FString Definition;
	uint32 Quantity = 0;
	TArray<FSpnItemStateField> State;
	/** Slot the stack occupies, empty if it is not equipped. */
	FString EquippedSlot;
};

/** Read-only copy of one container (stacks ordered by id). */
struct FSpnContainerView
{
	uint64 ContainerId = 0;
	FSpnContainerRulesData Rules;
	TArray<FSpnStackView> Stacks;
	uint64 WeightGrams = 0;
	uint64 VolumeMilliliters = 0;
};

/** What a successful transaction (or a replacement) touched; lets listeners refresh only what changed instead of copying the whole inventory. */
struct FSpnInventoryChange
{
	/** Inventory revision after the change. */
	uint64 Revision = 0;
	/** Containers named by the commands of the transaction (source and target of transfers, created and destroyed containers). */
	TArray<uint64> TouchedContainers;
	/** True when the whole inventory was replaced or reset (load, reset, catalog change): refresh everything. */
	bool bReplacedAll = false;

	bool Touches(uint64 ContainerId) const { return bReplacedAll || TouchedContainers.Contains(ContainerId); }
};
