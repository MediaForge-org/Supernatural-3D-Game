#include "Components/CapsuleComponent.h"
#include "Components/LightComponent.h"
#include "EngineUtils.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Spn/SpnBuildingObjects.h"
#include "Spn/SpnInteractionComponent.h"
#include "Spn/SpnTestDoor.h"
#include "Spn/SpnTestPickup.h"
#include "Spn/UI/SpnUiText.h"
#include "Spn/Tests/SpnMovementTestSupport.h"
#include "Spn/Tests/SpnQaRecorder.h"
#include "Spn/Tests/SpnTestPlayReset.h"
#include "SpnWorldStateSubsystem.h"

#if WITH_DEV_AUTOMATION_TESTS

// SpnQA.Building (NightBuild04): one human-like visit of Lvl_SpnBuildingTest with a real character, real key events (E = interact, F = force; they
// go player controller -> Enhanced Input -> interaction component -> candidate pick -> ASpnTestDoor / ASpnTestKey / ASpnLightSwitch / pickup) and
// real walking (W key). The journey: porch steps, front door, light switch, standing in the doorway (the door must not close through the player),
// the narrow archive door, the locked study door (refused, key-less Force refused), the key on the table, unlock + enter the study, the note in
// the study, the inner door, forcing the locked storage door from inside, and forcing the back door to leave. Then the state is saved, the world
// state is cleared, the level is reopened (fresh actors in their authored state) and the save is loaded: every door, the key, the switch and the
// pickup must be as they were. Nothing calls SpnUse / SpnForce or sets a door state directly during the journey.
// Level layout: Tools/create_spn_building_test_map.py. PLACEHOLDER geometry; the numbers are not tuning.

using namespace SpnMovementTest;

namespace
{
	constexpr EAutomationTestFlags BuildingFlags = EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter;
	const TCHAR* const Suite = TEXT("HouseJourney");
	const TCHAR* const QaSaveSlot = TEXT("spn_qa_building");
	const TCHAR* const BuildingMap = TEXT("/Game/SpnTest/Lvl_SpnBuildingTest");
	constexpr float FloorZ = 15.f;                       // interior floor top
	constexpr float EyeZ = 15.f + 97.f + 64.f;           // capsule centre on the interior floor + eye offset (approx.)

	struct FRun;
	using FCheckFn = TFunction<TPair<bool, FString>(FRun&)>;
	using FDoFn = TFunction<void(FRun&)>;

	enum class EOp : uint8 { Section, Wait, Walk, Face, Press, Shot, Check, Do };

	struct FOp
	{
		EOp Kind = EOp::Wait;
		FVector2D P = FVector2D::ZeroVector;   // Walk: waypoint
		float A = 0.f;                         // Walk: tolerance, Wait: seconds, Face: pitch override (unused)
		FString Id;                            // Face/Press: actor name, Check: check id, Section: title
		FKey Key;                              // Press
		FVector Target = FVector::ZeroVector;  // Face: point to look at
		FCheckFn Check;
		FDoFn Do;
	};

	struct FRun
	{
		TArray<FOp> Ops;
		SpnQa::FHeldKeys Keys;
		ASupernaturalProtCharacter* Character = nullptr;
		USpnInteractionComponent* Interaction = nullptr;
		int32 Index = -1;
		int32 Frames = 0;
		double StepStart = 0.0;
		FString Failure;
		float MaxStepZ = 0.f;
		float MinStepZ = 10000.f;
		FString PromptAtLook;

		template <typename T> T* Find(const TCHAR* StableId) const;
		ASpnTestDoor* Door(const TCHAR* StableId) const;
		bool Chosen(const AActor* Actor) const { return Interaction != nullptr && Interaction->FindTarget() == Actor; }
	};

	ASpnTestDoor* FRun::Door(const TCHAR* StableId) const
	{
		for (TActorIterator<ASpnTestDoor> It(Character->GetWorld()); It; ++It) { if (It->StableId == StableId) { return *It; } }
		return nullptr;
	}

	template <>
	ASpnLightSwitch* FRun::Find<ASpnLightSwitch>(const TCHAR* StableId) const
	{
		for (TActorIterator<ASpnLightSwitch> It(Character->GetWorld()); It; ++It) { if (It->StableId == StableId) { return *It; } }
		return nullptr;
	}

