#include "Camera/CameraActor.h"
#include "Components/CapsuleComponent.h"
#include "Engine/Engine.h"
#include "EngineUtils.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HAL/FileManager.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/FileHelper.h"
#include "Spn/SpnInteractionComponent.h"
#include "Spn/SpnNpc.h"
#include "Spn/SpnTestDoor.h"
#include "Spn/UI/SpnUiNotify.h"
#include "Spn/UI/SpnUiText.h"
#include "Spn/Tests/SpnMovementTestSupport.h"
#include "Spn/Tests/SpnQaRecorder.h"
#include "Spn/Tests/SpnTestPlayReset.h"
#include "SpnNpcMemorySubsystem.h"
#include "SpnNpcReaction.h"
#include "SpnSaveSubsystem.h"
#include "SpnWorldStateSubsystem.h"

#if WITH_DEV_AUTOMATION_TESTS

// SpnQA.NpcReaction (NightBuild05): the living-world loop in one rendered, walked scenario on Lvl_SpnNpcTest, with real key events:
//   1. the player approaches the building from the yard; a Caretaker works in the archive (patrol), a Neighbour in the foyer
//   2. a harmless noise is ignored
//   3. the player HOLDS F on the locked back door: the door shudders and breaks after 1.6 s -> a suspicious gameplay noise
//   4. the Caretaker (through one wall) hears it and walks to the place; the Neighbour (more walls, further away) does not
//   5. the Caretaker sees the player in the doorway: Alert, alarm (shout), then Flee
//   6. the game is SAVED in the middle of the alarm
//   7. the live state is cleared, the level is reopened (fresh actors) and the save is LOADED: door facts and NPC memories are back, the volatile
//      alarm is not, but the Caretaker resumes by investigating the remembered place
//   8. later, the same harmless noise that was ignored in step 2 now makes the (wary) Caretaker investigate
// Nothing sets an NPC state or a door state directly. The scenario checks report PASS / FAIL like every SpnQA test. PLACEHOLDER rules and numbers.

using namespace SpnMovementTest;

namespace
{
	constexpr EAutomationTestFlags NpcFlags = EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter;
	const TCHAR* const Suite = TEXT("NpcReaction");
	const TCHAR* const NpcMap = TEXT("/Game/SpnTest/Lvl_SpnNpcTest");
	const TCHAR* const NpcSlot = TEXT("spn_qa_npc");
	constexpr int64 CaretakerId = 1001;
	constexpr int64 NeighbourId = 1002;
	constexpr float FloorZ = 15.f;
	const FVector BackDoorAim(445.f, 900.f, FloorZ + 100.f);

	struct FRun;
	using FCheckFn = TFunction<TPair<bool, FString>(FRun&)>;
	using FDoFn = TFunction<void(FRun&)>;
	using FPredFn = TFunction<bool(FRun&)>;

	enum class EOp : uint8 { Section, Wait, WaitUntil, Walk, Face, Hold, Press, Shot, Check, Do, Camera, Speed };

	struct FOp
	{
		EOp Kind = EOp::Wait;
		FVector2D P = FVector2D::ZeroVector;
		float A = 0.f;         // Wait: seconds, Walk: tolerance, Hold: seconds, WaitUntil: timeout, Speed: dilation
		FString Id;
		FKey Key;
		FVector Target = FVector::ZeroVector;
		FCheckFn Check;
		FDoFn Do;
		FPredFn Pred;
	};

	/** What survives the level reopen between the two halves of the scenario. */
	struct FShared
	{
		FString SavedMemory;       // USpnNpcMemorySubsystem::SerializeState() right after the save
		double SavedTime = 0.0;
		int32 CaretakerMemoriesAtSave = 0;
		bool bSaved = false;
		FVector LastIncidentCm = FVector::ZeroVector;
	};

	struct FRun
	{
		TArray<FOp> Ops;
		SpnQa::FHeldKeys Keys;
		ASupernaturalProtCharacter* Character = nullptr;
		TSharedPtr<FShared> Shared;
		int32 Index = -1;
		int32 Frames = 0;
		double StepStart = 0.0;
		FString Failure;
		FVector CaretakerStart = FVector::ZeroVector;
		float CaretakerWalked = 0.f;
		FVector CaretakerLast = FVector::ZeroVector;
		bool bFirstTick = true;

		UWorld* World() const { return Character->GetWorld(); }
		ASpnNpc* Npc(int64 Id) const
		{
			for (TActorIterator<ASpnNpc> It(World()); It; ++It) { if (It->NpcId == Id) { return *It; } }
			return nullptr;
		}
		ASpnTestDoor* Door(const TCHAR* StableId) const
		{
			for (TActorIterator<ASpnTestDoor> It(World()); It; ++It) { if (It->StableId == StableId) { return *It; } }
			return nullptr;
		}
		USpnNpcMemorySubsystem* Memory() const { return Character->GetGameInstance()->GetSubsystem<USpnNpcMemorySubsystem>(); }
		USpnNoiseSubsystem* Noise() const { return Character->GetGameInstance()->GetSubsystem<USpnNoiseSubsystem>(); }
		USpnSaveSubsystem* Save() const { return Character->GetGameInstance()->GetSubsystem<USpnSaveSubsystem>(); }
		ACameraActor* Camera(const TCHAR* Tag) const
		{
			for (TActorIterator<ACameraActor> It(World()); It; ++It) { if (It->ActorHasTag(FName(Tag))) { return *It; } }
			return nullptr;
		}
	};

