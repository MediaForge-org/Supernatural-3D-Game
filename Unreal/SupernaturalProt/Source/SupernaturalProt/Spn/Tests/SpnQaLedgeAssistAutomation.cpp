#include "Components/CapsuleComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "Spn/Tests/SpnMovementTestSupport.h"
#include "Spn/Tests/SpnQaRecorder.h"

#if WITH_DEV_AUTOMATION_TESTS

// SpnQA.LedgeAssist on /Game/SpnTest/Lvl_SpnMovementTestV2, the jump lane (x -12500): the ledge assist of the developer decision 2026-11-14
// (bands up to 40 / 40-120 / 120-180 cm, explicit jump input only). Input is KEY events like the keyboard (W, Shift, Space); obstacles are
// runtime-spawned boxes, never saved. The checks read the outcome from the character (position, speed, movement mode) and the movement
// component's record of the decision (which move, why). Thresholds below are the PLACEHOLDER expectations of this first version.

using namespace SpnMovementTest;

namespace
{
	constexpr float LaneX = -12500.f;
	constexpr float LaneStartY = -12000.f;
	constexpr float CapsuleRadius = 42.f;
	constexpr EAutomationTestFlags QaFlags = EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter;

	AStaticMeshActor* SpawnBox(UWorld& World, const FVector& Center, const FVector& Size)
	{
		AStaticMeshActor* Actor = World.SpawnActor<AStaticMeshActor>(Center, FRotator::ZeroRotator);
		Actor->SetMobility(EComponentMobility::Movable);
		Actor->GetStaticMeshComponent()->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube")));
		Actor->GetStaticMeshComponent()->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
		Actor->SetActorScale3D(Size / 100.f);
		return Actor;
	}

	enum class EOutcome : uint8 { OnTop, Behind, InFront };

	struct FLedgeCase
	{
		const TCHAR* Name;
		ESpnGait Gait;
		bool bSprint;
		bool bStanding;
		float Height;
		float Depth;
		float Gap;          // capsule front to the face when Space is pressed (moving cases), or the standing distance
		bool bPress;        // false: never press Space (no automatic move may happen)
		float CeilingGap;   // > 0: a slab whose underside is this far above the top, over the start and the obstacle
		float YawOffset;    // degrees the run direction is turned away from head-on
		ESpnLedgeMove Expect;
		EOutcome Outcome;
		float MinKeep;      // lowest horizontal speed during the move as a share of the approach speed (moving cases)
		float MinSeconds, MaxSeconds;
		const TCHAR* Reason; // part of the decision's reason that must appear (refusals); empty = not checked
	};

	struct FLedgeRun
	{
		SpnQa::FHeldKeys Keys;
		TArray<AStaticMeshActor*> Boxes;
		float FrontY = 0.f;
		double PressAt = -1.0, EndAt = -1.0;
		int32 CountBefore = 0;
		float ApproachSpeed = 0.f, MinSpeed = 1.0e9f, MaxBottom = -1.0e9f;
		FVector EndLoc = FVector::ZeroVector;
		int32 EndMode = -1;
		bool bWasActive = false;
		TArray<TArray<double>> Trace;
	};
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnQaLedgeAssistTest, "SpnQA.LedgeAssist", QaFlags)

