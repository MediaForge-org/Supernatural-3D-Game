#include "Components/CapsuleComponent.h"
#include "CollisionQueryParams.h"
#include "EngineUtils.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Spn/SpnInteractionComponent.h"
#include "Spn/SpnTestDoor.h"
#include "Spn/Tests/SpnMovementTestSupport.h"
#include "Spn/Tests/SpnQaRecorder.h"
#include "Spn/Tests/SpnTestPlayReset.h"

#if WITH_DEV_AUTOMATION_TESTS

// SpnQA.Doors: a human-like door run per V2 district door. The character walks up to the CLOSED door with the W key, aims with the
// control rotation, presses the E KEY through the player controller (key -> Enhanced Input mapping -> Interact action -> DoInteract ->
// USpnInteractionComponent trace -> ASpnTestDoor::SpnUse), watches the door swing, walks through, tries to close it from inside and,
// if that is not possible, from outside, and finally bumps into the closed door. Nothing calls SpnUse or sets the door state directly.
// PLACEHOLDER test geometry and placeholder interaction values (range 250 cm, sweep radius 15 cm).

using namespace SpnMovementTest;

namespace
{
	FString MapV2() { return SpnQa::QaMapPath(); }

	struct FDoorSpec
	{
		const TCHAR* DoorId;
		const TCHAR* Building;
		FVector2D Out;     // 250 cm in front of the door, outside
		FVector2D Inside;  // 110 cm behind the door, inside
	};

	// Entrance points of the V2 district (same numbers as SpnMovementMapV2Automation.cpp BuildRoutes).
	const FDoorSpec DoorSpecs[] = {
		{TEXT("d_cottage1_door"), TEXT("Cottage1"), {-9800, -5440}, {-9290, -5440}},
		{TEXT("d_house1_door"), TEXT("House1"), {-9800, 8250}, {-9290, 8250}},
		{TEXT("d_cottage2_door"), TEXT("Cottage2"), {-9800, 12340}, {-9290, 12340}},
		{TEXT("d_storage_door"), TEXT("StorageHall"), {-9700, -12700}, {-9190, -12700}},
		{TEXT("d_workshop_door"), TEXT("Workshop"), {6300, -12200}, {6810, -12200}},
		{TEXT("d_courthouse_door"), TEXT("CourtHouse"), {4900, 11020}, {5410, 11020}},
		{TEXT("d_office_door"), TEXT("Office1"), {13800, 3500}, {13290, 3500}},
		{TEXT("d_cottage3_door"), TEXT("Cottage3"), {3700, 11160}, {3190, 11160}},
		{TEXT("mv_hut_door"), TEXT("TestHut"), {-5250, -9195}, {-4550, -9195}},
		// The V2 copy of this door swings outward (spn_district_layout.fix_v1_house_door): its open leaf stands 100 cm outside the wall, so the inside
		// point is 180 cm behind the wall plane (was 250) to keep the leaf within the 250 cm interaction reach for close_from_inside.
		{TEXT("mv_house_door"), TEXT("TestHouse"), {-1150, -9200}, {-520, -9200}},
	};

	ASpnTestDoor* FindDoorById(UWorld& World, const FString& Id)
	{
		for (TActorIterator<ASpnTestDoor> It(&World); It; ++It)
		{
			if (It->StableId == Id)
			{
				return *It;
			}
		}
		return nullptr;
	}

	FVector LeafCenter(const ASpnTestDoor& Door) { return Door.GetComponentsBoundingBox(/*bNonColliding*/ false).GetCenter(); }

