#include "Components/CapsuleComponent.h"
#include "Dom/JsonObject.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Framework/Application/SlateApplication.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Misc/FileHelper.h"
#include "Misc/PackageName.h"
#include "Misc/Paths.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Spn/SpnInteractionComponent.h"
#include "Spn/SpnSessionSubsystem.h"
#include "Spn/SpnTestDoor.h"
#include "Spn/Tests/SpnMovementTestSupport.h"
#include "Spn/Tests/SpnQaRecorder.h"
#include "Spn/Tests/SpnTestPlayReset.h"
#include "Spn/UI/SpnInventoryUiSubsystem.h"
#include "Spn/UI/SpnUiSubsystem.h"
#include "Spn/UI/SpnUiText.h"
#include "Spn/World/SpnDecorDoorSubsystem.h"
#include "SupernaturalProtCharacter.h"

#if WITH_DEV_AUTOMATION_TESTS

// SpnWorldQA.RouteControls (developer decisions 2026-11-14) in the generated world, with key events like a player:
//   doors that are part of a building facade: no prompt, but E / F at one is answered ("This door does not open."); an interactive door
//   keeps its prompt;
//   DualSense layout: Triangle does nothing in gameplay (reserved for a heavy attack), the touchpad toggles the map, D-pad up opens the
//   inventory (Circle closes it), Options toggles the pause menu, L3 tap = sprint toggle (ends when the stick is released), L3 hold = gait,
//   Square tap = Interact, Square held = Force (on a runtime-spawned locked, breakable test door).
// Gamepad keys are injected as key events (gameplay keys through the player controller, UI keys through Slate); NO physical controller.

using namespace SpnMovementTest;

namespace
{
	constexpr EAutomationTestFlags Flags = EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter;
	const TCHAR* const Suite = TEXT("SpnWorldControls");
	const TCHAR* const WorldMap = TEXT("/Game/SpnWorld/Lvl_SpnWorld");

	struct FCtl;
	using FStep = TFunction<bool(FCtl&, ASupernaturalProtCharacter&, double)>;

	struct FCtl
	{
		FAutomationTestBase* Test = nullptr;
		TArray<FStep> Steps;
		int32 Index = 0;
		double PhaseStart = 0.0;
		SpnQa::FHeldKeys Keys;
		FVector Start = FVector::ZeroVector;
		float StartYaw = 0.f;
		FVector HouseInFront = FVector::ZeroVector, HouseDoorway = FVector::ZeroVector;
		bool bPoints = false;
		bool bPromptChecked = false;
		FString DoorId;
		double DecorTime = -1.0;
		ESpnGait GaitBefore = ESpnGait::Jog;
		TWeakObjectPtr<ASpnTestDoor> PadDoor;
		FVector PadDoorAim = FVector::ZeroVector;

		void Check(ASupernaturalProtCharacter& C, const TCHAR* Id, bool bOk, const FString& Detail)
		{
			SpnQa::Check(*Test, &C, Suite, Id, bOk ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail, Detail);
		}
	};

	USpnUiSubsystem* Ui(const ASupernaturalProtCharacter& C)
	{
		const APlayerController* PC = Cast<APlayerController>(C.GetController());
		return PC != nullptr && PC->GetLocalPlayer() != nullptr ? PC->GetLocalPlayer()->GetSubsystem<USpnUiSubsystem>() : nullptr;
	}

	USpnInventoryUiSubsystem* InvUi(const ASupernaturalProtCharacter& C)
	{
		const APlayerController* PC = Cast<APlayerController>(C.GetController());
		return PC != nullptr && PC->GetLocalPlayer() != nullptr ? PC->GetLocalPlayer()->GetSubsystem<USpnInventoryUiSubsystem>() : nullptr;
	}

	/** A key through Slate, as the window delivers it (the UI routers sit there). */
	void SlateKey(const FKey& Key)
	{
		const FPlatformUserId User = FPlatformMisc::GetPlatformUserForUserIndex(0);
		const FModifierKeysState Modifiers;
		FSlateApplication::Get().ProcessKeyDownEvent(FKeyEvent(Key, Modifiers, User, false, 0, 0));
		FSlateApplication::Get().ProcessKeyUpEvent(FKeyEvent(Key, Modifiers, User, false, 0, 0));
	}