bool FSpnQaLedgeAssistTest::RunTest(const FString& Parameters)
{
	if (!FPackageName::DoesPackageExist(SpnQa::QaMapPath()))
	{
		AddError(*FString::Printf(TEXT("%s does not exist"), *SpnQa::QaMapPath()));
		return false;
	}
	AutomationOpenMap(SpnQa::QaMapPath());
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([] { SpnQa::Section(TEXT("Ledge assist: step on, vault, mantle, climb up; refusals")); return true; }));

	using M = ESpnLedgeMove;
	const FLedgeCase Cases[] = {
		// band 1 (up to 40 cm)
		{TEXT("step_on_35_jog"), ESpnGait::Jog, false, false, 35, 100, 10, true, 0, 0, M::StepOn, EOutcome::OnTop, 0.8f, 0.1f, 0.5f, TEXT("")},
		{TEXT("step_over_35_thin_walk"), ESpnGait::Walk, false, false, 35, 25, 10, true, 0, 0, M::Vault, EOutcome::Behind, 0.8f, 0.1f, 0.8f, TEXT("")},
		// band 2 (40-120 cm)
		{TEXT("mantle_90_standing"), ESpnGait::Jog, false, true, 90, 100, 5, true, 0, 0, M::Mantle, EOutcome::OnTop, 0.f, 0.4f, 1.4f, TEXT("")},
		{TEXT("mantle_110_jog"), ESpnGait::Jog, false, false, 110, 100, 10, true, 0, 0, M::Mantle, EOutcome::OnTop, 0.f, 0.3f, 1.2f, TEXT("")},   // high mantle: the body stops at the face (measured only)
		{TEXT("vault_90_thin_run"), ESpnGait::Run, false, false, 90, 30, 45, true, 0, 0, M::Vault, EOutcome::Behind, 0.5f, 0.2f, 0.9f, TEXT("")},
		// pressed very late (15 cm before the fence at run speed): still a vault, but the body is stopped at the fence first (measured only)
		{TEXT("vault_90_thin_run_late"), ESpnGait::Run, false, false, 90, 30, 15, true, 0, 0, M::Vault, EOutcome::Behind, 0.f, 0.2f, 0.9f, TEXT("")},
		{TEXT("vault_90_thin_standing_refused"), ESpnGait::Jog, false, true, 90, 30, 5, true, 0, 0, M::None, EOutcome::InFront, 0.f, 0.f, 0.f, TEXT("run-up")},
		// band 3 (120-180 cm)
		{TEXT("climb_160_standing"), ESpnGait::Jog, false, true, 160, 100, 5, true, 0, 0, M::ClimbUp, EOutcome::OnTop, 0.f, 1.0f, 2.5f, TEXT("")},
		{TEXT("climb_150_jog"), ESpnGait::Jog, false, false, 150, 100, 10, true, 0, 0, M::ClimbUp, EOutcome::OnTop, 0.f, 1.0f, 2.5f, TEXT("")},
		// refusals: the input stays an ordinary jump, nothing climbs
		{TEXT("too_high_200_standing"), ESpnGait::Jog, false, true, 200, 100, 5, true, 0, 0, M::None, EOutcome::InFront, 0.f, 0.f, 0.f, TEXT("higher")},
		{TEXT("no_press_90_jog"), ESpnGait::Jog, false, false, 90, 100, 10, false, 0, 0, M::None, EOutcome::InFront, 0.f, 0.f, 0.f, TEXT("")},
		{TEXT("pressed_too_far_90_jog"), ESpnGait::Jog, false, false, 90, 100, 100, true, 0, 0, M::None, EOutcome::InFront, 0.f, 0.f, 0.f, TEXT("nothing in front")},   // the probe only looks as far as the reach (58 cm at jog)
		{TEXT("low_ceiling_90_standing"), ESpnGait::Jog, false, true, 90, 100, 5, true, 120, 0, M::None, EOutcome::InFront, 0.f, 0.f, 0.f, TEXT("ceiling")},
		{TEXT("oblique_60deg_90_jog"), ESpnGait::Jog, false, false, 90, 100, 3, true, 0, 60, M::None, EOutcome::InFront, 0.f, 0.f, 0.f, TEXT("oblique")},
	};

	for (const FLedgeCase& Case : Cases)
	{
		const TSharedRef<FLedgeRun> Run = MakeShared<FLedgeRun>();
		const float FrontY = LaneStartY + 2400.f;
		// the oblique run starts beside and close to the box so it meets the face within the box's width
		const FVector Start = Case.bStanding ? FVector(LaneX, FrontY - CapsuleRadius - Case.Gap, StandZ)
			: Case.YawOffset != 0.f ? FVector(LaneX + 450.f, FrontY - 330.f, StandZ) : FVector(LaneX, FrontY - 900.f, StandZ);
		ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(*this, Start, 90.f + Case.YawOffset, 14.f,
			[Run, Case, FrontY](ASupernaturalProtCharacter& Character)
			{
				SpnQa::Section(FString::Printf(TEXT("Ledge assist: %s"), Case.Name));
				Character.GetSpnMovement()->SetBaseGait(Case.Gait);
				Run->FrontY = FrontY;
				UWorld& World = *Character.GetWorld();
				Run->Boxes.Add(SpawnBox(World, FVector(LaneX, FrontY + Case.Depth / 2.f, Case.Height / 2.f), FVector(600.f, Case.Depth, Case.Height)));
				if (Case.CeilingGap > 0.f)
				{
					const float Bottom = Case.Height + Case.CeilingGap;
					Run->Boxes.Add(SpawnBox(World, FVector(LaneX, FrontY - 100.f + Case.Depth / 2.f, Bottom + 10.f), FVector(600.f, Case.Depth + 200.f, 20.f)));
				}
				Run->CountBefore = Character.GetSpnMovement()->GetLedgeMoveCount();
			},
			[Run, Case](ASupernaturalProtCharacter& Character, float, double Elapsed)
			{
				USpnCharacterMovementComponent* Move = Character.GetSpnMovement();
				const FVector Loc = Character.GetActorLocation();
				const float Gap = Run->FrontY - (Loc.Y + CapsuleRadius);
				const bool bGround = Move->IsMovingOnGround();
				const float Speed = HorizontalSpeed(Character);
				if (Run->PressAt < 0.0)
				{
					Run->Keys.Set(Character, EKeys::W, !Case.bStanding);
					Run->Keys.Set(Character, EKeys::LeftShift, Case.bSprint && !Case.bStanding);
					const bool bAtPlace = Case.bStanding ? Elapsed > 0.6 : (Gap <= Case.Gap && Elapsed > 1.0);
					if (bAtPlace && bGround)
					{
						Run->PressAt = Elapsed;
						Run->ApproachSpeed = Speed;
						if (Case.bPress) { Run->Keys.Set(Character, EKeys::SpaceBar, true); }
					}
					return Elapsed > 12.0;
				}
				const double Since = Elapsed - Run->PressAt;
				if (Since > 0.1) { Run->Keys.Set(Character, EKeys::SpaceBar, false); }
				const bool bActive = Move->IsLedgeAssisting();
				if (bActive)
				{
					Run->bWasActive = true;
					Run->MinSpeed = FMath::Min(Run->MinSpeed, Speed);
				}
				Run->MaxBottom = FMath::Max(Run->MaxBottom, static_cast<float>(Loc.Z) - Character.GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
				if (Run->Trace.Num() < 300 && Since < 3.0)
				{
					Run->Trace.Add({Since, Gap, Loc.Z - StandZ, Speed, Character.GetVelocity().Z, static_cast<double>(Move->MovementMode.GetValue()), bActive ? 1.0 : 0.0});
				}
				if (Run->bWasActive && !bActive && Run->EndAt < 0.0)
				{
					Run->EndAt = Since;
					Run->EndLoc = Loc;
					Run->EndMode = static_cast<int32>(Move->MovementMode.GetValue());
					Run->Keys.Set(Character, EKeys::W, false);   // stop where the move left the character, so the result can be read
					Run->Keys.Set(Character, EKeys::LeftShift, false);
				}
				if (Run->EndAt >= 0.0) { return Since - Run->EndAt > 0.8; }
				if (!Run->bWasActive && Since > 1.6)
				{
					Run->EndLoc = Loc;
					return true;
				}
				return Since > 6.0;
			},
			[this, Run, Case](ASupernaturalProtCharacter& Character, bool bTimedOut)
			{
				Run->Keys.ReleaseAll(Character);
				USpnCharacterMovementComponent* Move = Character.GetSpnMovement();
				const FSpnLedgeRecord& Record = Move->GetLastLedgeRecord();
				const int32 Moves = Move->GetLedgeMoveCount() - Run->CountBefore;
				const FVector Final = Character.GetActorLocation();
				const float HalfHeight = Character.GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
				const float FinalBottom = static_cast<float>(Final.Z) - HalfHeight;
				SpnQa::Trace(TEXT("LedgeAssist"), Case.Name, {TEXT("t"), TEXT("gap"), TEXT("z"), TEXT("speed"), TEXT("vz"), TEXT("mode"), TEXT("assist")}, Run->Trace);
				for (AStaticMeshActor* Box : Run->Boxes) { if (Box != nullptr) { Box->Destroy(); } }
				if (Run->PressAt < 0.0)
				{
					SpnQa::Check(*this, &Character, TEXT("LedgeAssist"), Case.Name, SpnQa::EStatus::Blocked, TEXT("never reached the press point"));
					return;
				}
				const ESpnLedgeMove Seen = Moves > 0 ? Record.Move : ESpnLedgeMove::None;
				const bool bOnTop = Final.Y > Run->FrontY + CapsuleRadius - 5.f && Final.Y < Run->FrontY + Case.Depth + CapsuleRadius && FMath::Abs(FinalBottom - Case.Height) < 8.f;
				const bool bBehind = Final.Y > Run->FrontY + Case.Depth + CapsuleRadius && FMath::Abs(FinalBottom) < 8.f;
				// never got up onto / over it: back on the floor and never higher than an ordinary jump against the face would take it
				const bool bInFront = FMath::Abs(FinalBottom) < 8.f && Run->MaxBottom < Case.Height - 20.f && !bBehind;
				const float Keep = Run->ApproachSpeed > 20.f && Run->bWasActive ? Run->MinSpeed / Run->ApproachSpeed : 1.f;
				const FString Facts = FString::Printf(TEXT("move %s (%s, record: %s; %d move(s)), measured top %.0f cm, depth %.0f cm, face %.0f cm ahead at the press, approach %.0f cm/s, "
					"lowest speed during the move %.0f%%, planned %.2f s, took %.2f s (%s), final position %.0f cm past the face, feet at %.0f cm, mode after the move %d"),
					SpnLedgeAssist::MoveName(Seen), Moves > 0 ? *Record.Reason : TEXT("no move started"), *Record.Reason, Moves, Record.Height, Record.Depth, Record.FaceDistance, Run->ApproachSpeed,
					Keep * 100.f, Record.PlannedSeconds, Record.ActualSeconds, Record.bCompleted ? TEXT("completed") : *Record.EndReason, Final.Y - Run->FrontY, FinalBottom, Run->EndMode);
				FString Why;
				if (Seen != Case.Expect) { Why = FString::Printf(TEXT("expected %s"), SpnLedgeAssist::MoveName(Case.Expect)); }
				else if (Case.Expect != ESpnLedgeMove::None && Moves != 1) { Why = TEXT("not exactly one move per press"); }
				else if (Case.Expect != ESpnLedgeMove::None && !Record.bCompleted) { Why = TEXT("the move did not complete"); }
				else if (Case.Outcome == EOutcome::OnTop && !bOnTop) { Why = TEXT("not standing on top afterwards"); }
				else if (Case.Outcome == EOutcome::Behind && !bBehind) { Why = TEXT("not on the floor behind the obstacle"); }
				else if (Case.Outcome == EOutcome::InFront && !bInFront) { Why = TEXT("got onto or over the obstacle without an allowed move"); }
				else if (Case.Expect != ESpnLedgeMove::None && (Record.ActualSeconds < Case.MinSeconds || Record.ActualSeconds > Case.MaxSeconds))
				{
					Why = FString::Printf(TEXT("duration outside %.1f..%.1f s"), Case.MinSeconds, Case.MaxSeconds);
				}
				else if (Case.Expect != ESpnLedgeMove::None && !Case.bStanding && Keep < Case.MinKeep) { Why = FString::Printf(TEXT("kept less than %.0f%% of the approach speed"), Case.MinKeep * 100.f); }
				else if (Case.Reason[0] != 0 && !Record.Reason.Contains(Case.Reason)) { Why = FString::Printf(TEXT("refused for another reason than '%s'"), Case.Reason); }
				else if (bTimedOut) { Why = TEXT("timed out"); }
				SpnQa::Check(*this, &Character, TEXT("LedgeAssist"), Case.Name, Why.IsEmpty() ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail, (Why.IsEmpty() ? FString() : Why + TEXT(". ")) + Facts);
			}));
		ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.3f));
	}
	return true;
}

#endif
