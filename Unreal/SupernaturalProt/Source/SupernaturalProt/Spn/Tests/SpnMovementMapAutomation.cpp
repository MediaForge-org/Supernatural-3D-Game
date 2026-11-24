#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Level.h"
#include "Engine/StaticMeshActor.h"
#include "EngineUtils.h"
#include "GameFramework/SpringArmComponent.h"
#include "Spn/SpnPlayAreaGuard.h"
#include "Spn/Tests/SpnMovementTestSupport.h"
#include "Spn/SpnTestDoor.h"

#if WITH_DEV_AUTOMATION_TESTS

// M2-A / M2-A.1 movement tests in /Game/SpnTest/Lvl_SpnMovementTest (Docs/Proposals/0011). They play the map (PIE in the editor, or -game):
//   UnrealEditor-Cmd.exe <uproject> -ExecCmds="Automation RunTests SpnGame.Movement;Quit" -unattended -nullrhi
// The map comes from Tools/create_spn_movement_test_map.py; without it these tests fail with a message that says so.
// Tolerances are wide on purpose: automation frame times vary. Where the outcome depends on engine details that could not be
// checked without an engine (a jump over a 30-45 cm ledge, the 25 cm step limit itself), the test REPORTS the result with AddInfo
// instead of asserting it. WRITTEN WITHOUT AN ENGINE, NOT YET RUN: see the handover document.
// The constants below are compared with Tools/spn_movement_map_layout.py by Tools/test_spn_movement_map_layout.py.

using namespace SpnMovementTest;

#define SPN_MOVEMENT_MAP_TEST_FLAGS (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)

namespace
{
	// Layout constants (Tools/spn_movement_map_layout.py).
	constexpr float ExpectedJumpHeight = 45.f;
	constexpr float ExpectedStepHeight = 25.f;
	constexpr float JumpTolerance = 7.f;
	constexpr float ObstacleHeights[] = {20.f, 25.f, 30.f, 40.f, 45.f, 50.f, 60.f, 75.f, 80.f, 100.f, 120.f, 150.f};
	constexpr float ObstacleSpacing = 600.f;
	constexpr float ThinObstacleX = 3000.f;
	constexpr float ThinObstacleDepth = 40.f;
	constexpr float RampAngles[] = {5.f, 10.f, 15.f, 20.f, 30.f, 40.f, 50.f};
	constexpr float RampStartX = -9000.f;
	constexpr float RampLength = 1500.f;
	constexpr float RampSpacing = 700.f;
	constexpr float StairRises[] = {15.f, 18.f, 22.f, 25.f, 30.f, 40.f, 50.f};
	constexpr int32 StairSteps = 10;
	constexpr float StairStartX = -9000.f;
	constexpr float StairFirstY = 4000.f;
	constexpr float StairSpacing = 500.f;
	constexpr float LaneX = -12500.f;
	constexpr float LaneStartY = -12000.f;
	constexpr float WalkableAngleLimit = 44.8f;
	constexpr int32 ExpectedPrimitiveCount = 393;
	constexpr int32 MinimumResetPoints = 14;

	// A height within this distance of the step height or the jump apex is a boundary case: reported, not asserted.
	constexpr float BoundaryMargin = 2.f;

	enum class EVerdict : uint8
	{
		MustPass,
		MustBlock,
		Report
	};

	/** What the test may assert about walking up a ledge of this height (step limit from the settings, jump apex likewise). */
	EVerdict WalkVerdict(const FSpnMovementSettings& S, float Height)
	{
		if (Height <= S.MaxStepHeight - BoundaryMargin)
		{
			return EVerdict::MustPass;
		}
		return Height >= S.MaxStepHeight + BoundaryMargin ? EVerdict::MustBlock : EVerdict::Report;
	}

	constexpr float JumpMomentumAllowance = 6.f;   // cm above the apex that momentum and the capsule rounding can add (measured, see JumpVerdict)

	/** Same for a run-up with jump attempts: only a ledge clearly above the apex must block; the band in between is reported. */
	EVerdict JumpVerdict(const FSpnMovementSettings& S, float Height)
	{
		if (Height <= S.MaxStepHeight - BoundaryMargin)
		{
			return EVerdict::MustPass;
		}
		// + JumpMomentumAllowance: measured in M2A4.1 (SpnQA.Jump): at run speed (550 cm/s) a 50 cm ledge is cleared with the momentum, jog is blocked at 50, everything from 60 cm blocks.
		return Height >= S.JumpHeight + BoundaryMargin + 1.f + JumpMomentumAllowance ? EVerdict::MustBlock : EVerdict::Report;
	}

	/** State shared between the lambdas of one scenario. */
	struct FRunData
	{
		float MaxZ = -1.0e9f;
		float GroundZ = 0.f;
		float SpeedSum = 0.f;
		int32 SpeedSamples = 0;
		float PreviousSpeed = 0.f;
		float MaxSpeed = 0.f;
		float WorstAccelJump = 0.f; // largest speed gain beyond what the acceleration allows, cm/s
		float WorstBrakeRise = 0.f; // largest speed gain while braking, cm/s
		float StopTime = -1.f;
		float MinSpeed = 0.f;
		float ArmUsed = 0.f;
		int32 Phase = 0;
		double PhaseStart = 0.0;
		int32 TakeOffs = 0;
		bool bWasFalling = false;
		FVector End = FVector::ZeroVector;
		bool bTimedOut = false;
		bool bDone = false;

		// Jump scenarios
		float TakeOffSpeed = 0.f;
		float MinAirSpeed = TNumericLimits<float>::Max();
		float MaxAirSpeed = 0.f;
		double AirStart = -1.0;
		double AirTime = 0.0;
		float JumpZVelocity = 0.f;
		float GravityMagnitude = 0.f;
		TArray<ESpnMovementState> States;

		// Sprint-turn scenario
		ESpnGait GaitAfterTurn = ESpnGait::Walk;
		bool bGaitSampled = false;

		// Camera scenario
		int32 Samples = 0;
		int32 Violations = 0;

		void AddState(ESpnMovementState State)
		{
			if (States.IsEmpty() || States.Last() != State)
			{
				States.Add(State);
			}
		}
	};

	/** True when `Wanted` appears in `Sequence` in this order (other states may lie in between). */
	bool ContainsInOrder(const TArray<ESpnMovementState>& Sequence, std::initializer_list<ESpnMovementState> Wanted)
	{
		int32 Next = 0;
		const ESpnMovementState* WantedData = Wanted.begin();
		for (const ESpnMovementState State : Sequence)
		{
			if (Next < static_cast<int32>(Wanted.size()) && State == WantedData[Next])
			{
				++Next;
			}
		}
		return Next == static_cast<int32>(Wanted.size());
	}

	struct FJumpOptions
	{
		FVector Start = FVector(0.f, 0.f, StandZ);
		float Yaw = 0.f;
		float Stick = 0.f;     // forward input held before and during the jump
		ESpnGait Base = ESpnGait::Jog;
		bool bSprint = false;
		float SettleSeconds = 0.8f;
		bool bSpamJump = false; // Jump pressed all the time, never released
		float ObservationSeconds = 1.5f;
	};

