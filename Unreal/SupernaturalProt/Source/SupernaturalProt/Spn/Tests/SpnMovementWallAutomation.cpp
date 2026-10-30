#include "Spn/Tests/SpnMovementTestSupport.h"

#if WITH_DEV_AUTOMATION_TESTS

// Movement investigation after Night Build 01: pressing straight into a wall must not turn a small sideways nudge into a slide along the wall.
// Root cause (measured): the SPN ground steering keeps speed while it turns; against a wall the engine resets the velocity to the actual (sliding)
// displacement every move, so the steering kept "turning" towards the wall and built up 133 cm/s along it (285 cm in 3 s) - the intermittent
// SpnQA.MovementDemo ledge_80_blocks_even_with_a_jump failure. The test pushes into the 150 cm thin obstacle of the movement map, gives the character one
// 30 cm/s sideways nudge while it is pressed against the face, and measures the drift. Diagonal input along a wall must still slide (engine behaviour).

using namespace SpnMovementTest;

namespace
{
	constexpr EAutomationTestFlags WallFlags = EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter;
	constexpr float WallObstacleX = 3000.f;   // Tools/spn_movement_map_layout.py: thin obstacles, 40 cm deep, face at X 2980
	constexpr float WallObstacleY = (11 - 11 * 0.5f) * 600.f;   // index 11 of 12: the 150 cm obstacle
	constexpr float MaxAllowedDriftCm = 10.f;

	struct FWallPush
	{
		double ContactAt = -1.0;
		bool bNudged = false;
		float StartY = 0.f;
	};
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnMovementWallPushTest, "SpnGame.Movement.WallPushNoSideDrift", WallFlags)
bool FSpnMovementWallPushTest::RunTest(const FString&)
{
	if (!RequireMap(*this))
	{
		return false;
	}
	AutomationOpenMap(MapPackage());
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));

	for (const float Yaw : {0.f, 30.f})   // straight into the wall, and 30 degrees off it (must slide)
	{
		const TSharedRef<FWallPush> P = MakeShared<FWallPush>();
		ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(*this, FVector(WallObstacleX - 300.f, WallObstacleY, StandZ), 0.f, 6.f,
			[P](ASupernaturalProtCharacter& Character)
			{
				Character.GetSpnMovement()->SetBaseGait(ESpnGait::Jog);
				P->StartY = Character.GetActorLocation().Y;
			},
			[P, Yaw](ASupernaturalProtCharacter& Character, float, double Elapsed)
			{
				if (P->ContactAt >= 0.0)
				{
					Character.GetController()->SetControlRotation(FRotator(0.f, Yaw, 0.f));
				}
				Character.DoMove(0.f, 1.f);
				if (P->ContactAt < 0.0 && Character.GetActorLocation().X > WallObstacleX - 20.f - 45.f)
				{
					P->ContactAt = Elapsed;
				}
				if (P->ContactAt >= 0.0 && !P->bNudged && Elapsed > P->ContactAt + 0.2)
				{
					if (Yaw == 0.f)
					{
						Character.GetCharacterMovement()->Velocity.Y += 30.f;   // what a landing or a bump next to a wall can leave behind
					}
					P->bNudged = true;
				}
				return P->ContactAt >= 0.0 && Elapsed > P->ContactAt + 3.2;
			},
			[this, P, Yaw](ASupernaturalProtCharacter& Character, bool bTimedOut)
			{
				TestFalse(TEXT("reached the wall"), bTimedOut);
				const float Drift = static_cast<float>(FMath::Abs(Character.GetActorLocation().Y - P->StartY));
				AddInfo(FString::Printf(TEXT("MEASURED: input %.0f degrees off the wall normal: drift along the wall %.1f cm in 3 s, end speed %.1f cm/s"), Yaw, Drift, Character.GetVelocity().Size2D()));
				if (Yaw == 0.f)
				{
					TestTrue(FString::Printf(TEXT("pressed straight into the wall the character stays put (drift %.1f cm <= %.0f)"), Drift, MaxAllowedDriftCm), Drift <= MaxAllowedDriftCm);
					TestTrue(TEXT("and comes to rest"), Character.GetVelocity().Size2D() < 5.f);
				}
				else
				{
					TestTrue(FString::Printf(TEXT("diagonal input still slides along the wall (drift %.1f cm)"), Drift), Drift > 100.f);
				}
			}));
		ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.2f));
	}
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
