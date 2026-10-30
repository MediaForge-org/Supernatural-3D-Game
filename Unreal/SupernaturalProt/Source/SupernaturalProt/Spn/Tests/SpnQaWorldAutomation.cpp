#include "Camera/CameraComponent.h"
#include "CollisionQueryParams.h"
#include "Components/SkeletalMeshComponent.h"
#include "DynamicRHI.h"
#include "EngineUtils.h"
#include "GameFramework/SpringArmComponent.h"
#include "HAL/PlatformMemory.h"
#include "RHIStats.h"
#include "RenderTimer.h"
#include "Spn/SpnInteractionComponent.h"
#include "Spn/SpnTestDoor.h"
#include "Spn/Tests/SpnMovementTestSupport.h"
#include "Spn/Tests/SpnQaRecorder.h"
#include "Spn/Tests/SpnTestPlayReset.h"
#include "SpnWorldStateSubsystem.h"

#if WITH_DEV_AUTOMATION_TESTS

// SpnQA.Buildings / SpnQA.DoorReach / SpnQA.Camera / SpnQA.SaveLoad / SpnQA.Performance on the V2 level. Doors are opened with the E KEY
// (see SpnQaDoorsAutomation.cpp); the camera test prepares its doors through the world state because it only measures the camera.

using namespace SpnMovementTest;

namespace
{
	constexpr EAutomationTestFlags QaFlags = EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter;
	FString MapV2Path() { return SpnQa::QaMapPath(); }

	bool OpenV2(FAutomationTestBase& Test)
	{
		if (!FPackageName::DoesPackageExist(MapV2Path()))
		{
			Test.AddError(*FString::Printf(TEXT("%s does not exist: run create_spn_movement_test_map.py first."), *MapV2Path()));
			return false;
		}
		AutomationOpenMap(MapV2Path());
		ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));
		return true;
	}

	ASpnTestDoor* FindDoorById(UWorld& World, const FString& Id)
	{
		for (TActorIterator<ASpnTestDoor> It(&World); It; ++It) { if (It->StableId == Id) { return *It; } }
		return nullptr;
	}

	void FacePoint(ASupernaturalProtCharacter& Character, const FVector& Target)
	{
		const FVector From = Character.GetActorLocation();
		Character.GetController()->SetControlRotation(FRotator(0.f, FMath::RadiansToDegrees(FMath::Atan2(Target.Y - From.Y, Target.X - From.X)), 0.f));
	}

	struct FBuildingRoute
	{
		const TCHAR* Name;
		const TCHAR* DoorId;           // null: the building has no entrance door
		FVector2D Out;
		TArray<FVector2D> In;          // waypoints inside, the last one is the farthest point
		float FinalZ;                  // expected capsule Z at the last waypoint
		bool bStairs;
	};

	TArray<FBuildingRoute> BuildingRoutes()
	{
		return {
			{TEXT("Cottage1"), TEXT("d_cottage1_door"), {-9800, -5440}, {{-9550, -5440}, {-9290, -5440}}, StandZ, false},
			{TEXT("House1"), TEXT("d_house1_door"), {-9800, 8250}, {{-9550, 8250}, {-9330, 8250}, {-9330, 8440}, {-9330, 8886}, {-9330, 9000}}, 300.f + StandZ, true},
			{TEXT("Cottage2"), TEXT("d_cottage2_door"), {-9800, 12340}, {{-9550, 12340}, {-9290, 12340}}, StandZ, false},
			{TEXT("StorageHall"), TEXT("d_storage_door"), {-9700, -12700}, {{-9450, -12700}, {-9190, -12700}}, StandZ, false},
			{TEXT("Workshop"), TEXT("d_workshop_door"), {6300, -12200}, {{6550, -12200}, {6810, -12200}}, StandZ, false},
			{TEXT("Garage"), nullptr, {10900, -12520}, {{11150, -12520}, {11410, -12520}}, StandZ, false},
			{TEXT("CourtHouse"), TEXT("d_courthouse_door"), {4900, 11020}, {{5150, 11020}, {5370, 11020}, {5370, 11190}, {5370, 11686}, {5370, 11800}}, 320.f + StandZ, true},
			{TEXT("Office1"), TEXT("d_office_door"), {13800, 3500}, {{13550, 3500}, {13290, 3500}, {12500, 3500}, {12000, 3500}}, StandZ, false},
			{TEXT("Cottage3"), TEXT("d_cottage3_door"), {3700, 11160}, {{3450, 11160}, {3190, 11160}}, StandZ, false},
		};
	}

	struct FBuildingRun
	{
		FBuildingRoute Route;
		SpnQa::FHeldKeys Keys;
		ASpnTestDoor* Door = nullptr;
		int32 Phase = 0;   // 0 to door, 1 press E, 2 wait open, 3 inside waypoints, 4 return
		int32 Index = 0;
		double PhaseStart = 0.0;
		int32 Frames = 0;
		bool bOpenedByKey = false;
		TArray<FVector2D> Back;
		FString Failure;
		float MinGroundZ = 1.0e9f;
		bool bDone = false;
	};
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnQaBuildingsTest, "SpnQA.Buildings", QaFlags)

