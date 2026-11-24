#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpnBodyCoverage.h"
#include "SpnWardrobeComponent.generated.h"

class USkeletalMesh;
class USkeletalMeshComponent;
class UStaticMesh;
class UPrimitiveComponent;

/** A wearable: coverage data (core rules) + its OWN mesh. There is deliberately no way to merge a wearable into the body mesh. */
USTRUCT(BlueprintType)
struct FSpnWearableDef
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Wardrobe")
	FSpnWearableCoverage Coverage;

	/** Skinned to the human skeleton; follows the shown body (leader pose). Empty = data-only test item. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Wardrobe")
	TSoftObjectPtr<USkeletalMesh> Mesh;
};

/**
 * Optional detail geometry for one zone (generic: relief scars, piercings, prosthetics, anatomical detail of a content pack ...).
 * Either a skinned follower (SkeletalMesh, same skeleton as the body) or a rigid part on a bone (StaticMesh + Bone + Offset).
 */
USTRUCT(BlueprintType)
struct FSpnBodyDetailItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Wardrobe")
	FSpnBodyDetailDef Def;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Wardrobe")
	TSoftObjectPtr<USkeletalMesh> SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Wardrobe")
	TSoftObjectPtr<UStaticMesh> StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Wardrobe")
	FName Bone = TEXT("spine_03");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Wardrobe")
	FTransform Offset;

	/** cm; 0 = no distance cull. Detail is not needed at a distance (cheap LOD rule on top of the mesh's own LODs). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Wardrobe", meta = (ClampMin = "0"))
	float MaxDrawDistance = 2500.f;
};

/**
 * Modular body presentation on top of USpnCharacterVisualComponent (Docs/Proposals/0018). Keeps body, wearables (by layer), and optional
 * detail geometry SEPARATE, resolves coverage with the engine-independent rules (spn::core::ResolveCoverage) and applies the result to the
 * currently shown body (mannequin or retargeted MetaHuman). Re-applies itself whenever the body preset changes.
 *   - wearables: one follower component per worn item (leader = shown body), no merge path exists;
 *   - hidden body: material slots named "SpnZone_<zone>" on the shown body are hidden when a worn item hides that zone (a body mesh
 *     without such slots simply hides nothing - documented, not an error);
 *   - details: created only for ids the rules return (enabled category + exposed zone); otherwise no component exists at all.
 * Gameplay (capsule, movement, interaction) is never touched. Not saved (AppearanceSpec stays the save format; see 0018 "Save").
 */
UCLASS(Config = Game, ClassGroup = (Spn), meta = (BlueprintSpawnableComponent))
class SUPERNATURALPROT_API USpnWardrobeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USpnWardrobeComponent();

	/** Known wearables (data; PLACEHOLDER: no real garment exists yet). */
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Spn|Wardrobe")
	TArray<FSpnWearableDef> Wearables;

	/** Known optional details (data; only neutral technical markers exist). */
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Spn|Wardrobe")
	TArray<FSpnBodyDetailItem> Details;

	/** Which detail categories may appear. Default: none. Set by settings / installed content, never by the core itself. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Wardrobe")
	FSpnPresentationPolicy Policy;

	/** Wear exactly these items (ids of Wearables; unknown ids are reported and ignored) and re-apply. */
	UFUNCTION(BlueprintCallable, Category = "Spn|Wardrobe")
	void SetWorn(const TArray<FName>& Items);

	UFUNCTION(BlueprintCallable, Category = "Spn|Wardrobe")
	void SetPolicy(const FSpnPresentationPolicy& InPolicy);

	/** Rebuilds wearables, hidden zones and details on the shown body. */
	UFUNCTION(BlueprintCallable, Category = "Spn|Wardrobe")
	void Reapply();

	const FSpnCoverageResult& GetLastResult() const { return LastResult; }
	const TArray<TObjectPtr<USkeletalMeshComponent>>& GetWearableComponents() const { return WearableComponents; }
	const TArray<TObjectPtr<UPrimitiveComponent>>& GetDetailComponents() const { return DetailComponents; }
	int32 GetHiddenSectionCount() const { return HiddenSections; }
	const TArray<FName>& GetWorn() const { return Worn; }

protected:
	virtual void OnRegister() override;
	virtual void OnUnregister() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;

private:
	USkeletalMeshComponent* GetShownBody() const;
	void Clear();
	int32 ApplyZoneHiding(USkeletalMeshComponent* Body, int32 HiddenMask);

	UPROPERTY(Transient)
	TArray<TObjectPtr<USkeletalMeshComponent>> WearableComponents;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UPrimitiveComponent>> DetailComponents;

	UPROPERTY(Transient)
	TObjectPtr<USkeletalMeshComponent> ZoneHiddenBody;

	TArray<FName> Worn;
	FSpnCoverageResult LastResult;
	int32 HiddenSections = 0;
	FDelegateHandle PresentationHandle;
};
