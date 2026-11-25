#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HAL/FileManager.h"
#include "Misc/AutomationTest.h"
#include "Misc/Guid.h"
#include "Misc/Paths.h"
#include "Spn/SpnNpc.h"
#include "SpnNpcReaction.h"
#include "SpnSaveSubsystem.h"
#include "SupernaturalProtCharacter.h"

#if WITH_DEV_AUTOMATION_TESTS

// Player footstep noises (NightBuild06, Phase F; closes NightBuild05 KNOWN_ISSUES 6): the player character sends Footstep noises while it
// moves on the ground, louder the faster it goes (Core FootstepNoiseFor). They are never suspicious on their own: a calm NPC hears and ignores
// them (nothing remembered, no reaction), a WARY NPC (after a suspicious noise) does not ignore them. Headless game world, no map.

namespace SpnFootstepTest
{
	constexpr EAutomationTestFlags Flags = EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter;

	struct FStepWorld
	{
		UGameInstance* GameInstance = nullptr;
		UWorld* World = nullptr;
		TArray<AActor*> Spawned;
		FString SaveRoot;

		FStepWorld()
		{
			GameInstance = NewObject<UGameInstance>(GEngine);
			GameInstance->AddToRoot();
			GameInstance->InitializeStandalone();
			World = GameInstance->GetWorld();
			SaveRoot = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir() / TEXT("SpnTestTemp") / FGuid::NewGuid().ToString(EGuidFormats::Digits));
			GameInstance->GetSubsystem<USpnSaveSubsystem>()->SetSaveDirectory(SaveRoot);
		}

		~FStepWorld()
		{
			for (AActor* Actor : Spawned) { if (IsValid(Actor)) { Actor->Destroy(); } }
			GameInstance->Shutdown();
			GEngine->DestroyWorldContext(World);
			World->DestroyWorld(/*bInformEngineOfWorld*/ false);
			GameInstance->RemoveFromRoot();
			IFileManager::Get().DeleteDirectory(*SaveRoot, false, true);
		}

		USpnNoiseSubsystem& Noise() const { return *GameInstance->GetSubsystem<USpnNoiseSubsystem>(); }

		ASupernaturalProtCharacter* Player(const FVector& Location)
		{
			UClass* Class = LoadClass<ASupernaturalProtCharacter>(nullptr, TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter_C"));
			if (Class == nullptr) { return nullptr; }
			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			ASupernaturalProtCharacter* C = World->SpawnActor<ASupernaturalProtCharacter>(Class, FTransform(Location), Params);
			if (C != nullptr) { Spawned.Add(C); }
			return C;
		}

		ASpnNpc* Npc(int64 Id, const FVector& Location)
		{
			FActorSpawnParameters Params;
			Params.bDeferConstruction = true;
			ASpnNpc* N = World->SpawnActor<ASpnNpc>(ASpnNpc::StaticClass(), FTransform(Location), Params);
			N->NpcId = Id;
			N->DisplayName = FString::Printf(TEXT("Listener%lld"), Id);
			N->FinishSpawning(FTransform(Location));
			N->DispatchBeginPlay();
			Spawned.Add(N);
			return N;
		}
	};

