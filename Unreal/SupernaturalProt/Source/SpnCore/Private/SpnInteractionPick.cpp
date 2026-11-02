#include "SpnInteractionPick.h"

#include <spn/core/InteractionReach.h>

namespace
{
	spn::core::ReachConfig ToCore(const FSpnPickConfig& Config)
	{
		spn::core::ReachConfig Out;
		Out.maxDistance = Config.MaxDistanceCm / 100.0;
		Out.maxAngleDegrees = Config.MaxAngleDegrees;
		Out.maxHeightDifference = Config.MaxHeightDifferenceCm / 100.0;
		return Out;
	}

	spn::core::ReachQuery ToCore(const FSpnPickCandidate& Candidate)
	{
		const auto Metres = [](const FVector& V) { return spn::core::ReachVec3{V.X / 100.0, V.Y / 100.0, V.Z / 100.0}; };
		spn::core::ReachQuery Query;
		Query.eye = Metres(Candidate.EyeCm);
		Query.viewDirection = {Candidate.ViewDirection.X, Candidate.ViewDirection.Y, Candidate.ViewDirection.Z};
		Query.targetCentre = Metres(Candidate.CentreCm);
		Query.targetRadius = Candidate.RadiusCm / 100.0;
		Query.lineOfSightClear = Candidate.bLineOfSightClear;
		return Query;
	}
}

int32 SpnInteractionPick::Pick(const FSpnPickConfig& Config, const TArray<FSpnPickCandidate>& Candidates)
{
	std::vector<spn::core::InteractionCandidate> List;
	List.reserve(Candidates.Num());
	for (const FSpnPickCandidate& Candidate : Candidates)
	{
		spn::core::InteractionCandidate Item;
		Item.query = ToCore(Candidate);
		Item.priority = Candidate.Priority;
		List.push_back(std::move(Item));
	}
	const auto Choice = spn::core::PickInteractionCandidate(ToCore(Config), List);
	return Choice ? static_cast<int32>(Choice->index) : INDEX_NONE;
}

FString SpnInteractionPick::ReachName(const FSpnPickConfig& Config, const FSpnPickCandidate& Candidate)
{
	return FString(spn::core::ReachResultName(spn::core::CheckReach(ToCore(Config), ToCore(Candidate))));
}
