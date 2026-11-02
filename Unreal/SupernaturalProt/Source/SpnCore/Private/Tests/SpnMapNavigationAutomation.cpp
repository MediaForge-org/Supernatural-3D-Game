#include "Engine/GameInstance.h"
#include "HAL/PlatformTime.h"
#include "Misc/AutomationTest.h"
#include "SpnMapSubsystem.h"
#include "SpnWorldFrame.h"
#include "UObject/Package.h"

#if WITH_DEV_AUTOMATION_TESTS

// Unreal checks for road navigation on the SYNTHETIC test region (NightBuild03): coordinate adapter, waypoint -> road route, re-plan after a
// deviation, closures, unreachable targets, render-frame data and timing. Pure subsystem tests (no level, no widgets, headless).
// Expected lengths come from Docs/NightBuild02/MapNavigation/TEST_REGION.md (asserted by the Core tests as well).

namespace
{
	constexpr EAutomationTestFlags Flags = EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter;

	struct FNavEnv
	{
		UGameInstance* Instance = nullptr;
		USpnMapSubsystem* Map = nullptr;

		explicit FNavEnv(const FSpnWorldFrame& Frame = FSpnWorldFrame())
		{
			Instance = NewObject<UGameInstance>(GetTransientPackage());
			Instance->AddToRoot();
			Instance->Init();
			Map = Instance->GetSubsystem<USpnMapSubsystem>();
			Map->InstallSyntheticRegion(Frame);
		}
		~FNavEnv()
		{
			Instance->Shutdown();
			Instance->RemoveFromRoot();
		}

		FVector Cm(const FVector2D& Metres, double ZCm = 100.0) const
		{
			FVector Out;
			Map->MapToUnreal(Metres, static_cast<float>(ZCm), Out);
			return Out;
		}
		FVector2D Pos(const TCHAR* Key) const
		{
			FVector2D Out = FVector2D::ZeroVector;
			Map->GetSyntheticPosition(Key, Out);
			return Out;
		}
		/** Puts the player at a map position and lets the subsystem see it. */
		void PlacePlayer(const FVector2D& Metres) const
		{
			const FVector At = Cm(Metres);
			Map->ObservePlayer(At);
			Map->UpdateNavigation(At, 0.f);
		}
	};
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNavFrameTest, "SpnCore.Navigation.CoordinateAdapter", Flags)
bool FSpnNavFrameTest::RunTest(const FString&)
{
	// EastSouthUp: +X east, +Y south; origin offset shifts the canonical point that sits at Unreal (0,0,0)
	FSpnWorldFrame Frame;
	Frame.Axes = ESpnWorldAxes::EastSouthUp;
	Frame.OriginMetres = FVector(1000.0, 500.0, 0.0);
	FVector Unreal;
	TestTrue(TEXT("ToUnreal"), SpnWorldFrame::ToUnreal(Frame, FVector(1500.0, 800.0, 12.0), Unreal));
	TestEqual(TEXT("east metres -> +X cm"), Unreal.X, 50000.0, 1e-6);
	TestEqual(TEXT("north metres -> -Y cm (south positive)"), Unreal.Y, -30000.0, 1e-6);
	TestEqual(TEXT("height metres -> Z cm"), Unreal.Z, 1200.0, 1e-6);
	FVector World;
	TestTrue(TEXT("ToWorld"), SpnWorldFrame::ToWorld(Frame, Unreal, World));
	TestEqual(TEXT("round trip X"), World.X, 1500.0, 1e-6);
	TestEqual(TEXT("round trip Y"), World.Y, 800.0, 1e-6);
	TestEqual(TEXT("round trip Z"), World.Z, 12.0, 1e-6);

	FSpnWorldFrame Alt;
	Alt.Axes = ESpnWorldAxes::NorthEastUp;
	TestTrue(TEXT("NorthEastUp ToUnreal"), SpnWorldFrame::ToUnreal(Alt, FVector(100.0, 200.0, 0.0), Unreal));
	TestEqual(TEXT("north -> +X"), Unreal.X, 20000.0, 1e-6);
	TestEqual(TEXT("east -> +Y"), Unreal.Y, 10000.0, 1e-6);

	// heading: north is yaw -90 in EastSouthUp (heading east = yaw 0), yaw 0 in NorthEastUp
	double Yaw = 0.0;
	TestTrue(TEXT("bearing north"), SpnWorldFrame::BearingToYawDegrees(ESpnWorldAxes::EastSouthUp, 0.0, Yaw));
	TestEqual(TEXT("north = yaw -90"), Yaw, -90.0, 1e-6);
	TestTrue(TEXT("bearing east"), SpnWorldFrame::BearingToYawDegrees(ESpnWorldAxes::EastSouthUp, UE_DOUBLE_PI * 0.5, Yaw));
	TestEqual(TEXT("east = yaw 0"), Yaw, 0.0, 1e-6);
	double Bearing = 0.0;
	TestTrue(TEXT("yaw to bearing"), SpnWorldFrame::YawDegreesToBearing(ESpnWorldAxes::EastSouthUp, 90.0, Bearing));
	TestEqual(TEXT("yaw 90 (south) = bearing pi"), Bearing, UE_DOUBLE_PI, 1e-9);

	// non-finite input is refused and leaves the output untouched
	FVector Untouched(1, 2, 3);
	TestFalse(TEXT("NaN refused"), SpnWorldFrame::ToUnreal(Frame, FVector(NAN, 0, 0), Untouched));
	TestEqual(TEXT("output untouched"), Untouched.X, 1.0, 1e-12);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNavMapRoundTripTest, "SpnCore.Navigation.WorldToMapRoundTrip", Flags)
