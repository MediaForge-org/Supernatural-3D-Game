#include "SpnInventoryViewModel.h"
#include "SpnCoreModule.h"

#include "HAL/PlatformTime.h"
#include "Internationalization/Text.h"

#define LOCTEXT_NAMESPACE "SpnInventory"

namespace
{
	struct FErrorInfo
	{
		const TCHAR* Key;
		FText Text;
	};

	// English fallback texts (developer-facing wording; the keys are the stable identifiers for localisation).
	FErrorInfo InfoFor(ESpnInventoryError Error)
	{
		switch (Error)
		{
		case ESpnInventoryError::None: return {TEXT("spn.inventory.ok"), LOCTEXT("Ok", "Done.")};
		case ESpnInventoryError::NoCatalog: return {TEXT("spn.inventory.error.no_catalog"), LOCTEXT("NoCatalog", "The inventory is not available.")};
		case ESpnInventoryError::InvalidArgument: return {TEXT("spn.inventory.error.invalid_argument"), LOCTEXT("InvalidArgument", "That action is not valid.")};
		case ESpnInventoryError::CatalogLocked: return {TEXT("spn.inventory.error.catalog_locked"), LOCTEXT("CatalogLocked", "The item definitions cannot change while containers exist.")};
		case ESpnInventoryError::InternalError: return {TEXT("spn.inventory.error.internal"), LOCTEXT("Internal", "Something went wrong. Nothing was changed.")};
		case ESpnInventoryError::InvalidId: return {TEXT("spn.inventory.error.invalid_id"), LOCTEXT("InvalidId", "That is not a valid choice.")};
		case ESpnInventoryError::UnknownDefinition: return {TEXT("spn.inventory.error.unknown_definition"), LOCTEXT("UnknownDefinition", "Unknown item.")};
		case ESpnInventoryError::UnknownContainer: return {TEXT("spn.inventory.error.unknown_container"), LOCTEXT("UnknownContainer", "That container does not exist.")};
		case ESpnInventoryError::DuplicateContainer: return {TEXT("spn.inventory.error.duplicate_container"), LOCTEXT("DuplicateContainer", "That container already exists.")};
		case ESpnInventoryError::UnknownItem: return {TEXT("spn.inventory.error.unknown_item"), LOCTEXT("UnknownItem", "That item is no longer there.")};
		case ESpnInventoryError::InvalidQuantity: return {TEXT("spn.inventory.error.invalid_quantity"), LOCTEXT("InvalidQuantity", "That quantity is not possible.")};
		case ESpnInventoryError::NotStackable: return {TEXT("spn.inventory.error.not_stackable"), LOCTEXT("NotStackable", "This item cannot be stacked.")};
		case ESpnInventoryError::StackMismatch: return {TEXT("spn.inventory.error.stack_mismatch"), LOCTEXT("StackMismatch", "These items cannot be combined.")};
		case ESpnInventoryError::SameContainer: return {TEXT("spn.inventory.error.same_container"), LOCTEXT("SameContainer", "It is already there.")};
		case ESpnInventoryError::ContainerNotEmpty: return {TEXT("spn.inventory.error.container_not_empty"), LOCTEXT("ContainerNotEmpty", "The container is not empty.")};
		case ESpnInventoryError::CategoryNotAllowed: return {TEXT("spn.inventory.error.category_not_allowed"), LOCTEXT("CategoryNotAllowed", "This kind of item is not allowed here.")};
		case ESpnInventoryError::StackLimitReached: return {TEXT("spn.inventory.error.stack_limit"), LOCTEXT("StackLimit", "No room: too many different items.")};
		case ESpnInventoryError::WeightLimitReached: return {TEXT("spn.inventory.error.weight_limit"), LOCTEXT("WeightLimit", "Too heavy for this container.")};
		case ESpnInventoryError::VolumeLimitReached: return {TEXT("spn.inventory.error.volume_limit"), LOCTEXT("VolumeLimit", "Too bulky for this container.")};
		case ESpnInventoryError::ItemEquipped: return {TEXT("spn.inventory.error.item_equipped"), LOCTEXT("ItemEquipped", "Take it off first.")};
		case ESpnInventoryError::UnknownSlot: return {TEXT("spn.inventory.error.unknown_slot"), LOCTEXT("UnknownSlot", "There is no such slot here.")};
		case ESpnInventoryError::NotEquippable: return {TEXT("spn.inventory.error.not_equippable"), LOCTEXT("NotEquippable", "This cannot be equipped there.")};
		case ESpnInventoryError::AlreadyEquipped: return {TEXT("spn.inventory.error.already_equipped"), LOCTEXT("AlreadyEquipped", "Already equipped.")};
		case ESpnInventoryError::SlotOccupied: return {TEXT("spn.inventory.error.slot_occupied"), LOCTEXT("SlotOccupied", "The slot is occupied.")};
		case ESpnInventoryError::SlotEmpty: return {TEXT("spn.inventory.error.slot_empty"), LOCTEXT("SlotEmpty", "Nothing is equipped there.")};
		case ESpnInventoryError::InvalidState: return {TEXT("spn.inventory.error.invalid_state"), LOCTEXT("InvalidState", "The item data is not valid.")};
		case ESpnInventoryError::ArithmeticOverflow: return {TEXT("spn.inventory.error.overflow"), LOCTEXT("Overflow", "The numbers are too large.")};
		case ESpnInventoryError::LimitExceeded: return {TEXT("spn.inventory.error.limit_exceeded"), LOCTEXT("LimitExceeded", "A limit was reached.")};
		case ESpnInventoryError::InvalidDefinition: return {TEXT("spn.inventory.error.invalid_definition"), LOCTEXT("InvalidDefinition", "An item definition is not valid.")};
		case ESpnInventoryError::DuplicateDefinition: return {TEXT("spn.inventory.error.duplicate_definition"), LOCTEXT("DuplicateDefinition", "An item definition appears twice.")};
		}
		return {TEXT("spn.inventory.error.unknown"), LOCTEXT("Unknown", "Unknown error.")};
	}

