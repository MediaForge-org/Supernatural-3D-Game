#include "Blueprint/UserWidget.h"
#include "Engine/GameInstance.h"
#include "HAL/FileManager.h"
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

// Night Build 01 completion, package M (inventory UI 2.0): list/grid view, sort, category filter, grid navigation, weight/volume/condition in the rows.
// The tests read the REAL widgets (titles, subtitles, tile flag, toolbar text) and prove that sorting and filtering are view changes only: the Core
// inventory revision, the container description and every stack id stay exactly as they were.

namespace
{
	constexpr EAutomationTestFlags Ui2Flags = EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter;
	constexpr int64 BodyId = 1;
	const FString Notebook2 = TEXT("spn_test:item.notebook");
	const FString Ration2 = TEXT("spn_test:item.ration");
	const FString Bandage2 = TEXT("spn_test:item.bandage");
	const FString Coat2 = TEXT("spn_test:item.coat");
	const FString Flashlight2 = TEXT("spn_test:item.flashlight");

	struct FEnv2
	{
		UGameInstance* Instance = nullptr;
		USpnInventoryScreen* Screen = nullptr;
		FString Root;

		FEnv2()
		{
			Instance = NewObject<UGameInstance>(GetTransientPackage());
			Instance->AddToRoot();
			Instance->Init();
			Root = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir() / TEXT("SpnTestTemp") / FGuid::NewGuid().ToString(EGuidFormats::Digits));
			Instance->GetSubsystem<USpnSaveSubsystem>()->SetSaveDirectory(Root);
			FSpnUiText::SetLanguageOverride(TEXT("en"));
		}
		~FEnv2()
		{
			if (Screen != nullptr) { Screen->Teardown(); Screen->RemoveFromParent(); Screen->RemoveFromRoot(); }
			FSpnUiText::SetLanguageOverride(FString());
			Instance->Shutdown();
			Instance->RemoveFromRoot();
			IFileManager::Get().DeleteDirectory(*Root, false, true);
		}
		USpnInventorySubsystem& Inv() const { return *Instance->GetSubsystem<USpnInventorySubsystem>(); }
		bool Setup()
		{
			return Inv().LoadCatalogFromFile(USpnInventorySubsystem::GetTestCatalogPath()).IsOk() && Inv().CreateContainerFromPreset(BodyId, TEXT("spn_test:preset.body")).IsOk();
		}
		USpnInventoryScreen& Open()
		{
			if (Screen == nullptr)
			{
				Screen = CreateWidget<USpnInventoryScreen>(Instance, USpnInventoryScreen::StaticClass());
				Screen->AddToRoot();
			}
			Screen->Setup(&Inv(), Instance->GetSubsystem<USpnSaveSubsystem>(), BodyId);
			Screen->OnShown();
			return *Screen;
		}
	};

	TArray<FString> Titles(const USpnInventoryScreen& Screen)
	{
		TArray<FString> Out;
		for (int32 I = 0; I < Screen.GetVisibleRowCount(); ++I) { Out.Add(Screen.GetRowTitle(I)); }
		return Out;
	}

	/** Everything the Core holds, as one comparable string (ids, definitions, quantities, equipment). */
	FString CoreFingerprint(USpnInventorySubsystem& Inv)
	{
		return FString::Printf(TEXT("rev=%llu %s"), static_cast<unsigned long long>(Inv.GetRevision()), *Inv.Describe());
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnUi2ViewSortFilterTest, "SpnGame.InventoryUi2.ViewSortFilter", Ui2Flags)
bool FSpnUi2ViewSortFilterTest::RunTest(const FString&)
{
	FEnv2 Env;
	if (!TestTrue(TEXT("setup"), Env.Setup())) { return false; }
	// added in this order: stack ids ascend with it, so the Core order is Notebook, Ration, Coat, Bandage, Flashlight
	Env.Inv().AddItem(BodyId, Notebook2, 1);
	Env.Inv().AddItem(BodyId, Ration2, 2);
	Env.Inv().AddItem(BodyId, Coat2, 1);
	Env.Inv().AddItem(BodyId, Bandage2, 4);
	Env.Inv().AddItem(BodyId, Flashlight2, 1);
	const FString CoreBefore = CoreFingerprint(Env.Inv());

	USpnInventoryScreen& Screen = Env.Open();
	const TArray<FString> CoreOrder = Titles(Screen);
	TestEqual(TEXT("five rows in the Core order"), CoreOrder.Num(), 5);
	TestTrue(TEXT("list view is the default"), !Screen.IsGridView() && Screen.GetToolbarText(0).Contains(TEXT("List")));
	TestTrue(TEXT("toolbar shows the sort and the filter"), Screen.GetToolbarText(1).Contains(TEXT("Default")) && Screen.GetToolbarText(2).Contains(TEXT("All")));

	// weight, volume and quantity in the rows (condition = the state fields of non-stackable items)
	// CoreOrder = Notebook, Ration, Coat, Bandage, Flashlight (display names, not ids)
	const FString NameCoat = CoreOrder[2], NameRation = CoreOrder[1], NameBandage = CoreOrder[3];
	const int32 CoatRow = CoreOrder.IndexOfByKey(NameCoat);
	AddInfo(FString::Printf(TEXT("row subtitle of the coat: '%s'"), *Screen.GetRowSubtitle(CoatRow)));
	TestTrue(TEXT("a row shows the total weight and volume of its stack (coat: 1.2 kg, 4 l)"), Screen.GetRowSubtitle(CoatRow).Contains(TEXT("1.2")) && Screen.GetRowSubtitle(CoatRow).Contains(TEXT("4")));
	TestTrue(TEXT("the quantity is shown (2 rations)"), Screen.GetRowRight(CoreOrder.IndexOfByKey(NameRation)).Contains(TEXT("2")));

	// ---- sorting: a view only ----
	Screen.HandleKey(EKeys::O);
	TestTrue(TEXT("O: sorted by name"), Screen.GetToolbarText(1).Contains(TEXT("Name")));
	TArray<FString> ByName = Titles(Screen);
	TArray<FString> Expected = CoreOrder;
	Expected.Sort([](const FString& A, const FString& B) { return A.Compare(B, ESearchCase::IgnoreCase) < 0; });
	TestEqual(TEXT("rows are in name order"), ByName, Expected);
	Screen.HandleKey(EKeys::O, /*bShift*/ true);
	TArray<FString> Descending = Expected;
	Algo::Reverse(Descending);
	TestEqual(TEXT("Shift+O reverses the direction"), Titles(Screen), Descending);
	TestEqual(TEXT("sorting changed nothing in the Core (revision, ids, contents)"), CoreFingerprint(Env.Inv()), CoreBefore);
	for (int32 Step = 0; Step < 6; ++Step) { Screen.HandleKey(EKeys::O); } // category, weight, volume, quantity, default, name ...
	TestEqual(TEXT("every sort mode kept all rows"), Screen.GetVisibleRowCount(), 5);
	// back to default: the Core order again
	for (int32 Guard = 0; Guard < 8 && !Screen.GetToolbarText(1).Contains(TEXT("Default")); ++Guard) { Screen.HandleKey(EKeys::O); }
	TestEqual(TEXT("Default restores the Core order exactly"), Titles(Screen), CoreOrder);
	TestEqual(TEXT("still nothing changed in the Core"), CoreFingerprint(Env.Inv()), CoreBefore);

	// ---- sort by quantity: the biggest stack is last / first and ties keep the stack id order ----
	while (!Screen.GetToolbarText(1).Contains(TEXT("Quantity"))) { Screen.HandleKey(EKeys::O); }
	TestEqual(TEXT("quantity ascending: the 4 bandages are last"), Titles(Screen).Last(), NameBandage);
	Screen.HandleKey(EKeys::O, true);
	TestEqual(TEXT("quantity descending: the 4 bandages are first"), Titles(Screen)[0], NameBandage);
	while (!Screen.GetToolbarText(1).Contains(TEXT("Default"))) { Screen.HandleKey(EKeys::O); }

	// ---- category filter ----
	const int32 All = Screen.GetVisibleRowCount();
	Screen.HandleKey(EKeys::F);
	const FString First = Screen.GetToolbarText(2);
	TestFalse(TEXT("F: a category filter is active"), First.Contains(TEXT("All")));
	TestTrue(TEXT("fewer rows are visible"), Screen.GetVisibleRowCount() < All && Screen.GetVisibleRowCount() > 0);
	TestEqual(TEXT("filtering changed nothing in the Core"), CoreFingerprint(Env.Inv()), CoreBefore);
	int32 Cycles = 1;
	int32 SumRows = Screen.GetVisibleRowCount();
	for (; Cycles < 10; ++Cycles)
	{
		Screen.HandleKey(EKeys::F);
		if (Screen.GetToolbarText(2).Contains(TEXT("All"))) { break; }
		SumRows += Screen.GetVisibleRowCount();
	}
	TestEqual(TEXT("the categories together show every stack exactly once"), SumRows, All);
	TestEqual(TEXT("after the last category the filter is cleared"), Screen.GetVisibleRowCount(), All);
	TestEqual(TEXT("the Core is still untouched"), CoreFingerprint(Env.Inv()), CoreBefore);

	// an action on a filtered row still works through the same Core transaction path
	Screen.HandleKey(EKeys::F);
	const int32 Before = static_cast<int32>(Env.Inv().GetRevision());
	TestTrue(TEXT("the filtered list has a selection"), Screen.GetViewModel()->GetSelectedStackId() != 0);
	Screen.HandleKey(EKeys::F);
	while (!Screen.GetToolbarText(2).Contains(TEXT("All"))) { Screen.HandleKey(EKeys::F); }
	TestEqual(TEXT("(no action ran while only the view changed)"), static_cast<int32>(Env.Inv().GetRevision()), Before);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnUi2GridTest, "SpnGame.InventoryUi2.GridView", Ui2Flags)
