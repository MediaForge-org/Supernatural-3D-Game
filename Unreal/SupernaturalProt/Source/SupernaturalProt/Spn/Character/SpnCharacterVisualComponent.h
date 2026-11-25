#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SkinnedMeshComponent.h"
#include "SpnBodyCollision.h"
#include "SpnCharacterVisualComponent.generated.h"

class UAnimInstance;
class UIKRetargeter;
class USkeletalMesh;
class USkeletalMeshComponent;

/**
 * One body presentation (Docs/Proposals/0013 steps S2/S3). PLACEHOLDER data: no preset is a chosen character. Empty asset fields keep what the
 * character blueprint already shows (today the UE5 template mannequin), so a preset can be a pure height test on the mannequin.
 */
USTRUCT(BlueprintType)
struct FSpnBodyPreset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Body")
	FName Id = NAME_None;

	/** Body (leader pose) mesh. Empty = keep the blueprint's mesh. Must use the UE5 mannequin skeleton contract (or be retargeted). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Body")
	TSoftObjectPtr<USkeletalMesh> BodyMesh;

	/** Anim blueprint class; should derive from USpnAnimInstance. Empty = keep the blueprint's. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Body")
	TSoftClassPtr<UAnimInstance> AnimClass;

	/** Follower meshes (head, hair, one per clothing slot) that copy the body's pose (leader pose). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Body")
	TArray<TSoftObjectPtr<USkeletalMesh>> Followers;

	/** Body height in cm. 0 = the body mesh's own height (no scaling). Placeholder test bodies are a scaled mannequin. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Body", meta = (ClampMin = "0", ClampMax = "260"))
	float HeightCm = 0.f;

	/**
	 * Retargeted body on ANOTHER skeleton (e.g. a MetaHuman body), Proposal 0014 C2. When set (and it and Retargeter load), the gameplay mesh
	 * stays as it is but is hidden: its anim blueprint keeps running and a new component shows this mesh, posed every frame by runtime IK
	 * retargeting (USpnRetargetFollowAnimInstance). Followers then follow the retargeted body. BodyMesh / AnimClass are ignored and NOTHING is
	 * scaled: HeightCm is then only the measured height of that body (feet to the top of the head), used for the camera pivot.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Body|Retarget")
	TSoftObjectPtr<USkeletalMesh> RetargetMesh;

	/** IK retargeter: source = the gameplay mesh's IK rig, target = RetargetMesh's IK rig. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Body|Retarget")
	TSoftObjectPtr<UIKRetargeter> Retargeter;

	/** The IK pass of the retargeter runs up to this LOD of the retargeted body; -1 = at every LOD. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Body|Retarget")
	int32 RetargetIKLodThreshold = -1;

	/**
	 * Gameplay collision of this body (NightBuild06, Core BodyCollision). Keep (default) = the character's own capsule, unchanged
	 * gameplay. FromHeight = capsule half height from the body height (GetBodyHeightCm) plus CollisionHeadClearanceCm, radius unchanged,
	 * crouch scaled. Explicit = ExplicitCollision. Whether the protagonist's capsule follows its body is the developer's decision (0017).
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Body|Collision")
	ESpnBodyCollision Collision = ESpnBodyCollision::Keep;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Body|Collision", meta = (EditCondition = "Collision == ESpnBodyCollision::Explicit"))
	FSpnBodyCollisionProfile ExplicitCollision;
};

/**
 * Separates the visual body from the gameplay pawn: builds the presentation from a preset (body mesh, anim class, follower meshes, height)
 * and adapts the camera pivot to the body height. It never changes movement, step/jump height or interaction reach - a body with other
 * proportions does not change gameplay (0013 §4 point 4). The capsule changes ONLY when a preset opts in with Collision != Keep
 * (NightBuild06 body collision profile); no shipped preset does that except the clearly named *_fit test presets.
 *
 * A missing asset falls back to what the blueprint shows (the mannequin) and is reported in the log - the same rule as the Fab assets
 * of the world generator. Third-party bodies are never referenced from C++ or git; presets point to local content.
 *
 * Dev: console `Spn.Body <id>|none` (no argument lists the presets), command line `-SpnBody=<id>` (applied at BeginPlay, for QA runs).
 * Presets that point to local, non-git content (engine MetaHuman template, generated retargeters) live in config: ConfigPresets.
 */
