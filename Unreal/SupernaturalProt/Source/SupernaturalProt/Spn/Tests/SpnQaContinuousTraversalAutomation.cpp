#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EngineUtils.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Spn/SpnInteractionComponent.h"
#include "Spn/SpnTestDoor.h"
#include "Spn/Tests/SpnMovementTestSupport.h"
#include "Spn/Tests/SpnQaRecorder.h"
#include "Spn/Tests/SpnTestPlayReset.h"

#if WITH_DEV_AUTOMATION_TESTS

// SpnQA.ContinuousTraversal (M2A3 V3 upgrade): ONE uninterrupted walk of the real character through the V3 level.
// The character appears where the game mode spawns it (the V3 PlayerStart, outside building 1) and is NEVER moved by the test: this file
// contains no SetActorLocation, TeleportTo or transform change of the character, and no camera other than the character's own. Everything is
// done with simulated key events through the player controller (W to walk, E to use) and by turning the control rotation, the same pathways
// as a human player: W -> Enhanced Input -> movement component -> collision; E -> interaction trace -> ASpnTestDoor::SpnUse.
//
//   building 1 (Cottage1, residential wooden door): approach, prompt, E opens, doorway, hall, room 1, turn around, room 2, back, out, E closes
//   street: along West Avenue to building 2
//   building 2 (StorageHall, metal door): approach, prompt, E opens, doorway, aisle, turn around, back, out, E closes
//   final: walk away along the street
//
// Evidence recorded in <QaDir>: records.jsonl (checks), input_events.jsonl (every key edge and sampled look direction with the game time),
// traces.jsonl (position every 0.25 s) and, as checks, the largest displacement of one frame (a teleport would show up as a jump) and the
// distance actually walked. The waypoints are the Python routes of Tools/spn_interiors.py (DEMO_ROUTES, building origin added) and the
// street route that test_spn_interiors.py checks against the layout. PLACEHOLDER geometry; no tuning values.

using namespace SpnMovementTest;

namespace
{
	constexpr EAutomationTestFlags TraversalFlags = EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter;
	const TCHAR* const Suite = TEXT("ContinuousTraversal");
	const TCHAR* const DoorIds[] = {TEXT("d_cottage1_door"), TEXT("d_storage_door")};
	const FString PromptOpen = TEXT("E – Tür öffnen");
	const FString PromptClose = TEXT("E – Tür schließen");

	// Building 1: door plane x = -9400, y = -5440. Building 2: door plane x = -9300, y = -12700.
	const FVector2D Cottage1Front(-9600, -5440);
	const FVector2D Cottage1Doorway(-9400, -5440);
	const FVector2D Cottage1Out(-9640, -5440);
	const FVector2D Cottage1Route[] = {
		{-9320, -5440}, {-9320, -5630}, {-9100, -5630}, {-8760, -5630}, {-9100, -5630}, {-9320, -5630},
		{-9320, -5280}, {-9100, -5280}, {-8840, -5280}, {-9100, -5280}, {-9320, -5280}, {-9320, -5440},
	};
	const FVector2D StorageFront(-9500, -12700);
	const FVector2D StorageDoorway(-9300, -12700);
	const FVector2D StorageOut(-9540, -12700);
	const FVector2D StorageRoute[] = {
		{-9200, -12700}, {-8600, -12700}, {-8600, -12400}, {-8200, -12400}, {-8600, -12400}, {-8600, -12700}, {-9200, -12700},
	};
	// Street between the buildings: west across the sidewalk to West Avenue, south along it, east to the storage hall.
	const FVector2D StreetRoute[] = {{-10200, -5440}, {-10200, -9000}, {-10200, -12700}};

	enum class EOp : uint8
	{
		Section, Wait, Gait, Walk, FaceDoor, PressE, Turn, Shot, ShotUi, CheckClosed, CheckOpen, CheckPrompt, CheckPromptHidden, CheckInside, CheckTurned, CheckOutside
	};

	struct FOp
	{
		EOp Kind;
		FVector2D P = FVector2D::ZeroVector;
		float A = 0.f;
		FString Id;
		int32 Door = 0;
	};