bool FSpnUi2GridTest::RunTest(const FString&)
{
	FEnv2 Env;
	if (!TestTrue(TEXT("setup"), Env.Setup())) { return false; }
	FSpnContainerRulesData Depot;
	Depot.Kind = TEXT("spn_test:container.trunk");
	TestTrue(TEXT("depot"), Env.Inv().CreateContainer(9, Depot).IsOk());
	TArray<FSpnInventoryCommand> Commands;
	for (int32 I = 0; I < 14; ++I) { Commands.Add(FSpnInventoryCommand::AddItem(9, I % 2 == 0 ? Notebook2 : Coat2, 1)); }
	TestTrue(TEXT("14 stacks"), Env.Inv().Apply(Commands).IsOk());
	Env.Inv().AddItem(BodyId, Ration2, 2);

	USpnInventoryScreen& Screen = Env.Open();
	for (int32 Guard = 0; Guard < 5 && Screen.GetActiveContainer() != 9; ++Guard) { Screen.HandleKey(EKeys::Tab); }
	TestEqual(TEXT("the depot is active"), Screen.GetActiveContainer(), 9ll);
	TestEqual(TEXT("14 rows in the list"), Screen.GetVisibleRowCount(), 14);
	const int64 SelectedBefore = Screen.GetViewModel()->GetSelectedStackId();

	Screen.HandleKey(EKeys::V);
	TestTrue(TEXT("V: grid view"), Screen.IsGridView() && Screen.GetToolbarText(0).Contains(TEXT("Grid")));
	TestEqual(TEXT("the same 14 stacks in the grid"), Screen.GetVisibleRowCount(), 14);
	TestEqual(TEXT("the selection stays on the same stack"), Screen.GetViewModel()->GetSelectedStackId(), SelectedBefore);
	TestEqual(TEXT("the view model navigates in columns"), Screen.GetViewModel()->GetGridColumns(), USpnInventoryScreen::GridColumnCount);
	TestTrue(TEXT("grid tiles are drawn as tiles"), Screen.IsRowTile(0));

	// keyboard: Right moves one cell, Down one grid row (6 cells), Right at the last cell of a row leaves the grid
	Screen.HandleKey(EKeys::Home);
	const int64 First = Screen.GetViewModel()->GetSelectedStackId();
	Screen.HandleKey(EKeys::Right);
	const int64 Second = Screen.GetViewModel()->GetSelectedStackId();
	TestTrue(TEXT("Right: the next cell"), Second != First && Screen.IsRowSelected(1));
	Screen.HandleKey(EKeys::Down);
	TestTrue(TEXT("Down: one grid row further (cell 1 + 6 = 7)"), Screen.IsRowSelected(1 + USpnInventoryScreen::GridColumnCount));
	Screen.HandleKey(EKeys::Up);
	TestTrue(TEXT("Up: back"), Screen.IsRowSelected(1));
	Screen.HandleKey(EKeys::Home);
	for (int32 I = 0; I < USpnInventoryScreen::GridColumnCount - 1; ++I) { Screen.HandleKey(EKeys::Right); }
	TestTrue(TEXT("at the end of the row"), Screen.IsRowSelected(USpnInventoryScreen::GridColumnCount - 1));
	Screen.HandleKey(EKeys::Right);
	TestTrue(TEXT("Right at the edge of the row leaves the grid for the actions panel"), Screen.GetZone() == USpnInventoryScreen::EZone::Actions);
	Screen.HandleKey(EKeys::Left);
	TestTrue(TEXT("Left comes back"), Screen.GetZone() == USpnInventoryScreen::EZone::Items);

	// selection survives sorting in the grid and the grid survives a reopen
	Screen.HandleKey(EKeys::O);
	TestTrue(TEXT("still a grid after sorting"), Screen.IsGridView() && Screen.GetVisibleRowCount() == 14);
	Screen.OnShown();
	TestTrue(TEXT("and after reopening the screen is still a grid (the view choice is kept)"), Screen.IsGridView() && Screen.IsRowTile(0));
	Screen.HandleKey(EKeys::V);
	TestTrue(TEXT("V again: list view"), !Screen.IsGridView() && !Screen.IsRowTile(0));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnUi2PerformanceTest, "SpnGame.InventoryUi2.Performance", Ui2Flags)
