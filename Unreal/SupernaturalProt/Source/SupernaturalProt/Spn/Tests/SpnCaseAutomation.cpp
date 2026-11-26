#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/WorldSettings.h"
#include "HAL/FileManager.h"
#include "Misc/AutomationTest.h"
#include "Misc/Guid.h"
#include "Misc/Paths.h"
#include "Spn/Case/SpnCaseCreature.h"
#include "Spn/Case/SpnCaseObjects.h"
#include "Spn/Case/SpnCaseSubsystem.h"
#include "SpnNpcReaction.h"
#include "SpnSaveSubsystem.h"
#include "SupernaturalProtCharacter.h"

#if WITH_DEV_AUTOMATION_TESTS

// First case, night build A (NightBuild07, Docs/Proposals/0019): the blind creature, the bell, throwables, death and respawn, the save block.
// Headless game world with a floor, ticked frame by frame; no map, no navigation mesh (the creature steers straight, as logged).

namespace SpnCaseTest
{
	constexpr EAutomationTestFlags Flags = EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter;

	struct FCaseWorld
	{
		UGameInstance* GameInstance = nullptr;
		UWorld* World = nullptr;
		TArray<AActor*> Spawned;
		FString SaveRoot;

		FCaseWorld()
		{
			GameInstance = NewObject<UGameInstance>(GEngine);
			GameInstance->AddToRoot();
			GameInstance->InitializeStandalone();
			World = GameInstance->GetWorld();
			SaveRoot = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir() / TEXT("SpnTestTemp") / FGuid::NewGuid().ToString(EGuidFormats::Digits));
			GameInstance->GetSubsystem<USpnSaveSubsystem>()->SetSaveDirectory(SaveRoot);
			World->InitializeActorsForPlay(FURL());
			World->GetWorldSettings()->NotifyBeginPlay();
			Floor();
		}

		~FCaseWorld()
		{
			World->BeginTearingDown();
			for (AActor* Actor : Spawned) { if (IsValid(Actor)) { Actor->Destroy(); } }
			World->EndPlay(EEndPlayReason::Quit);
			GameInstance->Shutdown();
			GEngine->DestroyWorldContext(World);
			World->DestroyWorld(false);
			GameInstance->RemoveFromRoot();
			IFileManager::Get().DeleteDirectory(*SaveRoot, false, true);
		}

		USpnCaseSubsystem& Case() const { return *World->GetSubsystem<USpnCaseSubsystem>(); }
		USpnNoiseSubsystem& Noise() const { return *GameInstance->GetSubsystem<USpnNoiseSubsystem>(); }

		void Floor()
		{
			AActor* Actor = World->SpawnActor<AActor>(AActor::StaticClass(), FTransform(FVector(0.f, 0.f, -15.f)));
			UStaticMeshComponent* Mesh = NewObject<UStaticMeshComponent>(Actor, TEXT("Mesh"));
			Actor->SetRootComponent(Mesh);
			Mesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube")));
			Mesh->SetRelativeScale3D(FVector(80.f, 80.f, 0.3f));
			Mesh->SetCollisionProfileName(TEXT("BlockAll"));
			Actor->SetActorLocation(FVector(0.f, 0.f, -15.f));
			Mesh->RegisterComponent();
			Spawned.Add(Actor);
		}

