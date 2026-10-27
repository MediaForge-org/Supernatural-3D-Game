#include "Misc/AutomationTest.h"
#include "SpnMovement.h"
#include "Spn/SpnCharacterMovementComponent.h"
#include "SupernaturalProtCharacter.h"

#if WITH_DEV_AUTOMATION_TESTS

// M2-A movement tests that need no map (Docs/Proposals/0011):
//   UnrealEditor-Cmd.exe <uproject> -ExecCmds="Automation RunTests SpnGame.Movement.Settings;Quit" -unattended -nullrhi
// Map tests (jump height, speeds, obstacles, boundary, camera) are in SpnMovementMapAutomation.cpp.

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnMovementSettingsTest, "SpnGame.Movement.Settings",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)

bool FSpnMovementSettingsTest::RunTest(const FString& Parameters)
{
	const FSpnMovementSettings Defaults;
	FString Error;
	TestTrue(TEXT("default settings are valid"), SpnMovement::Validate(Defaults, Error));

	TestEqual(TEXT("walk 1.8 m/s"), SpnMovement::GaitSpeed(Defaults, ESpnGait::Walk), 180.f);
	TestEqual(TEXT("jog 3.5 m/s"), SpnMovement::GaitSpeed(Defaults, ESpnGait::Jog), 350.f);
	TestEqual(TEXT("run 5.5 m/s"), SpnMovement::GaitSpeed(Defaults, ESpnGait::Run), 550.f);
	TestEqual(TEXT("sprint 8.0 m/s"), SpnMovement::GaitSpeed(Defaults, ESpnGait::Sprint), 800.f);
	TestEqual(TEXT("default jump height 45 cm"), Defaults.JumpHeight, 45.f);
	TestEqual(TEXT("default step height 25 cm (not the jump height)"), Defaults.MaxStepHeight, 25.f);
	TestEqual(TEXT("default sprint start angle"), Defaults.SprintMaxAngleDegrees, 50.f);
	TestTrue(TEXT("sprint sustain angle is not below the start angle"), Defaults.SprintSustainAngleDegrees >= Defaults.SprintMaxAngleDegrees);

	// Engine default gravity is 980 cm/s^2; a 45 cm apex needs about 297 cm/s (the prototype's 500 cm/s gave 127 cm).
	const float Launch = SpnMovement::JumpVelocityForHeight(Defaults.JumpHeight, 980.f);
	TestTrue(FString::Printf(TEXT("launch velocity for 45 cm is about 297 cm/s (got %.1f)"), Launch), FMath::IsNearlyEqual(Launch, 297.0f, 1.5f));
	TestTrue(TEXT("apex round trip"), FMath::IsNearlyEqual(SpnMovement::JumpApexHeight(Launch, 980.f), 45.f, 0.1f));
	TestTrue(TEXT("the old 500 cm/s jump was about 127 cm"), FMath::IsNearlyEqual(SpnMovement::JumpApexHeight(500.f, 980.f), 127.55f, 0.5f));
	TestEqual(TEXT("no gravity gives no launch velocity"), SpnMovement::JumpVelocityForHeight(45.f, 0.f), 0.f);

	FSpnMovementSettings Broken = Defaults;
	Broken.RunSpeed = Broken.JogSpeed; // not strictly ascending
	TestFalse(TEXT("equal gait speeds are rejected"), SpnMovement::Validate(Broken, Error));
	TestFalse(TEXT("the rejection has a reason"), Error.IsEmpty());
	Broken = Defaults;
	Broken.DefaultGait = ESpnGait::Sprint;
	TestFalse(TEXT("Sprint as default gait is rejected"), SpnMovement::Validate(Broken, Error));
	Broken = Defaults;
	Broken.JumpHeight = 0.f;
	TestFalse(TEXT("zero jump height is rejected"), SpnMovement::Validate(Broken, Error));

	// Gait resolution through the bridge: sprint needs forward input; a refused sprint keeps the chosen base gait.
	TestTrue(TEXT("sprint forward"), SpnMovement::ResolveGait(Defaults, ESpnGait::Jog, true, ESpnGait::Sprint, FVector2D(1.0, 0.0)) == ESpnGait::Sprint);
	TestTrue(TEXT("sprint backwards falls back to the base gait"), SpnMovement::ResolveGait(Defaults, ESpnGait::Jog, true, ESpnGait::Sprint, FVector2D(-1.0, 0.0)) == ESpnGait::Jog);
	TestTrue(TEXT("a gait ceiling caps the sprint"), SpnMovement::ResolveGait(Defaults, ESpnGait::Run, true, ESpnGait::Jog, FVector2D(1.0, 0.0)) == ESpnGait::Jog);

	// Rejected new fields and the independence of step and jump height in the validation.
	Broken = Defaults;
	Broken.SprintSustainAngleDegrees = Defaults.SprintMaxAngleDegrees - 1.f;
	TestFalse(TEXT("a sustain angle below the start angle is rejected"), SpnMovement::Validate(Broken, Error));
	Broken = Defaults;
	Broken.TurnRateDegrees = 0.f;
	TestFalse(TEXT("zero turn rate is rejected"), SpnMovement::Validate(Broken, Error));
	Broken = Defaults;
	Broken.MaxStepHeight = Defaults.JumpHeight + 20.f;
	TestTrue(TEXT("a step height above the jump height is valid (the two are independent)"), SpnMovement::Validate(Broken, Error));
	Broken = Defaults;
	Broken.JumpHeight = 100.f;
	TestTrue(TEXT("a higher jump height leaves the step height alone and is valid"), SpnMovement::Validate(Broken, Error) && Broken.MaxStepHeight == Defaults.MaxStepHeight);

	// Gait resolution through the bridge: sprint needs forward input; a refused sprint keeps the chosen base gait.
	TestTrue(TEXT("sprint forward"), SpnMovement::ResolveGait(Defaults, ESpnGait::Jog, true, ESpnGait::Sprint, FVector2D(1.0, 0.0)) == ESpnGait::Sprint);
	TestTrue(TEXT("sprint backwards falls back to the base gait"), SpnMovement::ResolveGait(Defaults, ESpnGait::Jog, true, ESpnGait::Sprint, FVector2D(-1.0, 0.0)) == ESpnGait::Jog);
	TestTrue(TEXT("a gait ceiling caps the sprint"), SpnMovement::ResolveGait(Defaults, ESpnGait::Run, true, ESpnGait::Jog, FVector2D(1.0, 0.0)) == ESpnGait::Jog);

	// Sprint through a sharp turn: refused when starting from a walk, kept at sprint speed.
	TestTrue(TEXT("a 90 degree sprint start is refused"), SpnMovement::ResolveGait(Defaults, ESpnGait::Jog, true, ESpnGait::Sprint, FVector2D(0.0, 1.0), 0.f) == ESpnGait::Jog);
	TestTrue(TEXT("a 90 degree turn keeps the sprint at sprint speed"), SpnMovement::ResolveGait(Defaults, ESpnGait::Jog, true, ESpnGait::Sprint, FVector2D(0.0, 1.0), 800.f) == ESpnGait::Sprint);
	TestTrue(TEXT("a turn nearly back ends the sprint"), SpnMovement::ResolveGait(Defaults, ESpnGait::Jog, true, ESpnGait::Sprint, FVector2D(-0.866, 0.5), 800.f) == ESpnGait::Jog);

	// Observed gait and movement states through the bridge (cm/s).
	TestTrue(TEXT("observed gait at 310 cm/s is Jog"), SpnMovement::ObservedGait(Defaults, 310.f) == ESpnGait::Jog);
	TestTrue(TEXT("observed gait at 790 cm/s is Sprint"), SpnMovement::ObservedGait(Defaults, 790.f) == ESpnGait::Sprint);
	TestTrue(TEXT("standing is Idle"), SpnMovement::ClassifyMotion(Defaults, 0.f, 0.f, true, -1.f) == ESpnMovementState::Idle);
	TestTrue(TEXT("500 cm/s on the ground is Run"), SpnMovement::ClassifyMotion(Defaults, 500.f, 0.f, true, -1.f) == ESpnMovementState::Run);
	TestTrue(TEXT("rising in the air is Jumping"), SpnMovement::ClassifyMotion(Defaults, 0.f, 297.f, false, -1.f) == ESpnMovementState::Jumping);
	TestTrue(TEXT("falling in the air is Falling"), SpnMovement::ClassifyMotion(Defaults, 300.f, -200.f, false, -1.f) == ESpnMovementState::Falling);
	TestTrue(TEXT("0.1 s after touch-down is Landing"), SpnMovement::ClassifyMotion(Defaults, 300.f, 0.f, true, 0.1f) == ESpnMovementState::Landing);
	TestTrue(TEXT("0.3 s after touch-down the gait state is back"), SpnMovement::ClassifyMotion(Defaults, 300.f, 0.f, true, 0.3f) == ESpnMovementState::Jog);

	// The nine heights of the M2-A.1 work order against the 25 cm step height and the 45 cm jump apex. The classes are a rule of
	// thumb about the height; "JumpCandidate" does not promise that a jump gets over the ledge.
	struct FHeightCase
	{
		float Height;
		ESpnObstacleClass Expected;
	};
	const FHeightCase HeightCases[] = {
		{20.f, ESpnObstacleClass::StepUp},
		{25.f, ESpnObstacleClass::StepUp},
		{30.f, ESpnObstacleClass::JumpCandidate},
		{40.f, ESpnObstacleClass::JumpCandidate},
		{45.f, ESpnObstacleClass::Blocked}, // the apex itself is reached for an instant only
		{50.f, ESpnObstacleClass::Blocked},
		{75.f, ESpnObstacleClass::Blocked},
		{100.f, ESpnObstacleClass::Blocked},
		{150.f, ESpnObstacleClass::Blocked},
	};
	for (const FHeightCase& Case : HeightCases)
	{
		TestTrue(FString::Printf(TEXT("%.0f cm obstacle classification"), Case.Height), SpnMovement::ClassifyObstacle(Defaults, Case.Height) == Case.Expected);
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnMovementCharacterDefaultsTest, "SpnGame.Movement.CharacterDefaults",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)

bool FSpnMovementCharacterDefaultsTest::RunTest(const FString& Parameters)
{
	const ASupernaturalProtCharacter* Defaults = GetDefault<ASupernaturalProtCharacter>();
	if (!TestNotNull(TEXT("character default object"), Defaults))
	{
		return false;
	}
	TestNotNull(TEXT("the character uses USpnCharacterMovementComponent"), Defaults->GetSpnMovement());
	TestEqual(TEXT("one jump, no double jump"), Defaults->JumpMaxCount, 1);
	TestEqual(TEXT("no variable jump height"), Defaults->JumpMaxHoldTime, 0.f);
	if (const USpnCharacterMovementComponent* Movement = Defaults->GetSpnMovement())
	{
		TestEqual(TEXT("default jump height 45 cm"), Movement->Settings.JumpHeight, 45.f);
		TestEqual(TEXT("default step height 25 cm"), Movement->Settings.MaxStepHeight, 25.f);
	}
	return true;
}

// The gait request travels in the compressed move flags: round trip, untrusted values, and move merging.
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnMovementPredictionFlagsTest, "SpnGame.Movement.PredictionFlags",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)

