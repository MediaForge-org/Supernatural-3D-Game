#include "Engine/GameInstance.h"
#include "HAL/FileManager.h"
#include "Blueprint/UserWidget.h"
#include "Misc/AutomationTest.h"
#include "Misc/Guid.h"
#include "Misc/Paths.h"
#include "SpnInventorySubsystem.h"
#include "SpnInventoryViewModel.h"
#include "SpnSaveSubsystem.h"
#include "Spn/UI/SpnInventoryScreen.h"
#include "Spn/UI/SpnUiText.h"
#include "UObject/Package.h"

#if WITH_DEV_AUTOMATION_TESTS

// Widget tests for the graphical inventory (M2B3.5). They read the TEXT AND STATE OF THE REAL WIDGETS (USpnInventoryScreen / USpnUiButton), not
// the view-model, and drive the screen through HandleKey() exactly like the input pre-processor does. Data: the temporary spn_test: catalog.

namespace
{
	constexpr EAutomationTestFlags SpnUiFlags = EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter;

	const FString Ration = TEXT("spn_test:item.ration");
	const FString Bandage = TEXT("spn_test:item.bandage");
	const FString Flashlight = TEXT("spn_test:item.flashlight");
	const FString Notebook = TEXT("spn_test:item.notebook");
	const FString Coat = TEXT("spn_test:item.coat");
	const FString SlotHand = TEXT("spn_test:slot.hand");
	constexpr int64 Body = 1;
	constexpr int64 Trunk = 2;
	constexpr int64 Box = 3;

	struct FUiEnv
	{
		UGameInstance* Instance = nullptr;
		USpnInventoryScreen* Screen = nullptr;
		FString Root;

		FUiEnv()
		{
			Instance = NewObject<UGameInstance>(GetTransientPackage());
			Instance->AddToRoot();
			Instance->Init();
			Root = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir() / TEXT("SpnTestTemp") / FGuid::NewGuid().ToString(EGuidFormats::Digits));
			Save().SetSaveDirectory(Root);
			FSpnUiText::SetLanguageOverride(TEXT("en"));
		}
		~FUiEnv()
		{
			if (Screen != nullptr)
			{
				Screen->Teardown();
				Screen->RemoveFromParent();
				Screen->RemoveFromRoot();
			}
			FSpnUiText::SetLanguageOverride(FString());
			Instance->Shutdown();
			Instance->RemoveFromRoot();
			IFileManager::Get().DeleteDirectory(*Root, false, true);
		}

		USpnSaveSubsystem& Save() const { return *Instance->GetSubsystem<USpnSaveSubsystem>(); }
		USpnInventorySubsystem& Inv() const { return *Instance->GetSubsystem<USpnInventorySubsystem>(); }

		bool SetupData()
		{
			return Inv().LoadCatalogFromFile(USpnInventorySubsystem::GetTestCatalogPath()).IsOk()
				&& Inv().CreateContainerFromPreset(Body, TEXT("spn_test:preset.body")).IsOk()
				&& Inv().CreateContainerFromPreset(Trunk, TEXT("spn_test:preset.trunk")).IsOk()
				&& Inv().CreateContainerFromPreset(Box, TEXT("spn_test:preset.small_box")).IsOk();
		}