	/** Settles (standing or moving), then calls Jump once; records apex, air time, speeds and the movement states. */
	void AddJumpScenario(FAutomationTestBase& Test, const TSharedRef<FRunData>& Data, const FJumpOptions& Options)
	{
		ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(Test, Options.Start, Options.Yaw, Options.SettleSeconds + Options.ObservationSeconds + 8.f,
			[Options](ASupernaturalProtCharacter& Character)
			{
				Character.GetSpnMovement()->SetBaseGait(Options.Base);
				Character.GetSpnMovement()->SetSprintRequested(Options.bSprint);
			},
			[Data, Options](ASupernaturalProtCharacter& Character, float, double Elapsed)
			{
				USpnCharacterMovementComponent* Movement = Character.GetSpnMovement();
				if (Options.Stick > 0.f)
				{
					Character.DoMove(0.f, Options.Stick);
				}
				if (Data->Phase == 0)
				{
					if (Elapsed > Options.SettleSeconds && Movement->IsMovingOnGround())
					{
						Data->GroundZ = Character.GetActorLocation().Z;
						Data->MaxZ = Data->GroundZ;
						Data->TakeOffSpeed = HorizontalSpeed(Character);
						Data->JumpZVelocity = Movement->JumpZVelocity;
						Data->GravityMagnitude = -Movement->GetGravityZ();
						Data->AddState(Movement->GetMovementState());
						Data->Phase = 1;
						Data->PhaseStart = Elapsed;
						Character.Jump();
					}
					return false;
				}
				if (!Options.bSpamJump)
				{
					Character.StopJumping(); // a single press
				}
				Data->MaxZ = FMath::Max(Data->MaxZ, Character.GetActorLocation().Z);
				const bool bFalling = Movement->IsFalling();
				if (bFalling && !Data->bWasFalling && Character.GetVelocity().Z > 0.f)
				{
					++Data->TakeOffs;
				}
				Data->bWasFalling = bFalling;
				if (bFalling)
				{
					if (Data->AirStart < 0.0)
					{
						Data->AirStart = Elapsed;
					}
					Data->MinAirSpeed = FMath::Min(Data->MinAirSpeed, HorizontalSpeed(Character));
					Data->MaxAirSpeed = FMath::Max(Data->MaxAirSpeed, HorizontalSpeed(Character));
				}
				else if (Data->AirStart >= 0.0 && Data->AirTime <= 0.0)
				{
					Data->AirTime = Elapsed - Data->AirStart;
				}
				Data->AddState(Movement->GetMovementState());
				return Elapsed - Data->PhaseStart > Options.ObservationSeconds;
			},
			[Data](ASupernaturalProtCharacter& Character, bool)
			{
				Character.StopJumping();
				Data->bDone = true;
			}));
	}

	/** True when something that blocks the camera channel overlaps a small sphere at the camera. */
	bool CameraOverlapsGeometry(const ASupernaturalProtCharacter& Character)
	{
		const UCameraComponent* Camera = Character.GetFollowCamera();
		const UWorld* World = Character.GetWorld();
		if (Camera == nullptr || World == nullptr)
		{
			return false;
		}
		FCollisionQueryParams Params(SCENE_QUERY_STAT(SpnCameraClearance), /*bTraceComplex*/ false, &Character);
		return World->OverlapBlockingTestByChannel(Camera->GetComponentLocation(), FQuat::Identity, ECC_Camera, FCollisionShape::MakeSphere(4.f), Params);
	}

	float ObstacleY(int32 Index)
	{
		return (Index - (static_cast<int32>(UE_ARRAY_COUNT(ObstacleHeights)) - 1) * 0.5f) * ObstacleSpacing;
	}

	float RampY(int32 Index)
	{
		return (Index - (static_cast<int32>(UE_ARRAY_COUNT(RampAngles)) - 1) * 0.5f) * RampSpacing;
	}

	float StairY(int32 Index)
	{
		return StairFirstY + Index * StairSpacing;
	}

	/** Reports an outcome that the test deliberately does not judge. */
	void ReportCase(FAutomationTestBase& Test, const FString& What, bool bCrossed)
	{
		Test.AddInfo(FString::Printf(TEXT("MEASURED (not asserted): %s -> %s"), *What, bCrossed ? TEXT("crossed") : TEXT("blocked")));
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnMovementJumpTest, "SpnGame.Movement.Jump", SPN_MOVEMENT_MAP_TEST_FLAGS)

bool FSpnMovementJumpTest::RunTest(const FString& Parameters)
{
	if (!RequireMap(*this))
	{
		return false;
	}
	AutomationOpenMap(MapPackage());
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));

	// A single standing jump reaches the configured apex, takes the expected time and goes through the expected states.
	const TSharedRef<FRunData> Single = MakeShared<FRunData>();
	AddJumpScenario(*this, Single, FJumpOptions());
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, Single]
	{
		const float Apex = Single->MaxZ - Single->GroundZ;
		TestTrue(TEXT("the jump scenario ran"), Single->bDone);
		TestTrue(FString::Printf(TEXT("vertical jump height is %.0f cm +- %.0f (measured %.1f)"), ExpectedJumpHeight, JumpTolerance, Apex),
			FMath::Abs(Apex - ExpectedJumpHeight) <= JumpTolerance);
		TestEqual(TEXT("one take-off"), Single->TakeOffs, 1);

		// The launch velocity follows from the apex and the real gravity: v = sqrt(2 g h).
		const float ExpectedLaunch = FMath::Sqrt(2.f * Single->GravityMagnitude * ExpectedJumpHeight);
		TestTrue(FString::Printf(TEXT("JumpZVelocity %.1f matches sqrt(2 g h) = %.1f for g = %.0f"), Single->JumpZVelocity, ExpectedLaunch, Single->GravityMagnitude),
			FMath::IsNearlyEqual(Single->JumpZVelocity, ExpectedLaunch, 1.5f));
		// Flight time of a jump back to the same height: 2 v / g (about 0.61 s).
		const double ExpectedAir = 2.0 * Single->JumpZVelocity / FMath::Max(Single->GravityMagnitude, 1.f);
		TestTrue(FString::Printf(TEXT("air time %.2f s is close to 2v/g = %.2f s"), Single->AirTime, ExpectedAir), FMath::Abs(Single->AirTime - ExpectedAir) <= 0.15);
		TestTrue(TEXT("standing jump: no horizontal drift in the air"), Single->MaxAirSpeed < 15.f);

		TestTrue(TEXT("states in order: Jumping, Falling, Landing, Idle"), ContainsInOrder(Single->States, {ESpnMovementState::Jumping, ESpnMovementState::Falling, ESpnMovementState::Landing, ESpnMovementState::Idle}));
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.3f));

	// Jump pressed all the time: no double jump in the air and no automatic second jump after landing.
	const TSharedRef<FRunData> Held = MakeShared<FRunData>();
	FJumpOptions HeldOptions;
	HeldOptions.bSpamJump = true;
	HeldOptions.ObservationSeconds = 2.5f;
	AddJumpScenario(*this, Held, HeldOptions);
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, Held]
	{
		const float Apex = Held->MaxZ - Held->GroundZ;
		TestTrue(FString::Printf(TEXT("held jump key: apex stays %.0f cm +- %.0f (measured %.1f)"), ExpectedJumpHeight, JumpTolerance, Apex),
			FMath::Abs(Apex - ExpectedJumpHeight) <= JumpTolerance);
		TestEqual(TEXT("held jump key: exactly one take-off (no double jump, no bunny hop)"), Held->TakeOffs, 1);
		return true;
	}));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnMovementJumpMovingTest, "SpnGame.Movement.JumpMoving", SPN_MOVEMENT_MAP_TEST_FLAGS)

