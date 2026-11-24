#include "Components/CapsuleComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/LightComponent.h"
#include "Components/LocalFogVolumeComponent.h"
#include "Dom/JsonObject.h"
#include "Engine/DirectionalLight.h"
#include "Engine/PostProcessVolume.h"
#include "EngineUtils.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HAL/IConsoleManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Spn/SpnInteractionComponent.h"
#include "Spn/SpnTestDoor.h"
#include "Spn/Tests/SpnMovementTestSupport.h"
#include "Spn/Tests/SpnQaRecorder.h"
#include "Spn/Tests/SpnTestPlayReset.h"
#include "Spn/World/SpnWorldLighting.h"
#include "Spn/SpnSettingsSubsystem.h"
#include "Spn/UI/SpnMenuScreen.h"
#include "Spn/UI/SpnUiSubsystem.h"
#include "Engine/LocalPlayer.h"

#if WITH_DEV_AUTOMATION_TESTS

// SpnWorldQA.Integration (suite "SpnWorld"; Tools/run_gameplay_qa.ps1 -Suite World): gameplay integration in the generated placeholder world /Game/SpnWorld/Lvl_SpnWorld (not a QA test map).
// The character is played with real key input through the player controller (W, LeftShift, E), like SpnQA.BuildingAccess:
// spawn, walking and sprinting on asphalt, on the forest track and across a slope, running into a tree, a rock and the cabin wall,
// opening the cabin door with E, entering and leaving the interior (own exposure volume), and the four lighting scenarios switched with the
// console command Spn.Lighting. At every stop the camera and the feet are checked for clipping. The test points come from the world
// generator (Saved/SpnWorldGen/qa_points.json, Tools/spn_world/qa_points.py), so they follow the generated world.
// Checks report what they measured; a missing point or a missing function is reported as BLOCKED / NOT_TESTED / FAIL, never as PASS.

using namespace SpnMovementTest;

namespace
{
	constexpr EAutomationTestFlags WorldFlags = EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter;
	const TCHAR* const Suite = TEXT("SpnWorld");
	const TCHAR* const WorldMap = TEXT("/Game/SpnWorld/Lvl_SpnWorld");

	enum class EOp : uint8
	{
		Section, Teleport, Wait, Gait, MoveFor, WalkTo, PushInto, CheckSpawn, CheckSpeed, CheckReached, CheckStopped, CheckClipping,
		FaceDoor, PressE, CheckDoorOpen, CheckInside, CheckExposure, CheckOutside, Scenario, CheckScenario, Shot, Console, CheckScreenPercentage, CheckMenuQuality
	};

	struct FOp
	{
		EOp Kind;
		FVector P = FVector::ZeroVector;   // target / teleport point
		FVector Q = FVector::ZeroVector;   // second point (e.g. the obstacle centre)
		float A = 0.f;                     // seconds, tolerance or radius
		float B = 0.f;
		FString Id;
	};

	struct FPoints
	{
		bool bLoaded = false;
		FString Error;
		TSharedPtr<FJsonObject> Root;

		bool Vec(const TSharedPtr<FJsonObject>& Obj, const TCHAR* Field, FVector& Out) const
		{
			const TArray<TSharedPtr<FJsonValue>>* A = nullptr;
			if (!Obj.IsValid() || !Obj->TryGetArrayField(Field, A) || A->Num() != 3) { return false; }
			Out = FVector((*A)[0]->AsNumber(), (*A)[1]->AsNumber(), (*A)[2]->AsNumber());
			return true;
		}
		bool Pair(const TCHAR* Field, FVector& From, FVector& To) const
		{
			const TArray<TSharedPtr<FJsonValue>>* A = nullptr;
			if (!Root.IsValid() || !Root->TryGetArrayField(Field, A) || A->Num() != 2) { return false; }
			const auto Get = [](const TSharedPtr<FJsonValue>& V, FVector& Out)
			{
				const TArray<TSharedPtr<FJsonValue>>& C = V->AsArray();
				if (C.Num() != 3) { return false; }
				Out = FVector(C[0]->AsNumber(), C[1]->AsNumber(), C[2]->AsNumber());
				return true;
			};
			return Get((*A)[0], From) && Get((*A)[1], To);
		}
		TSharedPtr<FJsonObject> Obj(const TCHAR* Field) const
		{
			const TSharedPtr<FJsonObject>* O = nullptr;
			return Root.IsValid() && Root->TryGetObjectField(Field, O) ? *O : nullptr;
		}
	};

