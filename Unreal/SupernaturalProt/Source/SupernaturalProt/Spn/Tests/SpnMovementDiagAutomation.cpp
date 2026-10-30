#include "HAL/FileManager.h"
#include "HAL/IConsoleManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Components/CapsuleComponent.h"
#include "Spn/Tests/SpnMovementTestSupport.h"

#if WITH_DEV_AUTOMATION_TESTS

// Movement investigation (after Night Build 01): DIAGNOSTIC runs, not regression tests (prefix SpnDiag, never part of the SpnGame filter). Each run walks
// the real character with movement input into one geometry case of the movement test map, with the engine-event trace (spn.MoveTrace) on, and writes the
// trace to Saved/SpnDiag/<case>.txt. They assert nothing about the outcome; they only report it.

using namespace SpnMovementTest;

namespace
{
	constexpr EAutomationTestFlags DiagFlags = EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter;
	constexpr float DiagObstacleHeights[] = {20.f, 25.f, 30.f, 40.f, 45.f, 50.f, 60.f, 75.f, 80.f, 100.f, 120.f, 150.f};
	constexpr float DiagThinX = 3000.f;
	constexpr float DiagStairRises[] = {15.f, 18.f, 22.f, 25.f, 30.f, 40.f, 50.f};

	float DiagObstacleY(int32 Index) { return (Index - (static_cast<int32>(UE_ARRAY_COUNT(DiagObstacleHeights)) - 1) * 0.5f) * 600.f; }
	float DiagStairY(int32 Index) { return 4000.f + Index * 500.f; }

	struct FDiagRun
	{
		bool bJumped = false;
		float MaxZ = -1e9f;
		TArray<FString> Lines;
	};

	void SetTrace(bool bOn)
	{
		if (IConsoleVariable* Var = IConsoleManager::Get().FindConsoleVariable(TEXT("spn.MoveTrace")))
		{
			Var->Set(bOn ? 1 : 0, ECVF_SetByCode);
		}
	}

