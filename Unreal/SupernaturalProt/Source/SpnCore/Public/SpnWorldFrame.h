#pragma once

#include "CoreMinimal.h"

// Unreal adapter for the canonical world coordinates of the SPN core (M2B3.1). ENGINEERING PROPOSAL, not master canon.
//
// Canonical frame (core, Docs/M2B2/WORLD_COORDINATE_CONVENTIONS.md): metres, x = east, y = north, z = up, double precision.
// Unreal: centimetres, left-handed, X forward, Y right, Z up. A right-handed frame cannot be mapped to a left-handed one without
// mirroring one horizontal axis, so there are two presets. WHICH preset the game uses is NOT decided (master: open, WF-DC01 / CQ-101);
// this adapter takes it as a parameter and has no global state. Core types stay behind the private include path (Decision 0006).

/** Mirrors spn::core::UnrealAxes. */
enum class ESpnWorldAxes : uint8
{
	/** UE +X = east, UE +Y = south, +Z up; heading east = yaw 0. Keeps "X = east" of the authored test maps. */
	EastSouthUp,
	/** UE +X = north, UE +Y = east, +Z up; heading north = yaw 0. */
	NorthEastUp,
};

struct FSpnWorldFrame
{
	ESpnWorldAxes Axes = ESpnWorldAxes::EastSouthUp;
	/** Canonical point (metres: X east, Y north, Z up) that sits at Unreal (0, 0, 0). Zero = no offset. */
	FVector OriginMetres = FVector::ZeroVector;
};

namespace SpnWorldFrame
{
	/** Canonical metres (X east, Y north, Z up) -> Unreal centimetres. False (Out untouched) for non-finite or out-of-range input. */
	SPNCORE_API bool ToUnreal(const FSpnWorldFrame& Frame, const FVector& WorldMetres, FVector& OutUnrealCm);

	/** Unreal centimetres -> canonical metres. False (Out untouched) for non-finite or out-of-range input. */
	SPNCORE_API bool ToWorld(const FSpnWorldFrame& Frame, const FVector& UnrealCm, FVector& OutWorldMetres);

	/** Compass bearing (radians, clockwise from north) -> Unreal yaw in degrees, (-180, 180]. */
	SPNCORE_API bool BearingToYawDegrees(ESpnWorldAxes Axes, double BearingRadians, double& OutYawDegrees);

	/** Unreal yaw in degrees -> compass bearing in radians, [0, 2 pi). */
	SPNCORE_API bool YawDegreesToBearing(ESpnWorldAxes Axes, double YawDegrees, double& OutBearingRadians);
}
