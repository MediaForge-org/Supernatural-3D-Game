#include "Camera/CameraActor.h"
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

// SpnQA.BuildingAccess (M2A3 building access): one human-like visit of the Cottage1 entrance, with real key input and a real character.
// The character walks up to the CLOSED door, the E key (player controller -> Enhanced Input -> interaction trace -> ASpnTestDoor::SpnUse) opens
// it, the character walks through the doorway, tries to close the door while standing in it (must be refused: a door never closes through a
// pawn), walks through the hall into both rooms of the furnished cottage, turns around, walks back out, closes the door with E, repeats the
// visit, and finally the view switches to a camera above the street for an overview. Screenshots go to <QaDir>/shots (named BuildingAccess_*),
// the checks to records.jsonl as suite "BuildingAccess". Nothing calls SpnUse or sets the door state directly.
// The inside waypoints are Cottage1's origin (-9400, -5800) plus the DEMO_ROUTES offsets of Tools/spn_interiors.py (checked by
// test_spn_interiors.py against the furnished layout). PLACEHOLDER test geometry; the numbers are not tuning.

using namespace SpnMovementTest;

namespace
{
	constexpr EAutomationTestFlags AccessFlags = EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter;
	const TCHAR* const Suite = TEXT("BuildingAccess");
	const TCHAR* const DoorId = TEXT("d_cottage1_door");

	const FVector2D HomePoint(-10500, -5440);
	const FVector2D InFront(-9600, -5440);   // 200 cm in front of the closed door (door plane x = -9400)
	const FVector2D Doorway(-9400, -5440);
	const FVector2D Outside(-9640, -5440);

	// Hall, west room (room1), hall, east room (room2), hall: Cottage1 origin + DEMO_ROUTES["Cottage1"].
	const FVector2D InsideRoute[] = {
		{-9320, -5440}, {-9320, -5630}, {-9100, -5630}, {-8760, -5630}, {-9100, -5630}, {-9320, -5630},
		{-9320, -5280}, {-9100, -5280}, {-8840, -5280}, {-9100, -5280}, {-9320, -5280}, {-9320, -5440},
	};
	constexpr int32 FirstVisitPoints = UE_ARRAY_COUNT(InsideRoute);

	enum class EOp : uint8
	{
		Section, Wait, Walk, FaceDoor, PressE, Turn, Shot, CheckClosed, CheckOpen, CheckStillOpen, CheckAt, CheckInside, CheckTurned, CheckOutside,
		OverviewStart, OverviewRun, OverviewEnd
	};

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
		const auto EnterAndOpen = [&Add]()
		{
			Add(EOp::Walk, InFront, 30.f);
			Add(EOp::Wait, FVector2D::ZeroVector, 0.6f);
			Add(EOp::FaceDoor);
			Add(EOp::PressE);
			Add(EOp::Wait, FVector2D::ZeroVector, 1.2f);
			Add(EOp::CheckOpen);
		};
		const auto LeaveAndClose = [&Add]()
		{
			Add(EOp::Walk, Doorway, 25.f);
			Add(EOp::Walk, Outside, 30.f);
			Add(EOp::Wait, FVector2D::ZeroVector, 0.4f);
			Add(EOp::CheckOutside);
			Add(EOp::FaceDoor);
			Add(EOp::PressE);
			Add(EOp::Wait, FVector2D::ZeroVector, 1.2f);
			Add(EOp::CheckClosed, FVector2D::ZeroVector, 1.f);   // A > 0: the door was closed with E
		};