bool FSpnMovementJumpMovingTest::RunTest(const FString& Parameters)
{
	if (!RequireMap(*this))
	{
		return false;
	}
	AutomationOpenMap(MapPackage());
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));

	// Jumping at jog, run and sprint speed: same apex, the take-off momentum is kept, nothing is gained in the air.
	struct FCase
	{
		const TCHAR* Name;
		ESpnGait Base;
		bool bSprint;
		ESpnGait Gait; // for the expected top speed
	};
	const FCase Cases[] = {
		{TEXT("jog"), ESpnGait::Jog, false, ESpnGait::Jog},
		{TEXT("run"), ESpnGait::Run, false, ESpnGait::Run},
		{TEXT("sprint"), ESpnGait::Jog, true, ESpnGait::Sprint},
	};
	for (const FCase& Case : Cases)
	{
		const TSharedRef<FRunData> Data = MakeShared<FRunData>();
		FJumpOptions Options;
		Options.Start = FVector(LaneX, LaneStartY, StandZ);
		Options.Yaw = 90.f;
		Options.Stick = 1.f;
		Options.Base = Case.Base;
		Options.bSprint = Case.bSprint;
		Options.SettleSeconds = 4.f; // steady speed
		AddJumpScenario(*this, Data, Options);
		ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, Data, Case]
		{
			const FSpnMovementSettings Settings;
			const float TopSpeed = SpnMovement::GaitSpeed(Settings, Case.Gait);
			const float Apex = Data->MaxZ - Data->GroundZ;
			TestTrue(FString::Printf(TEXT("%s jump: scenario ran"), Case.Name), Data->bDone);
			TestTrue(FString::Printf(TEXT("%s jump: take-off speed %.0f is the gait speed %.0f"), Case.Name, Data->TakeOffSpeed, TopSpeed), Data->TakeOffSpeed > TopSpeed * 0.9f);
			TestTrue(FString::Printf(TEXT("%s jump: apex %.1f cm is the configured height, not higher because of the speed"), Case.Name, Apex),
				FMath::Abs(Apex - ExpectedJumpHeight) <= JumpTolerance);
			TestTrue(FString::Printf(TEXT("%s jump: momentum is kept in the air (slowest %.0f of %.0f)"), Case.Name, Data->MinAirSpeed, Data->TakeOffSpeed),
				Data->MinAirSpeed >= Data->TakeOffSpeed * 0.9f);
			TestTrue(FString::Printf(TEXT("%s jump: no speed gain in the air (fastest %.0f, take-off %.0f)"), Case.Name, Data->MaxAirSpeed, Data->TakeOffSpeed),
				Data->MaxAirSpeed <= FMath::Max(Data->TakeOffSpeed, TopSpeed) * 1.03f + 5.f);
			TestEqual(FString::Printf(TEXT("%s jump: one take-off"), Case.Name), Data->TakeOffs, 1);
			TestTrue(FString::Printf(TEXT("%s jump: states Jumping, Falling, Landing"), Case.Name), ContainsInOrder(Data->States, {ESpnMovementState::Jumping, ESpnMovementState::Falling, ESpnMovementState::Landing}));
			return true;
		}));
		ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.3f));
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnMovementGaitsTest, "SpnGame.Movement.Gaits", SPN_MOVEMENT_MAP_TEST_FLAGS)