	FPoints LoadPoints()
	{
		FPoints P;
		const FString Path = FPaths::ProjectSavedDir() / TEXT("SpnWorldGen") / TEXT("qa_points.json");
		FString Text;
		if (!FFileHelper::LoadFileToString(Text, *Path))
		{
			P.Error = FString::Printf(TEXT("%s is missing: build the world with Tools/spn_world_build.ps1 -Phase H"), *Path);
			return P;
		}
		if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Text), P.Root) || !P.Root.IsValid())
		{
			P.Error = FString::Printf(TEXT("%s is not valid JSON"), *Path);
			return P;
		}
		P.bLoaded = true;
		return P;
	}

	/** Builds the step list; parts whose points are missing become a NOT_TESTED note instead of silently disappearing. */
	TArray<FOp> BuildOps(const FPoints& Pts, TArray<FString>& Missing)
	{
		TArray<FOp> Ops;
		const auto Add = [&Ops](EOp Kind, const FVector& P = FVector::ZeroVector, const FVector& Q = FVector::ZeroVector, float A = 0.f, float B = 0.f, const FString& Id = FString())
		{
			Ops.Add({Kind, P, Q, A, B, Id});
		};
		FVector From, To;

		// render resolution (decision 2026-11-13): SpnWorld runs at the provisional standard 90 % with TSR, the quality mode 100 % is one command away
		Add(EOp::Section, {}, {}, 0, 0, TEXT("SpnWorld: render resolution (TSR)"));
		Add(EOp::CheckScreenPercentage, {}, {}, 90.f, 0, TEXT("screen_percentage_standard_90"));
		Add(EOp::Console, {}, {}, 0, 0, TEXT("Spn.Quality quality"));
		Add(EOp::CheckScreenPercentage, {}, {}, 100.f, 0, TEXT("screen_percentage_quality_100"));
		Add(EOp::Console, {}, {}, 0, 0, TEXT("Spn.Quality standard"));
		Add(EOp::CheckScreenPercentage, {}, {}, 90.f, 0, TEXT("screen_percentage_back_to_90"));
		Add(EOp::CheckMenuQuality, {}, {}, 0, 0, TEXT("graphics_menu_world_quality"));
		Add(EOp::Wait, {}, {}, 0.5f);
		Add(EOp::CheckScreenPercentage, {}, {}, 90.f, 0, TEXT("screen_percentage_after_the_menu"));

		Add(EOp::Section, {}, {}, 0, 0, TEXT("SpnWorld: spawn and asphalt"));
		if (Pts.Pair(TEXT("street"), From, To))
		{
			Add(EOp::Teleport, From, To);
			Add(EOp::Wait, {}, {}, 1.5f);
			Add(EOp::CheckSpawn, {}, {}, 0, 0, TEXT("spawn_on_ground"));
			Add(EOp::CheckClipping, {}, {}, 0, 0, TEXT("street_start"));
			Add(EOp::Gait, {}, {}, 0.f);                                  // 0 = walk
			Add(EOp::MoveFor, To, {}, 5.f, 0.f, TEXT("street_walk"));     // B = 0: no sprint
			Add(EOp::CheckSpeed, {}, {}, 120.f, 260.f, TEXT("street_walk"));
			Add(EOp::Gait, {}, {}, 1.f);                                  // 1 = jog base gait, sprint with LeftShift
			Add(EOp::MoveFor, To, {}, 4.f, 1.f, TEXT("street_sprint"));
			Add(EOp::CheckSpeed, {}, {}, 600.f, 1000.f, TEXT("street_sprint"));
			Add(EOp::CheckClipping, {}, {}, 0, 0, TEXT("street_end"));
		}
		else { Missing.Add(TEXT("street")); }

		Add(EOp::Section, {}, {}, 0, 0, TEXT("SpnWorld: forest track and slope"));
		if (Pts.Pair(TEXT("forest_track"), From, To))
		{
			Add(EOp::Teleport, From, To);
			Add(EOp::Wait, {}, {}, 1.2f);
			Add(EOp::Gait, {}, {}, 0.f);
			Add(EOp::MoveFor, To, {}, 5.f, 0.f, TEXT("track_walk"));
			Add(EOp::CheckSpeed, {}, {}, 120.f, 260.f, TEXT("track_walk"));
			Add(EOp::Gait, {}, {}, 1.f);
			Add(EOp::MoveFor, To, {}, 4.f, 1.f, TEXT("track_sprint"));
			Add(EOp::CheckSpeed, {}, {}, 600.f, 1000.f, TEXT("track_sprint"));
			Add(EOp::CheckClipping, {}, {}, 0, 0, TEXT("track"));
		}
		else { Missing.Add(TEXT("forest_track")); }
		FVector V;
		if (Pts.Vec(Pts.Obj(TEXT("uneven")), TEXT("from"), From) && Pts.Vec(Pts.Obj(TEXT("uneven")), TEXT("to"), To))
		{
			Add(EOp::Teleport, From, To);
			Add(EOp::Wait, {}, {}, 1.0f);
			Add(EOp::Gait, {}, {}, 1.f);
			Add(EOp::WalkTo, To, {}, 80.f, 25.f, TEXT("slope"));
			Add(EOp::CheckReached, To, {}, 80.f, 0.f, TEXT("slope_crossed"));
			Add(EOp::CheckClipping, {}, {}, 0, 0, TEXT("slope"));
		}
		else { Missing.Add(TEXT("uneven")); }

		Add(EOp::Section, {}, {}, 0, 0, TEXT("SpnWorld: collisions"));
		if (Pts.Vec(Pts.Obj(TEXT("tree")), TEXT("from"), From) && Pts.Vec(Pts.Obj(TEXT("tree")), TEXT("centre"), V))
		{
			const float Radius = static_cast<float>(Pts.Obj(TEXT("tree"))->GetNumberField(TEXT("radius")));
			Add(EOp::Teleport, From, V);
			Add(EOp::Wait, {}, {}, 1.0f);
			Add(EOp::Gait, {}, {}, 1.f);
			Add(EOp::PushInto, V, {}, 6.f, 0.f, TEXT("tree"));
			Add(EOp::CheckStopped, V, From, Radius, 0.f, TEXT("tree_blocks"));
		}
		else { Missing.Add(TEXT("tree")); }
		if (Pts.Vec(Pts.Obj(TEXT("rock")), TEXT("from"), From) && Pts.Vec(Pts.Obj(TEXT("rock")), TEXT("centre"), V))
		{
			const float KeepOut = static_cast<float>(Pts.Obj(TEXT("rock"))->GetNumberField(TEXT("keep_out")));
			Add(EOp::Teleport, From, V);
			Add(EOp::Wait, {}, {}, 1.0f);
			Add(EOp::PushInto, V, {}, 6.f, 0.f, TEXT("rock"));
			Add(EOp::CheckStopped, V, From, KeepOut, 1.f, TEXT("rock_blocks"));     // B = 1: standing on top of it also counts as blocked
		}
		else { Missing.Add(TEXT("rock")); }

		const TSharedPtr<FJsonObject> Cabin = Pts.Obj(TEXT("cabin"));
		FVector WallFrom, WallTarget, WallPlane, InFront, Doorway, Inside, Outside, RoomA, RoomB;
		if (Pts.Vec(Cabin, TEXT("wall_from"), WallFrom) && Pts.Vec(Cabin, TEXT("wall_target"), WallTarget) && Pts.Vec(Cabin, TEXT("wall_plane"), WallPlane))
		{
			Add(EOp::Teleport, WallFrom, WallTarget);
			Add(EOp::Wait, {}, {}, 1.0f);
			Add(EOp::PushInto, WallTarget, {}, 6.f, 0.f, TEXT("cabin_wall"));
			Add(EOp::CheckStopped, WallPlane, WallFrom, 0.f, 2.f, TEXT("cabin_wall_blocks"));   // B = 2: a wall plane, not a round obstacle
		}
		else { Missing.Add(TEXT("cabin wall")); }

		Add(EOp::Section, {}, {}, 0, 0, TEXT("SpnWorld: cabin door and interior"));
		if (Pts.Vec(Cabin, TEXT("in_front"), InFront) && Pts.Vec(Cabin, TEXT("doorway"), Doorway) && Pts.Vec(Cabin, TEXT("inside"), Inside) && Pts.Vec(Cabin, TEXT("outside"), Outside)
			&& Pts.Vec(Cabin, TEXT("room_a"), RoomA) && Pts.Vec(Cabin, TEXT("room_b"), RoomB))
		{
			// in front of the table on the door axis (the leaf opens inwards along the left jamb); older point files lack it: derive it
			FVector Centre;
			if (!Pts.Vec(Cabin, TEXT("centre"), Centre)) { Centre = Doorway + (Doorway - InFront).GetSafeNormal2D() * 190.f; }
			Inside = Centre;
			Add(EOp::Teleport, Outside, Doorway);
			Add(EOp::Wait, {}, {}, 1.0f);
			Add(EOp::Gait, {}, {}, 0.f);
			Add(EOp::WalkTo, InFront, {}, 30.f, 15.f, TEXT("to_the_door"));
			Add(EOp::Wait, {}, {}, 0.8f);
			Add(EOp::FaceDoor);
			Add(EOp::Shot, {}, {}, 0, 0, TEXT("door_closed"));
			Add(EOp::PressE);
			Add(EOp::Wait, {}, {}, 2.5f);
			Add(EOp::CheckDoorOpen, {}, {}, 0, 0, TEXT("cabin_door_opens_with_e"));
			Add(EOp::WalkTo, Doorway, {}, 35.f, 12.f, TEXT("doorway"));
			Add(EOp::WalkTo, Inside, {}, 35.f, 12.f, TEXT("inside"));
			Add(EOp::CheckInside, {}, {}, 0, 0, TEXT("inside_behind_the_door"));
			// deeper into the room (round the table): only there the third-person camera is inside the room as well
			Add(EOp::WalkTo, RoomA, {}, 35.f, 12.f, TEXT("room_a"));
			Add(EOp::WalkTo, RoomB, {}, 35.f, 12.f, TEXT("room_b"));
			Add(EOp::Wait, {}, {}, 2.5f);                                  // the eye adapts to the room
			Add(EOp::CheckInside, {}, {}, 0, 0, TEXT("inside_the_room"));
			Add(EOp::CheckExposure, {}, {}, 0, 0, TEXT("interior_exposure"));
			Add(EOp::CheckClipping, {}, {}, 0, 0, TEXT("interior"));
			Add(EOp::Shot, {}, {}, 0, 0, TEXT("interior"));
			Add(EOp::WalkTo, RoomA, {}, 35.f, 12.f, TEXT("back_room_a"));
			Add(EOp::WalkTo, Inside, {}, 35.f, 12.f, TEXT("back_inside"));
			Add(EOp::WalkTo, Doorway, {}, 35.f, 12.f, TEXT("back_to_the_doorway"));
			Add(EOp::WalkTo, Outside, {}, 40.f, 15.f, TEXT("out"));
			Add(EOp::CheckOutside, {}, {}, 0, 0, TEXT("left_the_cabin"));
		}
		else { Missing.Add(TEXT("cabin door")); }

		Add(EOp::Section, {}, {}, 0, 0, TEXT("SpnWorld: lighting scenarios"));
		for (const TCHAR* Name : {TEXT("day"), TEXT("overcast"), TEXT("night"), TEXT("spn_overcast")})
		{
			Add(EOp::Scenario, {}, {}, 0, 0, Name);
			Add(EOp::Wait, {}, {}, 2.0f);
			Add(EOp::CheckScenario, {}, {}, 0, 0, Name);
			Add(EOp::Shot, {}, {}, 0, 0, FString(TEXT("scenario_")) + Name);
		}
		Add(EOp::Scenario, {}, {}, 0, 0, TEXT("day"));
		return Ops;
	}

	struct FWorldRun
	{
		FPoints Points;
		TArray<FOp> Ops;
		TArray<FString> Missing;
		SpnQa::FHeldKeys Keys;
		ASpnTestDoor* Door = nullptr;
		ASpnWorldLighting* Lighting = nullptr;
		APostProcessVolume* Interior = nullptr;
		int32 Index = -1;
		int32 Frames = 0;
		double StepStart = 0.0;
		// speed window of the last MoveFor
		double MoveDist = 0.0;
		double MoveTime = 0.0;
		FVector LastPos = FVector::ZeroVector;
		bool bAirborne = false;
		float PushSpeed = 0.f;
		FString Failure;
	};

	FVector LeafCenter(const ASpnTestDoor& Door) { return Door.GetComponentsBoundingBox(/*bNonColliding*/ false).GetCenter(); }

	void Face(ASupernaturalProtCharacter& Character, const FVector& Target, float Pitch)
	{
		const FVector From = Character.GetActorLocation();
		Character.GetController()->SetControlRotation(FRotator(Pitch, FMath::RadiansToDegrees(FMath::Atan2(Target.Y - From.Y, Target.X - From.X)), 0.f));
	}

	/** Height of the first surface below At.Z + Above (a downward trace that ignores the character). Above = 3000 finds the terrain under a
	 *  generator point from high up (outdoors only); Above = small finds the floor under the feet or the camera, also under a roof. */
	bool GroundZ(const ASupernaturalProtCharacter& Character, const FVector& At, float& OutZ, float Above = 3000.f)
	{
		FHitResult Hit;
		FCollisionQueryParams Params(SCENE_QUERY_STAT(SpnQaWorldGround), false, &Character);
		if (Character.GetWorld()->LineTraceSingleByChannel(Hit, FVector(At.X, At.Y, At.Z + Above), FVector(At.X, At.Y, At.Z - 3000.f), ECC_Visibility, Params))
		{
			OutZ = Hit.ImpactPoint.Z;
			return true;
		}
		return false;
	}
}

