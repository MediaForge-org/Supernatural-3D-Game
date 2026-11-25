#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "DynamicRHI.h"
#include "Engine/CollisionProfile.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/Texture.h"
#include "RHIStats.h"
#include "RenderTimer.h"
#include "Spn/Character/SpnCharacterVisualComponent.h"
#include "Spn/Character/SpnRetargetFollowAnimInstance.h"
#include "Spn/Character/SpnWardrobeComponent.h"
#include "Spn/Tests/SpnMovementTestSupport.h"
#include "Spn/Tests/SpnQaRecorder.h"

#if WITH_DEV_AUTOMATION_TESTS

// SpnQA.CharacterBody (Protagonist V1 feasibility phase, developer order 2026-11-14, tasks 2 and 5; Docs/Proposals/0015):
// the current placeholder (UE5 mannequin, no preset) and the MetaHuman TEMPLATE test preset `mh_template` (engine content, runtime IK
// retargeting from the hidden mannequin) go through the SAME scenarios in one run, so the numbers are comparable:
//   views     fixed camera actors in front of / beside the character -> shots/CharacterBody_<preset>_<view>.png (comparison sheets)
//   lod/perf  the camera at 3 / 6 / 12 / 25 / 50 m: predicted LOD of the shown body and the head, GPU / game / render ms, draw calls
//   memory    textures used by the shown parts (resident and all mips) and the skeletal mesh resource size
//   gaits     walk, jog, sprint on the open lane: speed (must not change with the body), foot planting / sliding / ground contact
//   jump      standing jump: lands, foot height at the apex
//   ledge     a 90 cm mantle and a 35 cm step-on: the same move completes with either body (gameplay is the capsule)
// Thresholds: the gameplay checks (speed, move completes, retarget runs) are pass/fail. The foot, LOD and cost numbers are MEASUREMENTS
// (status PASS = measured), judged in the report against the orientation targets of 0015 section 8, not as hard limits.

using namespace SpnMovementTest;

namespace
{
	constexpr float LaneX = -12500.f;
	constexpr float LaneStartY = -12000.f;
	constexpr EAutomationTestFlags QaFlags = EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter;

	FString PresetLabel(FName Preset) { return Preset.IsNone() ? TEXT("placeholder") : Preset.ToString(); }

	USpnCharacterVisualComponent* VisualOf(ASupernaturalProtCharacter& Character) { return Character.GetVisual(); }

	/** Every mesh the player sees: the shown body and its followers. */
	TArray<USkeletalMeshComponent*> ShownParts(ASupernaturalProtCharacter& Character)
	{
		TArray<USkeletalMeshComponent*> Parts;
		if (USpnCharacterVisualComponent* Visual = VisualOf(Character))
		{
			Parts.Add(Visual->GetShownBody());
			for (USkeletalMeshComponent* F : Visual->GetFollowers()) { Parts.Add(F); }
		}
		else
		{
			Parts.Add(Character.GetMesh());
		}
		return Parts;
	}

	struct FFrameSum
	{
		int32 N = 0;
		double Gpu = 0, Game = 0, Render = 0, Draws = 0, Prims = 0;
		void Add()
		{
			++N;
			Gpu += FPlatformTime::ToMilliseconds(RHIGetGPUFrameCycles(0));
			Game += FPlatformTime::ToMilliseconds(GGameThreadTime);
			Render += FPlatformTime::ToMilliseconds(GRenderThreadTime);
			Draws += GNumDrawCallsRHI[0];
			Prims += GNumPrimitivesDrawnRHI[0];
		}
		FString Text() const
		{
			const double D = FMath::Max(N, 1);
			return FString::Printf(TEXT("GPU %.2f ms, game %.2f ms, render %.2f ms, %.0f draws, %.0f primitives (%d frames)"), Gpu / D, Game / D, Render / D, Draws / D, Prims / D, N);
		}
	};

	double RetargetMs(ASupernaturalProtCharacter& Character)
	{
		USpnCharacterVisualComponent* Visual = VisualOf(Character);
		USkeletalMeshComponent* Body = Visual != nullptr ? Visual->GetRetargetBody() : nullptr;
		USpnRetargetFollowAnimInstance* Follow = Body != nullptr ? Cast<USpnRetargetFollowAnimInstance>(Body->GetAnimInstance()) : nullptr;
		return Follow != nullptr ? Follow->GetAverageRetargetMs() : 0.0;
	}

	/** Foot samples of one run: ball bones (toe joint) of the shown body in world space. */
	struct FFootRun
	{
		SpnQa::FHeldKeys Keys;
		TArray<double> T;
		TArray<FVector> L, R;
		double SpeedSum = 0; int32 SpeedN = 0;
		double RetargetMsSum = 0; int32 RetargetN = 0;
		float MaxFootZ = 0.f;
		float HandBestDz = 1.0e9f, HandAtFace = 0.f;   // ledge moves: closest a hand came to the top surface, and its distance past the face then
		bool bLeftGround = false, bLanded = false;
		TArray<TArray<double>> Trace;
	};

