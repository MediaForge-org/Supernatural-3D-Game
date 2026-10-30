#pragma once

#include "CoreMinimal.h"
#include "SpnWorldFrame.h"

// Player pose of a save slot (Night Build 01 completion). ENGINEERING PROPOSAL, not master canon.
//
// Stored as the optional section "player.pose" (version 1) of the unified save slot. The Core container treats it as an unknown optional
// section (it is checksummed, carried over by older builds and ignored by them), so the file format and every older reader stay valid.
// A slot without it (every save made before this section existed) is a LEGACY slot: it loads completely and the player simply stays where
// the level put them; the load status says so (bPoseInSlot == false).
//
// The position is stored in the CANONICAL frame (metres, x east, y north, z up) through SpnWorldFrame, never as raw Unreal centimetres, so a
// later change of the axis preset or origin (CQ-101) is a frame parameter and not a save migration. The heading is a compass bearing.
//
// Text payload (strict, every field exactly once, in this order, '\n' terminated):
//   SPN-PLAYERPOSE 1
//   level <package path>
//   axes east_south_up|north_east_up
//   origin_m <x> <y> <z>
//   pos_m <x> <y> <z>
//   bearing_rad <b>
//   pitch_deg <p>
//   gait <word>
//   crouched 0|1

struct SPNCORE_API FSpnPlayerPose
{
	/** Package path of the level the pose belongs to, e.g. /Game/SpnTest/Lvl_SpnMovementTestV3Interiors. */
	FString Level;
	FSpnWorldFrame Frame;
	/** Canonical metres. */
	FVector WorldMetres = FVector::ZeroVector;
	/** Compass bearing, radians clockwise from north, [0, 2 pi). */
	double BearingRad = 0.0;
	/** Camera/control pitch in degrees, clamped to [-89, 89]. */
	double PitchDeg = 0.0;
	/** Locomotion gait name (walk/jog/sprint, see ESpnGait); free word, validated by the consumer. */
	FString Gait = TEXT("jog");
	bool bCrouched = false;

	/** Builds a pose from Unreal values. False if a coordinate is not representable in the frame. */
	static bool FromUnreal(const FString& InLevel, const FSpnWorldFrame& InFrame, const FVector& LocationCm, double YawDegrees, double InPitchDeg,
		const FString& InGait, bool bInCrouched, FSpnPlayerPose& Out);
	/** Unreal centimetres and yaw (degrees) in the frame stored with the pose. False if not representable. */
	bool ToUnreal(FVector& OutLocationCm, double& OutYawDegrees) const;

	/** Canonical text. */
	FString Serialize() const;
	/** Strict parse. On failure Out is untouched and Error says why (for logs and UI, not for logic). */
	static bool Parse(const FString& Payload, FSpnPlayerPose& Out, FString& Error);
};