bool FSpnNavMapRoundTripTest::RunTest(const FString&)
{
	FSpnWorldFrame Frame;
	Frame.OriginMetres = FVector(250.0, -75.0, 0.0);
	FNavEnv Env(Frame);
	for (const TCHAR* Key : {TEXT("westStreet"), TEXT("eastStreet"), TEXT("shopInside"), TEXT("hamletTarget")})
	{
		const FVector2D Metres = Env.Pos(Key);
		FVector Cm;
		TestTrue(TEXT("map -> unreal"), Env.Map->MapToUnreal(Metres, 150.f, Cm));
		FVector2D Back;
		TestTrue(TEXT("unreal -> map"), Env.Map->UnrealToMap(Cm, Back));
		TestEqual(FString::Printf(TEXT("%s x"), Key), Back.X, Metres.X, 1e-6);
		TestEqual(FString::Printf(TEXT("%s y"), Key), Back.Y, Metres.Y, 1e-6);
		TestEqual(TEXT("height is the caller's"), static_cast<double>(Cm.Z), 150.0, 1e-3);
	}
	// known value: west street (500, 0) with origin (250, -75) sits at Unreal X = 25000 cm, Y = -7500 cm (75 m north of the origin -> south-negative)
	const FVector At = Env.Cm(FVector2D(500.0, 0.0));
	TestEqual(TEXT("X"), At.X, 25000.0, 1e-6);
	TestEqual(TEXT("Y"), At.Y, -7500.0, 1e-6);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNavRouteTest, "SpnCore.Navigation.WaypointRoadRoute", Flags)