	/** Walks along +X from Start for Seconds with the given gait; optional jump when X passes JumpAtX. Writes the trace to Saved/SpnDiag/<Name>.txt. */
	void AddDiagRun(FAutomationTestBase& Test, const FString& Name, const FVector& Start, ESpnGait Gait, float Seconds, float JumpAtX, int32 FloorLimit = -1, float SubStep = -1.f, float Radius = -1.f)
	{
		const TSharedRef<FDiagRun> Run = MakeShared<FDiagRun>();
		ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(Test, Start, 0.f, Seconds,
			[Gait, FloorLimit, SubStep, Radius](ASupernaturalProtCharacter& Character)
			{
				Character.GetCapsuleComponent()->SetCapsuleRadius(Radius > 0.f ? Radius : 42.f); // experiment only; 42 = the character default
				if (SubStep > 0.f)
				{
					Character.GetSpnMovement()->MaxSimulationTimeStep = SubStep;
					Character.GetSpnMovement()->MaxSimulationIterations = 16;
				}
				Character.GetSpnMovement()->SetBaseGait(Gait);
				Character.GetSpnMovement()->Settings.bLimitFloorRiseToStepHeight = FloorLimit != 0; // -1 = default (on)
				Character.GetSpnMovement()->ConsumeMoveTrace();
				SetTrace(true);
			},
			[Run, JumpAtX](ASupernaturalProtCharacter& Character, float Dt, double Elapsed)
			{
				Character.DoMove(0.f, 1.f);
				if (JumpAtX > -1e8f && !Run->bJumped && Character.GetActorLocation().X > JumpAtX && !Character.GetSpnMovement()->IsFalling())
				{
					Character.Jump();
					Run->bJumped = true;
					Run->Lines.Add(TEXT("JUMP pressed"));
				}
				else if (Run->bJumped)
				{
					Character.StopJumping();
				}
				const FVector P = Character.GetActorLocation();
				Run->MaxZ = FMath::Max(Run->MaxZ, static_cast<float>(P.Z));
				Run->Lines.Add(FString::Printf(TEXT("FRAME t=%.4f dt=%.4f x=%.2f z=%.2f vx=%.1f vz=%.1f mode=%d"), Elapsed, Dt, P.X, P.Z, Character.GetVelocity().X, Character.GetVelocity().Z,
					static_cast<int32>(Character.GetSpnMovement()->MovementMode)));
				Run->Lines.Append(Character.GetSpnMovement()->ConsumeMoveTrace());
				return false;
			},
			[&Test, Run, Name](ASupernaturalProtCharacter& Character, bool)
			{
				Character.StopJumping();
				Character.GetSpnMovement()->Settings.bLimitFloorRiseToStepHeight = true;
				Character.GetCapsuleComponent()->SetCapsuleRadius(42.f);
				Run->Lines.Append(Character.GetSpnMovement()->ConsumeMoveTrace());
				SetTrace(false);
				const FVector P = Character.GetActorLocation();
				const FString File = FPaths::ProjectSavedDir() / TEXT("SpnDiag") / (Name + TEXT(".txt"));
				FFileHelper::SaveStringArrayToFile(Run->Lines, *File);
				Test.AddInfo(FString::Printf(TEXT("DIAG %s: end x=%.1f z=%.1f maxZ=%.1f mode=%d lines=%d file=%s"), *Name, P.X, P.Z, Run->MaxZ,
					static_cast<int32>(Character.GetSpnMovement()->MovementMode), Run->Lines.Num(), *File));
			}));
		ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.2f));
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnDiagLedgeWalkTest, "SpnDiag.Movement.LedgeWalk", DiagFlags)
bool FSpnDiagLedgeWalkTest::RunTest(const FString&)
{
	if (!RequireMap(*this)) { return false; }
	AutomationOpenMap(MapPackage());
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));
	for (const int32 Index : {1, 2, 3})
	{
		AddDiagRun(*this, FString::Printf(TEXT("ledge_walk_%.0f"), DiagObstacleHeights[Index]), FVector(DiagThinX - 300.f, DiagObstacleY(Index), StandZ), ESpnGait::Jog, 4.f, -1e9f);
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnDiagStairWalkTest, "SpnDiag.Movement.StairWalk", DiagFlags)
bool FSpnDiagStairWalkTest::RunTest(const FString&)
{
	if (!RequireMap(*this)) { return false; }
	AutomationOpenMap(MapPackage());
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));
	for (const int32 Index : {3, 4, 5})
	{
		AddDiagRun(*this, FString::Printf(TEXT("stair_walk_%.0f"), DiagStairRises[Index]), FVector(-9000.f - 200.f, DiagStairY(Index), StandZ), ESpnGait::Jog, 6.f, -1e9f);
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnDiagLateJumpTest, "SpnDiag.Movement.LateJump", DiagFlags)
bool FSpnDiagLateJumpTest::RunTest(const FString&)
{
	if (!RequireMap(*this)) { return false; }
	AutomationOpenMap(MapPackage());
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));
	// the jump key is pressed once the capsule front is 10 cm from the face (as SpnQA.Jump late_jump_*): axis at face - radius 42 - 10
	const float FaceX = DiagThinX - 20.f;
	for (const float R : {34.f, 30.f})
	{
		// the take-off rule stays "capsule front 10 cm before the face", i.e. it moves with the radius
		AddDiagRun(*this, FString::Printf(TEXT("late_jump_45_jog_r%.0f"), R), FVector(DiagThinX - 900.f, DiagObstacleY(4), StandZ), ESpnGait::Jog, 5.f, FaceX - R - 10.f, 1, -1.f, R);
		AddDiagRun(*this, FString::Printf(TEXT("late_jump_40_run_r%.0f"), R), FVector(DiagThinX - 900.f, DiagObstacleY(3), StandZ), ESpnGait::Run, 5.f, FaceX - R - 10.f, 1, -1.f, R);
	}
	for (const float Sub : {0.05f, 1.f / 120.f})
	{
		AddDiagRun(*this, FString::Printf(TEXT("late_jump_45_jog_sub%.0f"), 1.f / Sub), FVector(DiagThinX - 900.f, DiagObstacleY(4), StandZ), ESpnGait::Jog, 5.f, FaceX - 52.f, 1, Sub);
		AddDiagRun(*this, FString::Printf(TEXT("late_jump_40_run_sub%.0f"), 1.f / Sub), FVector(DiagThinX - 900.f, DiagObstacleY(3), StandZ), ESpnGait::Run, 5.f, FaceX - 52.f, 1, Sub);
	}
	for (const int32 Limit : {1, 0})
	{
		AddDiagRun(*this, FString::Printf(TEXT("late_jump_45_jog_limit%d"), Limit), FVector(DiagThinX - 900.f, DiagObstacleY(4), StandZ), ESpnGait::Jog, 5.f, FaceX - 52.f, Limit);
		AddDiagRun(*this, FString::Printf(TEXT("late_jump_40_run_limit%d"), Limit), FVector(DiagThinX - 900.f, DiagObstacleY(3), StandZ), ESpnGait::Run, 5.f, FaceX - 52.f, Limit);
	}
	return true;
}

