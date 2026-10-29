#include "Components/CapsuleComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "Spn/Tests/SpnMovementTestSupport.h"
#include "Spn/Tests/SpnQaRecorder.h"

#if WITH_DEV_AUTOMATION_TESTS

// SpnQA.Locomotion (M2A4.1): locomotion quality measured on the real character with KEY input (W, S, LeftShift) and nothing else.
//   direction reversal:   at walk/jog/sprint the heading of the velocity may turn at most as fast as the steering settings allow (no instant flip),
//                         the deceleration stays physical (a bounded multiple of the braking deceleration), the way back to full speed is measured.
//   rotation stability:   the actor yaw does not jitter while running straight, turns at most at the configured rotation rate, and is still once the input ends.
//   obstacle collision:   walking (no jump) over a 15 cm step is possible, a 60 cm and a 150 cm obstacle block the capsule (no penetration, no climbing).
// Test setup: each case starts with the character put on the V1 speed lane by FScenarioCommand (a documented setup teleport, as in SpnQA.Movement);
// the measured movement itself is key-driven. The numbers asserted are derived from the movement settings, not tuned to the result.

using namespace SpnMovementTest;

namespace
{
	constexpr float LaneX = -12500.f;
	constexpr float LaneStartY = -12000.f;
	constexpr EAutomationTestFlags LocomotionFlags = EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter;
	const TCHAR* const Suite = TEXT("Locomotion");

	bool OpenMapForLocomotion(FAutomationTestBase& Test)
	{
		const FString Map = SpnQa::QaMapPath();
		if (!FPackageName::DoesPackageExist(Map))
		{
			Test.AddError(*FString::Printf(TEXT("%s does not exist: run create_spn_movement_test_map.py first."), *Map));
			return false;
		}
		AutomationOpenMap(Map);
		ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));
		return true;
	}

	AStaticMeshActor* SpawnObstacle(UWorld& World, const FVector& Center, const FVector& Size)
	{
		AStaticMeshActor* Actor = World.SpawnActor<AStaticMeshActor>(Center, FRotator::ZeroRotator);
		Actor->SetMobility(EComponentMobility::Movable);
		Actor->GetStaticMeshComponent()->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube")));
		Actor->GetStaticMeshComponent()->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
		Actor->SetActorScale3D(Size / 100.f);
		return Actor;
	}

	double WrapDegrees(double Angle)
	{
		while (Angle > 180.0) { Angle -= 360.0; }
		while (Angle < -180.0) { Angle += 360.0; }
		return Angle;
	}

	struct FReversalRun
	{
		SpnQa::FHeldKeys Keys;
		double TurnStart = -1.0;
		double HeadingBefore = 0.0, SpeedBefore = 0.0;
		double MaxHeadingRate = 0.0, MaxDecel = 0.0, MinSpeed = 1.0e9, TimeReversed = -1.0, TimeBack = -1.0;
		double PrevHeading = 0.0, PrevSpeed = 0.0, PrevTime = 0.0;
		bool bHavePrev = false;
		TArray<TArray<double>> Trace;
	};

	struct FYawRun
	{
		SpnQa::FHeldKeys Keys;
		double ReleaseAt = -1.0, PrevYaw = 0.0, PrevTime = 0.0;
		double StraightMin = 1.0e9, StraightMax = -1.0e9, MaxYawRate = 0.0, YawAtRelease = 0.0, YawAtEnd = 0.0, MaxDriftAfterRelease = 0.0;
		bool bHavePrev = false;
	};

	struct FObstacleRun
	{
		SpnQa::FHeldKeys Keys;
		AStaticMeshActor* Block = nullptr;
		float FrontY = 0.f;
		float MaxY = -1.0e9f, MaxZ = -1.0e9f;
		bool bCrossed = false;
	};
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnQaLocomotionTest, "SpnQA.Locomotion", LocomotionFlags)