	FSpnCapacityMeter MakeMeter(ESpnCapacityKind Kind, int64 Used, int64 Max)
	{
		FSpnCapacityMeter Meter;
		Meter.Kind = Kind;
		Meter.Used = Used;
		Meter.Max = Max;
		Meter.bUnlimited = Max < 0;
		Meter.Fraction = Max < 0 ? -1.f : (Max == 0 ? (Used > 0 ? 1.f : 0.f) : static_cast<float>(static_cast<double>(Used) / static_cast<double>(Max)));
		Meter.bFull = Max >= 0 && Used >= Max;
		return Meter;
	}

	int64 ToId(uint64 Id) { return static_cast<int64>(Id); }
	uint64 ToCore(int64 Id) { return Id > 0 ? static_cast<uint64>(Id) : 0; }

	FString ActionName(ESpnInventoryAction Action)
	{
		switch (Action)
		{
		case ESpnInventoryAction::Equip: return TEXT("equip");
		case ESpnInventoryAction::Unequip: return TEXT("unequip");
		case ESpnInventoryAction::Transfer: return TEXT("transfer");
		case ESpnInventoryAction::Split: return TEXT("split");
		case ESpnInventoryAction::Merge: return TEXT("merge");
		case ESpnInventoryAction::Discard: return TEXT("discard");
		default: return TEXT("none");
		}
	}

	FText ActionDone(ESpnInventoryAction Action)
	{
		switch (Action)
		{
		case ESpnInventoryAction::Equip: return LOCTEXT("DoneEquip", "Equipped.");
		case ESpnInventoryAction::Unequip: return LOCTEXT("DoneUnequip", "Unequipped.");
		case ESpnInventoryAction::Transfer: return LOCTEXT("DoneTransfer", "Moved.");
		case ESpnInventoryAction::Split: return LOCTEXT("DoneSplit", "Split.");
		case ESpnInventoryAction::Merge: return LOCTEXT("DoneMerge", "Combined.");
		case ESpnInventoryAction::Discard: return LOCTEXT("DoneDiscard", "Discarded.");
		default: return FText::GetEmpty();
		}
	}
}

FSpnInventoryMessage SpnMakeInventoryErrorMessage(ESpnInventoryError Error, int32 CommandIndex)
{
	const FErrorInfo Info = InfoFor(Error);
	FSpnInventoryMessage Message;
	Message.Key = Info.Key;
	Message.Text = Info.Text;
	Message.Severity = Error == ESpnInventoryError::None ? ESpnMessageSeverity::Info : ESpnMessageSeverity::Error;
	Message.Error = Error;
	Message.CommandIndex = CommandIndex;
	return Message;
}

FSpnInventoryMessage SpnMakeSaveMessage(const FSpnSaveStatus& Status, bool bLoad)
{
	FSpnInventoryMessage Message;
	if (!bLoad)
	{
		if (!Status.IsOk())
		{
			Message.Key = TEXT("spn.save.save_failed");
			Message.Text = FText::Format(LOCTEXT("SaveFailed", "Saving failed ({0}). The previous save is unchanged."), FText::FromString(LexToString(Status.Result)));
			Message.Severity = ESpnMessageSeverity::Error;
		}
		return Message;
	}
	if (!Status.IsOk())
	{
		Message.Severity = ESpnMessageSeverity::Error;
		if (Status.Result == ESpnSaveResult::CatalogMismatch)
		{
			Message.Key = TEXT("spn.inventory.load.catalog_mismatch");
			Message.Text = LOCTEXT("CatalogMismatch", "This save was made with different item definitions and was not loaded. Nothing was changed.");
		}
		else
		{
			Message.Key = TEXT("spn.save.load_failed");
			Message.Text = FText::Format(LOCTEXT("LoadFailed", "Loading failed ({0}). Nothing was changed."), FText::FromString(LexToString(Status.Result)));
		}
		return Message;
	}
	if (Status.InventoryOutcome == ESpnInventoryLoadOutcome::MigratedEmptyNoSection)
	{
		Message.Key = TEXT("spn.inventory.load.migrated_empty");
		Message.Text = LOCTEXT("MigratedEmpty", "This save is from before inventories existed. The inventory starts empty.");
		Message.Severity = ESpnMessageSeverity::Info;
	}
	else if (Status.CatalogCheck == ESpnCatalogCheck::MismatchAccepted)
	{
		Message.Key = TEXT("spn.inventory.load.catalog_mismatch_accepted");
		Message.Text = LOCTEXT("MismatchAccepted", "This save was made with different item definitions. Item data may have changed.");
		Message.Severity = ESpnMessageSeverity::Warning;
	}
	else if (Status.CatalogCheck == ESpnCatalogCheck::LegacyNoFingerprint || Status.CatalogCheck == ESpnCatalogCheck::StaleFingerprint)
	{
		Message.Key = TEXT("spn.inventory.load.catalog_unverified");
		Message.Text = LOCTEXT("Unverified", "This save does not record its item definitions; they could not be verified.");
		Message.Severity = ESpnMessageSeverity::Info;
	}
	else if (Status.Source == ESpnSaveSource::Previous)
	{
		Message.Key = TEXT("spn.save.recovered_previous");
		Message.Text = LOCTEXT("RecoveredPrevious", "The latest save was damaged; the one before it was loaded.");
		Message.Severity = ESpnMessageSeverity::Warning;
	}
	return Message;
}