	TArray<FOp> BuildOps()
	{
		TArray<FOp> Ops;
		const auto Add = [&Ops](EOp Kind, FVector2D P = FVector2D::ZeroVector, float A = 0.f, const TCHAR* Id = TEXT(""), int32 Door = 0) { Ops.Add({Kind, P, A, Id, Door}); };
		const auto Wait = [&Add](double Seconds) { Add(EOp::Wait, FVector2D::ZeroVector, static_cast<float>(Seconds)); };

		Add(EOp::Section, FVector2D::ZeroVector, 0.f, TEXT("Walk 1: from the street to building 1"));
		Wait(4.0);
		Add(EOp::CheckPromptHidden, FVector2D::ZeroVector, 0.f, TEXT("prompt_hidden_far_from_doors"));
		Add(EOp::Shot, FVector2D::ZeroVector, 0.f, TEXT("01_start_environment"));
		Add(EOp::Walk, FVector2D(-10100, -5100), 40.f);
		Add(EOp::Walk, FVector2D(-9850, -5440), 40.f);
		Wait(0.5);
		Add(EOp::Shot, FVector2D::ZeroVector, 0.f, TEXT("02_building1_exterior"));
		Add(EOp::Gait, FVector2D::ZeroVector, 0.f);   // walking pace for the last metres, inside and at the doors
		Add(EOp::Walk, Cottage1Front, 30.f);
		Wait(0.6);
		Add(EOp::FaceDoor, FVector2D::ZeroVector, 0.f, TEXT(""), 0);
		Wait(0.8);
		Add(EOp::CheckPrompt, FVector2D::ZeroVector, 0.f, TEXT("prompt_open_building1"), 0);
		Add(EOp::ShotUi, FVector2D::ZeroVector, 0.f, TEXT("03_prompt_residential_door"));
		Wait(0.4);
		Add(EOp::Shot, FVector2D::ZeroVector, 0.f, TEXT("04_residential_door_closeup"));
		Add(EOp::CheckClosed, FVector2D::ZeroVector, 0.f, TEXT("door1_closed_before"), 0);
		Add(EOp::PressE, FVector2D::ZeroVector, 0.f, TEXT(""), 0);
		Wait(0.18);
		Add(EOp::Shot, FVector2D::ZeroVector, 0.f, TEXT("05_door_opening"));
		Wait(1.2);
		Add(EOp::CheckOpen, FVector2D::ZeroVector, 0.f, TEXT("door1_opens_with_e"), 0);
		Add(EOp::FaceDoor, FVector2D::ZeroVector, 0.f, TEXT(""), 0);
		Wait(0.5);
		Add(EOp::CheckPrompt, FVector2D::ZeroVector, 1.f, TEXT("prompt_close_building1"), 0);
		Add(EOp::Section, FVector2D::ZeroVector, 0.f, TEXT("Walk 2: through the door and the rooms of building 1"));
		Add(EOp::Walk, Cottage1Doorway, 22.f);
		Add(EOp::Shot, FVector2D::ZeroVector, 0.f, TEXT("06_threshold_crossing"));
		Add(EOp::Walk, Cottage1Route[0], 22.f);
		Add(EOp::CheckInside, Cottage1Route[0], 60.f, TEXT("building1_hall_reached"), 0);
		for (int32 I = 1; I <= 3; ++I) { Add(EOp::Walk, Cottage1Route[I], 22.f); }
		Wait(0.3);
		Add(EOp::CheckInside, Cottage1Route[3], 60.f, TEXT("building1_room1_reached"), 0);
		Add(EOp::Turn, FVector2D(180.f, 0.f), 2.2f);
		Add(EOp::CheckTurned, FVector2D::ZeroVector, 0.f, TEXT("building1_turned_around"), 0);
		Add(EOp::Shot, FVector2D::ZeroVector, 0.f, TEXT("07_interior_room1"));
		Wait(0.6);
		for (int32 I = 4; I <= 8; ++I) { Add(EOp::Walk, Cottage1Route[I], 22.f); }
		Wait(0.3);
		Add(EOp::CheckInside, Cottage1Route[8], 60.f, TEXT("building1_room2_reached"), 0);
		Add(EOp::Turn, FVector2D(-180.f, 0.f), 2.2f);
		Add(EOp::Shot, FVector2D::ZeroVector, 0.f, TEXT("08_interior_room2"));
		for (int32 I = 9; I < UE_ARRAY_COUNT(Cottage1Route); ++I) { Add(EOp::Walk, Cottage1Route[I], 22.f); }
		Add(EOp::Walk, Cottage1Doorway, 25.f);
		Add(EOp::Shot, FVector2D::ZeroVector, 0.f, TEXT("09_player_exiting"));
		Add(EOp::Walk, Cottage1Out, 30.f);
		Wait(0.4);
		Add(EOp::CheckOutside, Cottage1Doorway, 0.f, TEXT("building1_left"), 0);
		Add(EOp::FaceDoor, FVector2D::ZeroVector, 0.f, TEXT(""), 0);
		Add(EOp::PressE, FVector2D::ZeroVector, 0.f, TEXT(""), 0);
		Wait(1.4);
		Add(EOp::CheckClosed, FVector2D::ZeroVector, 1.f, TEXT("door1_closes_with_e"), 0);

		Add(EOp::Section, FVector2D::ZeroVector, 0.f, TEXT("Walk 3: along West Avenue to building 2"));
		Add(EOp::Gait, FVector2D::ZeroVector, 1.f);   // jogging pace on the street
		Add(EOp::Walk, StreetRoute[0], 40.f);
		Add(EOp::Walk, StreetRoute[1], 60.f);
		Add(EOp::Shot, FVector2D::ZeroVector, 0.f, TEXT("10_street_between_buildings"));
		Add(EOp::Walk, StreetRoute[2], 60.f);
		Add(EOp::Walk, FVector2D(-9700, -12700), 40.f);
		Add(EOp::Gait, FVector2D::ZeroVector, 0.f);
		Add(EOp::Walk, StorageFront, 30.f);
		Wait(0.5);
		Add(EOp::Shot, FVector2D::ZeroVector, 0.f, TEXT("11_building2_exterior"));
		Add(EOp::FaceDoor, FVector2D::ZeroVector, 0.f, TEXT(""), 1);
		Wait(0.8);
		Add(EOp::CheckPrompt, FVector2D::ZeroVector, 0.f, TEXT("prompt_open_building2"), 1);
		Add(EOp::ShotUi, FVector2D::ZeroVector, 0.f, TEXT("12_prompt_metal_door"));
		Wait(0.4);
		Add(EOp::Shot, FVector2D::ZeroVector, 0.f, TEXT("13_metal_door_closeup"));
		Add(EOp::CheckClosed, FVector2D::ZeroVector, 0.f, TEXT("door2_closed_before"), 1);
		Add(EOp::PressE, FVector2D::ZeroVector, 0.f, TEXT(""), 1);
		Wait(1.4);
		Add(EOp::CheckOpen, FVector2D::ZeroVector, 0.f, TEXT("door2_opens_with_e"), 1);

		Add(EOp::Section, FVector2D::ZeroVector, 0.f, TEXT("Walk 4: inside building 2"));
		Add(EOp::Walk, StorageDoorway, 22.f);
		Add(EOp::Walk, StorageRoute[0], 25.f);
		Add(EOp::CheckInside, StorageRoute[0], 60.f, TEXT("building2_entrance_reached"), 1);
		for (int32 I = 1; I <= 3; ++I) { Add(EOp::Walk, StorageRoute[I], 25.f); }
		Wait(0.3);
		Add(EOp::CheckInside, StorageRoute[3], 60.f, TEXT("building2_hall_reached"), 1);
		Add(EOp::Turn, FVector2D(180.f, 0.f), 2.2f);
		Add(EOp::CheckTurned, FVector2D::ZeroVector, 0.f, TEXT("building2_turned_around"), 1);
		Add(EOp::Shot, FVector2D::ZeroVector, 0.f, TEXT("14_interior_warehouse"));
		Wait(0.8);
		for (int32 I = 4; I < UE_ARRAY_COUNT(StorageRoute); ++I) { Add(EOp::Walk, StorageRoute[I], 25.f); }
		Add(EOp::Walk, StorageDoorway, 25.f);
		Add(EOp::Walk, StorageOut, 30.f);
		Wait(0.4);
		Add(EOp::CheckOutside, StorageDoorway, 0.f, TEXT("building2_left"), 1);
		Add(EOp::FaceDoor, FVector2D::ZeroVector, 0.f, TEXT(""), 1);
		Add(EOp::PressE, FVector2D::ZeroVector, 0.f, TEXT(""), 1);
		Wait(1.4);
		Add(EOp::CheckClosed, FVector2D::ZeroVector, 1.f, TEXT("door2_closes_with_e"), 1);

		Add(EOp::Section, FVector2D::ZeroVector, 0.f, TEXT("Walk 5: away from the building"));
		Add(EOp::Walk, FVector2D(-10000, -12700), 40.f);
		Add(EOp::Walk, FVector2D(-10200, -12300), 40.f);
		Add(EOp::Walk, FVector2D(-10200, -11200), 60.f);
		Add(EOp::Turn, FVector2D(-150.f, 0.f), 2.5f);
		Add(EOp::Shot, FVector2D::ZeroVector, 0.f, TEXT("15_environment_overview"));
		Wait(5.0);
		return Ops;
	}