bool FSpnMovementGaitsTest::RunTest(const FString& Parameters)
{
	if (!RequireMap(*this))
	{
		return false;
	}
	AutomationOpenMap(MapPackage());
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));

	struct FGaitCase
	{
		const TCHAR* Name;
		ESpnGait Base;
		bool bSprint;
		ESpnGait Expected;
		float Stick; // analog input magnitude
	};
	const FGaitCase Cases[] = {
		{TEXT("walk"), ESpnGait::Walk, false, ESpnGait::Walk, 1.f},
		{TEXT("jog"), ESpnGait::Jog, false, ESpnGait::Jog, 1.f},
		{TEXT("run"), ESpnGait::Run, false, ESpnGait::Run, 1.f},
		{TEXT("sprint"), ESpnGait::Jog, true, ESpnGait::Sprint, 1.f},
		{TEXT("jog, half stick"), ESpnGait::Jog, false, ESpnGait::Jog, 0.5f},
		{TEXT("run, quarter stick"), ESpnGait::Run, false, ESpnGait::Run, 0.25f},
	};

	// All runs start on the speed lane (a free strip along +Y; the 4.5 s sprint covers about 30 m of its 260 m).
	for (const FGaitCase& Case : Cases)
	{
		const TSharedRef<FRunData> Data = MakeShared<FRunData>();
		ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(*this, FVector(LaneX, LaneStartY, StandZ), 90.f, 14.f,
			[Case](ASupernaturalProtCharacter& Character)
			{
				USpnCharacterMovementComponent* Movement = Character.GetSpnMovement();
				Movement->SetBaseGait(Case.Base);
				Movement->SetSprintRequested(Case.bSprint);
			},
			[this, Data, Case](ASupernaturalProtCharacter& Character, float Dt, double Elapsed)
			{
				USpnCharacterMovementComponent* Movement = Character.GetSpnMovement();
				const FSpnMovementSettings& S = Movement->Settings;
				const float Speed = HorizontalSpeed(Character);
				const float Accel = SpnMovement::GaitAcceleration(S, Case.Expected);

				if (Data->Phase == 0) // accelerate and hold
				{
					Character.DoMove(0.f, Case.Stick);
					Data->WorstAccelJump = FMath::Max(Data->WorstAccelJump, (Speed - Data->PreviousSpeed) - Accel * Dt * 1.25f);
					Data->MaxSpeed = FMath::Max(Data->MaxSpeed, Speed);
					if (Elapsed > 3.5) // last second of a 4.5 s run: steady state
					{
						Data->SpeedSum += Speed;
						++Data->SpeedSamples;
					}
					if (Elapsed > 4.5)
					{
						Data->Phase = 1;
						Data->PhaseStart = Elapsed;
					}
				}
				else // release and brake to a stop
				{
					Data->WorstBrakeRise = FMath::Max(Data->WorstBrakeRise, Speed - Data->PreviousSpeed);
					if (Speed < 10.f && Data->StopTime < 0.f)
					{
						Data->StopTime = static_cast<float>(Elapsed - Data->PhaseStart);
						Data->PreviousSpeed = Speed;
						return true;
					}
				}
				Data->PreviousSpeed = Speed;
				return false;
			},
			[this, Data, Case](ASupernaturalProtCharacter& Character, bool bTimedOut)
			{
				const USpnCharacterMovementComponent* Movement = Character.GetSpnMovement();
				const FSpnMovementSettings& S = Movement->Settings;
				const float Target = SpnMovement::GaitSpeed(S, Case.Expected) * Case.Stick;
				const float Average = Data->SpeedSamples > 0 ? Data->SpeedSum / Data->SpeedSamples : 0.f;
				TestFalse(FString::Printf(TEXT("%s: scenario finished (did not time out)"), Case.Name), bTimedOut);
				// The minimum analog speed lifts a very small stick deflection to MinAnalogSpeed.
				const float Floor = FMath::Max(Target, S.MinAnalogSpeed);
				TestTrue(FString::Printf(TEXT("%s: steady speed %.0f cm/s is within 6%% of %.0f"), Case.Name, Average, Floor),
					FMath::Abs(Average - Floor) <= Floor * 0.06f + 4.f);
				TestTrue(FString::Printf(TEXT("%s: never faster than the gait (max %.0f)"), Case.Name, Data->MaxSpeed), Data->MaxSpeed <= SpnMovement::GaitSpeed(S, Case.Expected) * 1.04f);
				TestTrue(FString::Printf(TEXT("%s: no speed jump while accelerating (worst excess %.1f cm/s per frame)"), Case.Name, Data->WorstAccelJump), Data->WorstAccelJump <= 6.f);
				TestTrue(FString::Printf(TEXT("%s: speed never rises while braking (worst %.1f)"), Case.Name, Data->WorstBrakeRise), Data->WorstBrakeRise <= 3.f);
				// Braking from the steady speed at BrakingDeceleration takes speed / brake seconds; allow 60 % + 0.4 s.
				const float ExpectedStop = Floor / S.BrakingDeceleration;
				TestTrue(FString::Printf(TEXT("%s: stops in %.2f s (expected about %.2f s)"), Case.Name, Data->StopTime, ExpectedStop),
					Data->StopTime >= 0.f && Data->StopTime <= ExpectedStop * 1.6f + 0.4f);
			}));
		ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.2f));
	}

	// Sprint released while sprinting: the speed falls to the base gait smoothly, never below it.
	const TSharedRef<FRunData> Release = MakeShared<FRunData>();
	ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(*this, FVector(LaneX, LaneStartY, StandZ), 90.f, 14.f,
		[](ASupernaturalProtCharacter& Character)
		{
			Character.GetSpnMovement()->SetBaseGait(ESpnGait::Jog);
			Character.DoSprintStart();
		},
		[Release](ASupernaturalProtCharacter& Character, float Dt, double Elapsed)
		{
			USpnCharacterMovementComponent* Movement = Character.GetSpnMovement();
			const FSpnMovementSettings& S = Movement->Settings;
			const float Speed = HorizontalSpeed(Character);
			Character.DoMove(0.f, 1.f);
			if (Release->Phase == 0 && Elapsed > 4.5)
			{
				Release->Phase = 1;
				Release->PhaseStart = Elapsed;
				Release->MaxSpeed = Speed;
				Release->PreviousSpeed = Speed;
				Character.DoSprintEnd();
				Release->MinSpeed = TNumericLimits<float>::Max(); // lowest speed seen after the release
			}
			else if (Release->Phase == 1)
			{
				Release->MinSpeed = FMath::Min(Release->MinSpeed, Speed);
				// A drop faster than the braking deceleration allows would be an abrupt change.
				Release->WorstAccelJump = FMath::Max(Release->WorstAccelJump, (Release->PreviousSpeed - Speed) - S.BrakingDeceleration * Dt * 1.5f);
				Release->PreviousSpeed = Speed;
				return Elapsed - Release->PhaseStart > 2.5;
			}
			return false;
		},
		[this, Release](ASupernaturalProtCharacter& Character, bool)
		{
			const FSpnMovementSettings& S = Character.GetSpnMovement()->Settings;
			TestTrue(FString::Printf(TEXT("sprint reached %.0f cm/s before release"), Release->MaxSpeed), Release->MaxSpeed > SpnMovement::GaitSpeed(S, ESpnGait::Run));
			TestTrue(FString::Printf(TEXT("after release the speed settles at the jog speed (lowest %.0f)"), Release->MinSpeed), Release->MinSpeed > SpnMovement::GaitSpeed(S, ESpnGait::Jog) * 0.9f);
			TestTrue(FString::Printf(TEXT("sprint -> jog without an abrupt drop (worst excess %.1f cm/s per frame)"), Release->WorstAccelJump), Release->WorstAccelJump <= 8.f);
		}));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnMovementSprintTurnTest, "SpnGame.Movement.SprintTurn", SPN_MOVEMENT_MAP_TEST_FLAGS)

bool FSpnMovementSprintTurnTest::RunTest(const FString& Parameters)
{
	if (!RequireMap(*this))
	{
		return false;
	}
	AutomationOpenMap(MapPackage());
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));

	// At full sprint the input turns to the side (90 degrees, inside the sustain angle) or nearly back (150 degrees, outside it).
	// M2-A dropped the sprint for the 90 degree case too, which cost a few m/s on every sharp turn (Proposal 0011 finding 4.5).
	struct FTurnCase
	{
		const TCHAR* Name;
		float Right;
		float Forward;
		bool bSprintKept;
	};
	const FTurnCase Cases[] = {
		{TEXT("90 degrees right"), 1.f, 0.f, true},
		{TEXT("90 degrees left"), -1.f, 0.f, true},
		{TEXT("150 degrees"), 0.5f, -0.866f, false},
	};
	for (const FTurnCase& Case : Cases)
	{
		const TSharedRef<FRunData> Data = MakeShared<FRunData>();
		ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(*this, FVector(LaneX, LaneStartY, StandZ), 90.f, 12.f,
			[](ASupernaturalProtCharacter& Character)
			{
				Character.GetSpnMovement()->SetBaseGait(ESpnGait::Jog);
				Character.DoSprintStart();
			},
			[Data, Case](ASupernaturalProtCharacter& Character, float, double Elapsed)
			{
				const float Speed = HorizontalSpeed(Character);
				if (Data->Phase == 0)
				{
					Character.DoMove(0.f, 1.f);
					if (Elapsed > 4.5)
					{
						Data->Phase = 1;
						Data->PhaseStart = Elapsed;
						Data->TakeOffSpeed = Speed; // speed at the start of the turn
						Data->MinSpeed = Speed;
					}
					return false;
				}
				Character.DoMove(Case.Right, Case.Forward);
				const double Since = Elapsed - Data->PhaseStart;
				if (Since > 0.03 && !Data->bGaitSampled) // two frames in: the input has reached the movement, the character has hardly turned
				{
					Data->bGaitSampled = true;
					Data->GaitAfterTurn = Character.GetSpnMovement()->GetEffectiveGait();
				}
				if (Since < 0.8)
				{
					Data->MinSpeed = FMath::Min(Data->MinSpeed, Speed);
				}
				return Since > 1.0;
			},
			[this, Data, Case](ASupernaturalProtCharacter& Character, bool bTimedOut)
			{
				const FSpnMovementSettings& S = Character.GetSpnMovement()->Settings;
				const float SprintSpeed = SpnMovement::GaitSpeed(S, ESpnGait::Sprint);
				TestFalse(FString::Printf(TEXT("%s: scenario finished"), Case.Name), bTimedOut);
				TestTrue(FString::Printf(TEXT("%s: was sprinting before the turn (%.0f of %.0f)"), Case.Name, Data->TakeOffSpeed, SprintSpeed), Data->TakeOffSpeed > SprintSpeed * 0.95f);
				TestTrue(FString::Printf(TEXT("%s: the sprint is %s"), Case.Name, Case.bSprintKept ? TEXT("kept") : TEXT("ended")),
					(Data->GaitAfterTurn == ESpnGait::Sprint) == Case.bSprintKept);
				if (Case.bSprintKept)
				{
					TestTrue(FString::Printf(TEXT("%s: no speed dip during the turn (lowest %.0f of %.0f)"), Case.Name, Data->MinSpeed, Data->TakeOffSpeed),
						Data->MinSpeed >= Data->TakeOffSpeed * 0.9f);
				}
			}));
		ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.2f));
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnMovementIndependenceTest, "SpnGame.Movement.StepJumpIndependence", SPN_MOVEMENT_MAP_TEST_FLAGS)

