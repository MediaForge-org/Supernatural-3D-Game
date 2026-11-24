#include "Components/CapsuleComponent.h"
#include "Dom/JsonObject.h"
#include "Engine/BlockingVolume.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerStart.h"
#include "HAL/FileManager.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/FileHelper.h"
#include "Misc/PackageName.h"
#include "Misc/Paths.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Spn/SpnBuildingObjects.h"
#include "Spn/SpnInteractionComponent.h"
#include "Spn/SpnNpc.h"
#include "Spn/SpnPlayAreaGuard.h"
#include "Spn/SpnSessionSubsystem.h"
#include "Spn/SpnTestDoor.h"
#include "Spn/SpnWorldObjects.h"
#include "Spn/Tests/SpnMovementTestSupport.h"
#include "Spn/Tests/SpnQaRecorder.h"
#include "Spn/Tests/SpnTestPlayReset.h"
#include "Spn/UI/SpnUiText.h"
#include "SpnInventorySubsystem.h"
#include "SpnMapSubsystem.h"
#include "SpnNpcMemorySubsystem.h"
#include "SpnSaveSubsystem.h"
#include "SpnWorldFrame.h"
#include "Spn/World/SpnRouteGuideSubsystem.h"
#include "Spn/UI/SpnUiSubsystem.h"
#include "Engine/LocalPlayer.h"
#include "SupernaturalProtCharacter.h"

#if WITH_DEV_AUTOMATION_TESTS

// SpnWorldQA.Route (gameplay integration phase 1, 2026-11-13): the playable route of the real world, played with key events like a player.
//   1  a new game through the session (as the main menu's New Game does), the generated objects exist once each
//   2  NPCs: spawned on the ground, on the navigation mesh, walking their routines; the player meets the resident (it sees him), cannot walk through it
//   3  house A (oak_n7): open the door with E, enter, take the key and the notebook (a real inventory item), leave
//   4  house B (oak_n6): locked; E unlocks it with the key, E opens it, enter, take the flashlight, leave
//   5  north along Oak Street onto the pine track towards the cabin; the game is SAVED through the session (the pause menu's Save)
//   6  world boundary: the wall in the edge forest, the rockfall on the highway, the play-area guard
//   7  the level is left for the MAIN MENU and the save is LOADED from there (the menu's Load): doors, key, items, NPC memory, the pose
//   8  minimap in the real world (generated SpnWorld map data: bounds, player position, road route to the cabin)
// Points come from Saved/SpnWorldGen/qa_points.json ("gameplay"), written by the world generator (Tools/spn_world/gameplay.py, qa_points.py).

using namespace SpnMovementTest;

namespace
{
	constexpr EAutomationTestFlags Flags = EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter;
	const TCHAR* const Suite = TEXT("SpnWorldRoute");
	const TCHAR* const WorldMap = TEXT("/Game/SpnWorld/Lvl_SpnWorld");
	const TCHAR* const RouteSaveSlot = TEXT("spn_qa_world_route");
	constexpr int64 ResidentId = 2001;
	constexpr int64 WalkerId = 2002;

	struct FHouse
	{
		FString DoorId;
		bool bLocked = false;
		FVector Gate, InFront, Porch, Doorway, Inside, TableFront, Table;
	};

	struct FRoute
	{
		bool bLoaded = false;
		FString Error;
		FVector Start;
		float StartYaw = 0.f;
		FHouse A, B;
		FString KeyId;
		TMap<FString, FVector> Pickups;
		TArray<FVector> ToTrack;
		int32 NavVolumes = 0;
		float Edge = 0.f;
		bool bBoundaryWalk = false;
		FVector BoundaryFrom, BoundaryTo;
		FVector BarrierFrom, BarrierTo;
		FVector Outside;
	};

	FVector Vec(const TSharedPtr<FJsonObject>& O, const TCHAR* Field)
	{
		const TArray<TSharedPtr<FJsonValue>>* A = nullptr;
		if (O.IsValid() && O->TryGetArrayField(Field, A) && A->Num() == 3) { return FVector((*A)[0]->AsNumber(), (*A)[1]->AsNumber(), (*A)[2]->AsNumber()); }
		return FVector::ZeroVector;
	}

