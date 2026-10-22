#include "Camera/CameraComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "CollisionQueryParams.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Math/RandomStream.h"
#include "Misc/App.h"
#include "Misc/AutomationTest.h"
#include "Misc/Paths.h"
#include "SupernaturalProtCharacter.h"
#include "Tests/AutomationCommon.h"
#include "UnrealClient.h"

#if WITH_DEV_AUTOMATION_TESTS

// M1 camera collision checks with real collision queries (Docs/Proposals/0008). Plays /Game/SpnTest/Lvl_SpnDoorTest
// (PIE in the editor, or -game), puts the character with its back to the M1 wall and to the stairs and checks the
// camera the player actually sees (PlayerCameraManager), not only the spring-arm settings:
//   UnrealEditor-Cmd.exe <uproject> -ExecCmds="Automation RunTests SpnGame.TestMap.M1Camera;Quit" -unattended -nullrhi
// With a renderer (no -nullrhi) it also writes screenshots to Saved/Screenshots for a visual check.

namespace
{
	const TCHAR* const TestMapPackage = TEXT("/Game/SpnTest/Lvl_SpnDoorTest");

	// Values of Tools/create_spn_door_test_map.py (kept in step by SpnGame.TestMap.M1Layout).
	constexpr float WallX = 1300.f;
	constexpr float WallY = 800.f;
	constexpr float WallThickness = 40.f;
	constexpr float StairsX = 900.f;
	constexpr float StairsY = 0.f;
	constexpr float StandZ = 97.f;
	constexpr float ProbeSlack = 6.f; // the arm keeps the probe sphere (radius 12) off the surface; allow a few cm of error

	UWorld* FindPlayWorld()
	{
		for (const FWorldContext& Context : GEngine->GetWorldContexts())
		{
			if ((Context.WorldType == EWorldType::Game || Context.WorldType == EWorldType::PIE) && Context.World() != nullptr)
			{
				return Context.World();
			}
		}
		return nullptr;
	}

	struct FCameraRun
	{
		ASupernaturalProtCharacter* Character = nullptr;
		APlayerController* Controller = nullptr;
		int32 Violations = 0;
		FString FirstViolation;
		int32 Samples = 0;
		float MinArm = TNumericLimits<float>::Max();
		float MaxCameraHeightAbovePivot = TNumericLimits<float>::Lowest();
	};
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnTestMapM1CameraTest, "SpnGame.TestMap.M1Camera",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)

