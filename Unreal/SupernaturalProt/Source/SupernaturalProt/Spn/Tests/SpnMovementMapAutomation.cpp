#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/Level.h"
#include "Engine/StaticMeshActor.h"
#include "EngineUtils.h"
#include "GameFramework/SpringArmComponent.h"
#include "Spn/SpnPlayAreaGuard.h"
#include "Spn/Tests/SpnMovementTestSupport.h"
#include "Spn/SpnTestDoor.h"

#if WITH_DEV_AUTOMATION_TESTS

// M2-A movement tests in /Game/SpnTest/Lvl_SpnMovementTest (Docs/Proposals/0011). They play the map (PIE in the editor, or -game):
//   UnrealEditor-Cmd.exe <uproject> -ExecCmds="Automation RunTests SpnGame.Movement;Quit" -unattended -nullrhi
// The map comes from Tools/create_spn_movement_test_map.py; without it these tests fail with a message that says so.
// Tolerances are wide on purpose: automation frame times vary. WRITTEN WITHOUT AN ENGINE, NOT YET RUN: see the handover document.

using namespace SpnMovementTest;

#define SPN_MOVEMENT_MAP_TEST_FLAGS (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)

namespace
{
	// Layout constants (Tools/spn_movement_map_layout.py).
	constexpr float ExpectedJumpHeight = 45.f;
	constexpr float JumpTolerance = 7.f;
	constexpr float ObstacleHeights[] = {20.f, 30.f, 40.f, 50.f, 60.f, 80.f, 100.f, 120.f, 150.f};
	constexpr float ThinObstacleX = 3000.f;
	constexpr float ThinObstacleDepth = 40.f;
	constexpr float RampAngles[] = {5.f, 10.f, 15.f, 20.f, 30.f, 40.f, 50.f};
	constexpr float RampStartX = -9000.f;
	constexpr float RampLength = 1500.f;
	constexpr float StairRises[] = {15.f, 18.f, 22.f, 30.f, 40.f, 50.f};
	constexpr int32 StairSteps = 10;
	constexpr float WalkableAngleLimit = 44.8f;

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
	};

	/** Settles on the ground, then calls Jump once; returns the apex above the ground. Optionally holds Jump pressed throughout. */
	void AddJumpScenario(FAutomationTestBase& Test, const TSharedRef<FRunData>& Data, bool bSpamJump, float ObservationSeconds)
	{
		ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(Test, FVector(0.f, 0.f, StandZ), 0.f, 10.f,
			[](ASupernaturalProtCharacter&) {},
			[Data, bSpamJump, ObservationSeconds](ASupernaturalProtCharacter& Character, float, double Elapsed)
			{
				const UCharacterMovementComponent* Movement = Character.GetCharacterMovement();
				if (Data->Phase == 0)
				{
					if (Elapsed > 0.8 && Movement->IsMovingOnGround())
					{
						Data->GroundZ = Character.GetActorLocation().Z;
						Data->MaxZ = Data->GroundZ;
						Data->Phase = 1;
						Data->PhaseStart = Elapsed;
						Character.Jump();
					}
					return false;
				}
				if (!bSpamJump)
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
				if (bSpamJump)
				{
					Character.Jump(); // pressed all the time, never released
				}
				return Elapsed - Data->PhaseStart > ObservationSeconds;
			},
			[Data](ASupernaturalProtCharacter& Character, bool)
			{
				Character.StopJumping();
				Data->bDone = true;
			}));
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

	// A single standing jump reaches the configured apex.
	const TSharedRef<FRunData> Single = MakeShared<FRunData>();
	AddJumpScenario(*this, Single, /*bSpamJump*/ false, 1.5f);
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, Single]
	{
		const float Apex = Single->MaxZ - Single->GroundZ;
		TestTrue(TEXT("the jump scenario ran"), Single->bDone);
		TestTrue(FString::Printf(TEXT("vertical jump height is %.0f cm +- %.0f (measured %.1f)"), ExpectedJumpHeight, JumpTolerance, Apex),
			FMath::Abs(Apex - ExpectedJumpHeight) <= JumpTolerance);
		TestEqual(TEXT("one take-off"), Single->TakeOffs, 1);
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.3f));

	// Jump pressed all the time: no double jump in the air and no automatic second jump after landing.
	const TSharedRef<FRunData> Held = MakeShared<FRunData>();
	AddJumpScenario(*this, Held, /*bSpamJump*/ true, 2.5f);
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
	};

	for (const FGaitCase& Case : Cases)
	{
		const TSharedRef<FRunData> Data = MakeShared<FRunData>();
		ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(*this, FVector(-300.f, 0.f, StandZ), 180.f, 14.f,
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
				TestTrue(FString::Printf(TEXT("%s: steady speed %.0f cm/s is within 6%% of %.0f"), Case.Name, Average, Target),
					FMath::Abs(Average - Target) <= Target * 0.06f + 4.f);
				TestTrue(FString::Printf(TEXT("%s: never faster than the gait (max %.0f)"), Case.Name, Data->MaxSpeed), Data->MaxSpeed <= SpnMovement::GaitSpeed(S, Case.Expected) * 1.04f);
				TestTrue(FString::Printf(TEXT("%s: no speed jump while accelerating (worst excess %.1f cm/s per frame)"), Case.Name, Data->WorstAccelJump), Data->WorstAccelJump <= 6.f);
				TestTrue(FString::Printf(TEXT("%s: speed never rises while braking (worst %.1f)"), Case.Name, Data->WorstBrakeRise), Data->WorstBrakeRise <= 3.f);
				// Braking from Target at BrakingDeceleration takes Target / Brake seconds; allow 60 % + 0.4 s.
				const float ExpectedStop = Target / S.BrakingDeceleration;
				TestTrue(FString::Printf(TEXT("%s: stops in %.2f s (expected about %.2f s)"), Case.Name, Data->StopTime, ExpectedStop),
					Data->StopTime >= 0.f && Data->StopTime <= ExpectedStop * 1.6f + 0.4f);
			}));
		ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.2f));
	}

	// Sprint released while sprinting: the speed falls to the base gait smoothly, never below it.
	const TSharedRef<FRunData> Release = MakeShared<FRunData>();
	ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(*this, FVector(-300.f, 0.f, StandZ), 180.f, 14.f,
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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnMovementObstaclesTest, "SpnGame.Movement.Obstacles", SPN_MOVEMENT_MAP_TEST_FLAGS)