bool FSpnMovementIndependenceTest::RunTest(const FString& Parameters)
{
	if (!RequireMap(*this))
	{
		return false;
	}
	AutomationOpenMap(MapPackage());
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));

	// On the live character: the step height and the jump height are applied independently, and the defaults are 25 and 45 cm.
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this]
	{
		ASupernaturalProtCharacter* Character = FindCharacter();
		if (!TestNotNull(TEXT("character"), Character))
		{
			return true;
		}
		USpnCharacterMovementComponent* Movement = Character->GetSpnMovement();
		const FSpnMovementSettings Original = Movement->Settings;
		const float Gravity = -Movement->GetGravityZ();

		TestTrue(FString::Printf(TEXT("default step height %.1f cm"), Movement->MaxStepHeight), FMath::IsNearlyEqual(Movement->MaxStepHeight, ExpectedStepHeight, 0.01f));
		TestTrue(FString::Printf(TEXT("default JumpZVelocity %.1f gives %.0f cm"), Movement->JumpZVelocity, ExpectedJumpHeight),
			FMath::IsNearlyEqual(SpnMovement::JumpApexHeight(Movement->JumpZVelocity, Gravity), ExpectedJumpHeight, 0.5f));

		// Change only the step height.
		const float JumpBefore = Movement->JumpZVelocity;
		Movement->Settings.MaxStepHeight = 10.f;
		Movement->ApplySettings();
		TestTrue(TEXT("step height 10 cm is applied"), FMath::IsNearlyEqual(Movement->MaxStepHeight, 10.f, 0.01f));
		TestTrue(TEXT("the jump velocity did not move with the step height"), FMath::IsNearlyEqual(Movement->JumpZVelocity, JumpBefore, 0.01f));

		// Change only the jump height.
		const float StepBefore = Movement->MaxStepHeight;
		Movement->Settings.JumpHeight = 60.f;
		Movement->ApplySettings();
		TestTrue(FString::Printf(TEXT("jump height 60 cm gives apex %.1f"), SpnMovement::JumpApexHeight(Movement->JumpZVelocity, Gravity)),
			FMath::IsNearlyEqual(SpnMovement::JumpApexHeight(Movement->JumpZVelocity, Gravity), 60.f, 0.5f));
		TestTrue(TEXT("the step height did not move with the jump height"), FMath::IsNearlyEqual(Movement->MaxStepHeight, StepBefore, 0.01f));

		// Invalid settings are rejected and change nothing.
		const float JumpValid = Movement->JumpZVelocity;
		Movement->Settings.JumpHeight = -5.f;
		AddExpectedError(TEXT("invalid movement settings"), EAutomationExpectedErrorFlags::Contains, 1);
		Movement->ApplySettings();
		TestTrue(TEXT("invalid settings keep the previous jump velocity"), FMath::IsNearlyEqual(Movement->JumpZVelocity, JumpValid, 0.01f));

		Movement->Settings = Original;
		Movement->ApplySettings();
		return true;
	}));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnMovementObstaclesTest, "SpnGame.Movement.Obstacles", SPN_MOVEMENT_MAP_TEST_FLAGS)

bool FSpnMovementObstaclesTest::RunTest(const FString& Parameters)
{
	if (!RequireMap(*this))
	{
		return false;
	}
	AutomationOpenMap(MapPackage());
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));

	// Walk (and, in a second run, jog with jump attempts) at each thin obstacle. The step limit (25 cm) and the jump apex (45 cm)
	// come from the settings. Asserted: ledges clearly below the step limit are crossed, ledges clearly above it are not crossed by
	// walking, and nothing above the jump apex is crossed even with jumps. In between (jump candidates, the step limit itself, the
	// apex itself) the result depends on collision details; it is REPORTED so the owner can decide on MaxStepHeight.
	for (int32 Index = 0; Index < static_cast<int32>(UE_ARRAY_COUNT(ObstacleHeights)); ++Index)
	{
		const float Height = ObstacleHeights[Index];
		const float Y = ObstacleY(Index);
		const float FarFace = ThinObstacleX + ThinObstacleDepth / 2.f;
		for (const bool bTryJump : {false, true})
		{
			const TSharedRef<FRunData> Data = MakeShared<FRunData>();
			// This test measures the JUMP itself (step limit, apex, no exaggerated jump). Since 2026-11-14 a jump press right in front of an obstacle can
			// start the ledge assist (developer decision: mantle 40-120 cm on explicit input), which is not a jump; it is switched off here so the jump
			// physics stay checked exactly as before. The ledge assist has its own checks (SpnQA.LedgeAssist, Core LedgeAssistTests).
			ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(*this, FVector(ThinObstacleX - 300.f, Y, StandZ), 0.f, 4.f,
				[](ASupernaturalProtCharacter& Character) { Character.GetSpnMovement()->SetBaseGait(ESpnGait::Jog); Character.GetSpnMovement()->LedgeAssist.bEnabled = false; },
				[Data, bTryJump, FarFace](ASupernaturalProtCharacter& Character, float, double)
				{
					Character.DoMove(0.f, 1.f);
					if (bTryJump && Character.GetActorLocation().X > ThinObstacleX - 150.f)
					{
						Character.Jump();
					}
					Data->MaxZ = FMath::Max(Data->MaxZ, Character.GetActorLocation().Z);
					return Character.GetActorLocation().X > FarFace + 60.f; // crossed
				},
				[this, Data, Height, bTryJump](ASupernaturalProtCharacter& Character, bool bTimedOut)
				{
					Character.StopJumping();
					Character.GetSpnMovement()->LedgeAssist.bEnabled = true;
					const FSpnMovementSettings& S = Character.GetSpnMovement()->Settings;
					const ESpnObstacleClass Class = SpnMovement::ClassifyObstacle(S, Height);
					const bool bCrossed = !bTimedOut;
					AddInfo(FString::Printf(TEXT("MEASURED: %.0f cm obstacle%s -> %s (max Z %.0f, end X %.0f, end Z %.0f)"), Height, bTryJump ? TEXT(" with jump attempts") : TEXT(" walking"),
						bCrossed ? TEXT("crossed") : TEXT("blocked"), Data->MaxZ, Character.GetActorLocation().X, Character.GetActorLocation().Z));
					const EVerdict Verdict = bTryJump ? JumpVerdict(S, Height) : WalkVerdict(S, Height);
					const FString What = FString::Printf(TEXT("%.0f cm obstacle%s (core class %s, end X %.0f)"), Height, bTryJump ? TEXT(" with jump attempts") : TEXT(" walking"),
						Class == ESpnObstacleClass::StepUp ? TEXT("StepUp") : (Class == ESpnObstacleClass::JumpCandidate ? TEXT("JumpCandidate") : TEXT("Blocked")),
						Character.GetActorLocation().X);
					switch (Verdict)
					{
					case EVerdict::MustPass:
						TestTrue(What + TEXT(": crossed"), bCrossed);
						break;
					case EVerdict::MustBlock:
						TestFalse(What + TEXT(": blocks"), bCrossed);
						break;
					default:
						ReportCase(*this, What, bCrossed);
						break;
					}
					// A character that mounted the ledge and keeps hopping on top of it (end Z near the top) is a measured outcome, not an exaggerated jump.
					const bool bStandsOnLedge = Character.GetActorLocation().Z > StandZ + Height - 10.f;
					if (!bCrossed && bTryJump && !bStandsOnLedge)
					{
						// A blocked character is not pushed up to an exaggerated height by trying to jump.
						TestTrue(FString::Printf(TEXT("%.0f cm obstacle: no exaggerated jump (max Z %.0f)"), Height, Data->MaxZ),
							Data->MaxZ <= StandZ + ExpectedJumpHeight + JumpTolerance);
					}
				}));
			ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.2f));
		}
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnMovementStairsRampsTest, "SpnGame.Movement.StairsAndRamps", SPN_MOVEMENT_MAP_TEST_FLAGS)

