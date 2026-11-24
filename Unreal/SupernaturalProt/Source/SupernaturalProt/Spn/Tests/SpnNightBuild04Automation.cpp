#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "HAL/FileManager.h"
#include "InputCoreTypes.h"
#include "Misc/AutomationTest.h"
#include "Misc/Paths.h"
#include "Spn/SpnBuildingObjects.h"
#include "Spn/SpnInteractionComponent.h"
#include "Spn/SpnTestDoor.h"
#include "Spn/UI/SpnUiText.h"
#include "SpnInteractionPick.h"
#include "SpnWorldStateSubsystem.h"
#include "SupernaturalProtCharacter.h"

#if WITH_DEV_AUTOMATION_TESTS

// NightBuild04: door states (locked / key / forced / blocked), key and light switch objects, the candidate pick of the interaction component
// (walls, range, view angle, priority), persistence through the world state, and the input keys of Interact and Force.
// Headless game world without a map; the rendered, walked journey is the SpnQA / gameplay validation run.

namespace
{
	constexpr EAutomationTestFlags Flags =
		EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter;

	/** The prompt checks below look for German words: pin the UI language for the test and restore the user's choice afterwards
	 *  (otherwise the result depends on the engine culture / saved setting: a -game run on a de-DE Windows falls back to the 'en' culture
	 *  and showed "Door (locked)"). Create it after the test world, whose settings subsystems re-apply the saved language on start. */
	struct FGermanUi
	{
		FString Previous = FSpnUiText::GetLanguageOverride();
		FGermanUi() { FSpnUiText::SetLanguageOverride(TEXT("de")); }
		~FGermanUi() { FSpnUiText::SetLanguageOverride(Previous); }
	};

	struct FNb04World
	{
		UGameInstance* GameInstance = nullptr;
		UWorld* World = nullptr;
		TArray<AActor*> Spawned;

		FNb04World()
		{
			GameInstance = NewObject<UGameInstance>(GEngine);
			GameInstance->AddToRoot();
			GameInstance->InitializeStandalone();
			World = GameInstance->GetWorld();
		}

		~FNb04World()
		{
			for (AActor* Actor : Spawned)
			{
				if (IsValid(Actor))
				{
					Actor->Destroy();
				}
			}
			GameInstance->Shutdown();
			GEngine->DestroyWorldContext(World);
			World->DestroyWorld(/*bInformEngineOfWorld*/ false);
			GameInstance->RemoveFromRoot();
		}

		USpnWorldStateSubsystem& State() const { return *GameInstance->GetSubsystem<USpnWorldStateSubsystem>(); }

		ASpnTestDoor* Door(const TCHAR* Id, const FVector& Location, bool bLocked, bool bBreak, const TCHAR* Key = TEXT(""), bool bBlocked = false, bool bOpen = false)
		{
			FActorSpawnParameters Params;
			Params.bDeferConstruction = true;
			ASpnTestDoor* Door = World->SpawnActor<ASpnTestDoor>(ASpnTestDoor::StaticClass(), FTransform(Location), Params);
			Door->StableId = Id;
			Door->bAuthoredLocked = bLocked;
			Door->bBreakable = bBreak;
			Door->RequiredKeyId = Key;
			Door->bAuthoredBlocked = bBlocked;
			Door->bAuthoredOpen = bOpen;
			Door->FinishSpawning(FTransform(Location));
			Door->DispatchBeginPlay();
			Spawned.Add(Door);
			return Door;
		}

		ASpnTestKey* Key(const TCHAR* KeyId, const FVector& Location)
		{
			ASpnTestKey* Key = World->SpawnActor<ASpnTestKey>(ASpnTestKey::StaticClass(), FTransform(Location));
			Key->KeyId = KeyId;
			Key->DispatchBeginPlay();
			Spawned.Add(Key);
			return Key;
		}

