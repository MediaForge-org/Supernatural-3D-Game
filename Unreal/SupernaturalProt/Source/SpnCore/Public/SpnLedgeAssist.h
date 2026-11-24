#pragma once

#include "CoreMinimal.h"
#include "SpnLedgeAssist.generated.h"

/**
 * Ledge assist (developer decision 2026-11-14): StepOn / Vault / Mantle / ClimbUp, started only by the explicit jump input with the obstacle
 * right in front. Rules and the capsule path live in the engine-independent core (spn/core/LedgeAssist.h); this is the bridge in
 * centimetres. Every value is a PLACEHOLDER TEST VALUE; the three height bands (40 / 120 / 180 cm) are the developer's PROVISIONAL bands.
 */
UENUM(BlueprintType)
enum class ESpnLedgeMove : uint8
{
	None = 0,
	StepOn,
	Vault,
	Mantle,
	ClimbUp
};

USTRUCT(BlueprintType)
struct SPNCORE_API FSpnLedgeAssistSettings
{
	GENERATED_BODY()

	/** Off: the jump input is always an ordinary jump. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ledge assist")
	bool bEnabled = true;

	/** Upper ends of the three bands, cm (developer, provisional): up to 40 step on, 40-120 vault or mantle, 120-180 climb up. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ledge assist", meta = (ClampMin = "1"))
	float LowBandMaxCm = 40.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ledge assist", meta = (ClampMin = "1"))
	float MidBandMaxCm = 120.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ledge assist", meta = (ClampMin = "1"))
	float HighBandMaxCm = 180.f;

	/** A press starts a move only this close to the face (capsule front), cm: standing, plus seconds of approach speed, capped. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ledge assist", meta = (ClampMin = "0"))
	float ReachStandingCm = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ledge assist", meta = (ClampMin = "0"))
	float ReachPerSpeedSeconds = 0.08f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ledge assist", meta = (ClampMin = "0"))
	float ReachMaxCm = 60.f;

	/** Top surface the hands / the foot need behind the edge, cm. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ledge assist", meta = (ClampMin = "1"))
	float MinGripDepthCm = 15.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ledge assist", meta = (ClampMin = "1", ClampMax = "89"))
	float MaxApproachAngleDegrees = 45.f;

	/** Thicker obstacles are mantled, never vaulted, cm. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ledge assist", meta = (ClampMin = "1"))
	float VaultMaxDepthCm = 80.f;

	/** Run-up a vault in the middle band needs, cm/s. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ledge assist", meta = (ClampMin = "0"))
	float VaultMinSpeed = 200.f;

	/** Part of the capsule bottom that may pass beside the edge while rising (the planted leg), cm. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ledge assist", meta = (ClampMin = "1"))
	float LegAllowanceCm = 35.f;
};

/** What the movement component measured, in cm / degrees / cm/s (see spn::core::LedgeProbe). */
struct FSpnLedgeProbe
{
	bool bFaceFound = false;
	float FaceDistance = 0.f;
	float ApproachAngle = 0.f;
	float ApproachSpeed = 0.f;
	bool bSurfaceAllowed = true;
	bool bTopFound = false;
	float Height = 0.f;
	float TopAngle = 0.f;
	float Depth = 0.f;
	bool bLiftClear = true;
	bool bTopClear = true;
	bool bLandingFound = false;
	float LandingRise = 0.f;
	bool bLandingClear = false;
};

/** A planned move in cm / s (see spn::core::LedgePlan). The core plan is kept so the path can be sampled. */
struct SPNCORE_API FSpnLedgePlan
{
	ESpnLedgeMove Move = ESpnLedgeMove::None;
	FString Reason;
	float Height = 0.f;
	float Depth = 0.f;
	float ApproachSpeed = 0.f;
	float ExitSpeed = 0.f;
	float ForwardEnd = 0.f;
	float RiseHeight = 0.f;
	float LiftSeconds = 0.f;
	float TotalSeconds = 0.f;
	bool bEndsOnTop = false;

	/** Raw core plan (opaque storage, filled by Plan). */
	TArray<double> CoreData;
};

namespace SpnLedgeAssist
{
	SPNCORE_API FSpnLedgePlan Plan(const FSpnLedgeAssistSettings& Settings, float MaxStepHeightCm, const FSpnLedgeProbe& Probe, float CapsuleRadiusCm);

	/** Reach in front of the capsule (cm) for an approach speed (cm/s). */
	SPNCORE_API float Reach(const FSpnLedgeAssistSettings& Settings, float ApproachSpeed);

	/** Forward (X) and up (Y) of the capsule in cm at Seconds after the start; bOutDone once the move is complete. */
	SPNCORE_API FVector2D Sample(const FSpnLedgePlan& Plan, float Seconds, bool& bOutDone);

	SPNCORE_API const TCHAR* MoveName(ESpnLedgeMove Move);

	/** Empty when the settings are valid; otherwise the reason. */
	SPNCORE_API FString Validate(const FSpnLedgeAssistSettings& Settings, float MaxStepHeightCm);
}