	template <>
	ASpnTestKey* FRun::Find<ASpnTestKey>(const TCHAR* KeyId) const
	{
		for (TActorIterator<ASpnTestKey> It(Character->GetWorld()); It; ++It) { if (It->KeyId == KeyId) { return *It; } }
		return nullptr;
	}

	template <>
	ASpnTestPickup* FRun::Find<ASpnTestPickup>(const TCHAR* StableId) const
	{
		for (TActorIterator<ASpnTestPickup> It(Character->GetWorld()); It; ++It) { if (It->StableId == StableId) { return *It; } }
		return nullptr;
	}

	FVector Centre(const AActor& Actor) { return Actor.GetComponentsBoundingBox(/*bNonColliding*/ false).GetCenter(); }

	void FaceTarget(ASupernaturalProtCharacter& Character, const FVector& Target)
	{
		const FVector Eye = Character.GetActorLocation() + FVector(0.f, 0.f, 64.f);
		const FVector To = Target - Eye;
		Character.GetController()->SetControlRotation(FRotator(FMath::RadiansToDegrees(FMath::Atan2(To.Z, To.Size2D())), FMath::RadiansToDegrees(FMath::Atan2(To.Y, To.X)), 0.f));
	}

	FOp MakeOp(EOp Kind) { FOp Op; Op.Kind = Kind; return Op; }

	struct FBuilder
	{
		TArray<FOp>& Ops;
		void Section(const TCHAR* Title) { FOp Op = MakeOp(EOp::Section); Op.Id = Title; Ops.Add(Op); }
		void Wait(float Seconds) { FOp Op = MakeOp(EOp::Wait); Op.A = Seconds; Ops.Add(Op); }
		void Walk(float X, float Y, float Tolerance = 30.f) { FOp Op = MakeOp(EOp::Walk); Op.P = FVector2D(X, Y); Op.A = Tolerance; Ops.Add(Op); }
		void Face(FVector Target) { FOp Op = MakeOp(EOp::Face); Op.Target = Target; Ops.Add(Op); }
		void Press(const FKey& Key, FVector Target) { FOp Op = MakeOp(EOp::Press); Op.Key = Key; Op.Target = Target; Ops.Add(Op); }
		void Shot(const TCHAR* Name) { FOp Op = MakeOp(EOp::Shot); Op.Id = Name; Ops.Add(Op); }
		void Check(const TCHAR* Id, FCheckFn Fn) { FOp Op = MakeOp(EOp::Check); Op.Id = Id; Op.Check = MoveTemp(Fn); Ops.Add(Op); }
		void Do(FDoFn Fn) { FOp Op = MakeOp(EOp::Do); Op.Do = MoveTemp(Fn); Ops.Add(Op); }
	};

	TPair<bool, FString> Result(bool bOk, const FString& Detail) { return TPair<bool, FString>(bOk, Detail); }

	FString DoorText(const ASpnTestDoor& Door)
	{
		return FString::Printf(TEXT("%s: open %d, locked %d, broken %d, leaf %.0f/%.0f"), *Door.StableId, Door.IsOpen(), Door.IsLocked(), Door.IsBroken(), Door.GetCurrentAngle(), Door.GetTargetAngle());
	}

	bool Settled(const ASpnTestDoor& Door) { return FMath::IsNearlyEqual(Door.GetCurrentAngle(), Door.GetTargetAngle(), 1.f); }

	/** Point on the leaf / object the player aims at (its bounding-box centre; a closed door leaf is 200 cm high, aim at hand height). */
	FVector DoorAim(const ASpnTestDoor& Door) { FVector C = Centre(Door); C.Z = FloorZ + 100.f; return C; }