bool FSpnQaBuildingsTest::RunTest(const FString& Parameters)
{
	if (!OpenV2(*this)) { return false; }
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([]
	{
		SpnQa::Section(TEXT("Buildings: entrance, door, interior, stairs, return"));
		if (const UWorld* World = FindPlayWorld()) { if (APlayerController* PC = World->GetFirstPlayerController()) { SpnTestPlay::ResetWorldState(*PC); } }
		return true;
	}));

	for (const FBuildingRoute& Route : BuildingRoutes())
	{
		const TSharedRef<FBuildingRun> Run = MakeShared<FBuildingRun>();
		Run->Route = Route;
		const float Yaw = FMath::RadiansToDegrees(FMath::Atan2(Route.In[0].Y - Route.Out.Y, Route.In[0].X - Route.Out.X));
		ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(*this, FVector(Route.Out.X, Route.Out.Y, StandZ), Yaw, 120.f,
			[Run](ASupernaturalProtCharacter& Character)
			{
				SpnQa::Section(FString::Printf(TEXT("Building %s"), Run->Route.Name));
				Character.GetSpnMovement()->SetBaseGait(ESpnGait::Jog);
				if (Run->Route.DoorId != nullptr) { Run->Door = FindDoorById(*Character.GetWorld(), Run->Route.DoorId); }
				// way back: the inside waypoints reversed, then the outside point
				for (int32 I = Run->Route.In.Num() - 2; I >= 0; --I) { Run->Back.Add(Run->Route.In[I]); }
				Run->Back.Add(Run->Route.Out);
			},
			[this, Run](ASupernaturalProtCharacter& Character, float, double Elapsed)
			{
				FBuildingRun& R = *Run;
				APlayerController* PC = Cast<APlayerController>(Character.GetController());
				const FVector Loc = Character.GetActorLocation();
				auto Walk = [&](const FVector2D& P, float Stop) -> int32
				{
					if (FVector2D::Distance(FVector2D(Loc.X, Loc.Y), P) <= Stop) { R.Keys.Set(Character, EKeys::W, false); return 1; }
					if (Elapsed - R.PhaseStart > 25.0) { R.Keys.Set(Character, EKeys::W, false); return -1; }
					FacePoint(Character, FVector(P.X, P.Y, 0.f));
					R.Keys.Set(Character, EKeys::W, true);
					return 0;
				};
				auto Next = [&](int32 Phase) { R.Phase = Phase; R.Index = 0; R.PhaseStart = Elapsed; R.Frames = 0; };
				switch (R.Phase)
				{
				case 0:
				{
					if (R.Door == nullptr)
					{
						if (R.Route.DoorId != nullptr) { R.Failure = TEXT("door actor missing"); return true; }
						Next(3);
						return false;
					}
					const FVector Leaf = R.Door->GetComponentsBoundingBox(false).GetCenter();
					const int32 W = Walk(FVector2D(Leaf.X, Leaf.Y), 200.f);
					if (W < 0) { R.Failure = TEXT("could not walk up to the entrance door"); return true; }
					if (W > 0) { Next(1); }
					return false;
				}
				case 1:
				{
					const FVector Leaf = R.Door->GetComponentsBoundingBox(false).GetCenter();
					FacePoint(Character, Leaf);
					if (Character.GetVelocity().Size2D() > 15.f && Elapsed - R.PhaseStart < 3.0) { return false; }
					SpnQa::Key(*PC, EKeys::E, R.Frames == 0);
					if (++R.Frames > 1) { Next(2); }
					return false;
				}
				case 2:
					if (R.Door->IsOpen() && FMath::IsNearlyEqual(R.Door->GetCurrentAngle(), R.Door->GetTargetAngle(), 1.f)) { R.bOpenedByKey = true; Next(3); }
					else if (Elapsed - R.PhaseStart > 4.0) { R.Failure = TEXT("E did not open the entrance door"); return true; }
					return false;
				case 3:
				{
					if (!R.Route.In.IsValidIndex(R.Index)) { R.bDone = true; Next(4); return false; }
					const int32 W = Walk(R.Route.In[R.Index], 60.f);
					if (W < 0) { R.Failure = FString::Printf(TEXT("stuck %.0f cm from inside waypoint %d at (%.0f, %.0f, %.0f)"), FVector2D::Distance(FVector2D(Loc.X, Loc.Y), R.Route.In[R.Index]), R.Index, Loc.X, Loc.Y, Loc.Z); return true; }
					if (W > 0) { ++R.Index; R.PhaseStart = Elapsed; }
					return false;
				}
				case 4:
				{
					if (R.Index == 0 && R.Frames == 0)
					{
						const bool bHeightOk = FMath::Abs(Loc.Z - R.Route.FinalZ) <= 30.f;
						SpnQa::Check(*this, &Character, TEXT("Buildings"), FString::Printf(TEXT("%s.interior_reached"), R.Route.Name), bHeightOk ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail,
							FString::Printf(TEXT("farthest inside point reached at Z %.0f, expected %.0f +-30%s%s"), Loc.Z, R.Route.FinalZ, R.Route.bStairs ? TEXT(" (upper floor via the stairs)") : TEXT(""), R.bOpenedByKey ? TEXT(", door opened with E") : TEXT(", no door")));
						R.Frames = 1;
					}
					if (!R.Back.IsValidIndex(R.Index)) { R.Keys.Set(Character, EKeys::W, false); return true; }
					const int32 W = Walk(R.Back[R.Index], 70.f);
					if (W < 0) { R.Failure = FString::Printf(TEXT("return path blocked at (%.0f, %.0f, %.0f)"), Loc.X, Loc.Y, Loc.Z); return true; }
					if (W > 0) { ++R.Index; R.PhaseStart = Elapsed; }
					return false;
				}
				}
				return true;
			},
			[this, Run](ASupernaturalProtCharacter& Character, bool bTimedOut)
			{
				Run->Keys.ReleaseAll(Character);
				const FVector End = Character.GetActorLocation();
				if (!Run->Failure.IsEmpty() || bTimedOut)
				{
					SpnQa::Check(*this, &Character, TEXT("Buildings"), FString::Printf(TEXT("%s.route"), Run->Route.Name), SpnQa::EStatus::Blocked,
						Run->Failure.IsEmpty() ? FString(TEXT("timeout of the building run")) : Run->Failure);
					return;
				}
				const bool bBackOnGround = End.Z < StandZ + 30.f;
				SpnQa::Check(*this, &Character, TEXT("Buildings"), FString::Printf(TEXT("%s.return"), Run->Route.Name), bBackOnGround ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail,
					FString::Printf(TEXT("back outside at (%.0f, %.0f, %.0f)"), End.X, End.Y, End.Z));
			}));
		ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.3f));
	}
	return true;
}