	void CleanShot(const FString& Id, bool bShowUi)
	{
		if (FApp::CanEverRender())
		{
			FScreenshotRequest::RequestScreenshot(SpnQa::QaDir() / TEXT("shots") / (FString(TEXT("ContinuousTraversal_")) + Id + TEXT(".png")), bShowUi, /*bAddFilenameSuffix*/ false);
		}
	}

	struct FTraversalRun
	{
		TArray<FOp> Ops = BuildOps();
		SpnQa::FHeldKeys Keys;
		ASpnTestDoor* Doors[2] = {nullptr, nullptr};
		int32 Index = 0;
		int32 Frames = 0;
		double StepStart = 0.0;
		float TurnFromYaw = 0.f;
		float YawTurned = 0.f;
		FString Failure;
		FString InputLog;
		// movement trace
		bool bHavePrevious = false;
		FVector Previous = FVector::ZeroVector;
		double PreviousTime = 0.0;
		double MaxStepPerSecond = 0.0;
		double MaxStep = 0.0;
		double MaxZStep = 0.0;
		double Walked = 0.0;
		double LastTraceTime = -1.0;
		double LastLookLog = -1.0;
		TArray<TArray<double>> Trace;
		int32 Checks = 0;
		int32 Failed = 0;
	};

	FVector LeafCenter(const ASpnTestDoor& Door) { return Door.GetComponentsBoundingBox(/*bNonColliding*/ false).GetCenter(); }