	/** Planted = within 2.5 cm of the lowest height that foot reached in the run; sliding = horizontal speed between two planted samples. */
	FString FootStats(const FFootRun& Run, double& OutSlideP50, double& OutSlideP90, double& OutLowZ)
	{
		TArray<double> Slides;
		double Low = 1.0e9, PlantedZSum = 0; int32 Planted = 0, Samples = 0;
		for (const TArray<FVector>* Foot : {&Run.L, &Run.R})
		{
			double MinZ = 1.0e9;
			for (const FVector& P : *Foot) { MinZ = FMath::Min(MinZ, P.Z); }
			Low = FMath::Min(Low, MinZ);
			for (int32 i = 1; i < Foot->Num(); ++i)
			{
				++Samples;
				const bool bNow = (*Foot)[i].Z < MinZ + 2.5, bBefore = (*Foot)[i - 1].Z < MinZ + 2.5;
				if (bNow) { ++Planted; PlantedZSum += (*Foot)[i].Z; }
				const double Dt = Run.T[i] - Run.T[i - 1];
				if (bNow && bBefore && Dt > 1.0e-4) { Slides.Add(FVector::Dist2D((*Foot)[i], (*Foot)[i - 1]) / Dt); }
			}
		}
		Slides.Sort();
		auto Pct = [&Slides](double Q) { return Slides.Num() > 0 ? Slides[FMath::Clamp(static_cast<int32>(Q * (Slides.Num() - 1)), 0, Slides.Num() - 1)] : -1.0; };
		OutSlideP50 = Pct(0.5);
		OutSlideP90 = Pct(0.9);
		OutLowZ = Low;
		return FString::Printf(TEXT("planted %.0f%% of foot samples at %.1f cm (lowest %.1f cm) above the floor, sliding of a planted foot: median %.1f cm/s, 90th percentile %.1f cm/s (%d pairs)"),
			Samples > 0 ? 100.0 * Planted / Samples : 0.0, Planted > 0 ? PlantedZSum / Planted : -1.0, Low, OutSlideP50, OutSlideP90, Slides.Num());
	}

	void SampleFeet(ASupernaturalProtCharacter& Character, FFootRun& Run, double Elapsed)
	{
		USkeletalMeshComponent* Body = ShownParts(Character)[0];
		Run.T.Add(Elapsed);
		Run.L.Add(Body->GetSocketLocation(TEXT("ball_l")));
		Run.R.Add(Body->GetSocketLocation(TEXT("ball_r")));
		Run.MaxFootZ = FMath::Max(Run.MaxFootZ, static_cast<float>(FMath::Min(Run.L.Last().Z, Run.R.Last().Z)));
		if (Run.Trace.Num() < 400) { Run.Trace.Add({Elapsed, Run.L.Last().Z, Run.R.Last().Z, Character.GetVelocity().Size2D(), Run.L.Last().Y, Run.R.Last().Y}); }
		const double Ms = RetargetMs(Character);
		if (Ms > 0.0) { Run.RetargetMsSum += Ms; ++Run.RetargetN; }
	}

	AStaticMeshActor* SpawnBox(UWorld& World, const FVector& Center, const FVector& Size)
	{
		AStaticMeshActor* Actor = World.SpawnActor<AStaticMeshActor>(Center, FRotator::ZeroRotator);
		Actor->SetMobility(EComponentMobility::Movable);
		Actor->GetStaticMeshComponent()->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube")));
		Actor->GetStaticMeshComponent()->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
		Actor->SetActorScale3D(Size / 100.f);
		return Actor;
	}

	struct FViewRun
	{
		TObjectPtr<ACameraActor> Camera;
		int32 Stage = -1;
		double StageStart = 0;
		FFrameSum Frames;
		TArray<FString> Lines;
	};
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnQaCharacterBodyTest, "SpnQA.CharacterBody", QaFlags)

