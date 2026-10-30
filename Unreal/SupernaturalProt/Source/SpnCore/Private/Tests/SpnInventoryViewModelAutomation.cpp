#include "Engine/GameInstance.h"
#include "Engine/Texture2D.h"
#include "HAL/FileManager.h"
#include "HAL/PlatformTime.h"
#include "Misc/AutomationTest.h"
#include "Misc/FileHelper.h"
#include "Misc/Guid.h"
#include "Misc/Paths.h"
#include "SpnInventorySubsystem.h"
#include "SpnInventoryViewModel.h"
#include "SpnSaveSubsystem.h"
#include "SpnWorldStateSubsystem.h"
#include "UObject/Package.h"

#include <spn/core/SaveSlot.h>

#include <string>

#if WITH_DEV_AUTOMATION_TESTS

// Unreal checks for USpnInventoryViewModel, the catalog fingerprint and the save compatibility policies (M2B3.4). Data comes from the
// TEMPORARY test catalog (Content/SpnTest/Inventory/TestItemCatalog.json); every test uses its own game instance and a temporary save root.

namespace
{
	constexpr EAutomationTestFlags SpnVmFlags = EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter;

	const FString Ration = TEXT("spn_test:item.ration");
	const FString Bandage = TEXT("spn_test:item.bandage");
	const FString Flashlight = TEXT("spn_test:item.flashlight");
	const FString Notebook = TEXT("spn_test:item.notebook");
	const FString Coat = TEXT("spn_test:item.coat");
	const FString Crate = TEXT("spn_test:item.crate");
	const FString SlotHand = TEXT("spn_test:slot.hand");
	const FString SlotTorso = TEXT("spn_test:slot.torso");
	constexpr int64 Body = 1;
	constexpr int64 Trunk = 2;
	constexpr int64 Box = 3;
	const TCHAR* const SlotId = TEXT("slot_one");

	struct FScopedGameInstance
	{
		UGameInstance* GameInstance = nullptr;
		FScopedGameInstance()
		{
			GameInstance = NewObject<UGameInstance>(GetTransientPackage());
			GameInstance->AddToRoot();
			GameInstance->Init();
		}
		~FScopedGameInstance()
		{
			GameInstance->Shutdown();
			GameInstance->RemoveFromRoot();
		}
	};

	struct FVmEnv
	{
		FScopedGameInstance Instance;
		FString Root;
		USpnInventoryViewModel* Vm = nullptr;

		FVmEnv()
		{
			Root = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir() / TEXT("SpnTestTemp") / FGuid::NewGuid().ToString(EGuidFormats::Digits));
			Save().SetSaveDirectory(Root);
		}
		~FVmEnv()
		{
			if (Vm != nullptr)
			{
				Vm->Shutdown();
				Vm->RemoveFromRoot();
			}
			IFileManager::Get().DeleteDirectory(*Root, false, true);
		}

		USpnWorldStateSubsystem& World() const { return *Instance.GameInstance->GetSubsystem<USpnWorldStateSubsystem>(); }
		USpnSaveSubsystem& Save() const { return *Instance.GameInstance->GetSubsystem<USpnSaveSubsystem>(); }
		USpnInventorySubsystem& Inv() const { return *Instance.GameInstance->GetSubsystem<USpnInventorySubsystem>(); }
		FString File(const TCHAR* Id = SlotId) const { return Save().GetSlotFilePath(Id); }

		bool SetupCatalog()
		{
			return Inv().LoadCatalogFromFile(USpnInventorySubsystem::GetTestCatalogPath()).IsOk();
		}
		bool SetupContainers()
		{
			return Inv().CreateContainerFromPreset(Body, TEXT("spn_test:preset.body")).IsOk() && Inv().CreateContainerFromPreset(Trunk, TEXT("spn_test:preset.trunk")).IsOk()
				&& Inv().CreateContainerFromPreset(Box, TEXT("spn_test:preset.small_box")).IsOk();
		}
		bool Setup() { return SetupCatalog() && SetupContainers(); }

		USpnInventoryViewModel& MakeVm(const TArray<int64>& Bound)
		{
			Vm = NewObject<USpnInventoryViewModel>(GetTransientPackage());
			Vm->AddToRoot();
			Vm->Initialize(&Inv(), &Save());
			Vm->SetBoundContainers(Bound);
			return *Vm;
		}
	};

	void UseDoor(USpnWorldStateSubsystem& World, const TCHAR* Id)
	{
		World.TryInteract(USpnWorldStateSubsystem::TestDoorKind(), Id, ESpnInteractionVerb::Use, {{TEXT("open"), false}, {TEXT("locked"), false}}, TEXT("automation.player"));
	}

	bool ReadBytes(const FString& Path, TArray<uint8>& Out) { return FFileHelper::LoadFileToArray(Out, *Path, FILEREAD_Silent); }
	bool WriteBytes(const FString& Path, const TArray<uint8>& Bytes) { return FFileHelper::SaveArrayToFile(Bytes, *Path); }

	TArray<uint8> ToBytes(const std::string& Text)
	{
		TArray<uint8> Bytes;
		Bytes.Append(reinterpret_cast<const uint8*>(Text.data()), static_cast<int32>(Text.size()));
		return Bytes;
	}

	spn::core::SaveSlot ParsedSlot(const FString& Path)
	{
		TArray<uint8> Bytes;
		ReadBytes(Path, Bytes);
		return spn::core::SaveSlot::Parse(std::string(reinterpret_cast<const char*>(Bytes.GetData()), static_cast<std::size_t>(Bytes.Num()))).Value();
	}

	void WriteSlot(const FString& Path, const spn::core::SaveSlot& Slot) { WriteBytes(Path, ToBytes(Slot.Serialize())); }

	TArray<uint8> Fingerprint(FVmEnv& Env)
	{
		TArray<uint8> Bytes;
		if (Env.Save().SaveSlot(TEXT("probe")).IsOk())
		{
			ReadBytes(Env.File(TEXT("probe")), Bytes);
		}
		return Bytes;
	}

	bool Same(const TArray<uint8>& A, const TArray<uint8>& B) { return A.Num() > 0 && A == B; }

	/** The panel must be exactly what the Core says (Default sort, no filter). */
	bool MatchesCore(FAutomationTestBase& Test, FVmEnv& Env, USpnInventoryViewModel& Vm, int64 ContainerId, const TCHAR* Where)
	{
		FSpnContainerView View;
		FSpnContainerPanel Panel;
		const bool bCore = Env.Inv().GetContainer(static_cast<uint64>(ContainerId), View);
		const bool bPanel = Vm.GetPanel(ContainerId, Panel);
		if (!Test.TestEqual(FString::Printf(TEXT("%s: panel exists exactly when the container exists"), Where), bPanel, bCore) || !bCore)
		{
			return !bCore && !bPanel;
		}
		bool bOk = Test.TestEqual(FString::Printf(TEXT("%s: stack count"), Where), Panel.Rows.Num(), View.Stacks.Num());
		bOk &= Test.TestEqual(FString::Printf(TEXT("%s: weight meter"), Where), static_cast<uint64>(Panel.WeightMeter.Used), View.WeightGrams);
		bOk &= Test.TestEqual(FString::Printf(TEXT("%s: volume meter"), Where), static_cast<uint64>(Panel.VolumeMeter.Used), View.VolumeMilliliters);
		bOk &= Test.TestEqual(FString::Printf(TEXT("%s: stack meter"), Where), Panel.StackMeter.Used, static_cast<int64>(View.Stacks.Num()));
		bOk &= Test.TestEqual(FString::Printf(TEXT("%s: weight limit"), Where), Panel.WeightMeter.Max, View.Rules.MaxWeightGrams);
		bOk &= Test.TestEqual(FString::Printf(TEXT("%s: slot count"), Where), Panel.Slots.Num(), View.Rules.EquipSlots.Num());
		for (int32 Index = 0; Index < FMath::Min(Panel.Rows.Num(), View.Stacks.Num()); ++Index)
		{
			const FSpnItemRow& Row = Panel.Rows[Index];
			const FSpnStackView& Stack = View.Stacks[Index];
			FSpnItemDefinitionData Definition;
			Env.Inv().FindDefinition(Stack.Definition, Definition);
			bOk &= Row.StackId == static_cast<int64>(Stack.StackId) && Row.DefinitionId == Stack.Definition && static_cast<uint32>(Row.Quantity) == Stack.Quantity
				&& Row.EquippedSlot == Stack.EquippedSlot && Row.bEquipped == !Stack.EquippedSlot.IsEmpty() && Row.State.Num() == Stack.State.Num()
				&& static_cast<uint64>(Row.TotalWeightGrams) == Definition.WeightGrams * Stack.Quantity && static_cast<uint64>(Row.TotalVolumeMilliliters) == Definition.VolumeMilliliters * Stack.Quantity
				&& Row.MaxStack == Definition.MaxStack && Row.Category == Definition.Category;
		}
		Test.TestTrue(FString::Printf(TEXT("%s: every row equals the Core's stack"), Where), bOk);
		return bOk;
	}

	FSpnInventoryCommand AddCmd(int64 Container, const FString& Definition, uint32 Quantity = 1) { return FSpnInventoryCommand::AddItem(static_cast<uint64>(Container), Definition, Quantity); }
	uint64 AddUnique(FVmEnv& Env, int64 Container, const FString& Definition)
	{
		const FSpnInventoryStatus Status = Env.Inv().AddItem(static_cast<uint64>(Container), Definition);
		return Status.IsOk() ? Status.CreatedStacks[0] : 0;
	}
}