	/** What the interaction trace of the character hits right now (also non-interactables), for the failure details. */
	FString DescribeAim(ASupernaturalProtCharacter& Character, bool& bFoundInteractable)
	{
		USpnInteractionComponent* Interaction = Character.FindComponentByClass<USpnInteractionComponent>();
		bFoundInteractable = Interaction != nullptr && Interaction->FindTarget() != nullptr;
		if (Interaction == nullptr)
		{
			return TEXT("character has no interaction component");
		}
		FVector Eye;
		FRotator Rot;
		Character.GetActorEyesViewPoint(Eye, Rot);
		FHitResult Hit;
		FCollisionQueryParams Params(SCENE_QUERY_STAT(SpnQaAim), false, &Character);
		const bool bHit = Character.GetWorld()->SweepSingleByChannel(Hit, Eye, Eye + Rot.Vector() * Interaction->InteractRange, FQuat::Identity,
			Interaction->TraceChannel, FCollisionShape::MakeSphere(Interaction->TraceRadius), Params);
		return FString::Printf(TEXT("eye (%.0f, %.0f, %.0f) pitch %.0f, range %.0f, trace hit %s at %.0f cm"), Eye.X, Eye.Y, Eye.Z, Rot.Pitch,
			Interaction->InteractRange, bHit ? *GetNameSafe(Hit.GetActor()) : TEXT("nothing"), bHit ? Hit.Distance : 0.f);
	}

	/** What the character's capsule hits when it moves on towards Target (name, component, impact point), and the free width of the gap. */
	FString DescribeBlocker(ASupernaturalProtCharacter& Character, const FVector& Target)
	{
		const UCapsuleComponent* Capsule = Character.GetCapsuleComponent();
		const FVector From = Character.GetActorLocation();
		const FVector To(Target.X, Target.Y, From.Z);
		FHitResult Hit;
		FCollisionQueryParams Params(SCENE_QUERY_STAT(SpnQaBlocker), false, &Character);
		const bool bHit = Character.GetWorld()->SweepSingleByChannel(Hit, From, To, FQuat::Identity, ECC_Pawn,
			FCollisionShape::MakeCapsule(Capsule->GetScaledCapsuleRadius(), Capsule->GetScaledCapsuleHalfHeight()), Params);
		if (!bHit)
		{
			return TEXT("capsule sweep towards the inside point finds nothing blocking (the stall is not a static collision)");
		}
		return FString::Printf(TEXT("capsule sweep to the inside point hits %s / %s at (%.0f, %.0f, %.0f) after %.0f cm, normal (%.2f, %.2f)"), *GetNameSafe(Hit.GetActor()),
			*GetNameSafe(Hit.GetComponent()), Hit.ImpactPoint.X, Hit.ImpactPoint.Y, Hit.ImpactPoint.Z, Hit.Distance, Hit.ImpactNormal.X, Hit.ImpactNormal.Y);
	}

	enum class EPhase : uint8
	{
		Approach, SettleOutside, WaitOpen, WalkThrough, SettleInside, WaitCloseInside, WalkOut, ApproachLeaf, SettleLeaf, WaitCloseOutside, Bump, Done
	};

	/** State of one door run; Step() is called every frame by an FScenarioCommand. */
	struct FDoorRun
	{
		FAutomationTestBase* Test = nullptr;
		FDoorSpec Spec{};
		FString Suite;
		ASpnTestDoor* Door = nullptr;
		FVector ClosedCenter = FVector::ZeroVector;
		EPhase Phase = EPhase::Approach;
		double PhaseStart = 0.0;
		int32 PressFrames = 0;
		bool bWDown = false;
		bool bEDown = false;
		bool bClosedFromInside = false;
		bool bOpened = false;
		double BumpStartX = 0.0;

		void Record(const ASupernaturalProtCharacter& Character, const char* Check, SpnQa::EStatus Status, const FString& Detail) const
		{
			SpnQa::Check(*Test, &Character, Suite, FString::Printf(TEXT("%s.%hs"), Spec.Building, Check), Status, Detail);
		}

		void SetW(APlayerController& PC, bool bDown)
		{
			if (bWDown != bDown) { SpnQa::Key(PC, EKeys::W, bDown); bWDown = bDown; }
		}

		void Enter(EPhase Next, double Elapsed) { Phase = Next; PhaseStart = Elapsed; PressFrames = 0; }

		static void Face(ASupernaturalProtCharacter& Character, const FVector& Target)
		{
			const FVector From = Character.GetActorLocation();
			Character.GetController()->SetControlRotation(FRotator(0.f, FMath::RadiansToDegrees(FMath::Atan2(Target.Y - From.Y, Target.X - From.X)), 0.f));
		}