bool FSpnMovementStairsRampsTest::RunTest(const FString& Parameters)
{
	if (!RequireMap(*this))
	{
		return false;
	}
	AutomationOpenMap(MapPackage());
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));

	// Staircases: ten steps; climbed when the rise is clearly below the step limit, blocked when clearly above it, reported at the limit.
	for (int32 Index = 0; Index < static_cast<int32>(UE_ARRAY_COUNT(StairRises)); ++Index)
	{
		const float Rise = StairRises[Index];
		const float TopZ = StandZ + Rise * StairSteps;
		const TSharedRef<FRunData> Data = MakeShared<FRunData>();
		ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(*this, FVector(StairStartX - 200.f, StairY(Index), StandZ), 0.f, 10.f,
			[](ASupernaturalProtCharacter& Character) { Character.GetSpnMovement()->SetBaseGait(ESpnGait::Jog); },
			[Data, TopZ](ASupernaturalProtCharacter& Character, float, double)
			{
				Character.DoMove(0.f, 1.f);
				Data->MaxZ = FMath::Max(Data->MaxZ, Character.GetActorLocation().Z);
				return Character.GetActorLocation().Z > TopZ - 15.f;
			},
			[this, Rise, TopZ](ASupernaturalProtCharacter& Character, bool bTimedOut)
			{
				const FSpnMovementSettings& S = Character.GetSpnMovement()->Settings;
				const FString What = FString::Printf(TEXT("staircase with %.0f cm rise (end Z %.0f, top %.0f)"), Rise, Character.GetActorLocation().Z, TopZ);
				AddInfo(FString::Printf(TEXT("MEASURED: %s -> %s"), *What, bTimedOut ? TEXT("blocked") : TEXT("climbed")));
					switch (WalkVerdict(S, Rise))
				{
				case EVerdict::MustPass:
					TestFalse(What + TEXT(": climbed"), bTimedOut);
					break;
				case EVerdict::MustBlock:
					TestTrue(What + TEXT(": blocks"), bTimedOut);
					break;
				default:
					ReportCase(*this, What, !bTimedOut);
					break;
				}
			}));
		ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.2f));
	}

	// Ramps: walkable below the floor angle limit, the 50 degree ramp cannot be climbed. The rails keep the character on the slope.
	for (int32 Index = 0; Index < static_cast<int32>(UE_ARRAY_COUNT(RampAngles)); ++Index)
	{
		const float Angle = RampAngles[Index];
		const float Y = RampY(Index);
		const float GoalZ = StandZ + 0.5f * RampLength * FMath::Sin(FMath::DegreesToRadians(Angle)); // half way up the slope
		ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(*this, FVector(RampStartX - 200.f, Y, StandZ), 0.f, 12.f,
			[](ASupernaturalProtCharacter& Character) { Character.GetSpnMovement()->SetBaseGait(ESpnGait::Jog); },
			[GoalZ](ASupernaturalProtCharacter& Character, float, double)
			{
				Character.DoMove(0.f, 1.f);
				return Character.GetActorLocation().Z > GoalZ;
			},
			[this, Angle, GoalZ](ASupernaturalProtCharacter& Character, bool bTimedOut)
			{
				const bool bShouldClimb = Angle < WalkableAngleLimit;
				TestTrue(FString::Printf(TEXT("%.0f degree ramp: %s (end Z %.0f, half-way %.0f)"), Angle, bShouldClimb ? TEXT("walked up") : TEXT("cannot be walked up"),
					Character.GetActorLocation().Z, GoalZ), (!bTimedOut) == bShouldClimb);
			}));
		ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.2f));
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnMovementBoundaryTest, "SpnGame.Movement.Boundary", SPN_MOVEMENT_MAP_TEST_FLAGS)

bool FSpnMovementBoundaryTest::RunTest(const FString& Parameters)
{
	if (!RequireMap(*this))
	{
		return false;
	}
	AutomationOpenMap(MapPackage());
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));

	// Walk into each of the four boundary walls from 8 m away, sprinting: the character must stay inside.
	struct FWallCase
	{
		const TCHAR* Name;
		FVector Start;
		float Yaw;
		FVector Outward;
	};
	const float Edge = MapHalfExtent;
	const FWallCase Walls[] = {
		{TEXT("east"), FVector(Edge - 800.f, 0.f, StandZ), 0.f, FVector(1.f, 0.f, 0.f)},
		{TEXT("north"), FVector(0.f, Edge - 800.f, StandZ), 90.f, FVector(0.f, 1.f, 0.f)},
		{TEXT("west"), FVector(-Edge + 800.f, 0.f, StandZ), 180.f, FVector(-1.f, 0.f, 0.f)},
		{TEXT("south"), FVector(0.f, -Edge + 800.f, StandZ), -90.f, FVector(0.f, -1.f, 0.f)},
	};
	for (const FWallCase& Wall : Walls)
	{
		ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(*this, Wall.Start, Wall.Yaw, 3.5f,
			[](ASupernaturalProtCharacter& Character) { Character.DoSprintStart(); },
			[](ASupernaturalProtCharacter& Character, float, double)
			{
				Character.DoMove(0.f, 1.f);
				return false; // run for the full time
			},
			[this, Wall](ASupernaturalProtCharacter& Character, bool)
			{
				Character.DoSprintEnd();
				const float Outward = FVector::DotProduct(Character.GetActorLocation(), Wall.Outward);
				TestTrue(FString::Printf(TEXT("%s wall: stopped inside the square (%.0f of %.0f)"), Wall.Name, Outward, Edge), Outward < Edge - 20.f);
				TestTrue(FString::Printf(TEXT("%s wall: close to the wall after the run (%.0f)"), Wall.Name, Outward), Outward > Edge - 200.f);
			}));
		ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.2f));
	}

	// Second barrier: a pawn moved out of the guarded volume is put back at the reset point nearest to where it left.
	// Cases: straight up over the wall tops, far beyond the east wall, and under the floor.
	const FVector Outside[] = {FVector(0.f, 0.f, 6600.f), FVector(Edge + 3000.f, 500.f, 300.f), FVector(-4000.f, 2000.f, -6000.f)};
	for (const FVector& Place : Outside)
	{
		ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, Place]
		{
			ASupernaturalProtCharacter* Character = FindCharacter();
			if (!TestNotNull(TEXT("character"), Character))
			{
				return true;
			}
			Character->GetCharacterMovement()->StopMovementImmediately();
			Character->SetActorLocation(Place, false, nullptr, ETeleportType::TeleportPhysics);
			return true;
		}));
		ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.5f));
		ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, Place]
		{
			const UWorld* World = FindPlayWorld();
			const ASupernaturalProtCharacter* Character = FindCharacter();
			if (World == nullptr || !TestNotNull(TEXT("character"), Character))
			{
				return true;
			}
			int32 Guards = 0;
			for (TActorIterator<ASpnPlayAreaGuard> It(World); It; ++It)
			{
				++Guards;
				const FString Where = Place.ToString();
				TestTrue(FString::Printf(TEXT("guard put the pawn back (from %s)"), *Where), It->GetReturnCount() >= 1);
				TestTrue(TEXT("pawn is inside the guarded area again"), It->IsInside(Character));
				const FVector Expected = It->ChooseReturnLocation(Place);
				TestTrue(FString::Printf(TEXT("pawn is near the nearest reset point %s"), *Expected.ToString()), FVector::Dist(Character->GetActorLocation(), Expected) < 400.f);
				TestTrue(TEXT("the guard returned the pawn to that reset point"), It->GetLastReturnLocation().Equals(Expected, 0.1f));
				TestTrue(FString::Printf(TEXT("the guard has at least %d reset points"), MinimumResetPoints), It->ResetPoints.Num() >= MinimumResetPoints);
			}
			TestEqual(TEXT("exactly one guard"), Guards, 1);
			return true;
		}));
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnMovementCameraTest, "SpnGame.Movement.AlleyCamera", SPN_MOVEMENT_MAP_TEST_FLAGS)