		ASupernaturalProtCharacter* Player(const FVector& Location)
		{
			UClass* Class = LoadClass<ASupernaturalProtCharacter>(nullptr, TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter_C"));
			if (Class == nullptr) { return nullptr; }
			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			ASupernaturalProtCharacter* C = World->SpawnActor<ASupernaturalProtCharacter>(Class, FTransform(Location), Params);
			if (C == nullptr) { return nullptr; }
			C->GetCharacterMovement()->bRunPhysicsWithNoController = true;
			C->GetCharacterMovement()->SetDefaultMovementMode();
			C->bEmitFootstepNoise = false; // the tests send the noises they mean
			Spawned.Add(C);
			Case().SetPlayerOverride(C);
			return C;
		}

		ASpnCreature* Creature(const FVector& Location)
		{
			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			ASpnCreature* C = World->SpawnActor<ASpnCreature>(ASpnCreature::StaticClass(), FTransform(Location), Params);
			if (C != nullptr) { Spawned.Add(C); }
			return C;
		}

		void Run(float Seconds)
		{
			for (float T = 0.f; T < Seconds; T += 1.f / 60.f)
			{
				++GFrameCounter;
				World->Tick(LEVELTICK_All, 1.f / 60.f);
			}
		}

		void Emit(const FVector& At, float RangeCm, float Strength, ESpnNoiseKind Kind)
		{
			FSpnNoise N;
			N.LocationCm = At;
			N.RangeCm = RangeCm;
			N.Strength = Strength;
			N.Kind = Kind;
			N.SourceId = TEXT("test");
			Noise().EmitNoise(N);
		}
	};
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnCaseHearingTest, "SpnGame.Case.CreatureHearsASprintButNotASneak", SpnCaseTest::Flags)
bool FSpnCaseHearingTest::RunTest(const FString&)
{
	using namespace SpnCaseTest;
	FCaseWorld W;
	ASupernaturalProtCharacter* Player = W.Player(FVector(0.f, 0.f, 100.f));
	ASpnCreature* Creature = W.Creature(FVector(1000.f, 0.f, 110.f)); // 10 m away
	if (!TestNotNull(TEXT("player"), Player) || !TestNotNull(TEXT("creature"), Creature)) { return false; }
	W.Run(0.2f);
	TestTrue(TEXT("it starts on its patrol"), Creature->GetCreatureState() == ESpnCreatureState::Patrol);

	float Range = 0.f, Strength = 0.f;
	TestTrue(TEXT("a crouched walk makes a footstep"), SpnNpcSense::FootstepNoise(180.f, /*crouched*/ true, Range, Strength));
	W.Emit(FVector(0.f, 0.f, 10.f), Range, Strength, ESpnNoiseKind::Footstep);
	W.Run(0.2f);
	TestEqual(TEXT("a sneak 10 m away is not heard"), Creature->GetNoisesHeard(), 0);
	TestTrue(TEXT("and changes nothing"), Creature->GetCreatureState() == ESpnCreatureState::Patrol);

	TestTrue(TEXT("a sprint makes a footstep"), SpnNpcSense::FootstepNoise(800.f, false, Range, Strength));
	W.Emit(FVector(0.f, 0.f, 10.f), Range, Strength, ESpnNoiseKind::Footstep);
	W.Run(0.2f);
	TestEqual(TEXT("a sprint 10 m away is heard"), Creature->GetNoisesHeard(), 1);
	TestTrue(*FString::Printf(TEXT("it investigates the steps (state %s)"), LexToString(Creature->GetCreatureState())), Creature->GetCreatureState() == ESpnCreatureState::Investigate);
	TestTrue(TEXT("towards where they were"), FVector::Dist2D(Creature->GetGoalCm(), FVector::ZeroVector) < 50.f);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnCaseShotTest, "SpnGame.Case.ALoudNoiseStartsAPursuitAfterTheDifficultysHeadStart", SpnCaseTest::Flags)
