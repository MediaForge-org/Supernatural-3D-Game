#pragma once

#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Misc/AutomationTest.h"
#include "Misc/PackageName.h"
#include "Spn/SpnCharacterMovementComponent.h"
#include "SupernaturalProtCharacter.h"
#include "Tests/AutomationCommon.h"

#if WITH_DEV_AUTOMATION_TESTS

// Shared helpers of the SpnGame.Movement.* map tests (M2-A, Docs/Proposals/0011). Header-only so the tests stay independent.

namespace SpnMovementTest
{
	inline const TCHAR* MapPackage() { return TEXT("/Game/SpnTest/Lvl_SpnMovementTest"); }

	/** Values of Tools/spn_movement_map_layout.py (the Python layout test and SpnGame.Movement.MapLayout keep both in step). */
	constexpr float StandZ = 97.f;          // capsule half height 96 on a floor at Z = 0
	constexpr float MapHalfExtent = 15000.f;

	inline UWorld* FindPlayWorld()
	{
		for (const FWorldContext& Context : GEngine->GetWorldContexts())
		{
			if ((Context.WorldType == EWorldType::Game || Context.WorldType == EWorldType::PIE) && Context.World() != nullptr)
			{
				return Context.World();
			}
		}
		return nullptr;
	}

	inline ASupernaturalProtCharacter* FindCharacter()
	{
		const UWorld* World = FindPlayWorld();
		const APlayerController* Controller = World != nullptr ? World->GetFirstPlayerController() : nullptr;
		ASupernaturalProtCharacter* Character = Controller != nullptr ? Cast<ASupernaturalProtCharacter>(Controller->GetPawn()) : nullptr;
		return Character != nullptr && Character->GetSpnMovement() != nullptr ? Character : nullptr;
	}

	/** True (and an error on the test) when the generated map is missing, so a missing map fails loudly instead of passing. */
	inline bool RequireMap(FAutomationTestBase& Test)
	{
		if (!FPackageName::DoesPackageExist(MapPackage()))
		{
			Test.AddError(TEXT("/Game/SpnTest/Lvl_SpnMovementTest does not exist yet: generate it first (Docs/Setup/Windows-M2A-Handover.md, step 4)."));
			return false;
		}
		return true;
	}

	/**
	 * One scenario in the running map: teleports the character to Start facing Yaw, calls Begin once, then calls Step every
	 * frame with the frame time and the seconds since the start until it returns true or MaxSeconds pass, then calls Finish.
	 * The latent command waits for a possessed character itself.
	 */
	class FScenarioCommand : public IAutomationLatentCommand
	{
	public:
		using FBeginFn = TFunction<void(ASupernaturalProtCharacter&)>;
		using FStepFn = TFunction<bool(ASupernaturalProtCharacter&, float DeltaSeconds, double Elapsed)>;
		using FFinishFn = TFunction<void(ASupernaturalProtCharacter&, bool bTimedOut)>;

		FScenarioCommand(FAutomationTestBase& InTest, FVector InStart, float InYaw, float InMaxSeconds, FBeginFn InBegin, FStepFn InStep, FFinishFn InFinish)
			: Test(InTest), Start(InStart), Yaw(InYaw), MaxSeconds(InMaxSeconds), Begin(MoveTemp(InBegin)), Step(MoveTemp(InStep)), Finish(MoveTemp(InFinish))
		{
		}

		virtual bool Update() override
		{
			ASupernaturalProtCharacter* Character = FindCharacter();
			if (Character == nullptr)
			{
				if (++WaitFrames > 600)
				{
					Test.AddError(TEXT("no possessed ASupernaturalProtCharacter with a USpnCharacterMovementComponent after about 10 s in the map"));
					return true;
				}
				return false;
			}

			UWorld* World = Character->GetWorld();
			if (!bStarted)
			{
				bStarted = true;
				StartTime = World->GetTimeSeconds();
				Character->GetCharacterMovement()->StopMovementImmediately();
				Character->SetActorLocation(Start, false, nullptr, ETeleportType::TeleportPhysics);
				Character->GetController()->SetControlRotation(FRotator(0.f, Yaw, 0.f));
				Character->SetActorRotation(FRotator(0.f, Yaw, 0.f));
				if (Begin)
				{
					Begin(*Character);
				}
				return false;
			}

			const double Elapsed = World->GetTimeSeconds() - StartTime;
			const bool bDone = Step(*Character, World->GetDeltaSeconds(), Elapsed);
			if (bDone || Elapsed >= MaxSeconds)
			{
				if (Finish)
				{
					Finish(*Character, !bDone);
				}
				return true;
			}
			return false;
		}

	private:
		FAutomationTestBase& Test;
		FVector Start;
		float Yaw;
		float MaxSeconds;
		FBeginFn Begin;
		FStepFn Step;
		FFinishFn Finish;
		bool bStarted = false;
		double StartTime = 0.0;
		int32 WaitFrames = 0;
	};

	inline float HorizontalSpeed(const ASupernaturalProtCharacter& Character)
	{
		return Character.GetVelocity().Size2D();
	}

	/** Returns the movement component or null; tests treat null as a failure of their own. */
	inline USpnCharacterMovementComponent* Movement(ASupernaturalProtCharacter& Character)
	{
		return Character.GetSpnMovement();
	}
}

#endif