bool FSpnMovementCameraTest::RunTest(const FString& Parameters)
{
	if (!RequireMap(*this))
	{
		return false;
	}
	AutomationOpenMap(MapPackage());
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));

	// In the low section of the alley (ceiling underside at 230 cm, walls 75 cm either side) the camera, looking down from
	// behind the character, must be pulled in by the ceiling instead of sitting inside it.
	const TSharedRef<FRunData> Data = MakeShared<FRunData>();
	ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(*this, FVector(0.f, 5500.f, StandZ), -90.f, 3.f,
		[](ASupernaturalProtCharacter& Character)
		{
			Character.GetController()->SetControlRotation(FRotator(-20.f, -90.f, 0.f));
		},
		[Data](ASupernaturalProtCharacter& Character, float, double Elapsed)
		{
			Character.GetController()->SetControlRotation(FRotator(-20.f, -90.f, 0.f));
			if (Elapsed > 1.0)
			{
				const UCameraComponent* Camera = Character.GetFollowCamera();
				const USpringArmComponent* Boom = Character.GetCameraBoom();
				Data->End = Camera->GetComponentLocation();
				Data->ArmUsed = FVector::Dist(Camera->GetComponentLocation(), Boom->GetComponentLocation()); // arm length actually used
				return true;
			}
			return false;
		},
		[this, Data](ASupernaturalProtCharacter& Character, bool bTimedOut)
		{
			TestFalse(TEXT("alley camera scenario finished"), bTimedOut);
			TestTrue(FString::Printf(TEXT("camera stays below the 230 cm ceiling (camera Z %.0f)"), Data->End.Z), Data->End.Z < 230.f);
			TestTrue(FString::Printf(TEXT("camera stays between the alley walls (camera X %.0f)"), Data->End.X), FMath::Abs(Data->End.X) < 75.f);
			TestTrue(FString::Printf(TEXT("the arm is shorter than authored (%.0f of %.0f cm)"), Data->ArmUsed, Character.GetConfiguredArmLength()),
				Data->ArmUsed < Character.GetConfiguredArmLength() - 20.f);
		}));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnMovementCameraClearanceTest, "SpnGame.Movement.CameraClearance", SPN_MOVEMENT_MAP_TEST_FLAGS)

bool FSpnMovementCameraClearanceTest::RunTest(const FString& Parameters)
{
	if (!RequireMap(*this))
	{
		return false;
	}
	AutomationOpenMap(MapPackage());
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));

	// The camera must never sit inside blocking geometry, not even while the view is swung around quickly (240 degrees/s with
	// the pitch moving between -35 and +10 degrees). Poses: the low alley, the hut, the house hall, half way up the house stairs
	// (stair 9 of 17), between pillars, and on a staircase of the stair field.
	struct FPose
	{
		const TCHAR* Name;
		FVector Location;
		float Yaw;
	};
	const FPose Poses[] = {
		{TEXT("low alley"), FVector(0.f, 5500.f, StandZ), -90.f},
		{TEXT("hut interior"), FVector(-4400.f, -9200.f, StandZ + 5.f), 0.f},
		{TEXT("house hall"), FVector(-400.f, -9200.f, StandZ + 5.f), 90.f},
		{TEXT("house stairs"), FVector(-625.f, -9442.f, 5.f + 9.f * 18.53f + StandZ + 2.f), 90.f},
		{TEXT("between pillars"), FVector(-5100.f, -6100.f, StandZ), 0.f},
		{TEXT("stair field"), FVector(StairStartX + 100.f, StairY(0), StandZ + 3.f * 15.f), 0.f},
	};
	for (const FPose& Pose : Poses)
	{
		const TSharedRef<FRunData> Data = MakeShared<FRunData>();
		ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(*this, Pose.Location, Pose.Yaw, 4.f,
			[](ASupernaturalProtCharacter&) {},
			[Data, Pose](ASupernaturalProtCharacter& Character, float, double Elapsed)
			{
				const float Yaw = Pose.Yaw + static_cast<float>(Elapsed) * 240.f;
				const float Pitch = -12.5f - 22.5f * FMath::Sin(static_cast<float>(Elapsed) * 3.f);
				Character.GetController()->SetControlRotation(FRotator(Pitch, Yaw, 0.f));
				if (Elapsed > 0.5) // let the character settle first
				{
					++Data->Samples;
					if (CameraOverlapsGeometry(Character))
					{
						++Data->Violations;
					}
				}
				return Elapsed > 3.5;
			},
			[this, Data, Pose](ASupernaturalProtCharacter&, bool)
			{
				TestTrue(FString::Printf(TEXT("%s: camera sampled"), Pose.Name), Data->Samples > 30);
				TestEqual(FString::Printf(TEXT("%s: frames with the camera inside geometry (of %d)"), Pose.Name, Data->Samples), Data->Violations, 0);
			}));
		ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.2f));
	}
	return true;
}

