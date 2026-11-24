#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "HAL/FileManager.h"
#include "Misc/AutomationTest.h"
#include "Misc/Guid.h"
#include "Misc/Paths.h"
#include "Spn/SpnInteractionComponent.h"
#include "Spn/SpnNpc.h"
#include "Spn/SpnTestDoor.h"
#include "SpnNpcMemorySubsystem.h"
#include "SpnNpcReaction.h"
#include "SpnSaveSubsystem.h"
#include "SpnWorldStateSubsystem.h"

#if WITH_DEV_AUTOMATION_TESTS

// NightBuild05: door swing that never pushes a pawn, the timed break-in with its noise, NPC senses (line of sight, wall-attenuated hearing),
// reaction transitions, NPC memory, several NPCs, save/load during an alarm and different frame rates. Headless game world without a map and
// without a navigation mesh (the NPCs steer straight; the reaction logic is what is tested). The rendered, walked scenario is SpnQA.NpcReaction.

namespace
{
	constexpr EAutomationTestFlags Flags = EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter;

	struct FWorld
	{
		UGameInstance* GameInstance = nullptr;
		UWorld* World = nullptr;
		TArray<AActor*> Spawned;
		FString SaveRoot;

		FWorld()
		{
			GameInstance = NewObject<UGameInstance>(GEngine);
			GameInstance->AddToRoot();
			GameInstance->InitializeStandalone();
			World = GameInstance->GetWorld();
			SaveRoot = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir() / TEXT("SpnTestTemp") / FGuid::NewGuid().ToString(EGuidFormats::Digits));
			GameInstance->GetSubsystem<USpnSaveSubsystem>()->SetSaveDirectory(SaveRoot);
		}

		~FWorld()
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
			IFileManager::Get().DeleteDirectory(*SaveRoot, false, true);
		}

		USpnNoiseSubsystem& Noise() const { return *GameInstance->GetSubsystem<USpnNoiseSubsystem>(); }
		USpnNpcMemorySubsystem& Memory() const { return *GameInstance->GetSubsystem<USpnNpcMemorySubsystem>(); }
		USpnSaveSubsystem& Save() const { return *GameInstance->GetSubsystem<USpnSaveSubsystem>(); }

		ASpnTestDoor* Door(const TCHAR* Id, const FVector& Location, bool bLocked, bool bBreak)
		{
			FActorSpawnParameters Params;
			Params.bDeferConstruction = true;
			ASpnTestDoor* Door = World->SpawnActor<ASpnTestDoor>(ASpnTestDoor::StaticClass(), FTransform(Location), Params);
			Door->StableId = Id;
			Door->bAuthoredLocked = bLocked;
			Door->bBreakable = bBreak;
			Door->FinishSpawning(FTransform(Location));
			Door->DispatchBeginPlay();
			Spawned.Add(Door);
			return Door;
		}

		ACharacter* Pawn(const FVector& Location)
		{
			ACharacter* Pawn = World->SpawnActor<ACharacter>(ACharacter::StaticClass(), FTransform(Location));
			Spawned.Add(Pawn);
			return Pawn;
		}

		AActor* Wall(const FVector& Centre, const FVector& Size)
		{
			AActor* Wall = World->SpawnActor<AActor>(AActor::StaticClass(), FTransform(Centre));
			UStaticMeshComponent* Mesh = NewObject<UStaticMeshComponent>(Wall, TEXT("Mesh"));
			Wall->SetRootComponent(Mesh);
			Mesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube")));
			Mesh->SetRelativeScale3D(Size / 100.f);
			Mesh->SetCollisionProfileName(TEXT("BlockAll"));
			Wall->SetActorLocation(Centre);
			Mesh->RegisterComponent();
			Spawned.Add(Wall);
			return Wall;
		}

		ASpnNpc* Npc(int64 Id, const TCHAR* Name, const FVector& Location, float Yaw = 0.f)
		{
			FActorSpawnParameters Params;
			Params.bDeferConstruction = true;
			ASpnNpc* Npc = World->SpawnActor<ASpnNpc>(ASpnNpc::StaticClass(), FTransform(FRotator(0.f, Yaw, 0.f), Location), Params);
			Npc->NpcId = Id;
			Npc->DisplayName = Name;
			Npc->FinishSpawning(FTransform(FRotator(0.f, Yaw, 0.f), Location));
			Npc->DispatchBeginPlay();
			Spawned.Add(Npc);
			return Npc;
		}

		void Tick(ASpnNpc& Npc, float Seconds, float Dt = 1.f / 60.f)
		{
			for (float T = 0.f; T < Seconds; T += Dt)
			{
				Memory().AdvanceWorldTime(Dt);
				Npc.Tick(Dt);
			}
		}
	};

	FSpnNoise DoorBreakNoise(const FVector& At)
	{
		FSpnNoise N;
		N.LocationCm = At;
		N.RangeCm = 2000.f;
		N.Strength = 1.f;
		N.Kind = ESpnNoiseKind::DoorBreak;
		N.ObjectId = TEXT("placeholder.door.back_door");
		N.SourceId = TEXT("player");
		return N;
	}

	FSpnNoise DoorUseNoise(const FVector& At)
	{
		FSpnNoise N = DoorBreakNoise(At);
		N.Kind = ESpnNoiseKind::DoorUse;
		N.RangeCm = 600.f;
		N.Strength = 0.35f;
		return N;
	}

	bool SameSequence(const TArray<FSpnReactionLogEntry>& A, const TArray<FSpnReactionLogEntry>& B)
	{
		if (A.Num() != B.Num())
		{
			return false;
		}
		for (int32 Index = 0; Index < A.Num(); ++Index)
		{
			if (A[Index].To != B[Index].To)
			{
				return false;
			}
		}
		return true;
	}

	FString SequenceText(const TArray<FSpnReactionLogEntry>& Log)
	{
		FString Text;
		for (const FSpnReactionLogEntry& E : Log)
		{
			Text += FString::Printf(TEXT("%s@%.2f "), LexToString(E.To), E.WorldTime);
		}
		return Text;
	}
}

