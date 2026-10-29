#include "Engine/GameInstance.h"
#include "HAL/FileManager.h"
#include "Misc/AutomationTest.h"
#include "Misc/FileHelper.h"
#include "Misc/Guid.h"
#include "Misc/Paths.h"
#include "SpnInventorySubsystem.h"
#include "SpnSaveSubsystem.h"
#include "SpnWorldStateSubsystem.h"
#include "UObject/Package.h"

#include <spn/core/SaveSlot.h>

#include <string>

#if WITH_DEV_AUTOMATION_TESTS

// Unreal checks for USpnInventorySubsystem and its integration into USpnSaveSubsystem (M2B3.3). The item data comes from the TEMPORARY test
// catalog Content/SpnTest/Inventory/TestItemCatalog.json (test data, not game content); tests that need exact limits build their own tiny
// catalogs. Every test works in its own save root under Saved/SpnTestTemp/<guid> and deletes it afterwards: no real save is touched.

namespace
{
	constexpr EAutomationTestFlags SpnInvFlags = EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter;

	const FString Ration = TEXT("spn_test:item.ration");       // stackable 5, 200 g, 300 ml
	const FString Bandage = TEXT("spn_test:item.bandage");     // stackable 10, 50 g, 80 ml
	const FString Flashlight = TEXT("spn_test:item.flashlight"); // unique, equips to hand
	const FString Notebook = TEXT("spn_test:item.notebook");   // unique, not equippable
	const FString Coat = TEXT("spn_test:item.coat");           // unique, equips to torso
	const FString Crate = TEXT("spn_test:item.crate");         // unique, 15 kg, 60 l
	const FString SlotHand = TEXT("spn_test:slot.hand");
	const FString SlotTorso = TEXT("spn_test:slot.torso");
	constexpr uint64 Body = 1;
	constexpr uint64 Trunk = 2;
	constexpr uint64 Box = 3;
	const TCHAR* const TestSlotId = TEXT("slot_one");
	const TCHAR* const DoorA = TEXT("door_a");

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
		USpnWorldStateSubsystem* World() const { return GameInstance->GetSubsystem<USpnWorldStateSubsystem>(); }
		USpnSaveSubsystem* Save() const { return GameInstance->GetSubsystem<USpnSaveSubsystem>(); }
		USpnInventorySubsystem* Inventory() const { return GameInstance->GetSubsystem<USpnInventorySubsystem>(); }
	};

	struct FInvEnv
	{
		FScopedGameInstance Instance;
		FString Root;

		FInvEnv()
		{
			Root = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir() / TEXT("SpnTestTemp") / FGuid::NewGuid().ToString(EGuidFormats::Digits));
			Instance.Save()->SetSaveDirectory(Root);
		}
		~FInvEnv() { IFileManager::Get().DeleteDirectory(*Root, false, true); }

		USpnWorldStateSubsystem& World() const { return *Instance.World(); }
		USpnSaveSubsystem& Save() const { return *Instance.Save(); }
		USpnInventorySubsystem& Inv() const { return *Instance.Inventory(); }
		FString File(const TCHAR* SlotId = TestSlotId) const { return Save().GetSlotFilePath(SlotId); }
		FString Prev(const TCHAR* SlotId = TestSlotId) const { return File(SlotId) + TEXT(".prev"); }
	};

	/** The temporary test catalog from disk plus the three test containers. */
	bool SetupInventory(FInvEnv& Env)
	{
		const FSpnInventoryStatus Catalog = Env.Inv().LoadCatalogFromFile(USpnInventorySubsystem::GetTestCatalogPath());
		if (!Catalog.IsOk())
		{
			return false;
		}
		return Env.Inv().CreateContainerFromPreset(Body, TEXT("spn_test:preset.body")).IsOk() && Env.Inv().CreateContainerFromPreset(Trunk, TEXT("spn_test:preset.trunk")).IsOk()
			&& Env.Inv().CreateContainerFromPreset(Box, TEXT("spn_test:preset.small_box")).IsOk();
	}

	TMap<FString, bool> DoorDefaults() { return {{TEXT("open"), false}, {TEXT("locked"), false}}; }

	void UseDoor(USpnWorldStateSubsystem& World, const TCHAR* Id)
	{
		World.TryInteract(USpnWorldStateSubsystem::TestDoorKind(), Id, ESpnInteractionVerb::Use, DoorDefaults(), TEXT("automation.player"));
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

	/** Everything a save contains (facts AND inventory, journal and ids included) as bytes: equal bytes = equal state of both domains. */
	TArray<uint8> Fingerprint(FInvEnv& Env)
	{
		TArray<uint8> Bytes;
		if (Env.Save().SaveSlot(TEXT("probe")).IsOk())
		{
			ReadBytes(Env.File(TEXT("probe")), Bytes);
		}
		return Bytes;
	}

	bool Same(const TArray<uint8>& A, const TArray<uint8>& B) { return A.Num() > 0 && A == B; }

	/** Install a small catalog for exact-limit tests. */
	FSpnItemCatalogData TinyCatalog()
	{
		FSpnItemCatalogData Catalog;
		Catalog.bIsTestData = true;
		Catalog.Description = TEXT("tiny automation catalog");
		Catalog.Items.Add({TEXT("spn_test:item.heavy"), TEXT("spn_test:cat.misc"), 100, 60, 1, {}});   // weight only
		Catalog.Items.Add({TEXT("spn_test:item.bulky"), TEXT("spn_test:cat.misc"), 100, 1, 60, {}});   // volume only
		Catalog.Items.Add({TEXT("spn_test:item.plain"), TEXT("spn_test:cat.misc"), 4, 0, 0, {}});      // limits of 0 = no contribution
		FSpnContainerPresetData WeightBox;
		WeightBox.Id = TEXT("spn_test:preset.weight_box");
		WeightBox.Rules.Kind = TEXT("spn_test:container.box");
		WeightBox.Rules.MaxWeightGrams = 100;
		FSpnContainerPresetData VolumeBox;
		VolumeBox.Id = TEXT("spn_test:preset.volume_box");
		VolumeBox.Rules.Kind = TEXT("spn_test:container.box");
		VolumeBox.Rules.MaxVolumeMilliliters = 100;
		FSpnContainerPresetData Free;
		Free.Id = TEXT("spn_test:preset.free");
		Free.Rules.Kind = TEXT("spn_test:container.box");
		Catalog.ContainerPresets.Add(WeightBox);
		Catalog.ContainerPresets.Add(VolumeBox);
		Catalog.ContainerPresets.Add(Free);
		return Catalog;
	}
}

// === inventory subsystem =====================================================================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnInvInitializeTest, "SpnCore.Inventory.Initialize", SpnInvFlags)
bool FSpnInvInitializeTest::RunTest(const FString& Parameters)
{
	FInvEnv Env;
	if (!TestNotNull(TEXT("the game instance creates the inventory subsystem"), Env.Instance.Inventory()))
	{
		return false;
	}
	TestFalse(TEXT("it is inactive without a catalog"), Env.Inv().HasCatalog());
	TestEqual(TEXT("revision 0"), Env.Inv().GetRevision(), uint64{0});
	TestEqual(TEXT("no containers"), Env.Inv().ListContainers().Num(), 0);
	const FSpnInventoryStatus Status = Env.Inv().CreateContainerFromPreset(Body, TEXT("spn_test:preset.body"));
	TestEqual(TEXT("without a catalog every mutation says NoCatalog"), Status.Error, ESpnInventoryError::NoCatalog);
	TestEqual(TEXT("AddItem too"), Env.Inv().AddItem(Body, Ration, 1).Error, ESpnInventoryError::NoCatalog);
	FSpnContainerView View;
	TestFalse(TEXT("queries are safe and empty"), Env.Inv().GetContainer(Body, View));
	TestEqual(TEXT("CountOf is 0"), Env.Inv().CountOf(Body, Ration), uint64{0});
	TestTrue(TEXT("Describe says inactive"), Env.Inv().Describe().Contains(TEXT("inactive")));

	FScopedGameInstance Other;
	TestFalse(TEXT("every game instance has its own subsystem"), Other.Inventory() == Env.Instance.Inventory());
	TestTrue(TEXT("the save subsystem depends on it and exists too"), Env.Instance.Save() != nullptr);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnInvCatalogTest, "SpnCore.Inventory.CatalogRegistration", SpnInvFlags)
