#include "Engine/GameInstance.h"
#include "HAL/FileManager.h"
#include "Misc/AutomationTest.h"
#include "Misc/FileHelper.h"
#include "Misc/Guid.h"
#include "Misc/Paths.h"
#include "SpnNpcMemorySubsystem.h"
#include "SpnNpcReaction.h"
#include "SpnSaveSubsystem.h"
#include "SpnWorldStateSubsystem.h"
#include "UObject/Package.h"

#include <spn/core/SaveSlot.h>

#include <limits>
#include <string>

#if WITH_DEV_AUTOMATION_TESTS

// NightBuild05: the NPC memory subsystem, the noise channel and the integration of the NPC memory into the unified save slot ("world.npcs").
// Every test works in its own temporary save root under Saved/SpnTestTemp/<guid>; no real slot is touched.

namespace
{
	constexpr EAutomationTestFlags Flags = EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter;
	const TCHAR* const QaSlot = TEXT("slot_npc");
	constexpr int64 Caretaker = 1001;
	constexpr int64 Neighbour = 1002;

	struct FEnv
	{
		UGameInstance* GameInstance = nullptr;
		FString Root;

		FEnv()
		{
			GameInstance = NewObject<UGameInstance>(GetTransientPackage());
			GameInstance->AddToRoot();
			GameInstance->Init();
			Root = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir() / TEXT("SpnTestTemp") / FGuid::NewGuid().ToString(EGuidFormats::Digits));
			Save().SetSaveDirectory(Root);
		}

		~FEnv()
		{
			GameInstance->Shutdown();
			GameInstance->RemoveFromRoot();
			IFileManager::Get().DeleteDirectory(*Root, /*RequireExists*/ false, /*Tree*/ true);
		}