// --- doors -----------------------------------------------------------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNb05DoorNoPushTest, "SpnGame.NightBuild05.DoorNeverPushesAPawn", Flags)
bool FSpnNb05DoorNoPushTest::RunTest(const FString&)
{
	FWorld W;
	ASpnTestDoor* Door = W.Door(TEXT("push_door"), FVector::ZeroVector, false, false);
	ACharacter* Standing = W.Pawn(FVector(50.f, 60.f, 100.f)); // in the arc of the leaf (78 cm from the hinge, 50 degrees)
	const FVector Before = Standing->GetActorLocation();

	TestEqual(TEXT("the door opens"), Door->SpnUse(nullptr).Result, ESpnInteractResult::Changed);
	for (int32 Frame = 0; Frame < 240; ++Frame)
	{
		Door->Tick(1.f / 60.f);
	}
	TestTrue(TEXT("the leaf stopped short of the pawn"), Door->GetCurrentAngle() < 45.f);
	TestTrue(TEXT("and is not at its target"), Door->GetCurrentAngle() < Door->GetTargetAngle() - 1.f);
	TestTrue(TEXT("it knows it is waiting for the pawn"), Door->IsWaitingForPawn());
	TestTrue(TEXT("the pawn was not moved"), Standing->GetActorLocation().Equals(Before, 0.01f));

	Standing->SetActorLocation(FVector(300.f, -300.f, 100.f));
	for (int32 Frame = 0; Frame < 240; ++Frame)
	{
		Door->Tick(1.f / 60.f);
	}
	TestTrue(TEXT("with the way free the door finishes opening"), FMath::IsNearlyEqual(Door->GetCurrentAngle(), Door->GetTargetAngle(), 0.5f));
	TestFalse(TEXT("and stops waiting"), Door->IsWaitingForPawn());

	// closing onto a pawn is refused outright (Night Build 04) and the leaf never moves into it either
	Standing->SetActorLocation(FVector(50.f, 60.f, 100.f));
	TestEqual(TEXT("closing onto the pawn is refused"), Door->SpnUse(nullptr).Result, ESpnInteractResult::Refused);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNb05DoorSwingFpsTest, "SpnGame.NightBuild05.DoorStopsAtPawnAtAnyFrameRate", Flags)
