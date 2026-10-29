#include "Misc/AutomationTest.h"
#include "SpnWorldFrame.h"

#include <limits>

#if WITH_DEV_AUTOMATION_TESTS

namespace
{
	constexpr ESpnWorldAxes BothPresets[] = {ESpnWorldAxes::EastSouthUp, ESpnWorldAxes::NorthEastUp};

	const TCHAR* Name(ESpnWorldAxes Axes) { return Axes == ESpnWorldAxes::EastSouthUp ? TEXT("EastSouthUp") : TEXT("NorthEastUp"); }

	bool Near(const FVector& A, const FVector& B, double Tolerance = 1e-6) { return (A - B).Size() <= Tolerance; }
}

// --- known values ------------------------------------------------------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnWorldFrameKnownValues, "SpnCore.WorldFrame.KnownValues", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)
bool FSpnWorldFrameKnownValues::RunTest(const FString& Parameters)
{
	FVector Out;
	FSpnWorldFrame A;
	A.Axes = ESpnWorldAxes::EastSouthUp;
	TestTrue(TEXT("EastSouthUp converts"), SpnWorldFrame::ToUnreal(A, FVector(1.0, 2.0, 3.0), Out));
	TestTrue(TEXT("1 m east, 2 m north, 3 m up -> (100, -200, 300) cm"), Near(Out, FVector(100.0, -200.0, 300.0)));

	FSpnWorldFrame B;
	B.Axes = ESpnWorldAxes::NorthEastUp;
	TestTrue(TEXT("NorthEastUp converts"), SpnWorldFrame::ToUnreal(B, FVector(1.0, 2.0, 3.0), Out));
	TestTrue(TEXT("1 m east, 2 m north, 3 m up -> (200, 100, 300) cm"), Near(Out, FVector(200.0, 100.0, 300.0)));

	FSpnWorldFrame C;
	C.OriginMetres = FVector(1000.0, -2000.0, 5.0);
	TestTrue(TEXT("the origin point converts"), SpnWorldFrame::ToUnreal(C, C.OriginMetres, Out));
	TestTrue(TEXT("the origin sits at Unreal (0, 0, 0)"), Near(Out, FVector::ZeroVector));
	TestTrue(TEXT("one metre east of the origin converts"), SpnWorldFrame::ToUnreal(C, FVector(1001.0, -2000.0, 5.0), Out));
	TestTrue(TEXT("one metre east of the origin is 100 cm along UE +X (EastSouthUp)"), Near(Out, FVector(100.0, 0.0, 0.0)));
	return true;
}

// --- round trip, both presets, far from the origin ----------------------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnWorldFrameRoundTrip, "SpnCore.WorldFrame.RoundTrip", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)
bool FSpnWorldFrameRoundTrip::RunTest(const FString& Parameters)
{
	const FVector Points[] = {FVector(0, 0, 0), FVector(-105.5, 47.0, 1.0), FVector(47000.0, -23500.25, 312.5), FVector(-9.9e6, 9.9e6, -50.0)};
	for (const ESpnWorldAxes Axes : BothPresets)
	{
		FSpnWorldFrame Frame;
		Frame.Axes = Axes;
		for (const FVector& P : Points)
		{
			FVector Ue, Back;
			TestTrue(FString::Printf(TEXT("%s: ToUnreal"), Name(Axes)), SpnWorldFrame::ToUnreal(Frame, P, Ue));
			TestTrue(FString::Printf(TEXT("%s: ToWorld"), Name(Axes)), SpnWorldFrame::ToWorld(Frame, Ue, Back));
			TestTrue(FString::Printf(TEXT("%s: round trip of (%.2f, %.2f, %.2f) within 1 nm"), Name(Axes), P.X, P.Y, P.Z), Near(P, Back, 1e-9));
		}
	}
	return true;
}