bool FSpnTestMapM1CameraTest::RunTest(const FString& Parameters)
{
	AutomationOpenMap(TestMapPackage);
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));

	const TSharedRef<FCameraRun> Run = MakeShared<FCameraRun>();

	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, Run]
	{
		const UWorld* World = FindPlayWorld();
		Run->Controller = World != nullptr ? World->GetFirstPlayerController() : nullptr;
		Run->Character = Run->Controller != nullptr ? Cast<ASupernaturalProtCharacter>(Run->Controller->GetPawn()) : nullptr;
		TestNotNull(TEXT("third-person character possessed"), Run->Character);
		if (Run->Character != nullptr && FParse::Param(FCommandLine::Get(), TEXT("SpnCameraNoSmoothing")))
		{
			Run->Character->SetCameraArmRecoverySpeed(0.f); // before-the-fix behaviour: the "no pop" checks must then fail
		}
		if (Run->Character != nullptr && FParse::Param(FCommandLine::Get(), TEXT("SpnCameraNegativeControl")))
		{
			Run->Character->GetCameraBoom()->bDoCollisionTest = false; // negative control: the test must then fail
		}
		return true;
	}));

	// What the player actually sees must satisfy these for every sample: the camera is not inside blocking geometry,
	// has a clear line to the character, is not farther than the arm length from the pivot, not under the floor.
	const TSharedRef<TArray<FString>> ArmLog = MakeShared<TArray<FString>>();
	const auto CheckCamera = [this, Run, ArmLog](const FString& Label, bool bSettled = false)
	{
		if (Run->Character == nullptr)
		{
			return;
		}
		const UWorld* World = Run->Character->GetWorld();
		const USpringArmComponent* Boom = Run->Character->GetCameraBoom();
		const FVector Camera = Run->Controller->PlayerCameraManager->GetCameraLocation();
		const FVector Pivot = Boom->GetComponentLocation();
		const float Arm = FVector::Dist(Camera, Pivot);
		Run->MinArm = FMath::Min(Run->MinArm, Arm);
		Run->MaxCameraHeightAbovePivot = FMath::Max(Run->MaxCameraHeightAbovePivot, Camera.Z - Pivot.Z);
		++Run->Samples;

		FCollisionQueryParams Params(SCENE_QUERY_STAT(SpnCameraTest), false, Run->Character);
		FString Problem;
		if (World->OverlapBlockingTestByChannel(Camera, FQuat::Identity, ECC_Camera, FCollisionShape::MakeSphere(4.f), Params))
		{
			Problem = TEXT("camera is inside blocking geometry");
		}
		else if (World->LineTraceTestByChannel(Pivot, Camera, ECC_Camera, Params))
		{
			Problem = TEXT("geometry between character and camera");
		}
		else if (Arm > Run->Character->GetConfiguredArmLength() + 1.f)
		{
			Problem = FString::Printf(TEXT("camera %.0f cm from the pivot, arm length is %.0f"), Arm, Run->Character->GetConfiguredArmLength());
		}
		else if (Camera.Z < 2.f)
		{
			Problem = FString::Printf(TEXT("camera under the floor (Z %.0f)"), Camera.Z);
		}
		else if (bSettled && Run->Character->GetHideMeshCameraDistance() > 0.f)
		{
			// The character must not be seen from inside: hidden while the camera is squeezed, visible otherwise (after the tick settled).
			const float Limit = Run->Character->GetHideMeshCameraDistance();
			const bool bHidden = Run->Character->GetMesh()->bOwnerNoSee;
			if (Arm < Limit - 3.f && !bHidden)
			{
				Problem = FString::Printf(TEXT("camera only %.0f cm from the pivot but the character mesh is still drawn"), Arm);
			}
			else if (Arm > Limit + 3.f && bHidden)
			{
				Problem = FString::Printf(TEXT("camera %.0f cm from the pivot but the character mesh is hidden"), Arm);
			}
		}
		if (bSettled)
		{
			ArmLog->Add(FString::Printf(TEXT("%s: arm %.0f cm, camera %.0f cm above pivot"), *Label, Arm, Camera.Z - Pivot.Z));
		}
		if (!Problem.IsEmpty())
		{
			if (Run->Violations++ == 0)
			{
				Run->FirstViolation = FString::Printf(TEXT("%s: %s (camera %s, pivot %s)"), *Label, *Problem, *Camera.ToCompactString(), *Pivot.ToCompactString());
			}
		}
	};

	// Puts the character at Location with the control rotation (Pitch, Yaw); the camera hangs behind it.
	const auto Pose = [Run](const FVector& Location, float Pitch, float Yaw)
	{
		if (Run->Character != nullptr)
		{
			Run->Character->GetCharacterMovement()->StopMovementImmediately();
			Run->Character->SetActorLocation(Location, false, nullptr, ETeleportType::TeleportPhysics);
			Run->Controller->SetControlRotation(FRotator(Pitch, Yaw, 0.f));
		}
	};

	const auto Screenshot = [](const FString& Name)
	{
		if (FApp::CanEverRender())
		{
			FScreenshotRequest::RequestScreenshot(FPaths::ScreenShotDir() / Name + TEXT(".png"), /*bShowUI*/ false, /*bAddFilenameSuffix*/ false);
		}
	};

	// Static poses with the back to an obstacle (yaw 180: camera behind the character, i.e. towards +X).
	struct FSite { const TCHAR* Name; FVector Location; };
	const FSite Sites[] = {
		{TEXT("wall"), FVector(WallX - WallThickness / 2.f - 70.f, WallY, StandZ)},
		{TEXT("wall_close"), FVector(WallX - WallThickness / 2.f - 45.f, WallY, StandZ)},
		{TEXT("stairs"), FVector(StairsX - 60.f, StairsY, StandZ)},
	};
	const float Pitches[] = {-80.f, -70.f, -60.f, -45.f, -30.f, -20.f, -10.f, 0.f, 10.f, 20.f, 45.f, 80.f};
	for (const FSite& Site : Sites)
	{
		for (const float Pitch : Pitches)
		{
			const FString Label = FString::Printf(TEXT("%s pitch %.0f"), Site.Name, Pitch);
			const FVector Location = Site.Location;
			ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([Pose, Location, Pitch] { Pose(Location, Pitch, 180.f); return true; }));
			ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.25f));
			ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([CheckCamera, Screenshot, Label, Site, Pitch]
			{
				CheckCamera(Label, /*bSettled*/ true);
				if (FMath::IsNearlyEqual(Pitch, 0.f) || FMath::IsNearlyEqual(Pitch, -45.f) || FMath::IsNearlyEqual(Pitch, 45.f))
				{
					Screenshot(FString::Printf(TEXT("SpnCam_%s_pitch%+.0f"), Site.Name, Pitch));
				}
				return true;
			}));
			ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.15f)); // let a requested screenshot be taken
		}
	}

	// Fast mouse movement near the wall and the stairs: deterministic random look input, checked every tick.
	for (const FSite& Site : Sites)
	{
		const FVector Location = Site.Location;
		ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([Pose, Location] { Pose(Location, 0.f, 180.f); return true; }));
		ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.25f));
		const TSharedRef<int32> Tick = MakeShared<int32>(0);
		const TSharedRef<FRandomStream> Random = MakeShared<FRandomStream>(20261022);
		const FString Label = FString::Printf(TEXT("%s fast look"), Site.Name);
		ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([Run, CheckCamera, Tick, Random, Label, Location]
		{
			if (Run->Character == nullptr)
			{
				return true;
			}
			// Keep the character in place (it is the camera that is under test), spin the view fast.
			Run->Character->SetActorLocation(Location, false, nullptr, ETeleportType::TeleportPhysics);
			Run->Character->DoLook(Random->FRandRange(-40.f, 40.f), Random->FRandRange(-25.f, 25.f));
			CheckCamera(Label);
			return ++*Tick >= 240;
		}));
	}

	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.3f)); // the last queued look input of the fast-look phase must be consumed first
	// No pop: the camera is squeezed at the wall (pitch -10), then the view swings over the wall edge (pitch -25).
	// The arm must grow back gradually (rate-limited), while shortening stays immediate (checked by CheckCamera every tick).
	struct FArmSample { float Arm; float Delta; float Time; };
	const TSharedRef<TArray<FArmSample>> Recovery = MakeShared<TArray<FArmSample>>();
	const TSharedRef<double> RecoveryStart = MakeShared<double>(-1.0);
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([Pose, Sites] { Pose(Sites[0].Location, -10.f, 180.f); return true; }));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
	const TSharedRef<int32> Shots = MakeShared<int32>(0);
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, Run, CheckCamera, Recovery, RecoveryStart, Screenshot, Shots]
	{
		if (Run->Character == nullptr)
		{
			return true;
		}
		const UWorld* World = Run->Character->GetWorld();
		const float Arm = FVector::Dist(Run->Controller->PlayerCameraManager->GetCameraLocation(), Run->Character->GetCameraBoom()->GetComponentLocation());
		if (*RecoveryStart < 0.0)
		{
			AddInfo(FString::Printf(TEXT("pre-swing: character %s, control rotation %s, arm %.0f, TargetArmLength %.0f"), *Run->Character->GetActorLocation().ToCompactString(), *Run->Controller->GetControlRotation().ToCompactString(), Arm, Run->Character->GetCameraBoom()->TargetArmLength));
			TestTrue(FString::Printf(TEXT("before the swing the camera is squeezed (arm %.0f cm)"), Arm), Arm < 100.f);
			Run->Controller->SetControlRotation(FRotator(-25.f, 180.f, 0.f));
			*RecoveryStart = World->GetTimeSeconds();
			return false;
		}
		Recovery->Add({Arm, World->GetDeltaSeconds(), static_cast<float>(World->GetTimeSeconds() - *RecoveryStart)});
		{
			// Stills of the zoom-out for a visual check (with a renderer): shortly after the swing, 0.4 s, 0.9 s.
			const float Elapsed = Recovery->Last().Time;
			const float Marks[] = {0.05f, 0.4f, 0.9f};
			if (*Shots < 3 && Elapsed >= Marks[*Shots])
			{
				Screenshot(FString::Printf(TEXT("SpnCam_recovery_%d"), *Shots));
				++*Shots;
			}
		}
		CheckCamera(TEXT("recovery"));
		return World->GetTimeSeconds() - *RecoveryStart >= 2.5;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, Run, Recovery]
	{
		if (Run->Character == nullptr || !TestTrue(TEXT("recovery was sampled"), Recovery->Num() > 10))
		{
			return true;
		}
		const float Speed = Run->Character->GetCameraArmRecoverySpeed();
		const float Configured = Run->Character->GetConfiguredArmLength();
		float MaxStep = 0.f; float WorstExcess = 0.f; float FullAt = -1.f;
		for (int32 Index = 1; Index < Recovery->Num(); ++Index)
		{
			const FArmSample& Sample = (*Recovery)[Index];
			const float Step = Sample.Arm - (*Recovery)[Index - 1].Arm;
			MaxStep = FMath::Max(MaxStep, Step);
			WorstExcess = FMath::Max(WorstExcess, Step - Speed * Sample.Delta);
			if (FullAt < 0.f && Sample.Arm > Configured - 8.f) { FullAt = Sample.Time; }
		}
		AddInfo(FString::Printf(TEXT("recovery: first arm %.0f cm, largest step per frame %.1f cm, back to full length after %.2f s (speed limit %.0f cm/s)"),
			(*Recovery)[0].Arm, MaxStep, FullAt, Speed));
		TestTrue(FString::Printf(TEXT("no pop: first arm after the swing is still short (%.0f cm)"), (*Recovery)[0].Arm), (*Recovery)[0].Arm < 150.f);
		TestTrue(FString::Printf(TEXT("growth stays within the speed limit (worst excess %.1f cm per frame)"), WorstExcess), WorstExcess < 6.f);
		TestTrue(TEXT("arm grows back to its full length"), FullAt > 0.f);
		return true;
	}));
	// Shortening is immediate: swing back into the wall and check the arm within a few frames.
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([Run] { if (Run->Character != nullptr) { Run->Controller->SetControlRotation(FRotator(-10.f, 180.f, 0.f)); } return true; }));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.1f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, Run, CheckCamera]
	{
		if (Run->Character != nullptr)
		{
			const float Arm = FVector::Dist(Run->Controller->PlayerCameraManager->GetCameraLocation(), Run->Character->GetCameraBoom()->GetComponentLocation());
			TestTrue(FString::Printf(TEXT("shortening is immediate: arm %.0f cm 0.1 s after swinging back into the wall"), Arm), Arm < 100.f);
			CheckCamera(TEXT("swing back"));
		}
		return true;
	}));
	// Recovery: after leaving the obstacle the normal arm length returns (no lag, so within a few frames).
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([Pose] { Pose(FVector(-600.f, 0.f, StandZ), -20.f, 0.f); return true; }));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(2.0f)); // the arm grows back at CameraArmRecoverySpeed
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, Run, Screenshot]
	{
		if (Run->Character != nullptr)
		{
			const USpringArmComponent* Boom = Run->Character->GetCameraBoom();
			const float Arm = FVector::Dist(Run->Controller->PlayerCameraManager->GetCameraLocation(), Boom->GetComponentLocation());
			TestTrue(FString::Printf(TEXT("open floor: arm length is back to %.0f cm (measured %.1f)"), Run->Character->GetConfiguredArmLength(), Arm), FMath::Abs(Arm - Run->Character->GetConfiguredArmLength()) < 8.f);
		}
		Screenshot(TEXT("SpnCam_open_floor"));
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.2f));

	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, Run, ArmLog]
	{
		AddInfo(FString::Join(*ArmLog, TEXT("\n")));
		AddInfo(FString::Printf(TEXT("camera samples %d, shortest arm %.0f cm, highest camera above the pivot %.0f cm, violations %d"),
			Run->Samples, Run->MinArm, Run->MaxCameraHeightAbovePivot, Run->Violations));
		TestTrue(TEXT("camera was sampled"), Run->Samples > 700);
		TestEqual(*FString::Printf(TEXT("camera collision violations (first: %s)"), *Run->FirstViolation), Run->Violations, 0);
		return true;
	}));
	return true;
}

#endif