bool FSpnNb05DoorSwingFpsTest::RunTest(const FString&)
{
	float Angles[3] = {};
	const float Rates[3] = {30.f, 60.f, 144.f};
	for (int32 Index = 0; Index < 3; ++Index)
	{
		FWorld W;
		ASpnTestDoor* Door = W.Door(TEXT("fps_door"), FVector::ZeroVector, false, false);
		W.Pawn(FVector(50.f, 60.f, 100.f));
		Door->SpnUse(nullptr);
		for (float T = 0.f; T < 3.f; T += 1.f / Rates[Index])
		{
			Door->Tick(1.f / Rates[Index]);
		}
		Angles[Index] = Door->GetCurrentAngle();
		TestTrue(*FString::Printf(TEXT("%.0f fps: stopped before the pawn (%.1f)"), Rates[Index], Angles[Index]), Angles[Index] < 45.f);
	}
	TestTrue(TEXT("the stop angle hardly depends on the frame rate"), FMath::Abs(Angles[0] - Angles[2]) < 8.f && FMath::Abs(Angles[1] - Angles[2]) < 8.f);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNb05DoorNoiseTest, "SpnGame.NightBuild05.DoorEmitsNoise", Flags)
bool FSpnNb05DoorNoiseTest::RunTest(const FString&)
{
	FWorld W;
	TArray<FSpnNoise> Heard;
	W.Noise().OnNoise.AddLambda([&Heard](const FSpnNoise& N) { Heard.Add(N); });

	ASpnTestDoor* Free = W.Door(TEXT("free_door"), FVector(0.f, 0.f, 0.f), false, false);
	Free->SpnUse(nullptr);
	TestEqual(TEXT("opening a door is one noise"), Heard.Num(), 1);
	if (Heard.Num() == 1)
	{
		TestEqual(TEXT("a harmless door-use noise"), Heard[0].Kind, ESpnNoiseKind::DoorUse);
		TestFalse(TEXT("not suspicious"), SpnNpcSense::IsSuspicious(Heard[0]));
		TestEqual(TEXT("it names the door"), Heard[0].ObjectId, FString(TEXT("placeholder.door.free_door")));
	}
	Heard.Reset();

	ASpnTestDoor* Locked = W.Door(TEXT("locked_door"), FVector(1000.f, 0.f, 0.f), true, true);
	Locked->SpnUse(nullptr);
	TestEqual(TEXT("a refused Use is silent"), Heard.Num(), 0);
	const FSpnInteractStatus Forced = Locked->SpnForce(nullptr);
	TestEqual(TEXT("forcing breaks it"), Forced.Result, ESpnInteractResult::Changed);
	TestEqual(TEXT("one noise"), Heard.Num(), 1);
	if (Heard.Num() == 1)
	{
		TestEqual(TEXT("a door break"), Heard[0].Kind, ESpnNoiseKind::DoorBreak);
		TestTrue(TEXT("suspicious"), SpnNpcSense::IsSuspicious(Heard[0]));
		TestTrue(TEXT("loud and far-reaching"), Heard[0].Strength >= 0.9f && Heard[0].RangeCm >= 1500.f);
		TestTrue(TEXT("at the door"), FVector::Dist2D(Heard[0].LocationCm, FVector(1000.f, 0.f, 0.f)) < 150.f);
	}
	Heard.Reset();
	Locked->SpnForce(nullptr);
	TestEqual(TEXT("forcing an already broken door makes no new noise"), Heard.Num(), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNb05TimedBreakTest, "SpnGame.NightBuild05.TimedBreakInNeedsTheHold", Flags)
bool FSpnNb05TimedBreakTest::RunTest(const FString&)
{
	const float Rates[3] = {30.f, 60.f, 144.f};
	for (const float Rate : Rates)
	{
		FWorld W;
		int32 Noises = 0;
		W.Noise().OnNoise.AddLambda([&Noises](const FSpnNoise&) { ++Noises; });
		ASpnTestDoor* Door = W.Door(TEXT("hold_door"), FVector::ZeroVector, true, true);
		AActor* Owner = W.World->SpawnActor<AActor>();
		USceneComponent* Root = NewObject<USceneComponent>(Owner, TEXT("Root"));
		Owner->SetRootComponent(Root);
		Root->RegisterComponent();
		Owner->SetActorLocationAndRotation(FVector(50.f, -100.f, 100.f), FRotator(0.f, 90.f, 0.f));
		USpnInteractionComponent* Interaction = NewObject<USpnInteractionComponent>(Owner, TEXT("Interaction"));
		Interaction->RegisterComponent();
		W.Spawned.Add(Owner);

		const FString Tag = FString::Printf(TEXT("%.0f fps: "), Rate);
		TestTrue(Tag + TEXT("the door is the target"), Interaction->FindTarget() == Door);
		TestTrue(Tag + TEXT("the break starts"), Interaction->BeginForce());
		TestTrue(Tag + TEXT("forcing"), Interaction->IsForcing());
		TestFalse(Tag + TEXT("not broken at once"), Door->IsBroken());

		// cancelled halfway: nothing happens
		float Elapsed = 0.f;
		for (; Elapsed < Door->BreakSeconds * 0.5f; Elapsed += 1.f / Rate)
		{
			Interaction->TickComponent(1.f / Rate, LEVELTICK_All, nullptr);
		}
		TestTrue(Tag + TEXT("progress about half"), FMath::IsNearlyEqual(Interaction->GetForceProgress(), 0.5f, 0.1f));
		Interaction->EndForce();
		TestFalse(Tag + TEXT("released: not forcing"), Interaction->IsForcing());
		TestEqual(Tag + TEXT("released: progress back to 0"), Interaction->GetForceProgress(), 0.f);
		for (int32 Frame = 0; Frame < 400; ++Frame)
		{
			Interaction->TickComponent(1.f / Rate, LEVELTICK_All, nullptr);
		}
		TestFalse(Tag + TEXT("a cancelled hold breaks nothing"), Door->IsBroken());
		TestEqual(Tag + TEXT("and makes no noise"), Noises, 0);

		// held to the end: breaks once, exactly when the time has passed
		Interaction->BeginForce();
		float Held = 0.f;
		int32 Frames = 0;
		while (!Door->IsBroken() && Frames < 1000)
		{
			Interaction->TickComponent(1.f / Rate, LEVELTICK_All, nullptr);
			Held += 1.f / Rate;
			++Frames;
		}
		TestTrue(Tag + TEXT("the door broke"), Door->IsBroken());
		TestTrue(Tag + FString::Printf(TEXT("after the break time (%.2f s held, %.2f s needed)"), Held, Door->BreakSeconds),
			Held >= Door->BreakSeconds - 0.001f && Held <= Door->BreakSeconds + 2.f / Rate);
		TestFalse(Tag + TEXT("and no longer forcing"), Interaction->IsForcing());
		TestEqual(Tag + TEXT("exactly one noise"), Noises, 1);
		for (int32 Frame = 0; Frame < 100; ++Frame)
		{
			Interaction->TickComponent(1.f / Rate, LEVELTICK_All, nullptr);
		}
		TestEqual(Tag + TEXT("holding on makes no second break"), Noises, 1);
	}
	return true;
}

// --- senses ----------------------------------------------------------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNb05SightTest, "SpnGame.NightBuild05.NpcSightAndBlocking", Flags)
bool FSpnNb05SightTest::RunTest(const FString&)
{
	FWorld W;
	ASpnNpc* Npc = W.Npc(2001, TEXT("Watcher"), FVector(0.f, 0.f, 92.f), 0.f);
	ACharacter* Target = W.Pawn(FVector(800.f, 0.f, 92.f));
	Npc->SetPerceptionTarget(Target);

	Npc->SenseNow();
	TestTrue(TEXT("sees the target in the open"), Npc->SeesPlayer());

	AActor* Wall = W.Wall(FVector(400.f, 0.f, 150.f), FVector(20.f, 600.f, 300.f));
	Npc->SenseNow();
	TestFalse(TEXT("a wall in between hides it"), Npc->SeesPlayer());

	Wall->Destroy();
	Npc->SenseNow();
	TestTrue(TEXT("seen again when the wall is gone"), Npc->SeesPlayer());

	Target->SetActorLocation(FVector(-800.f, 0.f, 92.f));
	Npc->SenseNow();
	TestFalse(TEXT("not behind the NPC"), Npc->SeesPlayer());

	Target->SetActorLocation(FVector(2600.f, 0.f, 92.f));
	Npc->SenseNow();
	TestFalse(TEXT("not beyond the sight range"), Npc->SeesPlayer());

	// very close with a wall: still hidden (no X-ray at close range)
	Target->SetActorLocation(FVector(90.f, 0.f, 92.f));
	W.Wall(FVector(45.f, 0.f, 150.f), FVector(10.f, 600.f, 300.f));
	Npc->SenseNow();
	TestFalse(TEXT("a thin wall hides even a close target"), Npc->SeesPlayer());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNb05HearingTest, "SpnGame.NightBuild05.NpcHearingRangeAndWalls", Flags)
bool FSpnNb05HearingTest::RunTest(const FString&)
{
	FWorld W;
	ASpnNpc* Npc = W.Npc(2002, TEXT("Listener"), FVector(0.f, 0.f, 92.f));
	// break noise at 12 m in the open: heard (range 20 m)
	W.Noise().EmitNoise(DoorBreakNoise(FVector(1200.f, 0.f, 92.f)));
	TestEqual(TEXT("heard in the open at 12 m"), Npc->GetNoisesHeard(), 1);

	// one wall: range 10 m, noise at 12 m: not heard
	W.Wall(FVector(600.f, 0.f, 150.f), FVector(20.f, 800.f, 300.f));
	W.Noise().EmitNoise(DoorBreakNoise(FVector(1200.f, 0.f, 92.f)));
	TestEqual(TEXT("not heard at 12 m behind one wall (range halves to 10 m)"), Npc->GetNoisesNotHeard(), 1);

	// one wall, noise at 8 m: heard
	W.Noise().EmitNoise(DoorBreakNoise(FVector(800.f, 0.f, 92.f)));
	TestEqual(TEXT("heard at 8 m behind one wall"), Npc->GetNoisesHeard(), 2);

	// two walls: the range quarters (5 m): 12 m and 10 m are out; 7.5 m with just the first wall in between (range 10 m) is in
	W.Wall(FVector(900.f, 0.f, 150.f), FVector(20.f, 800.f, 300.f));
	const int32 NotHeardBefore = Npc->GetNoisesNotHeard();
	W.Noise().EmitNoise(DoorBreakNoise(FVector(1200.f, 0.f, 92.f)));
	TestEqual(TEXT("not heard at 12 m behind two walls (range 5 m)"), Npc->GetNoisesNotHeard(), NotHeardBefore + 1);
	W.Noise().EmitNoise(DoorBreakNoise(FVector(1000.f, 0.f, 92.f)));
	TestEqual(TEXT("not heard at 10 m behind two walls either"), Npc->GetNoisesNotHeard(), NotHeardBefore + 2);
	const int32 HeardBefore = Npc->GetNoisesHeard();
	W.Noise().EmitNoise(DoorBreakNoise(FVector(750.f, 0.f, 92.f)));
	TestEqual(TEXT("heard at 7.5 m between the two walls (one wall, range 10 m)"), Npc->GetNoisesHeard(), HeardBefore + 1);

	// outside the open range
	W.Noise().EmitNoise(DoorBreakNoise(FVector(0.f, 2500.f, 92.f)));
	TestEqual(TEXT("not heard at 25 m"), Npc->GetNoisesNotHeard(), NotHeardBefore + 3);

	// a harmless noise is heard but ignored by an NPC that has no reason to care (this one has not heard anything suspicious)
	ASpnNpc* Calm = W.Npc(2004, TEXT("Calm"), FVector(0.f, -3000.f, 92.f));
	W.Noise().EmitNoise(DoorUseNoise(FVector(0.f, -2800.f, 92.f)));
	W.Tick(*Calm, 0.3f);
	TestEqual(TEXT("a harmless noise changes nothing"), Calm->GetReactionState(), ESpnReactionState::Idle);
	TestEqual(TEXT("it was counted as ignored"), Calm->GetNoisesIgnored(), 1);
	TestEqual(TEXT("and nothing was remembered"), W.Memory().GetMemoryCount(2004), 0);
	return true;
}

// --- reaction, memory, several NPCs, save/load ------------------------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNb05ReactionTest, "SpnGame.NightBuild05.NpcReactionAndMemory", Flags)
bool FSpnNb05ReactionTest::RunTest(const FString&)
{
	FWorld W;
	ASpnNpc* Npc = W.Npc(2003, TEXT("Resident"), FVector(0.f, 0.f, 92.f));
	ACharacter* Player = W.Pawn(FVector(3000.f, 3000.f, 92.f)); // far away, out of sight
	Npc->SetPerceptionTarget(Player);

	W.Tick(*Npc, 1.f);
	TestEqual(TEXT("idle"), Npc->GetReactionState(), ESpnReactionState::Idle);
	TestFalse(TEXT("not wary"), Npc->IsWary());

	W.Noise().EmitNoise(DoorBreakNoise(FVector(900.f, 0.f, 92.f)));
	W.Tick(*Npc, 0.1f);
	TestEqual(TEXT("a break-in noise makes the NPC investigate"), Npc->GetReactionState(), ESpnReactionState::Investigate);
	TestEqual(TEXT("and it remembered it"), W.Memory().GetMemoryCount(2003), 1);
	TestTrue(TEXT("and is wary now"), W.Memory().Assess(2003).bWary && W.Memory().Assess(2003).bBrokeInRemembered);

	// the player appears in plain view close by -> alert + alarm, remembered, shout raised
	int32 Shouts = 0;
	W.Noise().OnNoise.AddLambda([&Shouts](const FSpnNoise& N) { if (N.Kind == ESpnNoiseKind::Shout) { ++Shouts; } });
	Player->SetActorLocation(FVector(500.f, 0.f, 92.f));
	W.Tick(*Npc, 0.3f);
	TestEqual(TEXT("sees the player: Alert"), Npc->GetReactionState(), ESpnReactionState::Alert);
	TestEqual(TEXT("one alarm"), Npc->GetAlarmsRaised(), 1);
	TestEqual(TEXT("one shout"), Shouts, 1);
	TestTrue(TEXT("the alarm is in the memory"), W.Memory().Assess(2003).bRecentAlarm);
	TestTrue(TEXT("and the sighting"), W.Memory().Assess(2003).bPlayerSeenRemembered);

	// the player comes close -> Flee
	Player->SetActorLocation(FVector(300.f, 0.f, 92.f));
	W.Tick(*Npc, 2.5f);
	TestEqual(TEXT("a wary NPC with the player close flees"), Npc->GetReactionState(), ESpnReactionState::Flee);
	TestEqual(TEXT("still only one alarm"), Npc->GetAlarmsRaised(), 1);

	// the player disappears: after the minimum flee time the NPC calms down in steps, never flipping
	Player->SetActorLocation(FVector(3000.f, 3000.f, 92.f));
	const int32 LogBefore = Npc->GetReactionLog().Num();
	W.Tick(*Npc, 120.f);
	TestEqual(TEXT("everything calmed down"), Npc->GetReactionState(), ESpnReactionState::Idle);
	const int32 Transitions = Npc->GetReactionLog().Num() - LogBefore;
	TestTrue(*FString::Printf(TEXT("in a few steps, not a flutter (%d)"), Transitions), Transitions >= 1 && Transitions <= 6);
	TestTrue(TEXT("the memory remains after the state is gone"), W.Memory().Assess(2003).bWary);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNb05MultipleNpcTest, "SpnGame.NightBuild05.SeveralNpcsReactIndependently", Flags)
bool FSpnNb05MultipleNpcTest::RunTest(const FString&)
{
	FWorld W;
	ACharacter* Player = W.Pawn(FVector(5000.f, 5000.f, 92.f));
	ASpnNpc* Near = W.Npc(3001, TEXT("Near"), FVector(0.f, 0.f, 92.f));
	ASpnNpc* Walled = W.Npc(3002, TEXT("Walled"), FVector(0.f, 700.f, 92.f));
	ASpnNpc* Far = W.Npc(3003, TEXT("Far"), FVector(0.f, 2600.f, 92.f));
	for (ASpnNpc* Npc : {Near, Walled, Far})
	{
		Npc->SetPerceptionTarget(Player);
	}
	W.Wall(FVector(0.f, 350.f, 150.f), FVector(1200.f, 20.f, 300.f)); // between Near and Walled

	// noise at (0, -400): Near hears it in the open (4 m); Walled is 11 m away behind one wall (range 10 m): no; Far is 30 m away: no
	W.Noise().EmitNoise(DoorBreakNoise(FVector(0.f, -400.f, 92.f)));
	W.Tick(*Near, 0.1f);
	W.Tick(*Walled, 0.1f);
	W.Tick(*Far, 0.1f);
	TestEqual(TEXT("the near NPC investigates"), Near->GetReactionState(), ESpnReactionState::Investigate);
	TestEqual(TEXT("the NPC behind walls does not hear it"), Walled->GetReactionState(), ESpnReactionState::Idle);
	TestEqual(TEXT("the far NPC does not hear it"), Far->GetReactionState(), ESpnReactionState::Idle);
	TestEqual(TEXT("only the listener remembers"), W.Memory().GetMemoryCount(3001), 1);
	TestEqual(TEXT("the others remember nothing"), W.Memory().GetMemoryCount(3002) + W.Memory().GetMemoryCount(3003), 0);
	TestEqual(TEXT("three NPCs are registered"), W.Memory().GetNpcCount(), 3);

	// ids are stable and distinct, and a duplicate id does not create a second record
	ASpnNpc* Twin = W.Npc(3001, TEXT("Twin"), FVector(200.f, 0.f, 92.f));
	TestNotNull(TEXT("the twin exists"), Twin);
	TestEqual(TEXT("still three NPC records"), W.Memory().GetNpcCount(), 3);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNb05SaveDuringAlarmTest, "SpnGame.NightBuild05.SaveDuringAlarmAndLoad", Flags)
bool FSpnNb05SaveDuringAlarmTest::RunTest(const FString&)
{
	FWorld W;
	ACharacter* Player = W.Pawn(FVector(3000.f, 3000.f, 92.f));
	ASpnNpc* Npc = W.Npc(4001, TEXT("Witness"), FVector(0.f, 0.f, 92.f));
	Npc->SetPerceptionTarget(Player);

	// before anything happened a harmless door noise is ignored
	W.Noise().EmitNoise(DoorUseNoise(FVector(300.f, 0.f, 92.f)));
	W.Tick(*Npc, 0.3f);
	TestEqual(TEXT("before the incident the NPC ignores a harmless noise"), Npc->GetReactionState(), ESpnReactionState::Idle);

	// the incident: break-in, sighting, alarm
	W.Noise().EmitNoise(DoorBreakNoise(FVector(900.f, 0.f, 92.f)));
	W.Tick(*Npc, 0.1f);
	Player->SetActorLocation(FVector(500.f, 0.f, 92.f));
	W.Tick(*Npc, 0.3f);
	TestEqual(TEXT("alert"), Npc->GetReactionState(), ESpnReactionState::Alert);

	// save in the middle of the alarm
	TestTrue(TEXT("saved during the alarm"), W.Save().SaveSlot(TEXT("slot_alarm")).IsOk());
	const FString Saved = W.Memory().SerializeState();
	TestFalse(TEXT("the saved text holds no reaction state (it is volatile)"), Saved.Contains(TEXT("Alert")) || Saved.Contains(TEXT("Investigate")));

	// the NPC calms down completely, then the game is loaded
	Player->SetActorLocation(FVector(3000.f, 3000.f, 92.f));
	W.Tick(*Npc, 200.f);
	TestEqual(TEXT("calm again"), Npc->GetReactionState(), ESpnReactionState::Idle);
	W.Memory().AdvanceWorldTime(50.0); // time passes, the alarm gets older than at save time but is within the resume window of the SAVED clock

	TestTrue(TEXT("loaded"), W.Save().LoadSlot(TEXT("slot_alarm")).IsOk());
	TestEqual(TEXT("the memory is exactly the saved one"), W.Memory().SerializeState(), Saved);
	TestEqual(TEXT("the volatile Alert state was not saved; the NPC resumes by investigating the last incident"), Npc->GetReactionState(), ESpnReactionState::Investigate);
	TestTrue(TEXT("it is wary"), Npc->IsWary());
	const FSpnReactionLogEntry& Last = Npc->GetReactionLog().Last();
	TestEqual(TEXT("and the log says why"), Last.Why, FString(TEXT("restored from memory")));

	// the remembered incident changes later behaviour: the harmless noise that was ignored before is now investigated
	Npc->RestoreFromMemory();
	W.Tick(*Npc, 200.f); // let the resumed investigation end
	TestEqual(TEXT("calm"), Npc->GetReactionState(), ESpnReactionState::Idle);
	W.Noise().EmitNoise(DoorUseNoise(FVector(300.f, 0.f, 92.f)));
	W.Tick(*Npc, 0.3f);
	TestEqual(TEXT("the same harmless noise now makes the wary NPC investigate"), Npc->GetReactionState(), ESpnReactionState::Investigate);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNb05FrameRateTest, "SpnGame.NightBuild05.ReactionSequenceIsFrameRateIndependent", Flags)
bool FSpnNb05FrameRateTest::RunTest(const FString&)
{
	TArray<FSpnReactionLogEntry> Logs[3];
	const float Rates[3] = {30.f, 60.f, 144.f};
	for (int32 Index = 0; Index < 3; ++Index)
	{
		FWorld W;
		ACharacter* Player = W.Pawn(FVector(3000.f, 3000.f, 92.f));
		ASpnNpc* Npc = W.Npc(5001, TEXT("Fps"), FVector(0.f, 0.f, 92.f));
		Npc->SetPerceptionTarget(Player);
		const float Dt = 1.f / Rates[Index];
		W.Tick(*Npc, 0.5f, Dt);
		W.Noise().EmitNoise(DoorBreakNoise(FVector(900.f, 0.f, 92.f)));
		W.Tick(*Npc, 0.5f, Dt);
		Player->SetActorLocation(FVector(500.f, 0.f, 92.f));
		W.Tick(*Npc, 0.5f, Dt);
		Player->SetActorLocation(FVector(300.f, 0.f, 92.f));
		W.Tick(*Npc, 3.f, Dt);
		Player->SetActorLocation(FVector(3000.f, 3000.f, 92.f));
		W.Tick(*Npc, 120.f, Dt);
		Logs[Index] = Npc->GetReactionLog();
	}
	TestTrue(*FString::Printf(TEXT("30 vs 60 fps: %s | %s"), *SequenceText(Logs[0]), *SequenceText(Logs[1])), SameSequence(Logs[0], Logs[1]));
	TestTrue(*FString::Printf(TEXT("60 vs 144 fps: %s | %s"), *SequenceText(Logs[1]), *SequenceText(Logs[2])), SameSequence(Logs[1], Logs[2]));
	TestTrue(TEXT("and it is the whole reaction"), Logs[1].Num() >= 4);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