	TArray<FOp> BuildJourney()
	{
		TArray<FOp> Ops;
		FBuilder B{Ops};
		const FVector FrontAim(-300.f, 0.f, FloorZ + 100.f);

		B.Section(TEXT("Building: outside, porch, front door"));
		B.Wait(2.0f);
		B.Shot(TEXT("01_exterior_dusk_porch_lamp"));
		B.Walk(-300, -190, 25);
		B.Face(FrontAim);
		B.Wait(0.5f);
		B.Check(TEXT("front_door_prompt_open"), [](FRun& R) { return Result(R.Interaction->GetCurrentPrompt().Contains(FSpnUiText::Get(TEXT("ui.door.open")).ToString()) && R.Chosen(R.Door(TEXT("front_door"))), FString::Printf(TEXT("prompt [%s] target %s, front_door is %s"), *R.Interaction->GetCurrentPrompt(), *GetNameSafe(R.Interaction->FindTarget()), *GetNameSafe(R.Door(TEXT("front_door"))))); });
		B.Walk(-300, -100, 25);
		B.Walk(-300, -25, 20);                   // over both porch steps
		B.Check(TEXT("porch_steps_climbed"), [](FRun& R) { const float Z = R.Character->GetActorLocation().Z; return Result(Z > FloorZ + 80.f && R.Character->GetCharacterMovement()->IsMovingOnGround(), FString::Printf(TEXT("capsule Z %.0f on the porch, on ground %d"), Z, R.Character->GetCharacterMovement()->IsMovingOnGround())); });
		B.Face(FrontAim);
		B.Wait(0.3f);
		B.Check(TEXT("front_door_closed_before"), [](FRun& R) { const ASpnTestDoor* D = R.Door(TEXT("front_door")); return Result(!D->IsOpen(), DoorText(*D)); });
		B.Press(EKeys::E, FrontAim);
		B.Wait(1.4f);
		B.Check(TEXT("front_door_opens_with_e"), [](FRun& R) { const ASpnTestDoor* D = R.Door(TEXT("front_door")); return Result(D->IsOpen() && Settled(*D) && FMath::Abs(D->GetCurrentAngle()) > 80.f, DoorText(*D)); });
		B.Shot(TEXT("02_front_door_open"));

		B.Section(TEXT("Building: doorway and foyer"));
		B.Walk(-300, 5, 20);
		B.Wait(0.4f);
		B.Press(EKeys::E, FVector(-355.f, 50.f, FloorZ + 100.f));   // aim at the open leaf: closing through the player must be refused
		B.Wait(1.0f);
		B.Check(TEXT("door_never_closes_through_the_player"), [](FRun& R) { const ASpnTestDoor* D = R.Door(TEXT("front_door")); return Result(D->IsOpen(), DoorText(*D)); });
		B.Walk(-300, 150, 30);
		B.Check(TEXT("inside_the_foyer"), [](FRun& R) { const FVector L = R.Character->GetActorLocation(); return Result(L.Y > 60.f && L.Z > FloorZ + 80.f && L.Z < FloorZ + 120.f, FString::Printf(TEXT("character at %.0f, %.0f, %.0f"), L.X, L.Y, L.Z)); });
		B.Shot(TEXT("03_foyer_lit"));

		B.Section(TEXT("Building: light switch"));
		B.Walk(-230, 110, 25);
		B.Face(FVector(-230.f, 12.f, 140.f));
		B.Wait(0.4f);
		B.Check(TEXT("switch_prompt_and_chosen"), [](FRun& R) { return Result(R.Chosen(R.Find<ASpnLightSwitch>(TEXT("foyer_light"))) && R.Interaction->GetCurrentPrompt().Contains(TEXT("ausschalten")), FString::Printf(TEXT("prompt '%s'"), *R.Interaction->GetCurrentPrompt())); });
		B.Press(EKeys::E, FVector(-230.f, 12.f, 140.f));
		B.Wait(0.4f);
		B.Check(TEXT("switch_turns_the_light_off"), [](FRun& R) { const ASpnLightSwitch* S = R.Find<ASpnLightSwitch>(TEXT("foyer_light")); const ULightComponent* L = S->Lights[0]->FindComponentByClass<ULightComponent>(); return Result(!S->IsOn() && !L->IsVisible(), FString::Printf(TEXT("switch on %d, light visible %d"), S->IsOn(), L->IsVisible())); });
		B.Shot(TEXT("04_foyer_light_off"));
		B.Press(EKeys::E, FVector(-230.f, 12.f, 140.f));
		B.Wait(0.4f);
		B.Check(TEXT("switch_turns_the_light_on"), [](FRun& R) { const ASpnLightSwitch* S = R.Find<ASpnLightSwitch>(TEXT("foyer_light")); const ULightComponent* L = S->Lights[0]->FindComponentByClass<ULightComponent>(); return Result(S->IsOn() && L->IsVisible(), FString::Printf(TEXT("switch on %d, light visible %d"), S->IsOn(), L->IsVisible())); });

		B.Section(TEXT("Building: the locked study door (without the key)"));
		B.Walk(-350, 330, 30);
		B.Face(FVector(-355.f, 450.f, FloorZ + 100.f));
		B.Wait(0.4f);
		B.Check(TEXT("study_door_prompt_locked"), [](FRun& R) { return Result(R.Interaction->GetCurrentPrompt().Contains(FSpnUiText::Get(TEXT("ui.door.locked")).ToString()) && !R.Interaction->GetCurrentPrompt().Contains(FSpnUiText::Get(TEXT("ui.door.force")).ToString()), FString::Printf(TEXT("prompt '%s'"), *R.Interaction->GetCurrentPrompt())); });
		B.Press(EKeys::E, FVector(-355.f, 450.f, FloorZ + 100.f));
		B.Wait(0.5f);
		B.Check(TEXT("study_door_refuses_use_without_key"), [](FRun& R) { const ASpnTestDoor* D = R.Door(TEXT("study_door")); return Result(!D->IsOpen() && D->IsLocked(), DoorText(*D)); });
		B.Press(EKeys::F, FVector(-355.f, 450.f, FloorZ + 100.f));
		B.Wait(0.5f);
		B.Check(TEXT("study_door_cannot_be_forced"), [](FRun& R) { const ASpnTestDoor* D = R.Door(TEXT("study_door")); return Result(!D->IsOpen() && D->IsLocked() && !D->IsBroken(), DoorText(*D)); });
		B.Shot(TEXT("05_study_door_locked"));

		B.Section(TEXT("Building: archive door, narrow passage, the key"));
		B.Walk(-120, 225, 30);
		B.Face(FVector(0.f, 225.f, FloorZ + 100.f));
		B.Wait(0.3f);
		B.Press(EKeys::E, FVector(0.f, 225.f, FloorZ + 100.f));
		B.Wait(1.4f);
		B.Check(TEXT("archive_door_opens"), [](FRun& R) { const ASpnTestDoor* D = R.Door(TEXT("archive_door")); return Result(D->IsOpen() && Settled(*D), DoorText(*D)); });
		B.Walk(-20, 225, 25);
		B.Walk(60, 225, 25);                      // beside the bench: a narrow passage
		B.Walk(180, 225, 30);
		B.Check(TEXT("through_the_narrow_passage"), [](FRun& R) { const FVector L = R.Character->GetActorLocation(); return Result(L.X > 120.f && L.Y > 150.f && L.Y < 300.f, FString::Printf(TEXT("character at %.0f, %.0f, %.0f"), L.X, L.Y, L.Z)); });
		B.Walk(300, 215, 30);
		B.Face(FVector(300.f, 90.f, FloorZ + 77.f));
		B.Wait(0.4f);
		B.Check(TEXT("key_prompt_and_chosen"), [](FRun& R) { return Result(R.Chosen(R.Find<ASpnTestKey>(TEXT("key.study"))), FString::Printf(TEXT("prompt '%s'"), *R.Interaction->GetCurrentPrompt())); });
		B.Press(EKeys::E, FVector(300.f, 90.f, FloorZ + 77.f));
		B.Wait(0.4f);
		B.Check(TEXT("key_is_taken"), [](FRun& R) { const ASpnTestKey* K = R.Find<ASpnTestKey>(TEXT("key.study")); return Result(K->IsHeld() && K->IsHidden(), FString::Printf(TEXT("held %d, hidden %d"), K->IsHeld(), K->IsHidden())); });
		B.Shot(TEXT("06_key_taken"));

		B.Section(TEXT("Building: unlock the study with the key"));
		B.Walk(180, 225, 30);
		B.Walk(60, 225, 25);
		B.Walk(-120, 240, 30);
		B.Walk(-350, 330, 30);
		B.Face(FVector(-355.f, 450.f, FloorZ + 100.f));
		B.Wait(0.4f);
		B.Check(TEXT("study_door_prompt_unlock"), [](FRun& R) { return Result(R.Interaction->GetCurrentPrompt().Contains(FSpnUiText::Get(TEXT("ui.door.unlock")).ToString()), FString::Printf(TEXT("prompt '%s'"), *R.Interaction->GetCurrentPrompt())); });
		B.Press(EKeys::E, FVector(-355.f, 450.f, FloorZ + 100.f));
		B.Wait(0.4f);
		B.Check(TEXT("study_door_unlocks_but_stays_shut"), [](FRun& R) { const ASpnTestDoor* D = R.Door(TEXT("study_door")); return Result(!D->IsLocked() && !D->IsOpen(), DoorText(*D)); });
		B.Press(EKeys::E, FVector(-355.f, 450.f, FloorZ + 100.f));
		B.Wait(1.4f);
		B.Check(TEXT("study_door_opens"), [](FRun& R) { const ASpnTestDoor* D = R.Door(TEXT("study_door")); return Result(D->IsOpen() && Settled(*D), DoorText(*D)); });
		B.Walk(-350, 440, 25);
		B.Walk(-350, 600, 30);
		B.Check(TEXT("inside_the_study"), [](FRun& R) { const FVector L = R.Character->GetActorLocation(); return Result(L.Y > 480.f, FString::Printf(TEXT("character at %.0f, %.0f, %.0f"), L.X, L.Y, L.Z)); });
		B.Shot(TEXT("07_study"));

		B.Section(TEXT("Building: the note in the study"));
		B.Walk(-460, 720, 30);
		B.Face(FVector(-460.f, 820.f, FloorZ + 83.f));
		B.Wait(0.4f);
		B.Press(EKeys::E, FVector(-460.f, 820.f, FloorZ + 83.f));
		B.Wait(0.5f);
		B.Check(TEXT("study_note_is_taken"), [](FRun& R) { const ASpnTestPickup* P = R.Find<ASpnTestPickup>(TEXT("study_note")); return Result(P != nullptr && P->IsTaken(), TEXT("pickup study_note taken")); });

		B.Section(TEXT("Building: inner door, forcing the storage door from inside"));
		B.Walk(-290, 700, 30);          // well outside the swing of the inner door (it opens towards -X)
		B.Face(FVector(-100.f, 675.f, FloorZ + 100.f));
		B.Press(EKeys::E, FVector(-100.f, 675.f, FloorZ + 100.f));
		B.Wait(1.4f);
		B.Check(TEXT("inner_door_opens"), [](FRun& R) { const ASpnTestDoor* D = R.Door(TEXT("inner_door")); return Result(D->IsOpen() && Settled(*D), DoorText(*D)); });
		B.Walk(-150, 692, 25);
		B.Walk(-60, 685, 25);
		B.Walk(60, 675, 30);
		B.Walk(250, 600, 30);                      // the storage, beside the locked storage door
		B.Face(FVector(250.f, 450.f, FloorZ + 100.f));
		B.Wait(0.4f);
		B.Check(TEXT("storage_door_offers_force"), [](FRun& R) { return Result(R.Interaction->GetCurrentPrompt().Contains(FSpnUiText::Get(TEXT("ui.door.force")).ToString()), FString::Printf(TEXT("prompt '%s'"), *R.Interaction->GetCurrentPrompt())); });
		B.Press(EKeys::E, FVector(250.f, 450.f, FloorZ + 100.f));
		B.Wait(0.4f);
		B.Check(TEXT("storage_door_use_is_refused"), [](FRun& R) { const ASpnTestDoor* D = R.Door(TEXT("storage_door")); return Result(!D->IsOpen() && D->IsLocked(), DoorText(*D)); });
		B.Press(EKeys::F, FVector(250.f, 450.f, FloorZ + 100.f));
		B.Wait(1.4f);
		B.Check(TEXT("storage_door_breaks_with_f"), [](FRun& R) { const ASpnTestDoor* D = R.Door(TEXT("storage_door")); return Result(D->IsBroken() && D->IsOpen() && !D->IsLocked() && Settled(*D), DoorText(*D)); });
		B.Shot(TEXT("08_storage_door_broken"));
		B.Walk(250, 520, 30);
		B.Walk(250, 400, 30);
		B.Check(TEXT("walked_through_the_broken_door"), [](FRun& R) { const FVector L = R.Character->GetActorLocation(); return Result(L.Y < 430.f && L.Z > FloorZ + 80.f, FString::Printf(TEXT("character at %.0f, %.0f, %.0f"), L.X, L.Y, L.Z)); });

		B.Section(TEXT("Building: second access, the back door from outside"));
		B.Walk(250, 250, 40);
		B.Walk(100, 228, 30);
		B.Walk(-60, 228, 30);
		B.Walk(-120, 240, 30);
		B.Walk(-300, 130, 40);
		B.Walk(-300, -25, 30);
		B.Walk(-300, -250, 40);
		B.Walk(700, -250, 60);
		B.Walk(700, 1050, 80);
		B.Walk(450, 1050, 60);
		B.Walk(450, 1000, 30);
		B.Face(FVector(445.f, 900.f, FloorZ + 100.f));
		B.Wait(0.5f);
		B.Check(TEXT("back_door_offers_force"), [](FRun& R) { return Result(R.Interaction->GetCurrentPrompt().Contains(FSpnUiText::Get(TEXT("ui.door.force")).ToString()), FString::Printf(TEXT("prompt '%s'"), *R.Interaction->GetCurrentPrompt())); });
		B.Press(EKeys::F, FVector(445.f, 900.f, FloorZ + 100.f));
		B.Wait(1.6f);
		B.Check(TEXT("back_door_breaks_with_f"), [](FRun& R) { const ASpnTestDoor* D = R.Door(TEXT("back_door")); return Result(D->IsBroken() && D->IsOpen() && Settled(*D), DoorText(*D)); });
		B.Shot(TEXT("09_back_door_broken"));
		B.Walk(450, 940, 30);
		B.Walk(450, 820, 40);
		B.Check(TEXT("entered_through_the_back_door"), [](FRun& R) { const FVector L = R.Character->GetActorLocation(); return Result(L.Y < 880.f && L.Z > FloorZ + 80.f, FString::Printf(TEXT("character at %.0f, %.0f, %.0f"), L.X, L.Y, L.Z)); });

		B.Section(TEXT("Building: repeated interaction at a door"));
		B.Walk(450, 700, 30);
		B.Face(FVector(445.f, 900.f, FloorZ + 100.f));
		B.Wait(0.3f);
		B.Do([](FRun& R) { R.Frames = 0; });
		for (int32 Index = 0; Index < 5; ++Index)
		{
			B.Press(EKeys::E, FVector(445.f, 900.f, FloorZ + 100.f));
			B.Wait(1.3f);
		}
		B.Check(TEXT("five_presses_close_the_open_door"), [](FRun& R) { const ASpnTestDoor* D = R.Door(TEXT("back_door")); return Result(!D->IsOpen() && Settled(*D) && D->IsBroken() && !D->IsLocked(), DoorText(*D)); });
		B.Press(EKeys::E, FVector(445.f, 900.f, FloorZ + 100.f));
		B.Wait(1.3f);
		B.Check(TEXT("back_door_open_again"), [](FRun& R) { const ASpnTestDoor* D = R.Door(TEXT("back_door")); return Result(D->IsOpen() && Settled(*D), DoorText(*D)); });
		B.Do([](FRun& R)
		{
			// Save the whole journey's state; the second part of the test reopens the level.
			USpnWorldStateSubsystem* State = R.Character->GetGameInstance()->GetSubsystem<USpnWorldStateSubsystem>();
			if (State == nullptr || !State->SaveToSlot(QaSaveSlot).IsOk()) { R.Failure = TEXT("saving the world state failed"); }
		});
		return Ops;
	}