		Add(EOp::Section, FVector2D::ZeroVector, 0.f, TEXT("Building access: approach, open, enter, explore, leave, close"));
		Add(EOp::Wait, FVector2D::ZeroVector, 2.0f);
		Add(EOp::Shot, FVector2D::ZeroVector, 0.f, TEXT("01_exterior_enterable_building"));
		Add(EOp::Wait, FVector2D::ZeroVector, 1.2f);
		Add(EOp::Walk, InFront, 30.f);
		Add(EOp::Wait, FVector2D::ZeroVector, 0.9f);
		Add(EOp::FaceDoor);
		Add(EOp::Wait, FVector2D::ZeroVector, 0.5f);
		Add(EOp::Shot, FVector2D::ZeroVector, 0.f, TEXT("02_entrance_closeup"));
		Add(EOp::Shot, FVector2D::ZeroVector, 0.f, TEXT("03_door_closed"));
		Add(EOp::CheckClosed);
		Add(EOp::PressE);
		Add(EOp::Wait, FVector2D::ZeroVector, 0.15f);
		Add(EOp::Wait, FVector2D::ZeroVector, 1.3f);
		Add(EOp::CheckOpen);
		Add(EOp::Shot, FVector2D::ZeroVector, 0.f, TEXT("04_door_open"));
		Add(EOp::Section, FVector2D::ZeroVector, 0.f, TEXT("Building access: through the doorway"));
		Add(EOp::Walk, Doorway, 25.f);
		Add(EOp::Wait, FVector2D::ZeroVector, 0.6f);
		Add(EOp::CheckAt, Doorway, 40.f, TEXT("standing_in_the_doorway"));
		Add(EOp::Shot, FVector2D::ZeroVector, 0.f, TEXT("05_player_in_doorway"));
		Add(EOp::FaceDoor);
		Add(EOp::PressE);   // closing while standing in the doorway must be refused
		Add(EOp::Wait, FVector2D::ZeroVector, 1.0f);
		Add(EOp::CheckStillOpen);
		Add(EOp::Section, FVector2D::ZeroVector, 0.f, TEXT("Building access: inside the cottage"));
		Add(EOp::Walk, InsideRoute[0], 22.f);
		Add(EOp::CheckInside, InsideRoute[0], 60.f, TEXT("hall_reached"));
		Add(EOp::Shot, FVector2D::ZeroVector, 0.f, TEXT("06_interior_hall"));
		Add(EOp::Walk, InsideRoute[1], 22.f);
		Add(EOp::Walk, InsideRoute[2], 22.f);
		Add(EOp::Walk, InsideRoute[3], 22.f);
		Add(EOp::Wait, FVector2D::ZeroVector, 0.4f);
		Add(EOp::CheckInside, InsideRoute[3], 60.f, TEXT("room1_reached"));
		Add(EOp::Turn, FVector2D(180.f, 0.f), 2.0f);
		Add(EOp::CheckTurned);
		Add(EOp::Shot, FVector2D::ZeroVector, 0.f, TEXT("07_interior_room"));
		Add(EOp::Wait, FVector2D::ZeroVector, 1.0f);
		for (int32 I = 4; I < 8; ++I) { Add(EOp::Walk, InsideRoute[I], 22.f); }
		Add(EOp::Walk, InsideRoute[8], 22.f);
		Add(EOp::Wait, FVector2D::ZeroVector, 0.4f);
		Add(EOp::CheckInside, InsideRoute[8], 60.f, TEXT("room2_reached"));
		Add(EOp::Turn, FVector2D(-180.f, 0.f), 2.0f);
		Add(EOp::Shot, FVector2D::ZeroVector, 0.f, TEXT("08_interior_second_room"));
		for (int32 I = 9; I < FirstVisitPoints; ++I) { Add(EOp::Walk, InsideRoute[I], 22.f); }
		Add(EOp::Section, FVector2D::ZeroVector, 0.f, TEXT("Building access: leaving and closing the door"));
		Add(EOp::Walk, Doorway, 25.f);
		Add(EOp::Shot, FVector2D::ZeroVector, 0.f, TEXT("09_player_exiting"));
		LeaveAndClose();
		Add(EOp::Shot, FVector2D::ZeroVector, 0.f, TEXT("10_door_closed_after_exit"));

		Add(EOp::Section, FVector2D::ZeroVector, 0.f, TEXT("Building access: second visit"));
		EnterAndOpen();
		Add(EOp::Walk, Doorway, 25.f);
		for (int32 I = 0; I < 3; ++I) { Add(EOp::Walk, InsideRoute[I], 22.f); }
		Add(EOp::CheckInside, InsideRoute[2], 60.f, TEXT("second_visit_inside"));
		Add(EOp::Walk, InsideRoute[1], 22.f);
		Add(EOp::Walk, InsideRoute[0], 22.f);
		LeaveAndClose();
		Add(EOp::Wait, FVector2D::ZeroVector, 0.8f);