// ----------------------------------------------------------------------------------------------------------- DoorReach

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnQaDoorReachTest, "SpnQA.DoorReach", QaFlags)

bool FSpnQaDoorReachTest::RunTest(const FString& Parameters)
{
	if (!OpenV2(*this)) { return false; }
	const TSharedRef<int32> Cell = MakeShared<int32>(0);
	const TSharedRef<TArray<FString>> Misses = MakeShared<TArray<FString>>();
	const TSharedRef<int32> Hits = MakeShared<int32>(0);
	static const float Distances[] = {30.f, 60.f, 120.f, 200.f, 240.f, 300.f};
	static const float Pitches[] = {10.f, 0.f, -20.f, -40.f};
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([] { SpnQa::Section(TEXT("Interaction reach: distance x camera pitch at a closed door")); return true; }));
	ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(*this, FVector(-9800, -5440, StandZ), 0.f, 30.f, nullptr,
		[Cell, Misses, Hits](ASupernaturalProtCharacter& Character, float, double)
		{
			const int32 N = UE_ARRAY_COUNT(Distances) * UE_ARRAY_COUNT(Pitches);
			if (*Cell >= N) { return true; }
			ASpnTestDoor* Door = FindDoorById(*Character.GetWorld(), TEXT("d_cottage1_door"));
			if (Door == nullptr) { return true; }
			const FVector Leaf = Door->GetComponentsBoundingBox(false).GetCenter();
			const float D = Distances[*Cell / UE_ARRAY_COUNT(Pitches)];
			const float P = Pitches[*Cell % UE_ARRAY_COUNT(Pitches)];
			Character.GetCharacterMovement()->StopMovementImmediately();
			Character.SetActorLocation(FVector(Leaf.X - D - 42.f, Leaf.Y, StandZ), false, nullptr, ETeleportType::TeleportPhysics);
			Character.SetActorRotation(FRotator(0, 0, 0));
			Character.GetController()->SetControlRotation(FRotator(P, 0.f, 0.f));
			USpnInteractionComponent* Interaction = Character.FindComponentByClass<USpnInteractionComponent>();
			const bool bFound = Interaction->FindTarget() == Door;
			*Hits += bFound ? 1 : 0;
			if (!bFound) { Misses->Add(FString::Printf(TEXT("%.0f cm/%.0f deg"), D, P)); }
			++*Cell;
			return false;
		},
		[this, Misses, Hits](ASupernaturalProtCharacter& Character, bool)
		{
			const int32 N = UE_ARRAY_COUNT(Distances) * UE_ARRAY_COUNT(Pitches);
			// Expectation (placeholder numbers): 60 to 200 cm at pitch +10..-20 degrees must find the door; the rest is reported.
			bool bCoreOk = true;
			for (const FString& M : *Misses)
			{
				for (const float D : {60.f, 120.f, 200.f})
				{
					for (const float P : {10.f, 0.f, -20.f}) { if (M == FString::Printf(TEXT("%.0f cm/%.0f deg"), D, P)) { bCoreOk = false; } }
				}
			}
			SpnQa::Check(*this, &Character, TEXT("DoorReach"), TEXT("reach_matrix"), bCoreOk ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail,
				FString::Printf(TEXT("door found in %d of %d (distance from the capsule edge x camera pitch); misses: %s. Must hit: 60-200 cm at pitch +10..-20 deg."), *Hits, N, Misses->Num() ? *FString::Join(*Misses, TEXT(", ")) : TEXT("none")));
		}));
	return true;
}

