#include "Engine/GameInstance.h"
#include "HAL/FileManager.h"
#include "Misc/AutomationTest.h"
#include "Misc/FileHelper.h"
#include "Misc/Guid.h"
#include "Misc/Paths.h"
#include "SpnInventorySubsystem.h"
#include "SpnMapSubsystem.h"
#include "SpnSaveSubsystem.h"
#include "SpnWorldStateSubsystem.h"
#include "SpnWorldFrame.h"
#include "UObject/Package.h"

#if WITH_DEV_AUTOMATION_TESTS

// Unreal checks for USpnMapSubsystem and its save integration (Night Build 01). Pure subsystem tests: no level, no widgets. Every save test works in
// its own temporary save root and removes it afterwards.

namespace
{
	constexpr EAutomationTestFlags Flags = EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter;

	struct FEnv
	{
		UGameInstance* Instance = nullptr;
		FString Root;

		FEnv()
		{
			Instance = NewObject<UGameInstance>(GetTransientPackage());
			Instance->AddToRoot();
			Instance->Init();
			Root = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir() / TEXT("SpnTestTemp") / FGuid::NewGuid().ToString(EGuidFormats::Digits));
			Save().SetSaveDirectory(Root);
		}

		~FEnv()
		{
			Instance->Shutdown();
			Instance->RemoveFromRoot();
			IFileManager::Get().DeleteDirectory(*Root, false, true);
		}

		USpnMapSubsystem& Map() const { return *Instance->GetSubsystem<USpnMapSubsystem>(); }
		USpnSaveSubsystem& Save() const { return *Instance->GetSubsystem<USpnSaveSubsystem>(); }
		USpnInventorySubsystem& Inventory() const { return *Instance->GetSubsystem<USpnInventorySubsystem>(); }
		USpnWorldStateSubsystem& World() const { return *Instance->GetSubsystem<USpnWorldStateSubsystem>(); }
	};

	/** Unreal centimetres of a map position (EastSouthUp: +X east, +Y south). */
	FVector Cm(double EastM, double NorthM) { return FVector(EastM * 100.0, -NorthM * 100.0, 100.0); }
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnMapWaypointTest, "SpnCore.Map.Waypoint", Flags)
bool FSpnMapWaypointTest::RunTest(const FString&)
{
	FEnv Env;
	USpnMapSubsystem& Map = Env.Map();
	TestFalse(TEXT("no definition yet"), Map.HasDefinition());
	int64 Id = 0;
	TestEqual(TEXT("without a definition a waypoint is refused"), Map.SetWaypointAt(FVector2D(1, 1), TEXT("x"), Id), ESpnMapResult::NoDefinition);
	TestEqual(TEXT("test definition installs"), Map.InstallTestDefinition(), ESpnMapResult::Ok);
	TestTrue(TEXT("has definition"), Map.HasDefinition());
	TestTrue(TEXT("flagged as test data"), Map.IsTestDefinition());

	TestEqual(TEXT("waypoint set"), Map.SetWaypointAt(FVector2D(10, 20), TEXT("Alpha"), Id), ESpnMapResult::Ok);
	TestTrue(TEXT("a marker id was allocated"), Id > 0);
	TestEqual(TEXT("one marker"), Map.GetMarkers().Num(), 1);
	FSpnMapMarkerView Waypoint;
	TestTrue(TEXT("waypoint readable"), Map.GetWaypoint(Waypoint));
	TestEqual(TEXT("label kept"), Waypoint.Label, FString(TEXT("Alpha")));
	TestTrue(TEXT("at the requested position"), Waypoint.Metres.Equals(FVector2D(10, 20), 1e-9));

	int64 Second = 0;
	TestEqual(TEXT("a new waypoint replaces the old one"), Map.SetWaypointAt(FVector2D(-30, 5), TEXT("Beta"), Second), ESpnMapResult::Ok);
	TestEqual(TEXT("still exactly one"), Map.GetMarkers().Num(), 1);
	TestTrue(TEXT("ids are never reused"), Second > Id);
	TestTrue(TEXT("the new one is the active waypoint"), Map.GetWaypoint(Waypoint) && Waypoint.Id == Second);

	const uint64 Revision = Map.GetRevision();
	int64 Rejected = 0;
	TestEqual(TEXT("outside the area is refused"), Map.SetWaypointAt(FVector2D(5000, 0), TEXT("far"), Rejected), ESpnMapResult::OutOfBounds);
	TestEqual(TEXT("NaN is refused"), Map.SetWaypointAt(FVector2D(NAN, 0), TEXT("nan"), Rejected), ESpnMapResult::OutOfBounds);
	TestEqual(TEXT("a refused waypoint changes nothing"), Map.GetRevision(), Revision);
	TestTrue(TEXT("the old waypoint is still there"), Map.GetWaypoint(Waypoint) && Waypoint.Id == Second);

	TestEqual(TEXT("unknown marker"), Map.RemoveMarker(999999), ESpnMapResult::UnknownTarget);
	TestEqual(TEXT("unknown POI"), Map.SetWaypointToPoi(TEXT("spn_test:poi.nope"), Rejected), ESpnMapResult::UnknownTarget);
	TestEqual(TEXT("a POI waypoint"), Map.SetWaypointToPoi(TEXT("spn_test:poi.birkenweg"), Id), ESpnMapResult::Ok);
	TestTrue(TEXT("linked to its POI"), Map.GetWaypoint(Waypoint) && Waypoint.LinkedPoi == TEXT("spn_test:poi.birkenweg"));
	TestEqual(TEXT("replaces again"), Map.GetMarkers().Num(), 1);
	TestTrue(TEXT("cleared"), Map.ClearWaypoint());
	TestFalse(TEXT("nothing to clear twice"), Map.ClearWaypoint());
	TestFalse(TEXT("no waypoint"), Map.GetWaypoint(Waypoint));

	// control characters in labels never reach the Core
	TestEqual(TEXT("a label with control characters is cleaned"), Map.SetWaypointAt(FVector2D(1, 1), FString(TEXT("A\nB\x01")), Id), ESpnMapResult::Ok);
	TestTrue(TEXT("label cleaned"), Map.GetWaypoint(Waypoint) && Waypoint.Label == TEXT("AB"));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnMapNavigationTest, "SpnCore.Map.Navigation", Flags)