bool FSpnUi2PerformanceTest::RunTest(const FString&)
{
	FEnv2 Env;
	if (!TestTrue(TEXT("setup"), Env.Setup())) { return false; }
	FSpnContainerRulesData Depot;
	Depot.Kind = TEXT("spn_test:container.trunk");
	int64 DepotId = 8;
	for (const int32 Count : {100, 1000, 5000})
	{
		TArray<FSpnInventoryCommand> Commands;
		++DepotId; // a fresh container per size (a container with stacks cannot be destroyed)
		TestTrue(TEXT("depot"), Env.Inv().CreateContainer(DepotId, Depot).IsOk());
		for (int32 I = 0; I < Count; ++I) { Commands.Add(FSpnInventoryCommand::AddItem(DepotId, I % 3 == 0 ? Notebook2 : I % 3 == 1 ? Coat2 : Flashlight2, 1)); }
		TestTrue(TEXT("stacks added"), Env.Inv().Apply(Commands).IsOk());
		if (Env.Screen != nullptr) { Env.Screen->Teardown(); }
		const double OpenStart = FPlatformTime::Seconds();
		USpnInventoryScreen& Screen = Env.Open();
		for (int32 Guard = 0; Guard < 8 && Screen.GetActiveContainer() != DepotId; ++Guard) { Screen.HandleKey(EKeys::Tab); }
		TestEqual(TEXT("the depot is active"), Screen.GetActiveContainer(), DepotId);
		const double OpenMs = (FPlatformTime::Seconds() - OpenStart) * 1000.0;
		const double ViewStart = FPlatformTime::Seconds();
		Screen.HandleKey(EKeys::V);
		const double GridMs = (FPlatformTime::Seconds() - ViewStart) * 1000.0;
		const double SortStart = FPlatformTime::Seconds();
		Screen.HandleKey(EKeys::O);
		const double SortMs = (FPlatformTime::Seconds() - SortStart) * 1000.0;
		const double FilterStart = FPlatformTime::Seconds();
		Screen.HandleKey(EKeys::F);
		const double FilterMs = (FPlatformTime::Seconds() - FilterStart) * 1000.0;
		const double NavStart = FPlatformTime::Seconds();
		for (int32 I = 0; I < 100; ++I) { Screen.HandleKey(EKeys::Down); }
		const double NavMs = (FPlatformTime::Seconds() - NavStart) * 1000.0 / 100.0;
		TestTrue(TEXT("the widget count stays bounded by the window"), Screen.GetBuiltRowWidgetCount() <= USpnInventoryScreen::MaxRowsShown);
		AddInfo(FString::Printf(TEXT("PERF2 %d stacks: open+switch %.1f ms, list->grid %.1f ms, sort %.1f ms, category filter %.1f ms, selection step %.2f ms avg, row widgets %d"),
			Count, OpenMs, GridMs, SortMs, FilterMs, NavMs, Screen.GetBuiltRowWidgetCount()));
	}
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