// === view-model: content ======================================================================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnVmPopulateTest, "SpnCore.InventoryViewModel.ReflectsTheRealInventory", SpnVmFlags)
bool FSpnVmPopulateTest::RunTest(const FString& Parameters)
{
	FVmEnv Env;
	// no catalog: nothing to show, nothing invented
	USpnInventoryViewModel& Vm = Env.MakeVm({Body, Box});
	TestFalse(TEXT("inactive without a catalog"), Vm.IsInventoryActive());
	TestEqual(TEXT("no panels"), Vm.GetPanels().Num(), 0);

	if (!TestTrue(TEXT("setup"), Env.Setup()))
	{
		return false;
	}
	TestTrue(TEXT("active now"), Vm.IsInventoryActive());
	TestEqual(TEXT("empty containers give empty panels, not placeholder rows"), Vm.GetPanels().Num(), 2);
	for (const FSpnContainerPanel& Panel : Vm.GetPanels())
	{
		TestEqual(TEXT("no rows in an empty container"), Panel.Rows.Num(), 0);
		TestEqual(TEXT("and no stack"), Panel.TotalStackCount, 0);
	}

	Env.Inv().AddItem(Body, Ration, 7);
	const uint64 Light = AddUnique(Env, Body, Flashlight);
	Env.Inv().Equip(Body, Light, SlotHand);
	Env.Inv().AddItem(Box, Bandage, 3);
	MatchesCore(*this, Env, Vm, Body, TEXT("body"));
	MatchesCore(*this, Env, Vm, Box, TEXT("box"));

	FSpnContainerPanel Panel;
	Vm.GetPanel(Body, Panel);
	TestEqual(TEXT("three stacks: 5 + 2 rations and a flashlight"), Panel.Rows.Num(), 3);
	TestEqual(TEXT("title is the container kind id, not an invented name"), Panel.Title.ToString(), FString(TEXT("spn_test:container.body")));
	TestEqual(TEXT("stack meter 3 of 8"), Panel.StackMeter.Max, int64{8});
	TestTrue(TEXT("stack fraction 3/8"), FMath::IsNearlyEqual(Panel.StackMeter.Fraction, 3.f / 8.f));
	TestEqual(TEXT("weight 7 x 200 + 400"), Panel.WeightMeter.Used, int64{1800});
	TestEqual(TEXT("volume 7 x 300 + 250"), Panel.VolumeMeter.Used, int64{2350});
	TestFalse(TEXT("limits exist"), Panel.WeightMeter.bUnlimited);
	TestFalse(TEXT("not full"), Panel.WeightMeter.bFull);
	const FSpnItemRow& First = Panel.Rows[0];
	TestTrue(TEXT("ration row: quantity 5, unit 200 g / 300 ml, total 1000 g / 1500 ml, stackable"), First.Quantity == 5 && First.UnitWeightGrams == 200 && First.UnitVolumeMilliliters == 300 && First.TotalWeightGrams == 1000 && First.TotalVolumeMilliliters == 1500 && First.bStackable && First.MaxStack == 5);
	TestFalse(TEXT("without a presentation set the name is the id and the row says so"), First.bHasPresentation);
	TestEqual(TEXT("display name falls back to the id"), First.DisplayName.ToString(), Ration);
	const FSpnItemRow& Lamp = Panel.Rows[2];
	TestTrue(TEXT("flashlight: equippable, equipped in the hand"), Lamp.bEquippable && Lamp.bEquipped && Lamp.EquippedSlot == SlotHand && !Lamp.bStackable);
	TestEqual(TEXT("slots are the container's: hand and torso"), Panel.Slots.Num(), 2);
	bool bHandOccupied = false, bTorsoFree = false;
	for (const FSpnEquipmentSlotView& Slot : Panel.Slots)
	{
		bHandOccupied |= Slot.SlotId == SlotHand && Slot.bOccupied && Slot.EquippedStackId == static_cast<int64>(Light) && Slot.EquippedDefinitionId == Flashlight;
		bTorsoFree |= Slot.SlotId == SlotTorso && !Slot.bOccupied && Slot.EquippedStackId == 0;
	}
	TestTrue(TEXT("hand shows the flashlight"), bHandOccupied);
	TestTrue(TEXT("torso is free"), bTorsoFree);

	// an unbound container has no panel
	FSpnContainerPanel None;
	TestFalse(TEXT("the trunk is not bound"), Vm.GetPanel(Trunk, None));
	Env.Inv().ResetInventory();
	TestEqual(TEXT("reset: the containers are gone, so are the panels"), Vm.GetPanels().Num(), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnVmPresentationTest, "SpnCore.InventoryViewModel.PresentationIsOptionalAndSeparate", SpnVmFlags)
bool FSpnVmPresentationTest::RunTest(const FString& Parameters)
{
	FVmEnv Env;
	if (!TestTrue(TEXT("setup"), Env.Setup()))
	{
		return false;
	}
	USpnInventoryViewModel& Vm = Env.MakeVm({Body});
	Env.Inv().AddItem(Body, Ration, 1);
	Env.Inv().AddItem(Body, Bandage, 1);
	const FString FingerprintBefore = Env.Inv().GetCatalogFingerprint();
	TestFalse(TEXT("there is a fingerprint"), FingerprintBefore.IsEmpty());

	USpnItemPresentationSet* Set = NewObject<USpnItemPresentationSet>(GetTransientPackage());
	Set->AddToRoot();
	FSpnItemPresentationEntry Entry;
	Entry.ItemId = Ration;
	Entry.DisplayName = FText::FromString(TEXT("Test ration"));
	Entry.Description = FText::FromString(TEXT("Test description"));
	Entry.SortGroup = TEXT("food");
	Entry.Icon = TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/Spn/Presentation/T_NotCreated.T_NotCreated")));
	Set->Entries.Add(Entry);
	FSpnItemPresentationEntry Orphan;
	Orphan.ItemId = TEXT("spn_test:item.removed");
	Set->Entries.Add(Orphan);
	Set->Entries.Add(Entry); // duplicate

	const int32 VersionBefore = Vm.GetVersion();
	Env.Inv().SetPresentationSet(Set);
	TestTrue(TEXT("installing a set refreshes the view"), Vm.GetVersion() > VersionBefore);
	FSpnContainerPanel Panel;
	Vm.GetPanel(Body, Panel);
	TestEqual(TEXT("the entry names the ration"), Panel.Rows[0].DisplayName.ToString(), FString(TEXT("Test ration")));
	TestTrue(TEXT("row carries the description, sort group and the soft icon reference (nothing loaded)"), Panel.Rows[0].bHasPresentation && Panel.Rows[0].Description.ToString() == TEXT("Test description") && Panel.Rows[0].SortGroup == FName(TEXT("food")) && !Panel.Rows[0].Icon.IsNull() && !Panel.Rows[0].Icon.IsValid());
	TestFalse(TEXT("the bandage has no entry: fallback"), Panel.Rows[1].bHasPresentation);
	TestEqual(TEXT("and shows its id"), Panel.Rows[1].DisplayName.ToString(), Bandage);

	const FSpnPresentationReport Report = Env.Inv().ValidatePresentation();
	TestTrue(TEXT("the orphan is reported"), Report.OrphanedEntries.Contains(TEXT("spn_test:item.removed")));
	TestTrue(TEXT("the duplicate is reported"), Report.DuplicateEntries.Contains(Ration));
	TestTrue(TEXT("items without presentation are listed (fallbacks apply)"), Report.ItemsWithoutPresentation.Contains(Bandage) && !Report.ItemsWithoutPresentation.Contains(Ration));
	TestFalse(TEXT("so the set is not clean"), Report.IsClean());

	// single source of truth: the presentation cannot change gameplay values or the fingerprint
	TestEqual(TEXT("the catalog fingerprint ignores the presentation"), Env.Inv().GetCatalogFingerprint(), FingerprintBefore);
	TestEqual(TEXT("the weight is still the catalog's"), Panel.Rows[0].UnitWeightGrams, int64{200});
	TestTrue(TEXT("a save made with a presentation set loads fine without one"), Env.Save().SaveSlot(SlotId).IsOk());
	Env.Inv().SetPresentationSet(nullptr);
	TestTrue(TEXT("removing the set"), Env.Save().LoadSlot(SlotId).IsOk());
	Set->RemoveFromRoot();
	return true;
}

// === view-model: refresh ======================================================================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnVmRefreshTest, "SpnCore.InventoryViewModel.MutationsUpdateThePanels", SpnVmFlags)
bool FSpnVmRefreshTest::RunTest(const FString& Parameters)
{
	FVmEnv Env;
	if (!TestTrue(TEXT("setup"), Env.Setup()))
	{
		return false;
	}
	USpnInventoryViewModel& Vm = Env.MakeVm({Body, Box});
	int32 Changed = 0;
	FSimpleMulticastDelegate Dummy;
	USpnInventorySubsystem& Inv = Env.Inv();

	const auto Step = [&](const TCHAR* What, auto&& Action)
	{
		const int32 Version = Vm.GetVersion();
		Action();
		TestTrue(FString::Printf(TEXT("%s: the version advanced"), What), Vm.GetVersion() > Version);
		MatchesCore(*this, Env, Vm, Body, What);
		MatchesCore(*this, Env, Vm, Box, What);
		TestEqual(FString::Printf(TEXT("%s: the Core is consistent"), What), Inv.CheckInvariants(), FString());
	};
	Step(TEXT("add"), [&]() { Inv.AddItem(Body, Ration, 7); });
	Step(TEXT("add unique"), [&]() { Inv.AddItem(Body, Flashlight); });
	FSpnContainerPanel Panel;
	Vm.GetPanel(Body, Panel);
	const int64 Ration1 = Panel.Rows[0].StackId, Ration2 = Panel.Rows[1].StackId, Light = Panel.Rows[2].StackId;
	Step(TEXT("split"), [&]() { Inv.SplitStack(Body, Ration1, 2); });
	Step(TEXT("merge"), [&]() { Inv.MergeStacks(Body, Ration2, Ration1); });
	Step(TEXT("equip"), [&]() { Inv.Equip(Body, Light, SlotHand); });
	Step(TEXT("unequip"), [&]() { Inv.Unequip(Body, SlotHand); });
	Step(TEXT("transfer"), [&]() { Inv.TransferItem(Body, Box, Ration1, 2); });
	Step(TEXT("remove"), [&]() { Inv.RemoveItem(Body, Light); });
	Step(TEXT("atomic batch"), [&]() { Inv.Apply({AddCmd(Body, Bandage, 3), AddCmd(Box, Bandage, 1)}); });
	(void)Changed;
	(void)Dummy;

	// a failed transaction changes nothing: no rebuild, no version change, and a message that names the problem
	const int32 Version = Vm.GetVersion();
	const int32 Rebuilds = Vm.GetRebuildCount();
	TestFalse(TEXT("the failing transaction fails"), Inv.AddItem(Box, Crate).IsOk());
	TestEqual(TEXT("no refresh for a failed transaction"), Vm.GetVersion(), Version);
	TestEqual(TEXT("no panel was rebuilt"), Vm.GetRebuildCount(), Rebuilds);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnVmIncrementalTest, "SpnCore.InventoryViewModel.OnlyTouchedBoundContainersAreRebuilt", SpnVmFlags)
bool FSpnVmIncrementalTest::RunTest(const FString& Parameters)
{
	FVmEnv Env;
	if (!TestTrue(TEXT("setup"), Env.Setup()))
	{
		return false;
	}
	USpnInventoryViewModel& Vm = Env.MakeVm({Body, Box});
	int32 Changed = 0;
	TArray<int64> Rebuilt;
	Vm.OnContainerChangedNative.AddLambda([&](int64 Id) { Rebuilt.Add(Id); });
	(void)Changed;
	Vm.OnChangedNative.AddLambda([&Changed]() { ++Changed; });

	int32 RebuildsBefore = Vm.GetRebuildCount();
	int32 VersionBefore = Vm.GetVersion();
	Env.Inv().AddItem(Trunk, Crate); // a container this screen does not show
	TestEqual(TEXT("an unbound container costs nothing: no rebuild"), Vm.GetRebuildCount(), RebuildsBefore);
	TestEqual(TEXT("no version change"), Vm.GetVersion(), VersionBefore);
	TestEqual(TEXT("no notification"), Changed, 0);

	Env.Inv().AddItem(Box, Bandage);
	TestEqual(TEXT("one bound container touched: one rebuild"), Vm.GetRebuildCount(), RebuildsBefore + 1);
	TestTrue(TEXT("and it is the box"), Rebuilt.Num() == 1 && Rebuilt[0] == Box);
	TestEqual(TEXT("one OnChanged"), Changed, 1);

	Rebuilt.Reset();
	RebuildsBefore = Vm.GetRebuildCount();
	const uint64 Stack = AddUnique(Env, Body, Notebook);
	TestEqual(TEXT("a transfer between a bound and an unbound container rebuilds only the bound one"), Vm.GetRebuildCount(), RebuildsBefore + 1);
	RebuildsBefore = Vm.GetRebuildCount();
	Env.Inv().TransferItem(Body, Trunk, Stack);
	TestEqual(TEXT("body touched, trunk not shown"), Vm.GetRebuildCount(), RebuildsBefore + 1);
	RebuildsBefore = Vm.GetRebuildCount();
	Env.Inv().Apply({FSpnInventoryCommand::TransferItem(Trunk, Box, Stack)});
	TestEqual(TEXT("trunk -> box rebuilds the box only"), Vm.GetRebuildCount(), RebuildsBefore + 1);

	RebuildsBefore = Vm.GetRebuildCount();
	Env.Inv().ResetInventory();
	VersionBefore = Vm.GetVersion();
	Env.Inv().ResetInventory();
	TestEqual(TEXT("(a second reset of an empty inventory still refreshes: the whole inventory was replaced)"), Vm.GetVersion(), VersionBefore + 1);
	TestEqual(TEXT("the panels of the vanished containers are gone"), Vm.GetPanels().Num(), 0);
	return true;
}

// === view-model: selection, actions, navigation =================================================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnVmSelectionTest, "SpnCore.InventoryViewModel.SelectionSurvivesRefreshes", SpnVmFlags)
bool FSpnVmSelectionTest::RunTest(const FString& Parameters)
{
	FVmEnv Env;
	if (!TestTrue(TEXT("setup"), Env.Setup()))
	{
		return false;
	}
	USpnInventoryViewModel& Vm = Env.MakeVm({Body, Box});
	const uint64 A = AddUnique(Env, Body, Notebook);
	const uint64 B = AddUnique(Env, Body, Flashlight);
	const uint64 C = AddUnique(Env, Body, Coat);
	int32 SelectionEvents = 0;
	Vm.OnSelectionChangedNative.AddLambda([&SelectionEvents]() { ++SelectionEvents; });

	TestFalse(TEXT("an unknown stack cannot be selected"), Vm.SelectStack(Body, 999));
	TestFalse(TEXT("a stack of another container cannot be selected there"), Vm.SelectStack(Box, static_cast<int64>(A)));
	TestTrue(TEXT("select the flashlight"), Vm.SelectStack(Body, static_cast<int64>(B)));
	FSpnItemRow Row;
	TestTrue(TEXT("the selected row"), Vm.GetSelectedRow(Row) && Row.DefinitionId == Flashlight);
	TestEqual(TEXT("one selection event"), SelectionEvents, 1);

	Env.Inv().AddItem(Body, Bandage); // an unrelated change keeps the selection
	TestEqual(TEXT("selection kept"), Vm.GetSelectedStackId(), static_cast<int64>(B));

	Env.Inv().TransferItem(Body, Box, B); // moves to the other bound container: the selection follows
	TestEqual(TEXT("the selection follows the stack"), Vm.GetSelectedContainerId(), Box);
	TestEqual(TEXT("still the same stack"), Vm.GetSelectedStackId(), static_cast<int64>(B));

	Env.Inv().RemoveItem(Box, B); // gone: nothing left in the box -> cleared
	TestEqual(TEXT("a removed last row clears the selection"), Vm.GetSelectedStackId(), int64{0});

	TestTrue(TEXT("select the notebook (first row of the body)"), Vm.SelectStack(Body, static_cast<int64>(A)));
	Env.Inv().RemoveItem(Body, A);
	TestEqual(TEXT("a removed row moves the selection to the neighbour that took its place"), Vm.GetSelectedStackId(), static_cast<int64>(C));
	TestTrue(TEXT("remove the rest"), Env.Inv().RemoveItem(Body, C).IsOk());
	TestEqual(TEXT("(bandage remains)"), Vm.GetPanels()[0].Rows.Num(), 1);

	TestTrue(TEXT("select it"), Vm.SelectStack(Body, Vm.GetPanels()[0].Rows[0].StackId));
	Env.Inv().ResetInventory();
	TestEqual(TEXT("a reset clears the selection"), Vm.GetSelectedStackId(), int64{0});
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnVmActionsTest, "SpnCore.InventoryViewModel.ActionsComeFromTheCoreNotFromTheUi", SpnVmFlags)
bool FSpnVmActionsTest::RunTest(const FString& Parameters)
{
	FVmEnv Env;
	if (!TestTrue(TEXT("setup"), Env.Setup()))
	{
		return false;
	}
	USpnInventoryViewModel& Vm = Env.MakeVm({Body, Box});
	USpnInventorySubsystem& Inv = Env.Inv();
	const uint64 Light = AddUnique(Env, Body, Flashlight);
	const uint64 Light2 = AddUnique(Env, Body, Flashlight);
	AddUnique(Env, Body, Notebook);
	Inv.AddItem(Body, Ration, 4);
	TestTrue(TEXT("select the first flashlight"), Vm.SelectStack(Body, static_cast<int64>(Light)));

	const uint64 Revision = Inv.GetRevision();
	const FSpnInventoryStatus LastFailureBefore = Inv.GetLastFailure();
	TArray<FSpnActionOption> Options = Vm.GetAvailableActions();
	TestEqual(TEXT("probing changes nothing (revision)"), Inv.GetRevision(), Revision);
	TestEqual(TEXT("and does not touch the failure memory"), Inv.GetLastFailure().Detail, LastFailureBefore.Detail);
	const auto Find = [&Options](ESpnInventoryAction Action, const FString& Slot = FString()) -> const FSpnActionOption*
	{
		return Options.FindByPredicate([&](const FSpnActionOption& Option) { return Option.Action == Action && (Slot.IsEmpty() || Option.Slot == Slot); });
	};
	const FSpnActionOption* EquipHand = Find(ESpnInventoryAction::Equip, SlotHand);
	TestTrue(TEXT("equip into the hand is offered and available"), EquipHand != nullptr && EquipHand->bAvailable);
	TestTrue(TEXT("the torso is never offered for a flashlight (the definition has no such slot)"), Find(ESpnInventoryAction::Equip, SlotTorso) == nullptr);
	const FSpnActionOption* ToBox = Find(ESpnInventoryAction::Transfer);
	TestTrue(TEXT("a transfer to the small box is offered and available"), ToBox != nullptr && ToBox->bAvailable && ToBox->TargetContainerId == Box);
	TestTrue(TEXT("discard is offered"), Find(ESpnInventoryAction::Discard) != nullptr && Find(ESpnInventoryAction::Discard)->bAvailable);
	TestTrue(TEXT("a unique item cannot be split or merged"), Find(ESpnInventoryAction::Split) == nullptr && Find(ESpnInventoryAction::Merge) == nullptr);

	// execute: the Core changes, the panels follow
	FSpnActionRequest Equip;
	Equip.Action = ESpnInventoryAction::Equip;
	Equip.ContainerId = Body;
	Equip.StackId = static_cast<int64>(Light);
	Equip.Slot = SlotHand;
	TestTrue(TEXT("equip"), Vm.ExecuteAction(Equip));
	TestEqual(TEXT("the message says what happened"), Vm.GetLastMessage().Key, FString(TEXT("spn.inventory.action.equip")));
	FSpnItemRow Row;
	TestTrue(TEXT("the selected row is now equipped"), Vm.GetSelectedRow(Row) && Row.bEquipped && Row.EquippedSlot == SlotHand);

	Options = Vm.GetAvailableActions();
	TestTrue(TEXT("now unequip is offered"), Find(ESpnInventoryAction::Unequip) != nullptr && Find(ESpnInventoryAction::Unequip)->bAvailable);
	TestTrue(TEXT("and an equipped item cannot be moved or discarded"), Find(ESpnInventoryAction::Transfer) == nullptr && Find(ESpnInventoryAction::Discard) == nullptr);

	// the second flashlight: the hand is taken - offered, but unavailable, with the Core's reason and a ready message
	TestTrue(TEXT("select the second flashlight"), Vm.SelectStack(Body, static_cast<int64>(Light2)));
	Options = Vm.GetAvailableActions();
	EquipHand = Find(ESpnInventoryAction::Equip, SlotHand);
	TestTrue(TEXT("equip is offered"), EquipHand != nullptr);
	if (EquipHand != nullptr)
	{
		TestFalse(TEXT("but not available"), EquipHand->bAvailable);
		TestEqual(TEXT("because the slot is occupied"), EquipHand->UnavailableReason, ESpnInventoryError::SlotOccupied);
		TestFalse(TEXT("with a message"), EquipHand->UnavailableText.IsEmpty());
	}
	TestFalse(TEXT("ExecuteDefaultAction cannot equip it either"), Vm.ExecuteDefaultAction());
	TestEqual(TEXT("and says why"), Vm.GetLastMessage().Error, ESpnInventoryError::SlotOccupied);

	// transfers: the box holds consumables and tools, 3 stacks
	TestTrue(TEXT("select the rations"), Vm.SelectStack(Body, Vm.GetPanels()[0].Rows.Last().StackId));
	Options = Vm.GetAvailableActions();
	TestTrue(TEXT("rations can be split and moved"), Find(ESpnInventoryAction::Split) != nullptr && Find(ESpnInventoryAction::Transfer) != nullptr);
	FSpnActionRequest Split;
	Split.Action = ESpnInventoryAction::Split;
	Split.ContainerId = Body;
	Split.StackId = Vm.GetSelectedStackId();
	Split.Quantity = 2;
	TestTrue(TEXT("split 2 of 4"), Vm.ExecuteAction(Split));
	TestEqual(TEXT("the split-off stack exists"), Env.Inv().CountOf(Body, Ration), uint64{4});
	Options = Vm.GetAvailableActions();
	TestTrue(TEXT("merge is now offered"), Find(ESpnInventoryAction::Merge) != nullptr && Find(ESpnInventoryAction::Merge)->bAvailable);

	// a request the Core refuses is reported, not hidden
	FSpnActionRequest Bad;
	Bad.Action = ESpnInventoryAction::Transfer;
	Bad.ContainerId = Body;
	Bad.StackId = Vm.GetSelectedStackId();
	Bad.TargetContainerId = 99;
	Bad.Quantity = 1;
	TestFalse(TEXT("transfer to a missing container"), Vm.ExecuteAction(Bad));
	TestEqual(TEXT("message with the Core's error"), Vm.GetLastMessage().Error, ESpnInventoryError::UnknownContainer);
	TestEqual(TEXT("severity error"), Vm.GetLastMessage().Severity, ESpnMessageSeverity::Error);
	TestFalse(TEXT("a request without an action is refused"), Vm.ExecuteAction(FSpnActionRequest()));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnVmDefaultActionTest, "SpnCore.InventoryViewModel.ConfirmEquipsAndUnequips", SpnVmFlags)
bool FSpnVmDefaultActionTest::RunTest(const FString& Parameters)
{
	FVmEnv Env;
	if (!TestTrue(TEXT("setup"), Env.Setup()))
	{
		return false;
	}
	USpnInventoryViewModel& Vm = Env.MakeVm({Body});
	const uint64 Coat1 = AddUnique(Env, Body, Coat);
	const uint64 Book = AddUnique(Env, Body, Notebook);
	TestTrue(TEXT("select the coat"), Vm.SelectStack(Body, static_cast<int64>(Coat1)));
	TestTrue(TEXT("confirm equips it"), Vm.ExecuteDefaultAction());
	FSpnItemRow Row;
	TestTrue(TEXT("equipped to the torso"), Vm.GetSelectedRow(Row) && Row.EquippedSlot == SlotTorso);
	TestTrue(TEXT("confirm again takes it off"), Vm.ExecuteDefaultAction());
	TestTrue(TEXT("unequipped"), Vm.GetSelectedRow(Row) && !Row.bEquipped);
	TestTrue(TEXT("select the notebook"), Vm.SelectStack(Body, static_cast<int64>(Book)));
	TestFalse(TEXT("nothing to equip or unequip"), Vm.ExecuteDefaultAction());
	TestEqual(TEXT("an informational message says so"), Vm.GetLastMessage().Key, FString(TEXT("spn.inventory.no_default_action")));
	Vm.ClearSelection();
	TestFalse(TEXT("without a selection nothing happens"), Vm.ExecuteDefaultAction());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnVmNavigationTest, "SpnCore.InventoryViewModel.NavigationForKeyboardMouseAndController", SpnVmFlags)
bool FSpnVmNavigationTest::RunTest(const FString& Parameters)
{
	FVmEnv Env;
	if (!TestTrue(TEXT("setup"), Env.Setup()))
	{
		return false;
	}
	USpnInventoryViewModel& Vm = Env.MakeVm({Body, Trunk});
	// body: 7 unique items in id order (notebook x4, flashlight, coat, ... within 8 stacks); trunk: 2 crates
	TArray<uint64> Ids;
	for (int32 I = 0; I < 5; ++I) { Ids.Add(AddUnique(Env, Body, Notebook)); }
	Ids.Add(AddUnique(Env, Body, Flashlight));
	Ids.Add(AddUnique(Env, Body, Coat));
	const uint64 Crate1 = AddUnique(Env, Trunk, Crate);
	const uint64 Crate2 = AddUnique(Env, Trunk, Crate);
	const auto Sel = [&Vm]() { return Vm.GetSelectedStackId(); };

	// list mode (1 column)
	TestTrue(TEXT("nothing selected: any step selects the first row"), Vm.Navigate(ESpnNavigationDirection::Up));
	TestEqual(TEXT("first row of the primary container"), Sel(), static_cast<int64>(Ids[0]));
	TestFalse(TEXT("Up at the top stays"), Vm.Navigate(ESpnNavigationDirection::Up));
	TestTrue(TEXT("Down"), Vm.Navigate(ESpnNavigationDirection::Down));
	TestEqual(TEXT("second row"), Sel(), static_cast<int64>(Ids[1]));
	TestTrue(TEXT("Next"), Vm.Navigate(ESpnNavigationDirection::Next));
	TestTrue(TEXT("Previous"), Vm.Navigate(ESpnNavigationDirection::Previous));
	TestEqual(TEXT("back on the second row"), Sel(), static_cast<int64>(Ids[1]));
	TestTrue(TEXT("Last"), Vm.Navigate(ESpnNavigationDirection::Last));
	TestEqual(TEXT("last row of the body"), Sel(), static_cast<int64>(Ids[6]));
	TestFalse(TEXT("Down at the bottom stays without wrapping"), Vm.Navigate(ESpnNavigationDirection::Down));
	Vm.SetWrapNavigation(true);
	TestTrue(TEXT("with wrapping Down goes to the top"), Vm.Navigate(ESpnNavigationDirection::Down));
	TestEqual(TEXT("first row"), Sel(), static_cast<int64>(Ids[0]));
	Vm.SetWrapNavigation(false);
	TestFalse(TEXT("First while on the first row stays"), Vm.Navigate(ESpnNavigationDirection::First));
	TestTrue(TEXT("PageDown moves five rows"), Vm.Navigate(ESpnNavigationDirection::PageDown));
	TestEqual(TEXT("row 5"), Sel(), static_cast<int64>(Ids[5]));
	TestTrue(TEXT("PageDown clamps to the last row"), Vm.Navigate(ESpnNavigationDirection::PageDown));
	TestEqual(TEXT("last row"), Sel(), static_cast<int64>(Ids[6]));
	TestTrue(TEXT("PageUp moves five rows back"), Vm.Navigate(ESpnNavigationDirection::PageUp));
	TestEqual(TEXT("row 1"), Sel(), static_cast<int64>(Ids[1]));
	TestTrue(TEXT("PageUp clamps to the first row"), Vm.Navigate(ESpnNavigationDirection::PageUp));
	TestEqual(TEXT("first row"), Sel(), static_cast<int64>(Ids[0]));

	// sections: Right (list mode) and NextSection switch to the other container
	TestTrue(TEXT("Right in list mode changes the section"), Vm.Navigate(ESpnNavigationDirection::Right));
	TestEqual(TEXT("now in the trunk"), Vm.GetSelectedContainerId(), Trunk);
	TestEqual(TEXT("on its first row"), Sel(), static_cast<int64>(Crate1));
	TestTrue(TEXT("Down inside the trunk"), Vm.Navigate(ESpnNavigationDirection::Down));
	TestEqual(TEXT("second crate"), Sel(), static_cast<int64>(Crate2));
	TestTrue(TEXT("PreviousSection returns to the body, keeping the row index"), Vm.Navigate(ESpnNavigationDirection::PreviousSection));
	TestEqual(TEXT("body, row 1"), Vm.GetSelectedContainerId(), Body);
	TestEqual(TEXT("same index"), Sel(), static_cast<int64>(Ids[1]));
	TestTrue(TEXT("NextSection wraps around between two sections"), Vm.Navigate(ESpnNavigationDirection::NextSection) && Vm.Navigate(ESpnNavigationDirection::NextSection));
	TestEqual(TEXT("and is back in the body"), Vm.GetSelectedContainerId(), Body);

	// grid mode: 3 columns -> rows [0 1 2] [3 4 5] [6]
	Vm.SetGridColumns(3);
	Vm.SelectStack(Body, static_cast<int64>(Ids[0]));
	TestTrue(TEXT("Right in a grid moves one cell"), Vm.Navigate(ESpnNavigationDirection::Right));
	TestEqual(TEXT("cell 1"), Sel(), static_cast<int64>(Ids[1]));
	TestTrue(TEXT("Down moves a row"), Vm.Navigate(ESpnNavigationDirection::Down));
	TestEqual(TEXT("cell 4"), Sel(), static_cast<int64>(Ids[4]));
	TestTrue(TEXT("Down into the short last row lands on its last cell"), Vm.Navigate(ESpnNavigationDirection::Down));
	TestEqual(TEXT("cell 6"), Sel(), static_cast<int64>(Ids[6]));
	TestFalse(TEXT("Right at the end of the last row stays"), Vm.Navigate(ESpnNavigationDirection::Right));
	TestTrue(TEXT("Up"), Vm.Navigate(ESpnNavigationDirection::Up));
	TestEqual(TEXT("cell 3"), Sel(), static_cast<int64>(Ids[3]));
	TestFalse(TEXT("Left at the start of a row stays"), Vm.Navigate(ESpnNavigationDirection::Left));

	// equipment zone: the slots of the primary container, hand and torso
	Env.Inv().Equip(Body, Ids[5], SlotHand);
	Vm.SetNavigationZone(ESpnNavigationZone::Equipment);
	TestEqual(TEXT("the zone"), Vm.GetNavigationZone(), ESpnNavigationZone::Equipment);
	TestEqual(TEXT("focus starts on the first slot"), Vm.GetFocusedSlot(), SlotHand);
	TestTrue(TEXT("Down moves to the next slot"), Vm.Navigate(ESpnNavigationDirection::Down));
	TestEqual(TEXT("torso"), Vm.GetFocusedSlot(), SlotTorso);
	TestEqual(TEXT("it is empty, so nothing is selected"), Sel(), int64{0});
	TestFalse(TEXT("no slot beyond the last"), Vm.Navigate(ESpnNavigationDirection::Down));
	TestTrue(TEXT("Up returns to the hand"), Vm.Navigate(ESpnNavigationDirection::Up));
	TestEqual(TEXT("an occupied slot selects its item"), Sel(), static_cast<int64>(Ids[5]));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnVmSortFilterTest, "SpnCore.InventoryViewModel.SortAndFilter", SpnVmFlags)
bool FSpnVmSortFilterTest::RunTest(const FString& Parameters)
{
	FVmEnv Env;
	if (!TestTrue(TEXT("setup"), Env.Setup()))
	{
		return false;
	}
	USpnInventoryViewModel& Vm = Env.MakeVm({Body});
	const uint64 Coat1 = AddUnique(Env, Body, Coat);   // 1200 g, clothing
	const uint64 Light = AddUnique(Env, Body, Flashlight); // 400 g, tool
	Env.Inv().AddItem(Body, Ration, 3);                // 600 g, consumable
	const auto Ids = [&Vm]()
	{
		TArray<int64> Out;
		FSpnContainerPanel Panel;
		Vm.GetPanel(Body, Panel);
		for (const FSpnItemRow& Row : Panel.Rows) { Out.Add(Row.StackId); }
		return Out;
	};
	const int64 RationStack = Env.Inv().LocateStack(3) == Body ? 3 : 0;
	TestEqual(TEXT("default: ascending stack id"), Ids(), TArray<int64>({1, 2, 3}));
	Vm.SetSortMode(ESpnInventorySort::TotalWeight);
	TestEqual(TEXT("by weight: flashlight 400, ration 600, coat 1200"), Ids(), TArray<int64>({static_cast<int64>(Light), RationStack, static_cast<int64>(Coat1)}));
	Vm.SetSortMode(ESpnInventorySort::TotalWeight, true);
	TestEqual(TEXT("descending"), Ids(), TArray<int64>({static_cast<int64>(Coat1), RationStack, static_cast<int64>(Light)}));
	Vm.SetSortMode(ESpnInventorySort::Name);
	TestEqual(TEXT("by name (the id without a presentation): coat, flashlight, ration"), Ids(), TArray<int64>({static_cast<int64>(Coat1), static_cast<int64>(Light), RationStack}));
	Vm.SetSortMode(ESpnInventorySort::Category);
	TestEqual(TEXT("by category: clothing, consumable, tool"), Ids(), TArray<int64>({static_cast<int64>(Coat1), RationStack, static_cast<int64>(Light)}));
	Vm.SetSortMode(ESpnInventorySort::Quantity, true);
	TestEqual(TEXT("by quantity descending: 3 rations first, ties by stack id"), Ids(), TArray<int64>({RationStack, static_cast<int64>(Coat1), static_cast<int64>(Light)}));

	Vm.SetSortMode(ESpnInventorySort::Default);
	Vm.SetCategoryFilter(TEXT("spn_test:cat.tool"));
	TestEqual(TEXT("filter: tools only"), Ids(), TArray<int64>({static_cast<int64>(Light)}));
	FSpnContainerPanel Panel;
	Vm.GetPanel(Body, Panel);
	TestEqual(TEXT("the capacity still counts everything"), Panel.TotalStackCount, 3);
	TestEqual(TEXT("and the weight meter too"), Panel.WeightMeter.Used, int64{2200});
	Vm.SelectStack(Body, static_cast<int64>(Light));
	Vm.SetCategoryFilter(TEXT("spn_test:cat.clothing"));
	TestEqual(TEXT("a selection that is filtered out moves to the visible row"), Vm.GetSelectedStackId(), static_cast<int64>(Coat1));
	Vm.SetCategoryFilter(FString());
	TestEqual(TEXT("filter cleared: all rows"), Ids().Num(), 3);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnVmMessagesTest, "SpnCore.InventoryViewModel.ErrorMessages", SpnVmFlags)
bool FSpnVmMessagesTest::RunTest(const FString& Parameters)
{
	TSet<FString> Keys;
	for (uint8 Value = 0; Value <= static_cast<uint8>(ESpnInventoryError::DuplicateDefinition); ++Value)
	{
		const ESpnInventoryError Error = static_cast<ESpnInventoryError>(Value);
		const FSpnInventoryMessage Message = SpnMakeInventoryErrorMessage(Error, 2);
		TestFalse(FString::Printf(TEXT("%s has a key"), LexToString(Error)), Message.Key.IsEmpty());
		TestFalse(FString::Printf(TEXT("%s has a text"), LexToString(Error)), Message.Text.IsEmpty());
		TestTrue(FString::Printf(TEXT("%s: the key is unique"), LexToString(Error)), !Keys.Contains(Message.Key));
		Keys.Add(Message.Key);
		TestEqual(TEXT("the error is carried"), Message.Error, Error);
		TestEqual(TEXT("and the command index"), Message.CommandIndex, 2);
		TestEqual(TEXT("None is information, everything else an error"), Message.Severity, Error == ESpnInventoryError::None ? ESpnMessageSeverity::Info : ESpnMessageSeverity::Error);
	}
	TestTrue(TEXT("keys follow the naming scheme"), SpnMakeInventoryErrorMessage(ESpnInventoryError::WeightLimitReached).Key == TEXT("spn.inventory.error.weight_limit"));
	return true;
}

// === save / load refresh and compatibility policies ==============================================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnVmSaveLoadTest, "SpnCore.InventoryViewModel.SaveLoadRefreshesTheView", SpnVmFlags)
bool FSpnVmSaveLoadTest::RunTest(const FString& Parameters)
{
	FVmEnv Env;
	if (!TestTrue(TEXT("setup"), Env.Setup()))
	{
		return false;
	}
	USpnInventoryViewModel& Vm = Env.MakeVm({Body, Box});
	Env.Inv().AddItem(Body, Ration, 3);
	const uint64 Light = AddUnique(Env, Body, Flashlight);
	Env.Inv().Equip(Body, Light, SlotHand);
	TestTrue(TEXT("select the flashlight"), Vm.SelectStack(Body, static_cast<int64>(Light)));
	TestTrue(TEXT("save"), Env.Save().SaveSlot(SlotId).IsOk());
	FSpnContainerPanel Saved;
	Vm.GetPanel(Body, Saved);

	// change a lot after the save
	Env.Inv().Unequip(Body, SlotHand);
	Env.Inv().TransferItem(Body, Box, Light);
	Env.Inv().RemoveItem(Body, 1, 3);
	Env.Inv().AddItem(Body, Bandage, 9);
	TestNotEqual(TEXT("the view changed"), Vm.GetPanels()[0].Rows.Num(), Saved.Rows.Num());

	const int32 Version = Vm.GetVersion();
	const FSpnSaveStatus Load = Env.Save().LoadSlot(SlotId);
	TestTrue(FString::Printf(TEXT("load (%s)"), *Load.Detail), Load.IsOk());
	TestTrue(TEXT("the view refreshed"), Vm.GetVersion() > Version);
	MatchesCore(*this, Env, Vm, Body, TEXT("body after load"));
	MatchesCore(*this, Env, Vm, Box, TEXT("box after load"));
	FSpnContainerPanel After;
	Vm.GetPanel(Body, After);
	TestEqual(TEXT("the panel equals the one at the time of the save"), After.Rows.Num(), Saved.Rows.Num());
	TestEqual(TEXT("the weight meter too"), After.WeightMeter.Used, Saved.WeightMeter.Used);
	TestTrue(TEXT("the equipment is back"), After.Slots.ContainsByPredicate([&](const FSpnEquipmentSlotView& Slot) { return Slot.SlotId == SlotHand && Slot.EquippedStackId == static_cast<int64>(Light); }));
	TestEqual(TEXT("the selection survived the load (same container, same stack id)"), Vm.GetSelectedStackId(), static_cast<int64>(Light));
	TestEqual(TEXT("in the container where it was saved"), Vm.GetSelectedContainerId(), Body);
	TestFalse(TEXT("a plain successful load needs no notice for the player"), Vm.GetLastMessage().IsSet());

	// a failed load changes neither the view nor the version
	TArray<uint8> Garbage;
	Garbage.Append(reinterpret_cast<const uint8*>("junk"), 4);
	WriteBytes(Env.File(), Garbage);
	IFileManager::Get().Delete(*(Env.File() + TEXT(".prev")));
	const int32 VersionBeforeFailure = Vm.GetVersion();
	const TArray<uint8> Before = Fingerprint(Env);
	TestFalse(TEXT("the load fails"), Env.Save().LoadSlot(SlotId).IsOk());
	TestEqual(TEXT("the view did not change"), Vm.GetVersion(), VersionBeforeFailure);
	TestTrue(TEXT("and neither did the state"), Same(Fingerprint(Env), Before));
	TestEqual(TEXT("the player is told"), Vm.GetLastMessage().Key, FString(TEXT("spn.save.load_failed")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnCompatFingerprintTest, "SpnCore.InventoryCompat.CatalogFingerprint", SpnVmFlags)
bool FSpnCompatFingerprintTest::RunTest(const FString& Parameters)
{
	FVmEnv Env;
	TestEqual(TEXT("no catalog, no fingerprint"), Env.Inv().GetCatalogFingerprint(), FString());
	if (!TestTrue(TEXT("setup"), Env.SetupCatalog()))
	{
		return false;
	}
	const FString Fingerprint = Env.Inv().GetCatalogFingerprint();
	TestTrue(TEXT("fp1: + 64 hex digits"), Fingerprint.StartsWith(TEXT("fp1:")) && Fingerprint.Len() == 68);
	// frozen: the value of the temporary test catalog; a change of the file or of the format must change it deliberately
	TestEqual(TEXT("frozen fingerprint of the temporary test catalog (verified independently with .NET SHA-256 over the canonical text)"), Fingerprint, FString(TEXT("fp1:483b2b138550f106f0aa2208737fa82ed0952debedb1cb6ed1b73be0d0c1a452")));

	FString Text;
	FFileHelper::LoadFileToString(Text, *USpnInventorySubsystem::GetTestCatalogPath());
	const auto FingerprintOf = [&Env](const FString& Json)
	{
		Env.Inv().ClearCatalog();
		const FSpnInventoryStatus Status = Env.Inv().SetCatalogFromJson(Json);
		return Status.IsOk() ? Env.Inv().GetCatalogFingerprint() : FString(TEXT("<rejected>"));
	};
	TestEqual(TEXT("loading the same text again gives the same fingerprint"), FingerprintOf(Text), Fingerprint);

	// formatting, key order, whitespace, description and presentation-only data do not matter
	FString Compact = Text.Replace(TEXT("\r"), TEXT("")).Replace(TEXT("\n"), TEXT(" ")).Replace(TEXT("  "), TEXT(" "));
	TestEqual(TEXT("different whitespace"), FingerprintOf(Compact), Fingerprint);
	const FString Reordered = TEXT("{\"container_presets\":[{\"id\":\"spn_test:preset.trunk\",\"kind\":\"spn_test:container.trunk\"}],\"items\":["
		"{\"volume_ml\":60000,\"weight_g\":15000,\"max_stack\":1,\"category\":\"spn_test:cat.bulky\",\"id\":\"spn_test:item.crate\"},"
		"{\"equip_slots\":[\"spn_test:slot.torso\"],\"volume_ml\":4000,\"weight_g\":1200,\"max_stack\":1,\"category\":\"spn_test:cat.clothing\",\"id\":\"spn_test:item.coat\"},"
		"{\"volume_ml\":120,\"weight_g\":150,\"max_stack\":1,\"category\":\"spn_test:cat.tool\",\"id\":\"spn_test:item.notebook\"},"
		"{\"equip_slots\":[\"spn_test:slot.hand\"],\"volume_ml\":250,\"weight_g\":400,\"max_stack\":1,\"category\":\"spn_test:cat.tool\",\"id\":\"spn_test:item.flashlight\"},"
		"{\"volume_ml\":80,\"weight_g\":50,\"max_stack\":10,\"category\":\"spn_test:cat.consumable\",\"id\":\"spn_test:item.bandage\"},"
		"{\"volume_ml\":300,\"weight_g\":200,\"max_stack\":5,\"category\":\"spn_test:cat.consumable\",\"id\":\"spn_test:item.ration\"}],"
		"\"description\":\"another description\",\"test_data\":false,\"schema\":1}");
	TestEqual(TEXT("other key order, other item order, other presets, other description"), FingerprintOf(Reordered), Fingerprint);

	// every gameplay-relevant change is visible
	const auto Changed = [&](const FString& From, const FString& To, const TCHAR* What)
	{
		const FString Fp = FingerprintOf(Text.Replace(*From, *To));
		TestTrue(FString::Printf(TEXT("%s changes the fingerprint (%s)"), What, *Fp), Fp != Fingerprint && Fp != TEXT("<rejected>"));
		return Fp;
	};
	TSet<FString> Seen{Fingerprint};
	Seen.Add(Changed(TEXT("\"weight_g\": 200"), TEXT("\"weight_g\": 201"), TEXT("weight")));
	Seen.Add(Changed(TEXT("\"volume_ml\": 300"), TEXT("\"volume_ml\": 301"), TEXT("volume")));
	Seen.Add(Changed(TEXT("\"max_stack\": 5"), TEXT("\"max_stack\": 6"), TEXT("stack limit")));
	Seen.Add(Changed(TEXT("\"category\": \"spn_test:cat.bulky\""), TEXT("\"category\": \"spn_test:cat.heavy\""), TEXT("category")));
	Seen.Add(Changed(TEXT("\"equip_slots\": [\"spn_test:slot.torso\"]"), TEXT("\"equip_slots\": [\"spn_test:slot.back\"]"), TEXT("equipment slot")));
	Seen.Add(Changed(TEXT("spn_test:item.notebook"), TEXT("spn_test:item.notebook2"), TEXT("item id")));
	TestEqual(TEXT("all variations are different from each other"), Seen.Num(), 7);
	// a container preset is not a definition: changing it does not change the fingerprint
	TestEqual(TEXT("container presets are not part of it"), FingerprintOf(Text.Replace(TEXT("\"max_stacks\": 8"), TEXT("\"max_stacks\": 9"))), Fingerprint);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnCompatSaveFormatTest, "SpnCore.InventoryCompat.FingerprintInTheSave", SpnVmFlags)
bool FSpnCompatSaveFormatTest::RunTest(const FString& Parameters)
{
	FVmEnv Env;
	if (!TestTrue(TEXT("setup"), Env.Setup()))
	{
		return false;
	}
	UseDoor(Env.World(), TEXT("door_a"));
	Env.Inv().AddItem(Body, Ration, 2);
	const FSpnSaveStatus Saved = Env.Save().SaveSlot(SlotId);
	TestTrue(TEXT("save"), Saved.IsOk());
	TestEqual(TEXT("the status names the fingerprint that was written"), Saved.StoredFingerprint, Env.Inv().GetCatalogFingerprint());

	const spn::core::SaveSlot Parsed = ParsedSlot(Env.File());
	TestEqual(TEXT("facts, inventory, meta and catalog sections"), static_cast<int32>(Parsed.Count()), 4);
	const spn::core::SaveSection* Catalog = Parsed.Find("slot.catalog");
	TestNotNull(TEXT("slot.catalog"), Catalog);
	if (Catalog != nullptr)
	{
		TestTrue(TEXT("a separate optional section of version 1"), Catalog->version == 1 && !Catalog->required);
		TestEqual(TEXT("payload: fingerprint and the inventory revision it belongs to"), FString(UTF8_TO_TCHAR(Catalog->payload.c_str())),
			FString::Printf(TEXT("catalog_fingerprint %s\ninventory_revision %llu\n"), *Env.Inv().GetCatalogFingerprint(), static_cast<unsigned long long>(Env.Inv().GetRevision())));
	}
	const spn::core::SaveSection* Meta = Parsed.Find("slot.meta");
	TestTrue(TEXT("slot.meta keeps its original two-key grammar (older builds still read it)"), Meta != nullptr && Meta->payload.find("fingerprint") == std::string::npos);

	const FSpnSaveStatus Load = Env.Save().LoadSlot(SlotId);
	TestTrue(TEXT("load"), Load.IsOk());
	TestEqual(TEXT("the catalog check says Match"), Load.CatalogCheck, ESpnCatalogCheck::Match);
	TestEqual(TEXT("the inventory was restored"), Load.InventoryOutcome, ESpnInventoryLoadOutcome::Restored);
	TestEqual(TEXT("both fingerprints are reported"), Load.StoredFingerprint, Load.CurrentFingerprint);
	TestFalse(TEXT("a verified load needs no notice"), SpnMakeSaveMessage(Load, true).IsSet());

	// a save without an active inventory has no catalog section
	Env.Inv().ClearCatalog();
	TestTrue(TEXT("save facts only"), Env.Save().SaveSlot(TEXT("slot_two")).IsOk());
	TestNull(TEXT("no fingerprint without an inventory"), ParsedSlot(Env.File(TEXT("slot_two"))).Find("slot.catalog"));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnCompatLegacyTest, "SpnCore.InventoryCompat.LegacySavesAndMigrationStatus", SpnVmFlags)
bool FSpnCompatLegacyTest::RunTest(const FString& Parameters)
{
	FVmEnv Env;
	if (!TestTrue(TEXT("setup"), Env.Setup()))
	{
		return false;
	}
	USpnInventoryViewModel& Vm = Env.MakeVm({Body});
	UseDoor(Env.World(), TEXT("door_a"));
	Env.Inv().AddItem(Body, Ration, 2);

	// (1) an M2B3.3 save: inventory but no fingerprint
	TestTrue(TEXT("save"), Env.Save().SaveSlot(SlotId).IsOk());
	spn::core::SaveSlot Legacy = ParsedSlot(Env.File());
	Legacy.Remove("slot.catalog");
	WriteSlot(Env.File(), Legacy);
	IFileManager::Get().Delete(*(Env.File() + TEXT(".prev")));
	TArray<uint8> OnDisk;
	ReadBytes(Env.File(), OnDisk);
	const FDateTime Stamp = IFileManager::Get().GetTimeStamp(*Env.File());
	Env.Inv().AddItem(Body, Bandage, 1);

	FSpnSaveStatus Load = Env.Save().LoadSlot(SlotId);
	TestTrue(FString::Printf(TEXT("a save without a fingerprint loads (%s)"), *Load.Detail), Load.IsOk());
	TestEqual(TEXT("explicitly marked as unverified"), Load.CatalogCheck, ESpnCatalogCheck::LegacyNoFingerprint);
	TestEqual(TEXT("the inventory was restored"), Load.InventoryOutcome, ESpnInventoryLoadOutcome::Restored);
	TestTrue(TEXT("no stored fingerprint, a current one"), Load.StoredFingerprint.IsEmpty() && !Load.CurrentFingerprint.IsEmpty());
	TestEqual(TEXT("the player is told once"), Vm.GetLastMessage().Key, FString(TEXT("spn.inventory.load.catalog_unverified")));
	TestEqual(TEXT("the rations are back, the later bandage is gone"), Env.Inv().CountOf(Body, Bandage), uint64{0});
	TArray<uint8> After;
	ReadBytes(Env.File(), After);
	TestTrue(TEXT("loading did NOT overwrite the old file (bytes)"), After == OnDisk);
	TestEqual(TEXT("or touch it (timestamp)"), IFileManager::Get().GetTimeStamp(*Env.File()), Stamp);
	TestFalse(TEXT("and created no previous generation"), IFileManager::Get().FileExists(*(Env.File() + TEXT(".prev"))));
	TestTrue(TEXT("the next explicit save writes the fingerprint"), Env.Save().SaveSlot(SlotId).IsOk() && ParsedSlot(Env.File()).Find("slot.catalog") != nullptr);

	// (2) a save from before inventories existed: facts only
	FVmEnv Old;
	UseDoor(Old.World(), TEXT("door_a"));
	Old.Save().SetSaveDirectory(Env.Root);
	TestTrue(TEXT("save facts only"), Old.Save().SaveSlot(TEXT("slot_old")).IsOk());
	TArray<uint8> OldBytes;
	ReadBytes(Env.File(TEXT("slot_old")), OldBytes);
	const FDateTime OldStamp = IFileManager::Get().GetTimeStamp(*Env.File(TEXT("slot_old")));
	Old.Save().ResetSaveDirectory();

	Env.Inv().AddItem(Body, Bandage, 4);
	Load = Env.Save().LoadSlot(TEXT("slot_old"));
	TestTrue(TEXT("a valid older save loads"), Load.IsOk());
	TestEqual(TEXT("the migration is explicit: the inventory is empty because the save has none"), Load.InventoryOutcome, ESpnInventoryLoadOutcome::MigratedEmptyNoSection);
	TestTrue(TEXT("what the live inventory held was discarded and the status says so"), Load.bInventoryResetToEmpty);
	TestEqual(TEXT("no catalog check without a section"), Load.CatalogCheck, ESpnCatalogCheck::NotApplicable);
	TestEqual(TEXT("empty inventory"), Env.Inv().ListContainers().Num(), 0);
	TestEqual(TEXT("the view shows it"), Vm.GetPanels().Num(), 0);
	TestEqual(TEXT("and tells the player"), Vm.GetLastMessage().Key, FString(TEXT("spn.inventory.load.migrated_empty")));
	TArray<uint8> OldAfter;
	ReadBytes(Env.File(TEXT("slot_old")), OldAfter);
	TestTrue(TEXT("the old file is untouched by the load"), OldAfter == OldBytes && IFileManager::Get().GetTimeStamp(*Env.File(TEXT("slot_old"))) == OldStamp);
	TestFalse(TEXT("and no previous generation was created"), IFileManager::Get().FileExists(*(Env.File(TEXT("slot_old")) + TEXT(".prev"))));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnCompatMismatchTest, "SpnCore.InventoryCompat.CatalogMismatchNeverResetsSilently", SpnVmFlags)
bool FSpnCompatMismatchTest::RunTest(const FString& Parameters)
{
	FVmEnv Env;
	if (!TestTrue(TEXT("setup"), Env.Setup()))
	{
		return false;
	}
	USpnInventoryViewModel& Vm = Env.MakeVm({Body});
	UseDoor(Env.World(), TEXT("door_a"));
	Env.Inv().AddItem(Body, Ration, 3);
	TestTrue(TEXT("save"), Env.Save().SaveSlot(SlotId).IsOk());
	const FString Original = Env.Inv().GetCatalogFingerprint();

	// the designers change the weight of a ration
	FString Text;
	FFileHelper::LoadFileToString(Text, *USpnInventorySubsystem::GetTestCatalogPath());
	const FString Changed = Text.Replace(TEXT("\"weight_g\": 200"), TEXT("\"weight_g\": 250"));
	Env.Inv().ResetInventory();
	TestTrue(TEXT("install the changed catalog"), Env.Inv().SetCatalogFromJson(Changed).IsOk());
	TestTrue(TEXT("containers for the new session"), Env.SetupContainers());
	Env.Inv().AddItem(Body, Bandage, 2);
	UseDoor(Env.World(), TEXT("door_b"));
	TestNotEqual(TEXT("another fingerprint"), Env.Inv().GetCatalogFingerprint(), Original);
	const TArray<uint8> Before = Fingerprint(Env);

	FSpnSaveStatus Load = Env.Save().LoadSlot(SlotId);
	TestEqual(TEXT("a mismatch is refused"), Load.Result, ESpnSaveResult::CatalogMismatch);
	TestTrue(TEXT("both fingerprints are in the status"), Load.StoredFingerprint.IsEmpty() ? Load.Detail.Contains(Original) : true);
	TestTrue(TEXT("nothing was reset or changed: facts and inventory byte-identical"), Same(Fingerprint(Env), Before));
	TestEqual(TEXT("the bandages are still there"), Env.Inv().CountOf(Body, Bandage), uint64{2});
	TestEqual(TEXT("the player is told"), Vm.GetLastMessage().Key, FString(TEXT("spn.inventory.load.catalog_mismatch")));
	TestEqual(TEXT("and there was no silent fall back to an older generation"), Load.Source, ESpnSaveSource::None);

	// accepting is an explicit decision and keeps the saved content
	FSpnLoadOptions Accept;
	Accept.bAcceptCatalogMismatch = true;
	Load = Env.Save().LoadSlot(SlotId, Accept);
	TestTrue(FString::Printf(TEXT("accepted (%s)"), *Load.Detail), Load.IsOk());
	TestEqual(TEXT("marked as accepted mismatch"), Load.CatalogCheck, ESpnCatalogCheck::MismatchAccepted);
	TestTrue(TEXT("with both fingerprints"), Load.StoredFingerprint == Original && Load.CurrentFingerprint == Env.Inv().GetCatalogFingerprint());
	TestEqual(TEXT("the saved rations are there (not reset)"), Env.Inv().CountOf(Body, Ration), uint64{3});
	TestEqual(TEXT("with the NEW weight"), Vm.GetPanels()[0].WeightMeter.Used, int64{750});
	TestEqual(TEXT("the player is warned"), Vm.GetLastMessage().Severity, ESpnMessageSeverity::Warning);

	// accepting does not skip validation: a catalog without the item cannot hold the inventory
	Env.Inv().ResetInventory();
	const FString Without = TEXT("{\"schema\":1,\"items\":[{\"id\":\"spn_test:item.other\",\"category\":\"spn_test:cat.x\"}]}");
	TestTrue(TEXT("install an unrelated catalog"), Env.Inv().SetCatalogFromJson(Without).IsOk());
	const FSpnSaveStatus Invalid = Env.Save().LoadSlot(SlotId, Accept);
	TestEqual(TEXT("still rejected by the inventory parser"), Invalid.Result, ESpnSaveResult::Rejected);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnCompatCorruptTest, "SpnCore.InventoryCompat.CorruptIsNeverMissing", SpnVmFlags)
bool FSpnCompatCorruptTest::RunTest(const FString& Parameters)
{
	FVmEnv Env;
	if (!TestTrue(TEXT("setup"), Env.Setup()))
	{
		return false;
	}
	UseDoor(Env.World(), TEXT("door_a"));
	Env.Inv().AddItem(Body, Ration, 2);
	TestTrue(TEXT("save"), Env.Save().SaveSlot(SlotId).IsOk());
	const spn::core::SaveSlot Good = ParsedSlot(Env.File());
	Env.Inv().AddItem(Body, Bandage, 5);
	UseDoor(Env.World(), TEXT("door_b"));
	const TArray<uint8> Before = Fingerprint(Env);

	const auto Check = [&](const TCHAR* Name, const spn::core::SaveSlot& Slot, ESpnSaveResult Expected)
	{
		WriteSlot(Env.File(), Slot);
		IFileManager::Get().Delete(*(Env.File() + TEXT(".prev")));
		const FSpnSaveStatus Status = Env.Save().LoadSlot(SlotId);
		TestEqual(FString::Printf(TEXT("%s: result"), Name), Status.Result, Expected);
		TestNotEqual(FString::Printf(TEXT("%s: never reported as a migration to empty"), Name), Status.InventoryOutcome, ESpnInventoryLoadOutcome::MigratedEmptyNoSection);
		TestTrue(FString::Printf(TEXT("%s: facts and inventory untouched"), Name), Same(Fingerprint(Env), Before));
	};
	{
		spn::core::SaveSlot Bad = Good;
		(void)Bad.Put(spn::core::SaveSection{"player.inventory", 1, false, "garbage\n"});
		Check(TEXT("corrupt inventory payload"), Bad, ESpnSaveResult::Rejected);
	}
	{
		spn::core::SaveSlot Bad = Good;
		(void)Bad.Put(spn::core::SaveSection{"player.inventory", 1, false, ""});
		Check(TEXT("empty inventory payload"), Bad, ESpnSaveResult::Rejected);
	}
	{
		spn::core::SaveSlot Bad = Good;
		(void)Bad.Put(spn::core::SaveSection{"slot.catalog", 1, false, "catalog_fingerprint nonsense\n"});
		Check(TEXT("corrupt catalog section"), Bad, ESpnSaveResult::Rejected);
	}
	{
		// the contrast: a MISSING section (valid older save) is a migration, a damaged one is not
		spn::core::SaveSlot Missing = Good;
		Missing.Remove("player.inventory");
		Missing.Remove("slot.catalog");
		(void)Missing.Put(spn::core::SaveSection{"slot.meta", 1, false, "facts_revision 1\n"});
		WriteSlot(Env.File(), Missing);
		const FSpnSaveStatus Status = Env.Save().LoadSlot(SlotId);
		TestTrue(TEXT("a missing section loads"), Status.IsOk());
		TestEqual(TEXT("as an explicit migration"), Status.InventoryOutcome, ESpnInventoryLoadOutcome::MigratedEmptyNoSection);
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnCompatUnsupportedTest, "SpnCore.InventoryCompat.NewerAndUnintegratedSectionsBlock", SpnVmFlags)
bool FSpnCompatUnsupportedTest::RunTest(const FString& Parameters)
{
	FVmEnv Env;
	if (!TestTrue(TEXT("setup"), Env.Setup()))
	{
		return false;
	}
	UseDoor(Env.World(), TEXT("door_a"));
	Env.Inv().AddItem(Body, Ration, 2);
	TestTrue(TEXT("save"), Env.Save().SaveSlot(SlotId).IsOk());
	const spn::core::SaveSlot Good = ParsedSlot(Env.File());
	const std::string CatalogPayload = Good.Find("slot.catalog")->payload;
	const TArray<uint8> Before = Fingerprint(Env);

	struct FCase { const TCHAR* Name; spn::core::SaveSlot Slot; };
	TArray<FCase> Cases;
	{
		spn::core::SaveSlot S = Good;
		(void)S.Put(spn::core::SaveSection{"slot.catalog", 2, false, CatalogPayload});
		Cases.Add({TEXT("slot.catalog version 2"), S});
	}
	{
		std::string Future = CatalogPayload;
		Future.replace(Future.find("fp1:"), 4, "fp2:");
		spn::core::SaveSlot S = Good;
		(void)S.Put(spn::core::SaveSection{"slot.catalog", 1, false, Future});
		Cases.Add({TEXT("a fingerprint algorithm from the future (fp2)"), S});
	}
	{
		spn::core::SaveSlot S = Good;
		(void)S.Put(spn::core::SaveSection{"future.required", 1, true, "x"});
		Cases.Add({TEXT("an unknown REQUIRED section"), S});
	}
	{
		spn::core::SaveSlot S = Good;
		(void)S.Put(spn::core::SaveSection{"world.facts", 2, true, Good.Find("world.facts")->payload});
		Cases.Add({TEXT("world.facts from a newer version"), S});
	}
	for (const FCase& Case : Cases)
	{
		WriteSlot(Env.File(), Case.Slot);
		IFileManager::Get().Delete(*(Env.File() + TEXT(".prev")));
		TArray<uint8> OnDisk, After;
		ReadBytes(Env.File(), OnDisk);
		const FSpnSaveStatus Load = Env.Save().LoadSlot(SlotId);
		TestEqual(FString::Printf(TEXT("%s: load is UnsupportedVersion"), Case.Name), Load.Result, ESpnSaveResult::UnsupportedVersion);
		TestEqual(FString::Printf(TEXT("%s: save over it is refused"), Case.Name), Env.Save().SaveSlot(SlotId).Result, ESpnSaveResult::UnsupportedVersion);
		ReadBytes(Env.File(), After);
		TestTrue(FString::Printf(TEXT("%s: file untouched"), Case.Name), OnDisk == After);
		TestTrue(FString::Printf(TEXT("%s: state untouched"), Case.Name), Same(Fingerprint(Env), Before));
	}
	// sections without an owner still block, with the inventory active
	{
		spn::core::SaveSlot S = Good;
		(void)S.Put(spn::core::SaveSection{"world.map", 1, false, "x"});
		WriteSlot(Env.File(), S);
		TestEqual(TEXT("world.map blocks the load"), Env.Save().LoadSlot(SlotId).Result, ESpnSaveResult::SectionsNotIntegrated);
	}
	// unknown OPTIONAL sections are preserved, and the catalog section is rewritten exactly once
	{
		spn::core::SaveSlot S = Good;
		(void)S.Put(spn::core::SaveSection{"future.cosmetics", 3, false, "payload"});
		WriteSlot(Env.File(), S);
		TestTrue(TEXT("loads"), Env.Save().LoadSlot(SlotId).IsOk());
		TestTrue(TEXT("saves again"), Env.Save().SaveSlot(SlotId).PreservedSections.Contains(TEXT("future.cosmetics")));
		const spn::core::SaveSlot After = ParsedSlot(Env.File());
		TestTrue(TEXT("kept byte for byte"), After.Find("future.cosmetics") != nullptr && After.Find("future.cosmetics")->payload == "payload");
		TestNotNull(TEXT("with a fresh fingerprint"), After.Find("slot.catalog"));
	}
	return true;
}

// === measurements ====================================================================================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnVmPerformanceTest, "SpnCore.InventoryViewModel.Performance", SpnVmFlags)
bool FSpnVmPerformanceTest::RunTest(const FString& Parameters)
{
	// Measures and reports (log + test info); the only assertions are about correctness. Numbers depend on the machine and on the build
	// (this runs in the Development editor build): they are for orders of magnitude, not for thresholds.
	FSpnItemCatalogData Catalog;
	Catalog.bIsTestData = true;
	Catalog.Items.Add({TEXT("spn_test:item.unique"), TEXT("spn_test:cat.misc"), 1, 10, 10, {}});
	FSpnContainerPresetData Free;
	Free.Id = TEXT("spn_test:preset.free");
	Free.Rules.Kind = TEXT("spn_test:container.bag");
	Catalog.ContainerPresets.Add(Free);

	for (const int32 Count : {10, 100, 1000, 10000})
	{
		FVmEnv Env;
		TestTrue(TEXT("catalog"), Env.Inv().SetCatalog(Catalog).IsOk());
		TestTrue(TEXT("containers"), Env.Inv().CreateContainerFromPreset(1, TEXT("spn_test:preset.free")).IsOk() && Env.Inv().CreateContainerFromPreset(2, TEXT("spn_test:preset.free")).IsOk());
		TArray<FSpnInventoryCommand> Fill;
		for (int32 I = 0; I < Count; ++I) { Fill.Add(FSpnInventoryCommand::AddItem(1, TEXT("spn_test:item.unique"))); }
		TestTrue(TEXT("fill in one transaction"), Env.Inv().Apply(Fill).IsOk());

		// (a) raw read of the container through the public API
		double Start = FPlatformTime::Seconds();
		FSpnContainerView View;
		for (int32 I = 0; I < 5; ++I) { Env.Inv().GetContainer(1, View); }
		const double RawMs = (FPlatformTime::Seconds() - Start) * 1000.0 / 5.0;

		// (b) full build of the view-model (what opening a screen costs)
		Start = FPlatformTime::Seconds();
		USpnInventoryViewModel& Vm = Env.MakeVm({1, 2});
		const double BuildMs = (FPlatformTime::Seconds() - Start) * 1000.0;
		FSpnContainerPanel Panel;
		TestTrue(TEXT("panel built"), Vm.GetPanel(1, Panel) && Panel.Rows.Num() == Count);

		// (c) one small change in a bound container (what every transaction costs while the screen is open)
		double Total = 0.0;
		for (int32 I = 0; I < 5; ++I)
		{
			Start = FPlatformTime::Seconds();
			Env.Inv().AddItem(1, TEXT("spn_test:item.unique"));
			Total += FPlatformTime::Seconds() - Start;
		}
		const double BoundMs = Total * 1000.0 / 5.0;

		// (d) the same change into the small second container (bound but tiny) and into an unbound one
		Total = 0.0;
		for (int32 I = 0; I < 5; ++I)
		{
			Start = FPlatformTime::Seconds();
			Env.Inv().AddItem(2, TEXT("spn_test:item.unique"));
			Total += FPlatformTime::Seconds() - Start;
		}
		const double SmallMs = Total * 1000.0 / 5.0;
		Vm.SetBoundContainers({2});
		Total = 0.0;
		for (int32 I = 0; I < 5; ++I)
		{
			Start = FPlatformTime::Seconds();
			Env.Inv().AddItem(1, TEXT("spn_test:item.unique"));
			Total += FPlatformTime::Seconds() - Start;
		}
		const double UnboundMs = Total * 1000.0 / 5.0;

		// (e) the same transaction without any view-model
		Vm.Shutdown();
		Total = 0.0;
		for (int32 I = 0; I < 5; ++I)
		{
			Start = FPlatformTime::Seconds();
			Env.Inv().AddItem(1, TEXT("spn_test:item.unique"));
			Total += FPlatformTime::Seconds() - Start;
		}
		const double NoVmMs = Total * 1000.0 / 5.0;

		const FString Line = FString::Printf(TEXT("PERF %5d stacks: raw GetContainer %.3f ms | view-model full build %.3f ms | AddItem with the container bound %.3f ms | small bound container %.3f ms | container not shown %.3f ms | no view-model %.3f ms"),
			Count, RawMs, BuildMs, BoundMs, SmallMs, UnboundMs, NoVmMs);
		UE_LOG(LogTemp, Display, TEXT("%s"), *Line);
		AddInfo(Line);
	}
	return true;
}
#endif