// --- invalid input is rejected and never written ----------------------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnWorldFrameRejectsInvalid, "SpnCore.WorldFrame.RejectsInvalid", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)
bool FSpnWorldFrameRejectsInvalid::RunTest(const FString& Parameters)
{
	const FSpnWorldFrame Frame;
	const FVector Sentinel(7.0, 8.0, 9.0);
	FVector Out = Sentinel;
	const double NaN = std::numeric_limits<double>::quiet_NaN();
	const double Inf = std::numeric_limits<double>::infinity();
	TestFalse(TEXT("NaN is rejected"), SpnWorldFrame::ToUnreal(Frame, FVector(NaN, 0, 0), Out));
	TestFalse(TEXT("infinity is rejected"), SpnWorldFrame::ToUnreal(Frame, FVector(0, Inf, 0), Out));
	TestFalse(TEXT("beyond 10,000 km is rejected"), SpnWorldFrame::ToUnreal(Frame, FVector(1.0e8, 0, 0), Out));
	TestFalse(TEXT("an Unreal NaN is rejected"), SpnWorldFrame::ToWorld(Frame, FVector(0, 0, NaN), Out));
	TestTrue(TEXT("a rejected conversion leaves the output untouched"), Out == Sentinel);
	double Yaw = 123.0;
	TestFalse(TEXT("a NaN bearing is rejected"), SpnWorldFrame::BearingToYawDegrees(ESpnWorldAxes::EastSouthUp, NaN, Yaw));
	TestTrue(TEXT("and leaves the yaw untouched"), Yaw == 123.0);
	return true;
}

// --- the claims about Unreal itself: engine rotation math vs. the core's axis map ----------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnWorldFrameEngineDirections, "SpnCore.WorldFrame.EngineDirections", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)
bool FSpnWorldFrameEngineDirections::RunTest(const FString& Parameters)
{
	// Engine facts the presets rely on (never assumed): yaw 0 faces +X and a positive yaw turns towards +Y.
	TestTrue(TEXT("engine: yaw 0 faces +X"), Near(FRotator(0.0, 0.0, 0.0).Vector(), FVector(1, 0, 0), 1e-9));
	TestTrue(TEXT("engine: yaw 90 faces +Y"), Near(FRotator(0.0, 90.0, 0.0).Vector(), FVector(0, 1, 0), 1e-9));
	{
		// measured: the engine matrix path is only float-accurate (x = 1.2e-8 for yaw 90), so this one comparison uses 1e-6, not the 1e-9 of the others
		const FVector ViaMatrix = FRotationMatrix(FRotator(0.0, 90.0, 0.0)).TransformVector(FVector(1, 0, 0));
		AddInfo(FString::Printf(TEXT("FRotationMatrix(yaw 90).TransformVector(+X) = (%.9f, %.9f, %.9f); FRotator::RotateVector(+X) = %s"), ViaMatrix.X, ViaMatrix.Y, ViaMatrix.Z, *FRotator(0.0, 90.0, 0.0).RotateVector(FVector(1, 0, 0)).ToString()));
		TestTrue(TEXT("engine: FRotationMatrix yaw 90 maps +X to +Y"), Near(ViaMatrix, FVector(0, 1, 0), 1e-6));
	}

	struct FCompass { const TCHAR* Label; double BearingDegrees; FVector Step; };
	const FCompass Compass[] = {{TEXT("north"), 0.0, FVector(0, 1, 0)}, {TEXT("east"), 90.0, FVector(1, 0, 0)}, {TEXT("south"), 180.0, FVector(0, -1, 0)}, {TEXT("west"), 270.0, FVector(-1, 0, 0)}};
	for (const ESpnWorldAxes Axes : BothPresets)
	{
		FSpnWorldFrame Frame;
		Frame.Axes = Axes;
		for (const FCompass& C : Compass)
		{
			FVector UeStep;
			double Yaw = 0.0;
			TestTrue(TEXT("a one-metre compass step converts"), SpnWorldFrame::ToUnreal(Frame, C.Step, UeStep));
			TestTrue(TEXT("the bearing converts"), SpnWorldFrame::BearingToYawDegrees(Axes, FMath::DegreesToRadians(C.BearingDegrees), Yaw));
			const FVector Facing = FRotator(0.0, Yaw, 0.0).Vector();
			TestTrue(FString::Printf(TEXT("%s: a character with the yaw of bearing '%s' (%.0f deg) walks along the converted '%s' step"), Name(Axes), C.Label, Yaw, C.Label),
				Near(Facing, UeStep.GetSafeNormal(), 1e-9));
			double Bearing = 0.0;
			TestTrue(TEXT("the yaw converts back"), SpnWorldFrame::YawDegreesToBearing(Axes, Yaw, Bearing));
			TestTrue(TEXT("and gives the same compass bearing"), FMath::Abs(FMath::FindDeltaAngleRadians(Bearing, FMath::DegreesToRadians(C.BearingDegrees))) < 1e-9);
		}
		// East x North = Up in the right-handed canonical frame. The Unreal image of the three axes has triple product -1 because the engine
		// is left-handed: the conversion is a mirror, not a rotation (this is expected and required).
		FVector E, N, U;
		SpnWorldFrame::ToUnreal(Frame, FVector(1, 0, 0), E);
		SpnWorldFrame::ToUnreal(Frame, FVector(0, 1, 0), N);
		SpnWorldFrame::ToUnreal(Frame, FVector(0, 0, 1), U);
		TestTrue(FString::Printf(TEXT("%s: the three axes keep a right-handed triple product of -1 in Unreal's component convention"), Name(Axes)),
			FMath::IsNearlyEqual(FVector::DotProduct(E, FVector::CrossProduct(N, U)) / FMath::Cube(100.0), -1.0, 1e-9));
	}
	return true;
}

