#include "Components/CapsuleComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "EngineUtils.h"
#include "Spn/SpnInteractionComponent.h"
#include "Spn/SpnTestDoor.h"
#include "Spn/Tests/SpnMovementTestSupport.h"
#include "Spn/Tests/SpnQaRecorder.h"
#include "Spn/Tests/SpnTestPlayReset.h"

#if WITH_DEV_AUTOMATION_TESTS

// SpnQA.MovementDemo (M2A4.1): the recorded movement regression demonstration on the V3 level. ONE uninterrupted run of the real character from the
// V3 PlayerStart, driven only by simulated KEY events (W, S, A/D never needed, LeftShift, Space, V, E) and the control rotation, like a human.
// The character is never repositioned: this file calls no SetActorLocation, TeleportTo or transform change on it (test obstacles are spawned in front
// of it and destroyed again; they are not saved in any level).
//   street (West Avenue): walk, brake, jog, run, sprint, sprint stop, 90 degree sprint turns right and left, reversal, standing / running / sprint jump,
//                         a 150 cm wall (blocks), a 15 cm step (walked over), a 40 cm ledge (jumped onto and over), an 80 cm ledge (blocks)
//   building 1:           approach, open the door with E, enter, room, turn around, leave, close the door
// Each exercise records a check (speed reached, stopped, jump apex, blocked or crossed). Evidence next to records.jsonl: input_events.jsonl and
// traces.jsonl (position and speed every 0.1 s). The limits are derived from the movement settings (gait speeds, jump height), not tuned to results.

using namespace SpnMovementTest;

namespace
{
	constexpr EAutomationTestFlags DemoFlags = EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter;
	const TCHAR* const Suite = TEXT("MovementDemo");
	const TCHAR* const DoorId = TEXT("d_cottage1_door");
	const FString PromptOpen = TEXT("E – Tür öffnen");

	const FVector2D RoadStart(-10450, -4700);
	const FVector2D Cottage1Front(-9600, -5440);
	const FVector2D Cottage1Doorway(-9400, -5440);
	const FVector2D Cottage1Out(-9640, -5440);
	const FVector2D Cottage1Route[] = {{-9320, -5440}, {-9320, -5630}, {-9100, -5630}, {-8760, -5630}, {-9100, -5630}, {-9320, -5630}, {-9320, -5440}};

	enum class EOp : uint8
	{
		Section, Wait, WaitSettled, GaitKey, KeyDown, KeyUp, ReleaseAll, Tap, Walk, Turn, FaceYaw, Mark, CheckSpeedAtLeast, CheckStopped, CheckApex, SpawnWall, ClearWalls,
		CheckBlocked, JumpAtWall, CheckBeyondWall, CheckGrounded, Shot, FaceDoor, PressE, CheckPrompt, CheckDoor, CheckInside, CheckOutside
	};

	// Key indices used in Op.A: 0 W, 1 S, 2 LeftShift, 3 Space, 4 E.
	const FKey& KeyOf(int32 Index)
	{
		static const FKey Keys[] = {EKeys::W, EKeys::S, EKeys::LeftShift, EKeys::SpaceBar, EKeys::E};
		return Keys[FMath::Clamp(Index, 0, 4)];
	}

	struct FOp
	{
		EOp Kind;
		FVector2D P = FVector2D::ZeroVector;
		float A = 0.f;
		FString Id;
	};

