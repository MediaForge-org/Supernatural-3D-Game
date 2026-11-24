#pragma once

#include "CoreMinimal.h"
#include "SpnBodyCoverage.generated.h"

/** Mirror of spn::core::BodyZone (Core/include/spn/core/BodyCoverage.h); bit index = value. PLACEHOLDER zone set. */
UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "false"))
enum class ESpnBodyZone : uint8
{
	Head, Face, Neck, Chest, Abdomen, Back, Pelvis, Buttocks,
	UpperArms, Forearms, Hands, Thighs, Knees, Calves, Feet
};

/** Mirror of spn::core::WearLayer, inner to outer. */
UENUM(BlueprintType)
enum class ESpnWearLayer : uint8
{
	Underwear, Base, Outer, Accessory
};

/** What one worn item does to the body (data only; the wearable's mesh is a separate component, never merged into the body). */
USTRUCT(BlueprintType)
struct SPNCORE_API FSpnWearableCoverage
{
	GENERATED_BODY()

	/** Content id "namespace:name". */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Body") FName Item;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Body") ESpnWearLayer Layer = ESpnWearLayer::Base;
	/** Zones the item covers (bit = ESpnBodyZone). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Body", meta = (Bitmask, BitmaskEnum = "/Script/SpnCore.ESpnBodyZone")) int32 Covers = 0;
	/** Zones whose body geometry may be removed under it (subset of Covers). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Body", meta = (Bitmask, BitmaskEnum = "/Script/SpnCore.ESpnBodyZone")) int32 HidesBody = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Body") bool bOpaque = true;
};

/** Optional detail geometry of one zone; Category is a free content id that a content pack registers and the policy enables. */
USTRUCT(BlueprintType)
struct SPNCORE_API FSpnBodyDetailDef
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Body") FName Id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Body") FName Category;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Body") ESpnBodyZone Zone = ESpnBodyZone::Chest;
};

/** Decided OUTSIDE the core (settings, build, installed packs). Default: no optional detail geometry, covered body hidden. */
USTRUCT(BlueprintType)
struct SPNCORE_API FSpnPresentationPolicy
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Body") TArray<FName> EnabledDetailCategories;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Body") bool bHideCoveredBody = true;
};

USTRUCT(BlueprintType)
struct SPNCORE_API FSpnCoverageResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Spn|Body") int32 Covered = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Spn|Body") int32 HiddenBody = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Spn|Body") int32 Exposed = 0;
	/** Detail ids to create (enabled category AND exposed zone). */
	UPROPERTY(BlueprintReadOnly, Category = "Spn|Body") TArray<FName> Details;
	/** Readable issues (sheer item hiding body, layer conflict, invalid id ...). */
	UPROPERTY(BlueprintReadOnly, Category = "Spn|Body") TArray<FString> Issues;
};

namespace SpnBodyCoverage
{
	/** spn::core::ResolveCoverage on Unreal types. Items / details with an invalid content id are skipped and reported. */
	SPNCORE_API FSpnCoverageResult Resolve(const TArray<FSpnWearableCoverage>& Worn, const TArray<FSpnBodyDetailDef>& Details, const FSpnPresentationPolicy& Policy);

	SPNCORE_API int32 ZoneBit(ESpnBodyZone Zone);
	SPNCORE_API const TCHAR* ZoneName(ESpnBodyZone Zone);
	constexpr int32 ZoneCount = 15;
}