bool FSpnCaseShotTest::RunTest(const FString&)
{
	using namespace SpnCaseTest;
	FCaseWorld W;
	ASupernaturalProtCharacter* Player = W.Player(FVector(-3000.f, 0.f, 100.f)); // far: no contact
	ASpnCreature* Creature = W.Creature(FVector(1500.f, 0.f, 110.f));
	if (!TestNotNull(TEXT("player"), Player) || !TestNotNull(TEXT("creature"), Creature)) { return false; }
	TestTrue(TEXT("standard test profile"), W.Case().SetDifficulty(TEXT("test_standard")));
	const float Delay = W.Case().GetDifficulty().CreatureReactionDelay;
	W.Run(0.1f);

	W.Emit(FVector(0.f, 0.f, 10.f), 5000.f, 1.f, ESpnNoiseKind::Impact); // a shot-like noise 15 m away
	W.Run(0.2f);
	TestTrue(TEXT("first it listens: the player's head start"), Creature->IsListening());
	TestTrue(TEXT("still not hunting"), Creature->GetCreatureState() != ESpnCreatureState::Chase);
	W.Run(Delay + 0.2f);
	TestTrue(*FString::Printf(TEXT("after %.1f s it hunts (state %s)"), Delay, LexToString(Creature->GetCreatureState())), Creature->GetCreatureState() == ESpnCreatureState::Chase);
	TestTrue(TEXT("the noise's place"), FVector::Dist2D(Creature->GetGoalCm(), FVector::ZeroVector) < 50.f);

	TestTrue(TEXT("hardest test profile"), W.Case().SetDifficulty(TEXT("test_hardest")));
	TestTrue(TEXT("a new profile starts the creature from its patrol"), Creature->GetCreatureState() == ESpnCreatureState::Patrol);
	W.Emit(FVector(0.f, 0.f, 10.f), 5000.f, 1.f, ESpnNoiseKind::Impact);
	W.Run(0.1f);
	TestTrue(TEXT("on the hardest test profile there is no head start"), Creature->GetCreatureState() == ESpnCreatureState::Chase);
	TestFalse(TEXT("an unknown profile id is refused"), W.Case().SetDifficulty(TEXT("does_not_exist")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnCaseBellTest, "SpnGame.Case.TheBellStunsOnlyCreaturesInRange", SpnCaseTest::Flags)
bool FSpnCaseBellTest::RunTest(const FString&)
{
	using namespace SpnCaseTest;
	FCaseWorld W;
	ASupernaturalProtCharacter* Player = W.Player(FVector(0.f, -300.f, 100.f));
	ASpnCreature* Near = W.Creature(FVector(500.f, 0.f, 110.f));
	ASpnCreature* Far = W.Creature(FVector(3000.f, 0.f, 110.f));
	ASpnBell* Bell = W.World->SpawnActor<ASpnBell>(ASpnBell::StaticClass(), FTransform(FVector(0.f, 0.f, 250.f)));
	if (!TestNotNull(TEXT("player"), Player) || !TestNotNull(TEXT("creatures"), Far) || !TestNotNull(TEXT("bell"), Bell)) { return false; }
	W.Spawned.Add(Bell);
	W.Run(0.1f);
	TestTrue(TEXT("the bell offers a prompt"), !Bell->SpnPromptText(Player).IsEmpty());
	const FSpnInteractStatus Rung = Bell->SpnUse(Player);
	TestTrue(TEXT("ringing changes something"), Rung.Result == ESpnInteractResult::Changed);
	TestEqual(TEXT("one creature stunned"), Bell->GetLastStunned(), 1);
	TestTrue(TEXT("the near one (5 m) is stunned"), Near->GetCreatureState() == ESpnCreatureState::Stunned);
	TestTrue(TEXT("the far one (30 m) is not"), Far->GetCreatureState() != ESpnCreatureState::Stunned);
	TestTrue(TEXT("ringing again at once does nothing (cooldown)"), Bell->SpnUse(Player).Result == ESpnInteractResult::Unchanged);
	W.Run(9.f);
	TestTrue(*FString::Printf(TEXT("after the stun it searches (state %s)"), LexToString(Near->GetCreatureState())), Near->GetCreatureState() == ESpnCreatureState::Search);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnCaseDeathTest, "SpnGame.Case.ACatchKillsAndTheRespawnUsesTheDifficultysHealth", SpnCaseTest::Flags)
bool FSpnCaseDeathTest::RunTest(const FString&)
{
	using namespace SpnCaseTest;
	FCaseWorld W;
	ASupernaturalProtCharacter* Player = W.Player(FVector(0.f, 0.f, 100.f));
	ASpnCreature* Creature = W.Creature(FVector(140.f, 0.f, 110.f)); // inside the contact range
	if (!TestNotNull(TEXT("player"), Player) || !TestNotNull(TEXT("creature"), Creature)) { return false; }
	TestTrue(TEXT("standard test profile"), W.Case().SetDifficulty(TEXT("test_standard")));
	const FVector Start = Player->GetActorLocation();
	TestEqual(TEXT("full health at the start"), W.Case().GetHealth(), W.Case().GetMaxHealth());

	int32 Deaths = 0;
	W.Case().OnPlayerDied.AddLambda([&Deaths](const FString&) { ++Deaths; });
	W.Run(4.f);
	TestTrue(*FString::Printf(TEXT("the creature caught the player (catches %d, state %s)"), Creature->GetCatches(), LexToString(Creature->GetCreatureState())), W.Case().IsPlayerDead());
	TestEqual(TEXT("exactly one death event"), Deaths, 1);
	TestEqual(TEXT("the cause"), W.Case().GetLastDeathCause(), FString(TEXT("creature")));
	TestEqual(TEXT("health is 0"), W.Case().GetHealth(), 0.f);
	TestTrue(TEXT("the level start was noted"), W.Case().HasLevelStart());

	Player->SetActorLocation(FVector(600.f, 600.f, 100.f)); // wherever the body lies
	const FVector Frozen = Creature->GetActorLocation();
	W.Run(1.f);
	TestTrue(TEXT("while the player is dead the creature does not move"), FVector::Dist2D(Frozen, Creature->GetActorLocation()) < 5.f);
	TestEqual(TEXT("a dead player is not killed twice"), Deaths, 1);

	TestTrue(TEXT("respawn"), W.Case().Respawn());
	TestFalse(TEXT("alive again"), W.Case().IsPlayerDead());
	TestTrue(*FString::Printf(TEXT("without a save: back at the level start (%.0f cm away)"), FVector::Dist2D(Player->GetActorLocation(), Start)), FVector::Dist2D(Player->GetActorLocation(), Start) < 5.f);
	TestTrue(TEXT("the creature is back at its spawn"), FVector::Dist2D(Creature->GetActorLocation(), Creature->GetSpawnCm()) < 5.f);
	TestTrue(TEXT("and on its patrol"), Creature->GetCreatureState() == ESpnCreatureState::Patrol);
	const float Expected = W.Case().GetMaxHealth() * W.Case().GetDifficulty().RespawnHealth;
	TestTrue(*FString::Printf(TEXT("respawn health %.0f = %.0f %% of %.0f (test_standard)"), W.Case().GetHealth(), W.Case().GetDifficulty().RespawnHealth * 100.f, W.Case().GetMaxHealth()),
		FMath::IsNearlyEqual(W.Case().GetHealth(), Expected, 0.01f));
	TestFalse(TEXT("respawning a living player does nothing"), W.Case().Respawn());

	// the hardest test profile: about 10 % (TOD-04)
	TestTrue(TEXT("hardest test profile"), W.Case().SetDifficulty(TEXT("test_hardest")));
	W.Case().DamagePlayer(10000.f, TEXT("test"));
	TestTrue(TEXT("dead again"), W.Case().IsPlayerDead());
	W.Case().Respawn();
	TestTrue(*FString::Printf(TEXT("hardest: respawn health %.0f (10 %%)"), W.Case().GetHealth()), FMath::IsNearlyEqual(W.Case().GetHealth(), W.Case().GetMaxHealth() * 0.1f, 0.01f));
	TestEqual(TEXT("two deaths counted"), W.Case().GetDeathCount(), 2);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnCaseSaveBlockTest, "SpnGame.Case.SavingIsRefusedWhileTheCreatureHunts", SpnCaseTest::Flags)
bool FSpnCaseSaveBlockTest::RunTest(const FString&)
{
	using namespace SpnCaseTest;
	FCaseWorld W;
	ASupernaturalProtCharacter* Player = W.Player(FVector(-3000.f, 0.f, 100.f));
	ASpnCreature* Creature = W.Creature(FVector(1500.f, 0.f, 110.f));
	if (!TestNotNull(TEXT("player"), Player) || !TestNotNull(TEXT("creature"), Creature)) { return false; }
	W.Case().SetDifficulty(TEXT("test_hardest")); // no head start
	W.Run(0.1f);
	TestFalse(TEXT("on patrol: saving allowed"), W.Case().IsSaveBlocked());
	W.Emit(FVector(0.f, 0.f, 10.f), 5000.f, 1.f, ESpnNoiseKind::Impact);
	W.Run(0.1f);
	TestTrue(TEXT("it hunts"), Creature->GetCreatureState() == ESpnCreatureState::Chase);
	TestTrue(TEXT("hunting: saving refused"), W.Case().IsSaveBlocked());
	TestFalse(TEXT("an autosave is refused too"), W.Case().RequestAutosave(TEXT("test_point")));
	Creature->StunByBell();
	TestFalse(TEXT("stunned: saving allowed again"), W.Case().IsSaveBlocked());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnCaseThrowTest, "SpnGame.Case.AThrownBottleIsALure", SpnCaseTest::Flags)
bool FSpnCaseThrowTest::RunTest(const FString&)
{
	using namespace SpnCaseTest;
	FCaseWorld W;
	ASupernaturalProtCharacter* Player = W.Player(FVector(0.f, 0.f, 100.f));
	ASpnCreature* Creature = W.Creature(FVector(-600.f, 0.f, 110.f)); // behind the player, 6 m
	ASpnThrowablePickup* Bottle = W.World->SpawnActor<ASpnThrowablePickup>(ASpnThrowablePickup::StaticClass(), FTransform(FVector(100.f, 100.f, 20.f)));
	if (!TestNotNull(TEXT("player"), Player) || !TestNotNull(TEXT("creature"), Creature) || !TestNotNull(TEXT("bottle"), Bottle)) { return false; }
	W.Case().SetDifficulty(TEXT("test_hardest"));
	W.Run(0.1f);

	TestTrue(TEXT("nothing to throw yet"), W.Case().Throw(Player) == nullptr);
	TestTrue(TEXT("the bottle is taken"), Bottle->SpnUse(Player).Result == ESpnInteractResult::Changed);
	TestEqual(TEXT("one carried"), W.Case().GetThrowables(), 1);
	const int32 Max = GetDefault<USpnCaseSettings>()->MaxThrowables;
	for (int32 I = 1; I < Max; ++I) { W.Case().AddThrowable(); }
	TestFalse(TEXT("the hands are full at the maximum"), W.Case().AddThrowable());

	// thrown sideways (+Y): the creature's straight way to the impact passes the player at about 5 m, not through the player
	Player->SetActorRotation(FRotator(0.f, 90.f, 0.f));
	ASpnThrownObject* Thrown = W.Case().Throw(Player);
	if (!TestNotNull(TEXT("thrown"), Thrown)) { return false; }
	TestEqual(TEXT("one fewer carried"), W.Case().GetThrowables(), Max - 1);
	const uint64 NoisesBefore = W.Noise().GetEmittedCount();
	W.Run(3.f);
	TestTrue(TEXT("it landed"), Thrown->HasLanded());
	TestTrue(TEXT("landing made a noise"), W.Noise().GetEmittedCount() > NoisesBefore);
	const FVector Impact = Thrown->GetImpactCm();
	TestTrue(*FString::Printf(TEXT("it flew away from the player (%.0f cm)"), Impact.Y), Impact.Y > 300.f);
	TestTrue(*FString::Printf(TEXT("the creature goes for the impact, not for the player (state %s, goal %s)"), LexToString(Creature->GetCreatureState()), *Creature->GetGoalCm().ToCompactString()),
		Creature->GetCreatureState() == ESpnCreatureState::Chase && FVector::Dist2D(Creature->GetGoalCm(), Impact) < 100.f);
	TestFalse(TEXT("the lure kept the player alive"), W.Case().IsPlayerDead());
	return true;
}

#endif