// === lifetime ================================================================================================================================

void USpnInventoryViewModel::BeginDestroy()
{
	Shutdown();
	Super::BeginDestroy();
}

void USpnInventoryViewModel::Initialize(USpnInventorySubsystem* InInventory, USpnSaveSubsystem* InSave)
{
	Shutdown();
	Inventory = InInventory;
	Save = InSave;
	if (InInventory != nullptr)
	{
		ChangedHandle = InInventory->OnInventoryChangedDetailed.AddUObject(this, &USpnInventoryViewModel::HandleChanged);
	}
	if (InSave != nullptr)
	{
		LoadFinishedHandle = InSave->OnLoadFinished.AddUObject(this, &USpnInventoryViewModel::HandleLoadFinished);
	}
	RebuildAll();
	MarkChanged();
}

void USpnInventoryViewModel::Shutdown()
{
	if (USpnInventorySubsystem* Subsystem = Inventory.Get())
	{
		Subsystem->OnInventoryChangedDetailed.Remove(ChangedHandle);
	}
	if (USpnSaveSubsystem* SaveSubsystem = Save.Get())
	{
		SaveSubsystem->OnLoadFinished.Remove(LoadFinishedHandle);
	}
	ChangedHandle.Reset();
	LoadFinishedHandle.Reset();
	Inventory.Reset();
	Save.Reset();
	Panels.Reset();
	SelectedContainer = 0;
	SelectedStack = 0;
}

bool USpnInventoryViewModel::IsInventoryActive() const
{
	const USpnInventorySubsystem* Subsystem = Inventory.Get();
	return Subsystem != nullptr && Subsystem->HasCatalog();
}

// === building the panels ======================================================================================================================

TMap<FString, FSpnItemDefinitionData> USpnInventoryViewModel::MakeDefinitionMap() const
{
	TMap<FString, FSpnItemDefinitionData> Map;
	if (const USpnInventorySubsystem* Subsystem = Inventory.Get())
	{
		for (const FSpnItemDefinitionData& Item : Subsystem->GetCatalogData().Items)
		{
			Map.Add(Item.Id, Item);
		}
	}
	return Map;
}

void USpnInventoryViewModel::RebuildPanel(int64 ContainerId, TMap<FString, FSpnItemDefinitionData>& Definitions)
{
	USpnInventorySubsystem* Subsystem = Inventory.Get();
	const double Start = FPlatformTime::Seconds();
	FSpnContainerView View;
	if (Subsystem == nullptr || !Subsystem->GetContainer(ToCore(ContainerId), View))
	{
		Panels.Remove(ContainerId);
		return;
	}

	FSpnContainerPanel Panel;
	Panel.ContainerId = ContainerId;
	Panel.Kind = View.Rules.Kind;
	Panel.Title = FText::FromString(View.Rules.Kind.IsEmpty() ? FString::Printf(TEXT("#%lld"), ContainerId) : View.Rules.Kind);
	Panel.TotalStackCount = View.Stacks.Num();

	int64 StackUsed = View.Stacks.Num();
	Panel.StackMeter = MakeMeter(ESpnCapacityKind::Stacks, StackUsed, View.Rules.MaxStacks);
	Panel.WeightMeter = MakeMeter(ESpnCapacityKind::Weight, static_cast<int64>(View.WeightGrams), View.Rules.MaxWeightGrams);
	Panel.VolumeMeter = MakeMeter(ESpnCapacityKind::Volume, static_cast<int64>(View.VolumeMilliliters), View.Rules.MaxVolumeMilliliters);

	TMap<FString, int64> SlotToStack;
	TMap<FString, const FSpnStackView*> StackById;
	Panel.Rows.Reserve(View.Stacks.Num());
	for (const FSpnStackView& Stack : View.Stacks)
	{
		const FSpnItemDefinitionData* Definition = Definitions.Find(Stack.Definition);
		if (!Stack.EquippedSlot.IsEmpty())
		{
			SlotToStack.Add(Stack.EquippedSlot, ToId(Stack.StackId));
		}
		if (Definition != nullptr && !Definition->Category.IsEmpty())
		{
			Panel.Categories.AddUnique(Definition->Category);
		}
		if (!CategoryFilter.IsEmpty() && (Definition == nullptr || Definition->Category != CategoryFilter))
		{
			continue;
		}
		FSpnItemRow Row;
		Row.StackId = ToId(Stack.StackId);
		Row.ContainerId = ContainerId;
		Row.DefinitionId = Stack.Definition;
		Row.Quantity = static_cast<int32>(Stack.Quantity);
		Row.State = Stack.State;
		Row.EquippedSlot = Stack.EquippedSlot;
		Row.bEquipped = !Stack.EquippedSlot.IsEmpty();
		if (Definition != nullptr)
		{
			Row.Category = Definition->Category;
			Row.MaxStack = Definition->MaxStack;
			Row.bStackable = Definition->MaxStack > 1;
			Row.UnitWeightGrams = static_cast<int64>(Definition->WeightGrams);
			Row.UnitVolumeMilliliters = static_cast<int64>(Definition->VolumeMilliliters);
			Row.TotalWeightGrams = Row.UnitWeightGrams * Row.Quantity;
			Row.TotalVolumeMilliliters = Row.UnitVolumeMilliliters * Row.Quantity;
			Row.bEquippable = Definition->EquipSlots.Num() > 0;
		}
		const FSpnItemPresentationEntry* Presentation = Subsystem->FindPresentation(Stack.Definition);
		Row.bHasPresentation = Presentation != nullptr;
		Row.DisplayName = Presentation != nullptr && !Presentation->DisplayName.IsEmpty() ? Presentation->DisplayName : FText::FromString(Stack.Definition);
		if (Presentation != nullptr)
		{
			Row.Description = Presentation->Description;
			Row.SortGroup = Presentation->SortGroup;
			Row.Icon = Presentation->Icon;
		}
		Panel.Rows.Add(MoveTemp(Row));
	}

	Panel.Categories.Sort();

	// order
	const auto Compare = [this](const FSpnItemRow& A, const FSpnItemRow& B) -> bool
	{
		int32 Order = 0;
		switch (SortMode)
		{
		case ESpnInventorySort::Name: Order = A.DisplayName.ToString().Compare(B.DisplayName.ToString(), ESearchCase::IgnoreCase); break;
		case ESpnInventorySort::Category: Order = A.Category.Compare(B.Category, ESearchCase::IgnoreCase); break;
		case ESpnInventorySort::TotalWeight: Order = A.TotalWeightGrams < B.TotalWeightGrams ? -1 : (A.TotalWeightGrams > B.TotalWeightGrams ? 1 : 0); break;
		case ESpnInventorySort::TotalVolume: Order = A.TotalVolumeMilliliters < B.TotalVolumeMilliliters ? -1 : (A.TotalVolumeMilliliters > B.TotalVolumeMilliliters ? 1 : 0); break;
		case ESpnInventorySort::Quantity: Order = A.Quantity < B.Quantity ? -1 : (A.Quantity > B.Quantity ? 1 : 0); break;
		case ESpnInventorySort::Default: break;
		}
		if (Order == 0)
		{
			return A.StackId < B.StackId; // always a total order: ties by stack id, so the order is reproducible
		}
		return bSortDescending ? Order > 0 : Order < 0;
	};
	if (SortMode != ESpnInventorySort::Default || bSortDescending)
	{
		Panel.Rows.Sort(Compare);
	}

	for (const FString& SlotId : View.Rules.EquipSlots)
	{
		FSpnEquipmentSlotView Slot;
		Slot.SlotId = SlotId;
		Slot.DisplayName = FText::FromString(SlotId);
		if (const int64* Stack = SlotToStack.Find(SlotId))
		{
			Slot.EquippedStackId = *Stack;
			Slot.bOccupied = true;
			for (const FSpnStackView& S : View.Stacks)
			{
				if (ToId(S.StackId) == *Stack)
				{
					Slot.EquippedDefinitionId = S.Definition;
					const FSpnItemPresentationEntry* Presentation = Subsystem->FindPresentation(S.Definition);
					Slot.EquippedName = Presentation != nullptr && !Presentation->DisplayName.IsEmpty() ? Presentation->DisplayName : FText::FromString(S.Definition);
				}
			}
		}
		Panel.Slots.Add(MoveTemp(Slot));
	}

	Panels.Add(ContainerId, MoveTemp(Panel));
	++RebuildCount;
	LastRebuildMicroseconds = (FPlatformTime::Seconds() - Start) * 1.0e6;
}

