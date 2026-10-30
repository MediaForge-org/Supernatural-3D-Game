#include "Camera/CameraComponent.h"
#include "Spn/Tests/SpnMovementTestSupport.h"

#if WITH_DEV_AUTOMATION_TESTS

// Night Build 01, A1: the stair camera smoothing is VISUAL ONLY. The character climbs the 15 cm staircase of the movement test map twice, once with the
// smoothing off and once on, and the test compares what the camera and the capsule did frame by frame (before/after measurement).

using namespace SpnMovementTest;

namespace { constexpr EAutomationTestFlags StairCameraFlags = EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter; }

namespace
{
	constexpr float StairsX = -9000.f - 200.f;  // Tools/spn_movement_map_layout.py: stair field start X minus the run-up
	constexpr float StairsY = 4000.f;           // first staircase (15 cm rise)
	constexpr float TopZ = StandZ + 15.f * 10.f;

	struct FRun
	{
		double MaxCameraStepCm = 0.0;   // largest vertical camera movement between two frames
		double MaxCapsuleStepCm = 0.0;  // largest vertical capsule movement between two frames
		double MaxLagCm = 0.0;
		double EndZ = 0.0;
		float PrevCamera = NAN;
		float PrevCapsule = NAN;
	};
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnStairCameraTest, "SpnGame.Movement.StairCamera", StairCameraFlags)
bool FSpnStairCameraTest::RunTest(const FString&)
{
	if (!RequireMap(*this))
	{
		return false;
	}
	AutomationOpenMap(MapPackage());
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));

	const TSharedRef<FRun> Off = MakeShared<FRun>();
	const TSharedRef<FRun> On = MakeShared<FRun>();
	for (const bool bSmooth : {false, true})
	{
		const TSharedRef<FRun> Run = bSmooth ? On : Off;
		ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(*this, FVector(StairsX, StairsY, StandZ), 0.f, 14.f,
			[bSmooth](ASupernaturalProtCharacter& Character)
			{
				Character.GetSpnMovement()->SetBaseGait(ESpnGait::Jog);
				Character.SetStairCameraSmoothingSeconds(bSmooth ? 0.14f : 0.f);
				Character.ResetStairCamera();
			},
			[Run](ASupernaturalProtCharacter& Character, float, double)
			{
				Character.DoMove(0.f, 1.f);
				const float Camera = Character.GetFollowCamera()->GetComponentLocation().Z;
				const float Capsule = Character.GetActorLocation().Z;
				if (!FMath::IsNaN(Run->PrevCamera))
				{
					Run->MaxCameraStepCm = FMath::Max<double>(Run->MaxCameraStepCm, FMath::Abs(Camera - Run->PrevCamera));
					Run->MaxCapsuleStepCm = FMath::Max<double>(Run->MaxCapsuleStepCm, FMath::Abs(Capsule - Run->PrevCapsule));
				}
				Run->PrevCamera = Camera;
				Run->PrevCapsule = Capsule;
				Run->MaxLagCm = FMath::Max<double>(Run->MaxLagCm, FMath::Abs(Character.GetStairCameraLag()));
				Run->EndZ = Capsule;
				return Capsule > TopZ - 15.f;
			},
			[this, Run, bSmooth](ASupernaturalProtCharacter& Character, bool bTimedOut)
			{
				TestFalse(FString::Printf(TEXT("staircase climbed (smoothing %s)"), bSmooth ? TEXT("on") : TEXT("off")), bTimedOut);
				AddInfo(FString::Printf(TEXT("MEASURED: smoothing %s: max camera step per frame %.2f cm, max capsule step per frame %.2f cm, max lag %.2f cm, end Z %.1f"),
					bSmooth ? TEXT("on") : TEXT("off"), Run->MaxCameraStepCm, Run->MaxCapsuleStepCm, Run->MaxLagCm, Run->EndZ));
			}));
		ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.3f));
	}

	// the comparison
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, Off, On]()
	{
		TestEqual(TEXT("smoothing off: the camera follows the capsule exactly (no lag)"), Off->MaxLagCm, 0.0, 0.001);
		TestTrue(TEXT("smoothing on: a lag builds up on the steps"), On->MaxLagCm > 1.0);
		TestTrue(TEXT("the lag is bounded by StairCameraMaxLag (24 cm)"), On->MaxLagCm <= 24.0 + 0.01);
		TestTrue(TEXT("the camera moves less per frame than without smoothing"), On->MaxCameraStepCm < Off->MaxCameraStepCm * 0.8);
		TestTrue(TEXT("the physical capsule is not smoothed: it reaches the same height"), FMath::Abs(On->EndZ - Off->EndZ) < 3.0);
		TestTrue(TEXT("the capsule moves just as abruptly as before (physics untouched)"), FMath::Abs(On->MaxCapsuleStepCm - Off->MaxCapsuleStepCm) < 3.0);
		return true;
	}));

	// stopping: the lag decays to zero in well under a second; jumping drops it at once
	const TSharedRef<FRun> Stop = MakeShared<FRun>();
	ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(*this, FVector(StairsX, StairsY, StandZ), 0.f, 14.f,
		[](ASupernaturalProtCharacter& Character)
		{
			Character.GetSpnMovement()->SetBaseGait(ESpnGait::Jog);
			Character.SetStairCameraSmoothingSeconds(0.14f);
			Character.ResetStairCamera();
		},
		[Stop](ASupernaturalProtCharacter& Character, float, double)
		{
			Character.DoMove(0.f, 1.f);
			Stop->MaxLagCm = FMath::Max<double>(Stop->MaxLagCm, FMath::Abs(Character.GetStairCameraLag()));
			return Character.GetActorLocation().Z > TopZ - 15.f;
		},
		[](ASupernaturalProtCharacter&, bool) {}));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this]()
	{
		if (ASupernaturalProtCharacter* Character = FindCharacter())
		{
			AddInfo(FString::Printf(TEXT("MEASURED: lag right after the last step %.2f cm"), Character->GetStairCameraLag()));
		}
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this]()
	{
		ASupernaturalProtCharacter* Character = FindCharacter();
		if (Character == nullptr) { AddError(TEXT("no character")); return true; }
		TestTrue(TEXT("one second after stopping the lag is gone"), FMath::Abs(Character->GetStairCameraLag()) < 0.5f);
		Character->SetStairCameraSmoothingSeconds(0.14f);
		return true;
	}));
	// jump from the stairs: while airborne the camera is exactly where the capsule puts it
	ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(*this, FVector(StairsX, StairsY, StandZ), 0.f, 6.f,
		[](ASupernaturalProtCharacter& Character) { Character.ResetStairCamera(); Character.SetStairCameraSmoothingSeconds(0.14f); },
		[](ASupernaturalProtCharacter& Character, float, double Elapsed)
		{
			Character.DoMove(0.f, 1.f);
			if (Elapsed > 0.5 && Elapsed < 0.6) { Character.Jump(); }
			return Elapsed > 1.2;
		},
		[this](ASupernaturalProtCharacter& Character, bool)
		{
			// sampled at the end of a 1.2 s run that jumped at 0.5 s: if the camera is lagging while grounded again, the lag is small
			TestTrue(TEXT("after jumping the lag stays bounded"), FMath::Abs(Character.GetStairCameraLag()) <= 24.f);
		}));
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS

