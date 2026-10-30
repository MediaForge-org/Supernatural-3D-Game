#include "SupernaturalProt.h"
#include "SpnInventorySubsystem.h"
#include "SpnInventoryViewModel.h"
#include "SpnSaveSubsystem.h"

#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "HAL/IConsoleManager.h"

// DEVELOPER-ONLY inventory commands for exercising USpnInventorySubsystem in a real session (M2B3.3). Not a UI and not game content:
// they print to the log and the on-screen debug line only. They use the TEMPORARY test catalog (Content/SpnTest/Inventory/TestItemCatalog.json).
// Ids may be abbreviated for typing: "ration" means "spn_test:item.ration" (items), "hand" means "spn_test:slot.hand" (slots),
// "body" means "spn_test:preset.body" (presets). Container and stack ids are plain numbers.

#if !UE_BUILD_SHIPPING

namespace
{
	USpnInventorySubsystem* FindInventory(const UWorld* World, const TCHAR* Command)
	{
		const UGameInstance* GameInstance = World != nullptr ? World->GetGameInstance() : nullptr;
		USpnInventorySubsystem* Inventory = GameInstance != nullptr ? GameInstance->GetSubsystem<USpnInventorySubsystem>() : nullptr;
		if (Inventory == nullptr)
		{
			UE_LOG(LogSupernaturalProt, Warning, TEXT("%s FAILED: no inventory subsystem; run it while playing (PIE or game)"), Command);
		}
		return Inventory;
	}

	FString InvExpand(const FString& Text, const TCHAR* Prefix)
	{
		return Text.Contains(TEXT(":")) ? Text : FString(Prefix) + Text;
	}

	bool InvParseNumber(const TArray<FString>& Args, int32 Index, uint64& Out)
	{
		if (!Args.IsValidIndex(Index) || !Args[Index].IsNumeric() || Args[Index].StartsWith(TEXT("-")) || Args[Index].Contains(TEXT(".")))
		{
			return false;
		}
		Out = FCString::Strtoui64(*Args[Index], nullptr, 10);
		return true;
	}

	uint32 InvOptionalQuantity(const TArray<FString>& Args, int32 Index, uint32 Default)
	{
		uint64 Value = 0;
		return InvParseNumber(Args, Index, Value) ? static_cast<uint32>(FMath::Min<uint64>(Value, TNumericLimits<uint32>::Max())) : Default;
	}