	void FaceXY(ASupernaturalProtCharacter& Character, const FVector2D& Target, float Pitch)
	{
		const FVector From = Character.GetActorLocation();
		Character.GetController()->SetControlRotation(FRotator(Pitch, FMath::RadiansToDegrees(FMath::Atan2(Target.Y - From.Y, Target.X - From.X)), 0.f));
	}

	void LogInput(FTraversalRun& R, double Time, const TCHAR* Kind, const TCHAR* Name, const FString& Extra = FString())
	{
		R.InputLog += FString::Printf(TEXT("{\"t\":%.3f,\"event\":\"%s\",\"name\":\"%s\"%s}\n"), Time, Kind, Name, *Extra);
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnQaContinuousTraversalTest, "SpnQA.ContinuousTraversal", TraversalFlags)

namespace
{
	/** Waits for the possessed character (spawned by the game mode at the PlayerStart) and then drives it with input only. Never repositions it. */
	class FTraversalCommand : public IAutomationLatentCommand
	{
	public:
		FTraversalCommand(FAutomationTestBase& InTest, TSharedRef<FTraversalRun> InRun) : Test(InTest), Run(InRun) {}

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
				Character->GetSpnMovement()->SetBaseGait(ESpnGait::Jog);
				for (TActorIterator<ASpnTestDoor> It(World); It; ++It)
				{
					for (int32 I = 0; I < 2; ++I) { if (It->StableId == DoorIds[I]) { Run->Doors[I] = *It; } }
				}
				const FVector P = Character->GetActorLocation();
				UE_LOG(LogTemp, Display, TEXT("SPNQA ContinuousTraversal: character spawned at (%.0f, %.0f, %.0f), nothing will reposition it"), P.X, P.Y, P.Z);
				return false;
			}
			const double Elapsed = World->GetTimeSeconds() - StartTime;
			const bool bDone = Step(*Character, World->GetDeltaSeconds(), Elapsed);
			if (bDone || Elapsed > 420.0)
			{
				Finish(*Character, !bDone);
				return true;
			}
			return false;
		}