	TArray<FOp> BuildOps()
	{
		TArray<FOp> Ops;
		const auto Add = [&Ops](EOp Kind, FVector2D P = FVector2D::ZeroVector, float A = 0.f, const TCHAR* Id = TEXT("")) { Ops.Add({Kind, P, A, Id}); };
		const auto Wait = [&Add](double S) { Add(EOp::Wait, FVector2D::ZeroVector, static_cast<float>(S)); };
		const auto Down = [&Add](int32 K) { Add(EOp::KeyDown, FVector2D::ZeroVector, static_cast<float>(K)); };
		const auto Up = [&Add](int32 K) { Add(EOp::KeyUp, FVector2D::ZeroVector, static_cast<float>(K)); };
		const auto Section = [&Add](const TCHAR* Text) { Add(EOp::Section, FVector2D::ZeroVector, 0.f, Text); };
		// gait numbers: 0 walk, 1 jog, 2 run
		const auto Gait = [&Add](int32 G) { Add(EOp::GaitKey, FVector2D::ZeroVector, static_cast<float>(G)); };

		Section(TEXT("Movement demo: walking, braking, gaits"));
		Wait(3.0);
		Add(EOp::Shot, FVector2D::ZeroVector, 0.f, TEXT("01_start"));
		Add(EOp::Walk, FVector2D(-10450, -4700), 40.f);   // already there: spawn point of the level
		Add(EOp::FaceYaw, FVector2D(90.f, 0.f));          // face north along West Avenue
		Wait(0.6);
		Gait(0);
		Down(0); Wait(3.5); Add(EOp::CheckSpeedAtLeast, FVector2D::ZeroVector, 171.f, TEXT("walk_reaches_walk_speed")); Up(0);
		Wait(1.6); Add(EOp::CheckStopped, FVector2D::ZeroVector, 0.f, TEXT("walk_brakes_to_a_stop"));
		Gait(1);
		Down(0); Wait(3.0); Add(EOp::CheckSpeedAtLeast, FVector2D::ZeroVector, 332.f, TEXT("jog_reaches_jog_speed")); Up(0);
		Wait(1.6); Add(EOp::CheckStopped, FVector2D::ZeroVector, 0.f, TEXT("jog_brakes_to_a_stop"));
		Gait(2);
		Down(0); Wait(3.2); Add(EOp::CheckSpeedAtLeast, FVector2D::ZeroVector, 522.f, TEXT("run_reaches_run_speed")); Up(0);
		Wait(1.8); Add(EOp::CheckStopped, FVector2D::ZeroVector, 0.f, TEXT("run_brakes_to_a_stop"));

		Section(TEXT("Movement demo: sprint, stop, turns, reversal"));
		Gait(1);
		Down(2); Down(0); Wait(4.5); Add(EOp::CheckSpeedAtLeast, FVector2D::ZeroVector, 760.f, TEXT("sprint_reaches_sprint_speed")); Up(0); Up(2);
		Wait(2.4); Add(EOp::CheckStopped, FVector2D::ZeroVector, 0.f, TEXT("sprint_brakes_to_a_stop"));
		Down(2); Down(0); Wait(3.0);
		Add(EOp::Turn, FVector2D(-90.f, 0.f), 0.3f); Wait(0.7); Add(EOp::CheckSpeedAtLeast, FVector2D::ZeroVector, 640.f, TEXT("sprint_turn_right_keeps_speed"));
		Add(EOp::Turn, FVector2D(90.f, 0.f), 0.3f); Wait(1.2); Add(EOp::CheckSpeedAtLeast, FVector2D::ZeroVector, 640.f, TEXT("sprint_turn_left_keeps_speed"));
		Up(0); Up(2); Down(1);   // reversal: forward released, back pressed while sprinting
		Wait(2.4); Up(1); Wait(1.2);
		Add(EOp::CheckStopped, FVector2D::ZeroVector, 0.f, TEXT("reversal_ends_standing"));

		Section(TEXT("Movement demo: jumping and landing"));
		Add(EOp::FaceYaw, FVector2D(270.f, 0.f));   // back south along the avenue: keeps the whole demo within about 100 m of the start
		Wait(0.5);
		Add(EOp::Mark); Add(EOp::Tap, FVector2D::ZeroVector, 3.f); Wait(1.0); Add(EOp::CheckApex, FVector2D::ZeroVector, 0.f, TEXT("standing_jump_apex"));
		Add(EOp::CheckGrounded, FVector2D::ZeroVector, 0.f, TEXT("standing_jump_lands"));
		Down(0); Wait(2.0); Add(EOp::Mark); Add(EOp::Tap, FVector2D::ZeroVector, 3.f); Wait(1.3); Add(EOp::CheckApex, FVector2D::ZeroVector, 0.f, TEXT("jog_jump_apex")); Up(0);
		Wait(1.5);
		Gait(1);
		Down(2); Down(0); Wait(3.2); Add(EOp::Mark); Add(EOp::Tap, FVector2D::ZeroVector, 3.f); Wait(1.3); Add(EOp::CheckApex, FVector2D::ZeroVector, 0.f, TEXT("sprint_jump_apex")); Up(0); Up(2);
		Wait(2.4); Add(EOp::CheckGrounded, FVector2D::ZeroVector, 0.f, TEXT("sprint_jump_lands"));

		Section(TEXT("Movement demo: obstacles"));
		Add(EOp::FaceYaw, FVector2D(90.f, 0.f));
		Wait(0.5);
		Add(EOp::SpawnWall, FVector2D(650.f, 0.f), 150.f);
		Add(EOp::Shot, FVector2D::ZeroVector, 0.f, TEXT("02_wall_ahead"));
		Down(0); Wait(4.0); Add(EOp::CheckBlocked, FVector2D::ZeroVector, 0.f, TEXT("wall_150_blocks_walking")); Up(0);
		Add(EOp::Shot, FVector2D::ZeroVector, 0.f, TEXT("03_blocked_by_wall"));
		Add(EOp::ClearWalls);
		Wait(0.5);
		Add(EOp::SpawnWall, FVector2D(650.f, 0.f), 15.f);
		Down(0); Wait(3.0); Add(EOp::CheckBeyondWall, FVector2D::ZeroVector, 0.f, TEXT("step_15_is_walked_over")); Up(0);
		Add(EOp::ClearWalls);
		Wait(0.8);
		Add(EOp::SpawnWall, FVector2D(900.f, 0.f), 40.f);
		Down(0); Add(EOp::JumpAtWall, FVector2D::ZeroVector, 125.f); Wait(2.5); Add(EOp::CheckBeyondWall, FVector2D::ZeroVector, 0.f, TEXT("ledge_40_is_jumped_over")); Up(0);
		Add(EOp::Shot, FVector2D::ZeroVector, 0.f, TEXT("04_after_the_ledge"));
		Add(EOp::ClearWalls);
		Wait(0.8);
		Add(EOp::SpawnWall, FVector2D(900.f, 0.f), 80.f);
		Down(0); Add(EOp::JumpAtWall, FVector2D::ZeroVector, 125.f); Add(EOp::WaitSettled, FVector2D::ZeroVector, 25.f); Add(EOp::CheckBlocked, FVector2D::ZeroVector, 0.f, TEXT("ledge_80_blocks_even_with_a_jump")); Up(0);
		Add(EOp::Shot, FVector2D::ZeroVector, 0.f, TEXT("05_ledge_80_blocks"));
		Add(EOp::ClearWalls);
		Wait(1.0);

		Section(TEXT("Movement demo: back to building 1, enter and leave"));
		Gait(1);
		Add(EOp::Walk, FVector2D(-10200, -4300), 60.f);
		Add(EOp::Walk, FVector2D(-10200, -5000), 60.f);
		Add(EOp::Walk, FVector2D(-9850, -5440), 50.f);
		Add(EOp::Walk, Cottage1Front, 30.f);
		Wait(0.6);
		Add(EOp::FaceDoor);
		Wait(0.8);
		Add(EOp::CheckPrompt, FVector2D::ZeroVector, 0.f, TEXT("door_prompt"));
		Add(EOp::Shot, FVector2D::ZeroVector, 0.f, TEXT("06_door_prompt"));
		Add(EOp::PressE);
		Wait(1.3);
		Add(EOp::CheckDoor, FVector2D::ZeroVector, 1.f, TEXT("door_opens_with_e"));
		Add(EOp::Walk, Cottage1Doorway, 22.f);
		for (int32 I = 0; I < 4; ++I) { Add(EOp::Walk, Cottage1Route[I], 22.f); }
		Add(EOp::CheckInside, Cottage1Route[3], 60.f, TEXT("room_reached"));
		Add(EOp::Turn, FVector2D(180.f, 0.f), 2.0f);
		Add(EOp::Shot, FVector2D::ZeroVector, 0.f, TEXT("07_inside_the_room"));
		for (int32 I = 4; I < UE_ARRAY_COUNT(Cottage1Route); ++I) { Add(EOp::Walk, Cottage1Route[I], 22.f); }
		Add(EOp::Walk, Cottage1Doorway, 25.f);
		Add(EOp::Walk, Cottage1Out, 30.f);
		Wait(0.4);
		Add(EOp::CheckOutside, Cottage1Doorway, 0.f, TEXT("building_left"));
		Add(EOp::FaceDoor);
		Add(EOp::PressE);
		Wait(1.4);
		Add(EOp::CheckDoor, FVector2D::ZeroVector, 0.f, TEXT("door_closes_with_e"));
		Wait(2.5);
		return Ops;
	}