		USpnNpcMemorySubsystem& Memory() const { return *GameInstance->GetSubsystem<USpnNpcMemorySubsystem>(); }
		USpnNoiseSubsystem& Noise() const { return *GameInstance->GetSubsystem<USpnNoiseSubsystem>(); }
		USpnSaveSubsystem& Save() const { return *GameInstance->GetSubsystem<USpnSaveSubsystem>(); }
		USpnWorldStateSubsystem& World() const { return *GameInstance->GetSubsystem<USpnWorldStateSubsystem>(); }
		FString File() const { return Save().GetSlotFilePath(QaSlot); }
	};

	FSpnIncident DoorBroken(const TCHAR* Door = TEXT("placeholder.door.back_door"), float Severity = 0.9f)
	{
		FSpnIncident I;
		I.Kind = ESpnIncidentKind::HeardDoorBroken;
		I.LocationCm = FVector(450.f, 900.f, 0.f);
		I.Severity = Severity;
		I.ObjectId = Door;
		I.Accuracy = 0.8f;
		return I;
	}

	FSpnIncident Alarm()
	{
		FSpnIncident I;
		I.Kind = ESpnIncidentKind::Alarmed;
		I.LocationCm = FVector(500.f, 850.f, 0.f);
		I.Severity = 0.8f;
		return I;
	}

	spn::core::SaveSlot ParsedSlot(const FString& Path)
	{
		TArray<uint8> Bytes;
		FFileHelper::LoadFileToArray(Bytes, *Path, FILEREAD_Silent);
		return spn::core::SaveSlot::Parse(std::string(reinterpret_cast<const char*>(Bytes.GetData()), static_cast<std::size_t>(Bytes.Num()))).Value();
	}

	void WriteSlot(const FString& Path, const spn::core::SaveSlot& Container)
	{
		const std::string Text = Container.Serialize();
		TArray<uint8> Bytes;
		Bytes.Append(reinterpret_cast<const uint8*>(Text.data()), static_cast<int32>(Text.size()));
		FFileHelper::SaveArrayToFile(Bytes, *Path);
	}

	spn::core::SaveSection NpcSection(const std::string& Payload, uint32 Version = 1)
	{
		spn::core::SaveSection Section;
		Section.id = "world.npcs";
		Section.version = Version;
		Section.required = false;
		Section.payload = Payload;
		return Section;
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNpcMemoryBasicsTest, "SpnCore.NpcMemory.Basics", Flags)
bool FSpnNpcMemoryBasicsTest::RunTest(const FString&)
{
	FEnv Env;
	USpnNpcMemorySubsystem& M = Env.Memory();
	TestTrue(TEXT("registering works"), M.RegisterNpc(Caretaker, ESpnNpcTier::B, TEXT("Caretaker")));
	TestTrue(TEXT("registering twice is fine"), M.RegisterNpc(Caretaker, ESpnNpcTier::B, TEXT("Caretaker")));
	TestEqual(TEXT("and still one NPC"), M.GetNpcCount(), 1);
	TestFalse(TEXT("id 0 is invalid"), M.RegisterNpc(0, ESpnNpcTier::C, TEXT("x")));
	TestFalse(TEXT("negative id is invalid"), M.RegisterNpc(-5, ESpnNpcTier::C, TEXT("x")));
	TestFalse(TEXT("the player's id is not an NPC"), M.RegisterNpc(USpnNpcMemorySubsystem::PlayerEntityId, ESpnNpcTier::C, TEXT("x")));
	TestFalse(TEXT("ids >= 2^62 are reserved"), M.RegisterNpc(1ll << 62, ESpnNpcTier::C, TEXT("x")));
	TestFalse(TEXT("an unknown NPC remembers nothing"), M.Remember(777, DoorBroken()));
	TestFalse(TEXT("an unknown NPC is neutral"), M.Assess(777).bWary);

	TestTrue(TEXT("remember a door break"), M.Remember(Caretaker, DoorBroken()));
	const FSpnWariness W = M.Assess(Caretaker);
	TestTrue(TEXT("wary"), W.bWary);
	TestTrue(TEXT("remembers the door"), W.bBrokeInRemembered);
	TestTrue(TEXT("knows where"), W.bHasLastIncident && FVector::Dist2D(W.LastIncidentCm, FVector(450.f, 900.f, 0.f)) < 1.f);
	TestFalse(TEXT("did not see the player"), W.bPlayerSeenRemembered);
	const TArray<FSpnMemoryView> Views = M.GetMemories(Caretaker);
	TestEqual(TEXT("one memory"), Views.Num(), 1);
	if (Views.Num() == 1)
	{
		TestEqual(TEXT("it is a witnessed event"), Views[0].Kind, FString(TEXT("witnessed")));
		TestTrue(TEXT("about an object"), Views[0].bObjectSubject);
	}
	TestFalse(TEXT("the other NPC is not affected"), M.Assess(Neighbour).bWary);

	// time: the clock only moves forward
	const double T0 = M.GetWorldTime();
	M.AdvanceWorldTime(5.0);
	M.AdvanceWorldTime(-3.0);
	M.AdvanceWorldTime(std::numeric_limits<double>::quiet_NaN());
	TestTrue(TEXT("clock advanced by exactly the valid step"), FMath::IsNearlyEqual(M.GetWorldTime() - T0, 5.0));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNpcMemoryBoundedTest, "SpnCore.NpcMemory.BoundedRepeatedEvents", Flags)
bool FSpnNpcMemoryBoundedTest::RunTest(const FString&)
{
	FEnv Env;
	USpnNpcMemorySubsystem& M = Env.Memory();
	M.RegisterNpc(Caretaker, ESpnNpcTier::C, TEXT("Resident"));
	for (int32 Index = 0; Index < 500; ++Index)
	{
		M.AdvanceWorldTime(0.5);
		M.Remember(Caretaker, DoorBroken());
	}
	TestTrue(TEXT("tier C keeps at most 4 memories"), M.GetMemoryCount(Caretaker) <= 4);
	TestTrue(TEXT("and stays wary"), M.Assess(Caretaker).bWary);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNoiseChannelTest, "SpnCore.NpcMemory.NoiseChannel", Flags)