		/** 1 = reached (horizontal distance <= StopDist), -1 = stuck, 0 = still walking. */
		int32 Walk(ASupernaturalProtCharacter& Character, const FVector& Target, float StopDist, double Elapsed)
		{
			APlayerController* PC = Cast<APlayerController>(Character.GetController());
			if (FVector::Dist2D(Character.GetActorLocation(), Target) <= StopDist)
			{
				SetW(*PC, false);
				return 1;
			}
			if (Elapsed - PhaseStart > 15.0)
			{
				SetW(*PC, false);
				return -1;
			}
			Face(Character, Target);
			SetW(*PC, true);
			return 0;
		}

		/** Presses E in one frame and releases it in the next. Returns true after the release. */
		bool PressE(APlayerController& PC)
		{
			if (PressFrames == 0) { SpnQa::Key(PC, EKeys::E, true); bEDown = true; }
			else { SpnQa::Key(PC, EKeys::E, false); bEDown = false; }
			return ++PressFrames > 1;
		}

		bool Settled(const ASupernaturalProtCharacter& Character) const { return Character.GetVelocity().Size2D() < 15.f; }

		bool Step(ASupernaturalProtCharacter& Character, double Elapsed)
		{
			APlayerController* PC = Cast<APlayerController>(Character.GetController());
			if (PC == nullptr || Door == nullptr)
			{
				return true;
			}
			const FVector Inside(Spec.Inside.X, Spec.Inside.Y, 0.f);
			const FVector Out(Spec.Out.X, Spec.Out.Y, 0.f);
			switch (Phase)
			{
			case EPhase::Approach:
			{
				const int32 R = Walk(Character, ClosedCenter, 200.f, Elapsed);
				if (R < 0)
				{
					Record(Character, "approach", SpnQa::EStatus::Blocked, TEXT("could not walk up to the closed door within 15 s"));
					return true;
				}
				if (R > 0) { Enter(EPhase::SettleOutside, Elapsed); }
				return false;
			}
			case EPhase::SettleOutside:
			{
				Face(Character, ClosedCenter);
				if (!Settled(Character) && Elapsed - PhaseStart < 3.0) { return false; }
				bool bTarget = false;
				const FString Aim = DescribeAim(Character, bTarget);
				if (PressFrames == 0)
				{
					const USpnInteractionComponent* Interaction = Character.FindComponentByClass<USpnInteractionComponent>();
					const FString Prompt = Interaction != nullptr ? Interaction->GetCurrentPrompt() : FString();
					Record(Character, "prompt_shown", Prompt == TEXT("E \u2013 T\u00fcr \u00f6ffnen") ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail,
						FString::Printf(TEXT("on-screen prompt while looking at the closed door: '%s' (expected E - Tuer oeffnen)"), *Prompt));
				}
				if (PressFrames == 0) Record(Character, "interact_target", bTarget ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail,
					FString::Printf(TEXT("%.0f cm from the door: %s; interactable found: %s"), FVector::Dist2D(Character.GetActorLocation(), ClosedCenter), *Aim, bTarget ? TEXT("yes") : TEXT("no")));
				if (PressE(*PC)) { Enter(EPhase::WaitOpen, Elapsed); }
				return false;
			}
			case EPhase::WaitOpen:
			{
				if (bEDown) { PressE(*PC); }
				if (Door->IsOpen() && FMath::IsNearlyEqual(Door->GetCurrentAngle(), Door->GetTargetAngle(), 1.f))
				{
					bOpened = true;
					const FVector Leaf = LeafCenter(*Door);
					Record(Character, "opens", SpnQa::EStatus::Pass, FString::Printf(TEXT("E opened the door in %.2f s: angle %.0f, leaf centre (%.0f, %.0f) moved %.0f cm"),
						Elapsed - PhaseStart, Door->GetCurrentAngle(), Leaf.X, Leaf.Y, FVector::Dist2D(Leaf, ClosedCenter)));
					Enter(EPhase::WalkThrough, Elapsed);
				}
				else if (Elapsed - PhaseStart > 4.0)
				{
					bool bTarget = false;
					Record(Character, "opens", SpnQa::EStatus::Fail, FString::Printf(TEXT("E pressed but the door did not open (IsOpen %d, angle %.0f): %s"),
						Door->IsOpen(), Door->GetCurrentAngle(), *DescribeAim(Character, bTarget)));
					return true;
				}
				return false;
			}
			case EPhase::WalkThrough:
			{
				const int32 R = Walk(Character, Inside, 70.f, Elapsed);
				if (R != 0)
				{
					const bool bOk = R > 0;
					Record(Character, "walk_through", bOk ? SpnQa::EStatus::Pass : SpnQa::EStatus::Blocked,
						bOk ? FString::Printf(TEXT("walked through the open door to the inside in %.1f s"), Elapsed - PhaseStart)
							: FString::Printf(TEXT("stuck at (%.0f, %.0f, %.0f) in or behind the doorway; %s"), Character.GetActorLocation().X, Character.GetActorLocation().Y, Character.GetActorLocation().Z, *DescribeBlocker(Character, Inside)));
					if (!bOk) { return true; }
					Enter(EPhase::SettleInside, Elapsed);
				}
				return false;
			}
			case EPhase::SettleInside:
			{
				Face(Character, LeafCenter(*Door));
				if (!Settled(Character) && Elapsed - PhaseStart < 3.0) { return false; }
				if (PressFrames == 0)
				{
					bool bTarget = false;
					AddNote(Character, DescribeAim(Character, bTarget));
				}
				if (PressE(*PC)) { Enter(EPhase::WaitCloseInside, Elapsed); }
				return false;
			}
			case EPhase::WaitCloseInside:
			{
				if (bEDown) { PressE(*PC); }
				if (!Door->IsOpen() && FMath::IsNearlyEqual(Door->GetCurrentAngle(), 0.f, 1.f))
				{
					bClosedFromInside = true;
					Record(Character, "close_from_inside", SpnQa::EStatus::Pass, TEXT("E from the inside closed the door"));
					Enter(EPhase::Bump, Elapsed);
				}
				else if (Elapsed - PhaseStart > 2.5)
				{
					bool bTarget = false;
					Record(Character, "close_from_inside", SpnQa::EStatus::Fail, FString::Printf(TEXT("E at 110 cm behind the doorway, looking at the open leaf, does not close it: %s"), *DescribeAim(Character, bTarget)));
					Enter(EPhase::WalkOut, Elapsed);
				}
				return false;
			}
			case EPhase::WalkOut:
			{
				const int32 R = Walk(Character, Out, 90.f, Elapsed);
				if (R < 0) { Record(Character, "close_from_outside", SpnQa::EStatus::Blocked, TEXT("could not walk back out of the doorway")); return true; }
				if (R > 0) { Enter(EPhase::ApproachLeaf, Elapsed); }
				return false;
			}
			case EPhase::ApproachLeaf:
			{
				const int32 R = Walk(Character, LeafCenter(*Door), 200.f, Elapsed);
				if (R < 0) { Record(Character, "close_from_outside", SpnQa::EStatus::Blocked, TEXT("could not walk up to the open leaf")); return true; }
				if (R > 0) { Enter(EPhase::SettleLeaf, Elapsed); }
				return false;
			}
			case EPhase::SettleLeaf:
			{
				Face(Character, LeafCenter(*Door));
				if (!Settled(Character) && Elapsed - PhaseStart < 3.0) { return false; }
				if (PressE(*PC)) { Enter(EPhase::WaitCloseOutside, Elapsed); }
				return false;
			}
			case EPhase::WaitCloseOutside:
			{
				if (bEDown) { PressE(*PC); }
				if (!Door->IsOpen() && FMath::IsNearlyEqual(Door->GetCurrentAngle(), 0.f, 1.f))
				{
					Record(Character, "close_from_outside", SpnQa::EStatus::Pass, TEXT("E at the open leaf from outside closed the door"));
					Enter(EPhase::Bump, Elapsed);
				}
				else if (Elapsed - PhaseStart > 2.5)
				{
					bool bTarget = false;
					Record(Character, "close_from_outside", SpnQa::EStatus::Fail, FString::Printf(TEXT("door could not be closed from outside either: %s"), *DescribeAim(Character, bTarget)));
					return true;
				}
				return false;
			}
			case EPhase::Bump:
			{
				// Door closed: walking at it from the outside must stop at the leaf (collision follows the swing).
				if (PressFrames == 0)
				{
					Character.GetCharacterMovement()->StopMovementImmediately();
					Character.SetActorLocation(FVector(Out.X, Out.Y, StandZ), false, nullptr, ETeleportType::TeleportPhysics);
					BumpStartX = FVector::Dist2D(FVector(Out.X, Out.Y, 0), ClosedCenter);
					PressFrames = 1;
					PhaseStart = Elapsed;
					return false;
				}
				const int32 R = Walk(Character, Inside, 70.f, Elapsed);
				if (Elapsed - PhaseStart > 4.0 || R != 0)
				{
					SetW(*PC, false);
					const bool bPassed = R > 0;
					Record(Character, "closed_door_blocks", bPassed ? SpnQa::EStatus::Fail : SpnQa::EStatus::Pass,
						bPassed ? TEXT("character walked through the CLOSED door") : FString::Printf(TEXT("closed door stopped the character (%.0f cm from the leaf)"), FVector::Dist2D(Character.GetActorLocation(), ClosedCenter)));
					Enter(EPhase::Done, Elapsed);
					return true;
				}
				return false;
			}
			default:
				return true;
			}
		}