	void InvReport(const TCHAR* Command, const FSpnInventoryStatus& Status, const USpnInventorySubsystem& Inventory)
	{
		if (Status.IsOk())
		{
			FString Created;
			for (const uint64 Id : Status.CreatedStacks)
			{
				Created += FString::Printf(TEXT(" %llu"), static_cast<unsigned long long>(Id));
			}
			const FString Line = FString::Printf(TEXT("%s OK: revision %llu%s%s"), Command, static_cast<unsigned long long>(Status.Revision),
				Created.IsEmpty() ? TEXT("") : TEXT(", created stack(s):"), *Created);
			UE_LOG(LogSupernaturalProt, Display, TEXT("%s"), *Line);
			if (GEngine != nullptr)
			{
				GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Green, Line);
			}
		}
		else
		{
			const FString Line = FString::Printf(TEXT("%s FAILED: %s%s: %s (nothing changed, revision %llu)"), Command, LexToString(Status.Error),
				Status.FailedCommandIndex >= 0 ? *FString::Printf(TEXT(" at command %d"), Status.FailedCommandIndex) : TEXT(""), *Status.Detail,
				static_cast<unsigned long long>(Inventory.GetRevision()));
			UE_LOG(LogSupernaturalProt, Warning, TEXT("%s"), *Line);
			if (GEngine != nullptr)
			{
				GEngine->AddOnScreenDebugMessage(-1, 6.f, FColor::Red, Line);
			}
		}
	}

	void InvUsage(const TCHAR* Command, const TCHAR* Text)
	{
		UE_LOG(LogSupernaturalProt, Warning, TEXT("%s FAILED: usage: %s"), Command, Text);
	}

	/** Loads the test catalog if the inventory is inactive; returns false (and logs) if that fails. */
	bool InvEnsureCatalog(USpnInventorySubsystem& Inventory, const TCHAR* Command)
	{
		if (Inventory.HasCatalog())
		{
			return true;
		}
		const FSpnInventoryStatus Status = Inventory.LoadCatalogFromFile(USpnInventorySubsystem::GetTestCatalogPath());
		if (!Status.IsOk())
		{
			InvReport(Command, Status, Inventory);
			return false;
		}
		UE_LOG(LogSupernaturalProt, Display, TEXT("%s: loaded the TEMPORARY test catalog (%s)"), Command, *USpnInventorySubsystem::GetTestCatalogPath());
		return true;
	}

	void InvSetup(const TArray<FString>& Args, UWorld* World)
	{
		USpnInventorySubsystem* Inventory = FindInventory(World, TEXT("Spn.Inv.Setup"));
		if (Inventory == nullptr || !InvEnsureCatalog(*Inventory, TEXT("Spn.Inv.Setup")))
		{
			return;
		}
		// 1 = player body, 2 = car trunk, 3 = small box (test containers); existing ones are left alone
		const TPair<uint64, const TCHAR*> Containers[] = {{1, TEXT("spn_test:preset.body")}, {2, TEXT("spn_test:preset.trunk")}, {3, TEXT("spn_test:preset.small_box")}};
		for (const TPair<uint64, const TCHAR*>& Entry : Containers)
		{
			FSpnContainerView Existing;
			if (!Inventory->GetContainer(Entry.Key, Existing))
			{
				InvReport(*FString::Printf(TEXT("Spn.Inv.Setup container %llu"), static_cast<unsigned long long>(Entry.Key)), Inventory->CreateContainerFromPreset(Entry.Key, Entry.Value), *Inventory);
			}
		}
		UE_LOG(LogSupernaturalProt, Display, TEXT("Spn.Inv.Setup: %s"), *Inventory->Describe());
	}

	void InvCatalog(const TArray<FString>& Args, UWorld* World)
	{
		USpnInventorySubsystem* Inventory = FindInventory(World, TEXT("Spn.Inv.Catalog"));
		if (Inventory == nullptr || !InvEnsureCatalog(*Inventory, TEXT("Spn.Inv.Catalog")))
		{
			return;
		}
		const FSpnItemCatalogData& Data = Inventory->GetCatalogData();
		UE_LOG(LogSupernaturalProt, Display, TEXT("Spn.Inv.Catalog: %d item(s), %d preset(s)%s"), Data.Items.Num(), Data.ContainerPresets.Num(),
			Data.bIsTestData ? TEXT(" -- TEMPORARY TEST DATA") : TEXT(""));
		for (const FSpnItemDefinitionData& Item : Data.Items)
		{
			UE_LOG(LogSupernaturalProt, Display, TEXT("  item %s  category %s  max stack %d  %llu g  %llu ml  equip: %s"), *Item.Id, *Item.Category, Item.MaxStack,
				static_cast<unsigned long long>(Item.WeightGrams), static_cast<unsigned long long>(Item.VolumeMilliliters), *FString::Join(Item.EquipSlots, TEXT(",")));
		}
		for (const FSpnContainerPresetData& Preset : Data.ContainerPresets)
		{
			UE_LOG(LogSupernaturalProt, Display, TEXT("  preset %s  kind %s  stacks %lld  %lld g  %lld ml (negative = unlimited)  slots: %s"), *Preset.Id, *Preset.Rules.Kind,
				Preset.Rules.MaxStacks, Preset.Rules.MaxWeightGrams, Preset.Rules.MaxVolumeMilliliters, *FString::Join(Preset.Rules.EquipSlots, TEXT(",")));
		}
	}

	void InvList(const TArray<FString>& Args, UWorld* World)
	{
		if (USpnInventorySubsystem* Inventory = FindInventory(World, TEXT("Spn.Inv.List")))
		{
			UE_LOG(LogSupernaturalProt, Display, TEXT("Spn.Inv.List: %s"), *Inventory->Describe());
			if (GEngine != nullptr)
			{
				GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Cyan, Inventory->Describe());
			}
		}
	}

	void InvCreate(const TArray<FString>& Args, UWorld* World)
	{
		USpnInventorySubsystem* Inventory = FindInventory(World, TEXT("Spn.Inv.Create"));
		uint64 Container = 0;
		if (Inventory == nullptr)
		{
			return;
		}
		if (!InvParseNumber(Args, 0, Container) || !Args.IsValidIndex(1))
		{
			return InvUsage(TEXT("Spn.Inv.Create"), TEXT("Spn.Inv.Create <containerId> <preset>   e.g. Spn.Inv.Create 4 small_box"));
		}
		if (InvEnsureCatalog(*Inventory, TEXT("Spn.Inv.Create")))
		{
			InvReport(TEXT("Spn.Inv.Create"), Inventory->CreateContainerFromPreset(Container, InvExpand(Args[1], TEXT("spn_test:preset."))), *Inventory);
		}
	}

	void InvAdd(const TArray<FString>& Args, UWorld* World)
	{
		USpnInventorySubsystem* Inventory = FindInventory(World, TEXT("Spn.Inv.Add"));
		uint64 Container = 0;
		if (Inventory == nullptr)
		{
			return;
		}
		if (!InvParseNumber(Args, 0, Container) || !Args.IsValidIndex(1))
		{
			return InvUsage(TEXT("Spn.Inv.Add"), TEXT("Spn.Inv.Add <containerId> <item> [quantity]   e.g. Spn.Inv.Add 1 ration 3"));
		}
		if (InvEnsureCatalog(*Inventory, TEXT("Spn.Inv.Add")))
		{
			InvReport(TEXT("Spn.Inv.Add"), Inventory->AddItem(Container, InvExpand(Args[1], TEXT("spn_test:item.")), InvOptionalQuantity(Args, 2, 1)), *Inventory);
		}
	}

	void InvRemove(const TArray<FString>& Args, UWorld* World)
	{
		USpnInventorySubsystem* Inventory = FindInventory(World, TEXT("Spn.Inv.Remove"));
		uint64 Container = 0, Stack = 0;
		if (Inventory == nullptr)
		{
			return;
		}
		if (!InvParseNumber(Args, 0, Container) || !InvParseNumber(Args, 1, Stack))
		{
			return InvUsage(TEXT("Spn.Inv.Remove"), TEXT("Spn.Inv.Remove <containerId> <stackId> [quantity]"));
		}
		InvReport(TEXT("Spn.Inv.Remove"), Inventory->RemoveItem(Container, Stack, InvOptionalQuantity(Args, 2, 1)), *Inventory);
	}

	void InvTransfer(const TArray<FString>& Args, UWorld* World)
	{
		USpnInventorySubsystem* Inventory = FindInventory(World, TEXT("Spn.Inv.Transfer"));
		uint64 From = 0, To = 0, Stack = 0;
		if (Inventory == nullptr)
		{
			return;
		}
		if (!InvParseNumber(Args, 0, From) || !InvParseNumber(Args, 1, To) || !InvParseNumber(Args, 2, Stack))
		{
			return InvUsage(TEXT("Spn.Inv.Transfer"), TEXT("Spn.Inv.Transfer <fromContainer> <toContainer> <stackId> [quantity]"));
		}
		InvReport(TEXT("Spn.Inv.Transfer"), Inventory->TransferItem(From, To, Stack, InvOptionalQuantity(Args, 3, 1)), *Inventory);
	}

	void InvEquip(const TArray<FString>& Args, UWorld* World)
	{
		USpnInventorySubsystem* Inventory = FindInventory(World, TEXT("Spn.Inv.Equip"));
		uint64 Container = 0, Stack = 0;
		if (Inventory == nullptr)
		{
			return;
		}
		if (!InvParseNumber(Args, 0, Container) || !InvParseNumber(Args, 1, Stack) || !Args.IsValidIndex(2))
		{
			return InvUsage(TEXT("Spn.Inv.Equip"), TEXT("Spn.Inv.Equip <containerId> <stackId> <slot>   e.g. Spn.Inv.Equip 1 3 hand"));
		}
		InvReport(TEXT("Spn.Inv.Equip"), Inventory->Equip(Container, Stack, InvExpand(Args[2], TEXT("spn_test:slot."))), *Inventory);
	}

	void InvUnequip(const TArray<FString>& Args, UWorld* World)
	{
		USpnInventorySubsystem* Inventory = FindInventory(World, TEXT("Spn.Inv.Unequip"));
		uint64 Container = 0;
		if (Inventory == nullptr)
		{
			return;
		}
		if (!InvParseNumber(Args, 0, Container) || !Args.IsValidIndex(1))
		{
			return InvUsage(TEXT("Spn.Inv.Unequip"), TEXT("Spn.Inv.Unequip <containerId> <slot>"));
		}
		InvReport(TEXT("Spn.Inv.Unequip"), Inventory->Unequip(Container, InvExpand(Args[1], TEXT("spn_test:slot."))), *Inventory);
	}

	void InvLast(const TArray<FString>& Args, UWorld* World)
	{
		if (USpnInventorySubsystem* Inventory = FindInventory(World, TEXT("Spn.Inv.Last")))
		{
			const FSpnInventoryStatus& Failure = Inventory->GetLastFailure();
			if (Failure.IsOk())
			{
				UE_LOG(LogSupernaturalProt, Display, TEXT("Spn.Inv.Last: no failure so far"));
			}
			else
			{
				UE_LOG(LogSupernaturalProt, Display, TEXT("Spn.Inv.Last: %s%s: %s"), LexToString(Failure.Error),
					Failure.FailedCommandIndex >= 0 ? *FString::Printf(TEXT(" at command %d"), Failure.FailedCommandIndex) : TEXT(""), *Failure.Detail);
			}
		}
	}

	void InvReset(const TArray<FString>& Args, UWorld* World)
	{
		if (USpnInventorySubsystem* Inventory = FindInventory(World, TEXT("Spn.Inv.Reset")))
		{
			Inventory->ResetInventory();
			UE_LOG(LogSupernaturalProt, Display, TEXT("Spn.Inv.Reset OK: %s"), *Inventory->Describe());
		}
	}

	void InvView(const TArray<FString>& Args, UWorld* World)
	{
		USpnInventorySubsystem* Inventory = FindInventory(World, TEXT("Spn.Inv.View"));
		if (Inventory == nullptr)
		{
			return;
		}
		// a throw-away view-model bound to the containers 1 (body), 2 (trunk), 3 (small box): what a screen would show
		USpnInventoryViewModel* Vm = NewObject<USpnInventoryViewModel>(GetTransientPackage());
		Vm->AddToRoot();
		const UGameInstance* GameInstance = World->GetGameInstance();
		Vm->Initialize(Inventory, GameInstance != nullptr ? GameInstance->GetSubsystem<USpnSaveSubsystem>() : nullptr);
		Vm->SetBoundContainers({1, 2, 3});
		UE_LOG(LogSupernaturalProt, Display, TEXT("Spn.Inv.View: version %d, %d panel(s)"), Vm->GetVersion(), Vm->GetPanels().Num());
		for (const FSpnContainerPanel& Panel : Vm->GetPanels())
		{
			const auto Meter = [](const FSpnCapacityMeter& M) { return M.bUnlimited ? FString::Printf(TEXT("%lld (no limit)"), M.Used) : FString::Printf(TEXT("%lld / %lld (%.0f%%)"), M.Used, M.Max, M.Fraction * 100.f); };
			UE_LOG(LogSupernaturalProt, Display, TEXT("  panel %lld [%s]: %d row(s); stacks %s; weight %s g; volume %s ml"), Panel.ContainerId, *Panel.Kind, Panel.Rows.Num(), *Meter(Panel.StackMeter), *Meter(Panel.WeightMeter), *Meter(Panel.VolumeMeter));
			for (const FSpnEquipmentSlotView& Slot : Panel.Slots)
			{
				UE_LOG(LogSupernaturalProt, Display, TEXT("    slot %s: %s"), *Slot.SlotId, Slot.bOccupied ? *Slot.EquippedDefinitionId : TEXT("(free)"));
			}
			for (const FSpnItemRow& Row : Panel.Rows)
			{
				UE_LOG(LogSupernaturalProt, Display, TEXT("    row stack %lld: %d x %s  %lld g  %lld ml%s"), Row.StackId, Row.Quantity, *Row.DisplayName.ToString(), Row.TotalWeightGrams, Row.TotalVolumeMilliliters,
					Row.bEquipped ? *FString::Printf(TEXT("  [equipped: %s]"), *Row.EquippedSlot) : TEXT(""));
			}
		}
		if (Vm->Navigate(ESpnNavigationDirection::Down) || Vm->GetSelectedStackId() != 0)
		{
			UE_LOG(LogSupernaturalProt, Display, TEXT("  selected: container %lld stack %lld"), Vm->GetSelectedContainerId(), Vm->GetSelectedStackId());
			for (const FSpnActionOption& Option : Vm->GetAvailableActions())
			{
				UE_LOG(LogSupernaturalProt, Display, TEXT("    action %s: %s%s"), *Option.Label.ToString(), Option.bAvailable ? TEXT("available") : TEXT("NOT available: "), *Option.UnavailableText.ToString());
			}
		}
		Vm->Shutdown();
		Vm->RemoveFromRoot();
	}

	FAutoConsoleCommandWithWorldAndArgs InvViewCommand(TEXT("Spn.Inv.View"), TEXT("DEV: print what an inventory screen would show (view-model of containers 1, 2, 3, first selection and its actions)"), FConsoleCommandWithWorldAndArgsDelegate::CreateStatic(&InvView));
	FAutoConsoleCommandWithWorldAndArgs InvSetupCommand(TEXT("Spn.Inv.Setup"), TEXT("DEV: load the TEMPORARY test catalog and create test containers 1 (body), 2 (trunk), 3 (small box)"), FConsoleCommandWithWorldAndArgsDelegate::CreateStatic(&InvSetup));
	FAutoConsoleCommandWithWorldAndArgs InvCatalogCommand(TEXT("Spn.Inv.Catalog"), TEXT("DEV: list the item definitions and container presets of the catalog"), FConsoleCommandWithWorldAndArgsDelegate::CreateStatic(&InvCatalog));
	FAutoConsoleCommandWithWorldAndArgs InvListCommand(TEXT("Spn.Inv.List"), TEXT("DEV: print every container and stack"), FConsoleCommandWithWorldAndArgsDelegate::CreateStatic(&InvList));
	FAutoConsoleCommandWithWorldAndArgs InvCreateCommand(TEXT("Spn.Inv.Create"), TEXT("DEV: Spn.Inv.Create <containerId> <preset>"), FConsoleCommandWithWorldAndArgsDelegate::CreateStatic(&InvCreate));
	FAutoConsoleCommandWithWorldAndArgs InvAddCommand(TEXT("Spn.Inv.Add"), TEXT("DEV: Spn.Inv.Add <containerId> <item> [quantity]"), FConsoleCommandWithWorldAndArgsDelegate::CreateStatic(&InvAdd));
	FAutoConsoleCommandWithWorldAndArgs InvRemoveCommand(TEXT("Spn.Inv.Remove"), TEXT("DEV: Spn.Inv.Remove <containerId> <stackId> [quantity]"), FConsoleCommandWithWorldAndArgsDelegate::CreateStatic(&InvRemove));
	FAutoConsoleCommandWithWorldAndArgs InvTransferCommand(TEXT("Spn.Inv.Transfer"), TEXT("DEV: Spn.Inv.Transfer <fromContainer> <toContainer> <stackId> [quantity]"), FConsoleCommandWithWorldAndArgsDelegate::CreateStatic(&InvTransfer));
	FAutoConsoleCommandWithWorldAndArgs InvEquipCommand(TEXT("Spn.Inv.Equip"), TEXT("DEV: Spn.Inv.Equip <containerId> <stackId> <slot>"), FConsoleCommandWithWorldAndArgsDelegate::CreateStatic(&InvEquip));
	FAutoConsoleCommandWithWorldAndArgs InvUnequipCommand(TEXT("Spn.Inv.Unequip"), TEXT("DEV: Spn.Inv.Unequip <containerId> <slot>"), FConsoleCommandWithWorldAndArgsDelegate::CreateStatic(&InvUnequip));
	FAutoConsoleCommandWithWorldAndArgs InvLastCommand(TEXT("Spn.Inv.Last"), TEXT("DEV: show the most recent failed transaction"), FConsoleCommandWithWorldAndArgsDelegate::CreateStatic(&InvLast));
	FAutoConsoleCommandWithWorldAndArgs InvResetCommand(TEXT("Spn.Inv.Reset"), TEXT("DEV: empty the inventory (the catalog stays)"), FConsoleCommandWithWorldAndArgsDelegate::CreateStatic(&InvReset));
}

#endif
