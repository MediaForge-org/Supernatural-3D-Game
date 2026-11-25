#pragma once

#include "CoreMinimal.h"
#include "SpnBodyCollision.generated.h"

/** Mirror of spn::core::BodyCollisionMode (Core/include/spn/core/BodyCollision.h). */
UENUM(BlueprintType)
enum class ESpnBodyCollision : uint8
{
	/** The character's default capsule, whatever the body height (today's behaviour, the default). */
	Keep,
	/** Half height from the body height plus a head clearance; radius stays the default; the crouch scales with the half height. */
	FromHeight,
	/** The given radius / half heights, clamped to the core limits. */
	Explicit
};

/** A gameplay collision capsule (cm). */
USTRUCT(BlueprintType)
struct SPNCORE_API FSpnBodyCollisionProfile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Body|Collision", meta = (ClampMin = "0")) float Radius = 35.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Body|Collision", meta = (ClampMin = "0")) float HalfHeight = 90.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Body|Collision", meta = (ClampMin = "0")) float CrouchedHalfHeight = 40.f;

	bool Equals(const FSpnBodyCollisionProfile& Other, float Tolerance = 0.01f) const
	{
		return FMath::IsNearlyEqual(Radius, Other.Radius, Tolerance) && FMath::IsNearlyEqual(HalfHeight, Other.HalfHeight, Tolerance)
			&& FMath::IsNearlyEqual(CrouchedHalfHeight, Other.CrouchedHalfHeight, Tolerance);
	}
};

/** Result of spn::core::ResolveBodyCollision. */
struct SPNCORE_API FSpnBodyCollisionResolved
{
	FSpnBodyCollisionProfile Profile;
	/** Body top minus capsule top, cm (> 0: the head sticks out). 0 when the body height is unknown. */
	float HeadAboveCapsule = 0.f;
	bool bDiffersFromDefault = false;
	/** Core issue names ("half_height_clamped", ...), for the log. */
	TArray<FString> Issues;
};

/** Result of spn::core::FitPassage. */
struct SPNCORE_API FSpnPassageFit
{
	bool bCapsuleFitsStanding = false;
	bool bCapsuleFitsCrouched = false;
	bool bHeadClearsStanding = false;
	float Headroom = 0.f;
};

namespace SpnBodyCollision
{
	/** Defaults = the character's own capsule; HeadClearance = capsule top above the head in FromHeight mode. */
	SPNCORE_API FSpnBodyCollisionResolved Resolve(ESpnBodyCollision Mode, float BodyHeightCm, const FSpnBodyCollisionProfile& Explicit,
		const FSpnBodyCollisionProfile& Defaults, float HeadClearanceCm);

	SPNCORE_API FSpnPassageFit FitPassage(const FSpnBodyCollisionProfile& Profile, float BodyHeightCm, float ClearHeightCm, float ClearWidthCm);
}