void USpnInventoryViewModel::RebuildAll()
{
	TMap<FString, FSpnItemDefinitionData> Definitions = MakeDefinitionMap();
	Panels.Reset();
	for (const int64 Id : BoundContainers)
	{
		RebuildPanel(Id, Definitions);
	}
}

const FSpnContainerPanel* USpnInventoryViewModel::FindPanel(int64 ContainerId) const
{
	return Panels.Find(ContainerId);
}

bool USpnInventoryViewModel::GetPanel(int64 ContainerId, FSpnContainerPanel& OutPanel) const
{
	if (const FSpnContainerPanel* Panel = Panels.Find(ContainerId))
	{
		OutPanel = *Panel;
		return true;
	}
	return false;
}

TArray<FSpnContainerPanel> USpnInventoryViewModel::GetPanels() const
{
	TArray<FSpnContainerPanel> Out;
	for (const int64 Id : BoundContainers)
	{
		if (const FSpnContainerPanel* Panel = Panels.Find(Id))
		{
			Out.Add(*Panel);
		}
	}
	return Out;
}

void USpnInventoryViewModel::MarkChanged()
{
	++Version;
	OnChanged.Broadcast();
	OnChangedNative.Broadcast();
}

void USpnInventoryViewModel::SetBoundContainers(const TArray<int64>& ContainerIds)
{
	BoundContainers = ContainerIds;
	const int64 PreviousContainer = SelectedContainer;
	const int64 PreviousStack = SelectedStack;
	RebuildAll();
	RevalidateSelection(PreviousContainer, PreviousStack, 0);
	MarkChanged();
}

void USpnInventoryViewModel::SetSortMode(ESpnInventorySort Mode, bool bDescending)
{
	SortMode = Mode;
	bSortDescending = bDescending;
	const int64 PreviousContainer = SelectedContainer;
	const int64 PreviousStack = SelectedStack;
	RebuildAll();
	RevalidateSelection(PreviousContainer, PreviousStack, 0);
	MarkChanged();
}

void USpnInventoryViewModel::SetCategoryFilter(const FString& CategoryId)
{
	CategoryFilter = CategoryId;
	const int64 PreviousContainer = SelectedContainer;
	const int64 PreviousStack = SelectedStack;
	RebuildAll();
	RevalidateSelection(PreviousContainer, PreviousStack, 0);
	MarkChanged();
}

// === refresh ===================================================================================================================================