	void CleanShot(const FString& Id)
	{
		if (FApp::CanEverRender())
		{
			FScreenshotRequest::RequestScreenshot(SpnQa::QaDir() / TEXT("shots") / (FString(TEXT("Building_")) + Id + TEXT(".png")), /*bShowUI*/ false, /*bAddFilenameSuffix*/ false);
		}
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnQaBuildingTest, "SpnQA.HouseJourney", BuildingFlags)

bool FSpnQaBuildingTest::RunTest(const FString& Parameters)
{
	if (!FPackageName::DoesPackageExist(BuildingMap))
	{
		AddError(TEXT("/Game/SpnTest/Lvl_SpnBuildingTest does not exist: run create_spn_building_test_map.py first."));
		return false;
	}
	AutomationOpenMap(BuildingMap);
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([]
	{
		if (const UWorld* World = FindPlayWorld()) { if (APlayerController* PC = World->GetFirstPlayerController()) { SpnTestPlay::ResetWorldState(*PC); } }
		return true;
	}));

	const TSharedRef<FRun> Run = MakeShared<FRun>();
	Run->Ops = BuildJourney();
	ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(*this, FVector(-300.f, -600.f, StandZ), 90.f, 420.f,
		[Run](ASupernaturalProtCharacter& Character)
		{
			Character.GetSpnMovement()->SetBaseGait(ESpnGait::Jog);
			Run->Character = &Character;
			Run->Interaction = Character.FindComponentByClass<USpnInteractionComponent>();
		},
		[this, Run](ASupernaturalProtCharacter& Character, float, double Elapsed)
		{
			FRun& R = *Run;
			APlayerController* PC = Cast<APlayerController>(Character.GetController());
			const FVector Loc = Character.GetActorLocation();
			if (R.Index < 0) { R.Index = 0; R.StepStart = Elapsed; R.Frames = 0; }
			if (!R.Failure.IsEmpty()) { return true; }
			if (!R.Ops.IsValidIndex(R.Index)) { return true; }
			FOp& Op = R.Ops[R.Index];
			const double InStep = Elapsed - R.StepStart;
			const auto Next = [&]() { ++R.Index; R.StepStart = Elapsed; R.Frames = 0; };
			if (Op.Kind != EOp::Walk) { R.Keys.Set(Character, EKeys::W, false); }

			switch (Op.Kind)
			{
			case EOp::Section:
				SpnQa::Section(Op.Id);
				Next();
				break;
			case EOp::Wait:
				if (InStep >= Op.A) { Next(); }
				break;
			case EOp::Walk:
			{
				const FVector2D To = Op.P - FVector2D(Loc.X, Loc.Y);
				if (To.Size() <= Op.A) { R.Keys.Set(Character, EKeys::W, false); Next(); break; }
				if (InStep > 30.0)
				{
					R.Keys.Set(Character, EKeys::W, false);
					R.Failure = FString::Printf(TEXT("stuck %.0f cm from the waypoint (%.0f, %.0f) at (%.0f, %.0f, %.0f), step %d"), To.Size(), Op.P.X, Op.P.Y, Loc.X, Loc.Y, Loc.Z, R.Index);
					return true;
				}
				PC->SetControlRotation(FRotator(-5.f, FMath::RadiansToDegrees(FMath::Atan2(Op.P.Y - Loc.Y, Op.P.X - Loc.X)), 0.f));
				R.Keys.Set(Character, EKeys::W, true);
				break;
			}
			case EOp::Face:
				FaceTarget(Character, Op.Target);
				Next();
				break;
			case EOp::Press:
				FaceTarget(Character, Op.Target);
				if (R.Frames == 0 && Character.GetVelocity().Size2D() > 15.f && InStep < 3.0) { break; }
				SpnQa::Key(*PC, Op.Key, R.Frames == 0);
				if (++R.Frames > 1) { Next(); }
				break;
			case EOp::Shot:
				CleanShot(Op.Id);
				Next();
				break;
			case EOp::Check:
			{
				const TPair<bool, FString> Verdict = Op.Check(R);
				SpnQa::Check(*this, &Character, Suite, Op.Id, Verdict.Key ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail, Verdict.Value);
				Next();
				break;
			}
			case EOp::Do:
				Op.Do(R);
				Next();
				break;
			}
			return !R.Ops.IsValidIndex(R.Index);
		},
		[this, Run](ASupernaturalProtCharacter& Character, bool bTimedOut)
		{
			Run->Keys.ReleaseAll(Character);
			if (!Run->Failure.IsEmpty() || bTimedOut)
			{
				SpnQa::Check(*this, &Character, Suite, TEXT("journey"), SpnQa::EStatus::Blocked, Run->Failure.IsEmpty() ? FString(TEXT("timeout of the building journey")) : Run->Failure);
				return;
			}
			SpnQa::Check(*this, &Character, Suite, TEXT("journey"), SpnQa::EStatus::Pass, FString::Printf(TEXT("all %d steps of the journey ran"), Run->Ops.Num()));
		}));

	// Persistence: clear the live state, reopen the level (fresh actors in their authored state), then load the saved state.
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([]
	{
		if (const UWorld* World = FindPlayWorld()) { if (APlayerController* PC = World->GetFirstPlayerController()) { SpnTestPlay::ResetWorldState(*PC); } }
		return true;
	}));
	AutomationOpenMap(BuildingMap);
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.5f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this]
	{
		const UWorld* World = FindPlayWorld();
		ASupernaturalProtCharacter* Character = FindCharacter();
		if (World == nullptr || Character == nullptr) { return false; }
		const auto Door = [World](const TCHAR* Id) -> ASpnTestDoor* { for (TActorIterator<ASpnTestDoor> It(World); It; ++It) { if (It->StableId == Id) { return *It; } } return nullptr; };
		const ASpnTestDoor* Back = Door(TEXT("back_door"));
		const ASpnTestDoor* Study = Door(TEXT("study_door"));
		const bool bFresh = Back != nullptr && Study != nullptr && Back->IsLocked() && !Back->IsBroken() && !Back->IsOpen() && Study->IsLocked();
		SpnQa::Check(*this, Character, Suite, TEXT("reopened_level_starts_in_the_authored_state"), bFresh ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail,
			Back != nullptr ? DoorText(*Back) : FString(TEXT("back door missing")));
		USpnWorldStateSubsystem* State = Character->GetGameInstance()->GetSubsystem<USpnWorldStateSubsystem>();
		const bool bLoaded = State != nullptr && State->LoadFromSlot(QaSaveSlot).IsOk();
		SpnQa::Check(*this, Character, Suite, TEXT("save_loads_into_the_reopened_level"), bLoaded ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail, TEXT("LoadFromSlot after reopening the level"));
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.5f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this]
	{
		const UWorld* World = FindPlayWorld();
		ASupernaturalProtCharacter* Character = FindCharacter();
		if (World == nullptr || Character == nullptr) { return false; }
		const auto Door = [World](const TCHAR* Id) -> ASpnTestDoor* { for (TActorIterator<ASpnTestDoor> It(World); It; ++It) { if (It->StableId == Id) { return *It; } } return nullptr; };
		const auto Report = [&](const TCHAR* Id, bool bOk, const FString& Detail) { SpnQa::Check(*this, Character, Suite, Id, bOk ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail, Detail); };

		const ASpnTestDoor* Back = Door(TEXT("back_door"));
		const ASpnTestDoor* Storage = Door(TEXT("storage_door"));
		const ASpnTestDoor* Study = Door(TEXT("study_door"));
		const ASpnTestDoor* Archive = Door(TEXT("archive_door"));
		const ASpnTestDoor* Front = Door(TEXT("front_door"));
		Report(TEXT("after_load_back_door_broken_and_open"), Back && Back->IsBroken() && Back->IsOpen() && !Back->IsLocked() && FMath::Abs(Back->GetCurrentAngle()) > 80.f, Back ? DoorText(*Back) : TEXT("missing"));
		Report(TEXT("after_load_storage_door_broken_and_open"), Storage && Storage->IsBroken() && Storage->IsOpen() && !Storage->IsLocked(), Storage ? DoorText(*Storage) : TEXT("missing"));
		Report(TEXT("after_load_study_door_unlocked_and_open"), Study && !Study->IsLocked() && Study->IsOpen() && !Study->IsBroken(), Study ? DoorText(*Study) : TEXT("missing"));
		Report(TEXT("after_load_archive_door_open"), Archive && Archive->IsOpen(), Archive ? DoorText(*Archive) : TEXT("missing"));
		Report(TEXT("after_load_front_door_open"), Front && Front->IsOpen(), Front ? DoorText(*Front) : TEXT("missing"));

		bool bKeyGone = false, bNote = false, bLightOn = false;
		for (TActorIterator<ASpnTestKey> It(World); It; ++It) { bKeyGone = It->IsHeld() && It->IsHidden(); }
		for (TActorIterator<ASpnTestPickup> It(World); It; ++It) { bNote = It->StableId == TEXT("study_note") && It->IsTaken(); }
		for (TActorIterator<ASpnLightSwitch> It(World); It; ++It) { if (It->StableId == TEXT("foyer_light")) { bLightOn = It->IsOn() && It->Lights[0]->FindComponentByClass<ULightComponent>()->IsVisible(); } }
		Report(TEXT("after_load_key_stays_taken"), bKeyGone, TEXT("key.study held and hidden"));
		Report(TEXT("after_load_note_stays_taken"), bNote, TEXT("pickup study_note taken"));
		Report(TEXT("after_load_foyer_light_is_on"), bLightOn, TEXT("foyer switch on, lamp visible"));
		CleanShot(TEXT("10_after_load_back_door"));
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.5f));
	return true;
}

#endif
