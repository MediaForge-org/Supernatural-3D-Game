#include "Components/CapsuleComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "Spn/SpnPlayAreaGuard.h"
#include "Spn/Tests/SpnMovementTestSupport.h"
#include "Spn/Tests/SpnQaRecorder.h"

#if WITH_DEV_AUTOMATION_TESTS

// SpnQA.Spawn / SpnQA.Movement / SpnQA.Turns / SpnQA.Jump on /Game/SpnTest/Lvl_SpnMovementTestV2 (the V1 speed lane x -12500, y from -12000 is
// part of V2). Input goes through the player controller as KEY events (W/A/S/D, LeftShift, SpaceBar, V), the same entry point as the
// keyboard; only the aim (control rotation) is set directly. Obstacles for the jump cases are runtime-spawned boxes (never saved).
// Spec values (Walk 1.8, Jog 3.5, Run 5.5, Sprint 8.0 m/s; jump apex 45 cm) are the CURRENT placeholder numbers, not approved tuning.

using namespace SpnMovementTest;

namespace
{
	const TCHAR* MapV2Path() { return TEXT("/Game/SpnTest/Lvl_SpnMovementTestV2"); }
	constexpr float LaneX = -12500.f;
	constexpr float LaneStartY = -12000.f;
	constexpr float CapsuleRadius = 42.f;
	constexpr EAutomationTestFlags QaFlags = EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter;

	bool OpenV2(FAutomationTestBase& Test)
	{
		if (!FPackageName::DoesPackageExist(MapV2Path()))
		{
			Test.AddError(TEXT("/Game/SpnTest/Lvl_SpnMovementTestV2 does not exist: run create_spn_movement_test_map.py --v2 first."));
			return false;
		}
		AutomationOpenMap(MapV2Path());
		ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));
		return true;
	}

	AStaticMeshActor* SpawnBlock(UWorld& World, const FVector& Center, const FVector& Size)
	{
		AStaticMeshActor* Actor = World.SpawnActor<AStaticMeshActor>(Center, FRotator::ZeroRotator);
		Actor->SetMobility(EComponentMobility::Movable);
		Actor->GetStaticMeshComponent()->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube")));
		Actor->GetStaticMeshComponent()->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
		Actor->SetActorScale3D(Size / 100.f);
		return Actor;
	}

	const TCHAR* GaitName(ESpnGait Gait)
	{
		switch (Gait) { case ESpnGait::Walk: return TEXT("walk"); case ESpnGait::Jog: return TEXT("jog"); case ESpnGait::Run: return TEXT("run"); default: return TEXT("sprint"); }
	}

	/** Puts the base gait on `Gait` through the V key (cycle Walk -> Jog -> Run -> Walk), pressing it at most three times. */
	void SetBaseGaitWithKey(ASupernaturalProtCharacter& Character, ESpnGait Gait, SpnQa::FHeldKeys& Keys)
	{
		for (int32 I = 0; I < 3 && Character.GetSpnMovement()->GetBaseGait() != Gait; ++I)
		{
			Keys.Set(Character, EKeys::V, true);
			Keys.Set(Character, EKeys::V, false);
		}
	}
}

// ---------------------------------------------------------------------------------------------------------------- Spawn

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnQaSpawnTest, "SpnQA.Spawn", QaFlags)