		USpnInteractionComponent* Interactor(const FVector& Location, const FRotator& Rotation)
		{
			AActor* Owner = World->SpawnActor<AActor>();
			USceneComponent* Root = NewObject<USceneComponent>(Owner, TEXT("Root"));
			Owner->SetRootComponent(Root);
			Root->RegisterComponent();
			Owner->SetActorLocationAndRotation(Location, Rotation);
			USpnInteractionComponent* Interaction = NewObject<USpnInteractionComponent>(Owner, TEXT("Interaction"));
			Interaction->RegisterComponent();
			Spawned.Add(Owner);
			return Interaction;
		}

		/** A solid wall (engine cube, 100 cm scaled). */
		AActor* Wall(const FVector& Centre, const FVector& Size)
		{
			AActor* Wall = World->SpawnActor<AActor>(AActor::StaticClass(), FTransform(Centre));
			UStaticMeshComponent* Mesh = NewObject<UStaticMeshComponent>(Wall, TEXT("Mesh"));
			Wall->SetRootComponent(Mesh);
			Mesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube")));
			Mesh->SetRelativeScale3D(Size / 100.f);
			Mesh->SetCollisionProfileName(TEXT("BlockAll"));
			Wall->SetActorLocation(Centre);   // an AActor spawned without a root component ignores the spawn transform
			Mesh->RegisterComponent();
			Spawned.Add(Wall);
			return Wall;
		}
	};

	struct FSlot
	{
		FString Name;
		explicit FSlot(const TCHAR* InName) : Name(InName) { Delete(); }
		~FSlot() { Delete(); }
		void Delete() const { IFileManager::Get().DeleteDirectory(*FPaths::GetPath(USpnWorldStateSubsystem::GetSlotFilePath(Name)), false, true); }
	};