		void AddNote(const ASupernaturalProtCharacter& Character, const FString& Text) const { Test->AddInfo(FString::Printf(TEXT("%s: %s"), Spec.Building, *Text)); }
	};
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnQaDoorsTest, "SpnQA.Doors", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)

bool FSpnQaDoorsTest::RunTest(const FString& Parameters)
{
	if (!FPackageName::DoesPackageExist(MapV2()))
	{
		AddError(*FString::Printf(TEXT("%s does not exist: run create_spn_movement_test_map.py first."), *MapV2()));
		return false;
	}
	AutomationOpenMap(MapV2());
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this]
	{
		SpnQa::Section(TEXT("Doors: E key, open, walk through, close"));
		if (const UWorld* World = FindPlayWorld())
		{
			if (APlayerController* PC = World->GetFirstPlayerController()) { SpnTestPlay::ResetWorldState(*PC); }
		}
		return true;
	}));

	for (const FDoorSpec& Spec : DoorSpecs)
	{
		const TSharedRef<FDoorRun> Run = MakeShared<FDoorRun>();
		Run->Test = this;
		Run->Spec = Spec;
		Run->Suite = TEXT("Doors");
		const float Yaw = FMath::RadiansToDegrees(FMath::Atan2(Spec.Inside.Y - Spec.Out.Y, Spec.Inside.X - Spec.Out.X));
		ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(*this, FVector(Spec.Out.X, Spec.Out.Y, StandZ), Yaw, 90.f,
			[Run, Spec](ASupernaturalProtCharacter& Character)
			{
				SpnQa::Section(FString::Printf(TEXT("Door %s (%s)"), Spec.Building, Spec.DoorId));
				Run->Door = FindDoorById(*Character.GetWorld(), Spec.DoorId);
				if (Run->Door == nullptr)
				{
					SpnQa::Check(*Run->Test, &Character, Run->Suite, FString(Spec.Building) + TEXT(".exists"), SpnQa::EStatus::Fail, FString::Printf(TEXT("door actor %s not in the level"), Spec.DoorId));
					return;
				}
				Run->ClosedCenter = LeafCenter(*Run->Door);
				SpnQa::Check(*Run->Test, &Character, Run->Suite, FString(Spec.Building) + TEXT(".closed_at_start"), Run->Door->IsOpen() ? SpnQa::EStatus::Fail : SpnQa::EStatus::Pass,
					Run->Door->IsOpen() ? TEXT("door already open at start") : TEXT("door closed at start"));
				Character.GetSpnMovement()->SetBaseGait(ESpnGait::Jog);
			},
			[Run](ASupernaturalProtCharacter& Character, float, double Elapsed) { return Run->Step(Character, Elapsed); },
			[Run](ASupernaturalProtCharacter& Character, bool bTimedOut)
			{
				if (APlayerController* PC = Cast<APlayerController>(Character.GetController()))
				{
					Run->SetW(*PC, false);
				}
				if (bTimedOut)
				{
					Run->Record(Character, "timeout", SpnQa::EStatus::Blocked, FString::Printf(TEXT("door run did not finish within 90 s (phase %d)"), static_cast<int32>(Run->Phase)));
				}
			}));
		ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.3f));
	}
	return true;
}

#endif
