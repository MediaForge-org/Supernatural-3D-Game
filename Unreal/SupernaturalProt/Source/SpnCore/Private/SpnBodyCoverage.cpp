#include "SpnBodyCoverage.h"

#include <spn/core/BodyCoverage.h>

#include UE_INLINE_GENERATED_CPP_BY_NAME(SpnBodyCoverage)

static_assert(static_cast<int32>(spn::core::BodyZone::Count) == SpnBodyCoverage::ZoneCount, "ESpnBodyZone must mirror spn::core::BodyZone");

namespace
{
	std::optional<spn::core::ContentId> ToId(const FName& Name)
	{
		return spn::core::ContentId::Parse(TCHAR_TO_UTF8(*Name.ToString()));
	}

	FName FromId(const spn::core::ContentId& Id)
	{
		return FName(UTF8_TO_TCHAR(Id.Value().c_str()));
	}

	FString ZonesText(spn::core::ZoneMask Mask)
	{
		TArray<FString> Names;
		for (int32 i = 0; i < SpnBodyCoverage::ZoneCount; ++i)
		{
			if ((Mask >> i) & 1u) { Names.Add(UTF8_TO_TCHAR(spn::core::BodyZoneName(static_cast<spn::core::BodyZone>(i)))); }
		}
		return FString::Join(Names, TEXT("+"));
	}
}

int32 SpnBodyCoverage::ZoneBit(ESpnBodyZone Zone)
{
	return static_cast<int32>(spn::core::Zone(static_cast<spn::core::BodyZone>(Zone)));
}

const TCHAR* SpnBodyCoverage::ZoneName(ESpnBodyZone Zone)
{
	static TArray<FString> Names;
	if (Names.IsEmpty())
	{
		for (int32 i = 0; i < ZoneCount; ++i) { Names.Add(UTF8_TO_TCHAR(spn::core::BodyZoneName(static_cast<spn::core::BodyZone>(i)))); }
	}
	const int32 Index = static_cast<int32>(Zone);
	return Names.IsValidIndex(Index) ? *Names[Index] : TEXT("unknown");
}

FSpnCoverageResult SpnBodyCoverage::Resolve(const TArray<FSpnWearableCoverage>& Worn, const TArray<FSpnBodyDetailDef>& Details, const FSpnPresentationPolicy& Policy)
{
	FSpnCoverageResult Out;
	std::vector<spn::core::WearableCoverage> CoreWorn;
	for (const FSpnWearableCoverage& W : Worn)
	{
		const std::optional<spn::core::ContentId> Id = ToId(W.Item);
		if (!Id) { Out.Issues.Add(FString::Printf(TEXT("invalid wearable id '%s' (skipped)"), *W.Item.ToString())); continue; }
		CoreWorn.push_back({*Id, static_cast<spn::core::WearLayer>(W.Layer), static_cast<spn::core::ZoneMask>(W.Covers), static_cast<spn::core::ZoneMask>(W.HidesBody), W.bOpaque});
	}
	std::vector<spn::core::BodyDetailDef> CoreDetails;
	for (const FSpnBodyDetailDef& D : Details)
	{
		const std::optional<spn::core::ContentId> Id = ToId(D.Id);
		const std::optional<spn::core::ContentId> Category = ToId(D.Category);
		if (!Id || !Category) { Out.Issues.Add(FString::Printf(TEXT("invalid detail id/category '%s' / '%s' (skipped)"), *D.Id.ToString(), *D.Category.ToString())); continue; }
		CoreDetails.push_back({*Id, *Category, static_cast<spn::core::BodyZone>(D.Zone)});
	}
	spn::core::PresentationPolicy CorePolicy;
	CorePolicy.hideCoveredBody = Policy.bHideCoveredBody;
	for (const FName& C : Policy.EnabledDetailCategories)
	{
		if (const std::optional<spn::core::ContentId> Id = ToId(C)) { CorePolicy.enabledDetailCategories.push_back(*Id); }
	}
	const spn::core::CoverageResult R = spn::core::ResolveCoverage(CoreWorn, CoreDetails, CorePolicy);
	Out.Covered = static_cast<int32>(R.covered);
	Out.HiddenBody = static_cast<int32>(R.hiddenBody);
	Out.Exposed = static_cast<int32>(R.exposed);
	for (const spn::core::ContentId& Id : R.details) { Out.Details.Add(FromId(Id)); }
	for (const spn::core::CoverageIssue& I : R.issues)
	{
		const TCHAR* Kind = I.kind == spn::core::CoverageIssueKind::HidesUncoveredZone ? TEXT("hides zones it does not cover")
			: I.kind == spn::core::CoverageIssueKind::TransparentHides ? TEXT("is sheer but claims to hide body geometry") : TEXT("overlaps another item on the same layer");
		Out.Issues.Add(FString::Printf(TEXT("%s %s: %s"), *FromId(I.item).ToString(), Kind, *ZonesText(I.zones)));
	}
	return Out;
}