bool FSpnQaCharacterBodyTest::RunTest(const FString& Parameters)
{
	if (!FPackageName::DoesPackageExist(SpnQa::QaMapPath()))
	{
		AddError(*FString::Printf(TEXT("%s does not exist"), *SpnQa::QaMapPath()));
		return false;
	}
	AutomationOpenMap(SpnQa::QaMapPath());
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));

	const TSharedRef<FName> Initial = MakeShared<FName>();
	const TSharedRef<TMap<FString, double>> Speeds = MakeShared<TMap<FString, double>>();
	// -SpnQaBodies=none,mh_template,mh_male_193 (default: all three); "none" = the placeholder and always runs first (speed reference)
	FString BodyList = TEXT("none,mh_template,mh_male_193");
	// bShouldStopOnSeparator = false: the list is comma-separated (before NightBuild06 the parse stopped at the first comma, so an explicit
	// list ran only its first body)
	FParse::Value(FCommandLine::Get(), TEXT("SpnQaBodies="), BodyList, /*bShouldStopOnSeparator*/ false);
	TArray<FString> BodyIds;
	BodyList.ParseIntoArray(BodyIds, TEXT(","));
	TArray<FName> Presets = {NAME_None};
	for (const FString& Id : BodyIds) { if (!Id.Equals(TEXT("none"), ESearchCase::IgnoreCase)) { Presets.Add(FName(*Id)); } }
	const FVector ViewSpot(LaneX, LaneStartY, StandZ);

	for (const FName Preset : Presets)
	{
		const FString Label = PresetLabel(Preset);

		// --- apply the preset, views, LOD and cost
		const TSharedRef<FViewRun> View = MakeShared<FViewRun>();
		ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(*this, ViewSpot, 90.f, 40.f,
			[this, Preset, Label, Initial, View](ASupernaturalProtCharacter& Character)
			{
				SpnQa::Section(FString::Printf(TEXT("Character body %s: views, LOD, cost"), *Label));
				USpnCharacterVisualComponent* Visual = VisualOf(Character);
				if (Preset.IsNone() && Visual != nullptr) { *Initial = Visual->GetActivePreset(); }
				if (Visual != nullptr) { Visual->ApplyPreset(Preset); }
				View->Camera = Character.GetWorld()->SpawnActor<ACameraActor>(FVector::ZeroVector, FRotator::ZeroRotator);
			},
			[View, Label](ASupernaturalProtCharacter& Character, float, double Elapsed)
			{
				// stages: 0..3 views (2 s each), 4..8 LOD distances (1.5 s each)
				struct FStage { const TCHAR* Name; float Distance; float Yaw; float Height; float Fov; float Seconds; bool bShot; };
				static const FStage Stages[] = {
					{TEXT("front_full"), 520.f, 0.f, 100.f, 40.f, 2.0f, true},
					{TEXT("front_head"), 260.f, 0.f, -1.f, 18.f, 2.0f, true},          // Height < 0: aimed at the shown body's head bone
					{TEXT("threequarter_head"), 260.f, 40.f, -1.f, 18.f, 2.0f, true},
					{TEXT("side_full"), 520.f, 90.f, 100.f, 40.f, 2.0f, true},
					{TEXT("lod_3m"), 300.f, 20.f, 120.f, 70.f, 1.5f, false},
					{TEXT("lod_6m"), 600.f, 20.f, 120.f, 70.f, 1.5f, false},
					{TEXT("lod_12m"), 1200.f, 20.f, 120.f, 70.f, 1.5f, false},
					{TEXT("lod_25m"), 2500.f, 20.f, 120.f, 70.f, 1.5f, false},
					{TEXT("lod_50m"), 5000.f, 20.f, 120.f, 70.f, 1.5f, false},
				};
				APlayerController* PC = Cast<APlayerController>(Character.GetController());
				if (PC == nullptr || View->Camera == nullptr) { return true; }
				const int32 Count = UE_ARRAY_COUNT(Stages);
				if (View->Stage < 0 || Elapsed - View->StageStart >= Stages[View->Stage].Seconds)
				{
					if (View->Stage >= 0 && View->Stage < Count)
					{
						const FStage& Done = Stages[View->Stage];
						TArray<FString> Lods;
						for (USkeletalMeshComponent* Part : ShownParts(Character))
						{
							Lods.Add(FString::Printf(TEXT("%s LOD %d/%d"), *GetNameSafe(Part->GetSkeletalMeshAsset()), Part->GetPredictedLODLevel(), Part->GetNumLODs()));
						}
						View->Lines.Add(FString::Printf(TEXT("%s: %s; %s"), Done.Name, *FString::Join(Lods, TEXT(", ")), *View->Frames.Text()));
					}
					++View->Stage;
					View->StageStart = Elapsed;
					View->Frames = FFrameSum();
					if (View->Stage >= Count) { return true; }
					// the character faces +Y (yaw 90); the camera stands in front of it, turned around the character by Yaw
					const FStage& S = Stages[View->Stage];
					const FVector Feet = Character.GetActorLocation() - FVector(0.f, 0.f, Character.GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
					const FVector Facing = FRotator(0.f, 90.f + S.Yaw, 0.f).Vector();
					const FVector Target = S.Height >= 0.f ? Feet + FVector(0.f, 0.f, S.Height)
						: ShownParts(Character)[0]->GetSocketLocation(TEXT("head")) + FVector(0.f, 0.f, 4.f);
					const FVector Eye = Target + Facing * S.Distance;
					View->Camera->SetActorLocationAndRotation(Eye, (Target - Eye).Rotation());
					View->Camera->GetCameraComponent()->SetFieldOfView(S.Fov);
					View->Camera->GetCameraComponent()->bConstrainAspectRatio = false;
					PC->SetViewTarget(View->Camera);
					return false;
				}
				const FStage& S = Stages[View->Stage];
				const double In = Elapsed - View->StageStart;
				if (In > 0.6) { View->Frames.Add(); }
				if (S.bShot && In > 1.2 && In - Character.GetWorld()->GetDeltaSeconds() <= 1.2)
				{
					FScreenshotRequest::RequestScreenshot(SpnQa::QaDir() / TEXT("shots") / FString::Printf(TEXT("CharacterBody_%s_%s.png"), *Label, S.Name), false, false);
				}
				return false;
			},
			[this, View, Label, Preset](ASupernaturalProtCharacter& Character, bool bTimedOut)
			{
				if (APlayerController* PC = Cast<APlayerController>(Character.GetController())) { PC->SetViewTarget(&Character); }
				if (View->Camera != nullptr) { View->Camera->Destroy(); }
				USpnCharacterVisualComponent* Visual = VisualOf(Character);
				const bool bApplied = Visual != nullptr && Visual->GetActivePreset() == Preset;
				const bool bRetarget = !Preset.IsNone();
				USkeletalMeshComponent* RetargetBody = Visual != nullptr ? Visual->GetRetargetBody() : nullptr;
				USpnRetargetFollowAnimInstance* Follow = RetargetBody != nullptr ? Cast<USpnRetargetFollowAnimInstance>(RetargetBody->GetAnimInstance()) : nullptr;
				FString Why;
				if (!bApplied) { Why = TEXT("preset not active"); }
				else if (bRetarget && (RetargetBody == nullptr || Follow == nullptr)) { Why = TEXT("no retargeted body (asset missing? run Tools/character_mh_test_setup.py)"); }
				else if (bRetarget && !Follow->IsRetargetReady()) { Why = TEXT("the retarget processor did not initialise"); }
				else if (bRetarget && (Character.GetMesh()->IsVisible() || !RetargetBody->IsVisible())) { Why = TEXT("the gameplay mesh must be hidden and the retargeted body visible"); }
				else if (bRetarget && Visual->GetMissingAssetCount() > 0) { Why = FString::Printf(TEXT("%d asset(s) missing"), Visual->GetMissingAssetCount()); }
				else if (bTimedOut) { Why = TEXT("timed out"); }
				// proportions in the idle pose, measured on the SHOWN body from the capsule bottom; camera pivot and capsule for comparison
				USkeletalMeshComponent* Shown = ShownParts(Character)[0];
				const float Floor = Character.GetActorLocation().Z - Character.GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
				auto H = [Shown, Floor](const TCHAR* Bone) { return static_cast<float>(Shown->GetSocketLocation(Bone).Z) - Floor; };
				float Top = 0.f;
				for (USkeletalMeshComponent* Part : ShownParts(Character)) { Top = FMath::Max(Top, static_cast<float>(Part->Bounds.GetBox().Max.Z) - Floor); }
				const float ShoulderWidth = FVector::Dist(Shown->GetSocketLocation(TEXT("upperarm_l")), Shown->GetSocketLocation(TEXT("upperarm_r")));
				const FString Body = FString::Printf(TEXT("head bone %.1f, top of the shown meshes %.1f (bounds, idle), pelvis %.1f, hand_r %.1f, hip-to-ankle %.1f cm, shoulder joints %.1f cm apart; "
					"capsule radius %.0f / half height %.0f; camera base z %.1f; preset height %.0f"),
					H(TEXT("head")), Top, H(TEXT("pelvis")), H(TEXT("hand_r")), H(TEXT("thigh_l")) - H(TEXT("foot_l")), ShoulderWidth,
					Character.GetCapsuleComponent()->GetScaledCapsuleRadius(), Character.GetCapsuleComponent()->GetScaledCapsuleHalfHeight(),
					Character.GetCameraBase().Z, Visual != nullptr ? Visual->GetBodyHeightCm() : 0.f);
				SpnQa::Check(*this, &Character, TEXT("CharacterBody"), Label + TEXT("_applied"), Why.IsEmpty() ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail,
					(Why.IsEmpty() ? FString() : Why + TEXT(". ")) + FString::Printf(TEXT("shown parts: %d, follower(s) %d; "), ShownParts(Character).Num(), Visual != nullptr ? Visual->GetFollowers().Num() : 0) + Body);

				// memory of what is shown
				TSet<UTexture*> Textures;
				int64 MeshBytes = 0;
				TArray<FString> Meshes;
				for (USkeletalMeshComponent* Part : ShownParts(Character))
				{
					TArray<UTexture*> Used;
					// NightBuild06: EMaterialQualityLevel::Num trips an engine ensure in UE 5.8 (MaterialInterface.cpp), which marked this whole
					// automation test as failed in every run since 2026-11-14 while all its QA checks passed. High = the desktop default.
					Part->GetUsedTextures(Used, EMaterialQualityLevel::High);
					Textures.Append(Used);
					if (USkeletalMesh* Mesh = Part->GetSkeletalMeshAsset())
					{
						const int64 Bytes = Mesh->GetResourceSizeBytes(EResourceSizeMode::EstimatedTotal);
						MeshBytes += Bytes;
						TArray<FString> Screens;
						for (int32 Lod = 0; Lod < Mesh->GetLODNum(); ++Lod)
						{
							if (const FSkeletalMeshLODInfo* Info = Mesh->GetLODInfo(Lod)) { Screens.Add(FString::Printf(TEXT("%.3f"), Info->ScreenSize.Default)); }
						}
						Meshes.Add(FString::Printf(TEXT("%s %.1f MB, %d LODs (screen sizes %s), %d bones, %d materials, bounds radius %.0f cm, forced LOD %d"), *Mesh->GetName(), Bytes / 1048576.0,
							Mesh->GetLODNum(), *FString::Join(Screens, TEXT("/")), Mesh->GetRefSkeleton().GetNum(), Mesh->GetMaterials().Num(), Part->Bounds.SphereRadius, Part->GetForcedLOD()));
					}
				}
				int64 Resident = 0, AllMips = 0;
				int32 Biggest = 0;
				for (UTexture* Tex : Textures)
				{
					if (Tex == nullptr) { continue; }
					Resident += Tex->CalcTextureMemorySizeEnum(TMC_ResidentMips);
					AllMips += Tex->CalcTextureMemorySizeEnum(TMC_AllMips);
					Biggest = FMath::Max(Biggest, static_cast<int32>(Tex->GetSurfaceWidth()));
				}
				SpnQa::Check(*this, &Character, TEXT("CharacterBody"), Label + TEXT("_memory"), SpnQa::EStatus::Pass, FString::Printf(
					TEXT("measured: %d textures, %.1f MB resident / %.1f MB all mips, largest %d px; meshes %.1f MB (%s)"),
					Textures.Num(), Resident / 1048576.0, AllMips / 1048576.0, Biggest, MeshBytes / 1048576.0, *FString::Join(Meshes, TEXT("; "))));
				SpnQa::Check(*this, &Character, TEXT("CharacterBody"), Label + TEXT("_lod_cost"), View->Lines.Num() >= 9 ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail,
					TEXT("measured: ") + FString::Join(View->Lines, TEXT(" | ")) + FString::Printf(TEXT(" | retarget node %.3f ms"), RetargetMs(Character)));
			}));
		ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.3f));

		// --- modular body, rendered (NightBuild06 Phase H): the neutral detail marker (an engine sphere, category base:test_marker) appears on
		// the SHOWN body when its category is enabled and the chest is exposed, and no detail component exists once an opaque, data-only
		// test vest covers the chest. Two screenshots per body. PLACEHOLDER: no garment asset exists; the vest is data only (draws nothing).
		{
			struct FWardrobeRun { TObjectPtr<ACameraActor> Camera; int32 Stage = -1; double StageStart = 0; int32 Exposed = -1; int32 Covered = -1; bool bVisible = false; bool bOnShownBody = false; };
			const TSharedRef<FWardrobeRun> WR = MakeShared<FWardrobeRun>();
			ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(*this, ViewSpot, 90.f, 10.f,
				[WR, Label](ASupernaturalProtCharacter& Character)
				{
					SpnQa::Section(FString::Printf(TEXT("Character body %s: modular body marker"), *Label));
					WR->Camera = Character.GetWorld()->SpawnActor<ACameraActor>(FVector::ZeroVector, FRotator::ZeroRotator);
					USpnWardrobeComponent* Wardrobe = Character.GetWardrobe();
					if (Wardrobe == nullptr) { return; }
					if (!Wardrobe->Wearables.ContainsByPredicate([](const FSpnWearableDef& D) { return D.Coverage.Item == FName(TEXT("base:test_vest_data")); }))
					{
						FSpnWearableDef Vest;
						Vest.Coverage.Item = TEXT("base:test_vest_data");
						Vest.Coverage.Layer = ESpnWearLayer::Base;
						Vest.Coverage.Covers = SpnBodyCoverage::ZoneBit(ESpnBodyZone::Chest) | SpnBodyCoverage::ZoneBit(ESpnBodyZone::Abdomen) | SpnBodyCoverage::ZoneBit(ESpnBodyZone::Back);
						Vest.Coverage.bOpaque = true;
						Wardrobe->Wearables.Add(Vest);
					}
					FSpnPresentationPolicy Policy;
					Policy.EnabledDetailCategories.Add(TEXT("base:test_marker"));
					Wardrobe->SetWorn({});
					Wardrobe->SetPolicy(Policy);
				},
				[WR, Label](ASupernaturalProtCharacter& Character, float, double Elapsed)
				{
					APlayerController* PC = Cast<APlayerController>(Character.GetController());
					USpnWardrobeComponent* Wardrobe = Character.GetWardrobe();
					if (PC == nullptr || WR->Camera == nullptr || Wardrobe == nullptr) { return true; }
					if (WR->Stage < 0 || Elapsed - WR->StageStart >= 2.0)
					{
						++WR->Stage;
						WR->StageStart = Elapsed;
						if (WR->Stage == 1) { Wardrobe->SetWorn({TEXT("base:test_vest_data")}); }
						if (WR->Stage >= 2) { return true; }
						// close front view of the chest of the shown body
						const FVector Chest = ShownParts(Character)[0]->GetSocketLocation(TEXT("spine_03"));
						const FVector Eye = Chest + FRotator(0.f, 90.f, 0.f).Vector() * 140.f;
						WR->Camera->SetActorLocationAndRotation(Eye, (Chest - Eye).Rotation());
						WR->Camera->GetCameraComponent()->SetFieldOfView(40.f);
						PC->SetViewTarget(WR->Camera);
						return false;
					}
					const double In = Elapsed - WR->StageStart;
					if (In > 1.0 && In - Character.GetWorld()->GetDeltaSeconds() <= 1.0)
					{
						const int32 Count = Wardrobe->GetDetailComponents().Num();
						if (WR->Stage == 0)
						{
							WR->Exposed = Count;
							const UPrimitiveComponent* Marker = Count > 0 ? Wardrobe->GetDetailComponents()[0].Get() : nullptr;
							WR->bVisible = Marker != nullptr && Marker->IsVisible();
							WR->bOnShownBody = Marker != nullptr && Marker->GetAttachParent() == ShownParts(Character)[0];
						}
						else
						{
							WR->Covered = Count;
						}
						FScreenshotRequest::RequestScreenshot(SpnQa::QaDir() / TEXT("shots") / FString::Printf(TEXT("CharacterBody_%s_marker_%s.png"), *Label,
							WR->Stage == 0 ? TEXT("exposed") : TEXT("covered")), false, false);
					}
					return false;
				},
				[this, WR, Label](ASupernaturalProtCharacter& Character, bool bTimedOut)
				{
					if (APlayerController* PC = Cast<APlayerController>(Character.GetController())) { PC->SetViewTarget(&Character); }
					if (WR->Camera != nullptr) { WR->Camera->Destroy(); }
					if (USpnWardrobeComponent* Wardrobe = Character.GetWardrobe())
					{
						Wardrobe->SetWorn({});
						Wardrobe->SetPolicy(FSpnPresentationPolicy());   // back to the default: no optional detail at all
					}
					FString Why;
					if (bTimedOut) { Why = TEXT("timed out"); }
					else if (WR->Exposed != 1) { Why = FString::Printf(TEXT("exposed chest: %d detail component(s), expected 1"), WR->Exposed); }
					else if (!WR->bVisible) { Why = TEXT("the marker exists but is not visible"); }
					else if (!WR->bOnShownBody) { Why = TEXT("the marker is not attached to the shown body"); }
					else if (WR->Covered != 0) { Why = FString::Printf(TEXT("covered chest: %d detail component(s), expected 0"), WR->Covered); }
					SpnQa::Check(*this, &Character, TEXT("CharacterBody"), Label + TEXT("_modular_marker"), Why.IsEmpty() ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail,
						(Why.IsEmpty() ? FString() : Why + TEXT(". ")) + FString::Printf(TEXT("neutral marker on the shown body: exposed %d component(s) (visible %d), under the data-only test vest %d; screenshots CharacterBody_%s_marker_*.png"),
							WR->Exposed, WR->bVisible ? 1 : 0, WR->Covered, *Label));
				}));
			ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.3f));
		}

		// --- gaits: walk, jog, sprint (same input as the keyboard)
		// steady gaits (sampled after 1.2 s) and two transitions sampled from the first frame: start + stop, and a 90 degree turn while jogging.
		// NightBuild06 (Phase D, measurement only): gait changes walk -> jog and run -> sprint, braking out of a run, jogging backwards (S)
		// and sideways (D). The character turns to its movement direction, so S / D measure the turn plus the jog in the new direction.
		struct FGait
		{
			const TCHAR* Name; ESpnGait Gait; bool bSprint; float Seconds; bool bWhole; float TurnAt;
			ESpnGait SwitchTo = ESpnGait::Walk; float SwitchAt = 0.f; bool bSprintFromSwitch = false; FKey Key = EKeys::W;
		};
		for (const FGait& G : {FGait{TEXT("walk"), ESpnGait::Walk, false, 4.f, false, 0.f}, FGait{TEXT("jog"), ESpnGait::Jog, false, 4.f, false, 0.f},
			FGait{TEXT("run"), ESpnGait::Run, false, 4.f, false, 0.f}, FGait{TEXT("sprint"), ESpnGait::Jog, true, 3.5f, false, 0.f},
			FGait{TEXT("start_stop_walk"), ESpnGait::Walk, false, 2.f, true, 0.f}, FGait{TEXT("turn90_jog"), ESpnGait::Jog, false, 3.5f, true, 1.5f},
			FGait{TEXT("walk_to_jog"), ESpnGait::Walk, false, 3.5f, true, 0.f, ESpnGait::Jog, 1.5f},
			FGait{TEXT("run_to_sprint"), ESpnGait::Run, false, 3.5f, true, 0.f, ESpnGait::Run, 1.5f, true},
			FGait{TEXT("brake_from_run"), ESpnGait::Run, false, 2.5f, true, 0.f},
			FGait{TEXT("backward_jog"), ESpnGait::Jog, false, 4.f, false, 0.f, ESpnGait::Walk, 0.f, false, EKeys::S},
			FGait{TEXT("sideways_jog"), ESpnGait::Jog, false, 4.f, false, 0.f, ESpnGait::Walk, 0.f, false, EKeys::D}})
		{
			const TSharedRef<FFootRun> Run = MakeShared<FFootRun>();
			ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(*this, FVector(LaneX, LaneStartY, StandZ), 90.f, G.Seconds + 3.f,
				[G, Label](ASupernaturalProtCharacter& Character)
				{
					SpnQa::Section(FString::Printf(TEXT("Character body %s: %s"), *Label, G.Name));
					Character.GetSpnMovement()->SetBaseGait(G.Gait);
				},
				[Run, G](ASupernaturalProtCharacter& Character, float, double Elapsed)
				{
					const bool bSwitched = G.SwitchAt > 0.f && Elapsed >= G.SwitchAt;
					if (bSwitched && Elapsed - Character.GetWorld()->GetDeltaSeconds() < G.SwitchAt && G.SwitchTo != G.Gait)
					{
						Character.GetSpnMovement()->SetBaseGait(G.SwitchTo);
					}
					Run->Keys.Set(Character, G.Key, Elapsed < G.Seconds);
					Run->Keys.Set(Character, EKeys::LeftShift, (G.bSprint || (bSwitched && G.bSprintFromSwitch)) && Elapsed < G.Seconds);
					if (G.TurnAt > 0.f && Elapsed >= G.TurnAt && Elapsed - Character.GetWorld()->GetDeltaSeconds() < G.TurnAt)
					{
						Character.GetController()->SetControlRotation(FRotator(0.f, 0.f, 0.f));   // input direction turns from +Y to +X
					}
					if (G.bWhole ? Elapsed > 0.1 && Elapsed < G.Seconds + 0.8 : Elapsed > 1.2 && Elapsed < G.Seconds)
					{
						SampleFeet(Character, *Run, Elapsed);
						Run->SpeedSum += HorizontalSpeed(Character);
						++Run->SpeedN;
					}
					return Elapsed > G.Seconds + 0.8;
				},
				[this, Run, G, Label, Speeds, Preset](ASupernaturalProtCharacter& Character, bool)
				{
					Run->Keys.ReleaseAll(Character);
					SpnQa::Trace(TEXT("CharacterBody"), Label + TEXT("_") + G.Name, {TEXT("t"), TEXT("ball_l_z"), TEXT("ball_r_z"), TEXT("speed"), TEXT("ball_l_y"), TEXT("ball_r_y")}, Run->Trace);
					const double Speed = Run->SpeedN > 0 ? Run->SpeedSum / Run->SpeedN : 0.0;
					Speeds->Add(Label + G.Name, Speed);
					double P50 = 0, P90 = 0, Low = 0;
					const FString Feet = FootStats(*Run, P50, P90, Low);
					FString Why;
					if (Run->SpeedN < 30) { Why = TEXT("too few samples"); }
					else if (!Preset.IsNone() && !G.bWhole)
					{
						// gameplay must not change with the body: the same input gives the same speed as the placeholder
						const double* Ref = Speeds->Find(PresetLabel(NAME_None) + G.Name);
						if (Ref != nullptr && FMath::Abs(Speed - *Ref) > FMath::Max(0.03 * *Ref, 5.0))
						{
							Why = FString::Printf(TEXT("speed %.0f differs from the placeholder's %.0f cm/s"), Speed, *Ref);
						}
					}
					SpnQa::Check(*this, &Character, TEXT("CharacterBody"), Label + TEXT("_") + G.Name, Why.IsEmpty() ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail,
						(Why.IsEmpty() ? FString() : Why + TEXT(". ")) + FString::Printf(TEXT("mean speed %.0f cm/s; %s; retarget node %.3f ms (mean)"), Speed, *Feet,
							Run->RetargetN > 0 ? Run->RetargetMsSum / Run->RetargetN : 0.0));
				}));
			ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.3f));
		}

		// --- standing jump
		{
			const TSharedRef<FFootRun> Run = MakeShared<FFootRun>();
			ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(*this, FVector(LaneX, LaneStartY, StandZ), 90.f, 5.f,
				[Label](ASupernaturalProtCharacter&) { SpnQa::Section(FString::Printf(TEXT("Character body %s: jump"), *Label)); },
				[Run](ASupernaturalProtCharacter& Character, float, double Elapsed)
				{
					Run->Keys.Set(Character, EKeys::SpaceBar, Elapsed > 0.5 && Elapsed < 0.65);
					if (Elapsed > 0.4) { SampleFeet(Character, *Run, Elapsed); }
					const bool bGround = Character.GetSpnMovement()->IsMovingOnGround();
					if (!bGround) { Run->bLeftGround = true; }
					if (Run->bLeftGround && bGround && !Run->bLanded) { Run->bLanded = true; }
					return Run->bLanded && Elapsed > 2.5;
				},
				[this, Run, Label](ASupernaturalProtCharacter& Character, bool bTimedOut)
				{
					Run->Keys.ReleaseAll(Character);
					SpnQa::Trace(TEXT("CharacterBody"), Label + TEXT("_jump"), {TEXT("t"), TEXT("ball_l_z"), TEXT("ball_r_z"), TEXT("speed"), TEXT("ball_l_y"), TEXT("ball_r_y")}, Run->Trace);
					const bool bOk = Run->bLeftGround && Run->bLanded && !bTimedOut;
					SpnQa::Check(*this, &Character, TEXT("CharacterBody"), Label + TEXT("_jump"), bOk ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail,
						FString::Printf(TEXT("%sleft the ground %d, landed %d, highest point of the lower foot %.0f cm"), bOk ? TEXT("") : TEXT("the jump did not complete. "),
							Run->bLeftGround, Run->bLanded, Run->MaxFootZ));
				}));
			ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.3f));
		}

		// --- ledge assist: 90 cm mantle from standing, 35 cm step-on while jogging; a shot in the middle of the mantle
		struct FLedge { const TCHAR* Name; float Height; float Gap; bool bStanding; };
		for (const FLedge& L : {FLedge{TEXT("mantle_90"), 90.f, 5.f, true}, FLedge{TEXT("step_on_35"), 35.f, 10.f, false}})
		{
			const TSharedRef<FFootRun> Run = MakeShared<FFootRun>();
			const TSharedRef<double> PressAt = MakeShared<double>(-1.0);
			const TSharedRef<int32> CountBefore = MakeShared<int32>(0);
			const TSharedRef<TObjectPtr<AStaticMeshActor>> Box = MakeShared<TObjectPtr<AStaticMeshActor>>();
			const float FrontY = LaneStartY + 1200.f;
			const FVector Start = L.bStanding ? FVector(LaneX, FrontY - 42.f - L.Gap, StandZ) : FVector(LaneX, FrontY - 700.f, StandZ);
			ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(*this, Start, 90.f, 8.f,
				[L, Label, Box, CountBefore, FrontY](ASupernaturalProtCharacter& Character)
				{
					SpnQa::Section(FString::Printf(TEXT("Character body %s: %s"), *Label, L.Name));
					Character.GetSpnMovement()->SetBaseGait(ESpnGait::Jog);
					*Box = SpawnBox(*Character.GetWorld(), FVector(LaneX, FrontY + 50.f, L.Height / 2.f), FVector(600.f, 100.f, L.Height));
					*CountBefore = Character.GetSpnMovement()->GetLedgeMoveCount();
				},
				[Run, L, PressAt, Label, FrontY](ASupernaturalProtCharacter& Character, float, double Elapsed)
				{
					const float Gap = FrontY - (Character.GetActorLocation().Y + 42.f);
					if (*PressAt < 0.0)
					{
						Run->Keys.Set(Character, EKeys::W, !L.bStanding);
						if ((L.bStanding ? Elapsed > 0.6 : Gap <= L.Gap && Elapsed > 1.0) && Character.GetSpnMovement()->IsMovingOnGround())
						{
							*PressAt = Elapsed;
							Run->Keys.Set(Character, EKeys::SpaceBar, true);
						}
						return Elapsed > 6.0;
					}
					const double Since = Elapsed - *PressAt;
					if (Since > 0.1) { Run->Keys.Set(Character, EKeys::SpaceBar, false); }
					if (Since > 0.6) { Run->Keys.Set(Character, EKeys::W, false); }
					SampleFeet(Character, *Run, Elapsed);
					if (Since < 1.2)
					{
						USkeletalMeshComponent* Shown = ShownParts(Character)[0];
						for (const TCHAR* Hand : {TEXT("hand_l"), TEXT("hand_r")})
						{
							const FVector P = Shown->GetSocketLocation(Hand);
							const float Dz = FMath::Abs(static_cast<float>(P.Z) - L.Height);
							if (Dz < Run->HandBestDz) { Run->HandBestDz = Dz; Run->HandAtFace = static_cast<float>(P.Y) - FrontY; }
						}
					}
					if (Since > 0.25 && Since - Character.GetWorld()->GetDeltaSeconds() <= 0.25)
					{
						FScreenshotRequest::RequestScreenshot(SpnQa::QaDir() / TEXT("shots") / FString::Printf(TEXT("CharacterBody_%s_%s.png"), *Label, L.Name), false, false);
					}
					return Since > 2.8;
				},
				[this, Run, L, Label, Box, CountBefore, PressAt](ASupernaturalProtCharacter& Character, bool)
				{
					Run->Keys.ReleaseAll(Character);
					if (*Box != nullptr) { (*Box)->Destroy(); }
					const FSpnLedgeRecord& Record = Character.GetSpnMovement()->GetLastLedgeRecord();
					const int32 Moves = Character.GetSpnMovement()->GetLedgeMoveCount() - *CountBefore;
					const float Bottom = Character.GetActorLocation().Z - Character.GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
					const bool bOk = *PressAt >= 0.0 && Moves == 1 && Record.bCompleted && FMath::Abs(Bottom - L.Height) < 8.f;
					SpnQa::Check(*this, &Character, TEXT("CharacterBody"), Label + TEXT("_") + L.Name, bOk ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail,
						FString::Printf(TEXT("%smove %s, completed %d in %.2f s, feet on %.0f cm (top %.0f); closest hand to the top surface %.1f cm, %.1f cm past the face then (no hand IK to the ledge yet: measured only)"),
							bOk ? TEXT("") : TEXT("the ledge move did not end on top. "),
							SpnLedgeAssist::MoveName(Moves > 0 ? Record.Move : ESpnLedgeMove::None), Record.bCompleted, Record.ActualSeconds, Bottom, L.Height, Run->HandBestDz, Run->HandAtFace));
				}));
			ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.3f));
		}
	}

	// leave the presentation as the run started it (-SpnBody=... or none)
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([Initial]
	{
		if (ASupernaturalProtCharacter* Character = FindCharacter())
		{
			if (USpnCharacterVisualComponent* Visual = VisualOf(*Character)) { Visual->ApplyPreset(*Initial); }
		}
		return true;
	}));
	return true;
}

#endif
