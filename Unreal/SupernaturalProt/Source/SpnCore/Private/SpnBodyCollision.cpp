#include "SpnBodyCollision.h"

#include <spn/core/BodyCollision.h>

#include UE_INLINE_GENERATED_CPP_BY_NAME(SpnBodyCollision)

static_assert(static_cast<uint8>(spn::core::BodyCollisionMode::Explicit) == static_cast<uint8>(ESpnBodyCollision::Explicit),
	"ESpnBodyCollision must mirror spn::core::BodyCollisionMode");

namespace
{
	spn::core::BodyCollisionProfile ToCore(const FSpnBodyCollisionProfile& P)
	{
		return {P.Radius, P.HalfHeight, P.CrouchedHalfHeight};
	}
}

FSpnBodyCollisionResolved SpnBodyCollision::Resolve(ESpnBodyCollision Mode, float BodyHeightCm, const FSpnBodyCollisionProfile& Explicit,
	const FSpnBodyCollisionProfile& Defaults, float HeadClearanceCm)
{
	spn::core::BodyCollisionRules Rules;
	Rules.defaultRadiusCm = Defaults.Radius;
	Rules.defaultHalfHeightCm = Defaults.HalfHeight;
	Rules.defaultCrouchedHalfHeightCm = Defaults.CrouchedHalfHeight;
	Rules.headClearanceCm = HeadClearanceCm;
	const spn::core::BodyCollisionResult R = spn::core::ResolveBodyCollision(
		{static_cast<spn::core::BodyCollisionMode>(Mode), BodyHeightCm, ToCore(Explicit)}, Rules);

	FSpnBodyCollisionResolved Out;
	Out.Profile.Radius = R.profile.radiusCm;
	Out.Profile.HalfHeight = R.profile.halfHeightCm;
	Out.Profile.CrouchedHalfHeight = R.profile.crouchedHalfHeightCm;
	Out.HeadAboveCapsule = R.headAboveCapsuleCm;
	Out.bDiffersFromDefault = R.differsFromDefault;
	for (const spn::core::BodyCollisionIssue Issue : R.issues)
	{
		Out.Issues.Add(UTF8_TO_TCHAR(spn::core::BodyCollisionIssueName(Issue)));
	}
	return Out;
}

FSpnPassageFit SpnBodyCollision::FitPassage(const FSpnBodyCollisionProfile& Profile, float BodyHeightCm, float ClearHeightCm, float ClearWidthCm)
{
	const spn::core::PassageFit F = spn::core::FitPassage(ToCore(Profile), BodyHeightCm, ClearHeightCm, ClearWidthCm);
	FSpnPassageFit Out;
	Out.bCapsuleFitsStanding = F.capsuleFitsStanding;
	Out.bCapsuleFitsCrouched = F.capsuleFitsCrouched;
	Out.bHeadClearsStanding = F.headClearsStanding;
	Out.Headroom = F.headroomCm;
	return Out;
}