void USpnInventoryViewModel::HandleChanged(const FSpnInventoryChange& Change)
{
	const int64 PreviousContainer = SelectedContainer;
	const int64 PreviousStack = SelectedStack;
	int32 PreviousIndex = 0;
	if (const FSpnContainerPanel* Panel = FindPanel(PreviousContainer))
	{
		PreviousIndex = Panel->Rows.IndexOfByPredicate([PreviousStack](const FSpnItemRow& Row) { return Row.StackId == PreviousStack; });
		PreviousIndex = FMath::Max(PreviousIndex, 0);
	}

	TArray<int64> Rebuilt;
	TMap<FString, FSpnItemDefinitionData> Definitions;
	bool bHaveDefinitions = false;
	for (const int64 Id : BoundContainers)
	{
		if (Change.Touches(static_cast<uint64>(Id)))
		{
			if (!bHaveDefinitions)
			{
				Definitions = MakeDefinitionMap();
				bHaveDefinitions = true;
			}
			RebuildPanel(Id, Definitions);
			Rebuilt.Add(Id);
		}
	}
	if (Rebuilt.IsEmpty())
	{
		return; // nothing this screen shows was touched: no work, no notification
	}
	RevalidateSelection(PreviousContainer, PreviousStack, PreviousIndex);
	for (const int64 Id : Rebuilt)
	{
		OnContainerChanged.Broadcast(Id);
		OnContainerChangedNative.Broadcast(Id);
	}
	MarkChanged();
}

void USpnInventoryViewModel::HandleLoadFinished(const FSpnSaveStatus& Status)
{
	const FSpnInventoryMessage Message = SpnMakeSaveMessage(Status, /*bLoad*/ true);
	if (Message.IsSet())
	{
		PostMessage(Message);
	}
}

void USpnInventoryViewModel::PostMessage(const FSpnInventoryMessage& Message)
{
	LastMessage = Message;
	OnMessage.Broadcast(Message);
	OnMessageNative.Broadcast(Message);
}

void USpnInventoryViewModel::ClearMessage()
{
	LastMessage = FSpnInventoryMessage();
}

// === selection ===================================================================================================================================

const FSpnContainerPanel* USpnInventoryViewModel::SelectedPanel() const
{
	return SelectedStack != 0 ? FindPanel(SelectedContainer) : nullptr;
}

void USpnInventoryViewModel::SetSelectionInternal(int64 ContainerId, int64 StackId, bool bBroadcast)
{
	const bool bChanged = ContainerId != SelectedContainer || StackId != SelectedStack;
	SelectedContainer = ContainerId;
	SelectedStack = StackId;
	if (bChanged && bBroadcast)
	{
		OnSelectionChanged.Broadcast();
		OnSelectionChangedNative.Broadcast();
	}
}

void USpnInventoryViewModel::RevalidateSelection(int64 PreviousContainer, int64 PreviousStack, int32 PreviousIndex)
{
	if (PreviousStack == 0)
	{
		return;
	}
	// 1. still where it was
	if (const FSpnContainerPanel* Panel = FindPanel(PreviousContainer))
	{
		if (Panel->Rows.ContainsByPredicate([PreviousStack](const FSpnItemRow& Row) { return Row.StackId == PreviousStack; }))
		{
			SetSelectionInternal(PreviousContainer, PreviousStack, true);
			return;
		}
	}
	// 2. moved to another bound container: follow it
	for (const int64 Id : BoundContainers)
	{
		if (const FSpnContainerPanel* Panel = FindPanel(Id))
		{
			if (Panel->Rows.ContainsByPredicate([PreviousStack](const FSpnItemRow& Row) { return Row.StackId == PreviousStack; }))
			{
				SetSelectionInternal(Id, PreviousStack, true);
				return;
			}
		}
	}
	// 3. gone (or filtered out): the neighbouring row of the same container, else nothing
	if (const FSpnContainerPanel* Panel = FindPanel(PreviousContainer))
	{
		if (!Panel->Rows.IsEmpty())
		{
			const int32 Index = FMath::Clamp(PreviousIndex, 0, Panel->Rows.Num() - 1);
			SetSelectionInternal(PreviousContainer, Panel->Rows[Index].StackId, true);
			return;
		}
	}
	SetSelectionInternal(0, 0, true);
}

bool USpnInventoryViewModel::SelectStack(int64 ContainerId, int64 StackId)
{
	if (const FSpnContainerPanel* Panel = FindPanel(ContainerId))
	{
		if (Panel->Rows.ContainsByPredicate([StackId](const FSpnItemRow& Row) { return Row.StackId == StackId; }))
		{
			SetSelectionInternal(ContainerId, StackId, true);
			return true;
		}
	}
	return false;
}

void USpnInventoryViewModel::ClearSelection()
{
	SetSelectionInternal(0, 0, true);
}

bool USpnInventoryViewModel::GetSelectedRow(FSpnItemRow& OutRow) const
{
	if (const FSpnContainerPanel* Panel = SelectedPanel())
	{
		for (const FSpnItemRow& Row : Panel->Rows)
		{
			if (Row.StackId == SelectedStack)
			{
				OutRow = Row;
				return true;
			}
		}
	}
	return false;
}

// === navigation ==================================================================================================================================

void USpnInventoryViewModel::SetGridColumns(int32 Columns)
{
	GridColumns = FMath::Max(1, Columns);
}

void USpnInventoryViewModel::SetNavigationZone(ESpnNavigationZone InZone)
{
	Zone = InZone;
	if (Zone == ESpnNavigationZone::Equipment && FocusedSlot.IsEmpty() && !BoundContainers.IsEmpty())
	{
		if (const FSpnContainerPanel* Panel = FindPanel(BoundContainers[0]))
		{
			if (!Panel->Slots.IsEmpty())
			{
				FocusedSlot = Panel->Slots[0].SlotId;
			}
		}
	}
}