	void Aim(ASupernaturalProtCharacter& C, const FVector& Target)
	{
		FVector Eye;
		FRotator Unused;
		C.GetActorEyesViewPoint(Eye, Unused);
		const FVector To = Target - Eye;
		C.GetController()->SetControlRotation(FRotator(FMath::RadiansToDegrees(FMath::Atan2(To.Z, To.Size2D())), FMath::RadiansToDegrees(FMath::Atan2(To.Y, To.X)), 0.f));
		C.SetActorRotation(FRotator(0.f, FMath::RadiansToDegrees(FMath::Atan2(To.Y, To.X)), 0.f));
	}

	FVector Vec3(const TSharedPtr<FJsonObject>& O, const TCHAR* Field)
	{
		const TArray<TSharedPtr<FJsonValue>>* A = nullptr;
		if (O.IsValid() && O->TryGetArrayField(Field, A) && A->Num() == 3) { return FVector((*A)[0]->AsNumber(), (*A)[1]->AsNumber(), (*A)[2]->AsNumber()); }
		return FVector::ZeroVector;
	}

	void LoadPoints(FCtl& Ctl)
	{
		FString Text;
		TSharedPtr<FJsonObject> Root;
		const TSharedPtr<FJsonObject>* G = nullptr;
		if (FFileHelper::LoadFileToString(Text, *(FPaths::ProjectSavedDir() / TEXT("SpnWorldGen") / TEXT("qa_points.json")))
			&& FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Text), Root) && Root.IsValid() && Root->TryGetObjectField(TEXT("gameplay"), G))
		{
			Ctl.Start = Vec3(*G, TEXT("start"));
			Ctl.StartYaw = (*G)->GetNumberField(TEXT("start_yaw"));
			const TSharedPtr<FJsonObject> A = (*G)->GetObjectField(TEXT("house_a"));
			Ctl.HouseInFront = Vec3(A, TEXT("porch"));   // on the porch, within reach of the door (as the route checks the prompt)
			Ctl.HouseDoorway = Vec3(A, TEXT("doorway"));
			Ctl.bPoints = true;
		}
	}

	/** A decorative door the player can stand in front of: free floor 170 cm in front of it and a clear view. */
	const USpnDecorDoorSubsystem::FDoor* PickDecorDoor(UWorld& World, const FVector& Near, FVector& OutStand)
	{
		const USpnDecorDoorSubsystem* Doors = World.GetSubsystem<USpnDecorDoorSubsystem>();
		if (Doors == nullptr)
		{
			return nullptr;
		}
		TArray<const USpnDecorDoorSubsystem::FDoor*> Sorted;
		for (const USpnDecorDoorSubsystem::FDoor& D : Doors->GetDoors()) { Sorted.Add(&D); }
		Sorted.Sort([&Near](const USpnDecorDoorSubsystem::FDoor& A, const USpnDecorDoorSubsystem::FDoor& B) { return FVector::DistSquared2D(A.Centre, Near) < FVector::DistSquared2D(B.Centre, Near); });
		for (const USpnDecorDoorSubsystem::FDoor* D : Sorted)
		{
			const FVector Probe = D->Centre + D->Normal * 170.f;
			FHitResult Floor;
			if (!World.LineTraceSingleByChannel(Floor, Probe + FVector(0.f, 0.f, 200.f), Probe - FVector(0.f, 0.f, 400.f), ECC_Visibility) || Floor.ImpactNormal.Z < 0.8f)
			{
				continue;
			}
			const FVector Stand = Floor.ImpactPoint + FVector(0.f, 0.f, 99.f);
			if (World.OverlapBlockingTestByChannel(Stand, FQuat::Identity, ECC_Pawn, FCollisionShape::MakeCapsule(44.f, 96.f)))
			{
				continue;
			}
			FHitResult Sight;
			if (World.LineTraceSingleByChannel(Sight, Stand + FVector(0.f, 0.f, 64.f), D->Centre + D->Normal * 15.f, ECC_Visibility))
			{
				continue;
			}
			OutStand = Stand;
			return D;
		}
		return nullptr;
	}

	TArray<FStep> BuildSteps()
	{
		TArray<FStep> S;
		// --- doors of building facades -------------------------------------------------------------------------------------------------
		S.Add([](FCtl& Ctl, ASupernaturalProtCharacter& C, double T)
		{
			if (T < 0.2) { return false; }
			FVector Stand;
			const USpnDecorDoorSubsystem::FDoor* D = PickDecorDoor(*C.GetWorld(), Ctl.Start, Stand);
			const USpnDecorDoorSubsystem* Doors = C.GetWorld()->GetSubsystem<USpnDecorDoorSubsystem>();
			Ctl.Check(C, TEXT("decor_doors_loaded"), Doors != nullptr && Doors->GetDoorCount() > 0 && D != nullptr,
				FString::Printf(TEXT("%d facade doors loaded from %s; a reachable one: %s"), Doors != nullptr ? Doors->GetDoorCount() : 0, *USpnDecorDoorSubsystem::GetDataPath(), D != nullptr ? *D->Id : TEXT("none")));
			if (D == nullptr) { Ctl.Index = 2; return false; } // skip the decorative-door steps
			Ctl.DoorId = D->Id;
			C.SetActorLocation(Stand, false, nullptr, ETeleportType::TeleportPhysics);
			Aim(C, D->Centre);
			return true;
		});
		S.Add([](FCtl& Ctl, ASupernaturalProtCharacter& C, double T)
		{
			USpnInteractionComponent* I = C.FindComponentByClass<USpnInteractionComponent>();
			if (T < 0.6) { return false; }
			if (!Ctl.bPromptChecked)
			{
				Ctl.bPromptChecked = true;
				Ctl.Check(C, TEXT("decor_door_has_no_marker_or_prompt"), I->GetCurrentPrompt().IsEmpty(), FString::Printf(TEXT("door %s, prompt '%s'"), *Ctl.DoorId, *I->GetCurrentPrompt()));
				Ctl.Keys.Set(C, EKeys::E, true);
				return false;
			}
			Ctl.Keys.Set(C, EKeys::E, false);
			if (T < 1.0) { return false; }
			Ctl.DecorTime = I->GetLastDecorDoorTime();
			Ctl.Check(C, TEXT("decor_door_answers_interact"), I->GetLastDecorDoorId() == Ctl.DoorId && Ctl.DecorTime > 0.0,
				FString::Printf(TEXT("pressed E at %s: answered door '%s' (note '%s')"), *Ctl.DoorId, *I->GetLastDecorDoorId(), *FSpnUiText::Get(TEXT("ui.door.decor.use")).ToString()));
			Ctl.Keys.Set(C, EKeys::F, true);
			return true;
		});
		S.Add([](FCtl& Ctl, ASupernaturalProtCharacter& C, double T)
		{
			if (T < 0.3) { return false; }
			Ctl.Keys.Set(C, EKeys::F, false);
			if (!Ctl.DoorId.IsEmpty())
			{
				const USpnInteractionComponent* I = C.FindComponentByClass<USpnInteractionComponent>();
				Ctl.Check(C, TEXT("decor_door_answers_force"), I->GetLastDecorDoorId() == Ctl.DoorId && I->GetLastDecorDoorTime() > Ctl.DecorTime,
					FString::Printf(TEXT("held F at %s: answered door '%s' again"), *Ctl.DoorId, *I->GetLastDecorDoorId()));
			}
			// an interactive door keeps its clear prompt
			if (Ctl.bPoints)
			{
				C.SetActorLocation(Ctl.HouseInFront + FVector(0.f, 0.f, 100.f), false, nullptr, ETeleportType::TeleportPhysics);
				Aim(C, Ctl.HouseDoorway + FVector(0.f, 0.f, 100.f));
			}
			return true;
		});
		S.Add([](FCtl& Ctl, ASupernaturalProtCharacter& C, double T)
		{
			if (T < 0.8) { return false; }
			const USpnInteractionComponent* I = C.FindComponentByClass<USpnInteractionComponent>();
			const FString Open = FSpnUiText::Get(TEXT("ui.door.open")).ToString();
			Ctl.Check(C, TEXT("interactive_door_keeps_its_prompt"), Ctl.bPoints && I->GetCurrentPrompt().Contains(Open), FString::Printf(TEXT("house A door: prompt '%s' (expects '%s')"), *I->GetCurrentPrompt(), *Open));
			C.SetActorLocation(Ctl.Start + FVector(0.f, 0.f, 20.f), false, nullptr, ETeleportType::TeleportPhysics);
			SpnQa::FaceYaw(C, Ctl.StartYaw);
			return true;
		});
		// --- DualSense layout in gameplay (UI keys through Slate, synchronously: an open map or menu pauses the game) ----------------------
		S.Add([](FCtl& Ctl, ASupernaturalProtCharacter& C, double T)
		{
			if (T < 0.5) { return false; }
			USpnUiSubsystem* U = Ui(C);
			USpnInventoryUiSubsystem* Inv = InvUi(C);
			if (U == nullptr || Inv == nullptr) { Ctl.Check(C, TEXT("ui_routers_present"), false, TEXT("no UI subsystem")); return true; }
			SlateKey(EKeys::Gamepad_FaceButton_Top);
			const bool bTriangleFree = U->GetTopPage() == ESpnPage::None && !Inv->IsOpen() && !C.FindComponentByClass<USpnInteractionComponent>()->IsForcing();
			Ctl.Check(C, TEXT("triangle_is_free_in_gameplay"), bTriangleFree, FString::Printf(TEXT("after Triangle: page %d, inventory open %d"), static_cast<int32>(U->GetTopPage()), Inv->IsOpen()));
			SlateKey(EKeys::Gamepad_Special_Left);
			const bool bMapOpen = U->GetTopPage() == ESpnPage::Map;
			SlateKey(EKeys::Gamepad_Special_Left);
			const bool bMapClosed = U->GetTopPage() == ESpnPage::None;
			Ctl.Check(C, TEXT("touchpad_toggles_the_map"), bMapOpen && bMapClosed, FString::Printf(TEXT("touchpad: map open %d, touchpad again: closed %d"), bMapOpen, bMapClosed));
			U->CloseAll();
			SlateKey(EKeys::Gamepad_DPad_Up);
			const bool bInvOpen = Inv->IsOpen();
			SlateKey(EKeys::Gamepad_FaceButton_Right);
			const bool bInvClosed = !Inv->IsOpen();
			Ctl.Check(C, TEXT("dpad_up_opens_the_inventory_circle_closes"), bInvOpen && bInvClosed, FString::Printf(TEXT("D-pad up: inventory open %d, Circle: closed %d"), bInvOpen, bInvClosed));
			if (Inv->IsOpen()) { Inv->CloseInventory(); }
			SlateKey(EKeys::Gamepad_Special_Right);
			const bool bPause = U->GetTopPage() == ESpnPage::Pause;
			SlateKey(EKeys::Gamepad_Special_Right);
			const bool bResumed = U->GetTopPage() == ESpnPage::None;
			Ctl.Check(C, TEXT("options_toggles_the_pause_menu"), bPause && bResumed, FString::Printf(TEXT("Options: pause %d, Options again: resumed %d"), bPause, bResumed));
			U->CloseAll();
			return true;
		});
		// --- L3: tap = sprint toggle, hold = gait ---------------------------------------------------------------------------------------
		S.Add([](FCtl& Ctl, ASupernaturalProtCharacter& C, double T)
		{
			Ctl.Keys.Set(C, EKeys::W, true);
			if (T < 1.0) { return false; }
			if (T < 1.1) { Ctl.Keys.Set(C, EKeys::Gamepad_LeftThumbstick, true); return false; }
			Ctl.Keys.Set(C, EKeys::Gamepad_LeftThumbstick, false);
			if (T < 3.2) { return false; }
			const float Speed = HorizontalSpeed(C);
			const float Sprint = C.GetSpnMovement()->Settings.SprintSpeed;
			Ctl.Check(C, TEXT("l3_tap_toggles_the_sprint"), C.IsPadSprintToggled() && Speed >= 0.9f * Sprint, FString::Printf(TEXT("L3 tapped while moving: toggled %d, speed %.0f cm/s (sprint %.0f)"), C.IsPadSprintToggled(), Speed, Sprint));
			Ctl.Keys.Set(C, EKeys::W, false);
			return true;
		});
		S.Add([](FCtl& Ctl, ASupernaturalProtCharacter& C, double T)
		{
			if (T < 0.7) { return false; }
			Ctl.Check(C, TEXT("sprint_toggle_ends_when_the_stick_is_released"), !C.IsPadSprintToggled() && !C.GetSpnMovement()->IsSprintRequested(),
				FString::Printf(TEXT("0.7 s after letting go: toggled %d, sprint requested %d"), C.IsPadSprintToggled(), C.GetSpnMovement()->IsSprintRequested()));
			Ctl.GaitBefore = C.GetSpnMovement()->GetBaseGait();
			Ctl.Keys.Set(C, EKeys::Gamepad_LeftThumbstick, true);
			return true;
		});
		S.Add([](FCtl& Ctl, ASupernaturalProtCharacter& C, double T)
		{
			if (T < C.GetGaitHoldSeconds() + 0.25) { return false; }
			Ctl.Keys.Set(C, EKeys::Gamepad_LeftThumbstick, false);
			const ESpnGait After = C.GetSpnMovement()->GetBaseGait();
			Ctl.Check(C, TEXT("l3_hold_changes_the_gait"), After != Ctl.GaitBefore && !C.IsPadSprintToggled(),
				FString::Printf(TEXT("L3 held %.2f s: gait %d -> %d, sprint toggled %d"), C.GetGaitHoldSeconds() + 0.25, static_cast<int32>(Ctl.GaitBefore), static_cast<int32>(After), C.IsPadSprintToggled()));
			C.GetSpnMovement()->SetBaseGait(ESpnGait::Jog);
			// a locked, breakable test door 150 cm ahead (runtime only, never saved into the level)
			const FRotator View(0.f, C.GetActorRotation().Yaw, 0.f);
			FHitResult Floor;
			const FVector Ahead = C.GetActorLocation() + View.Vector() * 150.f;
			C.GetWorld()->LineTraceSingleByChannel(Floor, Ahead + FVector(0.f, 0.f, 100.f), Ahead - FVector(0.f, 0.f, 400.f), ECC_Visibility);
			const FTransform Place(FRotator(0.f, View.Yaw - 180.f, 0.f), Floor.bBlockingHit ? Floor.ImpactPoint : Ahead - FVector(0.f, 0.f, 97.f));
			ASpnTestDoor* Door = C.GetWorld()->SpawnActorDeferred<ASpnTestDoor>(ASpnTestDoor::StaticClass(), Place);
			if (Door != nullptr)
			{
				Door->StableId = TEXT("qa_pad_force_door");
				Door->bAuthoredLocked = true;
				Door->bBreakable = true;
				Door->FinishSpawning(Place);
				Ctl.PadDoor = Door;
				Ctl.PadDoorAim = Door->GetComponentsBoundingBox(false).GetCenter();
				Aim(C, Ctl.PadDoorAim);
			}
			return true;
		});
		S.Add([](FCtl& Ctl, ASupernaturalProtCharacter& C, double T)
		{
			ASpnTestDoor* Door = Ctl.PadDoor.Get();
			if (Door == nullptr) { Ctl.Check(C, TEXT("square_tap_does_not_force"), false, TEXT("the test door could not be spawned")); Ctl.Index = Ctl.Steps.Num() - 1; return false; }
			if (T < 0.5) { return false; }
			if (T < 0.6) { Ctl.Keys.Set(C, EKeys::Gamepad_FaceButton_Left, true); return false; }
			Ctl.Keys.Set(C, EKeys::Gamepad_FaceButton_Left, false);
			if (T < 1.2) { return false; }
			Ctl.Check(C, TEXT("square_tap_does_not_force"), Door->IsLocked() && !Door->IsBroken() && !Door->IsOpen(),
				FString::Printf(TEXT("Square tapped at the locked, breakable door: locked %d, broken %d, open %d"), Door->IsLocked(), Door->IsBroken(), Door->IsOpen()));
			Ctl.Keys.Set(C, EKeys::Gamepad_FaceButton_Left, true);
			return true;
		});
		S.Add([](FCtl& Ctl, ASupernaturalProtCharacter& C, double T)
		{
			ASpnTestDoor* Door = Ctl.PadDoor.Get();
			if (T < 2.6) { return false; }
			Ctl.Keys.Set(C, EKeys::Gamepad_FaceButton_Left, false);
			if (T < 3.8) { return false; }
			Ctl.Check(C, TEXT("square_held_breaks_the_door"), Door != nullptr && Door->IsBroken() && Door->IsOpen(),
				FString::Printf(TEXT("Square held 2.6 s (hold %.1f s + break %.1f s): broken %d, open %d"), C.GetForcePadHoldSeconds(), Door != nullptr ? Door->BreakSeconds : 0.f, Door != nullptr && Door->IsBroken(), Door != nullptr && Door->IsOpen()));
			if (Door != nullptr) { Door->Destroy(); }
			return true;
		});
		S.Add([](FCtl&, ASupernaturalProtCharacter&, double) { return true; }); // end
		return S;
	}
}