	FOp MakeOp(EOp Kind) { FOp Op; Op.Kind = Kind; return Op; }

	struct FBuilder
	{
		TArray<FOp>& Ops;
		void Section(const TCHAR* Title) { FOp Op = MakeOp(EOp::Section); Op.Id = Title; Ops.Add(Op); }
		void Wait(float Seconds) { FOp Op = MakeOp(EOp::Wait); Op.A = Seconds; Ops.Add(Op); }
		void WaitUntil(const TCHAR* Id, float Timeout, FPredFn Pred) { FOp Op = MakeOp(EOp::WaitUntil); Op.Id = Id; Op.A = Timeout; Op.Pred = MoveTemp(Pred); Ops.Add(Op); }
		void Walk(float X, float Y, float Tolerance = 30.f) { FOp Op = MakeOp(EOp::Walk); Op.P = FVector2D(X, Y); Op.A = Tolerance; Ops.Add(Op); }
		void Face(FVector Target) { FOp Op = MakeOp(EOp::Face); Op.Target = Target; Ops.Add(Op); }
		void Hold(const FKey& Key, FVector Target, float Seconds) { FOp Op = MakeOp(EOp::Hold); Op.Key = Key; Op.Target = Target; Op.A = Seconds; Ops.Add(Op); }
		void Shot(const TCHAR* Name) { FOp Op = MakeOp(EOp::Shot); Op.Id = Name; Ops.Add(Op); }
		void Check(const TCHAR* Id, FCheckFn Fn) { FOp Op = MakeOp(EOp::Check); Op.Id = Id; Op.Check = MoveTemp(Fn); Ops.Add(Op); }
		void Do(FDoFn Fn) { FOp Op = MakeOp(EOp::Do); Op.Do = MoveTemp(Fn); Ops.Add(Op); }
		/** Cut to a camera actor by tag; an empty tag cuts back to the player. */
		void Camera(const TCHAR* Tag) { FOp Op = MakeOp(EOp::Camera); Op.Id = Tag; Ops.Add(Op); }
		void Speed(float Dilation) { FOp Op = MakeOp(EOp::Speed); Op.A = Dilation; Ops.Add(Op); }
	};

	TPair<bool, FString> Result(bool bOk, const FString& Detail) { return TPair<bool, FString>(bOk, Detail); }

	FString NpcText(const ASpnNpc* Npc)
	{
		if (Npc == nullptr) { return TEXT("NPC missing"); }
		return FString::Printf(TEXT("%s: %s, wary %d, sees player %d, heard %d, ignored %d, not heard %d, alarms %d, at %.0f,%.0f"), *Npc->DisplayName, LexToString(Npc->GetReactionState()),
			Npc->IsWary(), Npc->SeesPlayer(), Npc->GetNoisesHeard(), Npc->GetNoisesIgnored(), Npc->GetNoisesNotHeard(), Npc->GetAlarmsRaised(), Npc->GetActorLocation().X, Npc->GetActorLocation().Y);
	}

	FString DoorText(const ASpnTestDoor* Door)
	{
		return Door != nullptr ? FString::Printf(TEXT("%s: open %d, locked %d, broken %d, leaf %.0f/%.0f"), *Door->StableId, Door->IsOpen(), Door->IsLocked(), Door->IsBroken(), Door->GetCurrentAngle(), Door->GetTargetAngle()) : FString(TEXT("door missing"));
	}

	void Caption(const FRun& R, const TCHAR* Key)
	{
		SpnUi::Notify(R.Character, FText::FromString(Key), SpnUi::ENote::Info);
	}

	/** A large centred title or end card (the recording has no other title screen). */
	void Title(const TCHAR* Text, float Seconds)
	{
		if (GEngine != nullptr) { GEngine->AddOnScreenDebugMessage(7300, Seconds, FColor(240, 240, 255), Text, /*bNewerOnTop*/ false, FVector2D(3.f, 3.f)); }
	}

	FSpnNoise HarmlessDoorNoise(const FVector& At)
	{
		FSpnNoise N;
		N.LocationCm = At;
		N.RangeCm = 600.f;
		N.Strength = 0.35f;
		N.Kind = ESpnNoiseKind::DoorUse;
		N.ObjectId = TEXT("placeholder.door.archive_door");
		N.SourceId = TEXT("qa");
		return N;
	}