bool FSpnMapNavigationTest::RunTest(const FString&)
{
	FEnv Env;
	USpnMapSubsystem& Map = Env.Map();
	Map.InstallTestDefinition();
	int64 Id = 0;
	TestFalse(TEXT("no waypoint, no navigation"), Map.GetNavigation(Cm(0, 0), 0.f).bActive);
	Map.SetWaypointAt(FVector2D(0, 100), TEXT("North"), Id);

	// EastSouthUp: yaw 0 = east, yaw -90 = north
	const FSpnNavigationReading East = Map.GetNavigation(Cm(0, 0), 0.f);
	TestTrue(TEXT("active"), East.bActive);
	TestTrue(TEXT("a straight line, never a route"), East.bStraightLine);
	TestEqual(TEXT("distance 100 m"), East.DistanceMetres, 100.0, 1e-6);
	TestEqual(TEXT("bearing north"), East.BearingRadians, 0.0, 1e-9);
	TestEqual(TEXT("facing east: the target is 90 degrees to the left"), East.RelativeRadians, -UE_DOUBLE_PI * 0.5, 1e-6);
	const FSpnNavigationReading North = Map.GetNavigation(Cm(0, 0), -90.f);
	TestEqual(TEXT("facing north: straight ahead"), North.RelativeRadians, 0.0, 1e-6);
	const FSpnNavigationReading South = Map.GetNavigation(Cm(0, 0), 90.f);
	TestEqual(TEXT("facing south: behind (pi)"), FMath::Abs(South.RelativeRadians), UE_DOUBLE_PI, 1e-6);
	TestFalse(TEXT("not arrived"), North.bArrived);

	// the bearing is measured from the player: a target to the east of the player
	Map.SetWaypointAt(FVector2D(50, 0), TEXT("East"), Id);
	const FSpnNavigationReading Right = Map.GetNavigation(Cm(0, 0), -90.f);
	TestEqual(TEXT("target east, facing north: 90 degrees to the right"), Right.RelativeRadians, UE_DOUBLE_PI * 0.5, 1e-6);
	TestEqual(TEXT("bearing east"), Right.BearingRadians, UE_DOUBLE_PI * 0.5, 1e-9);

	// arrival is announced once per waypoint
	int32 Reached = 0;
	Map.OnWaypointReached.AddLambda([&Reached](const FSpnMapMarkerView&) { ++Reached; });
	Map.SetArrivalRadiusMetres(4.0);
	Map.UpdateNavigation(Cm(20, 0), 0.f);
	TestEqual(TEXT("far away: nothing"), Reached, 0);
	Map.UpdateNavigation(Cm(47.5, 0), 0.f);
	TestEqual(TEXT("2.5 m away: arrived, announced"), Reached, 1);
	TestTrue(TEXT("reading says arrived"), Map.GetNavigation(Cm(47.5, 0), 0.f).bArrived);
	Map.UpdateNavigation(Cm(49, 0), 0.f);
	TestEqual(TEXT("not announced twice"), Reached, 1);
	Map.SetWaypointAt(FVector2D(60, 0), TEXT("Next"), Id);
	Map.UpdateNavigation(Cm(59, 0), 0.f);
	TestEqual(TEXT("a new waypoint announces again"), Reached, 2);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnMapMinimapTest, "SpnCore.Map.Minimap", Flags)