	void CleanShot(const FString& Id)
	{
		if (FApp::CanEverRender())
		{
			FScreenshotRequest::RequestScreenshot(SpnQa::QaDir() / TEXT("shots") / (FString(TEXT("MovementDemo_")) + Id + TEXT(".png")), /*bShowUI*/ false, /*bAddFilenameSuffix*/ false);
		}
	}

	struct FDemoRun
	{
		TArray<FOp> Ops = BuildOps();
		SpnQa::FHeldKeys Keys;
		ASpnTestDoor* Door = nullptr;
		TArray<TWeakObjectPtr<AStaticMeshActor>> Walls;
		int32 Index = 0, Frames = 0;
		double StepStart = 0.0, LastTrace = -1.0, LastLook = -1.0;
		float TurnFromYaw = 0.f;
		FVector MarkLoc = FVector::ZeroVector;
		float WallFar = 0.f;   // distance from the mark to the far face of the spawned obstacle, plus the capsule radius
		float MaxZSinceMark = -1.0e9f;
		FString Failure, InputLog;
		TArray<TArray<double>> Trace;
		bool bHavePrev = false;
		FVector Previous = FVector::ZeroVector;
		double MaxStepPerSecond = 0.0, Walked = 0.0, MaxStep = 0.0, MaxWindowSpeed = 0.0;
		TArray<TPair<double, FVector>> History;
		int32 Checks = 0, Failed = 0;
	};