	/** On-screen status of both residents (stays on screen; this is the readable "what does the NPC know" overlay of the recording). */
	void DrawStatus(const FRun& R)
	{
		if (GEngine == nullptr) { return; }
		const USpnNpcMemorySubsystem* Memory = R.Memory();
		int32 Key = 7100;
		for (const int64 Id : {CaretakerId, NeighbourId})
		{
			const ASpnNpc* Npc = R.Npc(Id);
			if (Npc == nullptr || Memory == nullptr) { continue; }
			const FSpnWariness W = Memory->Assess(Id);
			const FColor Color = Npc->GetReactionState() == ESpnReactionState::Idle ? FColor(150, 230, 160) : (Npc->GetReactionState() == ESpnReactionState::Flee ? FColor(255, 110, 110) : FColor(255, 215, 90));
			GEngine->AddOnScreenDebugMessage(Key++, 0.5f, Color, FString::Printf(TEXT("%-10s  %-16s  memories %d   wary %s%s%s   heard %d / not heard %d / ignored %d"), *Npc->DisplayName,
				LexToString(Npc->GetReactionState()), Memory->GetMemoryCount(Id), W.bWary ? TEXT("yes") : TEXT("no"), W.bBrokeInRemembered ? TEXT(" [break-in]") : TEXT(""),
				W.bRecentAlarm ? TEXT(" [alarm]") : TEXT(""), Npc->GetNoisesHeard(), Npc->GetNoisesNotHeard(), Npc->GetNoisesIgnored()), /*bNewerOnTop*/ false, FVector2D(1.7f, 1.7f));
		}
	}