bool FSpnInvCatalogTest::RunTest(const FString& Parameters)
{
	FInvEnv Env;
	TestTrue(TEXT("the test catalog file exists"), IFileManager::Get().FileExists(*USpnInventorySubsystem::GetTestCatalogPath()));
	const FSpnInventoryStatus Status = Env.Inv().LoadCatalogFromFile(USpnInventorySubsystem::GetTestCatalogPath());
	TestTrue(FString::Printf(TEXT("the test catalog loads (%s)"), *Status.Detail), Status.IsOk());
	TestTrue(TEXT("active"), Env.Inv().HasCatalog());
	const FSpnItemCatalogData& Data = Env.Inv().GetCatalogData();
	TestTrue(TEXT("it is marked as test data"), Data.bIsTestData);
	TestTrue(TEXT("and says so in its description"), Data.Description.Contains(TEXT("TEST DATA")));
	TestEqual(TEXT("6 item definitions"), Data.Items.Num(), 6);
	TestEqual(TEXT("3 container presets"), Data.ContainerPresets.Num(), 3);
	for (const FSpnItemDefinitionData& Item : Data.Items)
	{
		TestTrue(FString::Printf(TEXT("%s is in the test namespace"), *Item.Id), Item.Id.StartsWith(TEXT("spn_test:")));
	}

	FSpnItemDefinitionData Definition;
	TestTrue(TEXT("ration is registered"), Env.Inv().FindDefinition(Ration, Definition));
	TestTrue(TEXT("ration: stackable 5, 200 g, 300 ml"), Definition.MaxStack == 5 && Definition.WeightGrams == 200 && Definition.VolumeMilliliters == 300);
	TestTrue(TEXT("flashlight: unique, can occupy the hand"), Env.Inv().FindDefinition(Flashlight, Definition) && Definition.MaxStack == 1 && Definition.EquipSlots.Contains(SlotHand));
	TestFalse(TEXT("an unknown definition is not found"), Env.Inv().FindDefinition(TEXT("spn_test:item.nope"), Definition));
	FSpnContainerRulesData Rules;
	TestTrue(TEXT("the body preset exists"), Env.Inv().FindContainerPreset(TEXT("spn_test:preset.body"), Rules));
	TestTrue(TEXT("with 8 stacks, 20 kg, 30 l and two slots"), Rules.MaxStacks == 8 && Rules.MaxWeightGrams == 20000 && Rules.MaxVolumeMilliliters == 30000 && Rules.EquipSlots.Num() == 2);

	// a catalog cannot be replaced while containers exist, and a rejected catalog changes nothing
	TestTrue(TEXT("create a container"), Env.Inv().CreateContainerFromPreset(Body, TEXT("spn_test:preset.body")).IsOk());
	TestEqual(TEXT("replacing the catalog with containers present is refused"), Env.Inv().SetCatalog(TinyCatalog()).Error, ESpnInventoryError::CatalogLocked);
	TestEqual(TEXT("the old catalog is still there"), Env.Inv().GetCatalogData().Items.Num(), 6);
	Env.Inv().ResetInventory();
	TestEqual(TEXT("ResetInventory empties it"), Env.Inv().ListContainers().Num(), 0);
	TestTrue(TEXT("and keeps the catalog"), Env.Inv().HasCatalog());

	FSpnItemCatalogData Bad = TinyCatalog();
	{ const FSpnItemDefinitionData Copy = Bad.Items[0]; Bad.Items.Add(Copy); }
	TestEqual(TEXT("a duplicate definition is rejected by the Core"), Env.Inv().SetCatalog(Bad).Error, ESpnInventoryError::DuplicateDefinition);
	Bad = TinyCatalog();
	Bad.Items[0].Id = TEXT("Heavy Item");
	TestEqual(TEXT("a malformed id is rejected"), Env.Inv().SetCatalog(Bad).Error, ESpnInventoryError::InvalidArgument);
	Bad = TinyCatalog();
	Bad.Items[0].MaxStack = 0;
	TestEqual(TEXT("max stack 0 is rejected"), Env.Inv().SetCatalog(Bad).Error, ESpnInventoryError::InvalidArgument);
	Bad = TinyCatalog();
	Bad.Items[0].MaxStack = 5;
	Bad.Items[0].EquipSlots.Add(SlotHand);
	TestEqual(TEXT("an equippable stackable item is rejected by the Core"), Env.Inv().SetCatalog(Bad).Error, ESpnInventoryError::InvalidDefinition);
	Bad = TinyCatalog();
	{ const FSpnContainerPresetData Copy = Bad.ContainerPresets[0]; Bad.ContainerPresets.Add(Copy); }
	TestEqual(TEXT("a duplicate preset is rejected"), Env.Inv().SetCatalog(Bad).Error, ESpnInventoryError::InvalidArgument);
	TestEqual(TEXT("after all rejections the test catalog is still installed"), Env.Inv().GetCatalogData().Items.Num(), 6);
	TestFalse(TEXT("GetLastFailure remembers the last rejection"), Env.Inv().GetLastFailure().IsOk());

	TestEqual(TEXT("not JSON"), Env.Inv().SetCatalogFromJson(TEXT("this is not json")).Error, ESpnInventoryError::InvalidArgument);
	TestEqual(TEXT("wrong schema"), Env.Inv().SetCatalogFromJson(TEXT("{\"schema\":2,\"items\":[]}")).Error, ESpnInventoryError::InvalidArgument);
	TestEqual(TEXT("missing field"), Env.Inv().SetCatalogFromJson(TEXT("{\"schema\":1,\"items\":[{\"id\":\"spn_test:item.x\"}]}")).Error, ESpnInventoryError::InvalidArgument);
	TestEqual(TEXT("negative number"), Env.Inv().SetCatalogFromJson(TEXT("{\"schema\":1,\"items\":[{\"id\":\"spn_test:item.x\",\"category\":\"spn_test:cat.x\",\"weight_g\":-1}]}")).Error, ESpnInventoryError::InvalidArgument);
	TestEqual(TEXT("missing file"), Env.Inv().LoadCatalogFromFile(Env.Root / TEXT("nope.json")).Error, ESpnInventoryError::InvalidArgument);
	TestTrue(TEXT("a minimal valid JSON catalog installs"), Env.Inv().SetCatalogFromJson(TEXT("{\"schema\":1,\"items\":[{\"id\":\"spn_test:item.x\",\"category\":\"spn_test:cat.x\"}]}")).IsOk());
	TestFalse(TEXT("and is not marked as test data unless it says so"), Env.Inv().GetCatalogData().bIsTestData);
	Env.Inv().ClearCatalog();
	TestFalse(TEXT("ClearCatalog deactivates"), Env.Inv().HasCatalog());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnInvAddRemoveTest, "SpnCore.Inventory.AddRemoveAndStacks", SpnInvFlags)
bool FSpnInvAddRemoveTest::RunTest(const FString& Parameters)
{
	FInvEnv Env;
	if (!TestTrue(TEXT("setup"), SetupInventory(Env)))
	{
		return false;
	}
	USpnInventorySubsystem& Inv = Env.Inv();
	const uint64 BaseRevision = Inv.GetRevision();

	FSpnInventoryStatus Status = Inv.AddItem(Body, Ration, 3);
	TestTrue(TEXT("add 3 rations"), Status.IsOk());
	TestEqual(TEXT("one stack was created"), Status.CreatedStacks.Num(), 1);
	TestEqual(TEXT("the first stack id is 1"), Status.CreatedStacks[0], uint64{1});
	TestEqual(TEXT("the revision advanced by one"), Status.Revision, BaseRevision + 1);
	TestEqual(TEXT("CountOf"), Inv.CountOf(Body, Ration), uint64{3});
	FSpnContainerView View;
	TestTrue(TEXT("read the container"), Inv.GetContainer(Body, View));
	TestEqual(TEXT("weight 3 x 200 g"), View.WeightGrams, uint64{600});
	TestEqual(TEXT("volume 3 x 300 ml"), View.VolumeMilliliters, uint64{900});

	// stack behaviour: fills the existing stack first, then opens a new one at the stack limit of 5
	Status = Inv.AddItem(Body, Ration, 4);
	TestTrue(TEXT("add 4 more"), Status.IsOk());
	TestEqual(TEXT("one new stack (2 filled the first one)"), Status.CreatedStacks.Num(), 1);
	TestEqual(TEXT("total 7"), Inv.CountOf(Body, Ration), uint64{7});
	Inv.GetContainer(Body, View);
	TestEqual(TEXT("two stacks: 5 and 2"), View.Stacks.Num(), 2);
	TestTrue(TEXT("quantities 5 and 2"), View.Stacks[0].Quantity == 5 && View.Stacks[1].Quantity == 2);

	// split and merge
	TestTrue(TEXT("split 1 off the full stack"), Inv.SplitStack(Body, View.Stacks[0].StackId, 1).IsOk());
	Inv.GetContainer(Body, View);
	TestEqual(TEXT("three stacks"), View.Stacks.Num(), 3);
	const uint64 Source = View.Stacks[2].StackId;
	TestTrue(TEXT("merge the 1 back as far as it fits"), Inv.MergeStacks(Body, Source, View.Stacks[0].StackId).IsOk());
	Inv.GetContainer(Body, View);
	TestEqual(TEXT("two stacks again"), View.Stacks.Num(), 2);

	// removal: partial, whole
	const uint64 First = View.Stacks[0].StackId;
	TestTrue(TEXT("remove 2 of the first stack"), Inv.RemoveItem(Body, First, 2).IsOk());
	TestEqual(TEXT("5 left"), Inv.CountOf(Body, Ration), uint64{5});
	Inv.GetContainer(Body, View);
	TestTrue(TEXT("remove a whole stack"), Inv.RemoveItem(Body, View.Stacks[1].StackId, View.Stacks[1].Quantity).IsOk());
	Inv.GetContainer(Body, View);
	TestEqual(TEXT("one stack remains"), View.Stacks.Num(), 1);
	TestEqual(TEXT("and it is not the removed one"), View.Stacks[0].StackId, First);

	// ids are never reused
	const uint64 Next = Inv.GetNextStackId();
	Status = Inv.AddItem(Body, Bandage, 1);
	TestTrue(TEXT("a new item gets a fresh id"), Status.IsOk() && Status.CreatedStacks[0] == Next && Next > First);
	TestEqual(TEXT("the Core's invariants hold"), Inv.CheckInvariants(), FString());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnInvUniqueItemsTest, "SpnCore.Inventory.UniqueInstancesAndState", SpnInvFlags)