	FRoute LoadRoute()
	{
		FRoute R;
		FString Text;
		const FString Path = FPaths::ProjectSavedDir() / TEXT("SpnWorldGen") / TEXT("qa_points.json");
		TSharedPtr<FJsonObject> Root;
		if (!FFileHelper::LoadFileToString(Text, *Path) || !FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Text), Root) || !Root.IsValid())
		{
			R.Error = Path + TEXT(" is missing or invalid: build the world with Tools/spn_world_build.ps1 -Phase H");
			return R;
		}
		const TSharedPtr<FJsonObject>* G = nullptr;
		if (!Root->TryGetObjectField(TEXT("gameplay"), G))
		{
			R.Error = TEXT("qa_points.json has no 'gameplay' section (world built before the gameplay integration)");
			return R;
		}
		const TSharedPtr<FJsonObject>& Gp = *G;
		R.Start = Vec(Gp, TEXT("start"));
		R.StartYaw = Gp->GetNumberField(TEXT("start_yaw"));
		for (const TPair<FHouse*, const TCHAR*> H : {TPair<FHouse*, const TCHAR*>(&R.A, TEXT("house_a")), TPair<FHouse*, const TCHAR*>(&R.B, TEXT("house_b"))})
		{
			const TSharedPtr<FJsonObject> O = Gp->GetObjectField(H.Value);
			H.Key->DoorId = O->GetStringField(TEXT("door_id"));
			H.Key->bLocked = O->GetBoolField(TEXT("locked"));
			H.Key->Gate = Vec(O, TEXT("gate")); H.Key->InFront = Vec(O, TEXT("in_front")); H.Key->Porch = Vec(O, TEXT("porch")); H.Key->Doorway = Vec(O, TEXT("doorway"));
			H.Key->Inside = Vec(O, TEXT("inside")); H.Key->TableFront = Vec(O, TEXT("table_front")); H.Key->Table = Vec(O, TEXT("table"));
		}
		R.KeyId = Gp->GetStringField(TEXT("key_id"));
		for (const TPair<FString, TSharedPtr<FJsonValue>>& P : Gp->GetObjectField(TEXT("pickups"))->Values)
		{
			const TArray<TSharedPtr<FJsonValue>>& A = P.Value->AsArray();
			R.Pickups.Add(P.Key, FVector(A[0]->AsNumber(), A[1]->AsNumber(), A[2]->AsNumber()));
		}
		for (const TSharedPtr<FJsonValue>& V : Gp->GetArrayField(TEXT("to_track")))
		{
			const TArray<TSharedPtr<FJsonValue>>& A = V->AsArray();
			R.ToTrack.Add(FVector(A[0]->AsNumber(), A[1]->AsNumber(), A[2]->AsNumber()));
		}
		R.NavVolumes = static_cast<int32>(Gp->GetNumberField(TEXT("nav_volumes")));
		R.Edge = Gp->GetObjectField(TEXT("boundary"))->GetNumberField(TEXT("edge"));
		const TSharedPtr<FJsonObject>* BW = nullptr;
		if (Gp->TryGetObjectField(TEXT("boundary_walk"), BW)) { R.bBoundaryWalk = true; R.BoundaryFrom = Vec(*BW, TEXT("from")); R.BoundaryTo = Vec(*BW, TEXT("to")); }
		const TSharedPtr<FJsonObject> Bar = Gp->GetObjectField(TEXT("barrier_walk"));
		R.BarrierFrom = Vec(Bar, TEXT("from")); R.BarrierTo = Vec(Bar, TEXT("to"));
		R.Outside = Vec(Gp, TEXT("outside"));
		R.bLoaded = true;
		return R;
	}

	/** What the save must bring back (measured right before saving). */
	struct FShared
	{
		bool bSaved = false;
		FString SaveDetail;
		FVector SavedLoc = FVector::ZeroVector;
		bool bDoorAOpen = false, bDoorBOpen = false, bDoorBLocked = true;
		int32 Notebooks = 0, Flashlights = 0, ResidentMemories = 0;
		FVector LoadedLoc = FVector::ZeroVector;
		FString LoadDetail;
	};

	using FCheckFn = TFunction<TPair<bool, FString>(struct FRun&)>;
	using FPredFn = TFunction<bool(struct FRun&)>;
	using FDoFn = TFunction<void(struct FRun&)>;
	using FTargetFn = TFunction<FVector(struct FRun&)>;

	enum class EOp : uint8 { Section, Wait, WaitUntil, Walk, WalkToTarget, Push, Teleport, Face, Press, Check, Do };

	struct FOp
	{
		EOp Kind = EOp::Wait;
		FVector P = FVector::ZeroVector;
		float A = 0.f;      // Wait: s, Walk: tolerance, Push: s, WaitUntil: timeout
		float B = 0.f;      // Walk: time limit
		FString Id;
		FCheckFn Check;
		FPredFn Pred;
		FDoFn Do;
		FTargetFn Target;
	};

	struct FRun
	{
		TArray<FOp> Ops;
		FRoute Route;
		TSharedPtr<FShared> Shared;
		SpnQa::FHeldKeys Keys;
		ASupernaturalProtCharacter* Character = nullptr;
		int32 Index = -1;
		int32 Frames = 0;
		double StepStart = 0.0;
		FString Failure;
		float PushSpeed = 0.f;
		TMap<int64, float> NpcWalked;
		TMap<int64, FVector> NpcLast;
		bool bResidentSawPlayer = false;

		UWorld* World() const { return Character->GetWorld(); }
		UGameInstance* Instance() const { return Character->GetGameInstance(); }
		ASpnNpc* Npc(int64 Id) const { for (TActorIterator<ASpnNpc> It(World()); It; ++It) { if (It->NpcId == Id) { return *It; } } return nullptr; }
		ASpnTestDoor* Door(const FString& Id) const { for (TActorIterator<ASpnTestDoor> It(World()); It; ++It) { if (It->StableId == Id) { return *It; } } return nullptr; }
		ASpnTestKey* Key() const { for (TActorIterator<ASpnTestKey> It(World()); It; ++It) { if (It->KeyId == Route.KeyId) { return *It; } } return nullptr; }
		ASpnWorldPickup* Pickup(const FString& Id) const { for (TActorIterator<ASpnWorldPickup> It(World()); It; ++It) { if (It->StableId == Id) { return *It; } } return nullptr; }
		ASpnPlayAreaGuard* Guard() const { for (TActorIterator<ASpnPlayAreaGuard> It(World()); It; ++It) { return *It; } return nullptr; }
		int32 Count(const FString& Definition) const
		{
			const USpnInventorySubsystem* Inventory = Instance()->GetSubsystem<USpnInventorySubsystem>();
			FSpnContainerView View;
			int32 N = 0;
			if (Inventory != nullptr && Inventory->GetContainer(SpnWorld::PlayerContainerId, View))
			{
				for (const FSpnStackView& S : View.Stacks) { if (S.Definition == Definition) { N += static_cast<int32>(S.Quantity); } }
			}
			return N;
		}
		FString Prompt() const { const USpnInteractionComponent* I = Character->FindComponentByClass<USpnInteractionComponent>(); return I ? I->GetCurrentPrompt() : FString(); }
	};

	TPair<bool, FString> Result(bool b, const FString& Detail) { return TPair<bool, FString>(b, Detail); }

	/** Route guide prototype (night build 2026-11-14): the derived step, the objective line and the guide waypoint label. */
	TPair<bool, FString> GuideAt(FRun& R, int32 Expected, const TCHAR* TargetKey)
	{
		const USpnRouteGuideSubsystem* Guide = R.World()->GetSubsystem<USpnRouteGuideSubsystem>();
		const USpnMapSubsystem* Map = R.Instance()->GetSubsystem<USpnMapSubsystem>();
		FSpnMapMarkerView Waypoint;
		const bool bWaypoint = Map != nullptr && Map->GetWaypoint(Waypoint);
		const FString Wanted = TargetKey != nullptr ? FSpnUiText::Get(TargetKey).ToString() : FString();
		const bool bWaypointOk = TargetKey == nullptr ? !bWaypoint || !Waypoint.Label.Equals(FSpnUiText::Get(TEXT("ui.guide.target4")).ToString()) : (bWaypoint && Waypoint.Label == Wanted);
		const int32 Step = Guide != nullptr ? Guide->GetStep() : -2;
		return Result(Step == Expected && Guide != nullptr && !Guide->GetObjectiveText().IsEmpty() && bWaypointOk,
			FString::Printf(TEXT("step %d (want %d), objective '%s', waypoint %s '%s' (want '%s')"), Step, Expected, Guide ? *Guide->GetObjectiveText().ToString() : TEXT("-"),
				bWaypoint ? TEXT("set") : TEXT("none"), *Waypoint.Label, *Wanted));
	}

	FString DoorText(const ASpnTestDoor* D)
	{
		return D ? FString::Printf(TEXT("%s: open %d, locked %d, leaf %.0f/%.0f, has key %d"), *D->StableId, D->IsOpen(), D->IsLocked(), D->GetCurrentAngle(), D->GetTargetAngle(), D->HasRequiredKey())
			: FString(TEXT("door missing"));
	}

	FString NpcText(const ASpnNpc* N)
	{
		return N ? FString::Printf(TEXT("%s: %s, sees player %d, nav mesh %d, fallback steering %d, heard %d, at (%.0f, %.0f, %.0f)"), *N->DisplayName, LexToString(N->GetReactionState()),
			N->SeesPlayer(), N->HasNavigationMesh(), N->IsUsingFallbackSteering(), N->GetNoisesHeard(), N->GetActorLocation().X, N->GetActorLocation().Y, N->GetActorLocation().Z)
			: FString(TEXT("NPC missing"));
	}

	float GroundBelow(const UWorld* World, const FVector& At, const AActor* Ignore)
	{
		FHitResult Hit;
		FCollisionQueryParams Params(SCENE_QUERY_STAT(SpnRouteGround), false, Ignore);
		return World->LineTraceSingleByChannel(Hit, At + FVector(0.f, 0.f, 50.f), At - FVector(0.f, 0.f, 3000.f), ECC_Visibility, Params) ? static_cast<float>(Hit.ImpactPoint.Z) : -1e9f;
	}

	struct FBuilder
	{
		TArray<FOp>& Ops;
		FOp& Add(EOp Kind) { FOp& Op = Ops.AddDefaulted_GetRef(); Op.Kind = Kind; return Op; }
		void Section(const TCHAR* T) { Add(EOp::Section).Id = T; }
		void Wait(float S) { Add(EOp::Wait).A = S; }
		void WaitUntil(const TCHAR* Id, float Timeout, FPredFn P) { FOp& O = Add(EOp::WaitUntil); O.Id = Id; O.A = Timeout; O.Pred = MoveTemp(P); }
		void Walk(const FVector& P, float Tol = 35.f, float Limit = 30.f, const TCHAR* Id = TEXT("")) { FOp& O = Add(EOp::Walk); O.P = P; O.A = Tol; O.B = Limit; O.Id = Id; }
		void WalkToTarget(FTargetFn T, float Dist, float Limit, const TCHAR* Id) { FOp& O = Add(EOp::WalkToTarget); O.Target = MoveTemp(T); O.A = Dist; O.B = Limit; O.Id = Id; }
		void Push(FTargetFn T, float Seconds) { FOp& O = Add(EOp::Push); O.Target = MoveTemp(T); O.A = Seconds; }
		void Teleport(const FVector& P, const FVector& LookAt) { FOp& O = Add(EOp::Teleport); O.P = P; O.Target = [LookAt](FRun&) { return LookAt; }; }
		void Face(FTargetFn T) { Add(EOp::Face).Target = MoveTemp(T); }
		void PressE() { Add(EOp::Press); }
		void Check(const TCHAR* Id, FCheckFn F) { FOp& O = Add(EOp::Check); O.Id = Id; O.Check = MoveTemp(F); }
		void Do(FDoFn F) { Add(EOp::Do).Do = MoveTemp(F); }
	};

	void EnterAndLeave(FBuilder& B, const FHouse& H, const TCHAR* Name)
	{
		B.Walk(H.Gate, 45.f, 45.f, *FString::Printf(TEXT("to_%s_gate"), Name));       // through the gap of the garden fence
		B.Walk(H.InFront, 40.f, 15.f, *FString::Printf(TEXT("to_%s"), Name));
		B.Walk(H.Porch, 30.f, 15.f, *FString::Printf(TEXT("%s_porch"), Name));
	}

	TArray<FOp> BuildFirstHalf(const FRoute& Route)
	{
		TArray<FOp> Ops;
		FBuilder B{Ops};
		const FHouse A = Route.A, H = Route.B;
		const FString KeyId = Route.KeyId;

		B.Section(TEXT("SpnWorld route: new game at the edge of the town"));
		B.Do([](FRun& R)
		{
			// what the main menu's New Game does once the level is open: catalog, player container, a fresh world state
			if (USpnSessionSubsystem* Session = R.Instance()->GetSubsystem<USpnSessionSubsystem>()) { Session->StartNewGameInPlace(R.World()); }
			if (USpnNpcMemorySubsystem* Memory = R.Instance()->GetSubsystem<USpnNpcMemorySubsystem>()) { Memory->ResetAll(); }
			if (USpnSaveSubsystem* Save = R.Instance()->GetSubsystem<USpnSaveSubsystem>()) { Save->SetSaveDirectory(FPaths::ConvertRelativePathToFull(SpnQa::QaDir() / TEXT("saves"))); }
		});
		B.Wait(1.5f);
		B.Check(TEXT("start_on_the_ground_at_the_player_start"), [](FRun& R)
		{
			const FVector L = R.Character->GetActorLocation();
			float Best = 1e9f;
			for (TActorIterator<APlayerStart> It(R.World()); It; ++It) { Best = FMath::Min(Best, static_cast<float>(FVector::Dist2D(It->GetActorLocation(), R.Route.Start))); }
			const float Feet = L.Z - R.Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
			const float Ground = GroundBelow(R.World(), L, R.Character);
			return Result(Best < 50.f && R.Character->GetCharacterMovement()->IsMovingOnGround() && FMath::Abs(Feet - Ground) < 15.f,
				FString::Printf(TEXT("player start %.0f cm from the route start, on ground %d, feet %.0f, ground %.0f"), Best, R.Character->GetCharacterMovement()->IsMovingOnGround(), Feet, Ground));
		});
		B.Check(TEXT("guide_step1_the_open_house"), [](FRun& R) { return GuideAt(R, 0, TEXT("ui.guide.target1")); });
		B.Check(TEXT("new_game_has_catalog_and_player_container"), [](FRun& R)
		{
			const USpnInventorySubsystem* Inventory = R.Instance()->GetSubsystem<USpnInventorySubsystem>();
			FSpnContainerView View;
			const bool bContainer = Inventory && Inventory->GetContainer(SpnWorld::PlayerContainerId, View);
			const USpnSessionSubsystem* Session = R.Instance()->GetSubsystem<USpnSessionSubsystem>();
			return Result(Inventory && Inventory->HasCatalog() && bContainer && View.Stacks.Num() == 0 && Session && Session->IsGameFlowEnabled(),
				FString::Printf(TEXT("catalog %d, player container %d with %d stacks, game flow %d"), Inventory && Inventory->HasCatalog(), bContainer, View.Stacks.Num(), Session && Session->IsGameFlowEnabled()));
		});
		B.Check(TEXT("generated_objects_exist_once"), [](FRun& R)
		{
			// a repeated world build must not duplicate anything: every gameplay object exactly once, the volumes in the generated number
			TMap<FString, int32> Ids;
			int32 Npcs = 0, Guards = 0, Blocking = 0, Nav = 0;
			for (TActorIterator<ASpnTestDoor> It(R.World()); It; ++It) { Ids.FindOrAdd(TEXT("door ") + It->StableId)++; }
			for (TActorIterator<ASpnTestKey> It(R.World()); It; ++It) { Ids.FindOrAdd(TEXT("key ") + It->KeyId)++; }
			for (TActorIterator<ASpnWorldPickup> It(R.World()); It; ++It) { Ids.FindOrAdd(TEXT("pickup ") + It->StableId)++; }
			for (TActorIterator<ASpnNpc> It(R.World()); It; ++It) { Ids.FindOrAdd(FString::Printf(TEXT("npc %lld"), It->NpcId))++; ++Npcs; }
			for (TActorIterator<ASpnPlayAreaGuard> It(R.World()); It; ++It) { ++Guards; }
			for (TActorIterator<ABlockingVolume> It(R.World()); It; ++It) { ++Blocking; }     // the generator places only the four boundary walls
			for (TActorIterator<ANavMeshBoundsVolume> It(R.World()); It; ++It) { ++Nav; }
			FString Dupes;
			for (const TPair<FString, int32>& P : Ids) { if (P.Value != 1) { Dupes += FString::Printf(TEXT(" %s x%d;"), *P.Key, P.Value); } }
			const bool bAll = Ids.Contains(TEXT("door ") + R.Route.A.DoorId) && Ids.Contains(TEXT("door ") + R.Route.B.DoorId) && Ids.Contains(TEXT("door cabin_door"))
				&& Ids.Contains(TEXT("key ") + R.Route.KeyId) && R.Route.Pickups.Num() == 2 && Npcs == 2;
			return Result(Dupes.IsEmpty() && bAll && Guards == 1 && Blocking == 4 && Nav == R.Route.NavVolumes,
				FString::Printf(TEXT("%d ids, duplicates:%s doors/key/pickups/NPCs complete %d, guards %d, boundary volumes %d (4), navigation volumes %d (%d)"),
					Ids.Num(), Dupes.IsEmpty() ? TEXT(" none") : *Dupes, bAll, Guards, Blocking, Nav, R.Route.NavVolumes));
		});

		B.Section(TEXT("SpnWorld route: NPCs"));
		B.Wait(6.0f);
		for (const int64 Id : {ResidentId, WalkerId})
		{
			B.Check(*FString::Printf(TEXT("npc_%lld_on_the_ground_and_on_the_navigation_mesh"), Id), [Id](FRun& R)
			{
				const ASpnNpc* N = R.Npc(Id);
				if (N == nullptr) { return Result(false, TEXT("NPC missing")); }
				const FVector L = N->GetActorLocation();
				const float Feet = L.Z - N->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
				const float Ground = GroundBelow(R.World(), L, N);
				return Result(FMath::Abs(Feet - Ground) < 25.f && N->HasNavigationMesh() && !N->IsUsingFallbackSteering(),
					FString::Printf(TEXT("feet %.0f, ground %.0f | %s"), Feet, Ground, *NpcText(N)));
			});
			B.Check(*FString::Printf(TEXT("npc_%lld_walks_its_routine"), Id), [Id](FRun& R)
			{
				const float Walked = R.NpcWalked.FindRef(Id);
				return Result(Walked > 150.f, FString::Printf(TEXT("%.0f cm walked since the start | %s"), Walked, *NpcText(R.Npc(Id))));
			});
		}
		B.WalkToTarget([](FRun& R) { const ASpnNpc* N = R.Npc(ResidentId); return N ? N->GetActorLocation() : R.Character->GetActorLocation(); }, 260.f, 45.f, TEXT("meet_the_resident"));
		B.WaitUntil(TEXT("the_resident_sees_the_player"), 6.f, [](FRun& R) { return R.bResidentSawPlayer; });
		B.Push([](FRun& R) { const ASpnNpc* N = R.Npc(ResidentId); return N ? N->GetActorLocation() : R.Character->GetActorLocation(); }, 2.5f);
		B.Check(TEXT("the_player_cannot_walk_through_the_resident"), [](FRun& R)
		{
			const ASpnNpc* N = R.Npc(ResidentId);
			if (N == nullptr) { return Result(false, TEXT("NPC missing")); }
			const float D = FVector::Dist2D(N->GetActorLocation(), R.Character->GetActorLocation());
			const float Min = N->GetCapsuleComponent()->GetScaledCapsuleRadius() + R.Character->GetCapsuleComponent()->GetScaledCapsuleRadius();
			return Result(D >= Min - 10.f, FString::Printf(TEXT("centre distance %.0f cm (capsules %.0f), speed while pushing %.0f cm/s | %s"), D, Min, R.PushSpeed, *NpcText(N)));
		});
		B.Check(TEXT("the_resident_reacts_within_the_existing_rules"), [](FRun& R)
		{
			const ASpnNpc* N = R.Npc(ResidentId);
			FString Log;
			for (const FSpnReactionLogEntry& E : N ? N->GetReactionLog() : TArray<FSpnReactionLogEntry>()) { Log += FString::Printf(TEXT(" %s->%s (%s);"), LexToString(E.From), LexToString(E.To), *E.Why); }
			// a stranger walking in the street is no incident: the resident notices him, raises no alarm and does not flee
			return Result(N && N->GetAlarmsRaised() == 0 && N->GetReactionState() != ESpnReactionState::Flee, FString::Printf(TEXT("alarms %d | %s | log:%s"), N ? N->GetAlarmsRaised() : -1, *NpcText(N), Log.IsEmpty() ? TEXT(" -") : *Log));
		});

		B.Section(TEXT("SpnWorld route: house A (open): key and notebook"));
		EnterAndLeave(B, A, TEXT("house_a"));
		B.Face([A](FRun& R) { const ASpnTestDoor* D = R.Door(A.DoorId); return D ? D->GetComponentsBoundingBox(false).GetCenter() : A.Doorway; });
		B.Wait(0.5f);
		B.Check(TEXT("house_a_door_prompt_open"), [A](FRun& R) { return Result(R.Prompt().Contains(FSpnUiText::Get(TEXT("ui.door.open")).ToString()), FString::Printf(TEXT("prompt '%s' | %s"), *R.Prompt(), *DoorText(R.Door(A.DoorId)))); });
		B.PressE();
		B.WaitUntil(TEXT("house_a_door_opens_with_e"), 4.f, [A](FRun& R) { const ASpnTestDoor* D = R.Door(A.DoorId); return D && D->IsOpen() && FMath::IsNearlyEqual(D->GetCurrentAngle(), D->GetTargetAngle(), 1.f); });
		B.Walk(A.Doorway, 30.f, 12.f, TEXT("house_a_doorway"));
		B.Walk(A.Inside, 30.f, 12.f, TEXT("house_a_inside"));
		B.Walk(A.TableFront, 30.f, 12.f, TEXT("house_a_table"));
		B.Check(TEXT("house_a_player_inside_the_room"), [](FRun& R)
		{
			const FVector L = R.Character->GetActorLocation();
			for (TActorIterator<APostProcessVolume> It(R.World()); It; ++It)
			{
				if (It->GetActorNameOrLabel().Contains(TEXT("oak_n7")) && It->EncompassesPoint(L, 0.f, nullptr)) { return Result(R.Character->GetCharacterMovement()->IsMovingOnGround(), FString::Printf(TEXT("in %s, on ground"), *It->GetActorNameOrLabel())); }
			}
			return Result(false, FString::Printf(TEXT("not inside the oak_n7 room volume at (%.0f, %.0f, %.0f)"), L.X, L.Y, L.Z));
		});
		B.Face([](FRun& R) { const ASpnTestKey* K = R.Key(); return K ? K->GetActorLocation() : FVector::ZeroVector; });
		B.Wait(0.4f);
		B.Check(TEXT("key_prompt"), [](FRun& R) { return Result(!R.Prompt().IsEmpty(), FString::Printf(TEXT("prompt '%s'"), *R.Prompt())); });
		B.PressE();
		B.Wait(0.4f);
		B.Check(TEXT("key_taken"), [](FRun& R) { const ASpnTestKey* K = R.Key(); return Result(K && K->IsHeld() && K->IsHidden(), FString::Printf(TEXT("key %s held %d hidden %d"), K ? *K->KeyId : TEXT("missing"), K && K->IsHeld(), K && K->IsHidden())); });
		B.Face([](FRun& R) { const ASpnWorldPickup* P = R.Pickup(TEXT("spnworld.pickup.notebook")); return P ? P->GetActorLocation() : FVector::ZeroVector; });
		B.Wait(0.4f);
		B.PressE();
		B.Wait(0.4f);
		B.Wait(0.5f);
		B.Check(TEXT("guide_step2_after_the_key"), [](FRun& R) { return GuideAt(R, 1, TEXT("ui.guide.target2")); });
		B.Check(TEXT("notebook_taken_into_the_inventory"), [](FRun& R)
		{
			const ASpnWorldPickup* P = R.Pickup(TEXT("spnworld.pickup.notebook"));
			const int32 N = R.Count(TEXT("spn_test:item.notebook"));
			// "examinable": the item and its description are shown in the inventory screen (the existing way to look at an item)
			const FString Name = SpnWorld::ItemDisplayName(TEXT("spn_test:item.notebook")).ToString();
			const FString Desc = FSpnUiText::Get(TEXT("item.spn_test:item.notebook.desc")).ToString();
			return Result(P && P->IsTaken() && N == 1 && !Name.Contains(TEXT("spn_test:")),
				FString::Printf(TEXT("pickup taken %d, notebooks in the inventory %d, shown as '%s', description '%s'"), P && P->IsTaken(), N, *Name, *Desc));
		});
		B.Walk(A.Inside, 30.f, 12.f, TEXT("house_a_back_to_the_door"));
		B.Walk(A.Doorway, 30.f, 12.f, TEXT("house_a_out_doorway"));
		B.Walk(A.InFront, 40.f, 15.f, TEXT("house_a_outside"));
		B.Walk(A.Gate, 45.f, 15.f, TEXT("house_a_gate_out"));

		B.Section(TEXT("SpnWorld route: house B (locked): the key opens it"));
		EnterAndLeave(B, H, TEXT("house_b"));
		B.Face([H](FRun& R) { const ASpnTestDoor* D = R.Door(H.DoorId); return D ? D->GetComponentsBoundingBox(false).GetCenter() : H.Doorway; });
		B.Wait(0.5f);
		B.Check(TEXT("house_b_locked_and_the_key_is_known"), [H](FRun& R)
		{
			const ASpnTestDoor* D = R.Door(H.DoorId);
			return Result(D && D->IsLocked() && !D->IsOpen() && D->HasRequiredKey() && R.Prompt().Contains(FSpnUiText::Get(TEXT("ui.door.unlock")).ToString()), FString::Printf(TEXT("prompt '%s' | %s"), *R.Prompt(), *DoorText(D)));
		});
		B.PressE();
		B.Wait(0.5f);
		B.Check(TEXT("house_b_unlocked_with_e"), [H](FRun& R) { const ASpnTestDoor* D = R.Door(H.DoorId); return Result(D && !D->IsLocked() && !D->IsOpen(), DoorText(D)); });
		B.Wait(0.5f);
		B.Check(TEXT("guide_step3_after_unlocking"), [](FRun& R) { return GuideAt(R, 2, TEXT("ui.guide.target3")); });
		B.PressE();
		B.WaitUntil(TEXT("house_b_door_opens_with_e"), 4.f, [H](FRun& R) { const ASpnTestDoor* D = R.Door(H.DoorId); return D && D->IsOpen() && FMath::IsNearlyEqual(D->GetCurrentAngle(), D->GetTargetAngle(), 1.f); });
		B.Walk(H.Doorway, 30.f, 12.f, TEXT("house_b_doorway"));
		B.Walk(H.Inside, 30.f, 12.f, TEXT("house_b_inside"));
		B.Walk(H.TableFront, 30.f, 12.f, TEXT("house_b_table"));
		B.Face([](FRun& R) { const ASpnWorldPickup* P = R.Pickup(TEXT("spnworld.pickup.flashlight")); return P ? P->GetActorLocation() : FVector::ZeroVector; });
		B.Wait(0.4f);
		B.PressE();
		B.Wait(0.4f);
		B.Check(TEXT("flashlight_taken_into_the_inventory"), [](FRun& R)
		{
			const ASpnWorldPickup* P = R.Pickup(TEXT("spnworld.pickup.flashlight"));
			return Result(P && P->IsTaken() && R.Count(TEXT("spn_test:item.flashlight")) == 1, FString::Printf(TEXT("pickup taken %d, flashlights %d"), P && P->IsTaken(), R.Count(TEXT("spn_test:item.flashlight"))));
		});
		B.Walk(H.Inside, 30.f, 12.f, TEXT("house_b_back_to_the_door"));
		B.Walk(H.Doorway, 30.f, 12.f, TEXT("house_b_out_doorway"));
		B.Walk(H.InFront, 40.f, 15.f, TEXT("house_b_outside"));
		B.Walk(H.Gate, 45.f, 15.f, TEXT("house_b_gate_out"));
		B.Check(TEXT("guide_step4_after_the_flashlight"), [](FRun& R) { return GuideAt(R, 3, TEXT("ui.guide.target4")); });
		B.Check(TEXT("house_b_left_on_the_ground"), [](FRun& R) { return Result(R.Character->GetCharacterMovement()->IsMovingOnGround(), TEXT("outside in front of the house")); });

		B.Section(TEXT("SpnWorld route: towards the forest and the cabin"));
		for (int32 I = 0; I < Route.ToTrack.Num(); ++I) { B.Walk(Route.ToTrack[I], 80.f, 60.f, *FString::Printf(TEXT("to_track_%d"), I)); }
		B.Check(TEXT("on_the_pine_track_towards_the_cabin"), [](FRun& R)
		{
			const float D = FVector::Dist2D(R.Character->GetActorLocation(), R.Route.ToTrack.Last());
			return Result(D < 150.f && R.Character->GetCharacterMovement()->IsMovingOnGround(), FString::Printf(TEXT("%.0f cm from the track point, on ground %d | walker: %s"), D,
				R.Character->GetCharacterMovement()->IsMovingOnGround(), *NpcText(R.Npc(WalkerId))));
		});
		B.Check(TEXT("npcs_reach_their_routine_points"), [](FRun& R)
		{
			// by now (about two minutes) each NPC must have finished patrol legs of about 40 m; a leg given up counts only as information
			// (the player may have stood in the way)
			const ASpnNpc* N1 = R.Npc(ResidentId);
			const ASpnNpc* N2 = R.Npc(WalkerId);
			return Result(N1 && N2 && N1->GetPatrolLegsCompleted() >= 1 && N2->GetPatrolLegsCompleted() >= 1,
				FString::Printf(TEXT("resident: %d legs completed, %d given up; walker: %d completed, %d given up"), N1 ? N1->GetPatrolLegsCompleted() : -1, N1 ? N1->GetPatrolLegsGivenUp() : -1,
					N2 ? N2->GetPatrolLegsCompleted() : -1, N2 ? N2->GetPatrolLegsGivenUp() : -1));
		});
		B.Do([A, H](FRun& R)
		{
			// the pause menu's Save
			FShared& S = *R.Shared;
			S.SavedLoc = R.Character->GetActorLocation();
			S.bDoorAOpen = R.Door(A.DoorId) && R.Door(A.DoorId)->IsOpen();
			S.bDoorBOpen = R.Door(H.DoorId) && R.Door(H.DoorId)->IsOpen();
			S.bDoorBLocked = !R.Door(H.DoorId) || R.Door(H.DoorId)->IsLocked();
			S.Notebooks = R.Count(TEXT("spn_test:item.notebook"));
			S.Flashlights = R.Count(TEXT("spn_test:item.flashlight"));
			const USpnNpcMemorySubsystem* Memory = R.Instance()->GetSubsystem<USpnNpcMemorySubsystem>();
			S.ResidentMemories = Memory ? Memory->GetMemoryCount(ResidentId) : -1;
			USpnSessionSubsystem* Session = R.Instance()->GetSubsystem<USpnSessionSubsystem>();
			const FSpnSaveStatus Status = Session ? Session->SaveGame(RouteSaveSlot) : FSpnSaveStatus();
			S.bSaved = Status.IsOk();
			S.SaveDetail = FString::Printf(TEXT("%s %s"), LexToString(Status.Result), *Status.Detail);
		});
		B.Check(TEXT("saved_in_the_world"), [](FRun& R)
		{
			const FShared& S = *R.Shared;
			return Result(S.bSaved, FString::Printf(TEXT("%s; at (%.0f, %.0f, %.0f), doors A open %d / B open %d locked %d, notebooks %d, flashlights %d, resident memories %d"), *S.SaveDetail,
				S.SavedLoc.X, S.SavedLoc.Y, S.SavedLoc.Z, S.bDoorAOpen, S.bDoorBOpen, S.bDoorBLocked, S.Notebooks, S.Flashlights, S.ResidentMemories));
		});

		B.Section(TEXT("SpnWorld route: completion at the cabin (route guide prototype)"));
		B.Do([](FRun& R)
		{
			for (TActorIterator<ASpnTestDoor> It(R.World()); It; ++It)
			{
				if (It->StableId == TEXT("cabin_door"))
				{
					// in front of the cabin door, outside, on the ground (the guide finishes within USpnRouteGuideSubsystem::CabinRadiusCm)
					const FVector Door = It->GetActorLocation();
					const FVector Out = Door + (R.Route.ToTrack.Last() - Door).GetSafeNormal2D() * 350.f;   // towards the track: outside
					const float Ground = GroundBelow(R.World(), Out + FVector(0, 0, 300.f), R.Character);
					R.Character->TeleportTo(FVector(Out.X, Out.Y, Ground + R.Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 5.f), R.Character->GetActorRotation());
				}
			}
		});
		B.Wait(1.0f);
		B.Check(TEXT("guide_completes_at_the_cabin"), [](FRun& R)
		{
			const USpnRouteGuideSubsystem* Guide = R.World()->GetSubsystem<USpnRouteGuideSubsystem>();
			TPair<bool, FString> G = GuideAt(R, 4, nullptr);
			return Result(G.Key && Guide->IsCompleted(), G.Value);
		});

		B.Section(TEXT("SpnWorld route: world boundary"));
		if (Route.bBoundaryWalk)
		{
			B.Teleport(Route.BoundaryFrom, Route.BoundaryTo);
			B.Wait(1.0f);
			B.Push([BoundaryTo = Route.BoundaryTo](FRun&) { return BoundaryTo; }, 9.f);
			B.Check(TEXT("the_boundary_wall_in_the_edge_forest_stops_the_player"), [](FRun& R)
			{
				const FVector L = R.Character->GetActorLocation();
				const float Radius = R.Character->GetCapsuleComponent()->GetScaledCapsuleRadius();
				// reached the wall (not stopped earlier by a tree), held there while still pushing, and not past it
				const bool bAtWall = L.Y + Radius >= R.Route.Edge - 40.f;
				return Result(bAtWall && L.Y + Radius <= R.Route.Edge + 5.f && R.PushSpeed < 30.f, FString::Printf(TEXT("stopped at y %.0f (wall face %.0f, capsule %.0f, at the wall %d), speed %.0f cm/s while pushing, %.0f cm walked from the start point"), L.Y, R.Route.Edge, Radius, bAtWall, R.PushSpeed,
					FVector::Dist2D(L, R.Route.BoundaryFrom)));
			});
		}
		B.Teleport(Route.BarrierFrom, Route.BarrierTo);
		B.Wait(1.0f);
		B.Push([BarrierTo = Route.BarrierTo](FRun&) { return BarrierTo; }, 14.f);
		B.Check(TEXT("the_rockfall_closes_the_highway"), [](FRun& R)
		{
			const FVector L = R.Character->GetActorLocation();
			const float Radius = R.Character->GetCapsuleComponent()->GetScaledCapsuleRadius();
			// the rocks hold the player back: at least 3 m before the invisible wall, i.e. the wall is never met on the road
			return Result(FMath::Abs(L.X) + Radius <= R.Route.Edge - 300.f, FString::Printf(TEXT("stopped at (%.0f, %.0f), %.0f cm before the barrier centre, wall at x %.0f, speed %.0f cm/s"), L.X, L.Y,
				FVector::Dist2D(L, R.Route.BarrierTo), -R.Route.Edge, R.PushSpeed));
		});
		B.Do([](FRun& R) { R.Shared->LoadDetail = FString::FromInt(R.Guard() ? R.Guard()->GetReturnCount() : -1); });
		B.Teleport(Route.Outside, Route.Start);
		B.Wait(1.5f);
		B.Check(TEXT("the_play_area_guard_returns_a_player_who_got_out"), [](FRun& R)
		{
			const ASpnPlayAreaGuard* G = R.Guard();
			const FVector L = R.Character->GetActorLocation();
			const int32 Before = FCString::Atoi(*R.Shared->LoadDetail);
			return Result(G && G->GetReturnCount() == Before + 1 && FMath::Abs(L.Y) < R.Route.Edge && FMath::Abs(L.X) < R.Route.Edge,
				FString::Printf(TEXT("returns %d -> %d, now at (%.0f, %.0f, %.0f)"), Before, G ? G->GetReturnCount() : -1, L.X, L.Y, L.Z));
		});
		return Ops;
	}

	TArray<FOp> BuildSecondHalf(const FRoute& Route)
	{
		TArray<FOp> Ops;
		FBuilder B{Ops};
		const FHouse A = Route.A, H = Route.B;
		B.Section(TEXT("SpnWorld route: loaded from the main menu"));
		B.Wait(1.0f);
		B.Check(TEXT("load_from_the_main_menu_returns_to_the_world"), [](FRun& R)
		{
			const FString Level = R.World()->GetOutermost()->GetName();
			return Result(Level.Contains(TEXT("Lvl_SpnWorld")), FString::Printf(TEXT("level %s, load: %s"), *Level, *R.Shared->LoadDetail));
		});
		B.Check(TEXT("load_restores_the_player_position"), [](FRun& R)
		{
			const float D = FVector::Dist(R.Shared->LoadedLoc, R.Shared->SavedLoc);
			return Result(D < 120.f, FString::Printf(TEXT("loaded at (%.0f, %.0f, %.0f), saved at (%.0f, %.0f, %.0f): %.0f cm"), R.Shared->LoadedLoc.X, R.Shared->LoadedLoc.Y, R.Shared->LoadedLoc.Z,
				R.Shared->SavedLoc.X, R.Shared->SavedLoc.Y, R.Shared->SavedLoc.Z, D));
		});
		B.Check(TEXT("load_restores_the_doors"), [A, H](FRun& R)
		{
			const ASpnTestDoor* DA = R.Door(A.DoorId);
			const ASpnTestDoor* DB = R.Door(H.DoorId);
			const bool bOk = DA && DB && DA->IsOpen() == R.Shared->bDoorAOpen && DB->IsOpen() == R.Shared->bDoorBOpen && DB->IsLocked() == R.Shared->bDoorBLocked
				&& FMath::IsNearlyEqual(DB->GetCurrentAngle(), DB->GetTargetAngle(), 1.f);
			return Result(bOk, DoorText(DA) + TEXT(" | ") + DoorText(DB));
		});
		B.Check(TEXT("load_restores_the_key_and_the_items"), [](FRun& R)
		{
			const ASpnTestKey* K = R.Key();
			const ASpnWorldPickup* N = R.Pickup(TEXT("spnworld.pickup.notebook"));
			const ASpnWorldPickup* F = R.Pickup(TEXT("spnworld.pickup.flashlight"));
			const int32 Notebooks = R.Count(TEXT("spn_test:item.notebook")), Flashlights = R.Count(TEXT("spn_test:item.flashlight"));
			const bool bOk = K && K->IsHeld() && K->IsHidden() && N && N->IsTaken() && N->IsHidden() && F && F->IsTaken() && F->IsHidden() && Notebooks == R.Shared->Notebooks && Flashlights == R.Shared->Flashlights;
			return Result(bOk, FString::Printf(TEXT("key held %d hidden %d, notebook pickup taken %d hidden %d, flashlight pickup taken %d hidden %d, inventory notebooks %d (%d), flashlights %d (%d)"),
				K && K->IsHeld(), K && K->IsHidden(), N && N->IsTaken(), N && N->IsHidden(), F && F->IsTaken(), F && F->IsHidden(), Notebooks, R.Shared->Notebooks, Flashlights, R.Shared->Flashlights));
		});
		B.Check(TEXT("load_restores_the_npc_memory"), [](FRun& R)
		{
			const USpnNpcMemorySubsystem* Memory = R.Instance()->GetSubsystem<USpnNpcMemorySubsystem>();
			const int32 Now = Memory ? Memory->GetMemoryCount(ResidentId) : -1;
			return Result(Now == R.Shared->ResidentMemories, FString::Printf(TEXT("resident memories %d (saved %d) | %s"), Now, R.Shared->ResidentMemories, *NpcText(R.Npc(ResidentId))));
		});
		B.Check(TEXT("npcs_back_on_the_navigation_mesh_after_the_load"), [](FRun& R)
		{
			const ASpnNpc* N1 = R.Npc(ResidentId);
			const ASpnNpc* N2 = R.Npc(WalkerId);
			return Result(N1 && N2 && N1->HasNavigationMesh() && N2->HasNavigationMesh(), NpcText(N1) + TEXT(" | ") + NpcText(N2));
		});
		B.Check(TEXT("the_house_b_door_works_after_the_load"), [H](FRun& R)
		{
			// the loaded, unlocked door still takes Use (close and open again)
			ASpnTestDoor* D = R.Door(H.DoorId);
			if (D == nullptr) { return Result(false, TEXT("door missing")); }
			const bool bWasOpen = D->IsOpen();
			const ESpnInteractResult First = D->SpnUse(R.Character).Result;
			const bool bToggled = D->IsOpen() != bWasOpen;
			D->SpnUse(R.Character);
			return Result(First == ESpnInteractResult::Changed && bToggled && D->IsOpen() == bWasOpen, FString::Printf(TEXT("use -> %d, toggled %d | %s"), static_cast<int32>(First), bToggled, *DoorText(D)));
		});

		B.Section(TEXT("SpnWorld route: minimap in the real world"));
		B.Check(TEXT("minimap_shows_spnworld"), [](FRun& R)
		{
			// night build 2026-11-14: SpnWorld installs the map data its generator wrote (Tools/spn_world/map_data.py -> Content/SpnWorld/Data)
			USpnMapSubsystem* Map = R.Instance()->GetSubsystem<USpnMapSubsystem>();
			if (Map == nullptr || !Map->HasDefinition()) { return Result(false, TEXT("no map definition installed")); }
			FVector Metres;
			const bool bConverted = SpnWorldFrame::ToWorld(Map->GetFrame(), R.Character->GetActorLocation(), Metres);
			const FSpnMapBounds Bounds = Map->GetBounds();
			const FVector2D Size = Bounds.Max - Bounds.Min;
			const bool bInside = bConverted && Metres.X >= Bounds.Min.X && Metres.X <= Bounds.Max.X && Metres.Y >= Bounds.Min.Y && Metres.Y <= Bounds.Max.Y;
			TArray<FSpnMapPoiView> Pois;
			TArray<FSpnMapMarkerView> Markers;
			Map->QueryVisible(Bounds.Min, Bounds.Max, Pois, Markers);
			int32 OldVisible = 0;
			const FSpnMapPoiView* Cabin = nullptr;
			for (const FSpnMapPoiView& P : Pois)
			{
				OldVisible += (P.bVisible && P.Id.StartsWith(TEXT("spn_test:"))) ? 1 : 0;
				if (P.Id == TEXT("spnworld:poi.cabin")) { Cabin = &P; }
			}
			double RouteLength = 0.0;
			FString RouteFailure;
			const bool bRoute = Cabin != nullptr && Map->PlanRouteLength(FVector2D(Metres.X, Metres.Y), Cabin->Metres, RouteLength, RouteFailure);
			const double Straight = Cabin != nullptr ? FVector2D::Distance(FVector2D(Metres.X, Metres.Y), Cabin->Metres) : 0.0;
			R.Shared->LoadDetail = FString::Printf(TEXT("generated %d, roads %d, bounds %.0f x %.0f m, player at %.0f, %.0f m inside %d, old test POIs visible %d, cabin %s, route to cabin %d: %.0f m (straight %.0f m) %s"),
				Map->IsGeneratedRegion(), Map->HasRoadNetwork(), Size.X, Size.Y, Metres.X, Metres.Y, bInside, OldVisible, Cabin != nullptr ? (Cabin->bVisible ? TEXT("visible") : TEXT("hidden")) : TEXT("missing"),
				bRoute, RouteLength, Straight, *RouteFailure);
			const bool bOk = Map->IsGeneratedRegion() && Map->HasRoadNetwork() && FMath::IsNearlyEqual(Size.X, 1260.0, 5.0) && FMath::IsNearlyEqual(Size.Y, 1260.0, 5.0)
				&& bInside && OldVisible == 0 && Cabin != nullptr && Cabin->bVisible && bRoute && RouteLength >= Straight;
			return Result(bOk, R.Shared->LoadDetail);
		});
		B.Check(TEXT("map_screen_opens_in_spnworld"), [](FRun& R)
		{
			const ULocalPlayer* Player = R.World()->GetFirstLocalPlayerFromController();
			USpnUiSubsystem* Ui = Player != nullptr ? Player->GetSubsystem<USpnUiSubsystem>() : nullptr;
			// opened and closed within this step: the map screen pauses the game, so a timed wait would never end
			if (Ui != nullptr) { Ui->OpenMap(); }
			const bool bOpen = Ui != nullptr && Ui->GetTopPage() == ESpnPage::Map && Ui->GetMapScreen() != nullptr;
			const FString Detail = FString::Printf(TEXT("ui %d, top page %d, map screen %d"), Ui != nullptr, Ui ? static_cast<int32>(Ui->GetTopPage()) : -1, Ui && Ui->GetMapScreen());
			if (Ui != nullptr) { Ui->CloseAll(); }
			return Result(bOpen, Detail);
		});
		return Ops;
	}

	bool StepRun(FAutomationTestBase& Test, FRun& R, ASupernaturalProtCharacter& Character, float DeltaSeconds, double Elapsed)
	{
		APlayerController* PC = Cast<APlayerController>(Character.GetController());
		const FVector Loc = Character.GetActorLocation();
		if (R.Index < 0) { R.Index = 0; R.StepStart = Elapsed; R.Frames = 0; }
		if (!R.Failure.IsEmpty() || !R.Ops.IsValidIndex(R.Index)) { return true; }
		for (const int64 Id : {ResidentId, WalkerId})
		{
			if (const ASpnNpc* N = R.Npc(Id))
			{
				if (const FVector* Last = R.NpcLast.Find(Id)) { R.NpcWalked.FindOrAdd(Id) += FVector::Dist2D(*Last, N->GetActorLocation()); }
				R.NpcLast.Add(Id, N->GetActorLocation());
				if (Id == ResidentId && N->SeesPlayer()) { R.bResidentSawPlayer = true; }
			}
		}
		FOp& Op = R.Ops[R.Index];
		const double InStep = Elapsed - R.StepStart;
		const auto Next = [&]() { ++R.Index; R.StepStart = Elapsed; R.Frames = 0; };
		const auto Check = [&](const FString& Id, SpnQa::EStatus Status, const FString& Detail) { SpnQa::Check(Test, &Character, Suite, Id, Status, Detail); };
		const auto Aim = [&](const FVector& Target, float FixedPitch, bool bUsePitch)
		{
			const FVector Eye = Loc + FVector(0.f, 0.f, 64.f);
			const FVector To = Target - Eye;
			PC->SetControlRotation(FRotator(bUsePitch ? FMath::RadiansToDegrees(FMath::Atan2(To.Z, To.Size2D())) : FixedPitch, FMath::RadiansToDegrees(FMath::Atan2(To.Y, To.X)), 0.f));
		};
		const bool bMoving = Op.Kind == EOp::Walk || Op.Kind == EOp::WalkToTarget || Op.Kind == EOp::Push;
		if (!bMoving) { R.Keys.Set(Character, EKeys::W, false); }

		switch (Op.Kind)
		{
		case EOp::Section:
			SpnQa::Section(Op.Id);
			Next();
			break;
		case EOp::Wait:
			if (InStep >= Op.A) { Next(); }
			break;
		case EOp::WaitUntil:
			if (Op.Pred(R)) { Check(Op.Id, SpnQa::EStatus::Pass, FString::Printf(TEXT("after %.1f s"), InStep)); Next(); }
			else if (InStep >= Op.A) { Check(Op.Id, SpnQa::EStatus::Fail, FString::Printf(TEXT("not within %.0f s | %s | %s"), Op.A, *NpcText(R.Npc(ResidentId)), *DoorText(R.Door(R.Route.A.DoorId)))); Next(); }
			break;
		case EOp::Walk:
		case EOp::WalkToTarget:
		{
			const FVector Goal = Op.Kind == EOp::Walk ? Op.P : Op.Target(R);
			const float D = FVector::Dist2D(Loc, Goal);
			if (D <= Op.A) { R.Keys.Set(Character, EKeys::W, false); Next(); break; }
			if (InStep > Op.B)
			{
				R.Keys.Set(Character, EKeys::W, false);
				Check(TEXT("walk_") + Op.Id, SpnQa::EStatus::Blocked, FString::Printf(TEXT("did not reach the point within %.0f s: %.0f cm left, stuck at (%.0f, %.0f, %.0f)"), Op.B, D, Loc.X, Loc.Y, Loc.Z));
				R.Failure = TEXT("route blocked at ") + Op.Id;
				return true;
			}
			Aim(Goal, -6.f, false);
			R.Keys.Set(Character, EKeys::W, true);
			break;
		}
		case EOp::Push:
			Aim(Op.Target(R), -6.f, false);
			R.Keys.Set(Character, EKeys::W, true);
			R.PushSpeed = Character.GetVelocity().Size2D();
			if (InStep >= Op.A) { Next(); }
			break;
		case EOp::Teleport:
		{
			const float Ground = GroundBelow(R.World(), Op.P + FVector(0.f, 0.f, 2500.f), &Character);
			Character.GetCharacterMovement()->StopMovementImmediately();
			Character.SetActorLocation(FVector(Op.P.X, Op.P.Y, (Ground > -1e8f ? Ground : Op.P.Z) + Character.GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 10.f), false, nullptr, ETeleportType::TeleportPhysics);
			const FVector T = Op.Target(R);
			PC->SetControlRotation(FRotator(-6.f, FMath::RadiansToDegrees(FMath::Atan2(T.Y - Op.P.Y, T.X - Op.P.X)), 0.f));
			Next();
			break;
		}
		case EOp::Face:
			Aim(Op.Target(R), 0.f, true);
			if (Character.GetVelocity().Size2D() < 15.f || InStep > 2.0) { Next(); }
			break;
		case EOp::Press:
			SpnQa::Key(*PC, EKeys::E, R.Frames == 0);
			if (++R.Frames > 1) { Next(); }
			break;
		case EOp::Check:
		{
			const TPair<bool, FString> V = Op.Check(R);
			Check(Op.Id, V.Key ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail, V.Value);
			Next();
			break;
		}
		case EOp::Do:
			Op.Do(R);
			Next();
			break;
		}
		return !R.Ops.IsValidIndex(R.Index);
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnQaSpnWorldRouteTest, "SpnWorldQA.Route", Flags)