	/** Part 1: approach, break-in, reaction, save. */
	TArray<FOp> BuildFirstHalf()
	{
		TArray<FOp> Ops;
		FBuilder B{Ops};

		B.Do([](FRun& R)
		{
			R.CaretakerStart = R.Npc(CaretakerId) ? R.Npc(CaretakerId)->GetActorLocation() : FVector::ZeroVector;
			R.CaretakerLast = R.CaretakerStart;
			if (USpnNpcMemorySubsystem* Memory = R.Memory()) { Memory->ResetAll(); }   // a new game: nobody remembers anything
			if (USpnSaveSubsystem* Save = R.Save()) { Save->SetSaveDirectory(FPaths::ConvertRelativePathToFull(SpnQa::QaDir() / TEXT("saves"))); }
		});
		B.Section(TEXT("NPC: the yard behind the building"));
		B.Do([](FRun& R) { Title(TEXT("NIGHT BUILD 05  -  A LIVING WORLD: PERCEPTION, MEMORY, CONSEQUENCES"), 8.f); });
		B.Do([](FRun& R) { Caption(R, TEXT("1  The Caretaker works inside. The player approaches the building.")); });
		B.Camera(TEXT("SPN_Cam_Overview"));
		B.Wait(1.0f);
		B.Shot(TEXT("01_overview_routine"));
		B.Wait(9.0f);
		B.Check(TEXT("npcs_start_idle_and_unaware"), [](FRun& R) { return Result(R.Npc(CaretakerId) && R.Npc(NeighbourId) && R.Npc(CaretakerId)->GetReactionState() == ESpnReactionState::Idle && R.Npc(NeighbourId)->GetReactionState() == ESpnReactionState::Idle
			&& R.Memory()->GetMemoryCount(CaretakerId) == 0 && R.Memory()->GetMemoryCount(NeighbourId) == 0, NpcText(R.Npc(CaretakerId)) + TEXT(" | ") + NpcText(R.Npc(NeighbourId))); });
		B.Check(TEXT("caretaker_goes_about_a_routine"), [](FRun& R) { const ASpnNpc* N = R.Npc(CaretakerId); const float Moved = N ? FVector::Dist2D(N->GetActorLocation(), R.CaretakerStart) : 0.f;
			return Result(Moved > 80.f || R.CaretakerWalked > 80.f, FString::Printf(TEXT("%.0f cm from its start, %.0f cm walked, nav mesh %d, fallback steering %d"), Moved, R.CaretakerWalked, N && N->HasNavigationMesh(), N && N->IsUsingFallbackSteering())); });
		B.Do([](FRun& R)
		{
			// a harmless noise next to the Caretaker: heard, but nothing to worry about
			if (const ASpnNpc* N = R.Npc(CaretakerId)) { R.Noise()->EmitNoise(HarmlessDoorNoise(N->GetActorLocation() + FVector(150.f, 0.f, 80.f))); }
			Caption(R, TEXT("2  A harmless noise (a door being used): the Caretaker does not care."));
		});
		B.Wait(4.5f);
		B.Check(TEXT("harmless_noise_is_ignored_before_the_incident"), [](FRun& R) { const ASpnNpc* N = R.Npc(CaretakerId); return Result(N && N->GetNoisesIgnored() >= 1 && N->GetReactionState() == ESpnReactionState::Idle && R.Memory()->GetMemoryCount(CaretakerId) == 0, NpcText(N)); });
		B.Camera(TEXT(""));
		B.Do([](FRun& R) { Caption(R, TEXT("3  The player walks to the locked back door.")); });
		B.Walk(900, 1700, 80);
		B.Walk(700, 1400, 60);
		B.Walk(470, 1060, 30);
		B.Face(BackDoorAim);
		B.Wait(0.8f);
		B.Check(TEXT("back_door_offers_break_in"), [](FRun& R) { return Result(R.Character->FindComponentByClass<USpnInteractionComponent>()->GetCurrentPrompt().Contains(FSpnUiText::Get(TEXT("ui.door.force")).ToString()), FString::Printf(TEXT("prompt '%s'"), *R.Character->FindComponentByClass<USpnInteractionComponent>()->GetCurrentPrompt())); });
		B.Check(TEXT("back_door_locked_and_intact_before"), [](FRun& R) { const ASpnTestDoor* D = R.Door(TEXT("back_door")); return Result(D && D->IsLocked() && !D->IsBroken() && !D->IsOpen(), DoorText(D)); });
		B.Shot(TEXT("02_player_at_the_back_door"));
		B.Do([](FRun& R) { Caption(R, TEXT("4  F held: the door takes time to break (1.6 s).")); });
		B.Hold(EKeys::F, BackDoorAim, 2.2f);
		B.Check(TEXT("back_door_broke_after_the_hold"), [](FRun& R) { const ASpnTestDoor* D = R.Door(TEXT("back_door")); return Result(D && D->IsBroken() && D->IsOpen() && !D->IsLocked(), DoorText(D)); });
		B.Do([](FRun& R) { Caption(R, TEXT("5  The break-in is a loud, suspicious noise (gameplay event, no audio needed).")); });
		B.Camera(TEXT("SPN_Cam_BackDoor"));
		B.WaitUntil(TEXT("caretaker_hears_the_break_in_and_investigates"), 8.f, [](FRun& R) { const ASpnNpc* N = R.Npc(CaretakerId); return N && N->GetReactionState() == ESpnReactionState::Investigate; });
		B.Check(TEXT("caretaker_investigates_and_remembers"), [](FRun& R) { const ASpnNpc* N = R.Npc(CaretakerId); const FSpnWariness W = R.Memory()->Assess(CaretakerId);
			return Result(N && N->GetReactionState() == ESpnReactionState::Investigate && W.bWary && W.bBrokeInRemembered && !W.bPlayerSeenRemembered && R.Memory()->GetMemoryCount(CaretakerId) >= 1, NpcText(N)); });
		B.Check(TEXT("neighbour_behind_more_walls_does_not_hear_it"), [](FRun& R) { const ASpnNpc* N = R.Npc(NeighbourId); return Result(N && N->GetNoisesNotHeard() >= 1 && N->GetReactionState() == ESpnReactionState::Idle && R.Memory()->GetMemoryCount(NeighbourId) == 0, NpcText(N)); });
		B.Camera(TEXT("SPN_Cam_Storage"));
		B.Shot(TEXT("03_caretaker_investigates"));
		B.Do([](FRun& R) { Caption(R, TEXT("6  The Caretaker investigates the place the noise came from.")); });
		B.WaitUntil(TEXT("caretaker_sees_the_player_and_raises_the_alarm"), 40.f, [](FRun& R) { const ASpnNpc* N = R.Npc(CaretakerId); return N && (N->GetReactionState() == ESpnReactionState::Alert || N->GetReactionState() == ESpnReactionState::Flee); });
		B.Check(TEXT("alarm_is_raised_once_and_remembered"), [](FRun& R) { const ASpnNpc* N = R.Npc(CaretakerId); const FSpnWariness W = R.Memory()->Assess(CaretakerId);
			return Result(N && N->GetAlarmsRaised() == 1 && W.bRecentAlarm && W.bPlayerSeenRemembered, NpcText(N)); });
		B.Do([](FRun& R) { Caption(R, TEXT("7  It sees the player: alarm, then flight.")); });
		B.Check(TEXT("neighbour_hears_the_alarm_shout_and_investigates"), [](FRun& R) { const ASpnNpc* N = R.Npc(NeighbourId); const FSpnWariness W = R.Memory()->Assess(NeighbourId);
			return Result(N && N->GetNoisesHeard() >= 1 && N->GetReactionState() == ESpnReactionState::Investigate && R.Memory()->GetMemoryCount(NeighbourId) >= 1, NpcText(N) + FString::Printf(TEXT(" | suspicion %.2f"), W.Suspicion)); });
		B.Shot(TEXT("04_alert"));
		B.WaitUntil(TEXT("caretaker_flees"), 10.f, [](FRun& R) { const ASpnNpc* N = R.Npc(CaretakerId); return N && N->GetReactionState() == ESpnReactionState::Flee; });
		B.Wait(2.5f);
		B.Shot(TEXT("05_flee"));
		B.Camera(TEXT("SPN_Cam_Overview"));
		B.Wait(5.0f);
		B.Do([](FRun& R)
		{
			// SAVE in the middle of the alarm
			USpnSaveSubsystem* Save = R.Save();
			const FSpnSaveStatus Status = Save->SaveSlot(NpcSlot);
			R.Shared->bSaved = Status.IsOk();
			R.Shared->SavedMemory = R.Memory()->SerializeState();
			R.Shared->SavedTime = R.Memory()->GetWorldTime();
			R.Shared->CaretakerMemoriesAtSave = R.Memory()->GetMemoryCount(CaretakerId);
			R.Shared->LastIncidentCm = R.Memory()->Assess(CaretakerId).LastIncidentCm;
			Caption(R, TEXT("8  The game is saved during the alarm."));
			if (!Status.IsOk()) { R.Failure = FString::Printf(TEXT("saving failed: %s"), *Status.Detail); }
		});
		B.Check(TEXT("saved_during_the_alarm"), [](FRun& R) { const ASpnNpc* N = R.Npc(CaretakerId); return Result(R.Shared->bSaved && N && (N->GetReactionState() == ESpnReactionState::Flee || N->GetReactionState() == ESpnReactionState::Alert) && R.Shared->CaretakerMemoriesAtSave >= 3 && !R.Shared->SavedMemory.Contains(TEXT("Flee")),
			FString::Printf(TEXT("%s; %d caretaker memories; %d bytes of NPC memory in the slot"), *NpcText(N), R.Shared->CaretakerMemoriesAtSave, R.Shared->SavedMemory.Len())); });
		B.Wait(2.0f);
		return Ops;
	}