bool FSpnNoiseChannelTest::RunTest(const FString&)
{
	FEnv Env;
	USpnNoiseSubsystem& N = Env.Noise();
	int32 Heard = 0;
	FSpnNoise Last;
	N.OnNoise.AddLambda([&](const FSpnNoise& Noise) { ++Heard; Last = Noise; });

	FSpnNoise Noise;
	Noise.LocationCm = FVector(100.f, 200.f, 30.f);
	Noise.RangeCm = 2000.f;
	Noise.Strength = 1.f;
	Noise.Kind = ESpnNoiseKind::DoorBreak;
	Noise.ObjectId = TEXT("placeholder.door.back_door");
	Noise.SourceId = TEXT("player");
	const uint64 Seq1 = N.EmitNoise(Noise);
	TestEqual(TEXT("the first noise has sequence 1"), Seq1, 1ull);
	TestEqual(TEXT("it was delivered once"), Heard, 1);
	TestTrue(TEXT("with its position (cm round trip)"), Last.LocationCm.Equals(Noise.LocationCm, 0.5f));
	TestEqual(TEXT("kind"), Last.Kind, ESpnNoiseKind::DoorBreak);
	TestEqual(TEXT("object"), Last.ObjectId, Noise.ObjectId);
	TestTrue(TEXT("range"), FMath::IsNearlyEqual(Last.RangeCm, 2000.f, 0.5f));
	TestEqual(TEXT("second noise: sequence 2"), N.EmitNoise(Noise), 2ull);

	FSpnNoise Bad = Noise;
	Bad.RangeCm = 0.f;
	TestEqual(TEXT("zero range is dropped"), N.EmitNoise(Bad), 0ull);
	Bad = Noise;
	Bad.Strength = -1.f;
	TestEqual(TEXT("negative strength is dropped"), N.EmitNoise(Bad), 0ull);
	Bad = Noise;
	Bad.LocationCm.X = std::numeric_limits<double>::quiet_NaN();
	TestEqual(TEXT("NaN position is dropped"), N.EmitNoise(Bad), 0ull);
	Bad = Noise;
	Bad.Strength = 7.f;
	N.EmitNoise(Bad);
	TestTrue(TEXT("strength is clamped to 1"), Last.Strength <= 1.f);
	TestEqual(TEXT("three delivered"), Heard, 3);
	TestEqual(TEXT("three dropped"), static_cast<int32>(N.GetDroppedCount()), 3);

	// classification
	TestTrue(TEXT("door break is suspicious"), SpnNpcSense::IsSuspicious(Noise));
	FSpnNoise Step = Noise;
	Step.Kind = ESpnNoiseKind::Footstep;
	Step.Strength = 0.3f;
	TestFalse(TEXT("a footstep is harmless"), SpnNpcSense::IsSuspicious(Step));
	// hearing
	float D, R;
	TestTrue(TEXT("heard at 10 m in the open"), SpnNpcSense::CanHear(Noise, Noise.LocationCm + FVector(1000.f, 0.f, 0.f), 0, 1.f, 0.5f, D, R));
	TestFalse(TEXT("not at 10 m behind two walls"), SpnNpcSense::CanHear(Noise, Noise.LocationCm + FVector(1000.f, 0.f, 0.f), 2, 1.f, 0.5f, D, R));
	TestTrue(TEXT("effective range is 5 m behind two walls"), FMath::IsNearlyEqual(R, 500.f, 1.f));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNpcSightTest, "SpnCore.NpcMemory.SightVerdict", Flags)