		Add(EOp::Section, FVector2D::ZeroVector, 0.f, TEXT("Building access: view over the district"));
		Add(EOp::OverviewStart);
		Add(EOp::Shot, FVector2D::ZeroVector, 0.f, TEXT("11_overview_environment"));
		Add(EOp::OverviewRun, FVector2D::ZeroVector, 9.f);
		Add(EOp::OverviewEnd);
		return Ops;
	}

	/** Same file name scheme as SpnQa::Screenshot, but without the editor overlay text (the run video shows that instead). */
	void CleanShot(const FString& Id)
	{
		if (FApp::CanEverRender())
		{
			FScreenshotRequest::RequestScreenshot(SpnQa::QaDir() / TEXT("shots") / (FString(TEXT("BuildingAccess_")) + Id + TEXT(".png")), /*bShowUI*/ false, /*bAddFilenameSuffix*/ false);
		}
	}

	struct FAccessRun
	{
		TArray<FOp> Ops = BuildOps();
		SpnQa::FHeldKeys Keys;
		ASpnTestDoor* Door = nullptr;
		int32 Index = -1;
		int32 Frames = 0;
		double StepStart = 0.0;
		float TurnFromYaw = 0.f;
		float YawBeforeTurn = 0.f;
		float YawTurned = 0.f;
		int32 Opens = 0;
		int32 Closes = 0;
		int32 Exits = 0;
		bool bCameraBlending = false;
		FString Failure;
		TWeakObjectPtr<ACameraActor> Camera;
		FVector CameraFrom = FVector::ZeroVector;
		FVector CameraTo = FVector::ZeroVector;
	};

	FVector LeafCenter(const ASpnTestDoor& Door) { return Door.GetComponentsBoundingBox(/*bNonColliding*/ false).GetCenter(); }

	void Face(ASupernaturalProtCharacter& Character, const FVector2D& Target, float Pitch)
	{
		const FVector From = Character.GetActorLocation();
		Character.GetController()->SetControlRotation(FRotator(Pitch, FMath::RadiansToDegrees(FMath::Atan2(Target.Y - From.Y, Target.X - From.X)), 0.f));
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnQaBuildingAccessTest, "SpnQA.BuildingAccess", AccessFlags)