#if WITH_EDITOR // actor labels are editor-only
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnMovementMapLayoutTest, "SpnGame.Movement.MapLayout",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FSpnMovementMapLayoutTest::RunTest(const FString& Parameters)
{
	if (!RequireMap(*this))
	{
		return false;
	}
	const UWorld* Map = LoadObject<UWorld>(nullptr, TEXT("/Game/SpnTest/Lvl_SpnMovementTest.Lvl_SpnMovementTest"));
	if (!TestNotNull(TEXT("map loads"), Map) || !TestNotNull(TEXT("map has a level"), Map->PersistentLevel.Get()))
	{
		return false;
	}

	TMap<FString, const AStaticMeshActor*> Meshes;
	int32 Guards = 0;
	int32 Doors = 0;
	int32 Starts = 0;
	int32 ScriptMeshes = 0;
	const ASpnPlayAreaGuard* Guard = nullptr;
	const AActor* Start = nullptr;
	for (const AActor* Actor : Map->PersistentLevel->Actors)
	{
		if (const AStaticMeshActor* Mesh = Cast<AStaticMeshActor>(Actor))
		{
			Meshes.Add(Mesh->GetActorLabel(), Mesh);
			if (Mesh->GetActorLabel().StartsWith(TEXT("SPN_MV_")))
			{
				++ScriptMeshes;
				TestTrue(FString::Printf(TEXT("%s has a mesh"), *Mesh->GetActorLabel()), Mesh->GetStaticMeshComponent()->GetStaticMesh() != nullptr);
				TestTrue(FString::Printf(TEXT("%s has collision"), *Mesh->GetActorLabel()), Mesh->GetStaticMeshComponent()->IsCollisionEnabled());
			}
		}
		if (const ASpnPlayAreaGuard* AsGuard = Cast<ASpnPlayAreaGuard>(Actor))
		{
			Guard = AsGuard;
			++Guards;
		}
		Doors += Cast<ASpnTestDoor>(Actor) != nullptr ? 1 : 0;
		if (Actor != nullptr && Actor->GetClass()->GetName() == TEXT("PlayerStart"))
		{
			Start = Actor;
			++Starts;
		}
	}
	TestEqual(TEXT("one play-area guard"), Guards, 1);
	TestEqual(TEXT("one player start (a second one would make the spawn point random)"), Starts, 1);
	TestTrue(TEXT("test doors placed"), Doors >= 2);
	TestEqual(TEXT("number of generated shapes matches the layout"), ScriptMeshes, ExpectedPrimitiveCount);
	if (Start != nullptr)
	{
		TestTrue(FString::Printf(TEXT("player start at the plaza centre (%s)"), *Start->GetActorLocation().ToString()), Start->GetActorLocation().Equals(FVector(0.f, 0.f, 100.f), 1.f));
	}
	if (Guard != nullptr)
	{
		TestTrue(TEXT("guard covers the whole square"), Guard->BoxExtent.X >= MapHalfExtent && Guard->BoxExtent.Y >= MapHalfExtent);
		TestTrue(FString::Printf(TEXT("guard has %d reset points (at least %d)"), Guard->ResetPoints.Num(), MinimumResetPoints), Guard->ResetPoints.Num() >= MinimumResetPoints);
		for (const FVector& Point : Guard->ResetPoints)
		{
			TestTrue(FString::Printf(TEXT("reset point %s lies inside the square"), *Point.ToString()), FMath::Abs(Point.X) < MapHalfExtent && FMath::Abs(Point.Y) < MapHalfExtent);
		}
	}

	if (const AStaticMeshActor* const* Floor = Meshes.Find(TEXT("SPN_MV_Floor")); TestNotNull(TEXT("floor"), Floor != nullptr ? *Floor : nullptr))
	{
		const FBox Box = (*Floor)->GetComponentsBoundingBox(true);
		TestTrue(FString::Printf(TEXT("floor is 300 x 300 m (%.0f x %.0f cm)"), Box.GetSize().X, Box.GetSize().Y),
			FMath::IsNearlyEqual(Box.GetSize().X, 30000.f, 5.f) && FMath::IsNearlyEqual(Box.GetSize().Y, 30000.f, 5.f));
		TestTrue(TEXT("ground surface at Z = 0"), FMath::Abs(Box.Max.Z) < 1.f);
	}

	for (const float Height : ObstacleHeights)
	{
		const FString Label = FString::Printf(TEXT("SPN_MV_Obstacle_Thin_%03d"), FMath::RoundToInt(Height));
		if (const AStaticMeshActor* const* Obstacle = Meshes.Find(Label); TestNotNull(Label, Obstacle != nullptr ? *Obstacle : nullptr))
		{
			const FBox Box = (*Obstacle)->GetComponentsBoundingBox(true);
			TestTrue(Label + TEXT(" has the stated height"), FMath::IsNearlyEqual(Box.Max.Z, Height, 0.5f) && FMath::Abs(Box.Min.Z) < 0.5f);
			TestTrue(Label + TEXT(" has collision"), (*Obstacle)->GetStaticMeshComponent()->IsCollisionEnabled());
		}
	}
	for (const float Angle : RampAngles)
	{
		const FString Label = FString::Printf(TEXT("SPN_MV_Ramp_%02d"), FMath::RoundToInt(Angle));
		if (const AStaticMeshActor* const* Ramp = Meshes.Find(Label); TestNotNull(Label, Ramp != nullptr ? *Ramp : nullptr))
		{
			TestTrue(Label + TEXT(" has the stated pitch"), FMath::IsNearlyEqual((*Ramp)->GetActorRotation().Pitch, Angle, 0.1f));
		}
		for (const TCHAR* Rail : {TEXT("Rail_W"), TEXT("Rail_E"), TEXT("Landing_RailW"), TEXT("Landing_RailE"), TEXT("Landing_EndRail")})
		{
			const FString RailLabel = FString::Printf(TEXT("%s_%s"), *Label, Rail);
			TestTrue(RailLabel + TEXT(" exists (no open drop edge)"), Meshes.Contains(RailLabel));
		}
	}
	for (const float Rise : StairRises)
	{
		const FString Stairs = FString::Printf(TEXT("SPN_MV_Stairs_R%02d"), FMath::RoundToInt(Rise));
		const FString Label = FString::Printf(TEXT("%s_Step_%02d"), *Stairs, StairSteps);
		if (const AStaticMeshActor* const* Top = Meshes.Find(Label); TestNotNull(Label, Top != nullptr ? *Top : nullptr))
		{
			TestTrue(Label + TEXT(" top step height"), FMath::IsNearlyEqual((*Top)->GetComponentsBoundingBox(true).Max.Z, Rise * StairSteps, 0.5f));
		}
		for (const TCHAR* Rail : {TEXT("Side_W"), TEXT("Side_E"), TEXT("EndRail")})
		{
			const FString RailLabel = FString::Printf(TEXT("%s_%s"), *Stairs, Rail);
			TestTrue(RailLabel + TEXT(" exists (no open drop edge)"), Meshes.Contains(RailLabel));
		}
	}
	for (const TCHAR* Name : {TEXT("North"), TEXT("South"), TEXT("East"), TEXT("West")})
	{
		const FString Label = FString::Printf(TEXT("SPN_MV_Boundary_%s"), Name);
		if (const AStaticMeshActor* const* Wall = Meshes.Find(Label); TestNotNull(Label, Wall != nullptr ? *Wall : nullptr))
		{
			TestTrue(Label + TEXT(" is a high solid wall"), (*Wall)->GetStaticMeshComponent()->IsCollisionEnabled() && (*Wall)->GetComponentsBoundingBox(true).Max.Z >= 1500.f);
		}
	}
	for (const TCHAR* Name : {TEXT("SPN_MV_House_StairRail"), TEXT("SPN_MV_Warehouse_DockRamp_Rail_W"), TEXT("SPN_MV_Warehouse_DockRamp_Rail_E")})
	{
		TestTrue(FString(Name) + TEXT(" exists"), Meshes.Contains(Name));
	}
	return true;
}
#endif // WITH_EDITOR

#endif