bool FSpnNpcSightTest::RunTest(const FString&)
{
	const FVector Eye(0.f, 0.f, 70.f);
	TestTrue(TEXT("clear, in front"), SpnNpcSense::CanSee(Eye, 0.f, FVector(800.f, 0.f, 70.f), true, 1800.f, 120.f, 150.f));
	TestFalse(TEXT("blocked line"), SpnNpcSense::CanSee(Eye, 0.f, FVector(800.f, 0.f, 70.f), false, 1800.f, 120.f, 150.f));
	TestFalse(TEXT("blocked even when very close"), SpnNpcSense::CanSee(Eye, 0.f, FVector(60.f, 0.f, 70.f), false, 1800.f, 120.f, 150.f));
	TestFalse(TEXT("behind"), SpnNpcSense::CanSee(Eye, 0.f, FVector(-800.f, 0.f, 70.f), true, 1800.f, 120.f, 150.f));
	TestFalse(TEXT("too far"), SpnNpcSense::CanSee(Eye, 0.f, FVector(2500.f, 0.f, 70.f), true, 1800.f, 120.f, 150.f));
	TestTrue(TEXT("yaw 90 looks along +Y"), SpnNpcSense::CanSee(Eye, 90.f, FVector(0.f, 800.f, 70.f), true, 1800.f, 120.f, 150.f));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNpcMemorySaveLoadTest, "SpnCore.NpcMemory.SaveLoadRoundTrip", Flags)
bool FSpnNpcMemorySaveLoadTest::RunTest(const FString&)
{
	FEnv Env;
	USpnNpcMemorySubsystem& M = Env.Memory();
	M.RegisterNpc(Caretaker, ESpnNpcTier::B, TEXT("Caretaker"));
	M.RegisterNpc(Neighbour, ESpnNpcTier::C, TEXT("Neighbour"));
	M.AdvanceWorldTime(100.0);
	M.Remember(Caretaker, DoorBroken());
	M.AdvanceWorldTime(4.0);
	M.Remember(Caretaker, Alarm());
	M.AdvanceWorldTime(10.0);
	const double SavedTime = M.GetWorldTime();
	Env.World().SetFlag(USpnWorldStateSubsystem::TestDoorBrokenKey(TEXT("back_door")), true, TEXT("test"));

	const FSpnSaveStatus Saved = Env.Save().SaveSlot(QaSlot);
	TestTrue(TEXT("saved"), Saved.IsOk());
	const FString Before = M.SerializeState();   // after the save: the save brought the store clock up to the game time

	// a different state in memory, then load
	M.ResetAll();
	TestEqual(TEXT("memory emptied"), M.GetNpcCount(), 0);
	int32 Replaced = 0;
	M.OnReplaced.AddLambda([&Replaced]() { ++Replaced; });
	const FSpnSaveStatus Loaded = Env.Save().LoadSlot(QaSlot);
	TestTrue(TEXT("loaded"), Loaded.IsOk());
	TestTrue(TEXT("NPC memory restored"), Loaded.bNpcMemoryRestored);
	TestFalse(TEXT("not reset"), Loaded.bNpcMemoryResetToEmpty);
	TestEqual(TEXT("two NPCs back"), Loaded.NpcCount, 2);
	TestEqual(TEXT("listeners were told once"), Replaced, 1);
	TestTrue(TEXT("both NPCs known"), M.IsNpcKnown(Caretaker) && M.IsNpcKnown(Neighbour));
	TestEqual(TEXT("exactly the saved state"), M.SerializeState(), Before);
	TestTrue(TEXT("the clock continues from the saved time"), FMath::IsNearlyEqual(M.GetWorldTime(), SavedTime, 0.01));
	const FSpnWariness W = M.Assess(Caretaker);
	TestTrue(TEXT("still wary"), W.bWary && W.bBrokeInRemembered);
	TestTrue(TEXT("the alarm is still recent (volatile alarm state is re-derived from it)"), W.bRecentAlarm);
	ESpnReactionState State;
	FVector Target;
	M.GetInitialReaction(Caretaker, State, Target);
	TestEqual(TEXT("so the NPC resumes by investigating"), State, ESpnReactionState::Investigate);
	TestTrue(TEXT("the last incident"), FVector::Dist2D(Target, FVector(500.f, 850.f, 0.f)) < 5.f);
	M.GetInitialReaction(Neighbour, State, Target);
	TestEqual(TEXT("the neighbour saw nothing: Idle"), State, ESpnReactionState::Idle);
	TestEqual(TEXT("door facts load too"), Env.World().ReadFlag(USpnWorldStateSubsystem::TestDoorBrokenKey(TEXT("back_door"))).Get(false), true);

	// the alarm is only "going on" for a while; the memory itself stays
	M.AdvanceWorldTime(500.0);
	M.GetInitialReaction(Caretaker, State, Target);
	TestEqual(TEXT("much later: Idle again"), State, ESpnReactionState::Idle);
	TestTrue(TEXT("but still wary"), M.Assess(Caretaker).bWary);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNpcMemoryRepeatedSaveTest, "SpnCore.NpcMemory.RepeatedSaveLoadCycles", Flags)