bool FSpnQaBuildingAccessTest::RunTest(const FString& Parameters)
{
	const FString Map = SpnQa::QaMapPath();
	if (!FPackageName::DoesPackageExist(Map))
	{
		AddError(*FString::Printf(TEXT("%s does not exist: run create_spn_movement_test_map.py --v3 first."), *Map));
		return false;
	}
	AutomationOpenMap(Map);
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([]
	{
		if (const UWorld* World = FindPlayWorld()) { if (APlayerController* PC = World->GetFirstPlayerController()) { SpnTestPlay::ResetWorldState(*PC); } }
		return true;
	}));

	const TSharedRef<FAccessRun> Run = MakeShared<FAccessRun>();
	ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(*this, FVector(HomePoint.X, HomePoint.Y, StandZ), 0.f, 300.f,
		[Run](ASupernaturalProtCharacter& Character)
		{
			Character.GetSpnMovement()->SetBaseGait(ESpnGait::Jog);
			for (TActorIterator<ASpnTestDoor> It(Character.GetWorld()); It; ++It) { if (It->StableId == DoorId) { Run->Door = *It; } }
		},
		[this, Run](ASupernaturalProtCharacter& Character, float, double Elapsed)
		{
			FAccessRun& R = *Run;
			APlayerController* PC = Cast<APlayerController>(Character.GetController());
			const FVector Loc = Character.GetActorLocation();
			if (R.Door == nullptr) { R.Failure = TEXT("door actor d_cottage1_door is missing in the level"); return true; }
			if (R.Index < 0) { R.Index = 0; R.StepStart = Elapsed; R.Frames = 0; }
			if (!R.Ops.IsValidIndex(R.Index)) { return true; }
			const FOp& Op = R.Ops[R.Index];
			const double InStep = Elapsed - R.StepStart;
			const auto Next = [&]() { ++R.Index; R.StepStart = Elapsed; R.Frames = 0; };
			const auto Check = [&](const FString& Id, bool bOk, const FString& Detail)
			{
				SpnQa::Check(*this, &Character, Suite, Id, bOk ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail, Detail);
			};
			const bool bSettled = FMath::IsNearlyEqual(R.Door->GetCurrentAngle(), R.Door->GetTargetAngle(), 1.f);
			if (Op.Kind != EOp::Walk) { R.Keys.Set(Character, EKeys::W, false); }

			switch (Op.Kind)
			{
			case EOp::Section:
				SpnQa::Section(Op.Id);
				Next();
				break;
			case EOp::Wait:
				if (InStep >= Op.A) { Next(); }
				break;
			case EOp::Walk:
			{
				const FVector2D To = Op.P - FVector2D(Loc.X, Loc.Y);
				if (To.Size() <= Op.A) { R.Keys.Set(Character, EKeys::W, false); Next(); break; }
				if (InStep > 25.0)
				{
					R.Keys.Set(Character, EKeys::W, false);
					R.Failure = FString::Printf(TEXT("stuck %.0f cm from the waypoint (%.0f, %.0f) at (%.0f, %.0f, %.0f), step %d"), To.Size(), Op.P.X, Op.P.Y, Loc.X, Loc.Y, Loc.Z, R.Index);
					return true;
				}
				Face(Character, Op.P, -10.f);
				R.Keys.Set(Character, EKeys::W, true);
				break;
			}
			case EOp::FaceDoor:
				Face(Character, FVector2D(LeafCenter(*R.Door).X, LeafCenter(*R.Door).Y), 0.f);
				Next();
				break;
			case EOp::PressE:
				Face(Character, FVector2D(LeafCenter(*R.Door).X, LeafCenter(*R.Door).Y), 0.f);
				if (R.Frames == 0 && Character.GetVelocity().Size2D() > 15.f && InStep < 3.0) { break; }
				SpnQa::Key(*PC, EKeys::E, R.Frames == 0);
				if (++R.Frames > 1) { Next(); }
				break;
			case EOp::Turn:
				if (R.Frames == 0) { R.TurnFromYaw = PC->GetControlRotation().Yaw; R.YawBeforeTurn = R.TurnFromYaw; R.Frames = 1; }
				{
					const float Alpha = FMath::Clamp(static_cast<float>(InStep) / FMath::Max(Op.A, 0.01f), 0.f, 1.f);
					const float Yaw = R.TurnFromYaw + Op.P.X * FMath::InterpEaseInOut(0.f, 1.f, Alpha, 2.f);
					PC->SetControlRotation(FRotator(-12.f, Yaw, 0.f));
					R.YawTurned = Yaw - R.YawBeforeTurn;
					if (Alpha >= 1.f) { Next(); }
				}
				break;
			case EOp::Shot:
				CleanShot(Op.Id);
				Next();
				break;
			case EOp::CheckClosed:
			{
				const bool bClosed = !R.Door->IsOpen() && bSettled && FMath::IsNearlyZero(R.Door->GetCurrentAngle(), 1.f);
				Check(Op.A > 0.f ? FString::Printf(TEXT("door_closes_with_e_%d"), ++R.Closes) : FString(TEXT("door_closed_before")), bClosed,
					FString::Printf(TEXT("door open flag %d, leaf angle %.0f (target %.0f)"), R.Door->IsOpen(), R.Door->GetCurrentAngle(), R.Door->GetTargetAngle()));
				Next();
				break;
			}
			case EOp::CheckOpen:
			{
				const bool bOpen = R.Door->IsOpen() && bSettled && FMath::Abs(R.Door->GetCurrentAngle()) > 80.f;
				Check(FString::Printf(TEXT("door_opens_with_e_%d"), ++R.Opens), bOpen,
					FString::Printf(TEXT("E opened the door: leaf angle %.0f (target %.0f), prompt '%s'"), R.Door->GetCurrentAngle(), R.Door->GetTargetAngle(),
						Character.FindComponentByClass<USpnInteractionComponent>() ? *Character.FindComponentByClass<USpnInteractionComponent>()->GetCurrentPrompt() : TEXT("")));
				Next();
				break;
			}
			case EOp::CheckStillOpen:
				Check(TEXT("door_never_closes_through_the_player"), R.Door->IsOpen(),
					FString::Printf(TEXT("E pressed while standing in the doorway at (%.0f, %.0f): door open flag %d, leaf angle %.0f"), Loc.X, Loc.Y, R.Door->IsOpen(), R.Door->GetCurrentAngle()));
				Next();
				break;
			case EOp::CheckAt:
				Check(Op.Id, FVector2D::Distance(FVector2D(Loc.X, Loc.Y), Op.P) <= Op.A && Character.GetCharacterMovement()->IsMovingOnGround(),
					FString::Printf(TEXT("character at (%.0f, %.0f, %.0f), %.0f cm from the doorway centre, on ground %d"), Loc.X, Loc.Y, Loc.Z,
						FVector2D::Distance(FVector2D(Loc.X, Loc.Y), Op.P), Character.GetCharacterMovement()->IsMovingOnGround()));
				Next();
				break;
			case EOp::CheckInside:
			{
				// Inside = east of the door plane and standing on the interior floor (plinth 5 cm: capsule centre about Z 101).
				const bool bOk = Loc.X > Doorway.X + 20.f && FVector2D::Distance(FVector2D(Loc.X, Loc.Y), Op.P) <= Op.A && FMath::Abs(Loc.Z - (StandZ + 4.f)) <= 20.f
					&& Character.GetCharacterMovement()->IsMovingOnGround();
				Check(Op.Id, bOk, FString::Printf(TEXT("character at (%.0f, %.0f, %.0f), %.0f cm from the waypoint, on ground %d"), Loc.X, Loc.Y, Loc.Z,
					FVector2D::Distance(FVector2D(Loc.X, Loc.Y), Op.P), Character.GetCharacterMovement()->IsMovingOnGround()));
				Next();
				break;
			}
			case EOp::CheckTurned:
				Check(TEXT("turned_around_inside"), FMath::Abs(R.YawTurned) >= 170.f && Loc.X > Doorway.X + 20.f,
					FString::Printf(TEXT("view turned %.0f degrees while standing inside at (%.0f, %.0f)"), R.YawTurned, Loc.X, Loc.Y));
				Next();
				break;
			case EOp::CheckOutside:
				Check(FString::Printf(TEXT("left_the_building_%d"), ++R.Exits), Loc.X < Doorway.X - 60.f && FMath::Abs(Loc.Z - StandZ) <= 20.f,
					FString::Printf(TEXT("character back outside at (%.0f, %.0f, %.0f)"), Loc.X, Loc.Y, Loc.Z));
				Next();
				break;
			case EOp::OverviewStart:
				if (!R.bCameraBlending)
				{
					R.CameraFrom = FVector(-12900.f, -9200.f, 1900.f);
					R.CameraTo = FVector(-12900.f, -2400.f, 1900.f);
					FActorSpawnParameters Params;
					Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
					ACameraActor* Camera = Character.GetWorld()->SpawnActor<ACameraActor>(R.CameraFrom, FRotator(-16.f, 22.f, 0.f), Params);
					if (Camera == nullptr) { R.Failure = TEXT("could not spawn the overview camera"); return true; }
					R.Camera = Camera;
					PC->SetViewTargetWithBlend(Camera, 1.0f);
					R.bCameraBlending = true;
					R.StepStart = Elapsed;
				}
				else if (InStep >= 1.6) { Next(); }
				break;
			case EOp::OverviewRun:
				if (ACameraActor* Camera = R.Camera.Get())
				{
					const float Alpha = FMath::Clamp(static_cast<float>(InStep) / Op.A, 0.f, 1.f);
					Camera->SetActorLocation(FMath::Lerp(R.CameraFrom, R.CameraTo, Alpha));
				}
				if (InStep >= Op.A) { Next(); }
				break;
			case EOp::OverviewEnd:
				PC->SetViewTargetWithBlend(&Character, 0.8f);
				if (ACameraActor* Camera = R.Camera.Get()) { Camera->Destroy(); }
				Next();
				return false;
			}
			return !R.Ops.IsValidIndex(R.Index);
		},
		[this, Run](ASupernaturalProtCharacter& Character, bool bTimedOut)
		{
			Run->Keys.ReleaseAll(Character);
			if (!Run->Failure.IsEmpty() || bTimedOut)
			{
				SpnQa::Check(*this, &Character, Suite, TEXT("run"), SpnQa::EStatus::Blocked,
					Run->Failure.IsEmpty() ? FString(TEXT("timeout of the building access run")) : Run->Failure);
				return;
			}
			SpnQa::Check(*this, &Character, Suite, TEXT("run"), SpnQa::EStatus::Pass, FString::Printf(TEXT("all %d steps of the visit ran"), Run->Ops.Num()));
		}));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));
	return true;
}

#endif