bool FSpnQaSpawnTest::RunTest(const FString& Parameters)
{
	if (!OpenV2(*this)) { return false; }
	ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(*this, FVector(0.f, 0.f, 100.f), 0.f, 1.5f, nullptr,
		[](ASupernaturalProtCharacter&, float, double) { return false; },
		[this](ASupernaturalProtCharacter& Character, bool)
		{
			SpnQa::Section(TEXT("Spawn: PlayerStart, one player, no overlaps"));
			UWorld* World = Character.GetWorld();
			TArray<APlayerStart*> Starts;
			for (TActorIterator<APlayerStart> It(World); It; ++It) { Starts.Add(*It); }
			SpnQa::Check(*this, &Character, TEXT("Spawn"), TEXT("one_player_start"), Starts.Num() == 1 ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail,
				FString::Printf(TEXT("%d APlayerStart in the level"), Starts.Num()));
			if (Starts.Num() == 1)
			{
				const float Dist = FVector::Dist2D(Starts[0]->GetActorLocation(), Character.GetActorLocation());
				SpnQa::Check(*this, &Character, TEXT("Spawn"), TEXT("at_player_start"), Dist < 150.f ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail,
					FString::Printf(TEXT("player %.0f cm (horizontal) from PlayerStart (%.0f, %.0f)"), Dist, Starts[0]->GetActorLocation().X, Starts[0]->GetActorLocation().Y));
			}
			int32 Players = 0, Characters = 0;
			TArray<FString> OtherPawns;
			for (TActorIterator<APawn> It(World); It; ++It)
			{
				if (It->IsPlayerControlled()) { ++Players; } else { OtherPawns.Add(GetNameSafe(It->GetClass())); }
				Characters += Cast<ASupernaturalProtCharacter>(*It) != nullptr ? 1 : 0;
			}
			SpnQa::Check(*this, &Character, TEXT("Spawn"), TEXT("single_player_pawn"), Players == 1 && Characters == 1 ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail,
				FString::Printf(TEXT("%d player-controlled pawn(s), %d ASupernaturalProtCharacter(s) in the world"), Players, Characters));
			if (OtherPawns.Num() == 0)
			{
				SpnQa::Check(*this, &Character, TEXT("Spawn"), TEXT("npc_vs_player"), SpnQa::EStatus::NotImplemented,
					TEXT("no NPC/test pawn exists in the V2 level (Core Npc module is not integrated in Unreal); the guard is an ASpnPlayAreaGuard volume, not a pawn"));
			}
			else
			{
				SpnQa::Check(*this, &Character, TEXT("Spawn"), TEXT("npc_vs_player"), SpnQa::EStatus::Pass, FString::Printf(TEXT("%d non-player pawn(s) distinct from the player: %s"), OtherPawns.Num(), *FString::Join(OtherPawns, TEXT(", "))));
			}
			int32 Guards = 0;
			for (TActorIterator<ASpnPlayAreaGuard> It(World); It; ++It) { ++Guards; }
			SpnQa::Check(*this, &Character, TEXT("Spawn"), TEXT("play_area_guard"), Guards == 1 ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail, FString::Printf(TEXT("%d ASpnPlayAreaGuard"), Guards));
			FCollisionQueryParams Params(SCENE_QUERY_STAT(SpnQaSpawn), false, &Character);
			const bool bOverlap = World->OverlapBlockingTestByChannel(Character.GetActorLocation(), FQuat::Identity, ECC_Pawn,
				FCollisionShape::MakeCapsule(Character.GetCapsuleComponent()->GetScaledCapsuleRadius() - 1.f, Character.GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - 1.f), Params);
			SpnQa::Check(*this, &Character, TEXT("Spawn"), TEXT("no_spawn_overlap"), bOverlap ? SpnQa::EStatus::Fail : SpnQa::EStatus::Pass, bOverlap ? TEXT("capsule overlaps blocking geometry at spawn") : TEXT("capsule free of blocking geometry"));
			SpnQa::Check(*this, &Character, TEXT("Spawn"), TEXT("grounded"), Character.GetCharacterMovement()->IsMovingOnGround() ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail,
				FString::Printf(TEXT("movement mode %d, Z %.0f"), static_cast<int32>(Character.GetCharacterMovement()->MovementMode.GetValue()), Character.GetActorLocation().Z));
		}));
	return true;
}

// -------------------------------------------------------------------------------------------------------------- Movement

namespace
{
	struct FGaitRun
	{
		double Sum = 0; int32 N = 0; double T95 = -1; double BrakeStart = -1; FVector BrakePos = FVector::ZeroVector; double BrakeTime = -1; float BrakeDist = 0;
		float MaxSpeed = 0; SpnQa::FHeldKeys Keys;
	};
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnQaMovementTest, "SpnQA.Movement", QaFlags)