bool FSpnMovementPredictionFlagsTest::RunTest(const FString& Parameters)
{
	USpnCharacterMovementComponent* Receiver = NewObject<USpnCharacterMovementComponent>(GetTransientPackage());
	if (!TestNotNull(TEXT("movement component"), Receiver))
	{
		return false;
	}

	for (const ESpnGait Base : {ESpnGait::Walk, ESpnGait::Jog, ESpnGait::Run})
	{
		for (const bool bSprint : {false, true})
		{
			FSavedMove_SpnCharacter Move;
			Move.Clear();
			Move.SavedBaseGait = static_cast<uint8>(Base);
			Move.bSavedSprintRequested = bSprint;
			const uint8 Flags = Move.GetCompressedFlags();

			Receiver->SetBaseGait(ESpnGait::Walk);
			Receiver->SetSprintRequested(!bSprint);
			Receiver->UpdateFromCompressedFlags(Flags); // what the server does with the client's flags
			TestTrue(FString::Printf(TEXT("base gait %d survives the flags"), static_cast<int32>(Base)), Receiver->GetBaseGait() == Base);
			TestTrue(FString::Printf(TEXT("sprint %d survives the flags"), bSprint ? 1 : 0), Receiver->IsSprintRequested() == bSprint);
		}
	}

	// A client cannot ask for Sprint as a base gait (wire value 3): the server reads it as Run.
	FSavedMove_SpnCharacter Forged;
	Forged.Clear();
	Forged.SavedBaseGait = static_cast<uint8>(ESpnGait::Sprint);
	Receiver->UpdateFromCompressedFlags(Forged.GetCompressedFlags());
	TestTrue(TEXT("a forged base gait Sprint is read as Run"), Receiver->GetBaseGait() == ESpnGait::Run);

	// Moves with a different request are never merged into one.
	const FSavedMovePtr Jog = MakeShared<FSavedMove_SpnCharacter>();
	const FSavedMovePtr Run = MakeShared<FSavedMove_SpnCharacter>();
	static_cast<FSavedMove_SpnCharacter*>(Run.Get())->SavedBaseGait = static_cast<uint8>(ESpnGait::Run);
	const FSavedMove_SpnCharacter* JogMove = static_cast<const FSavedMove_SpnCharacter*>(Jog.Get());
	TestFalse(TEXT("moves of different gaits are not combined"), JogMove->CanCombineWith(Run, nullptr, 0.1f));
	return true;
}

#endif
