#pragma once

#include "CoreMinimal.h"
#include "SpnInventorySubsystem.h"
#include "SpnSaveSubsystem.h"
#include "UObject/Object.h"

#include "SpnInventoryViewModel.generated.h"

// Presentation model of the inventory for UMG (M2B3.4). ENGINEERING PROPOSAL, not master canon. No widget exists yet.
//
// WHAT IT IS: a read-mostly projection of the REAL inventory (USpnInventorySubsystem -> Core InventoryState) for the containers a screen is
// bound to, enriched with display data from the optional presentation set. It owns no gameplay rules: which actions are possible is decided
// by dry-running the real command in the Core (USpnInventorySubsystem::ProbeCommands); executing an action is a normal Apply(). There are no
// fake items: an empty inventory yields empty panels.
//
// WHAT IT GIVES A WIDGET: per bound container a panel with sorted/filtered item rows (names, quantity, unit and total weight/volume, state,
// icon soft reference), the capacity bars, the equipment slots; a selection with the list of available actions (each with the reason when it
// is unavailable); navigation that works the same for keyboard, mouse and controller (a widget only translates its input into
// Navigate / SelectStack / ExecuteAction calls; this class knows no input device and binds no keys); localisable message keys for errors and
// save/load notices; and change notifications.
//
// UPDATE MECHANISM: it listens to the subsystem's OnInventoryChangedDetailed (which names the containers a transaction touched) and
// OnInventoryReplaced (load, reset, catalog change), and rebuilds ONLY the bound panels that were touched; unbound containers cost nothing
// per change. Selection survives a refresh: a selected stack that moved to another bound container is followed, a removed one moves the
// selection to the neighbouring row. Version() increases with every refresh so widgets can poll cheaply instead of binding.
//
// Ids exposed to Blueprints are int64 (Blueprints have no uint64); real ids are positive and far below 2^63.

UENUM(BlueprintType)
enum class ESpnInventoryAction : uint8
{
	None,
	Equip,
	Unequip,
	Transfer,
	Split,
	Merge,
	Discard,
};

UENUM(BlueprintType)
enum class ESpnNavigationDirection : uint8
{
	Up,
	Down,
	Left,
	Right,
	Next,
	Previous,
	PageUp,
	PageDown,
	First,
	Last,
	NextSection,
	PreviousSection,
};

UENUM(BlueprintType)
enum class ESpnNavigationZone : uint8
{
	/** The item rows of the bound containers. */
	Items,
	/** The equipment slots of the primary container. */
	Equipment,
};

UENUM(BlueprintType)
enum class ESpnInventorySort : uint8
{
	/** The Core's order (ascending stack id): stable and reproducible. */
	Default,
	Name,
	Category,
	TotalWeight,
	TotalVolume,
	Quantity,
};

UENUM(BlueprintType)
enum class ESpnMessageSeverity : uint8
{
	Info,
	Warning,
	Error,
};

/** One stack as a row. All numbers come from the Core state and the catalog; nothing is derived by guessing. */
USTRUCT(BlueprintType)
struct SPNCORE_API FSpnItemRow
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	int64 StackId = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	int64 ContainerId = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	FString DefinitionId;
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	FString Category;
	/** From the presentation set; without an entry the definition id as text (bHasPresentation is false). */
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	FText DisplayName;
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	FText Description;
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	FName SortGroup;
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	TSoftObjectPtr<UTexture2D> Icon;
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	bool bHasPresentation = false;

	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	int32 Quantity = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	int32 MaxStack = 1;
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	bool bStackable = false;
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	int64 UnitWeightGrams = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	int64 UnitVolumeMilliliters = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	int64 TotalWeightGrams = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	int64 TotalVolumeMilliliters = 0;
	/** The definition can occupy at least one equipment slot. */
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	bool bEquippable = false;
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	bool bEquipped = false;
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	FString EquippedSlot;
	/** Per-instance state of a non-stackable item (charge, notes ...). */
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	TArray<FSpnItemStateField> State;
};

UENUM(BlueprintType)
enum class ESpnCapacityKind : uint8
{
	Stacks,
	Weight,
	Volume,
};

/** One capacity meter. Max < 0 means unlimited (Fraction is then -1). */
USTRUCT(BlueprintType)
struct SPNCORE_API FSpnCapacityMeter
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	ESpnCapacityKind Kind = ESpnCapacityKind::Stacks;
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	int64 Used = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	int64 Max = -1;
	/** Used / Max in 0..1 (may exceed 1 only if the content exceeds the limit, which the Core forbids); -1 when unlimited. */
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	float Fraction = -1.f;
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	bool bUnlimited = true;
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	bool bFull = false;
};