// Named so it runs AFTER SpnWorldQA.Route (tests run in name order): Route meets a patrolling NPC whose position depends on the time since the
// level opened, so a test before it shifts that timing.
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnQaSpnWorldControlsTest, "SpnWorldQA.RouteControls", Flags)

bool FSpnQaSpnWorldControlsTest::RunTest(const FString& Parameters)
{
	if (!FPackageName::DoesPackageExist(WorldMap))
	{
		AddError(TEXT("/Game/SpnWorld/Lvl_SpnWorld does not exist: build it with Tools/spn_world_build.ps1 -Phase H"));
		return false;
	}
	const TSharedRef<FCtl> Ctl = MakeShared<FCtl>();
	Ctl->Test = this;
	LoadPoints(*Ctl);
	Ctl->Steps = BuildSteps();

	AutomationOpenMap(WorldMap);
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(3.f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([]
	{
		if (UWorld* World = FindPlayWorld())
		{
			if (APlayerController* PC = World->GetFirstPlayerController()) { SpnTestPlay::ResetWorldState(*PC); }
			if (USpnSessionSubsystem* Session = World->GetGameInstance()->GetSubsystem<USpnSessionSubsystem>()) { Session->EnableGameFlow(); Session->StartNewGameInPlace(World); }
		}
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(2.f));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([] { SpnQa::Section(TEXT("SpnWorld controls: facade doors, DualSense layout, L3, Square hold")); return true; }));
	ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(*this, Ctl->bPoints ? Ctl->Start + FVector(0.f, 0.f, 20.f) : FVector(0.f, 0.f, 500.f), Ctl->StartYaw, 90.f,
		[Ctl](ASupernaturalProtCharacter& Character) { Character.GetSpnMovement()->SetBaseGait(ESpnGait::Jog); },
		[Ctl](ASupernaturalProtCharacter& Character, float, double Elapsed)
		{
			if (Ctl->Index >= Ctl->Steps.Num() - 1) { return true; }
			const int32 Before = Ctl->Index;
			if (Ctl->Steps[Ctl->Index](*Ctl, Character, Elapsed - Ctl->PhaseStart))
			{
				++Ctl->Index;
			}
			if (Ctl->Index != Before) { Ctl->PhaseStart = Elapsed; }
			return false;
		},
		[this, Ctl](ASupernaturalProtCharacter& Character, bool bTimedOut)
		{
			Ctl->Keys.ReleaseAll(Character);
			if (ASpnTestDoor* Door = Ctl->PadDoor.Get()) { Door->Destroy(); }
			SpnQa::Check(*this, &Character, Suite, TEXT("controls_run"), bTimedOut ? SpnQa::EStatus::Blocked : SpnQa::EStatus::Pass,
				bTimedOut ? FString::Printf(TEXT("timeout at step %d of %d"), Ctl->Index, Ctl->Steps.Num()) : FString::Printf(TEXT("all %d steps ran"), Ctl->Steps.Num()));
		}));
	return true;
}

#endif