// ---------------------------------------------------------------------------------------------------------------- Camera

namespace
{
	struct FCameraRun
	{
		SpnQa::FHeldKeys Keys;
		int32 Samples = 0, Inside = 0, Occluded = 0, Hidden = 0;
		float MinArm = 1.0e9f;
		TArray<FVector2D> Points;
		int32 Index = 0;
		double PhaseStart = 0.0;
		double JumpAt = -1.0;
	};
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnQaCameraTest, "SpnQA.Camera", QaFlags)

bool FSpnQaCameraTest::RunTest(const FString& Parameters)
{
	if (!OpenV2(*this)) { return false; }
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([]
	{
		SpnQa::Section(TEXT("Camera: doorway, narrow hall, stairwell, jump, open street"));
		if (const UWorld* World = FindPlayWorld()) { if (APlayerController* PC = World->GetFirstPlayerController()) { SpnTestPlay::ResetWorldState(*PC); } }
		return true;
	}));
	struct FCase { const TCHAR* Name; FVector Start; float Yaw; TArray<FVector2D> Points; bool bJump; bool bOpenDoors; bool bOpenStreet; };
	const FCase Cases[] = {
		{TEXT("open_street"), FVector(-12500, -12000, StandZ), 90.f, {{-12500, -11000}}, false, false, true},
		{TEXT("doorway_cottage1"), FVector(-9800, -5440, StandZ), 0.f, {{-9550, -5440}, {-9290, -5440}}, false, true, false},
		{TEXT("stairwell_house1"), FVector(-9800, 8250, StandZ), 0.f, {{-9550, 8250}, {-9330, 8250}, {-9330, 8440}, {-9330, 8886}, {-9330, 9000}}, false, true, false},
		{TEXT("corridor_office1"), FVector(13800, 3500, StandZ), 180.f, {{13550, 3500}, {13290, 3500}, {12500, 3500}, {12000, 3500}}, false, true, false},
		{TEXT("jump_in_hall_house1"), FVector(-9550, 8250, StandZ), 0.f, {{-9330, 8250}, {-9330, 8440}}, true, true, false},
	};
	for (const FCase& Case : Cases)
	{
		const TSharedRef<FCameraRun> Run = MakeShared<FCameraRun>();
		Run->Points = Case.Points;
		ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(*this, Case.Start, Case.Yaw, 60.f,
			[Case, Run](ASupernaturalProtCharacter& Character)
			{
				SpnQa::Section(FString::Printf(TEXT("Camera: %s"), Case.Name));
				Character.GetSpnMovement()->SetBaseGait(ESpnGait::Jog);
				if (Case.bOpenDoors)
				{
					for (TActorIterator<ASpnTestDoor> It(Character.GetWorld()); It; ++It) { if (!It->IsOpen()) { It->SpnUse(&Character); } }
				}
			},
			[Case, Run](ASupernaturalProtCharacter& Character, float, double Elapsed)
			{
				UWorld* World = Character.GetWorld();
				const FVector Loc = Character.GetActorLocation();
				if (!Run->Points.IsValidIndex(Run->Index)) { return true; }
				const FVector2D To = Run->Points[Run->Index] - FVector2D(Loc.X, Loc.Y);
				if (To.Size() < 60.f) { ++Run->Index; Run->PhaseStart = Elapsed; return false; }
				if (Elapsed - Run->PhaseStart > 20.0) { return true; }
				Character.GetController()->SetControlRotation(FRotator(-10.f, FMath::RadiansToDegrees(FMath::Atan2(To.Y, To.X)), 0.f));
				Run->Keys.Set(Character, EKeys::W, true);
				if (Case.bJump && Elapsed > 1.5)
				{
					if (Run->JumpAt < 0.0 || Elapsed - Run->JumpAt > 1.2) { Run->JumpAt = Elapsed; Run->Keys.Set(Character, EKeys::SpaceBar, true); }
					else if (Elapsed - Run->JumpAt > 0.1) { Run->Keys.Set(Character, EKeys::SpaceBar, false); }
				}
				const UCameraComponent* Camera = Character.GetFollowCamera();
				const FVector Cam = Camera->GetComponentLocation();
				const FVector Head = Loc + FVector(0, 0, 60.f);
				FCollisionQueryParams Params(SCENE_QUERY_STAT(SpnQaCamera), false, &Character);
				++Run->Samples;
				if (World->OverlapBlockingTestByChannel(Cam, FQuat::Identity, ECC_Camera, FCollisionShape::MakeSphere(6.f), Params)) { ++Run->Inside; }
				FHitResult Hit;
				if (World->LineTraceSingleByChannel(Hit, Head, Cam, ECC_Visibility, Params)) { ++Run->Occluded; }
				Run->Hidden += Character.GetMesh()->bOwnerNoSee ? 1 : 0;
				Run->MinArm = FMath::Min(Run->MinArm, static_cast<float>(FVector::Dist(Cam, Character.GetCameraBoom()->GetComponentLocation())));
				return false;
			},
			[this, Case, Run](ASupernaturalProtCharacter& Character, bool bTimedOut)
			{
				Run->Keys.ReleaseAll(Character);
				const bool bFinished = !Run->Points.IsValidIndex(Run->Index);
				if (!bFinished) { SpnQa::Check(*this, &Character, TEXT("Camera"), FString::Printf(TEXT("%s.route"), Case.Name), SpnQa::EStatus::Blocked, TEXT("route not completed, camera samples are partial")); }
				const float HiddenPct = Run->Samples ? 100.f * Run->Hidden / Run->Samples : 0.f, OccPct = Run->Samples ? 100.f * Run->Occluded / Run->Samples : 0.f;
				SpnQa::Check(*this, &Character, TEXT("Camera"), FString::Printf(TEXT("%s.camera_in_geometry"), Case.Name), Run->Inside == 0 ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail,
					FString::Printf(TEXT("%d of %d frames with the camera inside blocking geometry; shortest arm %.0f cm"), Run->Inside, Run->Samples, Run->MinArm));
				// Visibility of the player: the mesh is hidden on purpose when the arm is shorter than 70 cm; in the open it must always be visible.
				const bool bVisOk = Case.bOpenStreet ? (Run->Hidden == 0 && Run->Occluded == 0) : true;
				SpnQa::Check(*this, &Character, TEXT("Camera"), FString::Printf(TEXT("%s.player_visible"), Case.Name), bVisOk ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail,
					FString::Printf(TEXT("character mesh hidden %.0f%% of the frames (hidden by design below 70 cm arm), line of sight head-to-camera blocked %.0f%%%s"), HiddenPct, OccPct, Case.bOpenStreet ? TEXT(" (must be 0 in the open)") : TEXT("")));
			}));
		ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.3f));
	}
	return true;
}