bool FSpnInvUniqueItemsTest::RunTest(const FString& Parameters)
{
	FInvEnv Env;
	if (!TestTrue(TEXT("setup"), SetupInventory(Env)))
	{
		return false;
	}
	USpnInventorySubsystem& Inv = Env.Inv();
	const TArray<FSpnItemStateField> State = {FSpnItemStateField::MakeInt(TEXT("charge"), 87), FSpnItemStateField::MakeBool(TEXT("on"), true), FSpnItemStateField::MakeString(TEXT("note"), TEXT("Tür 3 – Keller"))};
	const FSpnInventoryStatus A = Inv.AddItem(Body, Flashlight, 1, State);
	const FSpnInventoryStatus B = Inv.AddItem(Body, Flashlight, 1, {FSpnItemStateField::MakeInt(TEXT("charge"), 12)});
	TestTrue(TEXT("two flashlights can be added"), A.IsOk() && B.IsOk());
	TestTrue(TEXT("they are separate instances with separate ids"), A.CreatedStacks[0] != B.CreatedStacks[0]);
	FSpnContainerView View;
	Inv.GetContainer(Body, View);
	TestEqual(TEXT("two stacks"), View.Stacks.Num(), 2);
	TestEqual(TEXT("quantity 1 each"), View.Stacks[0].Quantity, 1u);
	TestEqual(TEXT("three state fields kept"), View.Stacks[0].State.Num(), 3);
	bool bCharge = false, bOn = false, bNote = false;
	for (const FSpnItemStateField& Field : View.Stacks[0].State)
	{
		bCharge |= Field.Name == TEXT("charge") && Field.Type == ESpnStateValueType::Int && Field.IntValue == 87;
		bOn |= Field.Name == TEXT("on") && Field.Type == ESpnStateValueType::Bool && Field.BoolValue;
		bNote |= Field.Name == TEXT("note") && Field.Type == ESpnStateValueType::String && Field.StringValue == TEXT("Tür 3 – Keller");
	}
	TestTrue(TEXT("int, bool and (non-ASCII) string state round trip"), bCharge && bOn && bNote);
	TestEqual(TEXT("the second instance has only its own state"), View.Stacks[1].State.Num(), 1);

	TestEqual(TEXT("two of a unique item at once is refused (the Core: a non-stackable item has quantity 1)"), Inv.AddItem(Body, Notebook, 2).Error, ESpnInventoryError::InvalidQuantity);
	TestEqual(TEXT("state on a stackable item is refused"), Inv.AddItem(Body, Ration, 1, State).Error, ESpnInventoryError::InvalidState);
	TestEqual(TEXT("a duplicate state field name is refused before the Core"), Inv.AddItem(Body, Notebook, 1, {FSpnItemStateField::MakeInt(TEXT("a"), 1), FSpnItemStateField::MakeInt(TEXT("a"), 2)}).Error, ESpnInventoryError::InvalidArgument);
	TestEqual(TEXT("an invalid state name is refused by the Core"), Inv.AddItem(Body, Notebook, 1, {FSpnItemStateField::MakeInt(TEXT("Bad Name"), 1)}).Error, ESpnInventoryError::InvalidState);
	TestEqual(TEXT("none of that changed anything"), Inv.CountOf(Body, Notebook), uint64{0});
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnInvCapacityTest, "SpnCore.Inventory.CapacityLimits", SpnInvFlags)
bool FSpnInvCapacityTest::RunTest(const FString& Parameters)
{
	FInvEnv Env;
	if (!TestTrue(TEXT("setup"), SetupInventory(Env)))
	{
		return false;
	}
	USpnInventorySubsystem& Inv = Env.Inv();
	// small box (test preset): 3 stacks, 1000 g, 1500 ml, only consumables and tools
	TestTrue(TEXT("3 distinct stacks fit"), Inv.AddItem(Box, Ration, 1).IsOk() && Inv.AddItem(Box, Bandage, 1).IsOk() && Inv.AddItem(Box, Notebook, 1).IsOk());
	FSpnInventoryStatus Status = Inv.AddItem(Box, Flashlight, 1);
	TestEqual(TEXT("the 4th stack hits the stack limit"), Status.Error, ESpnInventoryError::StackLimitReached);
	TestEqual(TEXT("and the box is unchanged"), Inv.CountOf(Box, Flashlight), uint64{0});
	TestEqual(TEXT("a coat is the wrong category for the box"), Inv.AddItem(Box, Coat, 1).Error, ESpnInventoryError::CategoryNotAllowed);
	TestEqual(TEXT("a crate is the wrong category too"), Inv.AddItem(Box, Crate, 1).Error, ESpnInventoryError::CategoryNotAllowed);

	// exact fit and one over, on the body (8 stacks, 20 kg, 30 l)
	TestEqual(TEXT("one crate (15 kg, 60 l) exceeds only the 30 l volume limit of the body"), Inv.AddItem(Body, Crate, 1).Error, ESpnInventoryError::VolumeLimitReached);
	TestEqual(TEXT("it was not added"), Inv.CountOf(Body, Crate), uint64{0});
	TestTrue(TEXT("three crates fit the trunk (20 stacks, 80 kg, 200 l): 45 kg, 180 l"), Inv.AddItem(Trunk, Crate, 1).IsOk() && Inv.AddItem(Trunk, Crate, 1).IsOk() && Inv.AddItem(Trunk, Crate, 1).IsOk());
	TestEqual(TEXT("the 4th crate exceeds only the 200 l volume limit (240 l)"), Inv.AddItem(Trunk, Crate, 1).Error, ESpnInventoryError::VolumeLimitReached);

	// isolated limits with a tiny catalog
	Inv.ResetInventory();
	TestTrue(TEXT("install the tiny catalog"), Inv.SetCatalog(TinyCatalog()).IsOk());
	TestTrue(TEXT("weight box"), Inv.CreateContainerFromPreset(10, TEXT("spn_test:preset.weight_box")).IsOk());
	TestTrue(TEXT("volume box"), Inv.CreateContainerFromPreset(11, TEXT("spn_test:preset.volume_box")).IsOk());
	TestTrue(TEXT("free box"), Inv.CreateContainerFromPreset(12, TEXT("spn_test:preset.free")).IsOk());
	TestTrue(TEXT("1 heavy item (60 g) fits a 100 g box"), Inv.AddItem(10, TEXT("spn_test:item.heavy"), 1).IsOk());
	TestEqual(TEXT("a second one (120 g) exceeds the weight limit only"), Inv.AddItem(10, TEXT("spn_test:item.heavy"), 1).Error, ESpnInventoryError::WeightLimitReached);
	TestTrue(TEXT("1 bulky item (60 ml) fits a 100 ml box"), Inv.AddItem(11, TEXT("spn_test:item.bulky"), 1).IsOk());
	TestEqual(TEXT("a second one exceeds the volume limit only"), Inv.AddItem(11, TEXT("spn_test:item.bulky"), 1).Error, ESpnInventoryError::VolumeLimitReached);
	TestTrue(TEXT("items without weight and volume never hit a limit"), Inv.AddItem(10, TEXT("spn_test:item.plain"), 4).IsOk());
	TestTrue(TEXT("the free box has no limits"), Inv.AddItem(12, TEXT("spn_test:item.heavy"), 100).IsOk());
	TestEqual(TEXT("the Core's invariants hold"), Inv.CheckInvariants(), FString());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnInvEquipmentTest, "SpnCore.Inventory.Equipment", SpnInvFlags)
