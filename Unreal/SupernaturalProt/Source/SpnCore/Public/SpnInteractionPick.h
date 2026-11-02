#pragma once

#include "CoreMinimal.h"

/** One interactable near the player, measured by the engine side (cm / engine vectors). Mirrors spn::core::InteractionCandidate. */
struct FSpnPickCandidate
{
	FVector EyeCm = FVector::ZeroVector;
	FVector ViewDirection = FVector::ForwardVector;
	FVector CentreCm = FVector::ZeroVector;
	float RadiusCm = 0.f;
	bool bLineOfSightClear = true;
	int32 Priority = 0;
};

/** Reach limits in the engine's units (placeholders; the Core defaults are 2.5 m, 60 degrees, 1.5 m height difference). */
struct FSpnPickConfig
{
	float MaxDistanceCm = 250.f;
	float MaxAngleDegrees = 60.f;
	float MaxHeightDifferenceCm = 150.f;
};

namespace SpnInteractionPick
{
	/** Index of the candidate the player means (spn::core::PickInteractionCandidate): in reach, in view, with a clear line; the most central and
	 *  closest, ties by Priority. INDEX_NONE when none qualifies. Walls are the caller's line-of-sight result. */
	SPNCORE_API int32 Pick(const FSpnPickConfig& Config, const TArray<FSpnPickCandidate>& Candidates);

	/** Reach verdict of one candidate as a name ("Ok", "TooFar", "OutOfView", "TooHighOrLow", "Obstructed", "InvalidInput"). */
	SPNCORE_API FString ReachName(const FSpnPickConfig& Config, const FSpnPickCandidate& Candidate);
}