	/** Part 2 (fresh level): load, the memory is back, the alarm state is not; later behaviour differs because of the memory. */
	TArray<FOp> BuildSecondHalf()
	{
		TArray<FOp> Ops;
		FBuilder B{Ops};
		B.Do([](FRun& R) { if (USpnSaveSubsystem* Save = R.Save()) { Save->SetSaveDirectory(FPaths::ConvertRelativePathToFull(SpnQa::QaDir() / TEXT("saves"))); } });
		B.Do([](FRun& R) { Caption(R, TEXT("9  New session: the level is reopened and the save is loaded.")); });
		B.Camera(TEXT("SPN_Cam_Overview"));
		B.Wait(1.2f);
		B.Check(TEXT("reloaded_level_has_new_actors_in_the_authored_state"), [](FRun& R) { const ASpnTestDoor* D = R.Door(TEXT("back_door")); const ASpnNpc* N = R.Npc(CaretakerId);
			return Result(D && D->IsLocked() && !D->IsBroken() && N && N->GetReactionState() == ESpnReactionState::Idle && R.Memory()->GetMemoryCount(CaretakerId) == 0 && N->GetAlarmsRaised() == 0 && N->GetNoisesHeard() == 0 && N->GetReactionLog().Num() == 0, DoorText(D) + TEXT(" | ") + NpcText(N)); });
		B.Do([](FRun& R)
		{
			const FSpnSaveStatus Status = R.Save()->LoadSlot(NpcSlot);
			if (!Status.IsOk()) { R.Failure = FString::Printf(TEXT("loading failed: %s"), *Status.Detail); }
			R.Shared->bSaved = Status.IsOk() && Status.bNpcMemoryRestored;
		});
		B.Wait(0.6f);
		B.Check(TEXT("load_restores_door_and_npc_memory"), [](FRun& R) { const ASpnTestDoor* D = R.Door(TEXT("back_door"));
			return Result(R.Shared->bSaved && D && D->IsBroken() && D->IsOpen() && R.Memory()->SerializeState() == R.Shared->SavedMemory && R.Memory()->GetMemoryCount(CaretakerId) == R.Shared->CaretakerMemoriesAtSave,
				DoorText(D) + FString::Printf(TEXT(" | memory identical: %d, caretaker memories %d (saved %d)"), R.Memory()->SerializeState() == R.Shared->SavedMemory, R.Memory()->GetMemoryCount(CaretakerId), R.Shared->CaretakerMemoriesAtSave)); });
		B.Check(TEXT("neighbour_memory_is_restored_too"), [](FRun& R) { const ASpnNpc* N = R.Npc(NeighbourId); return Result(N && R.Memory()->GetMemoryCount(NeighbourId) >= 1, NpcText(N) + FString::Printf(TEXT(" | neighbour memories %d"), R.Memory()->GetMemoryCount(NeighbourId))); });
		B.Check(TEXT("time_continues_from_the_saved_clock"), [](FRun& R) { return Result(FMath::Abs(R.Memory()->GetWorldTime() - R.Shared->SavedTime) < 2.0, FString::Printf(TEXT("clock %.2f, saved at %.2f"), R.Memory()->GetWorldTime(), R.Shared->SavedTime)); });
		B.Check(TEXT("volatile_alarm_is_not_saved_but_caretaker_resumes_investigating"), [](FRun& R) { const ASpnNpc* N = R.Npc(CaretakerId); const FSpnWariness W = R.Memory()->Assess(CaretakerId);
			return Result(N && N->IsWary() && W.bRecentAlarm && N->GetReactionState() == ESpnReactionState::Investigate && N->GetReactionLog().Num() > 0 && N->GetReactionLog().Last().Why == TEXT("restored from memory"), NpcText(N)); });
		B.Do([](FRun& R) { Caption(R, TEXT("10  Not saved: the alarm itself. Saved: what the Caretaker remembers. It goes to the remembered place.")); });
		B.Shot(TEXT("06_after_load"));
		B.Wait(6.0f);
		B.Do([](FRun& R)
		{
			// the player leaves the sight of the building so the story can settle (a time-lapse of the calm-down follows)
			R.Character->SetActorLocation(FVector(1500.f, -700.f, StandZ), false, nullptr, ETeleportType::TeleportPhysics);
		});
		B.Camera(TEXT("SPN_Cam_Overview"));
		B.Do([](FRun& R) { Caption(R, TEXT("11  Time passes (x3): the Caretaker searches, then returns to its work.")); });
		B.Speed(3.f);
		B.WaitUntil(TEXT("caretaker_calms_down_again"), 150.f, [](FRun& R) { const ASpnNpc* N = R.Npc(CaretakerId); return N && N->GetReactionState() == ESpnReactionState::Idle; });
		B.Speed(1.f);
		B.Check(TEXT("caretaker_is_idle_again_but_still_wary"), [](FRun& R) { const ASpnNpc* N = R.Npc(CaretakerId); return Result(N && N->GetReactionState() == ESpnReactionState::Idle && N->IsWary(), NpcText(N)); });
		B.Do([](FRun& R)
		{
			// the SAME harmless noise as at the beginning
			if (const ASpnNpc* N = R.Npc(CaretakerId)) { R.Noise()->EmitNoise(HarmlessDoorNoise(N->GetActorLocation() + FVector(150.f, 0.f, 80.f))); }
			Caption(R, TEXT("12  The same harmless noise as in the beginning - now the Caretaker remembers the break-in."));
		});
		B.WaitUntil(TEXT("remembered_break_in_changes_the_reaction_to_a_harmless_noise"), 3.f, [](FRun& R) { const ASpnNpc* N = R.Npc(CaretakerId); return N && N->GetReactionState() == ESpnReactionState::Investigate; });
		B.Check(TEXT("wary_caretaker_investigates_a_harmless_noise"), [](FRun& R) { const ASpnNpc* N = R.Npc(CaretakerId); return Result(N && N->GetReactionState() == ESpnReactionState::Investigate, NpcText(N)); });
		B.Shot(TEXT("07_memory_changes_behaviour"));
		B.Wait(6.0f);
		B.Do([](FRun& R) { Caption(R, TEXT("13  Second visit: the back door is still broken (saved), the Caretaker still remembers.")); });
		B.Do([](FRun& R)
		{
			R.Character->SetActorLocation(FVector(1500.f, 2500.f, StandZ), false, nullptr, ETeleportType::TeleportPhysics);
			R.Character->GetController()->SetControlRotation(FRotator(0.f, -135.f, 0.f));
		});
		B.Camera(TEXT(""));
		B.Walk(900, 1700, 80);
		B.Walk(700, 1400, 60);
		B.Walk(470, 1040, 40);
		B.Walk(450, 820, 40);
		B.Check(TEXT("second_visit_back_door_is_still_open_and_broken"), [](FRun& R) { const ASpnTestDoor* D = R.Door(TEXT("back_door")); return Result(D && D->IsBroken() && D->IsOpen() && R.Character->GetActorLocation().Y < 880.f, DoorText(D) + FString::Printf(TEXT(" | player at %.0f, %.0f"), R.Character->GetActorLocation().X, R.Character->GetActorLocation().Y)); });
		B.Camera(TEXT("SPN_Cam_Storage"));
		B.Walk(300, 620, 40);
		B.Walk(250, 530, 40);
		B.WaitUntil(TEXT("wary_caretaker_reacts_to_the_sight_of_the_player"), 30.f, [](FRun& R) { const ASpnNpc* N = R.Npc(CaretakerId); return N && (N->GetReactionState() == ESpnReactionState::Alert || N->GetReactionState() == ESpnReactionState::Flee); });
		B.WaitUntil(TEXT("caretaker_flees_again"), 6.f, [](FRun& R) { const ASpnNpc* N = R.Npc(CaretakerId); return N && N->GetReactionState() == ESpnReactionState::Flee; });
		B.Check(TEXT("second_encounter_the_sight_alone_raises_the_alarm_without_any_noise"), [](FRun& R) { const ASpnNpc* N = R.Npc(CaretakerId); if (N == nullptr) { return Result(false, TEXT("no caretaker")); }
			// in this new session the Caretaker heard nothing suspicious: it went Idle -> Alert (or ReturnToRoutine -> Alert) because it REMEMBERS, and then fled
			bool bSightAlarm = false, bFled = false, bNoiseDriven = false;
			for (const FSpnReactionLogEntry& E : N->GetReactionLog()) { if (E.To == ESpnReactionState::Alert && E.Why.StartsWith(TEXT("sees the player"))) { bSightAlarm = true; } if (E.To == ESpnReactionState::Flee) { bFled = true; } if (E.Why.StartsWith(TEXT("noise: DoorBreak"))) { bNoiseDriven = true; } }
			return Result(bSightAlarm && bFled && !bNoiseDriven && N->GetAlarmsRaised() == 1, FString::Printf(TEXT("sight alarm %d, fled %d, break-in noise in this session %d | %s"), bSightAlarm, bFled, bNoiseDriven, *NpcText(N))); });
		B.Shot(TEXT("08_second_visit_flee"));
		B.Wait(5.0f);
		B.Camera(TEXT("SPN_Cam_Overview"));
		B.Do([](FRun& R) { Title(TEXT("SAVED: doors, NPC memories (what, where, how bad, when)     NOT SAVED: alarm state, positions"), 11.f); });
		B.Wait(12.0f);
		B.Do([](FRun& R) { Caption(R, TEXT("End of the scenario.")); });
		B.Wait(1.5f);
		return Ops;
	}