// Pushing straight into a high wall after a small sideways nudge (what a landing next to a wall can leave behind). Reports the sideways drift along the
// wall over 3 s with the SPN ground steering on and off.
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnDiagWallPushTest, "SpnDiag.Movement.WallPush", DiagFlags)
bool FSpnDiagWallPushTest::RunTest(const FString&)
{
	if (!RequireMap(*this)) { return false; }
	AutomationOpenMap(MapPackage());
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));
	for (const bool bSteering : {true, false})
	{
		for (const float Nudge : {0.f, 30.f})
		{
			struct FPush { bool bNudged = false; double ContactAt = -1.0; float StartY = 0.f; float MaxSide = 0.f; };
			const TSharedRef<FPush> P = MakeShared<FPush>();
			ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(*this, FVector(DiagThinX - 300.f, DiagObstacleY(11), StandZ), 0.f, 5.f,
				[bSteering, P](ASupernaturalProtCharacter& Character)
				{
					Character.GetSpnMovement()->SetBaseGait(ESpnGait::Jog);
					Character.GetSpnMovement()->Settings.bUseSteering = bSteering;
					P->StartY = Character.GetActorLocation().Y;
				},
				[P, Nudge](ASupernaturalProtCharacter& Character, float, double Elapsed)
				{
					Character.DoMove(0.f, 1.f);
					UCharacterMovementComponent* Move = Character.GetCharacterMovement();
					if (P->ContactAt < 0.0 && Character.GetActorLocation().X > DiagThinX - 20.f - 45.f)
					{
						P->ContactAt = Elapsed;
					}
					if (P->ContactAt >= 0.0 && !P->bNudged && Elapsed > P->ContactAt + 0.2)
					{
						Move->Velocity.Y += Nudge; // one small sideways push while pressed against the wall
						P->bNudged = true;
					}
					P->MaxSide = FMath::Max(P->MaxSide, FMath::Abs(static_cast<float>(Move->Velocity.Y)));
					return P->ContactAt >= 0.0 && Elapsed > P->ContactAt + 3.2;
				},
				[this, P, bSteering, Nudge](ASupernaturalProtCharacter& Character, bool)
				{
					AddInfo(FString::Printf(TEXT("DIAG wallpush steering=%d nudge=%.0f: sideways drift %.1f cm in 3 s, max sideways speed %.1f cm/s, end speed %.1f"), bSteering ? 1 : 0, Nudge,
						Character.GetActorLocation().Y - P->StartY, P->MaxSide, Character.GetVelocity().Size2D()));
					Character.GetSpnMovement()->Settings.bUseSteering = true;
				}));
			ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.2f));
		}
	}
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