bool FSpnMovementObstaclesTest::RunTest(const FString& Parameters)
{
	if (!RequireMap(*this))
	{
		return false;
	}
	AutomationOpenMap(MapPackage());
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));

	// Walk (and, for the blocked ones, also jump) at each thin obstacle. The expectation comes from the core's classification of
	// the height, so engine behaviour and the portable rule are checked against each other.
	for (int32 Index = 0; Index < static_cast<int32>(UE_ARRAY_COUNT(ObstacleHeights)); ++Index)
	{
		const float Height = ObstacleHeights[Index];
		const float Y = (Index - 4) * 600.f;
		const float FarFace = ThinObstacleX + ThinObstacleDepth / 2.f;
		for (const bool bTryJump : {false, true})
		{
			const TSharedRef<FRunData> Data = MakeShared<FRunData>();
			ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(*this, FVector(ThinObstacleX - 300.f, Y, StandZ), 0.f, 4.f,
				[](ASupernaturalProtCharacter& Character) { Character.GetSpnMovement()->SetBaseGait(ESpnGait::Jog); },
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
					const FSpnMovementSettings& S = Character.GetSpnMovement()->Settings;
					const ESpnObstacleClass Class = SpnMovement::ClassifyObstacle(S, Height);
					const bool bCrossed = !bTimedOut;
					const bool bShouldCross = Class != ESpnObstacleClass::Blocked;
					TestTrue(FString::Printf(TEXT("%.0f cm obstacle%s: %s (core class %d, end X %.0f)"), Height, bTryJump ? TEXT(" with jump attempts") : TEXT(""),
						bShouldCross ? TEXT("can be crossed") : TEXT("blocks"), static_cast<int32>(Class), Character.GetActorLocation().X),
						bCrossed == bShouldCross);
					if (!bShouldCross)
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

	// Staircases: ten steps; climbable up to the step height, the 50 cm staircase blocks.
	for (int32 Index = 0; Index < static_cast<int32>(UE_ARRAY_COUNT(StairRises)); ++Index)
	{
		const float Rise = StairRises[Index];
		const float TopZ = StandZ + Rise * StairSteps;
		const TSharedRef<FRunData> Data = MakeShared<FRunData>();
		ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(*this, FVector(-9000.f - 200.f, 4000.f + Index * 500.f, StandZ), 0.f, 8.f,
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
				const bool bShouldClimb = Rise <= S.MaxStepHeight;
				TestTrue(FString::Printf(TEXT("staircase with %.0f cm rise: %s (end Z %.0f, top %.0f)"), Rise, bShouldClimb ? TEXT("climbed") : TEXT("blocks"),
					Character.GetActorLocation().Z, TopZ), (!bTimedOut) == bShouldClimb);
			}));
		ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.2f));
	}

	// Ramps: walkable below the floor angle limit, the 50 degree ramp cannot be climbed.
	for (int32 Index = 0; Index < static_cast<int32>(UE_ARRAY_COUNT(RampAngles)); ++Index)
	{
		const float Angle = RampAngles[Index];
		const float Y = (Index - 3) * 700.f;
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

	// Second barrier: a pawn moved out of the guarded volume is put back (here: straight up, over the wall tops).
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this]
	{
		const UWorld* World = FindPlayWorld();
		ASupernaturalProtCharacter* Character = FindCharacter();
		ASpnPlayAreaGuard* Guard = nullptr;
		if (World != nullptr)
		{
			for (TActorIterator<ASpnPlayAreaGuard> It(World); It; ++It)
			{
				Guard = *It;
			}
		}
		if (!TestNotNull(TEXT("play-area guard in the map"), Guard) || !TestNotNull(TEXT("character"), Character))
		{
			return true;
		}
		Character->GetCharacterMovement()->StopMovementImmediately();
		Character->SetActorLocation(Guard->GetActorLocation() + FVector(0.f, 0.f, Guard->BoxExtent.Z + 600.f), false, nullptr, ETeleportType::TeleportPhysics);
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.5f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this]
	{
		const UWorld* World = FindPlayWorld();
		const ASupernaturalProtCharacter* Character = FindCharacter();
		if (World == nullptr || Character == nullptr)
		{
			return true;
		}
		for (TActorIterator<ASpnPlayAreaGuard> It(World); It; ++It)
		{
			TestTrue(TEXT("guard put the pawn back"), It->GetReturnCount() >= 1);
			TestTrue(TEXT("pawn is inside the guarded area again"), It->IsInside(Character));
			TestTrue(TEXT("pawn is near the safe location"), FVector::Dist(Character->GetActorLocation(), It->SafeLocation) < 400.f);
		}
		return true;
	}));
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
	for (const AActor* Actor : Map->PersistentLevel->Actors)
	{
		if (const AStaticMeshActor* Mesh = Cast<AStaticMeshActor>(Actor))
		{
			Meshes.Add(Mesh->GetActorLabel(), Mesh);
		}
		Guards += Cast<ASpnPlayAreaGuard>(Actor) != nullptr ? 1 : 0;
		Doors += Cast<ASpnTestDoor>(Actor) != nullptr ? 1 : 0;
		Starts += Actor != nullptr && Actor->GetClass()->GetName() == TEXT("PlayerStart") ? 1 : 0;
	}
	TestEqual(TEXT("one play-area guard"), Guards, 1);
	TestEqual(TEXT("one player start"), Starts, 1);
	TestTrue(TEXT("test doors placed"), Doors >= 2);

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
	}
	for (const float Rise : StairRises)
	{
		const FString Label = FString::Printf(TEXT("SPN_MV_Stairs_R%02d_Step_%02d"), FMath::RoundToInt(Rise), StairSteps);
		if (const AStaticMeshActor* const* Top = Meshes.Find(Label); TestNotNull(Label, Top != nullptr ? *Top : nullptr))
		{
			TestTrue(Label + TEXT(" top step height"), FMath::IsNearlyEqual((*Top)->GetComponentsBoundingBox(true).Max.Z, Rise * StairSteps, 0.5f));
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
	return true;
}
#endif // WITH_EDITOR

#endif
