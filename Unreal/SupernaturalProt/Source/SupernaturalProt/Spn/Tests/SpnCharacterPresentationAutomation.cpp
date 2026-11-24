#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/World.h"
#include "GameFramework/SpringArmComponent.h"
#include "Misc/AutomationTest.h"
#include "Spn/Character/SpnAnimInstance.h"
#include "Spn/Character/SpnCharacterVisualComponent.h"
#include "Spn/Character/SpnRetargetFollowAnimInstance.h"
#include "Spn/Character/SpnWardrobeComponent.h"
#include "Spn/SpnCharacterMovementComponent.h"
#include "SupernaturalProtCharacter.h"

#if WITH_DEV_AUTOMATION_TESTS

// Exchangeable character presentation (Docs/Proposals/0013 steps S1-S3, approved 2026-11-13): the anim base reads the movement state
// interface (S1), the visual component swaps body / followers from data and falls back to the mannequin (S2), body height changes mesh and
// camera but never the capsule or movement values (S3). Headless; the rendered checks are SpnQA / SpnWorldQA runs with -SpnBody=<preset>.

namespace
{
	constexpr EAutomationTestFlags Flags = EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter;

	struct FCharWorld
	{
		UGameInstance* GameInstance = nullptr;
		UWorld* World = nullptr;
		ASupernaturalProtCharacter* Player = nullptr;