bool USpnInventoryViewModel::MoveWithinPanel(const FSpnContainerPanel& Panel, int32 CurrentIndex, int32 NewIndex)
{
	const int32 Count = Panel.Rows.Num();
	if (Count == 0)
	{
		return false;
	}
	if (NewIndex < 0 || NewIndex >= Count)
	{
		if (!bWrapNavigation)
		{
			return false;
		}
		NewIndex = (NewIndex % Count + Count) % Count;
	}
	if (NewIndex == CurrentIndex)
	{
		return false;
	}
	SetSelectionInternal(Panel.ContainerId, Panel.Rows[NewIndex].StackId, true);
	return true;
}

bool USpnInventoryViewModel::ChangeSection(int32 Step)
{
	TArray<const FSpnContainerPanel*> WithRows;
	for (const int64 Id : BoundContainers)
	{
		if (const FSpnContainerPanel* Panel = FindPanel(Id))
		{
			if (!Panel->Rows.IsEmpty())
			{
				WithRows.Add(Panel);
			}
		}
	}
	if (WithRows.Num() < 2)
	{
		return false;
	}
	int32 Current = WithRows.IndexOfByPredicate([this](const FSpnContainerPanel* Panel) { return Panel->ContainerId == SelectedContainer; });
	int32 Index = 0;
	if (const FSpnContainerPanel* Panel = SelectedPanel())
	{
		Index = FMath::Max(0, Panel->Rows.IndexOfByPredicate([this](const FSpnItemRow& Row) { return Row.StackId == SelectedStack; }));
	}
	if (Current == INDEX_NONE)
	{
		Current = 0;
	}
	const int32 Target = ((Current + Step) % WithRows.Num() + WithRows.Num()) % WithRows.Num();
	const FSpnContainerPanel* To = WithRows[Target];
	SetSelectionInternal(To->ContainerId, To->Rows[FMath::Min(Index, To->Rows.Num() - 1)].StackId, true);
	return true;
}

bool USpnInventoryViewModel::NavigateEquipment(ESpnNavigationDirection Direction)
{
	if (BoundContainers.IsEmpty())
	{
		return false;
	}
	const FSpnContainerPanel* Panel = FindPanel(BoundContainers[0]);
	if (Panel == nullptr || Panel->Slots.IsEmpty())
	{
		return false;
	}
	int32 Index = Panel->Slots.IndexOfByPredicate([this](const FSpnEquipmentSlotView& Slot) { return Slot.SlotId == FocusedSlot; });
	const int32 Count = Panel->Slots.Num();
	int32 NewIndex = Index < 0 ? 0 : Index;
	switch (Direction)
	{
	case ESpnNavigationDirection::Up:
	case ESpnNavigationDirection::Left:
	case ESpnNavigationDirection::Previous:
	case ESpnNavigationDirection::PageUp:
		NewIndex = Index < 0 ? 0 : Index - 1;
		break;
	case ESpnNavigationDirection::Down:
	case ESpnNavigationDirection::Right:
	case ESpnNavigationDirection::Next:
	case ESpnNavigationDirection::PageDown:
		NewIndex = Index < 0 ? 0 : Index + 1;
		break;
	case ESpnNavigationDirection::First: NewIndex = 0; break;
	case ESpnNavigationDirection::Last: NewIndex = Count - 1; break;
	default: return false;
	}
	if (NewIndex < 0 || NewIndex >= Count)
	{
		if (!bWrapNavigation)
		{
			return false;
		}
		NewIndex = (NewIndex % Count + Count) % Count;
	}
	if (NewIndex == Index)
	{
		return false;
	}
	FocusedSlot = Panel->Slots[NewIndex].SlotId;
	const FSpnEquipmentSlotView& Slot = Panel->Slots[NewIndex];
	SetSelectionInternal(Slot.bOccupied ? Panel->ContainerId : 0, Slot.bOccupied ? Slot.EquippedStackId : 0, true);
	OnSelectionChanged.Broadcast(); // the focused slot changed even if the selection did not
	OnSelectionChangedNative.Broadcast();
	return true;
}