	/** Moves the player's velocity (no physics step) and runs the footstep emission for Seconds at 60 fps. */
	void Walk(ASupernaturalProtCharacter& Player, float SpeedCmS, float Seconds)
	{
		UCharacterMovementComponent* Move = Player.GetCharacterMovement();
		Move->SetMovementMode(MOVE_Walking);
		Move->Velocity = FVector(SpeedCmS, 0.f, 0.f);
		for (float T = 0.f; T < Seconds; T += 1.f / 60.f) { Player.TickFootstepNoise(1.f / 60.f); }
		Move->Velocity = FVector::ZeroVector;
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnFootstepEmitTest, "SpnGame.Npc.PlayerFootstepsAreGameplayNoises", SpnFootstepTest::Flags)
bool FSpnFootstepEmitTest::RunTest(const FString&)
{
	using namespace SpnFootstepTest;
	FStepWorld W;
	ASupernaturalProtCharacter* Player = W.Player(FVector(0.f, 0.f, 100.f));
	if (!TestNotNull(TEXT("player character spawned"), Player)) { return false; }

	const uint64 Before = W.Noise().GetEmittedCount();
	Walk(*Player, 0.f, 1.f);
	TestEqual(TEXT("standing: no footstep noise"), W.Noise().GetEmittedCount(), Before);

	Walk(*Player, 180.f, 2.f);   // 360 cm walked: two noises at 150 cm each
	const uint64 AfterWalk = W.Noise().GetEmittedCount();
	TestTrue(*FString::Printf(TEXT("walking 2 s: %llu footstep noise(s), one per %.0f cm"), AfterWalk - Before, Player->FootstepNoiseEveryCm), AfterWalk - Before == 2);
	const FSpnNoise WalkNoise = W.Noise().GetLastNoise();
	TestTrue(TEXT("the noise is a footstep"), WalkNoise.Kind == ESpnNoiseKind::Footstep);
	TestFalse(TEXT("a walking step is not suspicious"), SpnNpcSense::IsSuspicious(WalkNoise));
	TestTrue(*FString::Printf(TEXT("it is made at the feet (z %.1f)"), WalkNoise.LocationCm.Z),
		FMath::IsNearlyEqual(static_cast<float>(WalkNoise.LocationCm.Z), static_cast<float>(Player->GetActorLocation().Z) - Player->GetBodyCollision().HalfHeight, 0.1f));

	Walk(*Player, 800.f, 1.f);
	const FSpnNoise SprintNoise = W.Noise().GetLastNoise();
	TestTrue(*FString::Printf(TEXT("sprinting is louder: range %.0f > %.0f cm, strength %.2f > %.2f"), SprintNoise.RangeCm, WalkNoise.RangeCm, SprintNoise.Strength, WalkNoise.Strength),
		SprintNoise.RangeCm > WalkNoise.RangeCm && SprintNoise.Strength > WalkNoise.Strength);
	TestFalse(TEXT("a sprinting step is not suspicious either"), SpnNpcSense::IsSuspicious(SprintNoise));
	TestTrue(TEXT("sprinting sends more noises per second than walking"), W.Noise().GetEmittedCount() - AfterWalk >= 5);

	Player->bIsCrouched = true;
	Walk(*Player, 350.f, 1.f);
	const FSpnNoise Sneak = W.Noise().GetLastNoise();
	Player->bIsCrouched = false;
	Walk(*Player, 350.f, 1.f);
	const FSpnNoise Jog = W.Noise().GetLastNoise();
	TestTrue(*FString::Printf(TEXT("crouched steps are quieter (%.0f < %.0f cm)"), Sneak.RangeCm, Jog.RangeCm), Sneak.RangeCm < Jog.RangeCm);

	Player->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	const uint64 BeforeAir = W.Noise().GetEmittedCount();
	Player->GetCharacterMovement()->Velocity = FVector(600.f, 0.f, 0.f);
	for (int32 i = 0; i < 60; ++i) { Player->TickFootstepNoise(1.f / 60.f); }
	TestEqual(TEXT("in the air: no footsteps"), W.Noise().GetEmittedCount(), BeforeAir);

	Player->bEmitFootstepNoise = false;
	const uint64 BeforeOff = W.Noise().GetEmittedCount();
	Walk(*Player, 800.f, 1.f);
	TestEqual(TEXT("switched off: no footsteps"), W.Noise().GetEmittedCount(), BeforeOff);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnFootstepListenerTest, "SpnGame.Npc.FootstepsMatterOnlyToAWaryNpc", SpnFootstepTest::Flags)
bool FSpnFootstepListenerTest::RunTest(const FString&)
{
	using namespace SpnFootstepTest;
	FStepWorld W;
	ASupernaturalProtCharacter* Player = W.Player(FVector(0.f, 0.f, 100.f));
	if (!TestNotNull(TEXT("player character spawned"), Player)) { return false; }
	ASpnNpc* Near = W.Npc(9701, FVector(300.f, 0.f, 100.f));     // 3 m: hears a walk
	ASpnNpc* Far = W.Npc(9702, FVector(0.f, 800.f, 100.f));      // 8 m: hears only a sprint

	// calm NPCs: the walk is heard by the near one and ignored, the far one does not hear it
	Walk(*Player, 180.f, 2.f);
	TestTrue(*FString::Printf(TEXT("near NPC heard the walk (%d)"), Near->GetNoisesHeard()), Near->GetNoisesHeard() >= 1);
	TestEqual(TEXT("near, calm: every heard step is ignored"), Near->GetNoisesIgnored(), Near->GetNoisesHeard());
	TestEqual(TEXT("far NPC did not hear the walk"), Far->GetNoisesHeard(), 0);
	TestTrue(TEXT("far NPC counted the steps as not heard"), Far->GetNoisesNotHeard() >= 1);
	TestFalse(TEXT("footsteps do not make a calm NPC wary"), Near->IsWary());
	TestTrue(TEXT("and leave it in its routine"), Near->GetReactionState() == ESpnReactionState::Idle);

	Walk(*Player, 800.f, 1.f);
	TestTrue(*FString::Printf(TEXT("the far NPC hears a sprint (%d)"), Far->GetNoisesHeard()), Far->GetNoisesHeard() >= 1);
	TestEqual(TEXT("far, calm: the sprint steps are ignored too"), Far->GetNoisesIgnored(), Far->GetNoisesHeard());
	TestFalse(TEXT("a sprint does not make a calm NPC wary"), Far->IsWary());

	// something suspicious makes the near NPC wary; from then on it does not ignore the steps
	FSpnNoise Crash;
	Crash.Kind = ESpnNoiseKind::Impact;
	Crash.LocationCm = FVector(500.f, 0.f, 0.f);
	Crash.RangeCm = 1500.f;
	Crash.Strength = 0.9f;
	Crash.SourceId = TEXT("test");
	W.Noise().EmitNoise(Crash);
	TestTrue(TEXT("after a suspicious noise the near NPC is wary"), Near->IsWary());
	const int32 IgnoredBefore = Near->GetNoisesIgnored(), HeardBefore = Near->GetNoisesHeard();
	Walk(*Player, 180.f, 2.f);
	TestTrue(*FString::Printf(TEXT("the wary NPC hears the steps (%d new)"), Near->GetNoisesHeard() - HeardBefore), Near->GetNoisesHeard() > HeardBefore);
	TestEqual(TEXT("the wary NPC does not ignore them"), Near->GetNoisesIgnored(), IgnoredBefore);
	return true;
}

#endif