bool FSpnMapMinimapTest::RunTest(const FString&)
{
	FEnv Env;
	USpnMapSubsystem& Map = Env.Map();
	TestFalse(TEXT("no definition: invalid frame"), Map.BuildMinimapFrame(FSpnMinimapRequest()).bValid);
	Map.InstallTestDefinition();
	int64 Id = 0;
	Map.SetWaypointAt(FVector2D(50, 0), TEXT("East 50"), Id);

	FSpnMinimapRequest Request;
	Request.PlayerLocationCm = Cm(0, 0);
	Request.PlayerYawDegrees = -90.f; // facing north
	Request.Zoom = 1.f;
	Request.BaseMetresPerPixel = 0.5f;
	Request.RadiusPx = 110.f;
	Request.IndicatorRangeMetres = 400.f;

	FSpnMinimapFrameData Frame = Map.BuildMinimapFrame(Request);
	TestTrue(TEXT("valid frame"), Frame.bValid);
	const FSpnMinimapEntry* Waypoint = Frame.Entries.FindByPredicate([](const FSpnMinimapEntry& E) { return E.bPlayerWaypoint; });
	TestTrue(TEXT("the waypoint is on the minimap"), Waypoint != nullptr);
	if (Waypoint != nullptr)
	{
		TestEqual(TEXT("north-up: 50 m east at 0.5 m/px = 100 px right"), Waypoint->Screen.X, 100.0, 1e-6);
		TestEqual(TEXT("north-up: same height"), Waypoint->Screen.Y, 0.0, 1e-6);
		TestFalse(TEXT("inside the viewport"), Waypoint->bOffscreen);
		TestEqual(TEXT("distance"), Waypoint->DistanceMetres, 50.0, 1e-6);
	}
	TestEqual(TEXT("north-up: north is screen-up"), Frame.NorthOnScreenRadians, 0.0, 1e-9);

	Request.bHeadingUp = true;
	Frame = Map.BuildMinimapFrame(Request);
	Waypoint = Frame.Entries.FindByPredicate([](const FSpnMinimapEntry& E) { return E.bPlayerWaypoint; });
	TestTrue(TEXT("heading-up: still there"), Waypoint != nullptr);
	if (Waypoint != nullptr)
	{
		TestEqual(TEXT("heading north, target east: to the right"), Waypoint->Screen.X, 100.0, 1e-6);
	}
	Request.PlayerYawDegrees = 0.f; // facing east: the target is straight ahead
	Frame = Map.BuildMinimapFrame(Request);
	Waypoint = Frame.Entries.FindByPredicate([](const FSpnMinimapEntry& E) { return E.bPlayerWaypoint; });
	if (Waypoint != nullptr)
	{
		TestEqual(TEXT("heading east: straight ahead x"), Waypoint->Screen.X, 0.0, 1e-6);
		TestEqual(TEXT("heading east: straight ahead y (screen up)"), Waypoint->Screen.Y, -100.0, 1e-6);
	}

	// zoom halves the distance in pixels... and doubles it
	Request.bHeadingUp = false;
	Request.Zoom = 0.5f;
	Waypoint = nullptr;
	Frame = Map.BuildMinimapFrame(Request);
	Waypoint = Frame.Entries.FindByPredicate([](const FSpnMinimapEntry& E) { return E.bPlayerWaypoint; });
	TestTrue(TEXT("zoom out: 50 px"), Waypoint != nullptr && FMath::IsNearlyEqual(Waypoint->Screen.X, 50.0, 1e-6));

	// outside the viewport but inside the indicator range: an edge indicator
	Map.SetWaypointAt(FVector2D(150, 0), TEXT("far"), Id);
	Request.Zoom = 1.f;
	Frame = Map.BuildMinimapFrame(Request);
	Waypoint = Frame.Entries.FindByPredicate([](const FSpnMinimapEntry& E) { return E.bPlayerWaypoint; });
	TestTrue(TEXT("150 m = 300 px is outside a 110 px viewport"), Waypoint != nullptr && Waypoint->bOffscreen);
	if (Waypoint != nullptr)
	{
		TestTrue(TEXT("the indicator sits on the edge"), FMath::Abs(Waypoint->Screen.Size() - 102.0) < 0.5);
	}
	Request.IndicatorRangeMetres = 0.f;
	TestEqual(TEXT("no indicator range: nothing"), Map.BuildMinimapFrame(Request).Entries.FindByPredicate([](const FSpnMinimapEntry& E) { return E.bPlayerWaypoint; }) == nullptr, true);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnMapDiscoveryTest, "SpnCore.Map.Discovery", Flags)