bool USpnInventoryViewModel::Navigate(ESpnNavigationDirection Direction)
{
	if (Zone == ESpnNavigationZone::Equipment)
	{
		return NavigateEquipment(Direction);
	}
	TArray<const FSpnContainerPanel*> WithRows;
	for (const int64 Id : BoundContainers)
	{
		if (const FSpnContainerPanel* Panel = FindPanel(Id))
		{
			if (!Panel->Rows.IsEmpty())
			{
				WithRows.Add(Panel);
			}
		}
	}
	if (WithRows.IsEmpty())
	{
		return false;
	}
	const FSpnContainerPanel* Panel = SelectedPanel();
	if (Panel == nullptr)
	{
		// nothing selected yet: any step selects the first row (the last one for First/Last backwards moves)
		const FSpnContainerPanel* First = WithRows[0];
		const bool bBackwards = Direction == ESpnNavigationDirection::Last || Direction == ESpnNavigationDirection::PreviousSection;
		const FSpnContainerPanel* Target = bBackwards ? WithRows.Last() : First;
		SetSelectionInternal(Target->ContainerId, Target->Rows[bBackwards && Direction == ESpnNavigationDirection::Last ? Target->Rows.Num() - 1 : 0].StackId, true);
		return true;
	}
	const int32 Count = Panel->Rows.Num();
	const int32 Index = FMath::Max(0, Panel->Rows.IndexOfByPredicate([this](const FSpnItemRow& Row) { return Row.StackId == SelectedStack; }));
	const int32 Columns = FMath::Max(1, GridColumns);
	switch (Direction)
	{
	case ESpnNavigationDirection::Up:
		return MoveWithinPanel(*Panel, Index, Columns > 1 ? Index - Columns : Index - 1);
	case ESpnNavigationDirection::Down:
	{
		if (Columns > 1 && Index + Columns >= Count && Index / Columns < (Count - 1) / Columns)
		{
			return MoveWithinPanel(*Panel, Index, Count - 1); // the last row is shorter: land on its last item
		}
		return MoveWithinPanel(*Panel, Index, Columns > 1 ? Index + Columns : Index + 1);
	}
	case ESpnNavigationDirection::Left:
		if (Columns == 1)
		{
			return ChangeSection(-1);
		}
		return Index % Columns == 0 ? false : MoveWithinPanel(*Panel, Index, Index - 1);
	case ESpnNavigationDirection::Right:
		if (Columns == 1)
		{
			return ChangeSection(+1);
		}
		return (Index % Columns == Columns - 1 || Index == Count - 1) ? false : MoveWithinPanel(*Panel, Index, Index + 1);
	case ESpnNavigationDirection::Next: return MoveWithinPanel(*Panel, Index, Index + 1);
	case ESpnNavigationDirection::Previous: return MoveWithinPanel(*Panel, Index, Index - 1);
	case ESpnNavigationDirection::PageUp: return MoveWithinPanel(*Panel, Index, FMath::Max(0, Index - 5 * Columns));
	case ESpnNavigationDirection::PageDown: return MoveWithinPanel(*Panel, Index, FMath::Min(Count - 1, Index + 5 * Columns));
	case ESpnNavigationDirection::First: return MoveWithinPanel(*Panel, Index, 0);
	case ESpnNavigationDirection::Last: return MoveWithinPanel(*Panel, Index, Count - 1);
	case ESpnNavigationDirection::NextSection: return ChangeSection(+1);
	case ESpnNavigationDirection::PreviousSection: return ChangeSection(-1);
	}
	return false;
}

// === actions =====================================================================================================================================

TArray<FSpnActionOption> USpnInventoryViewModel::GetAvailableActions()
{
	TArray<FSpnActionOption> Options;
	USpnInventorySubsystem* Subsystem = Inventory.Get();
	FSpnItemRow Row;
	if (Subsystem == nullptr || !GetSelectedRow(Row))
	{
		return Options;
	}
	const FSpnContainerPanel* Panel = FindPanel(Row.ContainerId);
	FSpnItemDefinitionData Definition;
	Subsystem->FindDefinition(Row.DefinitionId, Definition);

	const auto Probe = [Subsystem](FSpnActionOption& Option, const FSpnInventoryCommand& Command)
	{
		const FSpnInventoryStatus Status = Subsystem->ProbeCommands({Command});
		Option.bAvailable = Status.IsOk();
		if (!Status.IsOk())
		{
			Option.UnavailableReason = Status.Error;
			Option.UnavailableText = SpnMakeInventoryErrorMessage(Status.Error, Status.FailedCommandIndex).Text;
		}
	};

	if (Row.bEquipped)
	{
		FSpnActionOption Option;
		Option.Action = ESpnInventoryAction::Unequip;
		Option.Slot = Row.EquippedSlot;
		Option.Label = FText::Format(LOCTEXT("UnequipLabel", "Unequip ({0})"), FText::FromString(Row.EquippedSlot));
		Probe(Option, FSpnInventoryCommand::UnequipSlot(ToCore(Row.ContainerId), Row.EquippedSlot));
		Options.Add(MoveTemp(Option));
	}
	else if (Panel != nullptr)
	{
		for (const FSpnEquipmentSlotView& Slot : Panel->Slots)
		{
			if (!Definition.EquipSlots.Contains(Slot.SlotId))
			{
				continue; // not a slot this item could ever use: not offered at all
			}
			FSpnActionOption Option;
			Option.Action = ESpnInventoryAction::Equip;
			Option.Slot = Slot.SlotId;
			Option.Label = FText::Format(LOCTEXT("EquipLabel", "Equip ({0})"), FText::FromString(Slot.SlotId));
			Probe(Option, FSpnInventoryCommand::EquipItem(ToCore(Row.ContainerId), ToCore(Row.StackId), Slot.SlotId));
			Options.Add(MoveTemp(Option));
		}
	}
	if (!Row.bEquipped)
	{
		for (const int64 Target : BoundContainers)
		{
			if (Target == Row.ContainerId || FindPanel(Target) == nullptr)
			{
				continue;
			}
			FSpnActionOption Option;
			Option.Action = ESpnInventoryAction::Transfer;
			Option.TargetContainerId = Target;
			Option.DefaultQuantity = Row.Quantity;
			Option.MaxQuantity = Row.Quantity;
			Option.Label = FText::Format(LOCTEXT("TransferLabel", "Move to {0}"), FindPanel(Target)->Title);
			Probe(Option, FSpnInventoryCommand::TransferItem(ToCore(Row.ContainerId), ToCore(Target), ToCore(Row.StackId), static_cast<uint32>(Row.Quantity)));
			Options.Add(MoveTemp(Option));
		}
		if (Row.bStackable && Row.Quantity >= 2)
		{
			FSpnActionOption Option;
			Option.Action = ESpnInventoryAction::Split;
			Option.DefaultQuantity = Row.Quantity / 2;
			Option.MaxQuantity = Row.Quantity - 1;
			Option.Label = LOCTEXT("SplitLabel", "Split");
			Probe(Option, FSpnInventoryCommand::SplitStack(ToCore(Row.ContainerId), ToCore(Row.StackId), static_cast<uint32>(Option.DefaultQuantity)));
			Options.Add(MoveTemp(Option));
		}
		if (Row.bStackable && Panel != nullptr)
		{
			for (const FSpnItemRow& Other : Panel->Rows)
			{
				if (Other.StackId != Row.StackId && Other.DefinitionId == Row.DefinitionId)
				{
					FSpnActionOption Option;
					Option.Action = ESpnInventoryAction::Merge;
					Option.TargetStackId = Other.StackId;
					Option.DefaultQuantity = 0;
					Option.MaxQuantity = Row.Quantity;
					Option.Label = LOCTEXT("MergeLabel", "Combine");
					Probe(Option, FSpnInventoryCommand::MergeStacks(ToCore(Row.ContainerId), ToCore(Row.StackId), ToCore(Other.StackId), 0));
					Options.Add(MoveTemp(Option));
					break; // one candidate is enough for a menu entry; the request can name any stack
				}
			}
		}
		FSpnActionOption Discard;
		Discard.Action = ESpnInventoryAction::Discard;
		Discard.DefaultQuantity = Row.Quantity;
		Discard.MaxQuantity = Row.Quantity;
		Discard.Label = LOCTEXT("DiscardLabel", "Discard");
		Probe(Discard, FSpnInventoryCommand::RemoveItem(ToCore(Row.ContainerId), ToCore(Row.StackId), static_cast<uint32>(Row.Quantity)));
		Options.Add(MoveTemp(Discard));
	}
	return Options;
}