bool FSpnNavRouteTest::RunTest(const FString&)
{
	FNavEnv Env;
	USpnMapSubsystem& Map = *Env.Map;
	TestTrue(TEXT("synthetic region has a road network"), Map.HasRoadNetwork());
	TestTrue(TEXT("flagged as test data"), Map.IsTestDefinition());
	Env.PlacePlayer(Env.Pos(TEXT("westStreet")));

	int64 Id = 0;
	TestEqual(TEXT("waypoint across the river"), Map.SetWaypointAt(Env.Pos(TEXT("eastStreet")), TEXT("East"), Id), ESpnMapResult::Ok);
	FSpnRouteView Route;
	TestTrue(TEXT("a route exists"), Map.GetRoute(Route));
	TestEqual(TEXT("driving route over the north bridge is 3,400 m"), Route.LengthMetres, 3400.0, 1.0);
	TestTrue(TEXT("the route is not the 1,000 m air line"), Route.LengthMetres > 3.0 * Route.StraightLineMetres);
	TestTrue(TEXT("it follows road segments"), Route.RoadSegmentIds.Num() > 0 && Route.RoadSegmentIds.Contains(106));
	TestTrue(TEXT("route starts at the player"), FVector2D::Distance(Route.Points[0], Env.Pos(TEXT("westStreet"))) < 1.0);

	const FSpnNavigationReading Reading = Map.GetNavigation(Env.Cm(Env.Pos(TEXT("westStreet"))), 0.f);
	TestTrue(TEXT("reading active"), Reading.bActive);
	TestFalse(TEXT("reading is NOT a straight line"), Reading.bStraightLine);
	TestEqual(TEXT("status"), Reading.Status, ESpnNavStatus::Active);
	TestEqual(TEXT("remaining distance = route length"), Reading.DistanceMetres, Route.LengthMetres, 1.0);
	TestTrue(TEXT("the compass has a point on the road"), Reading.bHasNextPoint);

	// on foot the shorter foot bridge is used
	Map.SetTravelMode(ESpnTravelMode::OnFoot);
	TestTrue(TEXT("route after mode change"), Map.GetRoute(Route));
	TestEqual(TEXT("on foot: 1,600 m over the foot bridge"), Route.LengthMetres, 1600.0, 1.0);
	Map.SetTravelMode(ESpnTravelMode::Driving);

	// cancel: no route, no waypoint
	TestTrue(TEXT("clear waypoint"), Map.ClearWaypoint());
	TestFalse(TEXT("no route after cancel"), Map.GetRoute(Route));
	FSpnMapMarkerView Marker;
	TestFalse(TEXT("no waypoint after cancel"), Map.GetWaypoint(Marker));
	TestFalse(TEXT("reading inactive"), Map.GetNavigation(Env.Cm(Env.Pos(TEXT("westStreet"))), 0.f).bActive);

	// changing the target replaces the route
	int64 First = 0, Second = 0;
	Map.SetWaypointAt(Env.Pos(TEXT("eastStreet")), TEXT("A"), First);
	const double LengthA = Map.GetRoute(Route) ? Route.LengthMetres : -1.0;
	Map.SetWaypointAt(Env.Pos(TEXT("westOpenField")), TEXT("B"), Second);
	TestTrue(TEXT("second target routes"), Map.GetRoute(Route));
	TestTrue(TEXT("a different, shorter route"), Route.LengthMetres < LengthA);
	TestEqual(TEXT("exactly one waypoint"), Map.GetMarkers().Num(), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNavClosureTest, "SpnCore.Navigation.ClosureAndReroute", Flags)
bool FSpnNavClosureTest::RunTest(const FString&)
{
	FNavEnv Env;
	USpnMapSubsystem& Map = *Env.Map;
	Env.PlacePlayer(Env.Pos(TEXT("westStreet")));
	int64 Id = 0;
	Map.SetWaypointAt(Env.Pos(TEXT("eastStreet")), TEXT("East"), Id);
	FSpnRouteView Before;
	TestTrue(TEXT("route before"), Map.GetRoute(Before));
	const uint64 GenerationBefore = Map.GetRouteGeneration();

	TestFalse(TEXT("an unknown segment cannot be closed"), Map.SetRoadClosed(99999, true));
	TestTrue(TEXT("close the north bridge"), Map.SetRoadClosed(106, true));
	TestTrue(TEXT("closed"), Map.IsRoadClosed(106));
	FSpnRouteView After;
	TestTrue(TEXT("a route still exists (tunnel + roundabout)"), Map.GetRoute(After));
	TestEqual(TEXT("detour is 4,680.4 m"), After.LengthMetres, 4680.4, 1.0);
	TestFalse(TEXT("the closed bridge is not used"), After.RoadSegmentIds.Contains(106));
	TestTrue(TEXT("the route generation changed (UI redraw key)"), Map.GetRouteGeneration() != GenerationBefore);

	// the render frame marks the closed road
	FSpnMapRenderRequest Request;
	Request.CenterMetres = FVector2D(1000.0, 600.0);
	Request.MetresPerPixel = 4.0;
	Request.HalfSizePx = FVector2D(600.0, 400.0);
	const FSpnMapRenderFrame Frame = Map.BuildMapRenderFrame(Request);
	TestTrue(TEXT("frame valid"), Frame.bValid);
	const FSpnRenderPolyline* Bridge = Frame.Roads.FindByPredicate([](const FSpnRenderPolyline& R) { return R.Id == 106; });
	TestTrue(TEXT("the bridge is in the frame"), Bridge != nullptr);
	TestTrue(TEXT("and flagged closed"), Bridge != nullptr && Bridge->bClosed);

	// reopening keeps a valid route (the session re-plans only when the current route is affected)
	TestTrue(TEXT("reopen"), Map.SetRoadClosed(106, false));
	TestFalse(TEXT("no longer closed"), Map.IsRoadClosed(106));
	FSpnRouteView Restored;
	TestTrue(TEXT("route after reopening"), Map.GetRoute(Restored));
	TestEqual(TEXT("still active"), Map.GetNavigation(Env.Cm(Env.Pos(TEXT("westStreet"))), 0.f).Status, ESpnNavStatus::Active);

	// closing the only crossings (bridge + both tunnel ramps' tunnel) leaves no driving route: an explicit NoRoute, not a straight line
	TestTrue(TEXT("close bridge"), Map.SetRoadClosed(106, true));
	TestTrue(TEXT("close tunnel"), Map.SetRoadClosed(133, true));
	const FSpnNavigationReading Cut = Map.GetNavigation(Env.Cm(Env.Pos(TEXT("westStreet"))), 0.f);
	TestEqual(TEXT("no vehicle crossing left"), Cut.Status, ESpnNavStatus::NoRoute);
	TestEqual(TEXT("two closed roads"), Map.GetClosedRoads().Num(), 2);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNavDeviationTest, "SpnCore.Navigation.DeviationReroute", Flags)
bool FSpnNavDeviationTest::RunTest(const FString&)
{
	FNavEnv Env;
	USpnMapSubsystem& Map = *Env.Map;
	Env.PlacePlayer(Env.Pos(TEXT("westStreet")));
	int64 Id = 0;
	Map.SetWaypointAt(Env.Pos(TEXT("eastStreet")), TEXT("East"), Id);
	FSpnRouteView First;
	TestTrue(TEXT("route"), Map.GetRoute(First));
	const uint64 Generation = Map.GetRouteGeneration();

	// moving along the route keeps it (same generation) and shortens the remaining distance
	const FVector2D OnRoute = First.Points[FMath::Min(3, First.Points.Num() - 1)];
	Map.AdvanceNavigationClock(5.0);
	Env.PlacePlayer(OnRoute);
	TestEqual(TEXT("route kept while on the road"), Map.GetRouteGeneration(), Generation);
	const FSpnNavigationReading Progress = Map.GetNavigation(Env.Cm(OnRoute), 0.f);
	TestTrue(TEXT("remaining distance shrank"), Progress.DistanceMetres < First.LengthMetres);

	// driving off the road onto another street re-plans from there (after the cooldown)
	const FVector2D Elsewhere = Env.Pos(TEXT("westOpenField"));
	Map.AdvanceNavigationClock(5.0);
	Env.PlacePlayer(Elsewhere);
	TestTrue(TEXT("a new route was planned"), Map.GetRouteGeneration() != Generation);
	FSpnRouteView Second;
	TestTrue(TEXT("route after deviation"), Map.GetRoute(Second));
	TestTrue(TEXT("starts at the new position"), FVector2D::Distance(Second.Points[0], Elsewhere) < 2.0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNavUnreachableTest, "SpnCore.Navigation.UnreachableTargets", Flags)
bool FSpnNavUnreachableTest::RunTest(const FString&)
{
	FNavEnv Env;
	USpnMapSubsystem& Map = *Env.Map;
	Env.PlacePlayer(Env.Pos(TEXT("westStreet")));
	int64 Id = 0;

	TestEqual(TEXT("island: waypoint is still created"), Map.SetWaypointAt(Env.Pos(TEXT("islandTarget")), TEXT("Island"), Id), ESpnMapResult::Ok);
	FSpnNavigationReading Reading = Map.GetNavigation(Env.Cm(Env.Pos(TEXT("westStreet"))), 0.f);
	TestEqual(TEXT("island: NoRoute"), Reading.Status, ESpnNavStatus::NoRoute);
	TestEqual(TEXT("island: reason"), Reading.FailureText, FString(TEXT("DestinationNotAccessible")));
	TestEqual(TEXT("no invented distance"), Reading.DistanceMetres, 0.0);
	FSpnRouteView Route;
	TestFalse(TEXT("no route drawn"), Map.GetRoute(Route));
	const FSpnMapRenderFrame Frame = Map.BuildMapRenderFrame([] { FSpnMapRenderRequest R; R.CenterMetres = FVector2D(1000, 0); R.MetresPerPixel = 4.0; return R; }());
	TestEqual(TEXT("no route overlay in the frame"), Frame.RouteRemaining.Num() + Frame.RouteTravelled.Num(), 0);

	Map.SetWaypointAt(Env.Pos(TEXT("hamletTarget")), TEXT("Hamlet"), Id);
	Reading = Map.GetNavigation(Env.Cm(Env.Pos(TEXT("westStreet"))), 0.f);
	TestEqual(TEXT("hamlet: NoRoute"), Reading.Status, ESpnNavStatus::NoRoute);
	TestEqual(TEXT("hamlet: separate network"), Reading.FailureText, FString(TEXT("NoPath")));

	// outside the map: refused, nothing changes
	int64 Rejected = 0;
	TestEqual(TEXT("far outside"), Map.SetWaypointAt(FVector2D(1.0e7, 0.0), TEXT("far"), Rejected), ESpnMapResult::OutOfBounds);
	TestEqual(TEXT("no marker id"), Rejected, static_cast<int64>(0));

	// a failed plan recovers when the target changes
	Map.SetWaypointAt(Env.Pos(TEXT("eastStreet")), TEXT("East"), Id);
	TestEqual(TEXT("recovers"), Map.GetNavigation(Env.Cm(Env.Pos(TEXT("westStreet"))), 0.f).Status, ESpnNavStatus::Active);

	double Length = 0.0;
	FString Failure;
	TestFalse(TEXT("PlanRouteLength island"), Map.PlanRouteLength(Env.Pos(TEXT("westStreet")), Env.Pos(TEXT("islandTarget")), Length, Failure));
	TestEqual(TEXT("failure text"), Failure, FString(TEXT("DestinationNotAccessible")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNavRenderFrameTest, "SpnCore.Navigation.RenderFrameZoomRotation", Flags)
bool FSpnNavRenderFrameTest::RunTest(const FString&)
{
	FNavEnv Env;
	USpnMapSubsystem& Map = *Env.Map;
	Env.PlacePlayer(Env.Pos(TEXT("westStreet")));

	FSpnMapRenderRequest Request;
	Request.CenterMetres = FVector2D(500.0, 0.0);
	Request.HalfSizePx = FVector2D(400.0, 300.0);
	Request.MetresPerPixel = 2.0;
	const FSpnMapRenderFrame Near = Map.BuildMapRenderFrame(Request);
	TestTrue(TEXT("near frame valid"), Near.bValid);
	TestEqual(TEXT("metres per pixel is the requested one"), Near.MetresPerPixel, 2.0, 1e-9);
	TestTrue(TEXT("roads present"), Near.Roads.Num() > 0);
	TestTrue(TEXT("buildings present when zoomed in"), Near.Buildings.Num() > 0);
	TestEqual(TEXT("north up: north points up"), Near.NorthOnScreenRadians, 0.0, 1e-9);

	// the player's own street is drawn through the centre: some road vertex lies near (0,0)
	double Nearest = 1.0e9;
	for (const FSpnRenderPolyline& Road : Near.Roads)
	{
		for (const FVector2D& P : Road.Points) { Nearest = FMath::Min(Nearest, P.Size()); }
	}
	TestTrue(TEXT("centre sits on the road network"), Nearest < 60.0);

	Request.MetresPerPixel = 16.0;
	const FSpnMapRenderFrame Far = Map.BuildMapRenderFrame(Request);
	TestEqual(TEXT("zoomed out"), Far.MetresPerPixel, 16.0, 1e-9);
	TestEqual(TEXT("level of detail drops the buildings far out"), Far.Buildings.Num(), 0);
	TestTrue(TEXT("but keeps the roads"), Far.Roads.Num() > 0);

	// a point 100 m east of the centre is 50 px to the right at 2 m/px (north up)
	Request.MetresPerPixel = 2.0;
	Request.CenterMetres = FVector2D(500.0, 0.0);
	// heading up with the player heading east (bearing pi/2): north must point to the screen's left (-pi/2 rotation of the north arrow)
	Request.bHeadingUp = true;
	Request.HeadingBearingRadians = UE_DOUBLE_PI * 0.5;
	const FSpnMapRenderFrame Rotated = Map.BuildMapRenderFrame(Request);
	TestTrue(TEXT("rotated frame valid"), Rotated.bValid);
	TestEqual(TEXT("north is a quarter turn away from up"), FMath::Abs(FMath::UnwindRadians(Rotated.NorthOnScreenRadians)), UE_DOUBLE_PI * 0.5, 1e-6);
	TestTrue(TEXT("the same features are emitted"), FMath::Abs(Rotated.Emitted - Near.Emitted) < Near.Emitted / 2 + 5);

	// route overlay appears with a route and is split at the player's progress
	int64 Id = 0;
	Map.SetWaypointAt(Env.Pos(TEXT("eastStreet")), TEXT("East"), Id);
	Request.bHeadingUp = false;
	Request.MetresPerPixel = 6.0;
	Request.CenterMetres = FVector2D(1000.0, 400.0);
	Request.HalfSizePx = FVector2D(500.0, 400.0);
	const FSpnMapRenderFrame WithRoute = Map.BuildMapRenderFrame(Request);
	TestTrue(TEXT("route remaining is drawn"), WithRoute.RouteRemaining.Num() > 0);
	TestTrue(TEXT("destination marker has a screen position"), WithRoute.bHasDestination);

	// invalid input is refused, not drawn
	FSpnMapRenderRequest Bad = Request;
	Bad.CenterMetres = FVector2D(NAN, 0.0);
	TestFalse(TEXT("NaN centre"), Map.BuildMapRenderFrame(Bad).bValid);
	Bad = Request;
	Bad.MetresPerPixel = -1.0;
	TestFalse(TEXT("negative scale"), Map.BuildMapRenderFrame(Bad).bValid);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNavPerformanceTest, "SpnCore.Navigation.Performance", Flags)
bool FSpnNavPerformanceTest::RunTest(const FString&)
{
	FNavEnv Env;
	USpnMapSubsystem& Map = *Env.Map;
	Env.PlacePlayer(Env.Pos(TEXT("westStreet")));

	double Length = 0.0;
	FString Failure;
	constexpr int32 PlanRuns = 200;
	const double PlanStart = FPlatformTime::Seconds();
	for (int32 I = 0; I < PlanRuns; ++I)
	{
		(void)Map.PlanRouteLength(Env.Pos(TEXT("westStreet")), Env.Pos(TEXT("eastStreet")), Length, Failure);
	}
	const double PlanMs = (FPlatformTime::Seconds() - PlanStart) * 1000.0 / PlanRuns;

	FSpnMapRenderRequest Request;
	Request.CenterMetres = FVector2D(1000.0, 400.0);
	Request.MetresPerPixel = 3.0;
	Request.HalfSizePx = FVector2D(960.0, 540.0);
	int64 Id = 0;
	Map.SetWaypointAt(Env.Pos(TEXT("eastStreet")), TEXT("East"), Id);
	constexpr int32 FrameRuns = 500;
	int32 Emitted = 0;
	const double FrameStart = FPlatformTime::Seconds();
	for (int32 I = 0; I < FrameRuns; ++I)
	{
		Emitted = Map.BuildMapRenderFrame(Request).Emitted;
	}
	const double FrameMs = (FPlatformTime::Seconds() - FrameStart) * 1000.0 / FrameRuns;

	AddInfo(FString::Printf(TEXT("PERF (Editor build, Development, synthetic region): PlanRoute west->east %.4f ms avg over %d runs; BuildMapRenderFrame 1920x1080 %.4f ms avg over %d runs (%d features)"),
		PlanMs, PlanRuns, FrameMs, FrameRuns, Emitted));
	// generous ceilings: a regression guard, not a performance claim
	TestTrue(TEXT("planning stays far below a frame"), PlanMs < 16.0);
	TestTrue(TEXT("a map frame stays far below a frame"), FrameMs < 16.0);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