bool FSpnQaMovementTest::RunTest(const FString& Parameters)
{
	if (!OpenV2(*this)) { return false; }
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([] { SpnQa::Section(TEXT("Movement: gaits, acceleration, braking (keys W / V / Shift)")); return true; }));

	// V key: Walk -> Jog -> Run -> Walk
	ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(*this, FVector(LaneX, LaneStartY, StandZ), 90.f, 3.f, nullptr,
		[this](ASupernaturalProtCharacter& Character, float, double Elapsed)
		{
			static SpnQa::FHeldKeys Keys;
			static int32 Phase = 0;
			static TArray<int32> Seen;
			if (Elapsed < 0.1) { Phase = 0; Seen.Reset(); Character.GetSpnMovement()->SetBaseGait(ESpnGait::Walk); return false; }
			if (Phase < 6 && (Phase % 2) == 0) { Keys.Set(Character, EKeys::V, true); }
			else if (Phase < 6) { Keys.Set(Character, EKeys::V, false); Seen.Add(static_cast<int32>(Character.GetSpnMovement()->GetBaseGait())); }
			++Phase;
			if (Phase < 8) { return false; }
			const bool bOk = Seen.Num() == 3 && Seen[0] == static_cast<int32>(ESpnGait::Jog) && Seen[1] == static_cast<int32>(ESpnGait::Run) && Seen[2] == static_cast<int32>(ESpnGait::Walk);
			SpnQa::Check(*this, &Character, TEXT("Movement"), TEXT("gait_cycle_key_V"), bOk ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail,
				FString::Printf(TEXT("V pressed 3 times starting at Walk gave gaits %d,%d,%d (expected Jog=%d, Run=%d, Walk=%d)"), Seen.IsValidIndex(0) ? Seen[0] : -1, Seen.IsValidIndex(1) ? Seen[1] : -1, Seen.IsValidIndex(2) ? Seen[2] : -1,
					static_cast<int32>(ESpnGait::Jog), static_cast<int32>(ESpnGait::Run), static_cast<int32>(ESpnGait::Walk)));
			return true;
		}, nullptr));

	struct FCase { ESpnGait Gait; bool bSprint; float SpecCm; };
	const FCase Cases[] = {{ESpnGait::Walk, false, 180.f}, {ESpnGait::Jog, false, 350.f}, {ESpnGait::Run, false, 550.f}, {ESpnGait::Jog, true, 800.f}};
	for (const FCase& Case : Cases)
	{
		const TSharedRef<FGaitRun> Run = MakeShared<FGaitRun>();
		const FString Name = Case.bSprint ? TEXT("sprint") : GaitName(Case.Gait);
		ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(*this, FVector(LaneX, LaneStartY, StandZ), 90.f, 20.f,
			[Run, Case, Name](ASupernaturalProtCharacter& Character)
			{
				SpnQa::Section(FString::Printf(TEXT("Movement: %s (spec %.1f m/s)"), *Name, Case.SpecCm / 100.f));
				Character.GetSpnMovement()->SetBaseGait(Case.Gait); // the V key itself is checked above (gait_cycle_key_V)
			},
			[Run, Case](ASupernaturalProtCharacter& Character, float, double Elapsed)
			{
				const float Speed = HorizontalSpeed(Character);
				Run->MaxSpeed = FMath::Max(Run->MaxSpeed, Speed);
				if (Run->BrakeStart < 0.0)
				{
					Run->Keys.Set(Character, EKeys::W, true);
					Run->Keys.Set(Character, EKeys::LeftShift, Case.bSprint);
					if (Run->T95 < 0.0 && Speed >= 0.95f * Case.SpecCm) { Run->T95 = Elapsed; }
					if (Elapsed > 5.0 - 1.0 && Elapsed <= 5.0) { Run->Sum += Speed; ++Run->N; }
					if (Elapsed > 5.0)
					{
						Run->Keys.Set(Character, EKeys::W, false);
						Run->Keys.Set(Character, EKeys::LeftShift, false);
						Run->BrakeStart = Elapsed;
						Run->BrakePos = Character.GetActorLocation();
					}
					return false;
				}
				if (Speed < 10.f)
				{
					Run->BrakeTime = Elapsed - Run->BrakeStart;
					Run->BrakeDist = FVector::Dist2D(Run->BrakePos, Character.GetActorLocation());
					return true;
				}
				return Elapsed - Run->BrakeStart > 5.0;
			},
			[this, Run, Case, Name](ASupernaturalProtCharacter& Character, bool bTimedOut)
			{
				Run->Keys.ReleaseAll(Character);
				const float Avg = Run->N > 0 ? static_cast<float>(Run->Sum / Run->N) : 0.f;
				const bool bSpeedOk = FMath::Abs(Avg - Case.SpecCm) <= 0.03f * Case.SpecCm;
				SpnQa::Check(*this, &Character, TEXT("Movement"), FString::Printf(TEXT("speed_%s"), *Name), bSpeedOk ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail,
					FString::Printf(TEXT("steady %.0f cm/s over the last second, spec %.0f +-3%% (max %.0f)"), Avg, Case.SpecCm, Run->MaxSpeed));
				const bool bAccelOk = Run->T95 >= 0.0 && Run->T95 <= 3.5;
				SpnQa::Check(*this, &Character, TEXT("Movement"), FString::Printf(TEXT("accel_%s"), *Name), bAccelOk ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail,
					FString::Printf(TEXT("95%% of the speed after %.2f s (limit 3.5 s, placeholder)"), Run->T95));
				const bool bBrakeOk = Run->BrakeTime >= 0.0 && Run->BrakeTime <= 2.0;
				SpnQa::Check(*this, &Character, TEXT("Movement"), FString::Printf(TEXT("brake_%s"), *Name), bBrakeOk ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail,
					FString::Printf(TEXT("stop in %.2f s over %.0f cm after releasing the key (limit 2 s, placeholder)"), Run->BrakeTime, Run->BrakeDist));
			}));
		ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.2f));
	}
	return true;
}