bool USpnInventoryViewModel::ExecuteAction(const FSpnActionRequest& Request)
{
	USpnInventorySubsystem* Subsystem = Inventory.Get();
	if (Subsystem == nullptr)
	{
		LastStatus = FSpnInventoryStatus();
		LastStatus.Error = ESpnInventoryError::NoCatalog;
		PostMessage(SpnMakeInventoryErrorMessage(ESpnInventoryError::NoCatalog));
		return false;
	}
	const uint32 Quantity = Request.Quantity > 0 ? static_cast<uint32>(Request.Quantity) : 0;
	FSpnInventoryCommand Command;
	switch (Request.Action)
	{
	case ESpnInventoryAction::Equip: Command = FSpnInventoryCommand::EquipItem(ToCore(Request.ContainerId), ToCore(Request.StackId), Request.Slot); break;
	case ESpnInventoryAction::Unequip: Command = FSpnInventoryCommand::UnequipSlot(ToCore(Request.ContainerId), Request.Slot); break;
	case ESpnInventoryAction::Transfer: Command = FSpnInventoryCommand::TransferItem(ToCore(Request.ContainerId), ToCore(Request.TargetContainerId), ToCore(Request.StackId), Quantity); break;
	case ESpnInventoryAction::Split: Command = FSpnInventoryCommand::SplitStack(ToCore(Request.ContainerId), ToCore(Request.StackId), Quantity); break;
	case ESpnInventoryAction::Merge: Command = FSpnInventoryCommand::MergeStacks(ToCore(Request.ContainerId), ToCore(Request.StackId), ToCore(Request.TargetStackId), Quantity); break;
	case ESpnInventoryAction::Discard: Command = FSpnInventoryCommand::RemoveItem(ToCore(Request.ContainerId), ToCore(Request.StackId), Quantity); break;
	case ESpnInventoryAction::None:
		LastStatus = FSpnInventoryStatus();
		LastStatus.Error = ESpnInventoryError::InvalidArgument;
		PostMessage(SpnMakeInventoryErrorMessage(ESpnInventoryError::InvalidArgument));
		return false;
	}
	LastStatus = Subsystem->Apply({Command});
	if (!LastStatus.IsOk())
	{
		PostMessage(SpnMakeInventoryErrorMessage(LastStatus.Error, LastStatus.FailedCommandIndex));
		return false;
	}
	FSpnInventoryMessage Message;
	Message.Key = TEXT("spn.inventory.action.") + ActionName(Request.Action);
	Message.Text = ActionDone(Request.Action);
	PostMessage(Message);
	return true;
}

bool USpnInventoryViewModel::ExecuteDefaultAction()
{
	FSpnItemRow Row;
	if (!GetSelectedRow(Row))
	{
		return false;
	}
	const TArray<FSpnActionOption> Options = GetAvailableActions();
	const FSpnActionOption* First = nullptr;
	for (const FSpnActionOption& Option : Options)
	{
		if (Option.Action != ESpnInventoryAction::Equip && Option.Action != ESpnInventoryAction::Unequip)
		{
			continue;
		}
		if (First == nullptr)
		{
			First = &Option;
		}
		if (Option.bAvailable)
		{
			FSpnActionRequest Request;
			Request.Action = Option.Action;
			Request.ContainerId = Row.ContainerId;
			Request.StackId = Row.StackId;
			Request.Slot = Option.Slot;
			return ExecuteAction(Request);
		}
	}
	if (First != nullptr)
	{
		PostMessage(SpnMakeInventoryErrorMessage(First->UnavailableReason));
	}
	else
	{
		FSpnInventoryMessage Message;
		Message.Key = TEXT("spn.inventory.no_default_action");
		Message.Text = LOCTEXT("NoDefault", "Nothing to do with this item here.");
		Message.Severity = ESpnMessageSeverity::Info;
		PostMessage(Message);
	}
	return false;
}

#undef LOCTEXT_NAMESPACE