// -------------------------------------------------------------------------------------------------------------- SaveLoad

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnQaSaveLoadTest, "SpnQA.SaveLoad", QaFlags)

bool FSpnQaSaveLoadTest::RunTest(const FString& Parameters)
{
	if (!OpenV2(*this)) { return false; }
	struct FState { int32 Step = 0; double At = 0; };
	const TSharedRef<FState> State = MakeShared<FState>();
	const FString Slot = TEXT("spn_qa_saveload");
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([] { SpnQa::Section(TEXT("Save / Load: door state through Spn.SaveWorld / Spn.LoadWorld")); return true; }));
	ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(*this, FVector(-9800, -5440, StandZ), 0.f, 40.f,
		[Slot](ASupernaturalProtCharacter& Character)
		{
			SpnTestPlay::ResetWorldState(*Cast<APlayerController>(Character.GetController()));
		},
		[this, State, Slot](ASupernaturalProtCharacter& Character, float, double Elapsed)
		{
			APlayerController* PC = Cast<APlayerController>(Character.GetController());
			ASpnTestDoor* Door = FindDoorById(*Character.GetWorld(), TEXT("d_cottage1_door"));
			if (Door == nullptr) { SpnQa::Check(*this, &Character, TEXT("SaveLoad"), TEXT("door"), SpnQa::EStatus::Blocked, TEXT("d_cottage1_door missing")); return true; }
			const FVector Leaf = Door->GetComponentsBoundingBox(false).GetCenter();
			auto Settled = [&](double Wait) { return Elapsed - State->At >= Wait; };
			auto Stand = [&]
			{
				Character.GetCharacterMovement()->StopMovementImmediately();
				Character.SetActorLocation(FVector(Leaf.X - 150.f - 42.f, Leaf.Y, StandZ), false, nullptr, ETeleportType::TeleportPhysics);
				Character.GetController()->SetControlRotation(FRotator(0, 0, 0));
			};
			switch (State->Step)
			{
			case 0: Stand(); SpnQa::Key(*PC, EKeys::E, true); State->Step = 1; State->At = Elapsed; return false;
			case 1: SpnQa::Key(*PC, EKeys::E, false); if (!Settled(1.0)) { return false; }
				SpnQa::Check(*this, &Character, TEXT("SaveLoad"), TEXT("open_by_key"), Door->IsOpen() ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail, TEXT("E opened the door before saving"));
				PC->ConsoleCommand(FString::Printf(TEXT("Spn.SaveWorld %s"), *Slot), true);
				State->Step = 2; State->At = Elapsed; return false;
			case 2: Stand(); SpnQa::Key(*PC, EKeys::E, true); State->Step = 3; State->At = Elapsed; return false;
			case 3: SpnQa::Key(*PC, EKeys::E, false); if (!Settled(1.0)) { return false; }
				SpnQa::Check(*this, &Character, TEXT("SaveLoad"), TEXT("changed_after_save"), !Door->IsOpen() ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail, TEXT("E closed the door again after saving"));
				PC->ConsoleCommand(FString::Printf(TEXT("Spn.LoadWorld %s"), *Slot), true);
				State->Step = 4; State->At = Elapsed; return false;
			case 4: if (!Settled(0.5)) { return false; }
			{
				const bool bOk = Door->IsOpen() && FMath::IsNearlyEqual(Door->GetCurrentAngle(), Door->OpenAngle, 1.f);
				SpnQa::Check(*this, &Character, TEXT("SaveLoad"), TEXT("door_state_restored"), bOk ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail,
					FString::Printf(TEXT("after Spn.LoadWorld the door is %s, angle %.0f (saved: open)"), Door->IsOpen() ? TEXT("open") : TEXT("closed"), Door->GetCurrentAngle()));
				return true;
			}
			}
			return true;
		},
		[this, Slot](ASupernaturalProtCharacter& Character, bool bTimedOut)
		{
			if (bTimedOut) { SpnQa::Check(*this, &Character, TEXT("SaveLoad"), TEXT("timeout"), SpnQa::EStatus::Blocked, TEXT("save/load sequence did not finish")); }
			IFileManager::Get().DeleteDirectory(*FPaths::GetPath(USpnWorldStateSubsystem::GetSlotFilePath(Slot)), false, true);
			SpnQa::Check(*this, &Character, TEXT("SaveLoad"), TEXT("player_position_saved"), SpnQa::EStatus::NotImplemented, TEXT("NOT IMPLEMENTED on THIS path: the console commands Spn.SaveWorld / Spn.LoadWorld save the world state only. The player pose IS saved by the game flow (USpnSaveSubsystem, section player.pose; tests SpnCore.Save.PlayerPose* and SpnGame.NightBuild.PoseWorld), which this level does not enable"));
			SpnQa::Check(*this, &Character, TEXT("SaveLoad"), TEXT("save_ui_and_autosave"), SpnQa::EStatus::NotImplemented, TEXT("not on this path: this level has only the console commands Spn.SaveWorld / Spn.LoadWorld. The game flow (menu, pause, save slots, Spn.Game.Enable) has a save-slot UI; there is still no quick-save key or autosave"));
			SpnQa::Check(*this, &Character, TEXT("SaveLoad"), TEXT("inventory_save_load"), SpnQa::EStatus::NotTested, TEXT("covered by SpnGame.TestPickup.SaveLoadRestoresInventory and SpnGame.TestMap.PlayPickups; the V2 level has no pickups"));
		}));
	return true;
}