bool FSpnMapDiscoveryTest::RunTest(const FString&)
{
	FEnv Env;
	USpnMapSubsystem& Map = Env.Map();
	Map.InstallTestDefinition();
	auto Find = [&Map](const TCHAR* Id) -> FSpnMapPoiView
	{
		for (const FSpnMapPoiView& P : Map.GetPois()) { if (P.Id == Id) { return P; } }
		return FSpnMapPoiView();
	};
	// the start is "Always" visible; a street far away is hidden until it is known
	TestTrue(TEXT("start POI always visible"), Find(TEXT("spn_test:poi.player_start")).bVisible);
	TestFalse(TEXT("far POI hidden"), Find(TEXT("spn_test:poi.hofweg_end")).bVisible);
	TestEqual(TEXT("far POI undiscovered"), Find(TEXT("spn_test:poi.hofweg_end")).Discovery, ESpnDiscovery::Undiscovered);

	// at the start (-10550, -4700) cm the Birkenweg POI at (-10200, -7000) cm lies 23 m away: known, not visited
	Map.ObservePlayer(FVector(-10550.0, -4700.0, 100.0));
	TestTrue(TEXT("23 m away: known"), Find(TEXT("spn_test:poi.birkenweg")).Discovery == ESpnDiscovery::Known);
	TestTrue(TEXT("and now visible"), Find(TEXT("spn_test:poi.birkenweg")).bVisible);
	TestFalse(TEXT("the far one is still hidden"), Find(TEXT("spn_test:poi.hofweg_end")).bVisible);
	TestEqual(TEXT("the test district counts as visited"), Map.GetRegionLabel(), FString(TEXT("Test district (placeholder)")));

	// walking up to it visits it; discovery never goes down
	Map.ObservePlayer(FVector(-10210.0, -6990.0, 100.0));
	TestEqual(TEXT("visited from 10 cm away"), Find(TEXT("spn_test:poi.birkenweg")).Discovery, ESpnDiscovery::Visited);
	Map.ObservePlayer(FVector(5000.0, 5000.0, 100.0));
	TestEqual(TEXT("walking away does not un-discover"), Find(TEXT("spn_test:poi.birkenweg")).Discovery, ESpnDiscovery::Visited);

	// a tiled world draws only a rectangle: QueryVisible returns just what lies inside it
	TArray<FSpnMapPoiView> Pois;
	TArray<FSpnMapMarkerView> Markers;
	Map.QueryVisible(FVector2D(-110, 40), FVector2D(-100, 55), Pois, Markers);
	TestTrue(TEXT("the start POI lies in the small rectangle"), Pois.ContainsByPredicate([](const FSpnMapPoiView& P) { return P.Id == TEXT("spn_test:poi.player_start"); }));
	TestFalse(TEXT("a distant POI does not"), Pois.ContainsByPredicate([](const FSpnMapPoiView& P) { return P.Id == TEXT("spn_test:poi.east_lot"); }));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnMapSaveTest, "SpnCore.Map.Save", Flags)