UCLASS(Config = Game, ClassGroup = (Spn), meta = (BlueprintSpawnableComponent))
class SUPERNATURALPROT_API USpnCharacterVisualComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USpnCharacterVisualComponent();

	/** Available presentations. The constructor adds two PLACEHOLDER height tests on the mannequin: test_160 and test_195. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Body")
	TArray<FSpnBodyPreset> Presets;

	/** Further presets from DefaultGame.ini ([/Script/SupernaturalProt.SpnCharacterVisualComponent]); looked up after Presets. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Spn|Body")
	TArray<FSpnBodyPreset> ConfigPresets;

	/** Applied at BeginPlay. None = leave the blueprint's presentation exactly as it is (today's default). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Body")
	FName InitialPreset = NAME_None;

	/** Height of the unscaled body in cm. 0 = measured from the body mesh bounds (the template mannequin measures about 180-190). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Body", meta = (ClampMin = "0"))
	float ReferenceHeightCm = 0.f;

	/** How much of the height difference the camera pivot follows (PLACEHOLDER: 0.9 = roughly the head). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spn|Body|Camera", meta = (ClampMin = "0", ClampMax = "1.5"))
	float CameraHeightFactor = 0.9f;

	/** Capsule top above the top of the head for presets with Collision = FromHeight, cm (PLACEHOLDER). */
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Spn|Body|Collision", meta = (ClampMin = "0", ClampMax = "30"))
	float CollisionHeadClearanceCm = 2.f;

	/** Applies a preset; None restores the blueprint's presentation. False (and the fallback stays) if the id is unknown. */
	UFUNCTION(BlueprintCallable, Category = "Spn|Body")
	bool ApplyPreset(FName Id);

	UFUNCTION(BlueprintPure, Category = "Spn|Body")
	FName GetActivePreset() const { return Active; }

	/** Height of the shown body in cm (reference height x scale). */
	UFUNCTION(BlueprintPure, Category = "Spn|Body")
	float GetBodyHeightCm() const;

	/** Unscaled body height used as the reference (ReferenceHeightCm or measured). */
	float GetReferenceHeightCm() const;

	/** The follower components created for the active preset (leader pose = the character mesh). */
	const TArray<TObjectPtr<USkeletalMeshComponent>>& GetFollowers() const { return FollowerComponents; }

	/** How many assets of the last applied preset were missing (fallback used). */
	int32 GetMissingAssetCount() const { return MissingAssets; }

	TArray<FName> GetPresetIds() const;

	/** The body collision the last ApplyPreset asked for (resolved by the core) and whether the character accepted it. */
	const FSpnBodyCollisionResolved& GetLastCollision() const { return LastCollision; }
	bool WasCollisionRefused() const { return bCollisionRefused; }

	/** The retargeted body of the active preset, or null (then the gameplay mesh itself is shown). */
	USkeletalMeshComponent* GetRetargetBody() const { return RetargetBody; }

	/** The mesh the player sees: the retargeted body if there is one, else the gameplay mesh. */
	USkeletalMeshComponent* GetShownBody() const;

	/** Owner-no-see for every shown part (close camera). The hidden gameplay mesh is left alone. */
	void SetShownOwnerNoSee(bool bNoSee);

	/** Broadcast after every ApplyPreset (the shown body may be a new component): wardrobe / details re-attach to it. */
	DECLARE_MULTICAST_DELEGATE(FOnPresentationChanged);
	FOnPresentationChanged OnPresentationChanged;

protected:
	virtual void BeginPlay() override;

private:
	USkeletalMeshComponent* GetBodyComponent() const;
	const FSpnBodyPreset* FindPreset(FName Id) const;
	void CaptureOriginal();
	void ClearFollowers();
	void ClearRetargetBody();

	UPROPERTY(Transient)
	TArray<TObjectPtr<USkeletalMeshComponent>> FollowerComponents;

	UPROPERTY(Transient)
	TObjectPtr<USkeletalMeshComponent> RetargetBody;

	EVisibilityBasedAnimTickOption OriginalTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;

	UPROPERTY(Transient)
	TObjectPtr<USkeletalMesh> OriginalMesh;

	UPROPERTY(Transient)
	TSubclassOf<UAnimInstance> OriginalAnimClass;

	FName Active = NAME_None;
	bool bOriginalCaptured = false;
	FVector OriginalMeshScale = FVector::OneVector;
	FVector OriginalCameraBase = FVector::ZeroVector;
	float CurrentScale = 1.f;
	float RetargetHeightCm = 0.f;   // > 0: a retargeted body of this measured height is shown (not scaled)
	int32 MissingAssets = 0;
	FSpnBodyCollisionResolved LastCollision;
	bool bCollisionRefused = false;
};