	private:
		void Trace(ASupernaturalProtCharacter& Character, float Dt, double Elapsed)
		{
			FTraversalRun& R = *Run;
			const FVector Loc = Character.GetActorLocation();
			if (R.bHavePrevious && Dt > 0.f)
			{
				const double Step3 = FVector::Dist(Loc, R.Previous);
				R.Walked += FVector::Dist2D(Loc, R.Previous);
				R.MaxStep = FMath::Max(R.MaxStep, Step3);
				R.MaxZStep = FMath::Max(R.MaxZStep, static_cast<double>(FMath::Abs(Loc.Z - R.Previous.Z)));
				R.MaxStepPerSecond = FMath::Max(R.MaxStepPerSecond, Step3 / Dt);
			}
			R.Previous = Loc;
			R.bHavePrevious = true;
			if (Elapsed - R.LastTraceTime >= 0.25)
			{
				R.LastTraceTime = Elapsed;
				R.Trace.Add({Elapsed, Loc.X, Loc.Y, Loc.Z, Character.GetVelocity().Size2D(), static_cast<double>(R.Index)});
			}
		}

		void Check(ASupernaturalProtCharacter& Character, const FString& Id, bool bOk, const FString& Detail)
		{
			++Run->Checks;
			Run->Failed += bOk ? 0 : 1;
			SpnQa::Check(Test, &Character, Suite, Id, bOk ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail, Detail);
		}