	void Finish(ASpnTestDoor& Door)
	{
		for (int32 Step = 0; Step < 100 && Door.IsActorTickEnabled(); ++Step)
		{
			Door.Tick(0.05f);
		}
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNb04LockedKeyTest, "SpnGame.NightBuild04.DoorLockedKeyUnlock", Flags)
bool FSpnNb04LockedKeyTest::RunTest(const FString& Parameters)
{
	FNb04World Game;
	FGermanUi German;   // after the world: its subsystems apply the saved language when they start
	ASpnTestDoor* Door = Game.Door(TEXT("cellar"), FVector::ZeroVector, /*bLocked*/ true, /*bBreak*/ false, TEXT("key.cellar"));
	ASpnTestKey* Key = Game.Key(TEXT("key.cellar"), FVector(0.f, -300.f, 50.f));

	TestTrue(TEXT("locked at start"), Door->IsLocked());
	TestFalse(TEXT("no key yet"), Door->HasRequiredKey());
	TestTrue(*FString::Printf(TEXT("prompt says locked (got '%s')"), *Door->SpnPromptText(nullptr)), Door->SpnPromptText(nullptr).Contains(TEXT("verschlossen")));
	TestTrue(TEXT("no force prompt on an unbreakable door"), Door->SpnForcePromptText(nullptr).IsEmpty());

	const FSpnInteractStatus Refused = Door->SpnUse(nullptr);
	TestEqual(TEXT("Use on a locked door is refused"), Refused.Result, ESpnInteractResult::Refused);
	TestFalse(TEXT("still closed"), Door->IsOpen());
	TestEqual(TEXT("Force on an unbreakable door is refused"), Door->SpnForce(nullptr).Result, ESpnInteractResult::Refused);
	TestFalse(TEXT("still closed after Force"), Door->IsOpen());

	TestFalse(TEXT("key not held"), Key->IsHeld());
	TestEqual(TEXT("take the key"), Key->SpnUse(nullptr).Result, ESpnInteractResult::Changed);
	TestTrue(TEXT("key held"), Key->IsHeld());
	TestTrue(TEXT("key is hidden"), Key->IsHidden());
	TestTrue(TEXT("door sees the key"), Door->HasRequiredKey());
	TestTrue(TEXT("prompt says unlock"), Door->SpnPromptText(nullptr).Contains(TEXT("aufschlie")));

	TestEqual(TEXT("Use unlocks"), Door->SpnUse(nullptr).Result, ESpnInteractResult::Changed);
	TestFalse(TEXT("unlocked"), Door->IsLocked());
	TestFalse(TEXT("but still closed"), Door->IsOpen());
	TestEqual(TEXT("Use opens"), Door->SpnUse(nullptr).Result, ESpnInteractResult::Changed);
	Finish(*Door);
	TestTrue(TEXT("open"), Door->IsOpen());
	TestEqual(TEXT("leaf swung to the open angle"), Door->GetCurrentAngle(), Door->OpenAngle);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNb04ForceTest, "SpnGame.NightBuild04.DoorForceBreaks", Flags)
bool FSpnNb04ForceTest::RunTest(const FString& Parameters)
{
	FNb04World Game;
	ASpnTestDoor* Door = Game.Door(TEXT("back"), FVector::ZeroVector, /*bLocked*/ true, /*bBreak*/ true);

	TestTrue(TEXT("force prompt offered"), !Door->SpnForcePromptText(nullptr).IsEmpty());
	TestEqual(TEXT("Use refused"), Door->SpnUse(nullptr).Result, ESpnInteractResult::Refused);
	TestEqual(TEXT("Force breaks it"), Door->SpnForce(nullptr).Result, ESpnInteractResult::Changed);
	Finish(*Door);
	TestTrue(TEXT("broken"), Door->IsBroken());
	TestFalse(TEXT("no longer locked"), Door->IsLocked());
	TestTrue(TEXT("open"), Door->IsOpen());
	TestTrue(TEXT("no force prompt any more"), Door->SpnForcePromptText(nullptr).IsEmpty());
	TestEqual(TEXT("Force again is refused"), Door->SpnForce(nullptr).Result, ESpnInteractResult::Refused);

	// It still closes and opens, and the lock is gone for good.
	TestEqual(TEXT("close"), Door->SpnUse(nullptr).Result, ESpnInteractResult::Changed);
	Finish(*Door);
	TestFalse(TEXT("closed"), Door->IsOpen());
	TestFalse(TEXT("closed but not locked"), Door->IsLocked());
	TestEqual(TEXT("open again"), Door->SpnUse(nullptr).Result, ESpnInteractResult::Changed);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNb04BlockedTest, "SpnGame.NightBuild04.DoorBlockedRefusesBoth", Flags)
bool FSpnNb04BlockedTest::RunTest(const FString& Parameters)
{
	FNb04World Game;
	FGermanUi German;   // after the world: its subsystems apply the saved language when they start
	ASpnTestDoor* Door = Game.Door(TEXT("jammed"), FVector::ZeroVector, /*bLocked*/ true, /*bBreak*/ true, TEXT(""), /*bBlocked*/ true);
	TestTrue(TEXT("blocked"), Door->IsBlocked());
	TestTrue(*FString::Printf(TEXT("prompt says blocked (got '%s')"), *Door->SpnPromptText(nullptr)), Door->SpnPromptText(nullptr).Contains(TEXT("blockiert")));
	TestTrue(TEXT("no force prompt"), Door->SpnForcePromptText(nullptr).IsEmpty());
	TestEqual(TEXT("Use refused"), Door->SpnUse(nullptr).Result, ESpnInteractResult::Refused);
	TestEqual(TEXT("Force refused"), Door->SpnForce(nullptr).Result, ESpnInteractResult::Refused);
	TestFalse(TEXT("nothing changed"), Door->IsOpen());
	TestFalse(TEXT("not broken"), Door->IsBroken());

	// Clearing the block (a mechanism's job later) makes it a normal locked breakable door.
	Game.State().SetFlag(USpnWorldStateSubsystem::TestDoorDisabledKey(TEXT("jammed")), false, TEXT("test"));
	TestFalse(TEXT("no longer blocked"), Door->IsBlocked());
	TestEqual(TEXT("Force now breaks it"), Door->SpnForce(nullptr).Result, ESpnInteractResult::Changed);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNb04ClosingObstructedTest, "SpnGame.NightBuild04.DoorDoesNotCloseOnPawn", Flags)
bool FSpnNb04ClosingObstructedTest::RunTest(const FString& Parameters)
{
	FNb04World Game;
	ASpnTestDoor* Door = Game.Door(TEXT("jamb"), FVector::ZeroVector, false, false, TEXT(""), false, /*bOpen*/ true);
	TestTrue(TEXT("authored open"), Door->IsOpen());
	// A pawn standing where the open leaf stands. The leaf is the BlockAll box; a pawn-type overlap query needs a pawn, so this checks the state
	// side: closing a door with nothing in the way works, and a refused closing changes nothing.
	TestFalse(TEXT("nothing in the swing"), Door->IsClosingObstructed());
	TestEqual(TEXT("closes"), Door->SpnUse(nullptr).Result, ESpnInteractResult::Changed);
	Finish(*Door);
	TestFalse(TEXT("closed"), Door->IsOpen());
	TestEqual(TEXT("closed angle is 0"), Door->GetCurrentAngle(), 0.f);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNb04PersistTest, "SpnGame.NightBuild04.PersistenceAcrossSaveLoad", Flags)
bool FSpnNb04PersistTest::RunTest(const FString& Parameters)
{
	const FSlot Slot(TEXT("spn_automation_nb04"));
	{
		FNb04World Game;
		ASpnTestDoor* Front = Game.Door(TEXT("front"), FVector(0.f, 0.f, 0.f), true, true);
		ASpnTestDoor* Cellar = Game.Door(TEXT("cellar"), FVector(500.f, 0.f, 0.f), true, false, TEXT("key.cellar"));
		ASpnTestDoor* Hall = Game.Door(TEXT("hall"), FVector(1000.f, 0.f, 0.f), false, false);
		ASpnTestKey* Key = Game.Key(TEXT("key.cellar"), FVector(0.f, 400.f, 50.f));
		Front->SpnForce(nullptr);            // broken + open
		Key->SpnUse(nullptr);                // key taken
		Cellar->SpnUse(nullptr);             // unlocked, still closed
		Hall->SpnUse(nullptr);               // opened
		if (!TestTrue(TEXT("save"), Game.State().SaveToSlot(Slot.Name).IsOk()))
		{
			return false;
		}
	}
	{
		// A new session: fresh actors in their authored state, then the load puts the saved state back.
		FNb04World Game;
		ASpnTestDoor* Front = Game.Door(TEXT("front"), FVector(0.f, 0.f, 0.f), true, true);
		ASpnTestDoor* Cellar = Game.Door(TEXT("cellar"), FVector(500.f, 0.f, 0.f), true, false, TEXT("key.cellar"));
		ASpnTestDoor* Hall = Game.Door(TEXT("hall"), FVector(1000.f, 0.f, 0.f), false, false);
		ASpnTestKey* Key = Game.Key(TEXT("key.cellar"), FVector(0.f, 400.f, 50.f));
		TestTrue(TEXT("fresh: front locked"), Front->IsLocked());
		TestFalse(TEXT("fresh: key lying there"), Key->IsHeld());
		if (!TestTrue(TEXT("load"), Game.State().LoadFromSlot(Slot.Name).IsOk()))
		{
			return false;
		}
		TestTrue(TEXT("front: still broken"), Front->IsBroken());
		TestFalse(TEXT("front: not locked"), Front->IsLocked());
		TestTrue(TEXT("front: open"), Front->IsOpen());
		TestEqual(TEXT("front: shown open at once"), Front->GetCurrentAngle(), Front->OpenAngle);
		TestFalse(TEXT("cellar: unlocked"), Cellar->IsLocked());
		TestFalse(TEXT("cellar: closed"), Cellar->IsOpen());
		TestTrue(TEXT("hall: open"), Hall->IsOpen());
		TestTrue(TEXT("key: taken"), Key->IsHeld());
		TestTrue(TEXT("key: hidden again"), Key->IsHidden());
		TestEqual(TEXT("cellar opens after the load"), Cellar->SpnUse(nullptr).Result, ESpnInteractResult::Changed);
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNb04SwitchTest, "SpnGame.NightBuild04.LightSwitchAndPersistence", Flags)
bool FSpnNb04SwitchTest::RunTest(const FString& Parameters)
{
	const FSlot Slot(TEXT("spn_automation_nb04_switch"));
	const auto SpawnLamp = [](FNb04World& Game)
	{
		AActor* Lamp = Game.World->SpawnActor<AActor>();
		UPointLightComponent* Light = NewObject<UPointLightComponent>(Lamp, TEXT("Light"));
		Lamp->SetRootComponent(Light);
		Light->RegisterComponent();
		Game.Spawned.Add(Lamp);
		return Lamp;
	};
	const auto SpawnSwitch = [](FNb04World& Game, AActor* Lamp)
	{
		ASpnLightSwitch* Switch = Game.World->SpawnActor<ASpnLightSwitch>(ASpnLightSwitch::StaticClass(), FTransform(FVector(0.f, 0.f, 120.f)));
		Switch->StableId = TEXT("hall_light");
		Switch->bAuthoredOn = true;
		Switch->Lights.Add(Lamp);
		Switch->DispatchBeginPlay();
		Game.Spawned.Add(Switch);
		return Switch;
	};
	{
		FNb04World Game;
		AActor* Lamp = SpawnLamp(Game);
		ASpnLightSwitch* Switch = SpawnSwitch(Game, Lamp);
		const ULightComponent* Light = Lamp->FindComponentByClass<ULightComponent>();
		TestTrue(TEXT("on at start"), Switch->IsOn() && Light->IsVisible());
		TestEqual(TEXT("toggle off"), Switch->SpnUse(nullptr).Result, ESpnInteractResult::Changed);
		TestFalse(TEXT("off"), Switch->IsOn());
		TestFalse(TEXT("the light follows"), Light->IsVisible());
		for (int32 Index = 0; Index < 6; ++Index)   // repeated use stays consistent
		{
			Switch->SpnUse(nullptr);
		}
		TestFalse(TEXT("an even number of further uses leaves it off"), Switch->IsOn());
		TestFalse(TEXT("light still off"), Light->IsVisible());
		TestTrue(TEXT("saved"), Game.State().SaveToSlot(Slot.Name).IsOk());
	}
	{
		FNb04World Game;
		AActor* Lamp = SpawnLamp(Game);
		ASpnLightSwitch* Switch = SpawnSwitch(Game, Lamp);
		const ULightComponent* Light = Lamp->FindComponentByClass<ULightComponent>();
		TestTrue(TEXT("a fresh session starts on"), Switch->IsOn() && Light->IsVisible());
		TestTrue(TEXT("load"), Game.State().LoadFromSlot(Slot.Name).IsOk());
		TestFalse(TEXT("loaded: off"), Switch->IsOn());
		TestFalse(TEXT("loaded: the light is off"), Light->IsVisible());
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNb04PickTest, "SpnGame.NightBuild04.InteractionPickWallsRangeAnglePriority", Flags)
bool FSpnNb04PickTest::RunTest(const FString& Parameters)
{
	FNb04World Game;
	// Eyes at 160 cm height looking along +Y. Doors are 100 cm wide leaves yawed -90 so they face the viewer (hinge at +50).
	const FVector Eye(0.f, 0.f, 160.f);
	USpnInteractionComponent* Interaction = Game.Interactor(Eye, FRotator(0.f, 90.f, 0.f));
	ASpnTestDoor* Near = Game.Door(TEXT("near"), FVector(50.f, 150.f, 0.f), false, false);
	Near->SetActorRotation(FRotator(0.f, -90.f, 0.f));
	ASpnTestDoor* Side = Game.Door(TEXT("side"), FVector(250.f, 140.f, 0.f), false, false);
	Side->SetActorRotation(FRotator(0.f, -90.f, 0.f));

	TestTrue(TEXT("the door ahead is found"), Interaction->FindTargetAlong(Eye, FVector(0.f, 1.f, 0.f)) == Near);
	TestNull(TEXT("looking the other way finds nothing"), Interaction->FindTargetAlong(Eye, FVector(0.f, -1.f, 0.f)));
	TestNull(TEXT("looking straight up finds nothing"), Interaction->FindTargetAlong(Eye, FVector(0.f, 0.f, 1.f)));
	TestTrue(TEXT("turning towards the side one picks it"), Interaction->FindTargetAlong(Eye, FVector(1.f, 0.5f, 0.f)) == Side);

	// A massive wall between the eyes and the door: nothing can be used through it.
	AActor* Wall = Game.Wall(FVector(0.f, 30.f, 100.f), FVector(600.f, 20.f, 300.f))   /* y 20..40: between the eyes (y 0) and the door (y 50..150) */;
	TestNull(TEXT("no interaction through a solid wall"), Interaction->FindTargetAlong(Eye, FVector(0.f, 1.f, 0.f)));
	Wall->Destroy();
	TestTrue(TEXT("without the wall it is found again"), Interaction->FindTargetAlong(Eye, FVector(0.f, 1.f, 0.f)) == Near);

	// Out of reach.
	TestNull(TEXT("too far away"), Interaction->FindTargetAlong(FVector(0.f, -400.f, 160.f), FVector(0.f, 1.f, 0.f)));

	// Nothing usable around: TryInteract / TryForce report that they hit nothing.
	USpnInteractionComponent* Lonely = Game.Interactor(FVector(5000.f, 5000.f, 160.f), FRotator::ZeroRotator);
	TestFalse(TEXT("TryInteract without a target"), Lonely->TryInteract());
	TestFalse(TEXT("TryForce without a target"), Lonely->TryForce());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNb04InteractVsForceTest, "SpnGame.NightBuild04.InteractAndForceAreSeparate", Flags)
bool FSpnNb04InteractVsForceTest::RunTest(const FString& Parameters)
{
	FNb04World Game;
	const FVector Eye(0.f, 0.f, 160.f);
	USpnInteractionComponent* Interaction = Game.Interactor(Eye, FRotator(0.f, 90.f, 0.f));
	ASpnTestDoor* Door = Game.Door(TEXT("front"), FVector(50.f, 150.f, 0.f), /*bLocked*/ true, /*bBreak*/ true);
	Door->SetActorRotation(FRotator(0.f, -90.f, 0.f));

	TestTrue(TEXT("Interact reaches the door"), Interaction->TryInteract());
	TestFalse(TEXT("a plain Interact never breaks it"), Door->IsBroken());
	TestFalse(TEXT("and it stays shut"), Door->IsOpen());
	TestTrue(TEXT("Force reaches the door"), Interaction->TryForce());
	TestTrue(TEXT("Force breaks it"), Door->IsBroken());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNb04InputKeysTest, "SpnGame.NightBuild04.InputKeysAreSeparate", Flags)
bool FSpnNb04InputKeysTest::RunTest(const FString& Parameters)
{
	const ASupernaturalProtCharacter::FSpnGameplayKeys Keys = GetDefault<ASupernaturalProtCharacter>()->GetGameplayKeys();
	TestEqual(TEXT("Interact keyboard key is E"), Keys.Interact, EKeys::E);
	TestEqual(TEXT("Interact on the DualSense is Square"), Keys.InteractPad, EKeys::Gamepad_FaceButton_Left);
	TestEqual(TEXT("Force keyboard key is F"), Keys.Force, EKeys::F);
	// Developer decision 2026-11-14 (supersedes "Force = Triangle" of NightBuild04): Triangle stays free for a later heavy attack; on the pad
	// Force is Square HELD (a tap is Interact), so the two verbs differ by the hold, not by the key.
	TestEqual(TEXT("Force on the DualSense is Square, held"), Keys.ForcePad, EKeys::Gamepad_FaceButton_Left);
	TestTrue(TEXT("the pad Force needs a hold"), GetDefault<ASupernaturalProtCharacter>()->GetForcePadHoldSeconds() >= 0.2f);
	TestFalse(TEXT("Triangle is not used by Interact, Force or Sprint"), Keys.InteractPad == EKeys::Gamepad_FaceButton_Top || Keys.ForcePad == EKeys::Gamepad_FaceButton_Top || Keys.SprintPad == EKeys::Gamepad_FaceButton_Top);
	TestEqual(TEXT("Sprint / gait on the DualSense is L3"), Keys.SprintPad, EKeys::Gamepad_LeftThumbstick);

	// Interact and Force never share a key with each other (keyboard), the sprint, or the jump button (Cross = Gamepad_FaceButton_Bottom).
	const TArray<FKey> Interact = {Keys.Interact, Keys.InteractPad};
	const TArray<FKey> Force = {Keys.Force};
	const TArray<FKey> Others = {Keys.Sprint, Keys.SprintPad, EKeys::SpaceBar, EKeys::Gamepad_FaceButton_Bottom, EKeys::LeftMouseButton};
	for (const FKey& Key : Interact)
	{
		TestFalse(*FString::Printf(TEXT("Interact key %s is not a Force key"), *Key.ToString()), Force.Contains(Key));
		TestFalse(*FString::Printf(TEXT("Interact key %s is not used elsewhere"), *Key.ToString()), Others.Contains(Key));
	}
	for (const FKey& Key : {Keys.Force, Keys.ForcePad})
	{
		TestFalse(*FString::Printf(TEXT("Force key %s is not used elsewhere"), *Key.ToString()), Others.Contains(Key));
	}

	// The prompt shows the keys that are actually mapped.
	const USpnInteractionComponent* Component = GetDefault<ASupernaturalProtCharacter>()->FindComponentByClass<USpnInteractionComponent>();
	if (TestNotNull(TEXT("character has the interaction component"), Component))
	{
		TestEqual(TEXT("component default prompt key"), Component->PromptKey, EKeys::E);
		TestEqual(TEXT("component default force key"), Component->ForceKey, EKeys::F);
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNb04PickReachNamesTest, "SpnGame.NightBuild04.PickCoreReachNames", Flags)
bool FSpnNb04PickReachNamesTest::RunTest(const FString& Parameters)
{
	FSpnPickConfig Config;
	FSpnPickCandidate Candidate;
	Candidate.EyeCm = FVector(0.f, 0.f, 160.f);
	Candidate.ViewDirection = FVector(0.f, 1.f, 0.f);
	Candidate.CentreCm = FVector(0.f, 150.f, 100.f);
	Candidate.RadiusCm = 50.f;
	TestEqual(TEXT("in reach"), SpnInteractionPick::ReachName(Config, Candidate), FString(TEXT("Ok")));
	Candidate.CentreCm = FVector(0.f, 900.f, 100.f);
	TestEqual(TEXT("too far"), SpnInteractionPick::ReachName(Config, Candidate), FString(TEXT("TooFar")));
	Candidate.CentreCm = FVector(0.f, -150.f, 100.f);
	TestEqual(TEXT("behind"), SpnInteractionPick::ReachName(Config, Candidate), FString(TEXT("OutOfView")));
	Candidate.CentreCm = FVector(0.f, 150.f, 100.f);
	Candidate.bLineOfSightClear = false;
	TestEqual(TEXT("behind a wall"), SpnInteractionPick::ReachName(Config, Candidate), FString(TEXT("Obstructed")));

	// Two candidates: the walled one never wins, the priority breaks a tie.
	TArray<FSpnPickCandidate> List;
	FSpnPickCandidate A;
	A.EyeCm = FVector(0.f, 0.f, 160.f);
	A.ViewDirection = FVector(0.f, 1.f, 0.f);
	A.CentreCm = FVector(0.f, 150.f, 120.f);
	A.RadiusCm = 20.f;
	FSpnPickCandidate B = A;
	B.Priority = 5;
	FSpnPickCandidate Walled = A;
	Walled.CentreCm = FVector(0.f, 100.f, 120.f);
	Walled.bLineOfSightClear = false;
	List = {Walled, A, B};
	TestEqual(TEXT("the higher priority wins a tie, the walled one is excluded"), SpnInteractionPick::Pick(Config, List), 2);
	List = {Walled};
	TestEqual(TEXT("only a walled candidate: none"), SpnInteractionPick::Pick(Config, List), (int32)INDEX_NONE);
	return true;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNb04FrameRateTest, "SpnGame.NightBuild04.DoorSwingIsFrameRateIndependent", Flags)
bool FSpnNb04FrameRateTest::RunTest(const FString& Parameters)
{
	// The same door opened and closed under very different frame rates ends in the same state after the same game time, and a use in the
	// middle of a swing reverses it from where it is (no jump, no overshoot past the open angle).
	for (const float Fps : {10.f, 30.f, 60.f, 144.f, 240.f})
	{
		FNb04World Game;
		ASpnTestDoor* Door = Game.Door(TEXT("fps"), FVector::ZeroVector, false, false);
		const float Dt = 1.f / Fps;
		Door->SpnUse(nullptr);   // open
		float Previous = Door->GetCurrentAngle();
		float Time = 0.f;
		while (Time < 0.25f)     // a quarter of a second into the swing
		{
			Door->Tick(Dt);
			Time += Dt;
			TestTrue(*FString::Printf(TEXT("%.0f fps: the angle only increases while opening"), Fps), Door->GetCurrentAngle() >= Previous);
			TestTrue(*FString::Printf(TEXT("%.0f fps: no overshoot"), Fps), Door->GetCurrentAngle() <= Door->OpenAngle + KINDA_SMALL_NUMBER);
			Previous = Door->GetCurrentAngle();
		}
		const float Expected = FMath::Min(Door->OpenAngle, Door->SwingSpeed * Time);
		TestTrue(*FString::Printf(TEXT("%.0f fps: angle %.1f is near the time-based %.1f"), Fps, Door->GetCurrentAngle(), Expected), FMath::Abs(Door->GetCurrentAngle() - Expected) <= Door->SwingSpeed * Dt + 0.5f);

		Door->SpnUse(nullptr);   // close again in the middle of the swing
		TestFalse(TEXT("closing is accepted mid-swing"), Door->IsOpen());
		const float AtReverse = Door->GetCurrentAngle();
		Door->Tick(Dt);
		TestTrue(*FString::Printf(TEXT("%.0f fps: continues from where it is, no jump"), Fps), FMath::Abs(Door->GetCurrentAngle() - AtReverse) <= Door->SwingSpeed * Dt + 0.01f);
		Finish(*Door);
		TestEqual(*FString::Printf(TEXT("%.0f fps: ends closed"), Fps), Door->GetCurrentAngle(), 0.f);
		TestFalse(TEXT("does not keep ticking once settled"), Door->IsActorTickEnabled());
	}
	return true;
}
#endif
