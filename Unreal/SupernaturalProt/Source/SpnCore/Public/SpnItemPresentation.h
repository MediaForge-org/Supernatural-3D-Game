#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/StreamableManager.h"
#include "UObject/SoftObjectPtr.h"

#include "SpnItemPresentation.generated.h"

class UAnimMontage;
class UMaterialInterface;
class USoundBase;
class UStaticMesh;
class UTexture2D;

// Optional presentation layer for inventory items (M2B3.4). ENGINEERING PROPOSAL, not master canon.
//
// SINGLE SOURCE OF TRUTH: gameplay values (stack limit, weight, volume, category, equipment slots) live ONLY in the portable catalog
// (Content/SpnTest/Inventory/TestItemCatalog.json today). This asset holds nothing but how an item LOOKS, SOUNDS and READS. The two are
// joined by the item id. Because the Core's catalog fingerprint hashes only the gameplay fields, editing a presentation asset can never
// invalidate a save, and no gameplay value can drift between two copies. USpnItemPresentationSet::ValidateAgainstCatalog reports ids that
// exist in only one of the two.
//
// Everything is optional: an item without an entry is shown with its id as text and no icon (the view-model flags this), so production
// assets are not needed to use the inventory. All asset references are SOFT: nothing is loaded until a widget asks for it (async).
//
// PACKAGED BUILDS (Unverified, no packaged build was made): this is a UPrimaryDataAsset with the primary asset type "SpnItemPresentation".
// To have the Asset Manager cook the sets and the assets they reference, add to Config/DefaultGame.ini:
//   [/Script/Engine.AssetManagerSettings]
//   +PrimaryAssetTypesToScan=(PrimaryAssetType="SpnItemPresentation",AssetBaseClass="/Script/SpnCore.SpnItemPresentationSet",bHasBlueprintClasses=False,
//       bIsEditorOnly=False,Directories=((Path="/Game/Spn/Presentation")),Rules=(Priority=-1,ChunkId=-1,CookRule=AlwaysCook))
// The catalog JSON is not a cooked asset; see Docs/M2B3/INVENTORY_VIEW_MODEL.md for the staging options.

/** Presentation of one item definition. No gameplay values. */
USTRUCT(BlueprintType)
struct SPNCORE_API FSpnItemPresentationEntry
{
	GENERATED_BODY()

	/** Item definition id this entry belongs to ("spn_test:item.ration"). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spn")
	FString ItemId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spn")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spn")
	FText Description;

	/** Free grouping label for sorting/sectioning in a UI; not a gameplay category. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spn")
	FName SortGroup;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spn|Assets")
	TSoftObjectPtr<UTexture2D> Icon;

	/** Mesh used when the item is shown in the world or in a 3D preview. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spn|Assets")
	TSoftObjectPtr<UStaticMesh> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spn|Assets")
	TSoftObjectPtr<UMaterialInterface> Material;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spn|Assets")
	TSoftObjectPtr<USoundBase> PickupSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spn|Assets")
	TSoftObjectPtr<USoundBase> UseSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spn|Assets")
	TSoftObjectPtr<UAnimMontage> UseMontage;
};

/** Mismatches between a presentation set and the gameplay catalog. Neither is an error for the game: the catalog wins. */
struct FSpnPresentationReport
{
	/** Entries whose item id is not in the catalog (renamed or removed items). */
	TArray<FString> OrphanedEntries;
	/** Catalog items without an entry (shown with fallbacks). */
	TArray<FString> ItemsWithoutPresentation;
	/** Item ids that appear more than once in the set (the first entry wins). */
	TArray<FString> DuplicateEntries;

	bool IsClean() const { return OrphanedEntries.IsEmpty() && DuplicateEntries.IsEmpty(); }
};

/** A set of item presentations. Create one asset per content area; the inventory subsystem merges nothing: it uses one set. */
UCLASS(BlueprintType)
class SPNCORE_API USpnItemPresentationSet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spn")
	TArray<FSpnItemPresentationEntry> Entries;

	/** First entry for the item id, or null. */
	const FSpnItemPresentationEntry* Find(const FString& ItemId) const;

	/** Compares the set with the ids of a catalog. */
	FSpnPresentationReport ValidateAgainstCatalog(const TArray<FString>& CatalogItemIds) const;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	static FPrimaryAssetType GetAssetType();
};