bool FSpnInvEquipmentTest::RunTest(const FString& Parameters)
{
	FInvEnv Env;
	if (!TestTrue(TEXT("setup"), SetupInventory(Env)))
	{
		return false;
	}
	USpnInventorySubsystem& Inv = Env.Inv();
	const uint64 Light = Inv.AddItem(Body, Flashlight).CreatedStacks[0];
	const uint64 Light2 = Inv.AddItem(Body, Flashlight).CreatedStacks[0];
	const uint64 CoatId = Inv.AddItem(Body, Coat).CreatedStacks[0];
	const uint64 Book = Inv.AddItem(Body, Notebook).CreatedStacks[0];
	const uint64 RationId = Inv.AddItem(Body, Ration, 2).CreatedStacks[0];
	const uint64 BoxLight = Inv.AddItem(Box, Flashlight).CreatedStacks[0];

	TestTrue(TEXT("the flashlight goes to the hand"), Inv.Equip(Body, Light, SlotHand).IsOk());
	TestEqual(TEXT("a flashlight can not occupy the (free) torso slot"), Inv.Equip(Body, Light2, SlotTorso).Error, ESpnInventoryError::NotEquippable);
	TestTrue(TEXT("the coat goes to the torso"), Inv.Equip(Body, CoatId, SlotTorso).IsOk());
	FSpnContainerView View;
	Inv.GetContainer(Body, View);
	int32 Equipped = 0;
	for (const FSpnStackView& Stack : View.Stacks)
	{
		Equipped += Stack.EquippedSlot.IsEmpty() ? 0 : 1;
		if (Stack.StackId == Light)
		{
			TestEqual(TEXT("the view names the slot"), Stack.EquippedSlot, SlotHand);
		}
	}
	TestEqual(TEXT("two stacks are equipped"), Equipped, 2);

	TestEqual(TEXT("an occupied hand is refused"), Inv.Equip(Body, Light2, SlotHand).Error, ESpnInventoryError::SlotOccupied);
	TestEqual(TEXT("a notebook is not equippable"), Inv.Equip(Body, Book, SlotHand).Error, ESpnInventoryError::NotEquippable);
	TestEqual(TEXT("a stackable ration is not equippable"), Inv.Equip(Body, RationId, SlotHand).Error, ESpnInventoryError::NotEquippable);
	TestEqual(TEXT("the box offers no slots"), Inv.Equip(Box, BoxLight, SlotHand).Error, ESpnInventoryError::UnknownSlot);
	TestEqual(TEXT("an equipped item cannot be equipped again"), Inv.Equip(Body, Light, SlotHand).Error, ESpnInventoryError::AlreadyEquipped);
	TestEqual(TEXT("a slot the container does not offer"), Inv.Equip(Body, Light2, TEXT("spn_test:slot.nope")).Error, ESpnInventoryError::UnknownSlot);
	TestEqual(TEXT("an equipped item cannot be removed"), Inv.RemoveItem(Body, Light).Error, ESpnInventoryError::ItemEquipped);
	TestEqual(TEXT("or transferred"), Inv.TransferItem(Body, Trunk, CoatId).Error, ESpnInventoryError::ItemEquipped);
	TestEqual(TEXT("unequipping a slot the container does not offer"), Inv.Unequip(Box, SlotHand).Error, ESpnInventoryError::UnknownSlot);

	// swapping is one atomic transaction
	const uint64 Before = Inv.GetRevision();
	FSpnInventoryStatus Swap = Inv.Apply({FSpnInventoryCommand::UnequipSlot(Body, SlotHand), FSpnInventoryCommand::EquipItem(Body, Light2, SlotHand)});
	TestTrue(TEXT("unequip + equip in one transaction"), Swap.IsOk());
	TestEqual(TEXT("one revision for the pair"), Inv.GetRevision(), Before + 1);
	Inv.GetContainer(Body, View);
	for (const FSpnStackView& Stack : View.Stacks)
	{
		if (Stack.StackId == Light2) { TestEqual(TEXT("the second flashlight is in the hand"), Stack.EquippedSlot, SlotHand); }
		if (Stack.StackId == Light) { TestTrue(TEXT("the first one is free"), Stack.EquippedSlot.IsEmpty()); }
	}
	// a swap whose second half fails leaves the first half undone
	Swap = Inv.Apply({FSpnInventoryCommand::UnequipSlot(Body, SlotHand), FSpnInventoryCommand::EquipItem(Body, Book, SlotHand)});
	TestEqual(TEXT("the swap fails on the unequippable notebook"), Swap.Error, ESpnInventoryError::NotEquippable);
	TestEqual(TEXT("at command 1"), Swap.FailedCommandIndex, 1);
	Inv.GetContainer(Body, View);
	for (const FSpnStackView& Stack : View.Stacks)
	{
		if (Stack.StackId == Light2) { TestEqual(TEXT("the unequip was rolled back"), Stack.EquippedSlot, SlotHand); }
	}
	TestTrue(TEXT("unequip works"), Inv.Unequip(Body, SlotHand).IsOk());
	TestEqual(TEXT("unequipping an empty slot is SlotEmpty"), Inv.Unequip(Body, SlotHand).Error, ESpnInventoryError::SlotEmpty);
	TestTrue(TEXT("and then the item can be removed"), Inv.RemoveItem(Body, Light2).IsOk());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnInvTransferTest, "SpnCore.Inventory.TransfersAndRollback", SpnInvFlags)