		bool Step(ASupernaturalProtCharacter& Character, float Dt, double Elapsed)
		{
			FTraversalRun& R = *Run;
			Trace(Character, Dt, Elapsed);
			APlayerController* PC = Cast<APlayerController>(Character.GetController());
			if (R.Doors[0] == nullptr || R.Doors[1] == nullptr) { R.Failure = TEXT("door actors d_cottage1_door / d_storage_door are missing in the level"); return true; }
			if (!R.Ops.IsValidIndex(R.Index)) { return true; }
			if (Elapsed - R.LastLookLog >= 0.25)
			{
				R.LastLookLog = Elapsed;
				LogInput(R, Elapsed, TEXT("look"), TEXT("yaw"), FString::Printf(TEXT(",\"yaw\":%.1f,\"step\":%d"), PC->GetControlRotation().Yaw, R.Index));
			}
			const FOp& Op = R.Ops[R.Index];
			ASpnTestDoor* Door = R.Doors[FMath::Clamp(Op.Door, 0, 1)];
			const FVector Loc = Character.GetActorLocation();
			const double InStep = Elapsed - R.StepStart;
			const auto Next = [&]() { ++R.Index; R.StepStart = Elapsed; R.Frames = 0; };
			const auto Key = [&](const FKey& K, bool bDown)
			{
				SpnQa::Key(*PC, K, bDown);
				LogInput(R, Elapsed, bDown ? TEXT("key_down") : TEXT("key_up"), *K.ToString());
			};
			const auto SetW = [&](bool bDown)
			{
				if (bDown != R.Keys.Down.Contains(EKeys::W)) { Key(EKeys::W, bDown); if (bDown) { R.Keys.Down.Add(EKeys::W); } else { R.Keys.Down.Remove(EKeys::W); } }
			};
			if (Op.Kind != EOp::Walk) { SetW(false); }
			const bool bSettled = FMath::IsNearlyEqual(Door->GetCurrentAngle(), Door->GetTargetAngle(), 1.f);
			const USpnInteractionComponent* Interaction = Character.FindComponentByClass<USpnInteractionComponent>();
			const FString Prompt = Interaction != nullptr ? Interaction->GetCurrentPrompt() : FString();

			switch (Op.Kind)
			{
			case EOp::Section:
				SpnQa::Section(Op.Id);
				Next();
				break;
			case EOp::Wait:
				if (InStep >= Op.A) { Next(); }
				break;
			case EOp::Gait:
				Character.GetSpnMovement()->SetBaseGait(Op.A > 0.5f ? ESpnGait::Jog : ESpnGait::Walk);
				Next();
				break;
			case EOp::Walk:
			{
				const FVector2D To = Op.P - FVector2D(Loc.X, Loc.Y);
				if (To.Size() <= Op.A) { SetW(false); Next(); break; }
				if (InStep > 40.0)
				{
					SetW(false);
					R.Failure = FString::Printf(TEXT("stuck %.0f cm from the waypoint (%.0f, %.0f) at (%.0f, %.0f, %.0f), step %d"), To.Size(), Op.P.X, Op.P.Y, Loc.X, Loc.Y, Loc.Z, R.Index);
					return true;
				}
				FaceXY(Character, Op.P, -10.f);
				SetW(true);
				break;
			}
			case EOp::FaceDoor:
				FaceXY(Character, FVector2D(LeafCenter(*Door).X, LeafCenter(*Door).Y), 0.f);
				Next();
				break;
			case EOp::PressE:
				FaceXY(Character, FVector2D(LeafCenter(*Door).X, LeafCenter(*Door).Y), 0.f);
				if (R.Frames == 0 && Character.GetVelocity().Size2D() > 15.f && InStep < 3.0) { break; }
				Key(EKeys::E, R.Frames == 0);
				if (++R.Frames > 1) { Next(); }
				break;
			case EOp::Turn:
				if (R.Frames == 0) { R.TurnFromYaw = PC->GetControlRotation().Yaw; R.YawTurned = 0.f; R.Frames = 1; }
				{
					const float Alpha = FMath::Clamp(static_cast<float>(InStep) / FMath::Max(Op.A, 0.01f), 0.f, 1.f);
					const float Yaw = R.TurnFromYaw + Op.P.X * FMath::InterpEaseInOut(0.f, 1.f, Alpha, 2.f);
					PC->SetControlRotation(FRotator(-12.f, Yaw, 0.f));
					R.YawTurned = Yaw - R.TurnFromYaw;
					if (Alpha >= 1.f) { Next(); }
				}
				break;
			case EOp::Shot:
				CleanShot(Op.Id, false);
				Next();
				break;
			case EOp::ShotUi:
				CleanShot(Op.Id, true);
				Next();
				break;
			case EOp::CheckClosed:
				Check(Character, Op.Id, !Door->IsOpen() && bSettled && FMath::IsNearlyZero(Door->GetCurrentAngle(), 1.f),
					FString::Printf(TEXT("door open flag %d, leaf angle %.0f (target %.0f)"), Door->IsOpen(), Door->GetCurrentAngle(), Door->GetTargetAngle()));
				Next();
				break;
			case EOp::CheckOpen:
				Check(Character, Op.Id, Door->IsOpen() && bSettled && FMath::Abs(Door->GetCurrentAngle()) > 80.f,
					FString::Printf(TEXT("E opened the door: leaf angle %.0f (target %.0f)"), Door->GetCurrentAngle(), Door->GetTargetAngle()));
				Next();
				break;
			case EOp::CheckPrompt:
			{
				const FString& Expected = Op.A > 0.f ? PromptClose : PromptOpen;
				Check(Character, Op.Id, Prompt == Expected, FString::Printf(TEXT("prompt shown while looking at the door: '%s' (expected '%s')"), *Prompt, *Expected));
				Next();
				break;
			}
			case EOp::CheckPromptHidden:
				Check(Character, Op.Id, Prompt.IsEmpty(), FString::Printf(TEXT("prompt far from any door: '%s' (expected none)"), *Prompt));
				Next();
				break;
			case EOp::CheckInside:
				Check(Character, Op.Id, FVector2D::Distance(FVector2D(Loc.X, Loc.Y), Op.P) <= Op.A && FMath::Abs(Loc.Z - (StandZ + 4.f)) <= 20.f && Character.GetCharacterMovement()->IsMovingOnGround(),
					FString::Printf(TEXT("character at (%.0f, %.0f, %.0f), %.0f cm from the waypoint, on ground %d"), Loc.X, Loc.Y, Loc.Z, FVector2D::Distance(FVector2D(Loc.X, Loc.Y), Op.P), Character.GetCharacterMovement()->IsMovingOnGround()));
				Next();
				break;
			case EOp::CheckTurned:
				Check(Character, Op.Id, FMath::Abs(R.YawTurned) >= 170.f, FString::Printf(TEXT("view turned %.0f degrees while standing inside at (%.0f, %.0f)"), R.YawTurned, Loc.X, Loc.Y));
				Next();
				break;
			case EOp::CheckOutside:
				Check(Character, Op.Id, Loc.X < Op.P.X - 60.f && FMath::Abs(Loc.Z - StandZ) <= 20.f,
					FString::Printf(TEXT("character back outside at (%.0f, %.0f, %.0f)"), Loc.X, Loc.Y, Loc.Z));
				Next();
				break;
			}
			return !R.Ops.IsValidIndex(R.Index);
		}