	void FaceXY(ASupernaturalProtCharacter& Character, const FVector2D& Target, float Pitch)
	{
		const FVector From = Character.GetActorLocation();
		Character.GetController()->SetControlRotation(FRotator(Pitch, FMath::RadiansToDegrees(FMath::Atan2(Target.Y - From.Y, Target.X - From.X)), 0.f));
	}

	AStaticMeshActor* SpawnBlock(UWorld& World, const FVector& Center, const FVector& Size)
	{
		AStaticMeshActor* Actor = World.SpawnActor<AStaticMeshActor>(Center, FRotator::ZeroRotator);
		Actor->SetMobility(EComponentMobility::Movable);
		Actor->GetStaticMeshComponent()->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube")));
		Actor->GetStaticMeshComponent()->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
		Actor->SetActorScale3D(Size / 100.f);
		return Actor;
	}

	void LogInput(FDemoRun& R, double Time, const TCHAR* Kind, const FString& Name, const FString& Extra = FString())
	{
		R.InputLog += FString::Printf(TEXT("{\"t\":%.3f,\"event\":\"%s\",\"name\":\"%s\"%s}\n"), Time, Kind, *Name, *Extra);
	}

	class FDemoCommand : public IAutomationLatentCommand
	{
	public:
		FDemoCommand(FAutomationTestBase& InTest, TSharedRef<FDemoRun> InRun) : Test(InTest), Run(InRun) {}

		virtual bool Update() override
		{
			ASupernaturalProtCharacter* Character = FindCharacter();
			if (Character == nullptr)
			{
				if (++WaitFrames > 900) { Test.AddError(TEXT("no possessed character after about 15 s in the map")); return true; }
				return false;
			}
			UWorld* World = Character->GetWorld();
			if (!bStarted)
			{
				bStarted = true;
				StartTime = World->GetTimeSeconds();
				for (TActorIterator<ASpnTestDoor> It(World); It; ++It) { if (It->StableId == DoorId) { Run->Door = *It; } }
				const FVector P = Character->GetActorLocation();
				UE_LOG(LogTemp, Display, TEXT("SPNQA MovementDemo: character spawned at (%.0f, %.0f, %.0f), nothing will reposition it"), P.X, P.Y, P.Z);
				return false;
			}
			const double Elapsed = World->GetTimeSeconds() - StartTime;
			const bool bDone = Step(*Character, World->GetDeltaSeconds(), Elapsed);
			if (bDone || Elapsed > 480.0) { Finish(*Character, !bDone); return true; }
			return false;
		}