bool FSpnInvTransferTest::RunTest(const FString& Parameters)
{
	FInvEnv Env;
	if (!TestTrue(TEXT("setup"), SetupInventory(Env)))
	{
		return false;
	}
	USpnInventorySubsystem& Inv = Env.Inv();
	const uint64 RationStack = Inv.AddItem(Body, Ration, 5).CreatedStacks[0];
	const uint64 LightStack = Inv.AddItem(Body, Flashlight).CreatedStacks[0];

	TestTrue(TEXT("a whole unique stack moves to the trunk"), Inv.TransferItem(Body, Trunk, LightStack).IsOk());
	TestEqual(TEXT("it keeps its id"), Inv.LocateStack(LightStack), Trunk);
	TestEqual(TEXT("and left the body"), Inv.CountOf(Body, Flashlight), uint64{0});

	TestTrue(TEXT("2 of 5 rations move"), Inv.TransferItem(Body, Trunk, RationStack, 2).IsOk());
	TestEqual(TEXT("3 stay"), Inv.CountOf(Body, Ration), uint64{3});
	TestEqual(TEXT("2 arrived"), Inv.CountOf(Trunk, Ration), uint64{2});
	TestTrue(TEXT("2 more merge into the matching stack"), Inv.TransferItem(Body, Trunk, RationStack, 2).IsOk());
	FSpnContainerView View;
	Inv.GetContainer(Trunk, View);
	int32 RationStacks = 0;
	for (const FSpnStackView& Stack : View.Stacks) { RationStacks += Stack.Definition == Ration ? 1 : 0; }
	TestEqual(TEXT("one ration stack of 4 in the trunk"), RationStacks, 1);

	// failures leave source and target untouched
	// fill the small box (3 stacks), then a transfer into the full box must fail and leave source and target untouched
	TestTrue(TEXT("fill the box"), Inv.AddItem(Box, Ration, 1).IsOk() && Inv.AddItem(Box, Bandage, 1).IsOk() && Inv.AddItem(Box, Notebook, 1).IsOk());
	const FString Describe = Inv.Describe();
	const uint64 Revision = Inv.GetRevision();
	const uint64 NextId = Inv.GetNextStackId();
	TestEqual(TEXT("a transfer into the full box hits its stack limit"), Inv.TransferItem(Trunk, Box, LightStack).Error, ESpnInventoryError::StackLimitReached);
	TestEqual(TEXT("the flashlight is still in the trunk"), Inv.LocateStack(LightStack), Trunk);
	TestEqual(TEXT("and nothing else changed"), Inv.Describe(), Describe);
	TestEqual(TEXT("same container"), Inv.TransferItem(Body, Body, RationStack).Error, ESpnInventoryError::SameContainer);
	TestEqual(TEXT("unknown target"), Inv.TransferItem(Body, 99, RationStack).Error, ESpnInventoryError::UnknownContainer);
	TestEqual(TEXT("more than the stack holds"), Inv.TransferItem(Body, Trunk, RationStack, 9).Error, ESpnInventoryError::InvalidQuantity);
	TestEqual(TEXT("the wrong source container"), Inv.TransferItem(Trunk, Body, 777).Error, ESpnInventoryError::UnknownItem);

	// atomic batch: the add and the first transfer succeed, the third command fails -> everything is undone
	const FSpnInventoryStatus Batch = Inv.Apply({
		FSpnInventoryCommand::AddItem(Body, Bandage, 4),
		FSpnInventoryCommand::TransferItem(Body, Trunk, RationStack, 1),
		FSpnInventoryCommand::TransferItem(Body, Trunk, 4242, 1),
	});
	TestEqual(TEXT("the batch fails on the unknown stack"), Batch.Error, ESpnInventoryError::UnknownItem);
	TestEqual(TEXT("at command 2"), Batch.FailedCommandIndex, 2);
	TestEqual(TEXT("the bandages were not added"), Inv.CountOf(Body, Bandage), uint64{0});
	TestEqual(TEXT("the first transfer was undone"), Inv.CountOf(Body, Ration), uint64{1});
	TestEqual(TEXT("the revision is unchanged"), Inv.GetRevision(), Revision);
	TestEqual(TEXT("no stack id was consumed by the failed batch"), Inv.GetNextStackId(), NextId);
	TestEqual(TEXT("the whole state is exactly as before the batch"), Inv.Describe(), Describe);
	TestEqual(TEXT("the failure is remembered"), Inv.GetLastFailure().FailedCommandIndex, 2);
	TestEqual(TEXT("and the Core's invariants hold"), Inv.CheckInvariants(), FString());

	// containers created in a failing batch do not exist afterwards
	const FSpnInventoryStatus Create = Inv.Apply({FSpnInventoryCommand::CreateContainer(50, FSpnContainerRulesData()), FSpnInventoryCommand::CreateContainer(50, FSpnContainerRulesData())});
	TestEqual(TEXT("creating the same container twice fails"), Create.Error, ESpnInventoryError::DuplicateContainer);
	TestFalse(TEXT("and the first creation was rolled back"), Inv.ListContainers().Contains(50));
	TestEqual(TEXT("a non-empty container cannot be destroyed"), Inv.DestroyContainer(Body).Error, ESpnInventoryError::ContainerNotEmpty);
	TestTrue(TEXT("an empty one can"), Inv.CreateContainer(51, FSpnContainerRulesData()).IsOk() && Inv.DestroyContainer(51).IsOk());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnInvInvalidTest, "SpnCore.Inventory.InvalidOperations", SpnInvFlags)
bool FSpnInvInvalidTest::RunTest(const FString& Parameters)
{
	FInvEnv Env;
	if (!TestTrue(TEXT("setup"), SetupInventory(Env)))
	{
		return false;
	}
	USpnInventorySubsystem& Inv = Env.Inv();
	const uint64 Stack = Inv.AddItem(Body, Ration, 2).CreatedStacks[0];
	const uint64 Revision = Inv.GetRevision();
	const FString Describe = Inv.Describe();

	struct FCase { const TCHAR* Name; FSpnInventoryStatus Status; ESpnInventoryError Expected; };
	const TArray<FCase> Cases = {
		{TEXT("container id 0"), Inv.AddItem(0, Ration, 1), ESpnInventoryError::InvalidArgument},
		{TEXT("stack id 0"), Inv.RemoveItem(Body, 0, 1), ESpnInventoryError::InvalidArgument},
		{TEXT("malformed definition id"), Inv.AddItem(Body, TEXT("Ration"), 1), ESpnInventoryError::InvalidArgument},
		{TEXT("malformed slot id"), Inv.Equip(Body, Stack, TEXT("Hand")), ESpnInventoryError::InvalidArgument},
		{TEXT("unknown preset"), Inv.CreateContainerFromPreset(60, TEXT("spn_test:preset.nope")), ESpnInventoryError::InvalidArgument},
		{TEXT("unknown definition"), Inv.AddItem(Body, TEXT("spn_test:item.nope"), 1), ESpnInventoryError::UnknownDefinition},
		{TEXT("unknown container"), Inv.AddItem(99, Ration, 1), ESpnInventoryError::UnknownContainer},
		{TEXT("duplicate container"), Inv.CreateContainerFromPreset(Body, TEXT("spn_test:preset.body")), ESpnInventoryError::DuplicateContainer},
		{TEXT("quantity 0"), Inv.AddItem(Body, Ration, 0), ESpnInventoryError::InvalidQuantity},
		{TEXT("removing more than present"), Inv.RemoveItem(Body, Stack, 3), ESpnInventoryError::InvalidQuantity},
		{TEXT("unknown stack"), Inv.RemoveItem(Body, 9999, 1), ESpnInventoryError::UnknownItem},
		{TEXT("splitting off the whole stack"), Inv.SplitStack(Body, Stack, 2), ESpnInventoryError::InvalidQuantity},
		{TEXT("merging a stack with itself (the Core reports InvalidId)"), Inv.MergeStacks(Body, Stack, Stack), ESpnInventoryError::InvalidId},
	};
	for (const FCase& Case : Cases)
	{
		TestEqual(FString::Printf(TEXT("%s -> %s"), Case.Name, LexToString(Case.Expected)), Case.Status.Error, Case.Expected);
		TestFalse(FString::Printf(TEXT("%s: a reason is given"), Case.Name), Case.Status.Detail.IsEmpty());
		TestEqual(FString::Printf(TEXT("%s: the revision in the status is unchanged"), Case.Name), Case.Status.Revision, Revision);
	}
	TestEqual(TEXT("no invalid operation changed the revision"), Inv.GetRevision(), Revision);
	TestEqual(TEXT("or anything else"), Inv.Describe(), Describe);
	TestEqual(TEXT("an empty transaction is a successful no-op"), Inv.Apply({}).IsOk() ? Inv.GetRevision() : 0, Revision);
	TestEqual(TEXT("the Core's invariants hold"), Inv.CheckInvariants(), FString());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnInvNotificationsTest, "SpnCore.Inventory.Notifications", SpnInvFlags)
bool FSpnInvNotificationsTest::RunTest(const FString& Parameters)
{
	FInvEnv Env;
	if (!TestTrue(TEXT("setup"), SetupInventory(Env)))
	{
		return false;
	}
	int32 Changed = 0;
	Env.Inv().OnInventoryChanged.AddLambda([&Changed]() { ++Changed; });
	Env.Inv().AddItem(Body, Ration, 1);
	TestEqual(TEXT("a successful transaction notifies once"), Changed, 1);
	Env.Inv().AddItem(Body, TEXT("spn_test:item.nope"), 1);
	TestEqual(TEXT("a failed one does not"), Changed, 1);
	Env.Inv().Apply({});
	TestEqual(TEXT("an empty one does not"), Changed, 1);
	Env.Inv().Apply({FSpnInventoryCommand::AddItem(Body, Bandage, 1), FSpnInventoryCommand::AddItem(Body, Bandage, 1)});
	TestEqual(TEXT("a batch notifies once"), Changed, 2);
	return true;
}

