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

	TestEqual(TEXT("walk 1.4 m/s"), SpnMovement::GaitSpeed(Defaults, ESpnGait::Walk), 140.f);
	TestEqual(TEXT("jog 3.0 m/s"), SpnMovement::GaitSpeed(Defaults, ESpnGait::Jog), 300.f);
	TestEqual(TEXT("run 5.0 m/s"), SpnMovement::GaitSpeed(Defaults, ESpnGait::Run), 500.f);
	TestEqual(TEXT("sprint 8.0 m/s"), SpnMovement::GaitSpeed(Defaults, ESpnGait::Sprint), 800.f);
	TestEqual(TEXT("default jump height 45 cm"), Defaults.JumpHeight, 45.f);

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

	// 50 cm is not a high jump: it is above step height (45) and jump height (45), so it is blocked for now.
	TestTrue(TEXT("40 cm is a step"), SpnMovement::ClassifyObstacle(Defaults, 40.f) == ESpnObstacleClass::StepUp);
	TestTrue(TEXT("50 cm is blocked"), SpnMovement::ClassifyObstacle(Defaults, 50.f) == ESpnObstacleClass::Blocked);
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