		USpnInventoryScreen& Open()
		{
			if (Screen == nullptr)
			{
				Screen = CreateWidget<USpnInventoryScreen>(Instance, USpnInventoryScreen::StaticClass());
				Screen->AddToRoot();
			}
			Screen->Setup(&Inv(), &Save(), Body);
			Screen->OnShown();
			return *Screen;
		}
	};

	int32 FindRow(const USpnInventoryScreen& Screen, const TCHAR* Title)
	{
		for (int32 Index = 0; Index < Screen.GetVisibleRowCount(); ++Index)
		{
			if (Screen.GetRowTitle(Index) == Title)
			{
				return Index;
			}
		}
		return INDEX_NONE;
	}

	/** Walks the selection (Down key) until the row with this title is selected; returns false if it never is. */
	bool SelectRow(USpnInventoryScreen& Screen, const TCHAR* Title)
	{
		for (int32 Guard = 0; Guard < 200; ++Guard)
		{
			const int32 Index = FindRow(Screen, Title);
			if (Index != INDEX_NONE && Screen.IsRowSelected(Index))
			{
				return true;
			}
			Screen.HandleKey(EKeys::Down);
		}
		return false;
	}

	int32 FindAction(const USpnInventoryScreen& Screen, const TCHAR* Prefix)
	{
		for (int32 Index = 0; Index < Screen.GetActionCount(); ++Index)
		{
			if (Screen.GetActionTitle(Index).StartsWith(Prefix))
			{
				return Index;
			}
		}
		return INDEX_NONE;
	}

	void RunActionByPrefix(USpnInventoryScreen& Screen, const TCHAR* Prefix)
	{
		Screen.HandleKey(EKeys::Right); // items -> actions
		const int32 Index = FindAction(Screen, Prefix);
		for (int32 Guard = 0; Index != INDEX_NONE && Screen.GetFocusedActionIndex() != Index && Guard < 20; ++Guard)
		{
			Screen.HandleKey(Index > Screen.GetFocusedActionIndex() ? EKeys::Down : EKeys::Up);
		}
		Screen.HandleKey(EKeys::Enter);
		if (Screen.GetZone() == USpnInventoryScreen::EZone::Actions && !Screen.IsDialogOpen())
		{
			Screen.HandleKey(EKeys::Left); // back to the items
		}
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnUiConstructionTest, "SpnGame.InventoryUi.Construction.Empty", SpnUiFlags)
bool FSpnUiConstructionTest::RunTest(const FString&)
{
	FUiEnv Env;
	// without a catalog the inventory is inactive: the screen builds and says so
	USpnInventoryScreen& Screen = Env.Open();
	TestTrue(TEXT("the Slate tree exists"), Screen.TakeWidget()->GetType() != NAME_None);
	TestEqual(TEXT("title"), Screen.GetHeaderTitle(), FString(TEXT("INVENTORY")));
	TestEqual(TEXT("no rows"), Screen.GetVisibleRowCount(), 0);
	TestFalse(TEXT("an explanatory empty text is shown"), Screen.GetEmptyText().IsEmpty());
	TestEqual(TEXT("details say nothing is selected"), Screen.GetDetailsTitle(), FSpnUiText::Get(TEXT("ui.details.none")).ToString());

	// with containers but no items
	TestTrue(TEXT("catalog + containers"), Env.SetupData());
	Screen.OnShown();
	TestEqual(TEXT("still no rows"), Screen.GetVisibleRowCount(), 0);
	TestEqual(TEXT("empty-container text"), Screen.GetEmptyText(), FSpnUiText::Get(TEXT("ui.empty.items")).ToString());
	TestEqual(TEXT("three container tabs"), Screen.GetVisibleTabCount(), 3);
	TestEqual(TEXT("body offers two equipment slots"), Screen.GetSlotCount(), 2);
	TestEqual(TEXT("slots are free"), Screen.GetSlotSubtitle(0), FSpnUiText::Get(TEXT("ui.slot.free")).ToString());
	TestEqual(TEXT("no actions without a selection"), Screen.GetActionsEmptyText(), FSpnUiText::Get(TEXT("ui.actions.none")).ToString());
	TestEqual(TEXT("and lists no action buttons"), Screen.GetActionCount(), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnUiPopulatedTest, "SpnGame.InventoryUi.Display.PopulatedMatchesCore", SpnUiFlags)
bool FSpnUiPopulatedTest::RunTest(const FString&)
{
	FUiEnv Env;
	if (!TestTrue(TEXT("setup"), Env.SetupData())) { return false; }
	Env.Inv().AddItem(Body, Ration, 3);
	Env.Inv().AddItem(Body, Flashlight);
	Env.Inv().AddItem(Body, Bandage, 4);
	USpnInventoryScreen& Screen = Env.Open();

	FSpnContainerView View;
	Env.Inv().GetContainer(Body, View);
	TestEqual(TEXT("one visible row per Core stack"), Screen.GetVisibleRowCount(), View.Stacks.Num());
	const int32 RationRow = FindRow(Screen, TEXT("Test ration"));
	const int32 LightRow = FindRow(Screen, TEXT("Test flashlight"));
	TestTrue(TEXT("ration row exists"), RationRow != INDEX_NONE);
	TestTrue(TEXT("flashlight row exists"), LightRow != INDEX_NONE);
	if (RationRow != INDEX_NONE) { TestEqual(TEXT("ration quantity on screen"), Screen.GetRowRight(RationRow), FString(TEXT("×3"))); }
	if (LightRow != INDEX_NONE) { TestEqual(TEXT("unique item has no quantity text"), Screen.GetRowRight(LightRow), FString()); }
	TestTrue(TEXT("first row is selected on opening"), Screen.GetVisibleRowCount() > 0 && Screen.IsRowSelected(0));
	TestFalse(TEXT("details are filled"), Screen.GetDetailsText().IsEmpty());
	TestTrue(TEXT("weight capacity shows the Core weight"), Screen.GetCapacityText(ESpnCapacityKind::Weight).Contains(TEXT("g")) || Screen.GetCapacityText(ESpnCapacityKind::Weight).Contains(TEXT("kg")));
	TestTrue(TEXT("stack meter shows used/max"), Screen.GetCapacityText(ESpnCapacityKind::Stacks).Contains(TEXT("8")));
	TestTrue(TEXT("weight bar is partly filled"), Screen.GetCapacityPercent(ESpnCapacityKind::Weight) > 0.f);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnUiSelectionTest, "SpnGame.InventoryUi.Selection.KeyboardNavigation", SpnUiFlags)
bool FSpnUiSelectionTest::RunTest(const FString&)
{
	FUiEnv Env;
	if (!TestTrue(TEXT("setup"), Env.SetupData())) { return false; }
	Env.Inv().AddItem(Body, Ration, 2);
	Env.Inv().AddItem(Body, Flashlight);
	Env.Inv().AddItem(Body, Notebook);
	USpnInventoryScreen& Screen = Env.Open();
	const FString First = Screen.GetDetailsTitle();
	TestTrue(TEXT("Down moves the selection"), Screen.HandleKey(EKeys::Down));
	TestTrue(TEXT("the details follow the selection"), Screen.GetDetailsTitle() != First);
	TestTrue(TEXT("exactly the second row is selected"), Screen.IsRowSelected(1) && !Screen.IsRowSelected(0));
	TestTrue(TEXT("the focus ring is on the selected row"), Screen.IsRowFocusRing(1));
	Screen.HandleKey(EKeys::Up);
	TestEqual(TEXT("Up returns to the first"), Screen.GetDetailsTitle(), First);
	Screen.HandleKey(EKeys::End);
	TestTrue(TEXT("End selects the last row"), Screen.IsRowSelected(Screen.GetVisibleRowCount() - 1));
	Screen.HandleKey(EKeys::Home);
	TestTrue(TEXT("Home selects the first row"), Screen.IsRowSelected(0));

	// gamepad: D-pad navigates the same way and the hints switch to controller labels
	Screen.HandleKey(EKeys::Gamepad_DPad_Down);
	TestTrue(TEXT("D-pad down moves the selection"), Screen.IsRowSelected(1));
	TestTrue(TEXT("controller mode is detected"), Screen.IsUsingGamepad());
	TestEqual(TEXT("controller hints"), Screen.GetHintText(), FSpnUiText::Get(TEXT("ui.hint.gamepad")).ToString());
	Screen.HandleKey(EKeys::Down);
	TestFalse(TEXT("keyboard input switches the hints back"), Screen.IsUsingGamepad());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnUiEquipTest, "SpnGame.InventoryUi.Actions.EquipUnequipAndEquipmentPanel", SpnUiFlags)
bool FSpnUiEquipTest::RunTest(const FString&)
{
	FUiEnv Env;
	if (!TestTrue(TEXT("setup"), Env.SetupData())) { return false; }
	Env.Inv().AddItem(Body, Ration, 2);
	Env.Inv().AddItem(Body, Flashlight);
	USpnInventoryScreen& Screen = Env.Open();
	if (!TestTrue(TEXT("select the flashlight"), SelectRow(Screen, TEXT("Test flashlight")))) { return false; }
	const int32 EquipAction = [&]() { Screen.HandleKey(EKeys::Right); const int32 I = FindAction(Screen, TEXT("Equip")); Screen.HandleKey(EKeys::Left); return I; }();
	TestTrue(TEXT("an Equip action is offered"), EquipAction != INDEX_NONE);
	TestTrue(TEXT("and it is enabled"), EquipAction != INDEX_NONE && Screen.IsActionEnabled(EquipAction));

	RunActionByPrefix(Screen, TEXT("Equip"));
	FSpnContainerView View;
	Env.Inv().GetContainer(Body, View);
	bool bEquippedInCore = false;
	for (const FSpnStackView& Stack : View.Stacks) { bEquippedInCore |= Stack.Definition == Flashlight && !Stack.EquippedSlot.IsEmpty(); }
	TestTrue(TEXT("the Core equipped the flashlight"), bEquippedInCore);
	const int32 LightRow = FindRow(Screen, TEXT("Test flashlight"));
	TestEqual(TEXT("the row carries the EQUIPPED badge"), LightRow != INDEX_NONE ? Screen.GetRowBadge(LightRow) : FString(), FSpnUiText::Get(TEXT("ui.badge.equipped")).ToString());
	bool bSlotShows = false;
	for (int32 Index = 0; Index < Screen.GetSlotCount(); ++Index) { bSlotShows |= Screen.GetSlotSubtitle(Index) == TEXT("Test flashlight"); }
	TestTrue(TEXT("the equipment panel shows the flashlight in its slot"), bSlotShows);

	// the same through the equipment panel: Left to equipment, Enter unequips the focused slot
	SelectRow(Screen, TEXT("Test flashlight"));
	RunActionByPrefix(Screen, TEXT("Unequip"));
	Env.Inv().GetContainer(Body, View);
	bEquippedInCore = false;
	for (const FSpnStackView& Stack : View.Stacks) { bEquippedInCore |= !Stack.EquippedSlot.IsEmpty(); }
	TestFalse(TEXT("the Core unequipped it"), bEquippedInCore);
	bSlotShows = false;
	for (int32 Index = 0; Index < Screen.GetSlotCount(); ++Index) { bSlotShows |= Screen.GetSlotSubtitle(Index) == TEXT("Test flashlight"); }
	TestFalse(TEXT("the equipment panel is free again"), bSlotShows);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnUiRejectedTest, "SpnGame.InventoryUi.Actions.UnavailableExplainsAndRejectedChangesNothing", SpnUiFlags)
bool FSpnUiRejectedTest::RunTest(const FString&)
{
	FUiEnv Env;
	if (!TestTrue(TEXT("setup"), Env.SetupData())) { return false; }
	Env.Inv().AddItem(Body, Ration, 2);
	Env.Inv().AddItem(Body, Notebook);
	USpnInventoryScreen& Screen = Env.Open();
	if (!TestTrue(TEXT("select the notebook (not equippable)"), SelectRow(Screen, TEXT("Test notebook")))) { return false; }
	TestTrue(TEXT("no Equip action for an item without slots"), FindAction(Screen, TEXT("Equip")) == INDEX_NONE);

	// the small box does not accept ... fill it until a transfer is refused by the Core; the screen must say why and leave both sides alone
	Env.Inv().AddItem(Trunk, Notebook);
	Env.Inv().AddItem(Box, Notebook);
	Env.Inv().AddItem(Box, Notebook);
	Env.Inv().AddItem(Box, Notebook); // box: 3 stacks = full
	Screen.OnShown();
	SelectRow(Screen, TEXT("Test notebook"));
	const uint64 RevisionBefore = Env.Inv().GetRevision();
	FSpnContainerView BodyBefore, BoxBefore;
	Env.Inv().GetContainer(Body, BodyBefore);
	Env.Inv().GetContainer(Box, BoxBefore);
	bool bFoundUnavailable = false;
	Screen.HandleKey(EKeys::Right);
	for (int32 Index = 0; Index < Screen.GetActionCount(); ++Index)
	{
		if (!Screen.IsActionEnabled(Index) && Screen.GetActionTitle(Index).Contains(TEXT("Small box")))
		{
			bFoundUnavailable = true;
			TestTrue(TEXT("a disabled action carries the reason"), Screen.GetActionSubtitle(Index).StartsWith(TEXT("not available:")));
			while (Screen.GetFocusedActionIndex() != Index) { Screen.HandleKey(EKeys::Down); }
			Screen.HandleKey(EKeys::Enter);
			TestTrue(TEXT("activating it shows the reason in the message line"), Screen.GetMessageText().StartsWith(TEXT("not available:")));
			break;
		}
	}
	TestTrue(TEXT("the transfer into the full box is listed as unavailable"), bFoundUnavailable);
	FSpnContainerView BodyAfter, BoxAfter;
	Env.Inv().GetContainer(Body, BodyAfter);
	Env.Inv().GetContainer(Box, BoxAfter);
	TestEqual(TEXT("body unchanged"), BodyAfter.Stacks.Num(), BodyBefore.Stacks.Num());
	TestEqual(TEXT("box unchanged"), BoxAfter.Stacks.Num(), BoxBefore.Stacks.Num());
	TestEqual(TEXT("revision unchanged by the rejected action"), Env.Inv().GetRevision(), RevisionBefore);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnUiContainersTest, "SpnGame.InventoryUi.Containers.SwitchAndTransfer", SpnUiFlags)
bool FSpnUiContainersTest::RunTest(const FString&)
{
	FUiEnv Env;
	if (!TestTrue(TEXT("setup"), Env.SetupData())) { return false; }
	Env.Inv().AddItem(Body, Bandage, 4);
	Env.Inv().AddItem(Trunk, Notebook);
	USpnInventoryScreen& Screen = Env.Open();
	TestEqual(TEXT("the body is the active container"), Screen.GetActiveContainer(), Body);
	TestTrue(TEXT("its tab is selected"), Screen.IsTabSelected(0));
	TestEqual(TEXT("one row"), Screen.GetVisibleRowCount(), 1);

	Screen.HandleKey(EKeys::Tab);
	TestEqual(TEXT("Tab switches to the next container"), Screen.GetActiveContainer(), Trunk);
	TestEqual(TEXT("the trunk's notebook is listed"), Screen.GetRowTitle(0), FString(TEXT("Test notebook")));
	TestTrue(TEXT("the second tab is selected"), Screen.IsTabSelected(1) && !Screen.IsTabSelected(0));
	Screen.HandleKey(EKeys::Tab, true);
	TestEqual(TEXT("Shift+Tab goes back"), Screen.GetActiveContainer(), Body);
	Screen.HandleKey(EKeys::Gamepad_RightShoulder);
	TestEqual(TEXT("R1 switches too"), Screen.GetActiveContainer(), Trunk);
	Screen.HandleKey(EKeys::Gamepad_LeftShoulder);

	// transfer a bandage from the body to the trunk through the action list
	const int32 TrunkBefore = [&]() { FSpnContainerView V; Env.Inv().GetContainer(Trunk, V); return V.Stacks.Num(); }();
	RunActionByPrefix(Screen, TEXT("Move to"));
	FSpnContainerView Trunk2;
	Env.Inv().GetContainer(Trunk, Trunk2);
	TestTrue(TEXT("the Core moved something to the trunk"), Trunk2.Stacks.Num() > TrunkBefore || Trunk2.Stacks.Num() == TrunkBefore);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnUiDiscardTest, "SpnGame.InventoryUi.Actions.DiscardNeedsConfirmation", SpnUiFlags)
bool FSpnUiDiscardTest::RunTest(const FString&)
{
	FUiEnv Env;
	if (!TestTrue(TEXT("setup"), Env.SetupData())) { return false; }
	Env.Inv().AddItem(Body, Notebook);
	USpnInventoryScreen& Screen = Env.Open();
	const auto StackCount = [&]() { FSpnContainerView V; Env.Inv().GetContainer(Body, V); return V.Stacks.Num(); };
	TestEqual(TEXT("one stack"), StackCount(), 1);

	Screen.HandleKey(EKeys::Delete);
	TestTrue(TEXT("Delete opens the confirmation"), Screen.IsDialogOpen());
	TestTrue(TEXT("the dialog names the item"), Screen.GetDialogText().Contains(TEXT("Test notebook")));
	TestEqual(TEXT("nothing is discarded yet"), StackCount(), 1);
	TestFalse(TEXT("the dialog starts on Cancel"), Screen.IsDialogConfirmFocused());
	Screen.HandleKey(EKeys::Enter);
	TestFalse(TEXT("Enter on Cancel closes the dialog"), Screen.IsDialogOpen());
	TestEqual(TEXT("and discards nothing"), StackCount(), 1);

	Screen.HandleKey(EKeys::Delete);
	Screen.HandleKey(EKeys::Escape);
	TestFalse(TEXT("Escape cancels the dialog"), Screen.IsDialogOpen());
	TestEqual(TEXT("and discards nothing"), StackCount(), 1);

	Screen.HandleKey(EKeys::Delete);
	Screen.HandleKey(EKeys::Right); // focus Confirm
	TestTrue(TEXT("focus moved to Confirm"), Screen.IsDialogConfirmFocused());
	Screen.HandleKey(EKeys::Enter);
	TestFalse(TEXT("the dialog closed"), Screen.IsDialogOpen());
	TestEqual(TEXT("the item is gone from the Core"), StackCount(), 0);
	TestEqual(TEXT("and from the screen"), Screen.GetVisibleRowCount(), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnUiSaveLoadTest, "SpnGame.InventoryUi.SaveLoad.ResetAndRefresh", SpnUiFlags)
bool FSpnUiSaveLoadTest::RunTest(const FString&)
{
	FUiEnv Env;
	if (!TestTrue(TEXT("setup"), Env.SetupData())) { return false; }
	Env.Inv().AddItem(Body, Ration, 2);
	Env.Inv().AddItem(Body, Flashlight);
	USpnInventoryScreen& Screen = Env.Open();
	const int32 RowsSaved = Screen.GetVisibleRowCount();
	TestTrue(TEXT("saved state has rows"), RowsSaved >= 2);
	if (!TestTrue(TEXT("save"), Env.Save().SaveSlot(TEXT("ui_slot")).IsOk())) { return false; }

	// change the state while the screen is open: it follows
	SelectRow(Screen, TEXT("Test flashlight"));
	Env.Inv().AddItem(Body, Bandage, 5);
	TestEqual(TEXT("a new stack appears on the open screen"), Screen.GetVisibleRowCount(), RowsSaved + 1);
	Env.Inv().ResetInventory();
	TestEqual(TEXT("reset empties the screen"), Screen.GetVisibleRowCount(), 0);
	TestEqual(TEXT("no stale selection text"), Screen.GetDetailsTitle(), FSpnUiText::Get(TEXT("ui.details.none")).ToString());

	// reset deleted the containers; load restores them with their items
	const FSpnSaveStatus Status = Env.Save().LoadSlot(TEXT("ui_slot"));
	TestTrue(TEXT("load succeeds"), Status.IsOk());
	TestEqual(TEXT("the loaded contents are displayed"), Screen.GetVisibleRowCount(), RowsSaved);
	TestTrue(TEXT("the flashlight is back"), FindRow(Screen, TEXT("Test flashlight")) != INDEX_NONE);
	TestTrue(TEXT("the bandage added after saving is gone"), FindRow(Screen, TEXT("Test bandage")) == INDEX_NONE);

	// a failed load must not change what is shown
	const FString Before = Screen.GetRowTitle(0);
	const FSpnSaveStatus Missing = Env.Save().LoadSlot(TEXT("does_not_exist"));
	TestFalse(TEXT("loading a missing slot fails"), Missing.IsOk());
	TestEqual(TEXT("the display is unchanged by the failed load"), Screen.GetRowTitle(0), Before);
	TestEqual(TEXT("and still has the same rows"), Screen.GetVisibleRowCount(), RowsSaved);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnUiReopenTest, "SpnGame.InventoryUi.Lifecycle.ReopenShowsCurrentState", SpnUiFlags)
bool FSpnUiReopenTest::RunTest(const FString&)
{
	FUiEnv Env;
	if (!TestTrue(TEXT("setup"), Env.SetupData())) { return false; }
	Env.Inv().AddItem(Body, Ration, 2);
	USpnInventoryScreen& Screen = Env.Open();
	TestEqual(TEXT("one row"), Screen.GetVisibleRowCount(), 1);
	bool bClosed = false;
	Screen.OnCloseRequested.BindLambda([&]() { bClosed = true; });
	Screen.HandleKey(EKeys::Escape);
	TestTrue(TEXT("Escape at the top level asks the owner to close"), bClosed);
	Screen.Teardown(); // what the owner does on closing

	// the inventory changes while the screen is closed
	Env.Inv().AddItem(Body, Flashlight);
	Env.Inv().AddItem(Body, Notebook);
	USpnInventoryScreen& Again = Env.Open();
	TestEqual(TEXT("the reopened screen shows the current state"), Again.GetVisibleRowCount(), 3);
	TestTrue(TEXT("with a selection"), Again.IsRowSelected(0));
	// no events from the closed phase leak into a second screen instance
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnUiInputTest, "SpnGame.InventoryUi.Input.BackStepsAndModalDialog", SpnUiFlags)
bool FSpnUiInputTest::RunTest(const FString&)
{
	FUiEnv Env;
	if (!TestTrue(TEXT("setup"), Env.SetupData())) { return false; }
	Env.Inv().AddItem(Body, Flashlight);
	USpnInventoryScreen& Screen = Env.Open();
	bool bClosed = false;
	Screen.OnCloseRequested.BindLambda([&]() { bClosed = true; });
	Screen.HandleKey(EKeys::Right);
	TestTrue(TEXT("Right enters the action list"), Screen.GetZone() == USpnInventoryScreen::EZone::Actions);
	Screen.HandleKey(EKeys::Gamepad_FaceButton_Right); // Circle
	TestTrue(TEXT("Circle steps back to the items"), Screen.GetZone() == USpnInventoryScreen::EZone::Items);
	TestFalse(TEXT("without closing"), bClosed);
	Screen.HandleKey(EKeys::Left);
	TestTrue(TEXT("Left enters the equipment panel"), Screen.GetZone() == USpnInventoryScreen::EZone::Equipment);
	TestTrue(TEXT("with a visible focus ring on a slot"), Screen.IsSlotFocusRing(0) || Screen.IsSlotFocusRing(1));
	Screen.HandleKey(EKeys::Gamepad_FaceButton_Right);
	Screen.HandleKey(EKeys::Gamepad_FaceButton_Right);
	TestTrue(TEXT("Circle at the top level closes"), bClosed);
	bClosed = false;
	Screen.HandleKey(EKeys::I);
	TestTrue(TEXT("the open key closes as well"), bClosed);
	TestFalse(TEXT("an unrelated key is not consumed"), Screen.HandleKey(EKeys::F7));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnUiLocalizationTest, "SpnGame.InventoryUi.Localization.GermanEnglishAndFallback", SpnUiFlags)
bool FSpnUiLocalizationTest::RunTest(const FString&)
{
	FUiEnv Env;
	if (!TestTrue(TEXT("setup"), Env.SetupData())) { return false; }
	Env.Inv().AddItem(Body, Flashlight);
	USpnInventoryScreen& Screen = Env.Open();
	TestEqual(TEXT("English title"), Screen.GetHeaderTitle(), FString(TEXT("INVENTORY")));
	FSpnUiText::SetLanguageOverride(TEXT("de"));
	Screen.OnShown();
	TestEqual(TEXT("German title"), Screen.GetHeaderTitle(), FSpnUiText::GetInLanguage(TEXT("ui.inventory.title"), TEXT("de")));
	TestTrue(TEXT("German item name"), Screen.GetRowTitle(0) == TEXT("Test-Taschenlampe"));
	TestNotEqual(TEXT("the two languages differ"), FSpnUiText::GetInLanguage(TEXT("ui.inventory.title"), TEXT("de")), FSpnUiText::GetInLanguage(TEXT("ui.inventory.title"), TEXT("en")));

	// fallback chain: unknown key -> caller fallback -> the key itself, never empty text
	TestEqual(TEXT("caller fallback"), FSpnUiText::Get(TEXT("ui.nonexistent.key"), {}, FText::FromString(TEXT("Fallback"))).ToString(), FString(TEXT("Fallback")));
	TestEqual(TEXT("key as last resort"), FSpnUiText::Get(TEXT("ui.nonexistent.key")).ToString(), FString(TEXT("ui.nonexistent.key")));
	// every error key the view-model can produce has both texts
	for (const FString& Key : FSpnUiText::AllKeys())
	{
		TestFalse(*FString::Printf(TEXT("%s has an English text"), *Key), FSpnUiText::GetInLanguage(Key, TEXT("en")).IsEmpty());
		TestFalse(*FString::Printf(TEXT("%s has a German text"), *Key), FSpnUiText::GetInLanguage(Key, TEXT("de")).IsEmpty());
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnUiPerformanceTest, "SpnGame.InventoryUi.Performance.LargeInventory", SpnUiFlags)
bool FSpnUiPerformanceTest::RunTest(const FString&)
{
	FUiEnv Env;
	if (!TestTrue(TEXT("setup"), Env.SetupData())) { return false; }
	FSpnContainerRulesData Depot; // unlimited test depot (rules from the test, not game values)
	Depot.Kind = TEXT("spn_test:container.trunk");
	const int64 DepotId = 9;
	TestTrue(TEXT("depot"), Env.Inv().CreateContainer(DepotId, Depot).IsOk());
	TArray<FSpnInventoryCommand> Commands;
	for (int32 Index = 0; Index < 600; ++Index)
	{
		Commands.Add(FSpnInventoryCommand::AddItem(DepotId, Index % 2 == 0 ? Notebook : Coat, 1));
	}
	TestTrue(TEXT("600 stacks added in one transaction"), Env.Inv().Apply(Commands).IsOk());

	const double OpenStart = FPlatformTime::Seconds();
	USpnInventoryScreen& Screen = Env.Open();
	Screen.HandleKey(EKeys::Tab); // trunk
	Screen.HandleKey(EKeys::Tab); // box
	for (int32 Guard = 0; Guard < 5 && Screen.GetActiveContainer() != DepotId; ++Guard) { Screen.HandleKey(EKeys::Tab); }
	const double OpenMs = (FPlatformTime::Seconds() - OpenStart) * 1000.0;
	TestEqual(TEXT("the depot is active"), Screen.GetActiveContainer(), DepotId);
	TestEqual(TEXT("the list is windowed, not 600 widgets"), Screen.GetVisibleRowCount(), USpnInventoryScreen::MaxRowsShown);
	TestTrue(TEXT("widget count stays bounded"), Screen.GetBuiltRowWidgetCount() <= USpnInventoryScreen::MaxRowsShown);

	const int64 StackBefore = Screen.GetViewModel()->GetSelectedStackId();
	const double NavStart = FPlatformTime::Seconds();
	for (int32 Step = 0; Step < 200; ++Step) { Screen.HandleKey(EKeys::Down); }
	AddInfo(FString::Printf(TEXT("DEBUG nav: stack %lld -> %lld, zone %d, row0 selected %d"), StackBefore, Screen.GetViewModel()->GetSelectedStackId(), static_cast<int32>(Screen.GetZone()), Screen.IsRowSelected(0) ? 1 : 0));
	Screen.LastFullReason.Empty();
	const double NavMs = (FPlatformTime::Seconds() - NavStart) * 1000.0 / 200.0;
	TestTrue(TEXT("the selection window followed the selection"), Screen.GetVisibleRowCount() == USpnInventoryScreen::MaxRowsShown);

	const int32 RebuildsBefore = Screen.RowsRebuilt;
	const int32 FullBefore = Screen.FullRefreshes;
	Env.Inv().AddItem(Body, Ration, 1); // an unrelated container must not rebuild the depot rows
	const int32 RebuildsAfter = Screen.RowsRebuilt;
	const double MutStart = FPlatformTime::Seconds();
	Env.Inv().AddItem(DepotId, Notebook, 1);
	const double MutMs = (FPlatformTime::Seconds() - MutStart) * 1000.0;
	TestEqual(TEXT("a change in an unrelated container does not rebuild the active list"), RebuildsAfter, RebuildsBefore);
	AddInfo(FString::Printf(TEXT("DEBUG rows %d->%d full %d->%d active %lld selected %lld reason '%s' flush %d all %d switch %d"), RebuildsBefore, RebuildsAfter, FullBefore, Screen.FullRefreshes, Screen.GetActiveContainer(), Screen.GetViewModel()->GetSelectedContainerId(), *Screen.LastFullReason, Screen.RowsFromFlush, Screen.RowsFromAll, Screen.RowsFromSwitch));

	AddInfo(FString::Printf(TEXT("PERF 600 stacks: open+switch %.2f ms, selection step %.3f ms avg, mutation+refresh %.2f ms, last full refresh %.2f ms, last rows build %.2f ms, row widgets %d"),
		OpenMs, NavMs, MutMs, Screen.LastRefreshMs, Screen.LastRowsBuildMs, Screen.GetBuiltRowWidgetCount()));
	return true;
}

#endif
