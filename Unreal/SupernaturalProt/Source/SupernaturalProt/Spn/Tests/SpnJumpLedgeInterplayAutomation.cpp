#include "Components/CapsuleComponent.h"
#include "Spn/Tests/SpnMovementTestSupport.h"

#if WITH_DEV_AUTOMATION_TESTS

// Jump physics and ledge assist TOGETHER (NightBuild06, Phase E). SpnGame.Movement.Obstacles switches the ledge assist off to measure the
// jump alone, SpnQA.LedgeAssist checks the assist on hand-built boxes; this test runs the thin obstacles of the movement map with the assist
// ON and one jump press, as a player would press it:
//   * "in reach": pressed once when the capsule front is about 30 cm from the face (inside the reach of Ledge-Assist-2026-11-14.md §1);
//   * "early":    pressed once 150 cm before the face (outside the reach): an ordinary jump that must stay an ordinary jump.
// Asserted from the developer's bands (2026-11-14: up to 40 step on / low vault, 40-120 vault or mantle, 120-180 climb up) and the jump
// settings: low and middle obstacles are crossed in reach with a ledge move; an early press never starts a ledge move; no outcome lifts the
// capsule higher than the obstacle (or the jump apex) allows; every started move ends. The 150 cm obstacle is only 40 cm deep, too shallow to
// stand on: its outcome is reported, not asserted.

namespace SpnJumpLedgeTest
{
	using namespace SpnMovementTest;

	// Same layout constants as SpnMovementMapAutomation.cpp (Tools/spn_movement_map_layout.py).
	constexpr float ObstacleHeights[] = {20.f, 25.f, 30.f, 40.f, 45.f, 50.f, 60.f, 75.f, 80.f, 100.f, 120.f, 150.f};
	constexpr float ObstacleSpacing = 600.f;
	constexpr float ThinObstacleX = 3000.f;
	constexpr float ThinObstacleDepth = 40.f;
	constexpr float LiftAllowance = 25.f;   // cm the capsule centre may rise above the top surface during a move (rounding of the path)

	float ObstacleY(int32 Index) { return (Index - (static_cast<int32>(UE_ARRAY_COUNT(ObstacleHeights)) - 1) * 0.5f) * ObstacleSpacing; }

	struct FRun
	{
		float MaxZ = -1.0e9f;
		bool bPressed = false;
		bool bReleased = false;
		int32 MovesBefore = 0;
		bool bSawLedgeMove = false;
		ESpnLedgeMove Move = ESpnLedgeMove::None;
	};
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnJumpLedgeInterplayTest, "SpnGame.Movement.JumpWithLedgeAssist",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)