bool FSpnNpcMemoryRepeatedSaveTest::RunTest(const FString&)
{
	FEnv Env;
	USpnNpcMemorySubsystem& M = Env.Memory();
	M.RegisterNpc(Caretaker, ESpnNpcTier::A, TEXT("Caretaker"));
	for (int32 Cycle = 0; Cycle < 12; ++Cycle)
	{
		M.AdvanceWorldTime(2.0);
		M.Remember(Caretaker, Cycle % 3 == 0 ? Alarm() : DoorBroken());
		if (!Env.Save().SaveSlot(QaSlot).IsOk())
		{
			AddError(FString::Printf(TEXT("cycle %d: save failed"), Cycle));
			return false;
		}
		const FString Before = M.SerializeState();
		if (!Env.Save().LoadSlot(QaSlot).IsOk() || M.SerializeState() != Before)
		{
			AddError(FString::Printf(TEXT("cycle %d: load did not reproduce the saved state"), Cycle));
			return false;
		}
	}
	TestEqual(TEXT("twelve memories, none lost, none duplicated"), M.GetMemoryCount(Caretaker), 12);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNpcMemoryOldSlotTest, "SpnCore.NpcMemory.SlotWithoutNpcSection", Flags)
bool FSpnNpcMemoryOldSlotTest::RunTest(const FString&)
{
	FEnv Env;
	// a save from before NightBuild05 (or from a game where no NPC had registered): no world.npcs
	Env.World().SetFlag(USpnWorldStateSubsystem::TestDoorOpenKey(TEXT("front_door")), true, TEXT("test"));
	TestTrue(TEXT("saved without NPCs"), Env.Save().SaveSlot(QaSlot).IsOk());
	TestTrue(TEXT("no NPC section was written for an empty memory"), ParsedSlot(Env.File()).Find("world.npcs") == nullptr);

	Env.Memory().RegisterNpc(Caretaker, ESpnNpcTier::B, TEXT("Caretaker"));
	Env.Memory().Remember(Caretaker, DoorBroken());
	int32 Replaced = 0;
	Env.Memory().OnReplaced.AddLambda([&Replaced]() { ++Replaced; });
	const FSpnSaveStatus Loaded = Env.Save().LoadSlot(QaSlot);
	TestTrue(TEXT("the old slot loads"), Loaded.IsOk());
	TestFalse(TEXT("nothing restored"), Loaded.bNpcMemoryRestored);
	TestTrue(TEXT("the memory was emptied and says so"), Loaded.bNpcMemoryResetToEmpty);
	TestFalse(TEXT("not a damage"), Loaded.bNpcMemoryDiscarded);
	TestEqual(TEXT("no NPC known"), Env.Memory().GetNpcCount(), 0);
	TestEqual(TEXT("NPCs were told to re-register"), Replaced, 1);
	TestEqual(TEXT("the door state of the old slot is intact"), Env.World().ReadFlag(USpnWorldStateSubsystem::TestDoorOpenKey(TEXT("front_door"))).Get(false), true);

	// the NPCs register again and a new save carries them
	Env.Memory().RegisterNpc(Caretaker, ESpnNpcTier::B, TEXT("Caretaker"));
	TestTrue(TEXT("re-saved"), Env.Save().SaveSlot(QaSlot).IsOk());
	TestTrue(TEXT("now with the section"), ParsedSlot(Env.File()).Find("world.npcs") != nullptr);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnNpcMemoryDamagedTest, "SpnCore.NpcMemory.DamagedNpcSection", Flags)