bool FSpnQaLocomotionTest::RunTest(const FString& Parameters)
{
	if (!OpenMapForLocomotion(*this)) { return false; }
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([] { SpnQa::Section(TEXT("Locomotion: direction reversal, rotation stability, obstacles")); return true; }));

	// ---- direction reversal: run at a gait, then W goes up and S goes down in the same frame (a 180 degree change of the wanted direction)
	struct FReversalCase { const TCHAR* Name; ESpnGait Gait; bool bSprint; };
	const FReversalCase ReversalCases[] = {{TEXT("reversal_walk"), ESpnGait::Walk, false}, {TEXT("reversal_jog"), ESpnGait::Jog, false}, {TEXT("reversal_sprint"), ESpnGait::Jog, true}};
	for (const FReversalCase& Case : ReversalCases)
	{
		const TSharedRef<FReversalRun> R = MakeShared<FReversalRun>();
		ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(*this, FVector(LaneX, LaneStartY, StandZ), 90.f, 16.f,
			[Case](ASupernaturalProtCharacter& Character) { Character.GetSpnMovement()->SetBaseGait(Case.Gait); },
			[Case, R](ASupernaturalProtCharacter& Character, float Dt, double Elapsed)
			{
				const FVector Vel = Character.GetVelocity();
				const double Speed = Vel.Size2D();
				const double Heading = Speed > 1.0 ? FMath::RadiansToDegrees(FMath::Atan2(Vel.Y, Vel.X)) : R->PrevHeading;
				if (R->TurnStart < 0.0)
				{
					R->Keys.Set(Character, EKeys::W, true);
					R->Keys.Set(Character, EKeys::LeftShift, Case.bSprint);
					if (Elapsed > 4.8 && Speed >= 0.97 * SpnMovement::GaitSpeed(Character.GetSpnMovement()->Settings, Case.bSprint ? ESpnGait::Sprint : Case.Gait))
					{
						R->TurnStart = Elapsed; R->HeadingBefore = Heading; R->SpeedBefore = Speed; R->MinSpeed = Speed;
						R->Keys.Set(Character, EKeys::W, false);
						R->Keys.Set(Character, EKeys::S, true);
					}
					else if (Elapsed > 9.0) { return true; }   // never reached its gait speed: reported below as no data
					R->PrevHeading = Heading; R->PrevSpeed = Speed; R->PrevTime = Elapsed; R->bHavePrev = true;
					return false;
				}
				const double Since = Elapsed - R->TurnStart;
				if (R->bHavePrev && Dt > 0.f && Speed > 100.0 && R->PrevSpeed > 100.0)
				{
					R->MaxHeadingRate = FMath::Max(R->MaxHeadingRate, FMath::Abs(WrapDegrees(Heading - R->PrevHeading)) / Dt);
				}
				if (R->bHavePrev && Dt > 0.f) { R->MaxDecel = FMath::Max(R->MaxDecel, (R->PrevSpeed - Speed) / Dt); }
				R->MinSpeed = FMath::Min(R->MinSpeed, Speed);
				if (R->TimeReversed < 0.0 && Speed > 100.0 && FMath::Abs(WrapDegrees(Heading - (R->HeadingBefore + 180.0))) < 30.0) { R->TimeReversed = Since; }
				if (R->TimeReversed >= 0.0 && R->TimeBack < 0.0 && Speed >= 0.95 * R->SpeedBefore) { R->TimeBack = Since; }
				if (R->Trace.Num() < 400) { R->Trace.Add({Since, Speed, Heading, Character.GetActorRotation().Yaw}); }
				R->PrevHeading = Heading; R->PrevSpeed = Speed; R->PrevTime = Elapsed;
				return Since > 4.0;
			},
			[this, Case, R](ASupernaturalProtCharacter& Character, bool)
			{
				R->Keys.ReleaseAll(Character);
				SpnQa::Trace(Suite, Case.Name, {TEXT("t"), TEXT("speed"), TEXT("velocity_heading"), TEXT("actor_yaw")}, R->Trace);
				const FSpnMovementSettings& S = Character.GetSpnMovement()->Settings;
				if (R->TurnStart < 0.0)
				{
					SpnQa::Check(*this, &Character, Suite, Case.Name, SpnQa::EStatus::Blocked, TEXT("the character never reached the gait speed before the reversal"));
					return;
				}
				// Limits derived from the settings: the heading follows at most MaxHeadingRate (+20% for the frame sampling), the deceleration is at most three
				// times the braking deceleration (speed lost to the turn comes on top of braking), and the heading really arrives at the opposite direction.
				const double HeadingLimit = S.bUseSteering ? 1.2 * S.MaxHeadingRateDegrees : 1.2 * 720.0;
				const double DecelLimit = 3.0 * S.BrakingDeceleration;
				const bool bNoFlip = R->MaxHeadingRate <= HeadingLimit;
				const bool bPhysicalDecel = R->MaxDecel <= DecelLimit;
				const bool bReverses = R->TimeReversed >= 0.0 || R->MinSpeed < 100.0;
				const bool bNotInstant = R->TimeReversed < 0.0 || R->TimeReversed >= 0.2;
				SpnQa::Check(*this, &Character, Suite, Case.Name,
					bNoFlip && bPhysicalDecel && bReverses && bNotInstant ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail,
					FString::Printf(TEXT("speed before %.0f cm/s; heading rate max %.0f deg/s (limit %.0f); deceleration max %.0f cm/s^2 (limit %.0f); lowest speed %.0f (%.0f%%); heading reversed after %s; back at 95%% speed after %s"),
						R->SpeedBefore, R->MaxHeadingRate, HeadingLimit, R->MaxDecel, DecelLimit, R->MinSpeed, 100.0 * R->MinSpeed / R->SpeedBefore,
						R->TimeReversed >= 0.0 ? *FString::Printf(TEXT("%.2f s"), R->TimeReversed) : TEXT("(stopped first)"),
						R->TimeBack >= 0.0 ? *FString::Printf(TEXT("%.2f s"), R->TimeBack) : TEXT("not within 4 s")));
			}));
		ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.2f));
	}

	// ---- rotation stability: straight sprint, then release; the yaw must neither jitter nor drift
	{
		const TSharedRef<FYawRun> R = MakeShared<FYawRun>();
		ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(*this, FVector(LaneX, LaneStartY, StandZ), 90.f, 12.f,
			[](ASupernaturalProtCharacter& Character) { Character.GetSpnMovement()->SetBaseGait(ESpnGait::Jog); },
			[R](ASupernaturalProtCharacter& Character, float Dt, double Elapsed)
			{
				const double Yaw = Character.GetActorRotation().Yaw;
				if (R->bHavePrev && Dt > 0.f) { R->MaxYawRate = FMath::Max(R->MaxYawRate, FMath::Abs(WrapDegrees(Yaw - R->PrevYaw)) / Dt); }
				R->PrevYaw = Yaw; R->PrevTime = Elapsed; R->bHavePrev = true;
				if (R->ReleaseAt < 0.0)
				{
					R->Keys.Set(Character, EKeys::W, true);
					if (Elapsed > 1.5) { R->StraightMin = FMath::Min(R->StraightMin, Yaw); R->StraightMax = FMath::Max(R->StraightMax, Yaw); }
					if (Elapsed > 4.5) { R->ReleaseAt = Elapsed; R->YawAtRelease = Yaw; R->Keys.Set(Character, EKeys::W, false); }
					return false;
				}
				R->MaxDriftAfterRelease = FMath::Max(R->MaxDriftAfterRelease, FMath::Abs(WrapDegrees(Yaw - R->YawAtRelease)));
				R->YawAtEnd = Yaw;
				return Elapsed - R->ReleaseAt > 2.0;
			},
			[this, R](ASupernaturalProtCharacter& Character, bool)
			{
				R->Keys.ReleaseAll(Character);
				const FSpnMovementSettings& S = Character.GetSpnMovement()->Settings;
				const double Spread = R->StraightMax - R->StraightMin;
				const bool bOk = Spread <= 1.0 && R->MaxDriftAfterRelease <= 1.0 && R->MaxYawRate <= 1.15 * S.TurnRateDegrees;
				SpnQa::Check(*this, &Character, Suite, TEXT("rotation_stability"), bOk ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail,
					FString::Printf(TEXT("yaw spread while running straight %.2f deg (limit 1.0); drift after the input ended %.2f deg (limit 1.0); highest yaw rate %.0f deg/s (limit %.0f = 1.15 x rotation rate)"),
						Spread, R->MaxDriftAfterRelease, R->MaxYawRate, 1.15 * S.TurnRateDegrees));
			}));
		ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.2f));
	}

	// ---- obstacle collision while walking (no jump key): a low step is walked over, higher obstacles block
	struct FObstacleCase { const TCHAR* Name; float Height; bool bMustCross; };
	const FObstacleCase ObstacleCases[] = {{TEXT("step_15_crossed"), 15.f, true}, {TEXT("obstacle_60_blocks"), 60.f, false}, {TEXT("wall_150_blocks"), 150.f, false}};
	for (const FObstacleCase& Case : ObstacleCases)
	{
		const TSharedRef<FObstacleRun> R = MakeShared<FObstacleRun>();
		ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(*this, FVector(LaneX, LaneStartY, StandZ), 90.f, 8.f,
			[Case, R](ASupernaturalProtCharacter& Character)
			{
				Character.GetSpnMovement()->SetBaseGait(ESpnGait::Jog);
				R->FrontY = LaneStartY + 1200.f;
				R->Block = SpawnObstacle(*Character.GetWorld(), FVector(LaneX, R->FrontY + 50.f, Case.Height / 2.f), FVector(400.f, 100.f, Case.Height));
			},
			[Case, R](ASupernaturalProtCharacter& Character, float, double Elapsed)
			{
				R->Keys.Set(Character, EKeys::W, true);
				const FVector Loc = Character.GetActorLocation();
				R->MaxY = FMath::Max(R->MaxY, static_cast<float>(Loc.Y));
				R->MaxZ = FMath::Max(R->MaxZ, static_cast<float>(Loc.Z));
				if (Loc.Y > R->FrontY + 150.f) { R->bCrossed = true; return true; }
				return Elapsed > 6.5;
			},
			[this, Case, R](ASupernaturalProtCharacter& Character, bool)
			{
				R->Keys.ReleaseAll(Character);
				if (R->Block != nullptr) { R->Block->Destroy(); }
				const bool bPenetrated = !Case.bMustCross && R->MaxY > R->FrontY - 42.f + 10.f;   // capsule radius 42: its centre stays 42 cm before the face
				const bool bClimbed = !Case.bMustCross && R->MaxZ > StandZ + 8.f;
				const bool bOk = Case.bMustCross ? R->bCrossed : (!R->bCrossed && !bPenetrated && !bClimbed);
				SpnQa::Check(*this, &Character, Suite, Case.Name, bOk ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail,
					FString::Printf(TEXT("%.0f cm obstacle walking (jog, no jump key): crossed %s (expected %s); capsule centre reached %.0f cm before the face (limit: 42-10), highest Z %.0f (standing %.0f)"),
						Case.Height, R->bCrossed ? TEXT("yes") : TEXT("no"), Case.bMustCross ? TEXT("yes") : TEXT("no"), R->FrontY - R->MaxY, R->MaxZ, StandZ));
			}));
		ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.2f));
	}
	return true;
}

#endif