bool FSpnMapSaveTest::RunTest(const FString&)
{
	FEnv Env;
	USpnMapSubsystem& Map = Env.Map();
	USpnSaveSubsystem& Save = Env.Save();
	Map.InstallTestDefinition();
	int64 Id = 0;
	Map.SetWaypointAt(FVector2D(12.5, -33.25), TEXT("Saved"), Id);
	Map.ObservePlayer(FVector(-10550.0, -4700.0, 100.0));

	const FSpnSaveStatus Saved = Save.SaveSlot(TEXT("slot_map"));
	TestTrue(TEXT("saved with a map"), Saved.IsOk());
	FSpnMapMarkerView Before;
	Map.GetWaypoint(Before);

	// change the state, then load: the waypoint and discovery come back
	Map.ClearWaypoint();
	Map.InstallTestDefinition(); // fresh state: no discovery either
	TestEqual(TEXT("cleared"), Map.GetMarkers().Num(), 0);
	int32 Replaced = 0;
	Map.OnMapReplaced.AddLambda([&Replaced]() { ++Replaced; });
	const FSpnSaveStatus Loaded = Save.LoadSlot(TEXT("slot_map"));
	TestTrue(TEXT("loaded"), Loaded.IsOk());
	TestTrue(TEXT("the map was restored"), Loaded.bMapRestored);
	TestEqual(TEXT("listeners were told the map was replaced"), Replaced, 1);
	FSpnMapMarkerView After;
	TestTrue(TEXT("the waypoint is back"), Map.GetWaypoint(After));
	TestEqual(TEXT("same id"), After.Id, Before.Id);
	TestEqual(TEXT("same label"), After.Label, Before.Label);
	TestTrue(TEXT("same position"), After.Metres.Equals(Before.Metres, 1e-9));
	bool bKnown = false;
	for (const FSpnMapPoiView& P : Map.GetPois()) { if (P.Id == TEXT("spn_test:poi.birkenweg")) { bKnown = P.Discovery != ESpnDiscovery::Undiscovered; } }
	TestTrue(TEXT("discovery is back"), bKnown);

	// the slot list sees it and says it is loadable
	TestTrue(TEXT("slot listed"), Save.ListSlots().Contains(TEXT("slot_map")));
	const USpnSaveSubsystem::FSpnSlotInfo Info = Save.InspectSlot(TEXT("slot_map"));
	TestEqual(TEXT("inspect: loadable"), Info.Result, ESpnSaveResult::Ok);
	TestTrue(TEXT("inspect: map section present"), Info.bHasMap);
	TestEqual(TEXT("inspect: one marker"), Info.MapMarkers, 1);

	// a slot with a map cannot be loaded while the map has no definition (it would be lost): refused, nothing changes
	Map.Clear();
	const FSpnSaveStatus Refused = Save.LoadSlot(TEXT("slot_map"));
	TestEqual(TEXT("without a definition the slot is not integrated"), Refused.Result, ESpnSaveResult::SectionsNotIntegrated);
	TestEqual(TEXT("inspect says the same"), Save.InspectSlot(TEXT("slot_map")).Result, ESpnSaveResult::SectionsNotIntegrated);
	Map.InstallTestDefinition();

	// a save from before the map existed (no world.map): loads, and the markers are cleared and reported
	Map.Clear();
	TestTrue(TEXT("save without a map"), Save.SaveSlot(TEXT("slot_old")).IsOk());
	Map.InstallTestDefinition();
	Map.SetWaypointAt(FVector2D(1, 1), TEXT("will be cleared"), Id);
	const FSpnSaveStatus Old = Save.LoadSlot(TEXT("slot_old"));
	TestTrue(TEXT("old slot loads"), Old.IsOk());
	TestTrue(TEXT("the reset is reported, never silent"), Old.bMapResetToEmpty);
	TestEqual(TEXT("and the markers are gone"), Map.GetMarkers().Num(), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnMapOldSpnWorldSaveTest, "SpnCore.Map.OldSpnWorldSaveWithGeneratedMap", Flags)
bool FSpnMapOldSpnWorldSaveTest::RunTest(const FString&)
{
	// 2026-11-14: no SpnWorld save from before the generated map data exists on this machine (every SpnWorld save was written after the change), so
	// the old save is produced the way the build of 2026-11-13 produced it in Lvl_SpnWorld: the TEST definition installed, a waypoint on one of its
	// POIs, discovery from walking. Then the current build's map (the generated SpnWorld data) loads it.
	const FString Generated = USpnMapSubsystem::GetSpnWorldMapPath();
	if (!FPaths::FileExists(Generated))
	{
		AddWarning(FString::Printf(TEXT("%s is missing (world not built): test skipped"), *Generated));
		return true;
	}
	FEnv Env;
	USpnMapSubsystem& Map = Env.Map();
	USpnSaveSubsystem& Save = Env.Save();
	Map.InstallTestDefinition();
	int64 Id = 0;
	TestEqual(TEXT("old build: waypoint on a test POI"), Map.SetWaypointToPoi(TEXT("spn_test:poi.storage_road"), Id), ESpnMapResult::Ok);
	Map.ObservePlayer(FVector(-10550.0, -4700.0, 100.0));   // the test definition's start: discovers the Birkenweg POI
	Map.ObservePlayer(FVector(-12400.0, 32200.0, 100.0));   // where the route's save point lies in SpnWorld (outside the test district)
	FSpnMapMarkerView Before;
	Map.GetWaypoint(Before);
	TestTrue(TEXT("old build: saved"), Save.SaveSlot(TEXT("slot_old_spnworld")).IsOk());

	FString Detail;
	TestEqual(TEXT("current build: the generated SpnWorld map installs"), Map.InstallGeneratedRegion(Generated, FSpnWorldFrame(), &Detail), ESpnMapResult::Ok);
	TestEqual(TEXT("inspect: the old slot is loadable"), Save.InspectSlot(TEXT("slot_old_spnworld")).Result, ESpnSaveResult::Ok);
	const FSpnSaveStatus Loaded = Save.LoadSlot(TEXT("slot_old_spnworld"));
	TestTrue(*FString::Printf(TEXT("the old SpnWorld slot loads (%s %s)"), LexToString(Loaded.Result), *Loaded.Detail), Loaded.IsOk());
	TestTrue(TEXT("the map state was restored, not reset"), Loaded.bMapRestored && !Loaded.bMapResetToEmpty);
	TestTrue(TEXT("still the generated map afterwards"), Map.IsGeneratedRegion());
	FSpnMapMarkerView After;
	TestTrue(TEXT("the old waypoint is back"), Map.GetWaypoint(After));
	TestTrue(TEXT("same position"), After.Metres.Equals(Before.Metres, 1e-6));
	bool bKnownHidden = false, bAnyLegacyVisible = false;
	for (const FSpnMapPoiView& P : Map.GetPois())
	{
		if (P.Id == TEXT("spn_test:poi.birkenweg")) { bKnownHidden = P.Discovery != ESpnDiscovery::Undiscovered && !P.bVisible; }
		if (P.Id.StartsWith(TEXT("spn_test:")) && P.bVisible) { bAnyLegacyVisible = true; }
	}
	TestTrue(TEXT("old discovery kept (the old POI stays hidden)"), bKnownHidden);
	TestFalse(TEXT("no old test POI is shown on the SpnWorld map"), bAnyLegacyVisible);

	// the reverse case (open risk of 2026-11-14): a CURRENT SpnWorld save judged against the test definition (another level's map) is refused
	// as a whole and changes nothing
	Map.ObservePlayer(FVector(-28174.0, 44052.0, 700.0));   // at the cabin: discovers generated SpnWorld POIs
	TestTrue(TEXT("new SpnWorld slot saved"), Save.SaveSlot(TEXT("slot_new_spnworld")).IsOk());
	Map.InstallTestDefinition();
	int64 Probe = 0;
	Map.SetWaypointAt(FVector2D(3.0, 4.0), TEXT("unchanged"), Probe);
	const USpnSaveSubsystem::FSpnSlotInfo Info = Save.InspectSlot(TEXT("slot_new_spnworld"));
	TestEqual(*FString::Printf(TEXT("inspect from another map: another game world, not damage (%s %s)"), LexToString(Info.Result), *Info.Detail), Info.Result, ESpnSaveResult::OtherWorld);
	const FSpnSaveStatus Refused = Save.LoadSlot(TEXT("slot_new_spnworld"));
	TestEqual(*FString::Printf(TEXT("load from another map refused as another game world (%s %s)"), LexToString(Refused.Result), *Refused.Detail), Refused.Result, ESpnSaveResult::OtherWorld);
	FSpnMapMarkerView Kept;
	TestTrue(TEXT("the current map state is untouched"), Map.GetWaypoint(Kept) && Kept.Label == TEXT("unchanged"));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnSaveInspectTest, "SpnCore.Save.InspectSlot", Flags)
bool FSpnSaveInspectTest::RunTest(const FString&)
{
	FEnv Env;
	USpnSaveSubsystem& Save = Env.Save();
	TestEqual(TEXT("missing slot"), Save.InspectSlot(TEXT("nothing_here")).Result, ESpnSaveResult::NotFound);
	TestFalse(TEXT("missing slot does not exist"), Save.InspectSlot(TEXT("nothing_here")).bExists);
	TestEqual(TEXT("bad slot id"), Save.InspectSlot(TEXT("Bad Slot")).Result, ESpnSaveResult::InvalidSlotId);
	TestTrue(TEXT("a plain save"), Save.SaveSlot(TEXT("slot_a")).IsOk());
	TestEqual(TEXT("plain save inspects fine"), Save.InspectSlot(TEXT("slot_a")).Result, ESpnSaveResult::Ok);

	// damage the file: no previous generation yet -> corrupt, and the live state is untouched
	const FString File = Save.GetSlotFilePath(TEXT("slot_a"));
	TArray<uint8> Bytes;
	TestTrue(TEXT("read"), FFileHelper::LoadFileToArray(Bytes, *File));
	Bytes[Bytes.Num() / 2] ^= 0x55;
	TestTrue(TEXT("write damaged"), FFileHelper::SaveArrayToFile(Bytes, *File));
	const USpnSaveSubsystem::FSpnSlotInfo Damaged = Save.InspectSlot(TEXT("slot_a"));
	TestTrue(TEXT("exists"), Damaged.bExists);
	TestEqual(TEXT("damaged: corrupt"), Damaged.Result, ESpnSaveResult::Corrupt);
	TestEqual(TEXT("loading it fails the same way"), Save.LoadSlot(TEXT("slot_a")).Result, ESpnSaveResult::Corrupt);

	// a second save keeps the previous generation; damaging only the main file falls back, and inspect says so
	TestTrue(TEXT("restore the file by saving over it"), Save.SaveSlot(TEXT("slot_b")).IsOk());
	TestTrue(TEXT("and again for a previous generation"), Save.SaveSlot(TEXT("slot_b")).IsOk());
	const FString FileB = Save.GetSlotFilePath(TEXT("slot_b"));
	TArray<uint8> BytesB;
	FFileHelper::LoadFileToArray(BytesB, *FileB);
	BytesB[BytesB.Num() / 2] ^= 0x55;
	FFileHelper::SaveArrayToFile(BytesB, *FileB);
	const USpnSaveSubsystem::FSpnSlotInfo Fallback = Save.InspectSlot(TEXT("slot_b"));
	TestEqual(TEXT("fallback: loadable"), Fallback.Result, ESpnSaveResult::Ok);
	TestTrue(TEXT("fallback is reported"), Fallback.bFromPrevious);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnSaveCatalogMismatchTest, "SpnCore.Save.CatalogMismatchFeedback", Flags)
bool FSpnSaveCatalogMismatchTest::RunTest(const FString&)
{
	FEnv Env;
	USpnInventorySubsystem& Inventory = Env.Inventory();
	USpnSaveSubsystem& Save = Env.Save();
	FString Json;
	if (!FFileHelper::LoadFileToString(Json, *USpnInventorySubsystem::GetTestCatalogPath()))
	{
		AddError(TEXT("the test catalog file is missing"));
		return false;
	}
	TestTrue(TEXT("catalog A"), Inventory.SetCatalogFromJson(Json).IsOk());
	TestTrue(TEXT("player container"), Inventory.CreateContainerFromPreset(1, TEXT("spn_test:preset.body")).IsOk());
	TestTrue(TEXT("an item"), Inventory.AddItem(1, TEXT("spn_test:item.ration"), 2).IsOk());
	TestTrue(TEXT("saved"), Save.SaveSlot(TEXT("slot_cat")).IsOk());
	TestEqual(TEXT("same catalog: ok"), Save.InspectSlot(TEXT("slot_cat")).Result, ESpnSaveResult::Ok);
	TestEqual(TEXT("fingerprint matches"), Save.InspectSlot(TEXT("slot_cat")).CatalogCheck, ESpnCatalogCheck::Match);

	// catalog B: a ration weighs 201 g instead of 200 g
	Inventory.ResetInventory();
	const FString Changed = Json.Replace(TEXT("\"weight_g\": 200,"), TEXT("\"weight_g\": 201,"));
	TestTrue(TEXT("the replacement changed the text"), Changed != Json);
	TestTrue(TEXT("catalog B installs"), Inventory.SetCatalogFromJson(Changed).IsOk());
	Inventory.CreateContainerFromPreset(1, TEXT("spn_test:preset.body"));
	const uint64 RevisionBefore = Inventory.GetRevision();
	TestEqual(TEXT("inspect: refused, never accepted automatically"), Save.InspectSlot(TEXT("slot_cat")).Result, ESpnSaveResult::CatalogMismatch);
	const FSpnSaveStatus Refused = Save.LoadSlot(TEXT("slot_cat"));
	TestEqual(TEXT("load: refused"), Refused.Result, ESpnSaveResult::CatalogMismatch);
	TestEqual(TEXT("a refused load changes nothing"), Inventory.GetRevision(), RevisionBefore);
	TestEqual(TEXT("and loads no item"), Inventory.CountOf(1, TEXT("spn_test:item.ration")), 0ull);

	FSpnLoadOptions Accept;
	Accept.bAcceptCatalogMismatch = true;
	TestEqual(TEXT("inspect with explicit acceptance"), Save.InspectSlot(TEXT("slot_cat"), Accept).Result, ESpnSaveResult::Ok);
	const FSpnSaveStatus Accepted = Save.LoadSlot(TEXT("slot_cat"), Accept);
	TestTrue(TEXT("accepted load"), Accepted.IsOk());
	TestEqual(TEXT("reported as an accepted mismatch"), Accepted.CatalogCheck, ESpnCatalogCheck::MismatchAccepted);
	TestEqual(TEXT("the items are back, validated against the new definitions"), Inventory.CountOf(1, TEXT("spn_test:item.ration")), 2ull);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