	private:
		void Check(ASupernaturalProtCharacter& Character, const FString& Id, bool bOk, const FString& Detail)
		{
			++Run->Checks;
			Run->Failed += bOk ? 0 : 1;
			SpnQa::Check(Test, &Character, Suite, Id, bOk ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail, Detail);
		}

		bool Step(ASupernaturalProtCharacter& Character, float Dt, double Elapsed)
		{
			FDemoRun& R = *Run;
			const FVector Loc = Character.GetActorLocation();
			const float Speed = Character.GetVelocity().Size2D();
			if (R.bHavePrev && Dt > 0.f)
			{
				R.Walked += FVector::Dist2D(Loc, R.Previous);
				R.MaxStepPerSecond = FMath::Max(R.MaxStepPerSecond, FVector::Dist(Loc, R.Previous) / Dt);
				R.MaxStep = FMath::Max(R.MaxStep, static_cast<double>(FVector::Dist(Loc, R.Previous)));
			}
			R.Previous = Loc; R.bHavePrev = true;
			// Speed over a 0.25 s window: one frame (dt jitter around a hitch or a screenshot) can show a 1500+ cm/s "speed" for a character that moves at jog speed.
			R.History.Add(TPair<double, FVector>(Elapsed, Loc));
			while (R.History.Num() > 1 && Elapsed - R.History[0].Key > 0.25) { R.History.RemoveAt(0); }
			if (Elapsed - R.History[0].Key >= 0.2) { R.MaxWindowSpeed = FMath::Max(R.MaxWindowSpeed, FVector::Dist(Loc, R.History[0].Value) / (Elapsed - R.History[0].Key)); }
			R.MaxZSinceMark = FMath::Max(R.MaxZSinceMark, static_cast<float>(Loc.Z));
			APlayerController* PC = Cast<APlayerController>(Character.GetController());
			if (Elapsed - R.LastTrace >= 0.1) { R.LastTrace = Elapsed; R.Trace.Add({Elapsed, Loc.X, Loc.Y, Loc.Z, static_cast<double>(Speed), static_cast<double>(R.Index)}); }
			if (Elapsed - R.LastLook >= 0.25) { R.LastLook = Elapsed; LogInput(R, Elapsed, TEXT("look"), TEXT("yaw"), FString::Printf(TEXT(",\"yaw\":%.1f,\"step\":%d"), PC->GetControlRotation().Yaw, R.Index)); }
			if (R.Door == nullptr) { R.Failure = TEXT("door actor d_cottage1_door is missing"); return true; }
			if (!R.Ops.IsValidIndex(R.Index)) { return true; }
			const FOp& Op = R.Ops[R.Index];
			const double InStep = Elapsed - R.StepStart;
			const auto Next = [&]() { ++R.Index; R.StepStart = Elapsed; R.Frames = 0; };
			const auto Key = [&](int32 K, bool bDown)
			{
				const FKey& Key = KeyOf(K);
				if (bDown == R.Keys.Down.Contains(Key)) { return; }
				SpnQa::Key(*PC, Key, bDown);
				if (bDown) { R.Keys.Down.Add(Key); } else { R.Keys.Down.Remove(Key); }
				LogInput(R, Elapsed, bDown ? TEXT("key_down") : TEXT("key_up"), Key.ToString());
			};
			const auto ForwardWall = [&]() -> AStaticMeshActor* { for (const TWeakObjectPtr<AStaticMeshActor>& W : R.Walls) { if (W.IsValid()) { return W.Get(); } } return nullptr; };
			const USpnInteractionComponent* Interaction = Character.FindComponentByClass<USpnInteractionComponent>();
			const FString Prompt = Interaction != nullptr ? Interaction->GetCurrentPrompt() : FString();
			const bool bDoorSettled = FMath::IsNearlyEqual(R.Door->GetCurrentAngle(), R.Door->GetTargetAngle(), 1.f);
			const FSpnMovementSettings& S = Character.GetSpnMovement()->Settings;

			switch (Op.Kind)
			{
			case EOp::Section: SpnQa::Section(Op.Id); Next(); break;
			case EOp::Wait: if (InStep >= Op.A) { Next(); } break;
			case EOp::WaitSettled:
				// Until the character stands still on the ground (or Op.A seconds): the old fixed wait failed once because the walk to the wall was not finished. The blocking check itself is unchanged.
				if (InStep >= 2.0 && Speed < 15.f && Character.GetCharacterMovement()->IsMovingOnGround()) { Next(); }
				else if (InStep >= Op.A) { Next(); }
				break;
			case EOp::GaitKey:
			{
				const ESpnGait Wanted = static_cast<ESpnGait>(static_cast<int32>(Op.A));
				if (Character.GetSpnMovement()->GetBaseGait() == Wanted) { Next(); break; }
				if (InStep > 3.0) { R.Failure = TEXT("the V key did not select the wanted gait"); return true; }
				// press V on one frame, release on the next (the base gait cycles Walk -> Jog -> Run -> Walk)
				if (R.Frames % 6 == 0) { SpnQa::Key(*PC, EKeys::V, true); LogInput(R, Elapsed, TEXT("key_down"), TEXT("V")); }
				else if (R.Frames % 6 == 1) { SpnQa::Key(*PC, EKeys::V, false); LogInput(R, Elapsed, TEXT("key_up"), TEXT("V")); }
				++R.Frames;
				break;
			}
			case EOp::KeyDown: Key(static_cast<int32>(Op.A), true); Next(); break;
			case EOp::KeyUp: Key(static_cast<int32>(Op.A), false); Next(); break;
			case EOp::ReleaseAll: R.Keys.ReleaseAll(Character); Next(); break;
			case EOp::Tap:
				Key(static_cast<int32>(Op.A), R.Frames == 0);
				if (++R.Frames > 2) { Key(static_cast<int32>(Op.A), false); Next(); }
				break;
			case EOp::Walk:
			{
				const FVector2D To = Op.P - FVector2D(Loc.X, Loc.Y);
				if (To.Size() <= Op.A) { Key(0, false); Next(); break; }
				if (InStep > 45.0) { Key(0, false); R.Failure = FString::Printf(TEXT("stuck %.0f cm from the waypoint (%.0f, %.0f) at (%.0f, %.0f, %.0f), step %d"), To.Size(), Op.P.X, Op.P.Y, Loc.X, Loc.Y, Loc.Z, R.Index); return true; }
				FaceXY(Character, Op.P, -10.f);
				Key(0, true);
				break;
			}
			case EOp::Turn:
				if (R.Frames == 0) { R.TurnFromYaw = PC->GetControlRotation().Yaw; R.Frames = 1; }
				{
					const float Alpha = FMath::Clamp(static_cast<float>(InStep) / FMath::Max(Op.A, 0.01f), 0.f, 1.f);
					PC->SetControlRotation(FRotator(-10.f, R.TurnFromYaw + Op.P.X * FMath::InterpEaseInOut(0.f, 1.f, Alpha, 2.f), 0.f));
					if (Alpha >= 1.f) { Next(); }
				}
				break;
			case EOp::FaceYaw: PC->SetControlRotation(FRotator(-10.f, Op.P.X, 0.f)); Next(); break;
			case EOp::Mark: R.MarkLoc = Loc; R.MaxZSinceMark = Loc.Z; Next(); break;
			case EOp::CheckSpeedAtLeast:
				Check(Character, Op.Id, Speed >= Op.A, FString::Printf(TEXT("speed %.0f cm/s, at least %.0f expected (95%% of the gait speed)"), Speed, Op.A));
				Next();
				break;
			case EOp::CheckStopped:
				Check(Character, Op.Id, Speed < 10.f, FString::Printf(TEXT("speed %.0f cm/s after the input ended (stopped below 10)"), Speed));
				Next();
				break;
			case EOp::CheckApex:
			{
				const float Apex = R.MaxZSinceMark - R.MarkLoc.Z;
				Check(Character, Op.Id, FMath::Abs(Apex - S.JumpHeight) <= 7.f, FString::Printf(TEXT("apex %.0f cm, configured jump height %.0f cm (+-7)"), Apex, S.JumpHeight));
				Next();
				break;
			}
			case EOp::CheckGrounded:
				Check(Character, Op.Id, Character.GetCharacterMovement()->IsMovingOnGround() && FMath::Abs(Loc.Z - StandZ) <= 12.f, FString::Printf(TEXT("on the ground at Z %.0f (standing %.0f)"), Loc.Z, StandZ));
				Next();
				break;
			case EOp::SpawnWall:
			{
				const FVector Forward = FRotator(0.f, PC->GetControlRotation().Yaw, 0.f).Vector();
				const FVector Center = Loc + Forward * Op.P.X;
				// 12 m wide: a 4 m wall let the character slide round its end once (run 20261029-022846), which is not what the check measures
				AStaticMeshActor* Wall = SpawnBlock(*Character.GetWorld(), FVector(Center.X, Center.Y, Op.A / 2.f), FVector(1200.f, 100.f, Op.A));
				Wall->SetActorRotation(FRotator(0.f, PC->GetControlRotation().Yaw - 90.f, 0.f));
				R.Walls.Add(Wall);
				R.MarkLoc = Loc;
				R.WallFar = Op.P.X + 50.f + 42.f;
				Next();
				break;
			}
			case EOp::ClearWalls:
				for (const TWeakObjectPtr<AStaticMeshActor>& W : R.Walls) { if (W.IsValid()) { W->Destroy(); } }
				R.Walls.Reset();
				Next();
				break;
			case EOp::JumpAtWall:
			{
				AStaticMeshActor* Wall = ForwardWall();
				if (Wall == nullptr) { Next(); break; }
				const float Distance = FVector::Dist2D(Loc, Wall->GetActorLocation()) - 50.f - 42.f;   // to the near face, from the capsule edge
				if (Distance <= Op.A) { Key(3, true); R.Frames = 1; Next(); }
				else if (InStep > 8.0) { R.Failure = TEXT("never reached the take-off point in front of the obstacle"); return true; }
				break;
			}
			case EOp::CheckBlocked:
			{
				AStaticMeshActor* Wall = ForwardWall();
				const float Gap = Wall != nullptr ? FVector::Dist2D(Loc, Wall->GetActorLocation()) - 50.f : 0.f;   // centre to the near face
				const bool bOk = Wall != nullptr && Speed < 15.f && Gap > 30.f && Gap < 60.f && FMath::Abs(Loc.Z - StandZ) <= 12.f;
				Check(Character, Op.Id, bOk, FString::Printf(TEXT("stopped %.0f cm (centre) in front of the obstacle face, speed %.0f, Z %.0f (standing %.0f); not crossed, not climbed"), Gap, Speed, Loc.Z, StandZ));
				Key(3, false);
				Next();
				break;
			}
			case EOp::CheckBeyondWall:
			{
				const FVector2D Moved = FVector2D(Loc.X - R.MarkLoc.X, Loc.Y - R.MarkLoc.Y);
				Check(Character, Op.Id, Moved.Size() > R.WallFar + 20.f && FMath::Abs(Loc.Z - StandZ) <= 12.f, FString::Printf(TEXT("travelled %.0f cm from the mark (far face of the obstacle at %.0f), Z %.0f (standing %.0f)"), Moved.Size(), R.WallFar, Loc.Z, StandZ));
				Key(3, false);
				Next();
				break;
			}
			case EOp::Shot: CleanShot(Op.Id); Next(); break;
			case EOp::FaceDoor: FaceXY(Character, FVector2D(R.Door->GetComponentsBoundingBox(false).GetCenter().X, R.Door->GetComponentsBoundingBox(false).GetCenter().Y), 0.f); Next(); break;
			case EOp::PressE:
				FaceXY(Character, FVector2D(R.Door->GetComponentsBoundingBox(false).GetCenter().X, R.Door->GetComponentsBoundingBox(false).GetCenter().Y), 0.f);
				if (R.Frames == 0 && Speed > 15.f && InStep < 3.0) { break; }
				Key(4, R.Frames == 0);
				if (++R.Frames > 1) { Key(4, false); Next(); }
				break;
			case EOp::CheckPrompt: Check(Character, Op.Id, Prompt == PromptOpen, FString::Printf(TEXT("prompt '%s'"), *Prompt)); Next(); break;
			case EOp::CheckDoor:
				Check(Character, Op.Id, Op.A > 0.5f ? (R.Door->IsOpen() && bDoorSettled && FMath::Abs(R.Door->GetCurrentAngle()) > 80.f) : (!R.Door->IsOpen() && bDoorSettled),
					FString::Printf(TEXT("door open flag %d, leaf angle %.0f"), R.Door->IsOpen(), R.Door->GetCurrentAngle()));
				Next();
				break;
			case EOp::CheckInside:
				Check(Character, Op.Id, Loc.X > Cottage1Doorway.X + 20.f && FVector2D::Distance(FVector2D(Loc.X, Loc.Y), Op.P) <= Op.A && Character.GetCharacterMovement()->IsMovingOnGround(),
					FString::Printf(TEXT("character at (%.0f, %.0f, %.0f)"), Loc.X, Loc.Y, Loc.Z));
				Next();
				break;
			case EOp::CheckOutside: Check(Character, Op.Id, Loc.X < Op.P.X - 60.f, FString::Printf(TEXT("character at (%.0f, %.0f, %.0f)"), Loc.X, Loc.Y, Loc.Z)); Next(); break;
			}
			return !R.Ops.IsValidIndex(R.Index);
		}