	void CleanShot(const FString& Id)
	{
		if (FApp::CanEverRender())
		{
			FScreenshotRequest::RequestScreenshot(SpnQa::QaDir() / TEXT("shots") / (FString(TEXT("Npc_")) + Id + TEXT(".png")), /*bShowUI*/ true, /*bAddFilenameSuffix*/ false);
		}
	}

	/** Runs ops on the scenario character; same engine for both halves. */
	bool StepRun(FAutomationTestBase& Test, FRun& R, ASupernaturalProtCharacter& Character, double Elapsed)
	{
		APlayerController* PC = Cast<APlayerController>(Character.GetController());
		const FVector Loc = Character.GetActorLocation();
		if (R.Index < 0) { R.Index = 0; R.StepStart = Elapsed; R.Frames = 0; }
		if (!R.Failure.IsEmpty() || !R.Ops.IsValidIndex(R.Index)) { return true; }
		DrawStatus(R);
		if (const ASpnNpc* N = R.Npc(CaretakerId))
		{
			R.CaretakerWalked += FVector::Dist2D(N->GetActorLocation(), R.CaretakerLast);
			R.CaretakerLast = N->GetActorLocation();
		}
		FOp& Op = R.Ops[R.Index];
		const double InStep = Elapsed - R.StepStart;
		const auto Next = [&]() { ++R.Index; R.StepStart = Elapsed; R.Frames = 0; };
		if (Op.Kind != EOp::Walk) { R.Keys.Set(Character, EKeys::W, false); }

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
			if (Op.Pred(R)) { SpnQa::Check(Test, &Character, Suite, Op.Id, SpnQa::EStatus::Pass, FString::Printf(TEXT("after %.1f s"), InStep)); Next(); }
			else if (InStep >= Op.A) { SpnQa::Check(Test, &Character, Suite, Op.Id, SpnQa::EStatus::Fail, FString::Printf(TEXT("did not happen within %.0f s; %s | %s"), Op.A, *NpcText(R.Npc(CaretakerId)), *NpcText(R.Npc(NeighbourId)))); Next(); }
			break;
		case EOp::Walk:
		{
			const FVector2D To = Op.P - FVector2D(Loc.X, Loc.Y);
			if (To.Size() <= Op.A) { R.Keys.Set(Character, EKeys::W, false); Next(); break; }
			if (InStep > 40.0)
			{
				R.Keys.Set(Character, EKeys::W, false);
				R.Failure = FString::Printf(TEXT("stuck %.0f cm from the waypoint (%.0f, %.0f) at (%.0f, %.0f, %.0f)"), To.Size(), Op.P.X, Op.P.Y, Loc.X, Loc.Y, Loc.Z);
				return true;
			}
			PC->SetControlRotation(FRotator(-5.f, FMath::RadiansToDegrees(FMath::Atan2(Op.P.Y - Loc.Y, Op.P.X - Loc.X)), 0.f));
			R.Keys.Set(Character, EKeys::W, true);
			break;
		}
		case EOp::Face:
		{
			const FVector Eye = Character.GetActorLocation() + FVector(0.f, 0.f, 64.f);
			const FVector To = Op.Target - Eye;
			PC->SetControlRotation(FRotator(FMath::RadiansToDegrees(FMath::Atan2(To.Z, To.Size2D())), FMath::RadiansToDegrees(FMath::Atan2(To.Y, To.X)), 0.f));
			Next();
			break;
		}
		case EOp::Hold:
		{
			const FVector Eye = Character.GetActorLocation() + FVector(0.f, 0.f, 64.f);
			const FVector To = Op.Target - Eye;
			PC->SetControlRotation(FRotator(FMath::RadiansToDegrees(FMath::Atan2(To.Z, To.Size2D())), FMath::RadiansToDegrees(FMath::Atan2(To.Y, To.X)), 0.f));
			if (R.Frames == 0 && Character.GetVelocity().Size2D() > 15.f && InStep < 3.0) { break; }
			if (R.Frames == 0) { SpnQa::Key(*PC, Op.Key, true); }
			++R.Frames;
			if (InStep >= Op.A) { SpnQa::Key(*PC, Op.Key, false); Next(); }
			break;
		}
		case EOp::Press:
			SpnQa::Key(*PC, Op.Key, R.Frames == 0);
			if (++R.Frames > 1) { Next(); }
			break;
		case EOp::Shot:
			CleanShot(Op.Id);
			Next();
			break;
		case EOp::Check:
		{
			const TPair<bool, FString> Verdict = Op.Check(R);
			SpnQa::Check(Test, &Character, Suite, Op.Id, Verdict.Key ? SpnQa::EStatus::Pass : SpnQa::EStatus::Fail, Verdict.Value);
			Next();
			break;
		}
		case EOp::Do:
			Op.Do(R);
			Next();
			break;
		case EOp::Camera:
		{
			AActor* Target = Op.Id.IsEmpty() ? static_cast<AActor*>(&Character) : static_cast<AActor*>(R.Camera(*Op.Id));
			if (Target != nullptr) { PC->SetViewTargetWithBlend(Target, 0.5f); }
			Next();
			break;
		}
		case EOp::Speed:
			UGameplayStatics::SetGlobalTimeDilation(R.World(), Op.A);
			Next();
			break;
		}
		return !R.Ops.IsValidIndex(R.Index);
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnQaNpcTest, "SpnQA.NpcReaction", NpcFlags)