// === save integration ==========================================================================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnInvSaveRoundTripTest, "SpnCore.Inventory.SaveLoadRoundTrip", SpnInvFlags)
bool FSpnInvSaveRoundTripTest::RunTest(const FString& Parameters)
{
	FInvEnv Env;
	if (!TestTrue(TEXT("setup"), SetupInventory(Env)))
	{
		return false;
	}
	USpnInventorySubsystem& Inv = Env.Inv();
	Inv.AddItem(Body, Ration, 7);
	const uint64 Light = Inv.AddItem(Body, Flashlight, 1, {FSpnItemStateField::MakeInt(TEXT("charge"), 55), FSpnItemStateField::MakeString(TEXT("note"), TEXT("Grüße"))}).CreatedStacks[0];
	Inv.Equip(Body, Light, SlotHand);
	const uint64 CrateStack = Inv.AddItem(Trunk, Crate, 1).CreatedStacks[0];
	Inv.TransferItem(Body, Box, Inv.AddItem(Body, Bandage, 3).CreatedStacks[0], 1);
	const FString Saved = Inv.Describe();
	const uint64 SavedRevision = Inv.GetRevision();
	const uint64 SavedNext = Inv.GetNextStackId();

	const FSpnSaveStatus SaveStatus = Env.Save().SaveSlot(TestSlotId);
	TestTrue(FString::Printf(TEXT("save (%s)"), *SaveStatus.Detail), SaveStatus.IsOk());
	TestEqual(TEXT("the status carries the inventory revision"), SaveStatus.InventoryRevision, SavedRevision);
	const spn::core::SaveSlot Parsed = ParsedSlot(Env.File());
	TestNotNull(TEXT("player.inventory is in the slot"), Parsed.Find("player.inventory"));
	TestNotNull(TEXT("world.facts too"), Parsed.Find("world.facts"));
	if (const spn::core::SaveSection* Section = Parsed.Find("player.inventory"))
	{
		TestTrue(TEXT("the payload is the Core's versioned format SPN-INVENTORY 1"), Section->payload.rfind("SPN-INVENTORY 1\n", 0) == 0 && Section->version == 1);
	}

	// change everything, then restore
	Inv.RemoveItem(Body, 1, 5);
	Inv.Unequip(Body, SlotHand);
	Inv.RemoveItem(Trunk, CrateStack);
	Inv.AddItem(Body, Bandage, 2);
	TestTrue(TEXT("the inventory differs now"), Inv.Describe() != Saved);

	const FSpnSaveStatus Load = Env.Save().LoadSlot(TestSlotId);
	TestTrue(FString::Printf(TEXT("load (%s)"), *Load.Detail), Load.IsOk());
	TestTrue(TEXT("the inventory was restored"), Load.bInventoryRestored);
	TestFalse(TEXT("not reset"), Load.bInventoryResetToEmpty);
	TestEqual(TEXT("containers, stacks, quantities, equipment, state: identical"), Inv.Describe(), Saved);
	TestEqual(TEXT("revision restored"), Inv.GetRevision(), SavedRevision);
	TestEqual(TEXT("the id counter is restored: ids are not reused"), Inv.GetNextStackId(), SavedNext);
	TestEqual(TEXT("the Core's invariants hold"), Inv.CheckInvariants(), FString());
	FSpnContainerView View;
	Inv.GetContainer(Body, View);
	bool bNote = false;
	for (const FSpnStackView& Stack : View.Stacks)
	{
		for (const FSpnItemStateField& Field : Stack.State) { bNote |= Field.Name == TEXT("note") && Field.StringValue == TEXT("Grüße"); }
	}
	TestTrue(TEXT("non-ASCII state survives"), bNote);
	TestTrue(TEXT("the restored inventory keeps working"), Inv.AddItem(Body, Bandage, 1).IsOk());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnInvCombinedTest, "SpnCore.Inventory.FactsAndInventoryTogether", SpnInvFlags)
bool FSpnInvCombinedTest::RunTest(const FString& Parameters)
{
	FInvEnv Env;
	if (!TestTrue(TEXT("setup"), SetupInventory(Env)))
	{
		return false;
	}
	UseDoor(Env.World(), DoorA); // facts revision 1
	Env.Inv().AddItem(Body, Ration, 2);
	TestTrue(TEXT("save both"), Env.Save().SaveSlot(TestSlotId).IsOk());
	const TArray<uint8> Saved = Fingerprint(Env);
	const spn::core::SaveSlot Parsed = ParsedSlot(Env.File());
	TestEqual(TEXT("the slot has facts, inventory and meta"), static_cast<int32>(Parsed.Count()), 3);
	if (const spn::core::SaveSection* Meta = Parsed.Find("slot.meta"))
	{
		TestTrue(TEXT("slot.meta records both revisions"), Meta->payload.find("facts_revision 1") != std::string::npos && Meta->payload.find("inventory_revision ") != std::string::npos);
	}

	UseDoor(Env.World(), DoorA); // close again
	UseDoor(Env.World(), TEXT("door_b"));
	Env.Inv().AddItem(Body, Bandage, 4);
	Env.Inv().Equip(Body, Env.Inv().AddItem(Body, Coat).CreatedStacks[0], SlotTorso);
	TestFalse(TEXT("both domains changed"), Same(Fingerprint(Env), Saved));

	// every listener sees the COMPLETE loaded state, whichever callback runs first
	bool bFactsListenerSawInventory = false, bInventoryListenerSawFacts = false, bLoadedSawBoth = false;
	Env.World().OnWorldStateReplaced.AddLambda([&]() { bFactsListenerSawInventory = Env.Inv().CountOf(Body, Ration) == 2 && Env.Inv().CountOf(Body, Coat) == 0; });
	Env.Inv().OnInventoryReplaced.AddLambda([&]() { bInventoryListenerSawFacts = Env.World().GetRevision() == 1; });
	Env.Save().OnLoaded.AddLambda([&]() { bLoadedSawBoth = Env.World().GetRevision() == 1 && Env.Inv().CountOf(Body, Ration) == 2; });
	const FSpnSaveStatus Load = Env.Save().LoadSlot(TestSlotId);
	TestTrue(FString::Printf(TEXT("load (%s)"), *Load.Detail), Load.IsOk());
	TestTrue(TEXT("the facts listener already saw the restored inventory"), bFactsListenerSawInventory);
	TestTrue(TEXT("the inventory listener already saw the restored facts"), bInventoryListenerSawFacts);
	TestTrue(TEXT("OnLoaded saw both"), bLoadedSawBoth);
	TestTrue(TEXT("both domains are byte-identical to the saved state"), Same(Fingerprint(Env), Saved));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnInvCorruptTest, "SpnCore.Inventory.CorruptInventoryPayload", SpnInvFlags)
bool FSpnInvCorruptTest::RunTest(const FString& Parameters)
{
	FInvEnv Env;
	if (!TestTrue(TEXT("setup"), SetupInventory(Env)))
	{
		return false;
	}
	UseDoor(Env.World(), DoorA);
	Env.Inv().AddItem(Body, Ration, 2);
	TestTrue(TEXT("save"), Env.Save().SaveSlot(TestSlotId).IsOk());
	spn::core::SaveSlot Good = ParsedSlot(Env.File());

	// the state we must keep: facts AND inventory differ from the file
	UseDoor(Env.World(), TEXT("door_b"));
	Env.Inv().AddItem(Body, Bandage, 3);
	const TArray<uint8> Before = Fingerprint(Env);
	int32 Replaced = 0;
	Env.World().OnWorldStateReplaced.AddLambda([&Replaced]() { ++Replaced; });
	Env.Inv().OnInventoryReplaced.AddLambda([&Replaced]() { ++Replaced; });

	const auto Check = [&](const TCHAR* Name, const spn::core::SaveSlot& Slot, ESpnSaveResult Expected)
	{
		WriteSlot(Env.File(), Slot);
		IFileManager::Get().Delete(*Env.Prev());
		const FSpnSaveStatus Status = Env.Save().LoadSlot(TestSlotId);
		TestEqual(FString::Printf(TEXT("%s: result"), Name), Status.Result, Expected);
		TestFalse(FString::Printf(TEXT("%s: a reason is given"), Name), Status.Detail.IsEmpty());
		TestTrue(FString::Printf(TEXT("%s: facts AND inventory are byte-identical to before"), Name), Same(Fingerprint(Env), Before));
		TestEqual(FString::Printf(TEXT("%s: no listener ran"), Name), Replaced, 0);
	};

	// valid facts, damaged inventory payloads (the container re-checksums them, only the domain parser can notice)
	for (const char* Payload : {"garbage\n", "SPN-INVENTORY 1\nnext 5\nrevision 1\ncontainers 1\nend\n", "SPN-INVENTORY 9\nend\n", ""})
	{
		spn::core::SaveSlot Bad = Good;
		(void)Bad.Put(spn::core::SaveSection{"player.inventory", 1, false, Payload});
		Check(*FString::Printf(TEXT("inventory payload '%hs'"), std::string(Payload).substr(0, 18).c_str()), Bad, ESpnSaveResult::Rejected);
	}
	// an inventory that names an unknown item definition (catalog mismatch)
	{
		std::string Payload = Good.Find("player.inventory")->payload;
		const std::size_t At = Payload.find("spn_test:item.ration");
		if (TestTrue(TEXT("the payload contains the definition id"), At != std::string::npos))
		{
			Payload.replace(At, 20, "spn_test:item.xxxxxx");
			spn::core::SaveSlot Bad = Good;
			(void)Bad.Put(spn::core::SaveSection{"player.inventory", 1, false, Payload});
			Check(TEXT("unknown item definition"), Bad, ESpnSaveResult::Rejected);
		}
	}
	// the inventory is from another moment than the facts
	{
		spn::core::SaveSlot Bad = Good;
		(void)Bad.Put(spn::core::SaveSection{"slot.meta", 1, false, "facts_revision 1\ninventory_revision 99\n"});
		Check(TEXT("slot.meta with a foreign inventory revision"), Bad, ESpnSaveResult::Rejected);
		spn::core::SaveSlot NoRevision = Good;
		(void)NoRevision.Put(spn::core::SaveSection{"slot.meta", 1, false, "facts_revision 1\n"});
		Check(TEXT("slot.meta without the inventory revision"), NoRevision, ESpnSaveResult::Rejected);
	}
	// a flipped bit anywhere
	{
		TArray<uint8> Bytes = ToBytes(Good.Serialize());
		Bytes[Bytes.Num() / 2] ^= 1;
		WriteBytes(Env.File(), Bytes);
		IFileManager::Get().Delete(*Env.Prev());
		const FSpnSaveStatus Status = Env.Save().LoadSlot(TestSlotId);
		TestEqual(TEXT("a flipped bit: Corrupt"), Status.Result, ESpnSaveResult::Corrupt);
		TestTrue(TEXT("a flipped bit: nothing changed"), Same(Fingerprint(Env), Before));
	}
	TestEqual(TEXT("the Core's invariants hold"), Env.Inv().CheckInvariants(), FString());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnInvMissingSectionTest, "SpnCore.Inventory.MissingInventorySection", SpnInvFlags)