// ----------------------------------------------------------------------------------------------------------------- Turns

namespace
{
	struct FTurnResult { float Before = 0, MinSpeed = 0, GaitSprint = 0, Final = 0, TimeToRecover = -1; bool bValid = false; float LocalAngle = 0; };
	struct FTurnShared { TMap<FString, TArray<FTurnResult>> Results; };
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnQaTurnsTest, "SpnQA.Turns", QaFlags)

bool FSpnQaTurnsTest::RunTest(const FString& Parameters)
{
	if (!OpenV2(*this)) { return false; }
	const TSharedRef<FTurnShared> Shared = MakeShared<FTurnShared>();

	struct FCase { const TCHAR* Name; bool bSprint; FKey Key; bool bReleaseW; };
	const FCase Cases[] = {
		{TEXT("sprint_right_90"), true, EKeys::D, true}, {TEXT("sprint_left_90"), true, EKeys::A, true},
		{TEXT("jog_right_90"), false, EKeys::D, true}, {TEXT("jog_left_90"), false, EKeys::A, true},
		{TEXT("jog_back_180"), false, EKeys::S, true}, {TEXT("sprint_back_180"), true, EKeys::S, true},
	};
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([] { SpnQa::Section(TEXT("Turns: 90/180 degrees, sprint left vs right")); return true; }));
	for (int32 Trial = 0; Trial < 3; ++Trial)
	{
		for (const FCase& Case : Cases)
		{
			const TSharedRef<FTurnResult> R = MakeShared<FTurnResult>();
			const TSharedRef<SpnQa::FHeldKeys> Keys = MakeShared<SpnQa::FHeldKeys>();
			const TSharedRef<double> Phase1 = MakeShared<double>(-1.0);
			ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(*this, FVector(LaneX, LaneStartY, StandZ), 90.f, 14.f,
				[Case, Keys](ASupernaturalProtCharacter& Character)
				{
					Character.GetSpnMovement()->SetBaseGait(ESpnGait::Walk);
					SetBaseGaitWithKey(Character, ESpnGait::Jog, *Keys);
				},
				[Case, R, Keys, Phase1](ASupernaturalProtCharacter& Character, float, double Elapsed)
				{
					const float Speed = HorizontalSpeed(Character);
					if (*Phase1 < 0.0)
					{
						Keys->Set(Character, EKeys::W, true);
						Keys->Set(Character, EKeys::LeftShift, Case.bSprint);
						if (Elapsed > 4.5)
						{
							*Phase1 = Elapsed; R->Before = Speed; R->MinSpeed = Speed;
							// turn: the forward key goes up, the side/back key goes down in the same frame
							Keys->Set(Character, EKeys::W, false);
							Keys->Set(Character, Case.Key, true);
						}
						return false;
					}
					const double Since = Elapsed - *Phase1;
					if (Since > 0.03 && Since < 0.1) { R->GaitSprint = Character.GetSpnMovement()->GetEffectiveGait() == ESpnGait::Sprint ? 1.f : 0.f; const FVector Local = Character.GetActorQuat().UnrotateVector(Character.GetCharacterMovement()->GetCurrentAcceleration().GetSafeNormal2D()); R->LocalAngle = FMath::RadiansToDegrees(FMath::Atan2(Local.Y, Local.X)); }
					if (Since < 0.8) { R->MinSpeed = FMath::Min(R->MinSpeed, Speed); }
					if (R->TimeToRecover < 0.0 && Since > 0.2 && Speed >= 0.95f * R->Before) { R->TimeToRecover = static_cast<float>(Since); }
					R->Final = Speed;
					return Since > 1.5;
				},
				[Case, R, Keys, Shared, this](ASupernaturalProtCharacter& Character, bool bTimedOut)
				{
					Keys->ReleaseAll(Character);
					R->bValid = !bTimedOut;
					Shared->Results.FindOrAdd(Case.Name).Add(*R);
				}));
			ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.1f));
		}
	}

	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, Shared, Cases]
	{
		ASupernaturalProtCharacter* Character = FindCharacter();
		auto Avg = [&](const FString& Name, float FTurnResult::*Member)
		{
			double Sum = 0; int32 N = 0;
			for (const FTurnResult& R : Shared->Results.FindRef(Name)) { if (R.bValid) { Sum += R.*Member; ++N; } }
			return N > 0 ? static_cast<float>(Sum / N) : -1.f;
		};
		for (const FCase& Case : Cases)
		{
			const float Before = Avg(Case.Name, &FTurnResult::Before), Min = Avg(Case.Name, &FTurnResult::MinSpeed), Rec = Avg(Case.Name, &FTurnResult::TimeToRecover);
			const float KeptSprint = Avg(Case.Name, &FTurnResult::GaitSprint), Angle = Avg(Case.Name, &FTurnResult::LocalAngle);
			const bool bSprintCase = Case.bSprint;
			const bool bBack = FString(Case.Name).Contains(TEXT("180"));
			// Sprint 90 degrees: the project requirement of SpnGame.Movement.SprintTurn (no dip below 90%, sprint kept). Jog 90 degrees (placeholder
			// criterion): never below 50% and back at 95% within 1.5 s. 180 degrees: measured only.
			const bool bOk = Before > 0 && (bBack || (bSprintCase ? (Min >= 0.9f * Before && KeptSprint > 0.5f) : (Min >= 0.5f * Before && Rec >= 0.f && Rec <= 1.5f)));
			SpnQa::Check(*this, Character, TEXT("Turns"), Case.Name, Before < 0 ? SpnQa::EStatus::Blocked : (bBack ? SpnQa::EStatus::Pass : (bOk ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail)),
				FString::Printf(TEXT("3 trials: before %.0f, lowest during the turn %.0f cm/s (%.0f%%), sprint kept after 30 ms %.0f%%, local input angle %.0f deg, back at 95%% after %.2f s%s"),
					Before, Min, Before > 0 ? 100.f * Min / Before : 0.f, KeptSprint * 100.f, Angle, Rec, bBack ? TEXT(" (180 degrees: measured, brake-and-reverse expected)") : TEXT("")));
		}
		const float SL = Avg(TEXT("sprint_left_90"), &FTurnResult::MinSpeed), SR = Avg(TEXT("sprint_right_90"), &FTurnResult::MinSpeed);
		const float GL = Avg(TEXT("sprint_left_90"), &FTurnResult::GaitSprint), GR = Avg(TEXT("sprint_right_90"), &FTurnResult::GaitSprint);
		const bool bSym = SL > 0 && SR > 0 && FMath::Abs(SL - SR) <= 0.05f * FMath::Max(SL, SR) && FMath::Abs(GL - GR) < 0.01f;
		SpnQa::Check(*this, Character, TEXT("Turns"), TEXT("sprint_left_right_symmetry"), bSym ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail,
			FString::Printf(TEXT("lowest speed left %.0f vs right %.0f cm/s; sprint kept left %.0f%% vs right %.0f%% (limit: 5%% speed difference, same gait)"), SL, SR, GL * 100.f, GR * 100.f));
		return true;
	}));
	return true;
}

