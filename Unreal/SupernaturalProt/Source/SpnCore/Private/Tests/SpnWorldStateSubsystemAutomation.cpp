#include "Engine/GameInstance.h"
#include "HAL/FileManager.h"
#include "Misc/AutomationTest.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "SpnWorldStateSubsystem.h"
#include "UObject/Package.h"

#if WITH_DEV_AUTOMATION_TESTS

// Unreal checks for USpnWorldStateSubsystem (Docs/Proposals/0004, section 5.2). Each test
// uses its own slot under Saved/SpnSaves/spn_automation_* and deletes only that slot.

namespace
{
	constexpr EAutomationTestFlags SpnWorldStateTestFlags =
		EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter;

	const TCHAR* const DoorA = TEXT("door_a");
	const TCHAR* const DoorB = TEXT("door_b");

	/** A game instance that lives for one scope, with its subsystems initialized. */
	struct FScopedGameInstance
	{
		UGameInstance* GameInstance = nullptr;

		FScopedGameInstance()
		{
			GameInstance = NewObject<UGameInstance>(GetTransientPackage());
			GameInstance->AddToRoot();
			GameInstance->Init();
		}

		~FScopedGameInstance()
		{
			GameInstance->Shutdown();
			GameInstance->RemoveFromRoot();
		}

		USpnWorldStateSubsystem* WorldState() const { return GameInstance->GetSubsystem<USpnWorldStateSubsystem>(); }
	};

	/** Deletes the test slot's directory before and after a test. */
	struct FScopedTestSlot
	{
		FString Name;

		explicit FScopedTestSlot(const TCHAR* InName) : Name(InName) { Delete(); }
		~FScopedTestSlot() { Delete(); }

		FString File() const { return USpnWorldStateSubsystem::GetSlotFilePath(Name); }

		void Delete() const
		{
			const FString File = USpnWorldStateSubsystem::GetSlotFilePath(Name);
			if (!File.IsEmpty())
			{
				IFileManager::Get().DeleteDirectory(*FPaths::GetPath(File), /*RequireExists*/ false, /*Tree*/ true);
			}
		}
	};

	TMap<FString, bool> DoorDefaults(bool bOpen, bool bLocked)
	{
		return {{TEXT("open"), bOpen}, {TEXT("locked"), bLocked}};
	}

	FSpnInteractStatus UseDoor(USpnWorldStateSubsystem& WorldState, const TCHAR* Id, bool bLocked = false)
	{
		return WorldState.TryInteract(USpnWorldStateSubsystem::TestDoorKind(), Id, ESpnInteractionVerb::Use,
			DoorDefaults(false, bLocked), TEXT("automation.player"));
	}

	bool ReadFileBytes(const FString& Path, TArray<uint8>& OutBytes)
	{
		return FFileHelper::LoadFileToArray(OutBytes, *Path, FILEREAD_Silent);
	}