bool FSpnInvMissingSectionTest::RunTest(const FString& Parameters)
{
	FInvEnv Env;
	// 1. a WorldFacts-only save (inventory inactive, exactly like an M2B3.2 save)
	UseDoor(Env.World(), DoorA);
	TestTrue(TEXT("save without an inventory catalog"), Env.Save().SaveSlot(TestSlotId).IsOk());
	spn::core::SaveSlot FactsOnly = ParsedSlot(Env.File());
	TestEqual(TEXT("it holds world.facts and slot.meta only"), static_cast<int32>(FactsOnly.Count()), 2);
	TestNull(TEXT("no inventory section"), FactsOnly.Find("player.inventory"));
	TestEqual(TEXT("the status says no inventory was saved"), Env.Save().SaveSlot(TestSlotId).InventoryRevision, uint64{0});

	// 2. loading it with an inactive inventory is as before
	UseDoor(Env.World(), TEXT("door_b"));
	FSpnSaveStatus Load = Env.Save().LoadSlot(TestSlotId);
	TestTrue(TEXT("a WorldFacts-only save loads with an inactive inventory"), Load.IsOk());
	TestFalse(TEXT("without touching an inventory"), Load.bInventoryRestored || Load.bInventoryResetToEmpty);

	// 3. loading it with an ACTIVE inventory: the facts are restored, the inventory starts fresh and the status says so
	if (!TestTrue(TEXT("setup"), SetupInventory(Env)))
	{
		return false;
	}
	Env.Inv().AddItem(Body, Ration, 3);
	UseDoor(Env.World(), TEXT("door_b"));
	Load = Env.Save().LoadSlot(TestSlotId);
	TestTrue(FString::Printf(TEXT("a WorldFacts-only save loads with an active inventory (%s)"), *Load.Detail), Load.IsOk());
	TestEqual(TEXT("the facts are restored"), Env.World().GetRevision(), uint64{1});
	TestFalse(TEXT("no inventory was restored"), Load.bInventoryRestored);
	TestTrue(TEXT("the inventory was reset and the status says so"), Load.bInventoryResetToEmpty);
	TestEqual(TEXT("the inventory is empty: no containers"), Env.Inv().ListContainers().Num(), 0);
	TestEqual(TEXT("revision 0"), Env.Inv().GetRevision(), uint64{0});
	TestTrue(TEXT("and usable again"), Env.Inv().CreateContainerFromPreset(Body, TEXT("spn_test:preset.body")).IsOk());

	// 4. a save with an active inventory contains it; the slot written before stays loadable
	TestTrue(TEXT("save with the active inventory"), Env.Save().SaveSlot(TEXT("slot_two")).IsOk());
	TestNotNull(TEXT("player.inventory is written even for an empty inventory"), ParsedSlot(Env.File(TEXT("slot_two"))).Find("player.inventory"));
	TestTrue(TEXT("the older facts-only slot still loads"), Env.Save().LoadSlot(TestSlotId).IsOk());

	// 5. a slot WITH an inventory cannot be loaded without a catalog: it is refused, not half-restored
	TestTrue(TEXT("the slot with inventory loads while active"), Env.Save().LoadSlot(TEXT("slot_two")).bInventoryRestored);
	Env.Inv().ClearCatalog();
	UseDoor(Env.World(), TEXT("door_c"));
	const TArray<uint8> Before = Fingerprint(Env);
	const FSpnSaveStatus Refused = Env.Save().LoadSlot(TEXT("slot_two"));
	TestEqual(TEXT("without a catalog the inventory section blocks the load"), Refused.Result, ESpnSaveResult::SectionsNotIntegrated);
	TestTrue(TEXT("and nothing changed"), Same(Fingerprint(Env), Before));
	TArray<uint8> OnDisk, After;
	ReadBytes(Env.File(TEXT("slot_two")), OnDisk);
	TestEqual(TEXT("saving over it is refused too"), Env.Save().SaveSlot(TEXT("slot_two")).Result, ESpnSaveResult::SectionsNotIntegrated);
	ReadBytes(Env.File(TEXT("slot_two")), After);
	TestTrue(TEXT("the file is untouched"), OnDisk == After);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnInvSectionsTest, "SpnCore.Inventory.UnknownAndUnsupportedSections", SpnInvFlags)