// ------------------------------------------------------------------------------------------------------------------ Jump

namespace
{
	struct FJumpCase
	{
		const TCHAR* Name;
		ESpnGait Gait;
		bool bSprint;
		float Height;     // obstacle height, 0 = none
		float Gap;        // gap between the capsule and the obstacle face at take-off, cm
		bool bWall;       // obstacle is a wall the jump must not get over
		bool bStanding;
	};

	struct FJumpRun
	{
		SpnQa::FHeldKeys Keys;
		AStaticMeshActor* Block = nullptr;
		float FrontY = 0.f, GroundZ = 0.f;
		double JumpAt = -1.0, SpaceUp = -1.0, Crossed = -1.0, Landed = -1.0, StartRun = 0.0;
		float TakeoffSpeed = 0.f, MinAirSpeed = 1.0e9f, MaxZ = -1.0e9f, ZAtLand = 0.f, MaxY = -1.0e9f, SpeedAfterLand = 0.f, ZVelAtLand = 0.f, StartX = 0.f, DriftX = 0.f;
		bool bAir = false;
		FVector TakeoffPos = FVector::ZeroVector;
		float PrevZVel = 0.f;
	};
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnQaJumpTest, "SpnQA.Jump", QaFlags)

bool FSpnQaJumpTest::RunTest(const FString& Parameters)
{
	if (!OpenV2(*this)) { return false; }
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([] { SpnQa::Section(TEXT("Jump: standing, moving, sprint, ledges, wall, landing")); return true; }));

	const FJumpCase Cases[] = {
		{TEXT("jump_standing"), ESpnGait::Jog, false, 0, 0, false, true},
		{TEXT("jump_jog"), ESpnGait::Jog, false, 0, 0, false, false},
		{TEXT("jump_run"), ESpnGait::Run, false, 0, 0, false, false},
		{TEXT("jump_sprint"), ESpnGait::Jog, true, 0, 0, false, false},
		{TEXT("ledge_30_jog"), ESpnGait::Jog, false, 30, 125, false, false},
		{TEXT("ledge_40_jog"), ESpnGait::Jog, false, 40, 125, false, false},
		{TEXT("ledge_45_jog"), ESpnGait::Jog, false, 45, 125, false, false},
		{TEXT("ledge_50_run"), ESpnGait::Run, false, 50, 125, false, false},
		{TEXT("ledge_60_run"), ESpnGait::Run, false, 60, 125, false, false},
		{TEXT("ledge_100_sprint"), ESpnGait::Jog, true, 100, 125, true, false},
		{TEXT("late_jump_ledge_45_jog"), ESpnGait::Jog, false, 45, 10, false, false},
		{TEXT("late_jump_ledge_40_run"), ESpnGait::Run, false, 40, 10, false, false},
		{TEXT("wall_150_collision_jog"), ESpnGait::Jog, false, 150, 80, true, false},
	};

	for (const FJumpCase& Case : Cases)
	{
		const TSharedRef<FJumpRun> Run = MakeShared<FJumpRun>();
		ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(*this, FVector(LaneX, LaneStartY, StandZ), 90.f, 22.f,
			[Run, Case](ASupernaturalProtCharacter& Character)
			{
				SpnQa::Section(FString::Printf(TEXT("Jump: %s"), Case.Name));
				Character.GetSpnMovement()->SetBaseGait(Case.Gait);
				Run->GroundZ = StandZ;
				Run->StartX = LaneX;
				if (Case.Height > 0.f)
				{
					Run->FrontY = LaneStartY + 2400.f;
					Run->Block = SpawnBlock(*Character.GetWorld(), FVector(LaneX, Run->FrontY + 50.f, Case.Height / 2.f), FVector(400.f, 100.f, Case.Height));
				}
			},
			[Run, Case](ASupernaturalProtCharacter& Character, float, double Elapsed)
			{
				const FVector Loc = Character.GetActorLocation();
				const float Speed = HorizontalSpeed(Character);
				const bool bGround = Character.GetCharacterMovement()->IsMovingOnGround();
				Run->MaxY = FMath::Max(Run->MaxY, Loc.Y);
				if (Run->JumpAt < 0.0)
				{
					Run->Keys.Set(Character, EKeys::W, !Case.bStanding);
					Run->Keys.Set(Character, EKeys::LeftShift, Case.bSprint && !Case.bStanding);
					const bool bAtSpeed = Case.bStanding ? Elapsed > 1.0 : Speed >= 0.97f * SpnMovement::GaitSpeed(Character.GetSpnMovement()->Settings, Case.bSprint ? ESpnGait::Sprint : Case.Gait);
					const bool bTrigger = Case.Height > 0.f ? (Run->FrontY - (Loc.Y + CapsuleRadius)) <= Case.Gap : (bAtSpeed && Elapsed > 2.5);
					if (Elapsed > 1.0 && (Case.bStanding || bTrigger) && bGround)
					{
						Run->JumpAt = Elapsed;
						Run->TakeoffSpeed = Speed;
						Run->TakeoffPos = Loc;
						Run->bAir = true;
						Run->Keys.Set(Character, EKeys::SpaceBar, true);
					}
					return Elapsed > 14.0; // never reached the trigger
				}
				if (Run->SpaceUp < 0.0 && Elapsed - Run->JumpAt > 0.1) { Run->Keys.Set(Character, EKeys::SpaceBar, false); Run->SpaceUp = Elapsed; }
				if (!bGround || Elapsed - Run->JumpAt < 0.15)
				{
					Run->MinAirSpeed = FMath::Min(Run->MinAirSpeed, Speed);
					Run->MaxZ = FMath::Max(Run->MaxZ, Loc.Z);
					Run->PrevZVel = Character.GetVelocity().Z;
				}
				if (Case.Height > 0.f && Run->Crossed < 0.0 && Loc.Y > Run->FrontY + 100.f + CapsuleRadius)
				{
					Run->Crossed = Elapsed - Run->JumpAt;
				}
				if (bGround && Elapsed - Run->JumpAt > 0.2 && Run->Landed < 0.0)
				{
					Run->Landed = Elapsed - Run->JumpAt;
					Run->ZAtLand = Loc.Z;
					Run->ZVelAtLand = Run->PrevZVel;
					Run->DriftX = Loc.X - Run->StartX;
				}
				if (Run->Landed >= 0.0)
				{
					Run->SpeedAfterLand = Speed;
					if (Elapsed - Run->JumpAt - Run->Landed > 0.6 || Run->Crossed >= 0.0) { return Elapsed - Run->JumpAt - Run->Landed > 0.6 || Case.Height <= 0.f; }
				}
				return Elapsed - Run->JumpAt > 6.0;
			},
			[this, Run, Case](ASupernaturalProtCharacter& Character, bool bTimedOut)
			{
				Run->Keys.ReleaseAll(Character);
				const FString Id = Case.Name;
				const FSpnMovementSettings& S = Character.GetSpnMovement()->Settings;
				if (Run->Block != nullptr) { Run->Block->Destroy(); }
				if (Run->JumpAt < 0.0)
				{
					SpnQa::Check(*this, &Character, TEXT("Jump"), Id, SpnQa::EStatus::Blocked, TEXT("never reached the take-off point (stuck or too slow)"));
					return;
				}
				const float Apex = Run->MaxZ - Run->TakeoffPos.Z;
				const float Retained = Run->TakeoffSpeed > 20.f ? 100.f * Run->MinAirSpeed / Run->TakeoffSpeed : 100.f;
				const FString Facts = FString::Printf(TEXT("take-off speed %.0f cm/s, lowest speed up to landing %.0f (%.0f%%), apex %.0f cm (spec %.0f), landed after %.2f s, Z at landing %.0f (ground %.0f), speed 0.6 s after landing %.0f, crossed %s"),
					Run->TakeoffSpeed, Run->MinAirSpeed, Retained, Apex, S.JumpHeight, Run->Landed, Run->ZAtLand, Run->GroundZ, Run->SpeedAfterLand, Run->Crossed >= 0.0 ? *FString::Printf(TEXT("after %.2f s"), Run->Crossed) : TEXT("no"));
				SpnQa::EStatus Status = SpnQa::EStatus::Pass;
				FString Why;
				if (Run->Landed < 0.0) { Status = SpnQa::EStatus::Fail; Why = TEXT("never landed within 6 s"); }
				else if (FMath::Abs(Run->ZAtLand - Run->GroundZ) > 8.f && Case.Height <= 0.f) { Status = SpnQa::EStatus::Fail; Why = TEXT("landing height differs from the ground (fell through or hovering)"); }
				else if (Case.Height <= 0.f)
				{
					if (FMath::Abs(Apex - S.JumpHeight) > 7.f) { Status = SpnQa::EStatus::Fail; Why = TEXT("apex differs from the configured jump height"); }
					else if (Retained < 90.f && !Case.bStanding) { Status = SpnQa::EStatus::Fail; Why = TEXT("horizontal speed lost in the air on open ground"); }
					else if (Case.bStanding && FMath::Abs(Run->DriftX) > 15.f) { Status = SpnQa::EStatus::Fail; Why = TEXT("standing jump drifted sideways"); }
				}
				else if (Case.bWall)
				{
					if (Run->Crossed >= 0.0) { Status = SpnQa::EStatus::Fail; Why = FString::Printf(TEXT("got over a %.0f cm obstacle (higher than the jump apex)"), Case.Height); }
					else if (Run->MaxY > Run->FrontY + 10.f) { Status = SpnQa::EStatus::Fail; Why = TEXT("capsule penetrated the obstacle face"); }
				}
				else if (Case.Gap < 50.f)
				{
					// The reported problem: the jump right before a ledge about as high as the apex goes almost straight up.
					if (Run->Crossed < 0.0 || Run->Crossed > 1.0) { Status = SpnQa::EStatus::Fail; Why = TEXT("near-vertical climb: crossing the ledge takes longer than 1 s (known issue, M2A2 report section 5, decision pending)"); }
					else if (Retained < 50.f) { Status = SpnQa::EStatus::Fail; Why = TEXT("horizontal speed collapses below 50% at the ledge face (known issue, M2A2 report section 5, decision pending)"); }
				}
				else if (Case.Height <= S.MaxStepHeight + 20.f)
				{
					if (Run->Crossed < 0.0) { Status = SpnQa::EStatus::Fail; Why = TEXT("could not get over a ledge below the jump apex"); }
					else if (Retained < 50.f) { Status = SpnQa::EStatus::Fail; Why = TEXT("speed collapses below 50% during the jump over the ledge"); }
				}
				else if (Case.Height > S.JumpHeight + 3.f)
				{
					if (Run->Crossed >= 0.0) { Status = SpnQa::EStatus::Fail; Why = FString::Printf(TEXT("got over a %.0f cm ledge although the apex is %.0f cm (known, SpnGame.Movement.Obstacles fails the same way)"), Case.Height, S.JumpHeight); }
				}
				SpnQa::Check(*this, &Character, TEXT("Jump"), Id, Status, (Why.IsEmpty() ? FString() : Why + TEXT(". ")) + Facts);
			}));
		ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.3f));
	}
	return true;
}

#endif