// ----------------------------------------------------------------------------------------------------------- Performance

namespace
{
	struct FPerf { double Frame = 0, Worst = 0, Game = 0, Render = 0, Gpu = 0, Draws = 0; int32 N = 0; TArray<float> Frames; SpnQa::FHeldKeys Keys; };
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnQaPerformanceTest, "SpnQA.Performance", QaFlags)

bool FSpnQaPerformanceTest::RunTest(const FString& Parameters)
{
	if (!OpenV2(*this)) { return false; }
	const TSharedRef<FPerf> Perf = MakeShared<FPerf>();
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([] { SpnQa::Section(TEXT("Performance: 12 s of sprinting through the district street")); return true; }));
	ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(*this, FVector(-12500, -12000, StandZ), 90.f, 13.f, nullptr,
		[Perf](ASupernaturalProtCharacter& Character, float Dt, double Elapsed)
		{
			Perf->Keys.Set(Character, EKeys::W, true);
			Perf->Keys.Set(Character, EKeys::LeftShift, true);
			if (Elapsed < 1.f) { return false; } // skip the first second (shader/streaming warm-up)
			const double Ms = FApp::GetDeltaTime() * 1000.0;
			Perf->Frame += Ms; Perf->Worst = FMath::Max(Perf->Worst, Ms); Perf->Frames.Add(static_cast<float>(Ms));
			Perf->Game += FPlatformTime::ToMilliseconds(GGameThreadTime); Perf->Render += FPlatformTime::ToMilliseconds(GRenderThreadTime);
			Perf->Gpu += FPlatformTime::ToMilliseconds(RHIGetGPUFrameCycles(0)); Perf->Draws += GNumDrawCallsRHI[0];
			++Perf->N;
			return Elapsed > 12.0;
		},
		[this, Perf](ASupernaturalProtCharacter& Character, bool)
		{
			Perf->Keys.ReleaseAll(Character);
			if (Perf->N == 0) { SpnQa::Check(*this, &Character, TEXT("Performance"), TEXT("frame_stats"), SpnQa::EStatus::Blocked, TEXT("no frames sampled")); return; }
			Perf->Frames.Sort();
			const float P95 = Perf->Frames[FMath::Min(Perf->Frames.Num() - 1, static_cast<int32>(Perf->Frames.Num() * 0.95f))];
			const float P99 = Perf->Frames[FMath::Min(Perf->Frames.Num() - 1, static_cast<int32>(Perf->Frames.Num() * 0.99f))];
			const double AvgMs = Perf->Frame / Perf->N;
			const FPlatformMemoryStats Mem = FPlatformMemory::GetStats();
			// Placeholder budget (not an approved target): >= 30 FPS on average while recording, worst frame below 100 ms.
			const bool bOk = AvgMs > 0 && 1000.0 / AvgMs >= 30.0 && Perf->Worst < 100.0;
			SpnQa::Check(*this, &Character, TEXT("Performance"), TEXT("frame_stats"), bOk ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail,
				FString::Printf(TEXT("avg %.1f FPS (%.2f ms), p95 %.2f ms, p99 %.2f ms, worst %.1f ms, game thread %.2f ms, render thread %.2f ms, GPU %.2f ms, draw calls %.0f, used physical memory %.0f MB, %d frames, %dx%d"),
					1000.0 / AvgMs, AvgMs, P95, P99, Perf->Worst, Perf->Game / Perf->N, Perf->Render / Perf->N, Perf->Gpu / Perf->N, Perf->Draws / Perf->N, Mem.UsedPhysical / (1024.0 * 1024.0), Perf->N,
					GEngine->GameViewport != nullptr && GEngine->GameViewport->Viewport != nullptr ? GEngine->GameViewport->Viewport->GetSizeXY().X : 0,
					GEngine->GameViewport != nullptr && GEngine->GameViewport->Viewport != nullptr ? GEngine->GameViewport->Viewport->GetSizeXY().Y : 0));
		}));
	return true;
}

#endif