// --- known test-map locations ----------------------------------------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnWorldFrameTestMapLocations, "SpnCore.WorldFrame.TestMapLocations", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)
bool FSpnWorldFrameTestMapLocations::RunTest(const FString& Parameters)
{
	// Lvl_SpnMovementTestV3Interiors PlayerStart (Tools/spn_v3_style.py PLAYER_START) and "West Avenue" / "North Street" of the shared district layout
	// (Tools/spn_district_layout.py: authored as "X east, Y north", centimetres). The numbers below are those authored values.
	const FVector PlayerStartCm(-10550.0, -4700.0, 100.0);
	const FVector WestAvenueCm(-10200.0, 0.0, 0.0);
	const FVector NorthStreetCm(0.0, 14200.0, 0.0);

	FSpnWorldFrame EastSouth;
	EastSouth.Axes = ESpnWorldAxes::EastSouthUp;
	FVector W;
	TestTrue(TEXT("PlayerStart converts (EastSouthUp)"), SpnWorldFrame::ToWorld(EastSouth, PlayerStartCm, W));
	TestTrue(TEXT("EastSouthUp: PlayerStart = 105.5 m west, 47 m NORTH, 1 m up (authored Y=-4700 reads as south in the layout labels, but UE +Y is south in this preset)"),
		Near(W, FVector(-105.5, 47.0, 1.0)));
	TestTrue(TEXT("EastSouthUp: 'North Street' (authored +Y) is 142 m SOUTH of the origin: the authored compass labels are mirrored in this preset"),
		SpnWorldFrame::ToWorld(EastSouth, NorthStreetCm, W) && Near(W, FVector(0.0, -142.0, 0.0)));
	TestTrue(TEXT("EastSouthUp: 'West Avenue' (authored X=-10200) stays 102 m west"), SpnWorldFrame::ToWorld(EastSouth, WestAvenueCm, W) && Near(W, FVector(-102.0, 0.0, 0.0)));

	FSpnWorldFrame NorthEast;
	NorthEast.Axes = ESpnWorldAxes::NorthEastUp;
	TestTrue(TEXT("NorthEastUp: PlayerStart = 47 m WEST, 105.5 m SOUTH, 1 m up"),
		SpnWorldFrame::ToWorld(NorthEast, PlayerStartCm, W) && Near(W, FVector(-47.0, -105.5, 1.0)));
	TestTrue(TEXT("NorthEastUp: 'North Street' (authored +Y) is 142 m EAST: neither preset reproduces the authored labels"),
		SpnWorldFrame::ToWorld(NorthEast, NorthStreetCm, W) && Near(W, FVector(142.0, 0.0, 0.0)));
	return true;
}

#endif