		void Finish(ASupernaturalProtCharacter& Character, bool bTimedOut)
		{
			FDemoRun& R = *Run;
			R.Keys.ReleaseAll(Character);
			for (const TWeakObjectPtr<AStaticMeshActor>& W : R.Walls) { if (W.IsValid()) { W->Destroy(); } }
			if (!R.Failure.IsEmpty() || bTimedOut) { SpnQa::Check(Test, &Character, Suite, TEXT("run"), SpnQa::EStatus::Blocked, R.Failure.IsEmpty() ? FString(TEXT("timeout of the demo run")) : R.Failure); }
			else { SpnQa::Check(Test, &Character, Suite, TEXT("run"), SpnQa::EStatus::Pass, FString::Printf(TEXT("all %d steps ran, %d checks, %d failed"), R.Ops.Num(), R.Checks, R.Failed)); }
			// A repositioning would show up as a jump: one frame of more than 200 cm (sprint, the fastest gait, is 800 cm/s) or more than 1000 cm/s over a 0.25 s window.
			SpnQa::Check(Test, &Character, Suite, TEXT("no_teleport_continuous_motion"), R.MaxStep <= 200.0 && R.MaxWindowSpeed <= 1000.0 ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail,
				FString::Printf(TEXT("largest displacement of one frame %.1f cm (limit 200), highest speed over 0.25 s %.0f cm/s (limit 1000; per-frame speed %.0f is only noise of the frame time), %.0f cm walked"), R.MaxStep, R.MaxWindowSpeed, R.MaxStepPerSecond, R.Walked));
			SpnQa::Trace(Suite, TEXT("position_speed_every_0.1s"), {TEXT("t"), TEXT("x"), TEXT("y"), TEXT("z"), TEXT("speed2d"), TEXT("step")}, R.Trace);
			IFileManager::Get().MakeDirectory(*SpnQa::QaDir(), true);
			FFileHelper::SaveStringToFile(R.InputLog, *(SpnQa::QaDir() / TEXT("input_events.jsonl")), FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM);
		}