bool FSpnQaNpcTest::RunTest(const FString& Parameters)
{
	if (!FPackageName::DoesPackageExist(NpcMap))
	{
		AddError(TEXT("/Game/SpnTest/Lvl_SpnNpcTest does not exist: run create_spn_building_test_map.py with SPN_VARIANT=npc first."));
		return false;
	}
	const TSharedRef<FShared> Shared = MakeShared<FShared>();

	const auto MakeHalf = [this, Shared](TArray<FOp> Ops, const TCHAR* Name)
	{
		const TSharedRef<FRun> Run = MakeShared<FRun>();
		Run->Ops = MoveTemp(Ops);
		Run->Shared = Shared;
		const FString Label = Name;
		ADD_LATENT_AUTOMATION_COMMAND(FScenarioCommand(*this, FVector(1500.f, 2500.f, StandZ), -135.f, 900.f,
			[Run](ASupernaturalProtCharacter& Character)
			{
				Character.GetSpnMovement()->SetBaseGait(ESpnGait::Walk);   // a careful approach
				Run->Character = &Character;
			},
			[this, Run](ASupernaturalProtCharacter& Character, float, double Elapsed) { return StepRun(*this, *Run, Character, Elapsed); },
			[this, Run, Label](ASupernaturalProtCharacter& Character, bool bTimedOut)
			{
				Run->Keys.ReleaseAll(Character);
				UGameplayStatics::SetGlobalTimeDilation(Character.GetWorld(), 1.f);
				if (!Run->Failure.IsEmpty() || bTimedOut)
				{
					SpnQa::Check(*this, &Character, Suite, FString::Printf(TEXT("%s_run"), *Label), SpnQa::EStatus::Blocked, Run->Failure.IsEmpty() ? FString(TEXT("timeout")) : Run->Failure);
					return;
				}
				SpnQa::Check(*this, &Character, Suite, FString::Printf(TEXT("%s_run"), *Label), SpnQa::EStatus::Pass, FString::Printf(TEXT("all %d steps ran"), Run->Ops.Num()));
			}));
	};

	AutomationOpenMap(NpcMap);
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(2.5f));   // the navigation mesh is built when the level starts
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([]
	{
		if (const UWorld* World = FindPlayWorld()) { if (APlayerController* PC = World->GetFirstPlayerController()) { SpnTestPlay::ResetWorldState(*PC); } }
		return true;
	}));
	MakeHalf(BuildFirstHalf(), TEXT("first_half"));

	// the live state is cleared, the level reopened; only the save file survives
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([]
	{
		if (const UWorld* World = FindPlayWorld())
		{
			if (APlayerController* PC = World->GetFirstPlayerController()) { SpnTestPlay::ResetWorldState(*PC); }
			if (ASupernaturalProtCharacter* Character = FindCharacter())
			{
				if (USpnNpcMemorySubsystem* Memory = Character->GetGameInstance()->GetSubsystem<USpnNpcMemorySubsystem>()) { Memory->ResetAll(); }
			}
		}
		return true;
	}));
	// A REAL reload: AutomationOpenMap does nothing when the map is already loaded (the first version of this test did not notice that), so the level is opened
	// through the console. All actors are new, the game instance and its subsystems stay (as in a level change); the live memory was emptied above.
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([]
	{
		if (const UWorld* World = FindPlayWorld()) { if (APlayerController* PC = World->GetFirstPlayerController()) { PC->ConsoleCommand(TEXT("open /Game/SpnTest/Lvl_SpnNpcTest"), /*bWriteToLog*/ true); } }
		return true;
	}));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(5.f));
	MakeHalf(BuildSecondHalf(), TEXT("second_half"));
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([]
	{
		if (ASupernaturalProtCharacter* Character = FindCharacter())
		{
			if (USpnSaveSubsystem* Save = Character->GetGameInstance()->GetSubsystem<USpnSaveSubsystem>())
			{
				IFileManager::Get().DeleteDirectory(*FPaths::GetPath(Save->GetSlotFilePath(NpcSlot)), false, true);   // the QA slot is removed again
				Save->ResetSaveDirectory();
			}
		}
		return true;
	}));
	return true;
}

#endif