USTRUCT(BlueprintType)
struct SPNCORE_API FSpnEquipmentSlotView
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	FString SlotId;
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	FText DisplayName;
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	int64 EquippedStackId = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	FString EquippedDefinitionId;
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	FText EquippedName;
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	bool bOccupied = false;
};

/** Everything a screen shows for one container. */
USTRUCT(BlueprintType)
struct SPNCORE_API FSpnContainerPanel
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	int64 ContainerId = 0;
	/** The container kind id as text (no invented titles). */
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	FString Kind;
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	FText Title;
	/** Visible rows after sorting and filtering. */
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	TArray<FSpnItemRow> Rows;
	/** Stacks in the container regardless of the filter. */
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	int32 TotalStackCount = 0;
	/** Category ids that occur in the container regardless of the filter (sorted); what a category filter can offer. */
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	TArray<FString> Categories;
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	FSpnCapacityMeter StackMeter;
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	FSpnCapacityMeter WeightMeter;
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	FSpnCapacityMeter VolumeMeter;
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	TArray<FSpnEquipmentSlotView> Slots;
};

/** One thing the user could do with the selected stack, with the Core's verdict. */
USTRUCT(BlueprintType)
struct SPNCORE_API FSpnActionOption
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	ESpnInventoryAction Action = ESpnInventoryAction::None;
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	FText Label;
	/** Transfer: the other container. */
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	int64 TargetContainerId = 0;
	/** Merge: the stack that receives. */
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	int64 TargetStackId = 0;
	/** Equip: the slot. */
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	FString Slot;
	/** Default quantity for the action (the whole stack for Transfer/Discard, half for Split) and the maximum. */
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	int32 DefaultQuantity = 1;
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	int32 MaxQuantity = 1;
	/** The Core accepts the action right now. */
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	bool bAvailable = false;
	/** When not available: why (ESpnInventoryError) and a message ready to display. */
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	ESpnInventoryError UnavailableReason = ESpnInventoryError::None;
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	FText UnavailableText;
};

/** A request to run an action; what a button or a confirm press sends. Unused fields stay default. */
USTRUCT(BlueprintType)
struct SPNCORE_API FSpnActionRequest
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Spn")
	ESpnInventoryAction Action = ESpnInventoryAction::None;
	UPROPERTY(BlueprintReadWrite, Category = "Spn")
	int64 ContainerId = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Spn")
	int64 StackId = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Spn")
	int64 TargetContainerId = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Spn")
	int64 TargetStackId = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Spn")
	FString Slot;
	UPROPERTY(BlueprintReadWrite, Category = "Spn")
	int32 Quantity = 1;
};

/** A notice for the player. Key is stable and localisable; Text is the English fallback. */
USTRUCT(BlueprintType)
struct SPNCORE_API FSpnInventoryMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	FString Key;
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	FText Text;
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	ESpnMessageSeverity Severity = ESpnMessageSeverity::Info;
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	ESpnInventoryError Error = ESpnInventoryError::None;
	/** Failed transaction: index of the command, -1 otherwise. */
	UPROPERTY(BlueprintReadOnly, Category = "Spn")
	int32 CommandIndex = -1;

	bool IsSet() const { return !Key.IsEmpty(); }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSpnViewModelChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpnViewModelContainerChanged, int64, ContainerId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSpnViewModelSelectionChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpnViewModelMessage, const FSpnInventoryMessage&, Message);

/** Message lookup shared by the view-model and widgets. */
SPNCORE_API FSpnInventoryMessage SpnMakeInventoryErrorMessage(ESpnInventoryError Error, int32 CommandIndex = -1);
/** Message for a finished load/save, or an unset message if there is nothing worth telling the player. */
SPNCORE_API FSpnInventoryMessage SpnMakeSaveMessage(const FSpnSaveStatus& Status, bool bLoad);

UCLASS(BlueprintType)
class SPNCORE_API USpnInventoryViewModel : public UObject
{
	GENERATED_BODY()

public:
	virtual void BeginDestroy() override;

	/** Binds to the subsystem (and optionally the save subsystem for load notices). Rebuilds the panels. Safe to call again. */
	void Initialize(USpnInventorySubsystem* InInventory, USpnSaveSubsystem* InSave = nullptr);
	/** Unbinds from all delegates. */
	UFUNCTION(BlueprintCallable, Category = "Spn|Inventory")
	void Shutdown();

	// --- what the screen shows -----------------------------------------------------------------------------------------------------