bool FSpnNpcMemoryDamagedTest::RunTest(const FString&)
{
	FEnv Env;
	USpnNpcMemorySubsystem& M = Env.Memory();
	M.RegisterNpc(Caretaker, ESpnNpcTier::B, TEXT("Caretaker"));
	M.Remember(Caretaker, DoorBroken());
	Env.World().SetFlag(USpnWorldStateSubsystem::TestDoorOpenKey(TEXT("front_door")), true, TEXT("test"));
	TestTrue(TEXT("saved"), Env.Save().SaveSlot(QaSlot).IsOk());

	// garbage that passes the container checksum but not the NPC parser
	spn::core::SaveSlot Damaged = ParsedSlot(Env.File());
	Damaged.Remove("world.npcs");
	TestFalse(TEXT("put garbage"), Damaged.Put(NpcSection("this is not an npc memory\n")).has_value());
	WriteSlot(Env.File(), Damaged);

	Env.World().SetFlag(USpnWorldStateSubsystem::TestDoorOpenKey(TEXT("front_door")), false, TEXT("test"));
	TestEqual(TEXT("the slot list still calls the slot loadable"), Env.Save().InspectSlot(QaSlot).Result, ESpnSaveResult::Ok);
	const FSpnSaveStatus Loaded = Env.Save().LoadSlot(QaSlot);
	TestTrue(TEXT("the rest of the slot loads"), Loaded.IsOk());
	TestTrue(TEXT("the damaged NPC section was discarded and reported"), Loaded.bNpcMemoryDiscarded);
	TestFalse(TEXT("with a reason"), Loaded.NpcMemoryDetail.IsEmpty());
	TestEqual(TEXT("the memory is empty, not half loaded"), M.GetNpcCount(), 0);
	TestEqual(TEXT("the door state of the same slot was loaded"), Env.World().ReadFlag(USpnWorldStateSubsystem::TestDoorOpenKey(TEXT("front_door"))).Get(false), true);
	// a damaged section of the right shape but with a bad value
	M.RegisterNpc(Caretaker, ESpnNpcTier::B, TEXT("Caretaker"));
	const FString Good = M.SerializeState();
	spn::core::SaveSlot Truncated = ParsedSlot(Env.File());
	Truncated.Remove("world.npcs");
	TestFalse(TEXT("put truncated"), Truncated.Put(NpcSection(std::string(TCHAR_TO_UTF8(*Good.Left(Good.Len() / 2))))).has_value());
	WriteSlot(Env.File(), Truncated);
	const FSpnSaveStatus Loaded2 = Env.Save().LoadSlot(QaSlot);
	TestTrue(TEXT("a truncated NPC section is survivable too"), Loaded2.IsOk() && Loaded2.bNpcMemoryDiscarded);

	// a section from a NEWER build is never overwritten or half-read
	spn::core::SaveSlot Newer = ParsedSlot(Env.File());
	Newer.Remove("world.npcs");
	TestFalse(TEXT("put newer"), Newer.Put(NpcSection("SPN-NPCMEM 2\n", 2)).has_value());
	WriteSlot(Env.File(), Newer);
	TestEqual(TEXT("loading refuses it"), Env.Save().LoadSlot(QaSlot).Result, ESpnSaveResult::UnsupportedVersion);
	TestEqual(TEXT("saving over it refuses too"), Env.Save().SaveSlot(QaSlot).Result, ESpnSaveResult::UnsupportedVersion);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