		void Finish(ASupernaturalProtCharacter& Character, bool bTimedOut)
		{
			FTraversalRun& R = *Run;
			R.Keys.ReleaseAll(Character);
			if (!R.Failure.IsEmpty() || bTimedOut)
			{
				SpnQa::Check(Test, &Character, Suite, TEXT("run"), SpnQa::EStatus::Blocked, R.Failure.IsEmpty() ? FString(TEXT("timeout of the traversal run")) : R.Failure);
			}
			else
			{
				SpnQa::Check(Test, &Character, Suite, TEXT("run"), SpnQa::EStatus::Pass, FString::Printf(TEXT("all %d steps ran, %d checks, %d failed"), R.Ops.Num(), R.Checks, R.Failed));
			}
			// A repositioning would show up as one frame with a huge displacement. The fastest gait (sprint) is about 800 cm/s; 1500 cm/s is the limit.
			SpnQa::Check(Test, &Character, Suite, TEXT("no_teleport_continuous_motion"), R.MaxStepPerSecond <= 1500.0 ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail,
				FString::Printf(TEXT("largest displacement of one frame %.1f cm, highest speed over one frame %.0f cm/s (limit 1500), largest vertical step %.1f cm"), R.MaxStep, R.MaxStepPerSecond, R.MaxZStep));
			SpnQa::Check(Test, &Character, Suite, TEXT("walked_distance"), R.Walked > 15000.0 ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail,
				FString::Printf(TEXT("%.0f cm walked on foot (both buildings and the street between them need more than 15000 cm)"), R.Walked));
			SpnQa::Trace(Suite, TEXT("position_every_0.25s"), {TEXT("t"), TEXT("x"), TEXT("y"), TEXT("z"), TEXT("speed2d"), TEXT("step")}, R.Trace);
			IFileManager::Get().MakeDirectory(*SpnQa::QaDir(), true);
			FFileHelper::SaveStringToFile(R.InputLog, *(SpnQa::QaDir() / TEXT("input_events.jsonl")), FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM);
		}

		FAutomationTestBase& Test;
		TSharedRef<FTraversalRun> Run;
		bool bStarted = false;
		double StartTime = 0.0;
		int32 WaitFrames = 0;
	};
}

bool FSpnQaContinuousTraversalTest::RunTest(const FString& Parameters)
{
	const FString Map = SpnQa::QaMapPath();
	if (!FPackageName::DoesPackageExist(Map))
	{
		AddError(*FString::Printf(TEXT("%s does not exist: run create_spn_movement_test_map.py --v3 first."), *Map));
		return false;
	}
	if (!Map.Contains(TEXT("V3Interiors")))
	{
		// The walk starts at the V3 PlayerStart and uses V3 geometry (door frames, furniture, door styles); on another level it would only fail for that reason.
		AddWarning(*FString::Printf(TEXT("SpnQA.ContinuousTraversal needs the V3 level (-Map /Game/SpnTest/Lvl_SpnMovementTestV3Interiors); skipped on %s."), *Map));
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
	ADD_LATENT_AUTOMATION_COMMAND(FTraversalCommand(*this, MakeShared<FTraversalRun>()));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));
	return true;
}

#endif