		FCharWorld()
		{
			GameInstance = NewObject<UGameInstance>(GEngine);
			GameInstance->AddToRoot();
			GameInstance->InitializeStandalone();
			World = GameInstance->GetWorld();
			UClass* Class = LoadClass<ASupernaturalProtCharacter>(nullptr, TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter_C"));
			if (Class != nullptr)
			{
				FActorSpawnParameters Params;
				Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				Player = World->SpawnActor<ASupernaturalProtCharacter>(Class, FTransform(FVector(0.f, 0.f, 200.f)), Params);
			}
		}

		~FCharWorld()
		{
			if (IsValid(Player)) { Player->Destroy(); }
			GameInstance->Shutdown();
			GEngine->DestroyWorldContext(World);
			World->DestroyWorld(false);
			GameInstance->RemoveFromRoot();
		}
	};
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnAnimBaseTest, "SpnGame.Character.AnimBaseReadsTheMovementState", Flags)
bool FSpnAnimBaseTest::RunTest(const FString&)
{
	FCharWorld W;
	if (!TestNotNull(TEXT("player character spawned"), W.Player)) { return false; }
	USkeletalMeshComponent* Mesh = W.Player->GetMesh();
	if (Mesh->GetAnimInstance() == nullptr) { Mesh->InitAnim(true); }
	USpnAnimInstance* Anim = Cast<USpnAnimInstance>(Mesh->GetAnimInstance());
	if (!TestNotNull(TEXT("the player's anim blueprint (ABP_Unarmed) derives from USpnAnimInstance"), Anim)) { return false; }
	USpnCharacterMovementComponent* Movement = W.Player->GetSpnMovement();
	if (!TestNotNull(TEXT("SPN movement component"), Movement)) { return false; }

	struct FCase { const TCHAR* Name; EMovementMode Mode; FVector Velocity; };
	const FCase Cases[] = {
		{TEXT("standing"), MOVE_Walking, FVector::ZeroVector},
		{TEXT("walking"), MOVE_Walking, FVector(170.f, 0.f, 0.f)},
		{TEXT("jogging"), MOVE_Walking, FVector(0.f, 330.f, 0.f)},
		{TEXT("sprinting"), MOVE_Walking, FVector(780.f, 0.f, 0.f)},
		{TEXT("jumping"), MOVE_Falling, FVector(200.f, 0.f, 300.f)},
		{TEXT("falling"), MOVE_Falling, FVector(0.f, 0.f, -400.f)},
	};
	TSet<ESpnMovementState> Seen;
	for (const FCase& Case : Cases)
	{
		Movement->SetMovementMode(Case.Mode);
		Movement->Velocity = Case.Velocity;
		Anim->RefreshFromOwner();
		const ESpnMovementState Expected = Movement->GetMovementState();
		Seen.Add(Expected);
		TestTrue(*FString::Printf(TEXT("%s: anim state %d = movement state %d"), Case.Name, static_cast<int32>(Anim->SpnMovementState), static_cast<int32>(Expected)),
			Anim->SpnMovementState == Expected);
		TestTrue(*FString::Printf(TEXT("%s: ground speed"), Case.Name), FMath::IsNearlyEqual(Anim->SpnGroundSpeed, static_cast<float>(Case.Velocity.Size2D()), 0.01f));
		TestEqual(*FString::Printf(TEXT("%s: in the air"), Case.Name), Anim->bSpnInAir, Case.Mode == MOVE_Falling);
		TestTrue(*FString::Printf(TEXT("%s: observed gait"), Case.Name), Anim->SpnObservedGait == Movement->GetObservedGait());
	}
	TestTrue(*FString::Printf(TEXT("the cases cover several states (%d)"), Seen.Num()), Seen.Num() >= 4);
	TestTrue(TEXT("found the SPN movement"), Anim->HasSpnMovement());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnBodyHeightTest, "SpnGame.Character.BodyHeightKeepsGameplay", Flags)
bool FSpnBodyHeightTest::RunTest(const FString&)
{
	FCharWorld W;
	if (!TestNotNull(TEXT("player character spawned"), W.Player)) { return false; }
	USpnCharacterVisualComponent* Visual = W.Player->GetVisual();
	if (!TestNotNull(TEXT("visual component"), Visual)) { return false; }
	USkeletalMeshComponent* Mesh = W.Player->GetMesh();
	UCapsuleComponent* Capsule = W.Player->GetCapsuleComponent();
	USpnCharacterMovementComponent* Movement = W.Player->GetSpnMovement();

	const float Radius = Capsule->GetUnscaledCapsuleRadius(), Half = Capsule->GetUnscaledCapsuleHalfHeight();
	const float Step = Movement->MaxStepHeight, JumpZ = Movement->JumpZVelocity, Walk = Movement->MaxWalkSpeed;
	const FVector MeshScale = Mesh->GetRelativeScale3D(), MeshLocation = Mesh->GetRelativeLocation();
	USkeletalMesh* MeshAsset = Mesh->GetSkeletalMeshAsset();
	const FVector CameraBase = W.Player->GetCameraBase();
	const float Reference = Visual->GetReferenceHeightCm();
	AddInfo(FString::Printf(TEXT("mannequin reference height %.1f cm (from %s), camera base z %.1f"), Reference, *GetNameSafe(MeshAsset), CameraBase.Z));
	TestTrue(TEXT("no preset by default: the blueprint's presentation is untouched"), Visual->GetActivePreset().IsNone() && FMath::IsNearlyEqual(Visual->GetBodyHeightCm(), Reference));
	TestTrue(*FString::Printf(TEXT("the mannequin measures like a person (%.0f cm)"), Reference), Reference > 150.f && Reference < 210.f);

	for (const float Height : {160.f, 195.f})
	{
		const FName Id(*FString::Printf(TEXT("test_%.0f"), Height));
		TestTrue(*FString::Printf(TEXT("%s applies"), *Id.ToString()), Visual->ApplyPreset(Id));
		TestTrue(*FString::Printf(TEXT("%s: body height %.1f"), *Id.ToString(), Visual->GetBodyHeightCm()), FMath::IsNearlyEqual(Visual->GetBodyHeightCm(), Height, 0.5f));
		TestTrue(*FString::Printf(TEXT("%s: mesh scaled"), *Id.ToString()), Mesh->GetRelativeScale3D().Equals(MeshScale * (Height / Reference), 0.001f));
		TestTrue(*FString::Printf(TEXT("%s: feet stay on the capsule bottom (mesh origin unchanged)"), *Id.ToString()), Mesh->GetRelativeLocation().Equals(MeshLocation, 0.01f));
		const float ExpectedCameraZ = CameraBase.Z + (Height - Reference) * Visual->CameraHeightFactor;
		TestTrue(*FString::Printf(TEXT("%s: camera pivot z %.1f (expected %.1f)"), *Id.ToString(), W.Player->GetCameraBase().Z, ExpectedCameraZ),
			FMath::IsNearlyEqual(static_cast<float>(W.Player->GetCameraBase().Z), ExpectedCameraZ, 0.1f));
		TestTrue(*FString::Printf(TEXT("%s: boom follows the base"), *Id.ToString()), FMath::IsNearlyEqual(static_cast<float>(W.Player->GetCameraBoom()->GetRelativeLocation().Z), ExpectedCameraZ, 0.1f));
		TestTrue(*FString::Printf(TEXT("%s: capsule unchanged"), *Id.ToString()),
			FMath::IsNearlyEqual(Capsule->GetUnscaledCapsuleRadius(), Radius) && FMath::IsNearlyEqual(Capsule->GetUnscaledCapsuleHalfHeight(), Half));
		TestTrue(*FString::Printf(TEXT("%s: step, jump and walk speed unchanged"), *Id.ToString()),
			FMath::IsNearlyEqual(Movement->MaxStepHeight, Step) && FMath::IsNearlyEqual(Movement->JumpZVelocity, JumpZ) && FMath::IsNearlyEqual(Movement->MaxWalkSpeed, Walk));
		TestTrue(*FString::Printf(TEXT("%s: same body asset (a scaled mannequin, no new content)"), *Id.ToString()), Mesh->GetSkeletalMeshAsset() == MeshAsset);
	}

	TestTrue(TEXT("none restores the blueprint's presentation"), Visual->ApplyPreset(NAME_None));
	TestTrue(TEXT("mesh scale restored"), Mesh->GetRelativeScale3D().Equals(MeshScale, 0.0001f));
	TestTrue(TEXT("camera base restored"), W.Player->GetCameraBase().Equals(CameraBase, 0.01f));
	TestFalse(TEXT("an unknown preset is refused"), Visual->ApplyPreset(TEXT("no_such_body")));
	TestTrue(TEXT("and changes nothing"), Visual->GetActivePreset().IsNone() && Mesh->GetRelativeScale3D().Equals(MeshScale, 0.0001f));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnBodyFollowerTest, "SpnGame.Character.FollowersAndMissingAssets", Flags)
bool FSpnBodyFollowerTest::RunTest(const FString&)
{
	FCharWorld W;
	if (!TestNotNull(TEXT("player character spawned"), W.Player)) { return false; }
	USpnCharacterVisualComponent* Visual = W.Player->GetVisual();
	USkeletalMeshComponent* Mesh = W.Player->GetMesh();
	USkeletalMesh* MeshAsset = Mesh->GetSkeletalMeshAsset();
	UClass* AnimClass = Mesh->GetAnimClass();

	// clothing-slot demo (0013 S2): a second template mannequin mesh as a follower of the body's pose
	FSpnBodyPreset Demo;
	Demo.Id = TEXT("follower_demo");
	Demo.Followers.Add(TSoftObjectPtr<USkeletalMesh>(FSoftObjectPath(TEXT("/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple"))));
	Visual->Presets.Add(Demo);
	TestTrue(TEXT("demo applies"), Visual->ApplyPreset(Demo.Id));
	if (TestEqual(TEXT("one follower"), Visual->GetFollowers().Num(), 1))
	{
		const USkeletalMeshComponent* Follower = Visual->GetFollowers()[0];
		TestTrue(TEXT("follower uses the body as leader pose"), Follower->LeaderPoseComponent.Get() == Mesh);
		TestTrue(TEXT("follower is attached to the body"), Follower->GetAttachParent() == Mesh);
		TestTrue(TEXT("follower has no collision (gameplay stays with the capsule)"), Follower->GetCollisionEnabled() == ECollisionEnabled::NoCollision);
	}
	TestEqual(TEXT("nothing missing"), Visual->GetMissingAssetCount(), 0);

	// a preset whose assets are not installed (e.g. Fab / MetaHuman content on another machine): fallback to the mannequin, reported
	FSpnBodyPreset Missing;
	Missing.Id = TEXT("not_installed");
	Missing.BodyMesh = TSoftObjectPtr<USkeletalMesh>(FSoftObjectPath(TEXT("/Game/NotInstalled/SKM_Missing.SKM_Missing")));
	Missing.AnimClass = TSoftClassPtr<UAnimInstance>(FSoftObjectPath(TEXT("/Game/NotInstalled/ABP_Missing.ABP_Missing_C")));
	Missing.Followers.Add(TSoftObjectPtr<USkeletalMesh>(FSoftObjectPath(TEXT("/Game/NotInstalled/SKM_Coat.SKM_Coat"))));
	Visual->Presets.Add(Missing);
	TestTrue(TEXT("missing preset still applies"), Visual->ApplyPreset(Missing.Id));
	TestEqual(TEXT("three missing assets reported"), Visual->GetMissingAssetCount(), 3);
	TestTrue(TEXT("the mannequin body stays"), Mesh->GetSkeletalMeshAsset() == MeshAsset);
	TestTrue(TEXT("the anim blueprint stays"), Mesh->GetAnimClass() == AnimClass);
	TestEqual(TEXT("the old followers are removed, no new one"), Visual->GetFollowers().Num(), 0);
	return true;
}

// Protagonist V1 feasibility phase (2026-11-14, Docs/Proposals/0015): the config preset mh_template = the engine's MetaHuman TEMPLATE body on its
// own skeleton, posed by runtime IK retargeting from the hidden gameplay mesh. Needs the local retargeter of Tools/character_mh_test_setup.py.
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnRetargetedBodyTest, "SpnGame.Character.RetargetedTestBody", Flags)
bool FSpnRetargetedBodyTest::RunTest(const FString&)
{
	FCharWorld W;
	if (!TestNotNull(TEXT("player character spawned"), W.Player)) { return false; }
	USpnCharacterVisualComponent* Visual = W.Player->GetVisual();
	USkeletalMeshComponent* Mesh = W.Player->GetMesh();
	UCapsuleComponent* Capsule = W.Player->GetCapsuleComponent();
	USpnCharacterMovementComponent* Movement = W.Player->GetSpnMovement();
	USkeletalMesh* MeshAsset = Mesh->GetSkeletalMeshAsset();
	UClass* AnimClass = Mesh->GetAnimClass();
	const FVector MeshScale = Mesh->GetRelativeScale3D(), CameraBase = W.Player->GetCameraBase();
	const EVisibilityBasedAnimTickOption TickOption = Mesh->VisibilityBasedAnimTickOption;
	const float Half = Capsule->GetUnscaledCapsuleHalfHeight(), Step = Movement->MaxStepHeight, Walk = Movement->MaxWalkSpeed;

	TestTrue(TEXT("mh_template is a known preset (DefaultGame.ini ConfigPresets)"), Visual->GetPresetIds().Contains(FName(TEXT("mh_template"))));
	if (!TestTrue(TEXT("mh_template applies"), Visual->ApplyPreset(TEXT("mh_template")))) { return false; }
	if (!TestEqual(TEXT("no missing asset (else run Tools/character_mh_test_setup.py)"), Visual->GetMissingAssetCount(), 0)) { return false; }
	USkeletalMeshComponent* Body = Visual->GetRetargetBody();
	if (!TestNotNull(TEXT("a retargeted body exists"), Body)) { return false; }
	TestTrue(TEXT("it shows a body on another skeleton"), Body->GetSkeletalMeshAsset() != nullptr && Body->GetSkeletalMeshAsset()->GetSkeleton() != MeshAsset->GetSkeleton());
	TestTrue(TEXT("it is attached to the gameplay mesh (same origin: feet at the capsule bottom)"), Body->GetAttachParent() == Mesh && Body->GetRelativeLocation().IsNearlyZero());
	TestTrue(TEXT("it has no collision"), Body->GetCollisionEnabled() == ECollisionEnabled::NoCollision);
	TestTrue(TEXT("it is the shown body"), Visual->GetShownBody() == Body);
	TestTrue(TEXT("the gameplay mesh is hidden"), !Mesh->IsVisible());
	TestTrue(TEXT("the gameplay mesh still updates its bones while hidden"), Mesh->VisibilityBasedAnimTickOption == EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones);
	TestTrue(TEXT("the gameplay mesh keeps its mesh, anim blueprint and scale"), Mesh->GetSkeletalMeshAsset() == MeshAsset && Mesh->GetAnimClass() == AnimClass && Mesh->GetRelativeScale3D().Equals(MeshScale));
	USpnRetargetFollowAnimInstance* Follow = Cast<USpnRetargetFollowAnimInstance>(Body->GetAnimInstance());
	TestNotNull(TEXT("the body runs the retarget anim instance"), Follow);
	if (TestEqual(TEXT("one follower (the head)"), Visual->GetFollowers().Num(), 1))
	{
		TestTrue(TEXT("the head follows the retargeted body"), Visual->GetFollowers()[0]->LeaderPoseComponent.Get() == Body && Visual->GetFollowers()[0]->GetAttachParent() == Body);
	}
	TestTrue(TEXT("capsule, step and walk speed unchanged"), FMath::IsNearlyEqual(Capsule->GetUnscaledCapsuleHalfHeight(), Half) && FMath::IsNearlyEqual(Movement->MaxStepHeight, Step) && FMath::IsNearlyEqual(Movement->MaxWalkSpeed, Walk));
	TestTrue(TEXT("camera base unchanged"), W.Player->GetCameraBase().Equals(CameraBase, 0.01f));

	// evaluate a few frames by hand (no world tick in this test): the source pose first, then the retarget
	for (int32 Frame = 0; Frame < 3; ++Frame)
	{
		Mesh->TickAnimation(1.f / 30.f, false);
		Mesh->RefreshBoneTransforms();
		Body->TickAnimation(1.f / 30.f, false);
		Body->RefreshBoneTransforms();
	}
	if (Follow != nullptr)
	{
		TestTrue(TEXT("the retarget processor initialised with mannequin -> MetaHuman"), Follow->IsRetargetReady());
	}
	const FVector Head = Body->GetSocketLocation(TEXT("head")) - Body->GetComponentLocation();
	AddInfo(FString::Printf(TEXT("retargeted head bone %.1f cm above the feet (mannequin %.1f)"), Head.Z, (Mesh->GetSocketLocation(TEXT("head")) - Mesh->GetComponentLocation()).Z));
	TestTrue(TEXT("the retargeted head is at a standing person's height (not a collapsed reference pose)"), Head.Z > 120.f && Head.Z < 200.f);

	// switching back and to another preset leaves nothing behind
	TestTrue(TEXT("none applies"), Visual->ApplyPreset(NAME_None));
	TestTrue(TEXT("the retargeted body is gone"), Visual->GetRetargetBody() == nullptr && Visual->GetFollowers().Num() == 0);
	TestTrue(TEXT("the gameplay mesh is visible again with its old tick option"), Mesh->IsVisible() && Mesh->VisibilityBasedAnimTickOption == TickOption);
	TestTrue(TEXT("test_195 still works after it"), Visual->ApplyPreset(TEXT("test_195")) && Mesh->IsVisible() && Visual->GetRetargetBody() == nullptr);
	return true;
}