bool FSpnJumpLedgeInterplayTest::RunTest(const FString& Parameters)
{
	using namespace SpnJumpLedgeTest;
	if (!RequireMap(*this))
	{
		return false;
	}
	AutomationOpenMap(MapPackage());
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));

	const float NearFace = ThinObstacleX - ThinObstacleDepth / 2.f;
	const float FarFace = ThinObstacleX + ThinObstacleDepth / 2.f;
	for (int32 Index = 0; Index < static_cast<int32>(UE_ARRAY_COUNT(ObstacleHeights)); ++Index)
	{
		const float Height = ObstacleHeights[Index];
		const float Y = ObstacleY(Index);
		for (const bool bEarly : {false, true})
		{
			const float PressGap = bEarly ? 150.f : 30.f;   // capsule front to face when the press happens, cm
			const TSharedRef<FRun> Data = MakeShared<FRun>();
			ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(*this, FVector(ThinObstacleX - 420.f, Y, StandZ), 0.f, 5.f,
				[Data](ASupernaturalProtCharacter& Character)
				{
					Character.GetSpnMovement()->LedgeAssist.bEnabled = true;
					Character.GetSpnMovement()->SetBaseGait(ESpnGait::Jog);
					Data->MovesBefore = Character.GetSpnMovement()->GetLedgeMoveCount();
				},
				[Data, NearFace, FarFace, PressGap](ASupernaturalProtCharacter& Character, float, double)
				{
					USpnCharacterMovementComponent* Move = Character.GetSpnMovement();
					Character.DoMove(0.f, 1.f);
					const float Front = static_cast<float>(Character.GetActorLocation().X) + Character.GetCapsuleComponent()->GetScaledCapsuleRadius();
					if (!Data->bPressed && Front >= NearFace - PressGap)
					{
						Data->bPressed = true;
						Character.Jump();   // one press, as a player would
					}
					else if (Data->bPressed && !Data->bReleased)
					{
						Data->bReleased = true;
						Character.StopJumping();
					}
					if (Move->IsLedgeAssisting())
					{
						Data->bSawLedgeMove = true;
						Data->Move = Move->GetActiveLedgeMove();
					}
					Data->MaxZ = FMath::Max(Data->MaxZ, static_cast<float>(Character.GetActorLocation().Z));
					// crossed and back on the floor behind it, or standing still on top after a move
					const bool bCrossed = Character.GetActorLocation().X > FarFace + 60.f && Move->IsMovingOnGround();
					return bCrossed;
				},
				[this, Data, Height, bEarly](ASupernaturalProtCharacter& Character, bool bTimedOut)
				{
					Character.StopJumping();
					USpnCharacterMovementComponent* Move = Character.GetSpnMovement();
					const FSpnMovementSettings& S = Move->Settings;
					const FSpnLedgeAssistSettings& L = Move->LedgeAssist;
					const bool bCrossed = !bTimedOut;
					const int32 Moves = Move->GetLedgeMoveCount() - Data->MovesBefore;
					const FSpnLedgeRecord& Record = Move->GetLastLedgeRecord();
					const FString What = FString::Printf(TEXT("%.0f cm thin obstacle, jog, press %s"), Height, bEarly ? TEXT("early (150 cm)") : TEXT("in reach (30 cm)"));
					AddInfo(FString::Printf(TEXT("MEASURED: %s -> %s, ledge moves %d (%s, record: %s / %s), max centre Z %.0f, end (%.0f, %.0f)"), *What,
						bCrossed ? TEXT("crossed") : TEXT("not crossed"), Moves, *UEnum::GetValueAsString(Data->Move), *UEnum::GetValueAsString(Record.Move),
						*Record.Reason, Data->MaxZ, Character.GetActorLocation().X, Character.GetActorLocation().Z));

					TestTrue(What + TEXT(": the press happened"), Data->bPressed);
					TestFalse(What + TEXT(": no ledge move is still running at the end"), Move->IsLedgeAssisting());
					TestTrue(*FString::Printf(TEXT("%s: at most one ledge move per press (%d)"), *What, Moves), Moves <= 1);

					// never lifted higher than the obstacle (plus the path rounding) or the jump apex allows
					const float Allowed = StandZ + FMath::Max(S.JumpHeight + 7.f + 6.f, Height + LiftAllowance);
					TestTrue(*FString::Printf(TEXT("%s: no exaggerated lift (max %.0f <= %.0f)"), *What, Data->MaxZ, Allowed), Data->MaxZ <= Allowed);

					if (bEarly)
					{
						// an early press is outside the reach: an ordinary jump, never a ledge move
						TestEqual(What + TEXT(": an early press starts no ledge move"), Moves, 0);
						return;
					}
					if (Height <= S.MaxStepHeight)
					{
						// up to the step height walking climbs it: the press needs no ledge move (Ledge-Assist-2026-11-14.md §1, first row)
						TestTrue(What + TEXT(": crossed (walkable step)"), bCrossed);
						TestEqual(What + TEXT(": no ledge move for a walkable step"), Moves, 0);
					}
					else if (Height <= L.MidBandMaxCm)
					{
						// 25-120 cm, 40 cm thin, free floor behind, a jog (350 cm/s > 200 run-up): step-over / vault per the bands
						TestTrue(What + TEXT(": crossed"), bCrossed);
						TestTrue(*FString::Printf(TEXT("%s: crossed with a ledge move (%d)"), *What, Moves), Moves == 1 && Data->bSawLedgeMove);
					}
					else
					{
						// 150 cm, 40 cm deep: too shallow to stand on, outcome reported
						AddInfo(FString::Printf(TEXT("MEASURED (not asserted): %s -> %s"), *What, bCrossed ? TEXT("crossed") : TEXT("not crossed")));
					}
				}));
			ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.2f));
		}
	}
	return true;
}

#endif