	bool WriteFileBytes(const FString& Path, const TArray<uint8>& Bytes)
	{
		return FFileHelper::SaveArrayToFile(Bytes, *Path);
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnWorldStateInitializeTest, "SpnCore.WorldState.Initialize", SpnWorldStateTestFlags)

bool FSpnWorldStateInitializeTest::RunTest(const FString& Parameters)
{
	FScopedGameInstance Instance;
	USpnWorldStateSubsystem* WorldState = Instance.WorldState();
	if (!TestNotNull(TEXT("game instance creates the world state subsystem"), WorldState))
	{
		return false;
	}

	TestEqual(TEXT("fresh store is empty"), WorldState->GetRevision(), uint64{0});
	TestFalse(TEXT("untouched door has no fact"), WorldState->ReadFlag(USpnWorldStateSubsystem::TestDoorOpenKey(DoorA)).IsSet());

	const FSpnInteractStatus Opened = UseDoor(*WorldState, DoorA);
	TestEqual(TEXT("Use opens the test door"), Opened.Result, ESpnInteractResult::Changed);
	TestEqual(TEXT("one journaled change"), WorldState->GetRevision(), uint64{1});
	TestEqual(TEXT("door fact is written"), WorldState->ReadFlag(USpnWorldStateSubsystem::TestDoorOpenKey(DoorA)), TOptional<bool>(true));

	const FSpnInteractStatus Refused = UseDoor(*WorldState, DoorB, /*bLocked*/ true);
	TestEqual(TEXT("locked door refuses"), Refused.Result, ESpnInteractResult::Refused);
	TestEqual(TEXT("refusal names the locked feedback"), Refused.Feedback, FString(TEXT("placeholder:feedback.door.locked")));
	TestEqual(TEXT("refusal writes nothing"), WorldState->GetRevision(), uint64{1});

	TestEqual(TEXT("invalid stable id"), UseDoor(*WorldState, TEXT("Door A")).Result, ESpnInteractResult::InvalidId);
	TestEqual(TEXT("unregistered kind"),
		WorldState->TryInteract(TEXT("mod_x:interactable.chest"), DoorA, ESpnInteractionVerb::Use, {}, TEXT("automation.player")).Result,
		ESpnInteractResult::UnknownKind);
	TestEqual(TEXT("malformed kind"),
		WorldState->TryInteract(TEXT("not a kind"), DoorA, ESpnInteractionVerb::Use, {}, TEXT("automation.player")).Result,
		ESpnInteractResult::UnknownKind);
	TestEqual(TEXT("errors write nothing"), WorldState->GetRevision(), uint64{1});

	// Each game instance owns its own store.
	FScopedGameInstance Other;
	TestEqual(TEXT("a second game instance starts empty"), Other.WorldState()->GetRevision(), uint64{0});
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnWorldStateSaveLoadTest, "SpnCore.WorldState.SaveAndLoadSlot", SpnWorldStateTestFlags)

bool FSpnWorldStateSaveLoadTest::RunTest(const FString& Parameters)
{
	const FScopedTestSlot Slot(TEXT("spn_automation_save_load"));
	const FString OpenA = USpnWorldStateSubsystem::TestDoorOpenKey(DoorA);
	const FString OpenB = USpnWorldStateSubsystem::TestDoorOpenKey(DoorB);

	TArray<uint8> SavedBytes;
	{
		FScopedGameInstance Instance;
		USpnWorldStateSubsystem& WorldState = *Instance.WorldState();
		UseDoor(WorldState, DoorA); // open
		UseDoor(WorldState, DoorB); // open
		UseDoor(WorldState, DoorB); // closed again
		TestEqual(TEXT("three changes before saving"), WorldState.GetRevision(), uint64{3});

		const FSpnSlotStatus Saved = WorldState.SaveToSlot(Slot.Name);
		TestTrue(FString::Printf(TEXT("save succeeds (%s)"), LexToString(Saved.Result)), Saved.IsOk());
		TestTrue(TEXT("slot file exists"), FPaths::FileExists(Slot.File()));
		TestFalse(TEXT("no temporary file is left behind"), FPaths::FileExists(Slot.File() + TEXT(".tmp")));
		TestTrue(TEXT("slot file is readable"), ReadFileBytes(Slot.File(), SavedBytes));
		const FString Header = TEXT("SPNFACTS 1\n");
		TestTrue(TEXT("slot file is WorldFacts format v1"),
			SavedBytes.Num() > Header.Len() && FMemory::Memcmp(SavedBytes.GetData(), TCHAR_TO_ANSI(*Header), Header.Len()) == 0);
	}

	{
		FScopedGameInstance Instance;
		USpnWorldStateSubsystem& WorldState = *Instance.WorldState();
		TestEqual(TEXT("new game instance starts empty"), WorldState.GetRevision(), uint64{0});

		int32 Broadcasts = 0;
		WorldState.OnWorldStateReplaced.AddLambda([&Broadcasts] { ++Broadcasts; });

		const FSpnSlotStatus Loaded = WorldState.LoadFromSlot(Slot.Name);
		TestTrue(FString::Printf(TEXT("load succeeds (%s)"), LexToString(Loaded.Result)), Loaded.IsOk());
		TestEqual(TEXT("load broadcasts once"), Broadcasts, 1);
		TestEqual(TEXT("journal restored"), WorldState.GetRevision(), uint64{3});
		TestEqual(TEXT("door_a restored open"), WorldState.ReadFlag(OpenA), TOptional<bool>(true));
		TestEqual(TEXT("door_b restored closed"), WorldState.ReadFlag(OpenB), TOptional<bool>(false));

		// Saving the loaded state again gives the same bytes.
		TestTrue(TEXT("re-save succeeds"), WorldState.SaveToSlot(Slot.Name).IsOk());
		TArray<uint8> ResavedBytes;
		TestTrue(TEXT("re-saved file is readable"), ReadFileBytes(Slot.File(), ResavedBytes));
		TestTrue(TEXT("re-save is byte-identical"), ResavedBytes == SavedBytes);

		// Playing on and saving over the slot replaces the old file.
		TestEqual(TEXT("door_a closes after the load"), UseDoor(WorldState, DoorA).Result, ESpnInteractResult::Changed);
		TestTrue(TEXT("overwrite succeeds"), WorldState.SaveToSlot(Slot.Name).IsOk());
		TestFalse(TEXT("no temporary file after overwrite"), FPaths::FileExists(Slot.File() + TEXT(".tmp")));
	}

	{
		FScopedGameInstance Instance;
		USpnWorldStateSubsystem& WorldState = *Instance.WorldState();
		TestTrue(TEXT("overwritten slot loads"), WorldState.LoadFromSlot(Slot.Name).IsOk());
		TestEqual(TEXT("overwritten slot has the new revision"), WorldState.GetRevision(), uint64{4});
		TestEqual(TEXT("overwritten slot has door_a closed"), WorldState.ReadFlag(OpenA), TOptional<bool>(false));
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnWorldStateFailedLoadTest, "SpnCore.WorldState.FailedLoadKeepsState", SpnWorldStateTestFlags)

bool FSpnWorldStateFailedLoadTest::RunTest(const FString& Parameters)
{
	const FScopedTestSlot Slot(TEXT("spn_automation_failed_load"));
	const FScopedTestSlot Missing(TEXT("spn_automation_missing"));
	const FString OpenA = USpnWorldStateSubsystem::TestDoorOpenKey(DoorA);
	const FString OpenB = USpnWorldStateSubsystem::TestDoorOpenKey(DoorB);

	// A valid save with door_b open, door_a untouched.
	TArray<uint8> ValidBytes;
	{
		FScopedGameInstance Instance;
		UseDoor(*Instance.WorldState(), DoorB);
		if (!TestTrue(TEXT("valid save written"), Instance.WorldState()->SaveToSlot(Slot.Name).IsOk())
			|| !TestTrue(TEXT("valid save readable"), ReadFileBytes(Slot.File(), ValidBytes)))
		{
			return false;
		}
	}

	// The live state the failed loads must not touch: door_a open, door_b untouched.
	FScopedGameInstance Instance;
	USpnWorldStateSubsystem& WorldState = *Instance.WorldState();
	UseDoor(WorldState, DoorA);
	int32 Broadcasts = 0;
	WorldState.OnWorldStateReplaced.AddLambda([&Broadcasts] { ++Broadcasts; });

	auto ExpectStateKept = [&](const FString& What)
	{
		TestEqual(What + TEXT(": revision kept"), WorldState.GetRevision(), uint64{1});
		TestEqual(What + TEXT(": door_a still open"), WorldState.ReadFlag(OpenA), TOptional<bool>(true));
		TestFalse(What + TEXT(": door_b still untouched"), WorldState.ReadFlag(OpenB).IsSet());
		TestEqual(What + TEXT(": no replace broadcast"), Broadcasts, 0);
	};

	TestEqual(TEXT("missing slot"), WorldState.LoadFromSlot(Missing.Name).Result, ESpnSlotResult::SlotNotFound);
	ExpectStateKept(TEXT("missing slot"));

	TestEqual(TEXT("invalid slot name"), WorldState.LoadFromSlot(TEXT("../spn_automation_failed_load")).Result, ESpnSlotResult::InvalidSlotName);
	ExpectStateKept(TEXT("invalid slot name"));

	struct FDamage
	{
		const TCHAR* Name;
		TArray<uint8> Bytes;
		const TCHAR* ExpectedDetail; // nullptr: any core error
	};
	TArray<FDamage> Damages;
	Damages.Add({TEXT("empty file"), {}, nullptr});
	Damages.Add({TEXT("truncated by one byte"), TArray<uint8>(ValidBytes.GetData(), ValidBytes.Num() - 1), nullptr});
	Damages.Add({TEXT("truncated to half"), TArray<uint8>(ValidBytes.GetData(), ValidBytes.Num() / 2), nullptr});
	{
		TArray<uint8> WrongVersion = ValidBytes;
		WrongVersion[9] = '2'; // "SPNFACTS 2"
		Damages.Add({TEXT("unsupported version"), MoveTemp(WrongVersion), TEXT("unsupported version")});
	}
	{
		// Tamper the recorded current value of door_b: the replay no longer matches the journal.
		const FString Text = FString(FUTF8ToTCHAR(reinterpret_cast<const ANSICHAR*>(ValidBytes.GetData()), ValidBytes.Num()));
		// Record "set <key> b:true - b:true <cause>": the field after "-" is the current value.
		const FString Tampered = Text.Replace(TEXT("\t-\tb:true\t"), TEXT("\t-\tb:false\t"));
		const FTCHARToUTF8 TamperedUtf8(*Tampered);
		if (TestNotEqual(TEXT("tamper target present in the valid save"), Tampered, Text))
		{
			Damages.Add({TEXT("tampered value"), TArray<uint8>(reinterpret_cast<const uint8*>(TamperedUtf8.Get()), TamperedUtf8.Length()),
				TEXT("journal mismatch")});
		}
	}
	{
		TArray<uint8> Garbage;
		for (const ANSICHAR Char : "not a save at all\n")
		{
			if (Char != '\0')
			{
				Garbage.Add(static_cast<uint8>(Char));
			}
		}
		Damages.Add({TEXT("garbage"), MoveTemp(Garbage), TEXT("invalid header")});
	}

	// Each rejected file logs one warning; exactly that many are expected.
	AddExpectedMessagePlain(FString::Printf(TEXT("slot '%s' rejected ("), *Slot.Name), ELogVerbosity::Warning,
		EAutomationExpectedMessageFlags::Contains, Damages.Num());
	for (const FDamage& Damage : Damages)
	{
		if (!TestTrue(FString::Printf(TEXT("%s: damaged file written"), Damage.Name), WriteFileBytes(Slot.File(), Damage.Bytes)))
		{
			continue;
		}
		const FSpnSlotStatus Status = WorldState.LoadFromSlot(Slot.Name);
		TestEqual(FString::Printf(TEXT("%s: rejected as corrupt"), Damage.Name), Status.Result, ESpnSlotResult::CorruptData);
		TestFalse(FString::Printf(TEXT("%s: core error named"), Damage.Name), Status.Detail.IsEmpty());
		if (Damage.ExpectedDetail != nullptr)
		{
			TestEqual(FString::Printf(TEXT("%s: core error"), Damage.Name), Status.Detail, FString(Damage.ExpectedDetail));
		}
		ExpectStateKept(Damage.Name);
	}

	// After all failures the valid save still loads and replaces the state.
	TestTrue(TEXT("valid save restored on disk"), WriteFileBytes(Slot.File(), ValidBytes));
	TestTrue(TEXT("valid save loads"), WorldState.LoadFromSlot(Slot.Name).IsOk());
	TestEqual(TEXT("valid load broadcasts once"), Broadcasts, 1);
	TestFalse(TEXT("loaded state: door_a untouched"), WorldState.ReadFlag(OpenA).IsSet());
	TestEqual(TEXT("loaded state: door_b open"), WorldState.ReadFlag(OpenB), TOptional<bool>(true));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnWorldStateSlotNameTest, "SpnCore.WorldState.SlotNames", SpnWorldStateTestFlags)

bool FSpnWorldStateSlotNameTest::RunTest(const FString& Parameters)
{
	TestTrue(TEXT("single letter"), USpnWorldStateSubsystem::IsValidSlotName(TEXT("a")));
	TestTrue(TEXT("letters, digits, underscore"), USpnWorldStateSubsystem::IsValidSlotName(TEXT("slot_01")));
	TestTrue(TEXT("64 characters"), USpnWorldStateSubsystem::IsValidSlotName(FString::ChrN(64, TEXT('a'))));

	const TCHAR* const Invalid[] = {
		TEXT(""), TEXT("Slot"), TEXT("1slot"), TEXT("_slot"), TEXT("slot.1"), TEXT("slot."), TEXT("slot-1"), TEXT("slot 1"),
		TEXT("../slot"), TEXT("slot/sub"), TEXT("slot\\sub"), TEXT("c:slot"), TEXT("sl\u00F6t"),
	};
	for (const TCHAR* Name : Invalid)
	{
		TestFalse(FString::Printf(TEXT("'%s' rejected"), Name), USpnWorldStateSubsystem::IsValidSlotName(Name));
		TestTrue(FString::Printf(TEXT("'%s' has no path"), Name), USpnWorldStateSubsystem::GetSlotFilePath(Name).IsEmpty());
	}
	TestFalse(TEXT("65 characters rejected"), USpnWorldStateSubsystem::IsValidSlotName(FString::ChrN(65, TEXT('a'))));

	const FString Path = USpnWorldStateSubsystem::GetSlotFilePath(TEXT("slot_01"));
	TestTrue(TEXT("slot file lives under Saved/SpnSaves/<slot>/"), Path.EndsWith(TEXT("/SpnSaves/slot_01/worldfacts.spnfacts")));
	TestTrue(TEXT("slot file path is absolute"), !FPaths::IsRelative(Path));

	FScopedGameInstance Instance;
	TestEqual(TEXT("save to an invalid slot is refused"), Instance.WorldState()->SaveToSlot(TEXT("../escape")).Result,
		ESpnSlotResult::InvalidSlotName);
	return true;
}

#endif