bool FSpnQaSpnWorldRouteTest::RunTest(const FString& Parameters)
{
	if (!FPackageName::DoesPackageExist(WorldMap))
	{
		AddError(TEXT("/Game/SpnWorld/Lvl_SpnWorld does not exist: build it with Tools/spn_world_build.ps1 -Phase H"));
		return false;
	}
	const FRoute Route = LoadRoute();
	if (!Route.bLoaded)
	{
		AddError(Route.Error);
		return false;
	}
	const TSharedRef<FShared> Shared = MakeShared<FShared>();

	const auto RunHalf = [this, Shared, Route](TArray<FOp> Ops, const TCHAR* Name, TFunction<FVector()> Start, float Yaw)
	{
		const TSharedRef<FRun> Run = MakeShared<FRun>();
		Run->Ops = MoveTemp(Ops);
		Run->Route = Route;
		Run->Shared = Shared;
		const FString Label = Name;
		// queued directly (a command added from inside another latent command would land at the END of the queue). FScenarioCommand teleports
		// to its start point; the second half's start (the loaded pose) is only known later, so Begin puts the character there again.
		ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(*this, Start(), Yaw, 600.f,
			[Run, Start](ASupernaturalProtCharacter& Character)
			{
				Character.SetActorLocation(Start(), false, nullptr, ETeleportType::TeleportPhysics);
				Character.GetSpnMovement()->SetBaseGait(ESpnGait::Walk);
				Run->Character = &Character;
			},
			[this, Run](ASupernaturalProtCharacter& Character, float DeltaSeconds, double Elapsed) { return StepRun(*this, *Run, Character, DeltaSeconds, Elapsed); },
			[this, Run, Label](ASupernaturalProtCharacter& Character, bool bTimedOut)
			{
				Run->Keys.ReleaseAll(Character);
				const bool bOk = Run->Failure.IsEmpty() && !bTimedOut;
				SpnQa::Check(*this, &Character, Suite, Label + TEXT("_run"), bOk ? SpnQa::EStatus::Pass : SpnQa::EStatus::Blocked,
					bOk ? FString::Printf(TEXT("all %d steps ran"), Run->Ops.Num()) : (Run->Failure.IsEmpty() ? FString(TEXT("timeout")) : Run->Failure));
			}));
	};

	AutomationOpenMap(WorldMap);
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(4.f));     // the navigation mesh is built when the level starts
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([]
	{
		if (const UWorld* World = FindPlayWorld()) { if (APlayerController* PC = World->GetFirstPlayerController()) { SpnTestPlay::ResetWorldState(*PC); } }
		return true;
	}));
	const FVector Start = Route.Start + FVector(0.f, 0.f, 20.f);
	RunHalf(BuildFirstHalf(Route), TEXT("first_half"), [Start]() { return Start; }, Route.StartYaw);

	// leave for the main menu (the live world is gone, only the save file survives), then the menu's Load: it opens the level of the saved pose
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([]
	{
		if (const UWorld* World = FindPlayWorld())
		{
			if (APlayerController* PC = World->GetFirstPlayerController()) { SpnTestPlay::ResetWorldState(*PC); }
			if (USpnNpcMemorySubsystem* Memory = World->GetGameInstance()->GetSubsystem<USpnNpcMemorySubsystem>()) { Memory->ResetAll(); }
			if (USpnSessionSubsystem* Session = World->GetGameInstance()->GetSubsystem<USpnSessionSubsystem>()) { Session->ReturnToMainMenu(); }
		}
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(5.f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([Shared]
	{
		if (const UWorld* World = FindPlayWorld())
		{
			if (USpnSessionSubsystem* Session = World->GetGameInstance()->GetSubsystem<USpnSessionSubsystem>())
			{
				const FSpnSaveStatus Status = Session->LoadGame(RouteSaveSlot);
				Shared->LoadDetail = FString::Printf(TEXT("from %s: %s %s"), *World->GetMapName(), LexToString(Status.Result), *Status.Detail);
			}
		}
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(10.f));    // level, navigation, pose
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([Shared]
	{
		if (const ASupernaturalProtCharacter* Character = FindCharacter()) { Shared->LoadedLoc = Character->GetActorLocation(); }
		return true;
	}));
	RunHalf(BuildSecondHalf(Route), TEXT("second_half"), [Shared]() { return Shared->LoadedLoc; }, 0.f);

	// "New game" from the main menu, as Tools/play_spnworld.ps1 starts the game (-SpnGameplayMap=/Game/SpnWorld/Lvl_SpnWorld): the world opens
	// with a fresh state at the route start, without any console command.
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([]
	{
		if (const UWorld* World = FindPlayWorld()) { if (USpnSessionSubsystem* Session = World->GetGameInstance()->GetSubsystem<USpnSessionSubsystem>()) { Session->ReturnToMainMenu(); } }
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(5.f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([]
	{
		if (const UWorld* World = FindPlayWorld()) { if (USpnSessionSubsystem* Session = World->GetGameInstance()->GetSubsystem<USpnSessionSubsystem>()) { Session->StartNewGame(); } }   // the menu item's action
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(10.f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, Route]
	{
		const TCHAR* Id = TEXT("new_game_from_the_main_menu_opens_spnworld_fresh");
		const ASupernaturalProtCharacter* Character = FindCharacter();
		const UWorld* World = FindPlayWorld();
		const FString Level = World != nullptr ? World->GetOutermost()->GetName() : FString(TEXT("no world"));
		if (!USpnSessionSubsystem::GameplayMapName().Contains(TEXT("Lvl_SpnWorld")))
		{
			SpnQa::Check(*this, Character, Suite, Id, SpnQa::EStatus::NotTested, FString::Printf(TEXT("the run was started without -SpnGameplayMap=/Game/SpnWorld/Lvl_SpnWorld (New Game opened %s)"), *Level));
			return true;
		}
		if (Character == nullptr || World == nullptr)
		{
			SpnQa::Check(*this, Character, Suite, Id, SpnQa::EStatus::Fail, FString::Printf(TEXT("no player character after New Game (level %s)"), *Level));
			return true;
		}
		const ASpnTestDoor* DoorB = nullptr;
		for (TActorIterator<ASpnTestDoor> It(World); It; ++It) { if (It->StableId == Route.B.DoorId) { DoorB = *It; } }
		const ASpnTestKey* Key = nullptr;
		for (TActorIterator<ASpnTestKey> It(World); It; ++It) { if (It->KeyId == Route.KeyId) { Key = *It; } }
		const USpnInventorySubsystem* Inventory = World->GetGameInstance()->GetSubsystem<USpnInventorySubsystem>();
		FSpnContainerView View;
		const bool bContainer = Inventory != nullptr && Inventory->GetContainer(SpnWorld::PlayerContainerId, View);
		const float FromStart = FVector::Dist2D(Character->GetActorLocation(), Route.Start);
		const bool bOk = Level.Contains(TEXT("Lvl_SpnWorld")) && FromStart < 300.f && DoorB != nullptr && DoorB->IsLocked() && !DoorB->IsOpen() && Key != nullptr && !Key->IsHeld()
			&& bContainer && View.Stacks.Num() == 0;
		SpnQa::Check(*this, Character, Suite, Id, bOk ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail,
			FString::Printf(TEXT("level %s, %.0f cm from the route start, house B %s, key held %d, player container %d with %d stacks"), *Level, FromStart, *DoorText(DoorB), Key != nullptr && Key->IsHeld(), bContainer, View.Stacks.Num()));
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([]
	{
		if (ASupernaturalProtCharacter* Character = FindCharacter())
		{
			if (USpnSaveSubsystem* Save = Character->GetGameInstance()->GetSubsystem<USpnSaveSubsystem>())
			{
				IFileManager::Get().DeleteDirectory(*FPaths::GetPath(Save->GetSlotFilePath(RouteSaveSlot)), false, true);   // the QA slot is removed again
				Save->ResetSaveDirectory();
			}
		}
		return true;
	}));
	return true;
}

#endif