	/** Containers shown, in screen order; the first is the primary one (the player's body). Containers that do not exist yet have no panel. */
	UFUNCTION(BlueprintCallable, Category = "Spn|Inventory")
	void SetBoundContainers(const TArray<int64>& ContainerIds);
	UFUNCTION(BlueprintPure, Category = "Spn|Inventory")
	TArray<int64> GetBoundContainers() const { return BoundContainers; }
	UFUNCTION(BlueprintCallable, Category = "Spn|Inventory")
	bool GetPanel(int64 ContainerId, FSpnContainerPanel& OutPanel) const;
	const FSpnContainerPanel* FindPanel(int64 ContainerId) const;
	/** Panels of the bound containers that exist, in bound order. */
	UFUNCTION(BlueprintPure, Category = "Spn|Inventory")
	TArray<FSpnContainerPanel> GetPanels() const;

	/** Increases with every refresh; cheap to poll. */
	UFUNCTION(BlueprintPure, Category = "Spn|Inventory")
	int32 GetVersion() const { return Version; }
	/** True if the inventory is active (a catalog is loaded). */
	UFUNCTION(BlueprintPure, Category = "Spn|Inventory")
	bool IsInventoryActive() const;

	UFUNCTION(BlueprintCallable, Category = "Spn|Inventory")
	void SetSortMode(ESpnInventorySort Mode, bool bDescending = false);
	/** Only rows of this category id are shown; empty shows all. */
	UFUNCTION(BlueprintCallable, Category = "Spn|Inventory")
	void SetCategoryFilter(const FString& CategoryId);
	UFUNCTION(BlueprintPure, Category = "Spn|Inventory")
	ESpnInventorySort GetSortMode() const { return SortMode; }
	UFUNCTION(BlueprintPure, Category = "Spn|Inventory")
	bool IsSortDescending() const { return bSortDescending; }
	UFUNCTION(BlueprintPure, Category = "Spn|Inventory")
	FString GetCategoryFilter() const { return CategoryFilter; }
	UFUNCTION(BlueprintPure, Category = "Spn|Inventory")
	int32 GetGridColumns() const { return GridColumns; }

	// --- selection and navigation ---------------------------------------------------------------------------------------------------

	UFUNCTION(BlueprintCallable, Category = "Spn|Inventory")
	bool SelectStack(int64 ContainerId, int64 StackId);
	UFUNCTION(BlueprintCallable, Category = "Spn|Inventory")
	void ClearSelection();
	UFUNCTION(BlueprintPure, Category = "Spn|Inventory")
	int64 GetSelectedStackId() const { return SelectedStack; }
	UFUNCTION(BlueprintPure, Category = "Spn|Inventory")
	int64 GetSelectedContainerId() const { return SelectedContainer; }
	UFUNCTION(BlueprintCallable, Category = "Spn|Inventory")
	bool GetSelectedRow(FSpnItemRow& OutRow) const;

	/** Columns of the grid a widget draws; 1 = a plain list. Used by Up/Down/Left/Right. */
	UFUNCTION(BlueprintCallable, Category = "Spn|Inventory")
	void SetGridColumns(int32 Columns);
	UFUNCTION(BlueprintCallable, Category = "Spn|Inventory")
	void SetNavigationZone(ESpnNavigationZone Zone);
	UFUNCTION(BlueprintPure, Category = "Spn|Inventory")
	ESpnNavigationZone GetNavigationZone() const { return Zone; }
	/** When true, moving past the first/last row wraps around. Default false. */
	UFUNCTION(BlueprintCallable, Category = "Spn|Inventory")
	void SetWrapNavigation(bool bWrap) { bWrapNavigation = bWrap; }
	/**
	 * One navigation step for keyboard arrows, D-pad / stick, mouse wheel or a widget's own focus logic. Returns true if the selection changed.
	 * Items zone, list (1 column): Up/Down/Next/Previous move by one row, Left/Right change the section (container). Grid: Left/Right move within
	 * a row, Up/Down by a row. NextSection/PreviousSection always change the container. Equipment zone: Up/Down/Left/Right/Next/Previous move
	 * through the slots of the primary container; a slot with an item selects that item.
	 */
	UFUNCTION(BlueprintCallable, Category = "Spn|Inventory")
	bool Navigate(ESpnNavigationDirection Direction);
	/** The equipment slot the Equipment zone is focused on, empty if none. */
	UFUNCTION(BlueprintPure, Category = "Spn|Inventory")
	FString GetFocusedSlot() const { return FocusedSlot; }

	// --- actions --------------------------------------------------------------------------------------------------------------------