// Modular body architecture (2026-11-15, Docs/Proposals/0018): wearables are separate components on the shown body, optional detail geometry
// exists only when the policy enables its category AND its zone is exposed, everything follows a body preset change. Neutral test data only.
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnWardrobeTest, "SpnGame.Character.WardrobeLayersAndDetails", Flags)
bool FSpnWardrobeTest::RunTest(const FString&)
{
	FCharWorld W;
	if (!TestNotNull(TEXT("player character spawned"), W.Player)) { return false; }
	USpnWardrobeComponent* Wardrobe = W.Player->GetWardrobe();
	USpnCharacterVisualComponent* Visual = W.Player->GetVisual();
	if (!TestNotNull(TEXT("wardrobe component"), Wardrobe)) { return false; }
	USkeletalMeshComponent* Mesh = W.Player->GetMesh();
	USkeletalMesh* BodyAsset = Mesh->GetSkeletalMeshAsset();
	const float Half = W.Player->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();

	Wardrobe->Reapply();
	TestTrue(TEXT("default: nothing worn, nothing drawn, no detail"), Wardrobe->GetWorn().IsEmpty() && Wardrobe->GetWearableComponents().IsEmpty() && Wardrobe->GetDetailComponents().IsEmpty());
	TestTrue(TEXT("the config holds the neutral detail marker"), Wardrobe->Details.ContainsByPredicate([](const FSpnBodyDetailItem& D) { return D.Def.Id == FName(TEXT("base:detail_marker_test")); }));

	const int32 Trunk = SpnBodyCoverage::ZoneBit(ESpnBodyZone::Chest) | SpnBodyCoverage::ZoneBit(ESpnBodyZone::Abdomen) | SpnBodyCoverage::ZoneBit(ESpnBodyZone::Back);
	const int32 Hips = SpnBodyCoverage::ZoneBit(ESpnBodyZone::Pelvis) | SpnBodyCoverage::ZoneBit(ESpnBodyZone::Buttocks);
	auto MakeWearable = [](const TCHAR* Id, ESpnWearLayer Layer, int32 Covers, int32 Hides, bool bOpaque, const TCHAR* MeshPath)
	{
		FSpnWearableDef D;
		D.Coverage.Item = Id; D.Coverage.Layer = Layer; D.Coverage.Covers = Covers; D.Coverage.HidesBody = Hides; D.Coverage.bOpaque = bOpaque;
		if (MeshPath != nullptr) { D.Mesh = TSoftObjectPtr<USkeletalMesh>(FSoftObjectPath(MeshPath)); }
		return D;
	};
	// a dummy garment mesh (the template Quinn mesh) - only to prove "own component, leader pose, never merged"
	Wardrobe->Wearables.Add(MakeWearable(TEXT("base:underwear_test"), ESpnWearLayer::Underwear, Hips, Hips, true, nullptr));
	Wardrobe->Wearables.Add(MakeWearable(TEXT("base:shirt_test"), ESpnWearLayer::Base, Trunk, Trunk, true, TEXT("/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple")));
	Wardrobe->Wearables.Add(MakeWearable(TEXT("base:sheer_test"), ESpnWearLayer::Base, Trunk, 0, false, nullptr));

	Wardrobe->SetWorn({TEXT("base:underwear_test"), TEXT("base:shirt_test")});
	if (TestEqual(TEXT("one drawn wearable (the data-only underwear draws nothing)"), Wardrobe->GetWearableComponents().Num(), 1))
	{
		const USkeletalMeshComponent* Shirt = Wardrobe->GetWearableComponents()[0];
		TestTrue(TEXT("the wearable is its own component following the shown body"), Shirt != Mesh && Shirt->LeaderPoseComponent.Get() == Visual->GetShownBody() && Shirt->GetAttachParent() == Visual->GetShownBody());
		TestTrue(TEXT("the wearable has no collision"), Shirt->GetCollisionEnabled() == ECollisionEnabled::NoCollision);
	}
	TestTrue(TEXT("the body mesh asset is untouched (nothing merged into it)"), Mesh->GetSkeletalMeshAsset() == BodyAsset);
	TestEqual(TEXT("covered = hips + trunk"), Wardrobe->GetLastResult().Covered, Hips | Trunk);
	TestEqual(TEXT("hidden body = hips + trunk"), Wardrobe->GetLastResult().HiddenBody, Hips | Trunk);
	AddInfo(FString::Printf(TEXT("hidden body sections on the mannequin: %d (it has no SpnZone_ material slots, so nothing can be hidden - expected)"), Wardrobe->GetHiddenSectionCount()));

	// optional detail: off by default, on with the category, gone again when the zone is covered
	Wardrobe->SetWorn({});
	TestEqual(TEXT("policy empty: no detail even with the zone exposed"), Wardrobe->GetDetailComponents().Num(), 0);
	FSpnPresentationPolicy Policy;
	Policy.EnabledDetailCategories.Add(TEXT("base:test_marker"));
	Wardrobe->SetPolicy(Policy);
	if (TestEqual(TEXT("category enabled + chest exposed: one detail component"), Wardrobe->GetDetailComponents().Num(), 1))
	{
		const UPrimitiveComponent* Detail = Wardrobe->GetDetailComponents()[0];
		TestTrue(TEXT("detail attached to the shown body at spine_03"), Detail->GetAttachParent() == Visual->GetShownBody() && Detail->GetAttachSocketName() == FName(TEXT("spine_03")));
		TestTrue(TEXT("detail has a draw distance (LOD rule)"), Detail->CachedMaxDrawDistance > 0.f);
	}
	Wardrobe->SetWorn({TEXT("base:sheer_test")});
	TestEqual(TEXT("a sheer top does not cover: detail stays"), Wardrobe->GetDetailComponents().Num(), 1);
	Wardrobe->SetWorn({TEXT("base:shirt_test")});
	TestEqual(TEXT("an opaque top covers the chest: the detail component no longer exists (no cost when dressed)"), Wardrobe->GetDetailComponents().Num(), 0);

	// unknown ids are reported, never fatal
	Wardrobe->SetWorn({TEXT("base:no_such_item")});
	TestTrue(TEXT("unknown wearable reported"), Wardrobe->GetLastResult().Issues.ContainsByPredicate([](const FString& S) { return S.Contains(TEXT("no_such_item")); }));

	// a body preset change re-attaches everything to the new shown body (the retargeted MetaHuman, if its local assets exist)
	Wardrobe->SetWorn({TEXT("base:shirt_test")});
	if (Visual->GetPresetIds().Contains(FName(TEXT("mh_template"))) && Visual->ApplyPreset(TEXT("mh_template")) && Visual->GetRetargetBody() != nullptr)
	{
		TestTrue(TEXT("after the preset change the wearable follows the retargeted body"),
			Wardrobe->GetWearableComponents().Num() == 1 && Wardrobe->GetWearableComponents()[0]->LeaderPoseComponent.Get() == Visual->GetRetargetBody());
		Visual->ApplyPreset(NAME_None);
		TestTrue(TEXT("and back on the mannequin after 'none'"), Wardrobe->GetWearableComponents().Num() == 1 && Wardrobe->GetWearableComponents()[0]->LeaderPoseComponent.Get() == Mesh);
	}
	TestTrue(TEXT("gameplay capsule unchanged by all of it"), FMath::IsNearlyEqual(W.Player->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight(), Half));
	return true;
}

#endif