		FAutomationTestBase& Test;
		TSharedRef<FDemoRun> Run;
		bool bStarted = false;
		double StartTime = 0.0;
		int32 WaitFrames = 0;
	};
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnQaMovementDemoTest, "SpnQA.MovementDemo", DemoFlags)

bool FSpnQaMovementDemoTest::RunTest(const FString& Parameters)
{
	const FString Map = SpnQa::QaMapPath();
	if (!FPackageName::DoesPackageExist(Map)) { AddError(*FString::Printf(TEXT("%s does not exist: run create_spn_movement_test_map.py --v3 first."), *Map)); return false; }
	if (!Map.Contains(TEXT("V3Interiors")))
	{
		AddWarning(*FString::Printf(TEXT("SpnQA.MovementDemo needs the V3 level (-Map /Game/SpnTest/Lvl_SpnMovementTestV3Interiors); skipped on %s."), *Map));
		return true;
	}
	AutomationOpenMap(Map);
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([]
	{
		if (const UWorld* World = FindPlayWorld()) { if (APlayerController* PC = World->GetFirstPlayerController()) { SpnTestPlay::ResetWorldState(*PC); } }
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.5f));
	ADD_LATENT_AUTOMATION_COMMAND(FDemoCommand(*this, MakeShared<FDemoRun>()));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));
	return true;
}

#endif