	/** The actions that make sense for the selected stack, each probed against the real inventory. Empty without a selection. */
	UFUNCTION(BlueprintCallable, Category = "Spn|Inventory")
	TArray<FSpnActionOption> GetAvailableActions();
	/** Runs an action as one real transaction. Returns true on success; the outcome is in GetLastMessage() and GetLastStatus(). */
	UFUNCTION(BlueprintCallable, Category = "Spn|Inventory")
	bool ExecuteAction(const FSpnActionRequest& Request);
	/** The confirm button: equips the selected stack into its first available slot, or unequips it if it is equipped; false if neither applies. */
	UFUNCTION(BlueprintCallable, Category = "Spn|Inventory")
	bool ExecuteDefaultAction();
	const FSpnInventoryStatus& GetLastStatus() const { return LastStatus; }

	// --- messages ---------------------------------------------------------------------------------------------------------------------

	UFUNCTION(BlueprintPure, Category = "Spn|Inventory")
	FSpnInventoryMessage GetLastMessage() const { return LastMessage; }
	UFUNCTION(BlueprintCallable, Category = "Spn|Inventory")
	void ClearMessage();

	// --- notifications -----------------------------------------------------------------------------------------------------------------

	/** Native (C++) counterparts of the dynamic delegates below, broadcast right after them. */
	FSimpleMulticastDelegate OnChangedNative;
	TMulticastDelegate<void(int64)> OnContainerChangedNative;
	FSimpleMulticastDelegate OnSelectionChangedNative;
	TMulticastDelegate<void(const FSpnInventoryMessage&)> OnMessageNative;

	/** Any panel, selection or message refresh. */
	UPROPERTY(BlueprintAssignable, Category = "Spn|Inventory")
	FSpnViewModelChanged OnChanged;
	/** One bound container was rebuilt. */
	UPROPERTY(BlueprintAssignable, Category = "Spn|Inventory")
	FSpnViewModelContainerChanged OnContainerChanged;
	UPROPERTY(BlueprintAssignable, Category = "Spn|Inventory")
	FSpnViewModelSelectionChanged OnSelectionChanged;
	UPROPERTY(BlueprintAssignable, Category = "Spn|Inventory")
	FSpnViewModelMessage OnMessage;

	// --- diagnostics (tests and profiling) ------------------------------------------------------------------------------------------------

	/** How many panels were rebuilt since creation (a refresh that touches no bound container adds nothing). */
	int32 GetRebuildCount() const { return RebuildCount; }
	/** Duration of the most recent panel rebuild in microseconds. */
	double GetLastRebuildMicroseconds() const { return LastRebuildMicroseconds; }

private:
	void HandleChanged(const FSpnInventoryChange& Change);
	void HandleLoadFinished(const FSpnSaveStatus& Status);
	void RebuildPanel(int64 ContainerId, TMap<FString, FSpnItemDefinitionData>& Definitions);
	void RebuildAll();
	void RevalidateSelection(int64 PreviousContainer, int64 PreviousStack, int32 PreviousIndex);
	void SetSelectionInternal(int64 ContainerId, int64 StackId, bool bBroadcast);
	void PostMessage(const FSpnInventoryMessage& Message);
	void MarkChanged();
	const FSpnContainerPanel* SelectedPanel() const;
	bool MoveWithinPanel(const FSpnContainerPanel& Panel, int32 CurrentIndex, int32 NewIndex);
	bool ChangeSection(int32 Step);
	bool NavigateEquipment(ESpnNavigationDirection Direction);
	TMap<FString, FSpnItemDefinitionData> MakeDefinitionMap() const;

	TWeakObjectPtr<USpnInventorySubsystem> Inventory;
	TWeakObjectPtr<USpnSaveSubsystem> Save;
	FDelegateHandle ChangedHandle;
	FDelegateHandle LoadFinishedHandle;

	TArray<int64> BoundContainers;
	TMap<int64, FSpnContainerPanel> Panels;
	int64 SelectedContainer = 0;
	int64 SelectedStack = 0;
	FString FocusedSlot;
	ESpnNavigationZone Zone = ESpnNavigationZone::Items;
	int32 GridColumns = 1;
	bool bWrapNavigation = false;
	ESpnInventorySort SortMode = ESpnInventorySort::Default;
	bool bSortDescending = false;
	FString CategoryFilter;
	int32 Version = 0;
	int32 RebuildCount = 0;
	double LastRebuildMicroseconds = 0.0;
	FSpnInventoryMessage LastMessage;
	FSpnInventoryStatus LastStatus;
};
