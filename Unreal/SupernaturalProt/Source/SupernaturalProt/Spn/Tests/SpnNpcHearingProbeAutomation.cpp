#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Misc/AutomationTest.h"
#include "Spn/SpnNpc.h"
#include "Spn/SpnTestDoor.h"

#if WITH_DEV_AUTOMATION_TESTS

// Diagnostic probe (2026-11-13): run in a -game process on /Game/SpnTest/Lvl_SpnNpcTest. Reproduces the wall count of the Caretaker's alarm shout
// as heard by the Neighbour for a grid of Caretaker positions around the two positions recorded in the intermittent QA runs ((358,627): 2 walls,
// not heard; (364,637): 1 wall, heard). Logs every distinct obstacle set; asserts nothing about the outcome (that is SpnQA.NpcReaction's job).

namespace
{
	constexpr EAutomationTestFlags Flags = EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter;

	UWorld* GameWorld()
	{
		for (const FWorldContext& Context : GEngine->GetWorldContexts())
		{
			if (Context.WorldType == EWorldType::Game || Context.WorldType == EWorldType::PIE) { return Context.World(); }
		}
		return nullptr;
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNpcShoutProbe, "SpnDiag.Npc.AlarmShoutWallProbe", Flags)
bool FSpnNpcShoutProbe::RunTest(const FString&)
{
	UWorld* World = GameWorld();
	ASpnNpc* Caretaker = nullptr;
	ASpnNpc* Neighbour = nullptr;
	for (TActorIterator<ASpnNpc> It(World); World && It; ++It)
	{
		if (It->NpcId == 1001) { Caretaker = *It; }
		if (It->NpcId == 1002) { Neighbour = *It; }
	}
	if (Caretaker == nullptr || Neighbour == nullptr)
	{
		AddWarning(TEXT("needs /Game/SpnTest/Lvl_SpnNpcTest loaded (-game <map>); not run"));
		return true;
	}
	const FVector Ear = Neighbour->GetEarLocation();
	const float MouthZ = Caretaker->GetEarLocation().Z;   // the shout leaves at actor location + eye height
	AddInfo(FString::Printf(TEXT("neighbour ear %.0f,%.0f,%.0f; shout height %.0f"), Ear.X, Ear.Y, Ear.Z, MouthZ));

	TMap<FString, int32> Sets;
	FBox MultiBox(ForceInit);
	int32 Two = 0, One = 0, Other = 0;
	for (float X = 330.f; X <= 400.f; X += 2.f)
	{
		for (float Y = 600.f; Y <= 670.f; Y += 2.f)
		{
			TArray<FString> Obstacles;
			const int32 Walls = Neighbour->CountWallsBetween(FVector(X, Y, MouthZ), Ear, FString(), &Obstacles);
			(Walls == 2 ? Two : Walls == 1 ? One : Other)++;
			if (Walls != 1) { MultiBox += FVector(X, Y, 0.f); }
			TArray<FString> Names;
			for (const FString& O : Obstacles) { FString N; O.Split(TEXT(" at "), &N, nullptr); Names.Add(N); }
			Sets.FindOrAdd(FString::Printf(TEXT("%d: %s"), Walls, *FString::Join(Names, TEXT(" + "))))++;
		}
	}
	for (const TPair<FString, int32>& S : Sets)
	{
		AddInfo(FString::Printf(TEXT("%4d positions -> %s"), S.Value, *S.Key));
		TestFalse(*FString::Printf(TEXT("the open storage door is never counted as a wall (%s)"), *S.Key), S.Key.Contains(TEXT("storage_door")));
	}
	AddInfo(FString::Printf(TEXT("grid 330..400 x 600..670 (2 cm): 1 wall %d, 2 walls %d, other %d"), One, Two, Other));
	if (MultiBox.IsValid) { AddInfo(FString::Printf(TEXT("positions with more than one wall: x %.0f..%.0f, y %.0f..%.0f"), MultiBox.Min.X, MultiBox.Max.X, MultiBox.Min.Y, MultiBox.Max.Y)); }
	// the break-in noise of the back door (it leaves at the door panel; the door itself is the source, not an obstacle)
	for (TActorIterator<ASpnTestDoor> It(World); It; ++It)
	{
		if (It->StableId != TEXT("back_door")) { continue; }
		// the noise leaves at the panel centre: the closed leaf's middle, 100 cm above the floor (as SpnQA.NpcReaction aims at it)
		const FVector Hinge = It->GetHingeLocation();
		const FVector Source = Hinge + It->GetActorRotation().RotateVector(FVector(It->LeafWidth * 0.5f, 0.f, 0.f)) + FVector(0.f, 0.f, 100.f);
		for (ASpnNpc* Npc : {Caretaker, Neighbour})
		{
			TArray<FString> Obstacles;
			const int32 Walls = Npc->CountWallsBetween(Source, Npc->GetEarLocation(), TEXT("placeholder.door.back_door"), &Obstacles);
			AddInfo(FString::Printf(TEXT("back-door break-in near %.0f,%.0f -> %s (%.1f m): %d wall(s): %s"), Source.X, Source.Y, *Npc->DisplayName,
				FVector::Dist(Source, Npc->GetEarLocation()) / 100.f, Walls, *FString::Join(Obstacles, TEXT("; "))));
		}
	}
	for (TActorIterator<ASpnTestDoor> It(World); It; ++It)
	{
		AddInfo(FString::Printf(TEXT("door %s: open %d"), *It->StableId, It->IsOpen()));
	}
	for (const FVector2D P : {FVector2D(358.f, 627.f), FVector2D(358.f, 628.f), FVector2D(364.f, 637.f)})
	{
		TArray<FString> Obstacles;
		const int32 Walls = Neighbour->CountWallsBetween(FVector(P.X, P.Y, MouthZ), Ear, FString(), &Obstacles);
		AddInfo(FString::Printf(TEXT("recorded %.0f,%.0f: %d wall(s): %s"), P.X, P.Y, Walls, *FString::Join(Obstacles, TEXT("; "))));
	}
	return true;
}

// Regression (headless): the hearing model counts a closed door between noise and listener as a wall, an open door leaf never.
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNpcOpenDoorNotAWallTest, "SpnGame.Npc.OpenDoorLeafIsNotAWall", Flags)
bool FSpnNpcOpenDoorNotAWallTest::RunTest(const FString&)
{
	UGameInstance* GameInstance = NewObject<UGameInstance>(GEngine);
	GameInstance->AddToRoot();
	GameInstance->InitializeStandalone();
	UWorld* World = GameInstance->GetWorld();

	const auto SpawnDoor = [World](const TCHAR* Id, const FVector& At, bool bOpen)
	{
		FActorSpawnParameters Params;
		Params.bDeferConstruction = true;
		ASpnTestDoor* Door = World->SpawnActor<ASpnTestDoor>(ASpnTestDoor::StaticClass(), FTransform(At), Params);
		Door->StableId = Id;
		Door->bAuthoredOpen = bOpen;
		Door->FinishSpawning(FTransform(At));
		Door->DispatchBeginPlay();     // jumps the leaf to the authored state
		return Door;
	};
	// the leaf runs from the hinge along +X when closed (100 cm wide), along +Y when open at 90 degrees
	ASpnTestDoor* Closed = SpawnDoor(TEXT("closed_door"), FVector(0.f, 0.f, 0.f), false);
	ASpnTestDoor* Open = SpawnDoor(TEXT("open_door"), FVector(1000.f, 0.f, 0.f), true);
	ASpnNpc* Npc = World->SpawnActor<ASpnNpc>(ASpnNpc::StaticClass(), FTransform(FVector(0.f, 3000.f, 100.f)));

	TestTrue(TEXT("authored states"), !Closed->IsOpen() && Open->IsOpen());
	TArray<FString> Obstacles;
	TestEqual(TEXT("a line through the closed leaf: one wall"), Npc->CountWallsBetween(FVector(50.f, -200.f, 100.f), FVector(50.f, 200.f, 100.f), FString(), &Obstacles), 1);
	AddInfo(FString::Join(Obstacles, TEXT("; ")));
	Obstacles.Reset();
	TestEqual(TEXT("a line through the open leaf: no wall"), Npc->CountWallsBetween(FVector(800.f, 50.f, 100.f), FVector(1200.f, 50.f, 100.f), FString(), &Obstacles), 0);
	// and the open leaf is really there: the same line hits it physically
	FHitResult Hit;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(SpnOpenLeafCheck), false, Npc);
	TestTrue(TEXT("the open leaf blocks the trace physically (so the door rule, not a missing collision, makes the difference)"),
		World->LineTraceSingleByChannel(Hit, FVector(800.f, 50.f, 100.f), FVector(1200.f, 50.f, 100.f), ECC_Visibility, Params) && Hit.GetActor() == Open);
	TestEqual(TEXT("the source door itself is never counted"), Npc->CountWallsBetween(FVector(50.f, -200.f, 100.f), FVector(50.f, 200.f, 100.f), TEXT("placeholder.door.closed_door")), 0);

	for (AActor* A : TArray<AActor*>{Closed, Open, Npc}) { A->Destroy(); }
	GameInstance->Shutdown();
	GEngine->DestroyWorldContext(World);
	World->DestroyWorld(false);
	GameInstance->RemoveFromRoot();
	return true;
}

#endif