bool FSpnInvSectionsTest::RunTest(const FString& Parameters)
{
	FInvEnv Env;
	if (!TestTrue(TEXT("setup"), SetupInventory(Env)))
	{
		return false;
	}
	UseDoor(Env.World(), DoorA);
	Env.Inv().AddItem(Body, Ration, 2);
	TestTrue(TEXT("save"), Env.Save().SaveSlot(TestSlotId).IsOk());
	const spn::core::SaveSlot Good = ParsedSlot(Env.File());

	// unknown optional sections survive a load and a save next to the inventory
	{
		spn::core::SaveSlot Future = Good;
		(void)Future.Put(spn::core::SaveSection{"future.cosmetics", 3, false, "future payload"});
		WriteSlot(Env.File(), Future);
		TestTrue(TEXT("a slot with an unknown optional section loads"), Env.Save().LoadSlot(TestSlotId).IsOk());
		Env.Inv().AddItem(Body, Bandage, 1);
		const FSpnSaveStatus Saved = Env.Save().SaveSlot(TestSlotId);
		TestTrue(TEXT("and saves again"), Saved.IsOk() && Saved.PreservedSections.Contains(TEXT("future.cosmetics")));
		const spn::core::SaveSlot After = ParsedSlot(Env.File());
		const spn::core::SaveSection* Kept = After.Find("future.cosmetics");
		TestTrue(TEXT("the unknown section is byte-identical"), Kept != nullptr && Kept->payload == "future payload" && Kept->version == 3);
		TestNotNull(TEXT("the inventory is there too"), After.Find("player.inventory"));
	}

	// sections without an owner (map, objects) still block, even with an active inventory
	for (const char* Section : {"world.map", "world.objects"})
	{
		spn::core::SaveSlot WithSection = Good;
		(void)WithSection.Put(spn::core::SaveSection{Section, 1, false, "payload of a domain without an Unreal owner"});
		WriteSlot(Env.File(), WithSection);
		TArray<uint8> OnDisk, After;
		ReadBytes(Env.File(), OnDisk);
		const TArray<uint8> Before = Fingerprint(Env);
		TestEqual(FString::Printf(TEXT("%hs blocks the load"), Section), Env.Save().LoadSlot(TestSlotId).Result, ESpnSaveResult::SectionsNotIntegrated);
		TestEqual(FString::Printf(TEXT("%hs blocks the save"), Section), Env.Save().SaveSlot(TestSlotId).Result, ESpnSaveResult::SectionsNotIntegrated);
		ReadBytes(Env.File(), After);
		TestTrue(TEXT("file untouched"), OnDisk == After);
		TestTrue(TEXT("state untouched"), Same(Fingerprint(Env), Before));
	}

	// a NEWER version of a known section is never read as "missing" and never overwritten
	for (const char* Section : {"player.inventory", "slot.meta"})
	{
		spn::core::SaveSlot Newer = Good;
		const spn::core::SaveSection* Old = Good.Find(Section);
		(void)Newer.Put(spn::core::SaveSection{Section, 2, false, Old->payload});
		WriteSlot(Env.File(), Newer);
		IFileManager::Get().Delete(*Env.Prev());
		TArray<uint8> OnDisk, After;
		ReadBytes(Env.File(), OnDisk);
		const TArray<uint8> Before = Fingerprint(Env);
		const FSpnSaveStatus Load = Env.Save().LoadSlot(TestSlotId);
		TestEqual(FString::Printf(TEXT("%hs version 2: load is UnsupportedVersion"), Section), Load.Result, ESpnSaveResult::UnsupportedVersion);
		TestFalse(TEXT("the inventory was not silently reset"), Load.bInventoryResetToEmpty);
		TestEqual(FString::Printf(TEXT("%hs version 2: save is UnsupportedVersion"), Section), Env.Save().SaveSlot(TestSlotId).Result, ESpnSaveResult::UnsupportedVersion);
		ReadBytes(Env.File(), After);
		TestTrue(TEXT("file untouched"), OnDisk == After);
		TestTrue(TEXT("state untouched"), Same(Fingerprint(Env), Before));
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnInvPreviousGenerationTest, "SpnCore.Inventory.PreviousGenerationWithInventory", SpnInvFlags)
bool FSpnInvPreviousGenerationTest::RunTest(const FString& Parameters)
{
	FInvEnv Env;
	if (!TestTrue(TEXT("setup"), SetupInventory(Env)))
	{
		return false;
	}
	UseDoor(Env.World(), DoorA);
	Env.Inv().AddItem(Body, Ration, 1);
	TestTrue(TEXT("save generation 1"), Env.Save().SaveSlot(TestSlotId).IsOk());
	const TArray<uint8> Generation1 = Fingerprint(Env);
	UseDoor(Env.World(), TEXT("door_b"));
	Env.Inv().AddItem(Body, Ration, 2);
	TestTrue(TEXT("save generation 2"), Env.Save().SaveSlot(TestSlotId).IsOk());

	// the main file is checksum-valid but its inventory is rejected -> the previous generation is restored as a whole (facts AND inventory)
	spn::core::SaveSlot Bad = ParsedSlot(Env.File());
	(void)Bad.Put(spn::core::SaveSection{"player.inventory", 1, false, "garbage\n"});
	WriteSlot(Env.File(), Bad);
	UseDoor(Env.World(), TEXT("door_c"));
	Env.Inv().AddItem(Body, Bandage, 5);
	const FSpnSaveStatus Status = Env.Save().LoadSlot(TestSlotId);
	TestTrue(FString::Printf(TEXT("load falls back (%s)"), *Status.Detail), Status.IsOk());
	TestEqual(TEXT("from the previous generation"), Status.Source, ESpnSaveSource::Previous);
	TestFalse(TEXT("and says why"), Status.CurrentProblem.IsEmpty());
	TestTrue(TEXT("generation 1 is live: facts and inventory together"), Same(Fingerprint(Env), Generation1));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnInvRepeatedTest, "SpnCore.Inventory.RepeatedSaveLoadCycles", SpnInvFlags)
bool FSpnInvRepeatedTest::RunTest(const FString& Parameters)
{
	FInvEnv Env;
	if (!TestTrue(TEXT("setup"), SetupInventory(Env)))
	{
		return false;
	}
	uint64 LastNext = Env.Inv().GetNextStackId();
	for (int32 Cycle = 1; Cycle <= 20; ++Cycle)
	{
		UseDoor(Env.World(), DoorA);
		Env.Inv().AddItem(Body, Bandage, static_cast<uint32>(1 + Cycle % 3));
		if (Cycle % 4 == 0)
		{
			FSpnContainerView View;
			Env.Inv().GetContainer(Body, View);
			Env.Inv().TransferItem(Body, Trunk, View.Stacks[0].StackId, 1);
		}
		const TArray<uint8> Saved = Fingerprint(Env);
		const FSpnSaveStatus SaveStatus = Env.Save().SaveSlot(TestSlotId);
		if (!TestTrue(FString::Printf(TEXT("cycle %d: save (%s)"), Cycle, *SaveStatus.Detail), SaveStatus.IsOk()))
		{
			return false;
		}
		UseDoor(Env.World(), TEXT("door_b"));
		Env.Inv().AddItem(Body, Ration, 1); // diverge
		const FSpnSaveStatus Load = Env.Save().LoadSlot(TestSlotId);
		if (!TestTrue(FString::Printf(TEXT("cycle %d: load (%s)"), Cycle, *Load.Detail), Load.IsOk() && Load.bInventoryRestored))
		{
			return false;
		}
		TestTrue(FString::Printf(TEXT("cycle %d: both domains identical to the saved state"), Cycle), Same(Fingerprint(Env), Saved));
		TestEqual(FString::Printf(TEXT("cycle %d: invariants"), Cycle), Env.Inv().CheckInvariants(), FString());
		TestTrue(FString::Printf(TEXT("cycle %d: stack ids only grow"), Cycle), Env.Inv().GetNextStackId() >= LastNext);
		LastNext = Env.Inv().GetNextStackId();
	}
	TArray<FString> Files;
	IFileManager::Get().FindFiles(Files, *(Env.File().LeftChop(FString(TEXT("game.spnsave")).Len()) / TEXT("*")), true, false);
	TestEqual(TEXT("current + previous only"), Files.Num(), 2);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnInvReentrancyTest, "SpnCore.Inventory.CallbacksCannotReenterSaveOrLoad", SpnInvFlags)
bool FSpnInvReentrancyTest::RunTest(const FString& Parameters)
{
	FInvEnv Env;
	if (!TestTrue(TEXT("setup"), SetupInventory(Env)))
	{
		return false;
	}
	Env.Inv().AddItem(Body, Ration, 2);
	TestTrue(TEXT("save"), Env.Save().SaveSlot(TestSlotId).IsOk());
	const TArray<uint8> Saved = Fingerprint(Env);
	Env.Inv().AddItem(Body, Ration, 1);

	ESpnSaveResult NestedLoad = ESpnSaveResult::Ok, NestedSave = ESpnSaveResult::Ok;
	Env.World().OnWorldStateReplaced.AddLambda([&]()
	{
		NestedLoad = Env.Save().LoadSlot(TestSlotId).Result;
		NestedSave = Env.Save().SaveSlot(TEXT("nested")).Result;
	});
	int32 SaveCallbackNested = 0;
	Env.Save().OnSaved.AddLambda([&]() { SaveCallbackNested = Env.Save().SaveSlot(TEXT("nested2")).Result == ESpnSaveResult::Busy ? 1 : 2; });

	TestTrue(TEXT("the outer load succeeds"), Env.Save().LoadSlot(TestSlotId).IsOk());
	TestEqual(TEXT("a load from inside the callback is refused with Busy"), NestedLoad, ESpnSaveResult::Busy);
	TestEqual(TEXT("a save from inside the callback is refused with Busy"), NestedSave, ESpnSaveResult::Busy);
	TestFalse(TEXT("no nested slot was written"), Env.Save().DoesSlotExist(TEXT("nested")));
	TestTrue(TEXT("the loaded state is intact"), Same(Fingerprint(Env), Saved));
	TestTrue(TEXT("after the callbacks the subsystem is usable again"), Env.Save().SaveSlot(TestSlotId).IsOk());
	TestEqual(TEXT("a save from inside OnSaved is refused too"), SaveCallbackNested, 1);

	// a listener may use the (fully loaded) inventory
	Env.Inv().OnInventoryReplaced.AddLambda([&]() { Env.Inv().AddItem(Body, Bandage, 1); });
	Env.Inv().AddItem(Body, Ration, 1);
	TestTrue(TEXT("load with a listener that adds an item afterwards"), Env.Save().LoadSlot(TestSlotId).IsOk());
	TestEqual(TEXT("the listener's change is applied on top of the complete state"), Env.Inv().CountOf(Body, Bandage), uint64{1});
	TestEqual(TEXT("the loaded rations are exact"), Env.Inv().CountOf(Body, Ration), uint64{2});
	return true;
}
#endif