// Not under "SpnQA.": the full QA suite runs on the small QA map and must stay comparable; this one has its own suite (-Suite World).
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnQaSpnWorldTest, "SpnWorldQA.Integration", WorldFlags)

bool FSpnQaSpnWorldTest::RunTest(const FString& Parameters)
{
	if (!FPackageName::DoesPackageExist(WorldMap))
	{
		AddError(FString::Printf(TEXT("%s does not exist: build it with Tools/spn_world_build.ps1 -Phase H"), WorldMap));
		return false;
	}
	const TSharedRef<FWorldRun> Run = MakeShared<FWorldRun>();
	Run->Points = LoadPoints();
	if (Run->Points.bLoaded)
	{
		Run->Ops = BuildOps(Run->Points, Run->Missing);
	}
	FVector Start(0.f, 0.f, 500.f);
	if (Run->Ops.Num() > 1 && Run->Ops[1].Kind == EOp::Teleport) { Start = Run->Ops[1].P + FVector(0.f, 0.f, 120.f); }

	AutomationOpenMap(WorldMap);
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(3.f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([]
	{
		if (const UWorld* World = FindPlayWorld()) { if (APlayerController* PC = World->GetFirstPlayerController()) { SpnTestPlay::ResetWorldState(*PC); } }
		return true;
	}));

	ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(*this, Start, 0.f, 420.f,
		[Run](ASupernaturalProtCharacter& Character)
		{
			UWorld* World = Character.GetWorld();
			for (TActorIterator<ASpnTestDoor> It(World); It; ++It) { if (It->StableId == TEXT("cabin_door")) { Run->Door = *It; } }
			Run->Lighting = ASpnWorldLighting::Find(World);
			// the cabin's room volume: the one nearest to the cabin door (since 2026-11-13 the houses of the gameplay route have room volumes too)
			double Best = TNumericLimits<double>::Max();
			for (TActorIterator<APostProcessVolume> It(World); It; ++It)
			{
				if (!It->ActorHasTag(ASpnWorldLighting::InteriorPostTag)) { continue; }
				const double D = Run->Door != nullptr ? FVector::Dist(It->GetActorLocation(), Run->Door->GetActorLocation()) : 0.0;
				if (D < Best) { Best = D; Run->Interior = *It; }
			}
		},
		[this, Run](ASupernaturalProtCharacter& Character, float DeltaSeconds, double Elapsed)
		{
			FWorldRun& R = *Run;
			if (!R.Points.bLoaded) { R.Failure = R.Points.Error; return true; }
			if (R.Index < 0)
			{
				R.Index = 0; R.StepStart = Elapsed; R.Frames = 0;
				for (const FString& M : R.Missing)
				{
					SpnQa::Check(*this, &Character, Suite, TEXT("points_") + M.Replace(TEXT(" "), TEXT("_")), SpnQa::EStatus::NotTested,
						FString::Printf(TEXT("qa_points.json has no '%s' entry (the generator found no suitable place); this part did not run"), *M));
				}
			}
			if (!R.Ops.IsValidIndex(R.Index)) { return true; }
			APlayerController* PC = Cast<APlayerController>(Character.GetController());
			UCharacterMovementComponent* Move = Character.GetCharacterMovement();
			const FOp& Op = R.Ops[R.Index];
			const double InStep = Elapsed - R.StepStart;
			const FVector Loc = Character.GetActorLocation();
			const float HalfHeight = Character.GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
			const float CapsuleRadius = Character.GetCapsuleComponent()->GetScaledCapsuleRadius();
			const auto Next = [&]() { ++R.Index; R.StepStart = Elapsed; R.Frames = 0; };
			const auto Check = [&](const FString& Id, SpnQa::EStatus Status, const FString& Detail) { SpnQa::Check(*this, &Character, Suite, Id, Status, Detail); };
			const auto Pass = [](bool bOk) { return bOk ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail; };
			const bool bMoving = Op.Kind == EOp::MoveFor || Op.Kind == EOp::WalkTo || Op.Kind == EOp::PushInto;
			if (!bMoving) { R.Keys.Set(Character, EKeys::W, false); R.Keys.Set(Character, EKeys::LeftShift, false); }

			switch (Op.Kind)
			{
			case EOp::Section:
				SpnQa::Section(Op.Id);
				Next();
				break;
			case EOp::Teleport:
			{
				float Z = Op.P.Z;
				GroundZ(Character, Op.P, Z);
				Move->StopMovementImmediately();
				Character.SetActorLocation(FVector(Op.P.X, Op.P.Y, Z + HalfHeight + 10.f), false, nullptr, ETeleportType::TeleportPhysics);
				Face(Character, Op.Q, -8.f);
				Character.SetActorRotation(FRotator(0.f, PC->GetControlRotation().Yaw, 0.f));
				Next();
				break;
			}
			case EOp::Wait:
				if (InStep >= Op.A) { Next(); }
				break;
			case EOp::Gait:
				Character.GetSpnMovement()->SetBaseGait(Op.A > 0.5f ? ESpnGait::Jog : ESpnGait::Walk);
				Next();
				break;
			case EOp::MoveFor:     // hold W (and LeftShift if B > 0) towards P for A seconds; the speed counts after 1 s of acceleration
			{
				if (R.Frames++ == 0) { R.MoveDist = 0.0; R.MoveTime = 0.0; R.LastPos = Loc; R.bAirborne = false; }
				Face(Character, Op.P, -8.f);
				R.Keys.Set(Character, EKeys::W, true);
				R.Keys.Set(Character, EKeys::LeftShift, Op.B > 0.5f);
				if (InStep > 1.0) { R.MoveDist += FVector::Dist2D(Loc, R.LastPos); R.MoveTime += DeltaSeconds; }
				if (!Move->IsMovingOnGround() && InStep > 0.3) { R.bAirborne = true; }
				R.LastPos = Loc;
				if (InStep >= Op.A || FVector::Dist2D(Loc, Op.P) < 150.f) { Next(); }
				break;
			}
			case EOp::CheckSpeed:
			{
				const double Speed = R.MoveTime > 0.0 ? R.MoveDist / R.MoveTime : 0.0;
				Check(Op.Id + TEXT("_speed"), Pass(Speed >= Op.A && Speed <= Op.B && !R.bAirborne),
					FString::Printf(TEXT("average %.0f cm/s over %.1f s (expected %.0f-%.0f), left the ground %d, at (%.0f, %.0f, %.0f)"),
						Speed, R.MoveTime, Op.A, Op.B, R.bAirborne, Loc.X, Loc.Y, Loc.Z));
				Next();
				break;
			}
			case EOp::WalkTo:      // A = stop distance, B = time limit
				if (FVector::Dist2D(Loc, Op.P) <= Op.A) { Next(); break; }
				if (InStep > Op.B)
				{
					Check(TEXT("walk_") + Op.Id, SpnQa::EStatus::Blocked, FString::Printf(TEXT("did not reach the waypoint within %.0f s: %.0f cm left, stuck at (%.0f, %.0f, %.0f)"),
						Op.B, FVector::Dist2D(Loc, Op.P), Loc.X, Loc.Y, Loc.Z));
					Next();
					break;
				}
				Face(Character, Op.P, -8.f);
				R.Keys.Set(Character, EKeys::W, true);
				break;
			case EOp::PushInto:    // keep walking into the obstacle for A seconds; the speed of the last frame shows whether it still moved
				Face(Character, Op.P, -8.f);
				R.Keys.Set(Character, EKeys::W, true);
				R.PushSpeed = HorizontalSpeed(Character);
				if (InStep >= Op.A) { Next(); }
				break;
			case EOp::CheckSpawn:
			{
				float Z = 0.f;
				const bool bGround = GroundZ(Character, Loc, Z, 0.f);
				const float Feet = Loc.Z - HalfHeight;
				Check(Op.Id, Pass(PC != nullptr && Move->IsMovingOnGround() && bGround && FMath::Abs(Feet - Z) < 15.f),
					FString::Printf(TEXT("possessed %d, on ground %d, feet %.0f, ground %.0f"), PC != nullptr, Move->IsMovingOnGround(), Feet, Z));
				Next();
				break;
			}
			case EOp::CheckReached:
			{
				const float D = FVector::Dist2D(Loc, Op.P);
				Check(Op.Id, Pass(D <= Op.A + 20.f && Move->IsMovingOnGround()),
					FString::Printf(TEXT("%.0f cm from the target on the slope (tolerance %.0f), on ground %d, height %.0f -> target %.0f"), D, Op.A, Move->IsMovingOnGround(), Loc.Z - HalfHeight, Op.P.Z));
				Next();
				break;
			}
			case EOp::CheckStopped:
			{
				// progress along the line from the approach point (Q) towards the obstacle (P)
				const FVector2D Dir = (FVector2D(Op.P) - FVector2D(Op.Q)).GetSafeNormal();
				const float Along = FVector2D::DotProduct(FVector2D(Loc) - FVector2D(Op.Q), Dir);
				const float Total = FVector2D::Distance(FVector2D(Op.P), FVector2D(Op.Q));
				const float Remaining = Total - Along;
				bool bOk = false;
				FString Rule;
				if (Op.B > 1.5f)       // wall plane: the capsule must stay in front of it
				{
					bOk = Remaining >= CapsuleRadius - 10.f;
					Rule = FString::Printf(TEXT("stays %.0f cm in front of the wall plane (capsule radius %.0f)"), Remaining, CapsuleRadius);
				}
				else if (Op.B > 0.5f)  // an irregular scanned rock: its surface is not known here; it blocks if the character stopped (W still held)
				{                      // outside its core, or stands on it
					const bool bOnTop = Loc.Z - HalfHeight > Op.P.Z + 25.f;
					const bool bStopped = R.PushSpeed < 30.f && Remaining >= CapsuleRadius + 10.f;
					bOk = bStopped || bOnTop;
					Rule = FString::Printf(TEXT("stopped %d (speed %.0f cm/s while pushing), centre distance %.0f cm (core: capsule %.0f + 10), on top %d"),
						R.PushSpeed < 30.f, R.PushSpeed, Remaining, CapsuleRadius, bOnTop);
				}
				else
				{
					const float Floor = Op.A + CapsuleRadius - 15.f;
					bOk = Remaining >= Floor;
					Rule = FString::Printf(TEXT("centre distance %.0f cm (needs >= %.0f: trunk %.0f + capsule %.0f - 15), speed %.0f cm/s while pushing"), Remaining, Floor, Op.A,
						CapsuleRadius, R.PushSpeed);
				}
				Check(Op.Id, Pass(bOk), FString::Printf(TEXT("after walking into it: %s, at (%.0f, %.0f, %.0f)"), *Rule, Loc.X, Loc.Y, Loc.Z));
				Next();
				break;
			}
			case EOp::CheckClipping:
			{
				FString Problems;
				const FVector Cam = PC->PlayerCameraManager != nullptr ? PC->PlayerCameraManager->GetCameraLocation() : FVector::ZeroVector;
				FCollisionQueryParams Params(SCENE_QUERY_STAT(SpnQaWorldClip), false, &Character);
				if (Character.GetWorld()->OverlapAnyTestByChannel(Cam, FQuat::Identity, ECC_Camera, FCollisionShape::MakeSphere(8.f), Params)) { Problems += TEXT(" camera inside geometry;"); }
				float CamGround = 0.f;
				if (GroundZ(Character, Cam, CamGround, 0.f) && Cam.Z < CamGround + 20.f) { Problems += FString::Printf(TEXT(" camera %.0f cm above the ground;"), Cam.Z - CamGround); }
				float Z = 0.f;
				const float Feet = Loc.Z - HalfHeight;
				if (GroundZ(Character, Loc, Z, 0.f) && Move->IsMovingOnGround() && FMath::Abs(Feet - Z) > 15.f) { Problems += FString::Printf(TEXT(" feet %.0f cm off the ground;"), Feet - Z); }
				Check(Op.Id + TEXT("_no_clipping"), Pass(Problems.IsEmpty()),
					FString::Printf(TEXT("camera (%.0f, %.0f, %.0f), ground below camera %.0f, feet %.0f, ground %.0f%s"), Cam.X, Cam.Y, Cam.Z, CamGround, Feet, Z,
						Problems.IsEmpty() ? TEXT("") : *(TEXT(":") + Problems)));
				Next();
				break;
			}
			case EOp::FaceDoor:
				if (R.Door == nullptr) { Check(TEXT("cabin_door_exists"), SpnQa::EStatus::Fail, TEXT("no ASpnTestDoor with stable id cabin_door in the level")); R.Index = R.Ops.Num(); break; }
				Face(Character, LeafCenter(*R.Door), 0.f);
				Next();
				break;
			case EOp::PressE:
				if (R.Door == nullptr) { Next(); break; }
				Face(Character, LeafCenter(*R.Door), 0.f);
				if (R.Frames == 0 && HorizontalSpeed(Character) > 15.f && InStep < 3.0) { break; }
				SpnQa::Key(*PC, EKeys::E, R.Frames == 0);
				if (++R.Frames > 1) { Next(); }
				break;
			case EOp::CheckDoorOpen:
			{
				if (R.Door == nullptr) { Next(); break; }
				const USpnInteractionComponent* Interaction = Character.FindComponentByClass<USpnInteractionComponent>();
				const bool bOk = R.Door->IsOpen() && FMath::IsNearlyEqual(R.Door->GetCurrentAngle(), R.Door->GetTargetAngle(), 1.f);
				Check(Op.Id, Pass(bOk), FString::Printf(TEXT("door open flag %d, leaf angle %.0f of %.0f, player %.0f cm from the leaf, prompt '%s'"), R.Door->IsOpen(),
					R.Door->GetCurrentAngle(), R.Door->GetTargetAngle(), FVector::Dist2D(Loc, LeafCenter(*R.Door)), Interaction ? *Interaction->GetCurrentPrompt() : TEXT("-")));
				Next();
				break;
			}
			case EOp::CheckInside:
			{
				const bool bInVolume = R.Interior != nullptr && R.Interior->EncompassesPoint(Loc, 0.f, nullptr);
				Check(Op.Id, Pass(bInVolume && Move->IsMovingOnGround()), FString::Printf(TEXT("interior volume %s, character inside it %d, on ground %d, at (%.0f, %.0f, %.0f)"),
					R.Interior ? *R.Interior->GetName() : TEXT("missing"), bInVolume, Move->IsMovingOnGround(), Loc.X, Loc.Y, Loc.Z));
				Next();
				break;
			}
			case EOp::CheckExposure:
			{
				// the camera must be inside the room volume and the volume must carry the interior exposure range of the current scenario
				const FVector Cam = PC->PlayerCameraManager->GetCameraLocation();
				const FSpnLightingScenario* S = R.Lighting ? R.Lighting->Scenarios.FindByPredicate([&](const FSpnLightingScenario& X) { return X.Name == R.Lighting->GetCurrentScenario(); }) : nullptr;
				const bool bCam = R.Interior != nullptr && R.Interior->EncompassesPoint(Cam, 0.f, nullptr);
				const bool bValues = S != nullptr && R.Interior != nullptr && R.Interior->Settings.bOverride_AutoExposureMinBrightness
					&& FMath::IsNearlyEqual(R.Interior->Settings.AutoExposureMinBrightness, S->InteriorExposureMinEV, 0.01f)
					&& FMath::IsNearlyEqual(R.Interior->Settings.AutoExposureMaxBrightness, S->InteriorExposureMaxEV, 0.01f);
				Check(Op.Id, Pass(bCam && bValues), FString::Printf(TEXT("scenario %s, camera in the room volume %d, room exposure EV %.1f-%.1f (scenario %.1f-%.1f), outside EV %.1f-%.1f"),
					R.Lighting ? *R.Lighting->GetCurrentScenario().ToString() : TEXT("-"), bCam,
					R.Interior ? R.Interior->Settings.AutoExposureMinBrightness : 0.f, R.Interior ? R.Interior->Settings.AutoExposureMaxBrightness : 0.f,
					S ? S->InteriorExposureMinEV : 0.f, S ? S->InteriorExposureMaxEV : 0.f, S ? S->ExposureMinEV : 0.f, S ? S->ExposureMaxEV : 0.f));
				Next();
				break;
			}
			case EOp::CheckOutside:
			{
				const FVector Cam = PC->PlayerCameraManager->GetCameraLocation();
				const bool bOut = R.Interior == nullptr || (!R.Interior->EncompassesPoint(Loc, 0.f, nullptr) && !R.Interior->EncompassesPoint(Cam, 0.f, nullptr));
				Check(Op.Id, Pass(bOut && Move->IsMovingOnGround()), FString::Printf(TEXT("character and camera outside the room volume %d, on ground %d, at (%.0f, %.0f, %.0f)"),
					bOut, Move->IsMovingOnGround(), Loc.X, Loc.Y, Loc.Z));
				Next();
				break;
			}
			case EOp::Scenario:
				PC->ConsoleCommand(FString(TEXT("Spn.Lighting ")) + Op.Id);
				Next();
				break;
			case EOp::Console:
				PC->ConsoleCommand(Op.Id);
				Next();
				break;
			case EOp::CheckMenuQuality:
			{
				// the real settings page: select the row, Right = next value (as a player would with the keyboard), read value and resolution
				USpnUiSubsystem* Ui = PC->GetLocalPlayer() != nullptr ? PC->GetLocalPlayer()->GetSubsystem<USpnUiSubsystem>() : nullptr;
				USpnSettingsSubsystem* Settings = PC->GetGameInstance()->GetSubsystem<USpnSettingsSubsystem>();
				if (Ui == nullptr || Settings == nullptr) { Check(Op.Id, SpnQa::EStatus::Fail, TEXT("UI or settings subsystem missing")); Next(); break; }
				const FSpnPlayerSettings Original = Settings->Get();
				Ui->OpenSettings();
				USpnMenuScreen* Menu = Ui->GetMenuScreen(ESpnPage::Settings);
				const int32 Row = Menu != nullptr ? Menu->FindIndex(TEXT("worldquality")) : INDEX_NONE;
				if (Row == INDEX_NONE) { Ui->CloseAll(); Check(Op.Id, SpnQa::EStatus::Fail, TEXT("the graphics settings have no 'worldquality' row")); Next(); break; }
				Menu->Select(Row);
				const FString Before = Menu->GetItemValue(Row);
				const float PercentBefore = ASpnWorldLighting::GetScreenPercentage();
				Menu->HandleKey(EKeys::Right, false);
				const int32 RowAfter = Menu->FindIndex(TEXT("worldquality"));
				const FString Changed = Menu->GetItemValue(RowAfter);
				const float PercentChanged = ASpnWorldLighting::GetScreenPercentage();
				const bool bSavedChanged = Settings->Get().bWorldQualityMode != Original.bWorldQualityMode;
				Menu->HandleKey(EKeys::Right, false);
				const FString Back = Menu->GetItemValue(Menu->FindIndex(TEXT("worldquality")));
				const float PercentBack = ASpnWorldLighting::GetScreenPercentage();
				Ui->CloseAll();
				Settings->Set(Original);     // leave the user's saved choice as it was
				const bool bOk = !Original.bWorldQualityMode && FMath::IsNearlyEqual(PercentBefore, 90.f) && FMath::IsNearlyEqual(PercentChanged, 100.f)
					&& Changed.Contains(TEXT("100")) && bSavedChanged && FMath::IsNearlyEqual(PercentBack, 90.f) && Back.Contains(TEXT("90"));
				Check(Op.Id, bOk ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail, FString::Printf(TEXT("row '%s' -> Right: '%s' r.ScreenPercentage %.0f (saved %d) -> Right: '%s' %.0f; started at %.0f, saved choice before: %s"),
					*Before, *Changed, PercentChanged, bSavedChanged, *Back, PercentBack, PercentBefore, Original.bWorldQualityMode ? TEXT("quality") : TEXT("standard")));
				Next();
				break;
			}
			case EOp::CheckScreenPercentage:
			{
				const float Now = ASpnWorldLighting::GetScreenPercentage();
				const FString Aa = IConsoleManager::Get().FindConsoleVariable(TEXT("r.AntiAliasingMethod")) != nullptr
					? FString::FromInt(IConsoleManager::Get().FindConsoleVariable(TEXT("r.AntiAliasingMethod"))->GetInt()) : FString(TEXT("?"));
				Check(Op.Id, FMath::IsNearlyEqual(Now, Op.A, 0.5f) ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail,
					FString::Printf(TEXT("r.ScreenPercentage %.0f (expected %.0f), r.AntiAliasingMethod %s (4 = TSR), quality mode %s"), Now, Op.A, *Aa,
						R.Lighting != nullptr && R.Lighting->IsQualityMode() ? TEXT("on") : TEXT("off")));
				Next();
				break;
			}
			case EOp::CheckScenario:
			{
				if (R.Lighting == nullptr) { Check(TEXT("scenario_") + Op.Id, SpnQa::EStatus::Fail, TEXT("the level has no ASpnWorldLighting")); Next(); break; }
				const FSpnLightingScenario* S = R.Lighting->Scenarios.FindByPredicate([&](const FSpnLightingScenario& X) { return X.Name == FName(*Op.Id); });
				if (S == nullptr) { Check(TEXT("scenario_") + Op.Id, SpnQa::EStatus::Fail, TEXT("scenario not defined in SPN_WorldLighting")); Next(); break; }
				FString Problems;
				if (R.Lighting->GetCurrentScenario() != S->Name) { Problems += FString::Printf(TEXT(" current scenario is %s;"), *R.Lighting->GetCurrentScenario().ToString()); }
				for (TActorIterator<ADirectionalLight> It(Character.GetWorld()); It; ++It)
				{
					const ULightComponent* L = It->GetLightComponent();
					const bool bMoon = It->ActorHasTag(ASpnWorldLighting::MoonTag);
					const float Want = bMoon ? S->MoonLux : S->SunLux;
					if (L && !FMath::IsNearlyEqual(L->Intensity, Want, 1.f)) { Problems += FString::Printf(TEXT(" %s %.0f lux (want %.0f);"), bMoon ? TEXT("moon") : TEXT("sun"), L->Intensity, Want); }
				}
				int32 Lamps = 0, LampsWrong = 0, Mist = 0, MistWrong = 0;
				for (TActorIterator<AActor> It(Character.GetWorld()); It; ++It)
				{
					if (It->ActorHasTag(ASpnWorldLighting::LampTag))
					{
						TInlineComponentArray<ULightComponent*> Lights(*It);
						for (ULightComponent* L : Lights) { ++Lamps; LampsWrong += L->IsVisible() != S->bLampsOn; }
					}
					if (It->ActorHasTag(ASpnWorldLighting::MistTag))
					{
						TInlineComponentArray<ULocalFogVolumeComponent*> Fog(*It);
						for (ULocalFogVolumeComponent* F : Fog) { ++Mist; MistWrong += F->IsVisible() != (S->MistDensity > 0.f); }
					}
				}
				if (LampsWrong) { Problems += FString::Printf(TEXT(" %d of %d lamps wrong;"), LampsWrong, Lamps); }
				if (MistWrong) { Problems += FString::Printf(TEXT(" %d of %d mist patches wrong;"), MistWrong, Mist); }
				if (R.Interior && !FMath::IsNearlyEqual(R.Interior->Settings.AutoExposureMinBrightness, S->InteriorExposureMinEV, 0.01f)) { Problems += TEXT(" room exposure not updated;"); }
				Check(TEXT("scenario_") + Op.Id, Pass(Problems.IsEmpty()), FString::Printf(TEXT("Spn.Lighting %s: sun %.0f lux, lamps on %d (%d lights), mist %.2f (%d patches), room EV %.1f-%.1f%s"),
					*Op.Id, S->SunLux, S->bLampsOn, Lamps, S->MistDensity, Mist, S->InteriorExposureMinEV, S->InteriorExposureMaxEV, Problems.IsEmpty() ? TEXT("") : *(TEXT(":") + Problems)));
				Next();
				break;
			}
			case EOp::Shot:
				SpnQa::Screenshot(FString(Suite) + TEXT("_") + Op.Id);
				Next();
				break;
			}
			return !R.Ops.IsValidIndex(R.Index);
		},
		[this, Run](ASupernaturalProtCharacter& Character, bool bTimedOut)
		{
			Run->Keys.ReleaseAll(Character);
			if (!Run->Failure.IsEmpty() || bTimedOut)
			{
				SpnQa::Check(*this, &Character, Suite, TEXT("run"), SpnQa::EStatus::Blocked,
					Run->Failure.IsEmpty() ? FString::Printf(TEXT("timeout at step %d of %d"), Run->Index, Run->Ops.Num()) : Run->Failure);
				return;
			}
			SpnQa::Check(*this, &Character, Suite, TEXT("run"), SpnQa::EStatus::Pass, FString::Printf(TEXT("all %d steps ran"), Run->Ops.Num()));
		}));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));
	return true;
}

#endif
