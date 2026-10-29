#include "Engine/GameInstance.h"
#include "HAL/FileManager.h"
#include "Misc/AutomationTest.h"
#include "Misc/FileHelper.h"
#include "Misc/Guid.h"
#include "Misc/Paths.h"
#include "SpnSaveSubsystem.h"
#include "SpnWorldStateSubsystem.h"
#include "UObject/Package.h"

#include <spn/core/SaveSlot.h>

#include <string>

#if WITH_DEV_AUTOMATION_TESTS

// Unreal checks for USpnSaveSubsystem (M2B3.2). Every test works in its own temporary save root under Saved/SpnTestTemp/<guid> (set with
// SetSaveDirectory) and removes it afterwards: no real slot, no user save is read or written.

namespace
{
	constexpr EAutomationTestFlags SpnSaveTestFlags =
		EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter;

	const TCHAR* const DoorA = TEXT("door_a");
	const TCHAR* const DoorB = TEXT("door_b");
	const TCHAR* const TestSlotId = TEXT("slot_one");

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

		USpnWorldStateSubsystem* World() const { return GameInstance->GetSubsystem<USpnWorldStateSubsystem>(); }
		USpnSaveSubsystem* Save() const { return GameInstance->GetSubsystem<USpnSaveSubsystem>(); }
	};

	/** A game instance whose save root is a fresh temporary directory, removed in the destructor. */
	struct FTestEnv
	{
		FScopedGameInstance Instance;
		FString Root;

		FTestEnv()
		{
			Root = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir() / TEXT("SpnTestTemp") / FGuid::NewGuid().ToString(EGuidFormats::Digits));
			Instance.Save()->SetSaveDirectory(Root);
		}

		~FTestEnv() { IFileManager::Get().DeleteDirectory(*Root, /*RequireExists*/ false, /*Tree*/ true); }

		USpnWorldStateSubsystem& World() const { return *Instance.World(); }
		USpnSaveSubsystem& Save() const { return *Instance.Save(); }
		FString File(const TCHAR* SlotId = TestSlotId) const { return Save().GetSlotFilePath(SlotId); }
		FString Prev(const TCHAR* SlotId = TestSlotId) const { return File(SlotId) + TEXT(".prev"); }
	};

	TMap<FString, bool> DoorDefaults() { return {{TEXT("open"), false}, {TEXT("locked"), false}}; }

	FSpnInteractStatus UseDoor(USpnWorldStateSubsystem& World, const TCHAR* Id)
	{
		return World.TryInteract(USpnWorldStateSubsystem::TestDoorKind(), Id, ESpnInteractionVerb::Use, DoorDefaults(), TEXT("automation.player"));
	}

	TOptional<bool> DoorOpen(const USpnWorldStateSubsystem& World, const TCHAR* Id) { return World.ReadFlag(USpnWorldStateSubsystem::TestDoorOpenKey(Id)); }

	bool ReadBytes(const FString& Path, TArray<uint8>& Out) { return FFileHelper::LoadFileToArray(Out, *Path, FILEREAD_Silent); }
	bool WriteBytes(const FString& Path, const TArray<uint8>& Bytes) { return FFileHelper::SaveArrayToFile(Bytes, *Path); }

	TArray<uint8> ToBytes(const std::string& Text)
	{
		TArray<uint8> Bytes;
		Bytes.Append(reinterpret_cast<const uint8*>(Text.data()), static_cast<int32>(Text.size()));
		return Bytes;
	}

	std::string ToText(const TArray<uint8>& Bytes)
	{
		return std::string(reinterpret_cast<const char*>(Bytes.GetData()), static_cast<std::size_t>(Bytes.Num()));
	}

	/** The facts of `World` exactly as a save would store them: save to a probe slot and return the bytes. */
	TArray<uint8> Fingerprint(FTestEnv& Env)
	{
		TArray<uint8> Bytes;
		const FSpnSaveStatus Status = Env.Save().SaveSlot(TEXT("probe"));
		if (Status.IsOk())
		{
			ReadBytes(Env.File(TEXT("probe")), Bytes);
		}
		return Bytes;
	}

	/** A valid container holding the facts of a saved slot plus extra sections / modifications. */
	spn::core::SaveSlot ParsedSlot(const FString& Path)
	{
		TArray<uint8> Bytes;
		ReadBytes(Path, Bytes);
		return spn::core::SaveSlot::Parse(ToText(Bytes)).Value();
	}

	void FlipByte(const FString& Path, int32 FromEnd)
	{
		TArray<uint8> Bytes;
		ReadBytes(Path, Bytes);
		Bytes[Bytes.Num() - 1 - FromEnd] ^= 0x01;
		WriteBytes(Path, Bytes);
	}
}

// --- initialisation and invalid input ------------------------------------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnSaveInitializeTest, "SpnCore.SaveSubsystem.Initialize", SpnSaveTestFlags)
bool FSpnSaveInitializeTest::RunTest(const FString& Parameters)
{
	FTestEnv Env;
	if (!TestNotNull(TEXT("the game instance creates the save subsystem"), Env.Instance.Save()))
	{
		return false;
	}
	TestNotNull(TEXT("and the world-state subsystem it depends on"), Env.Instance.World());
	TestEqual(TEXT("the save root is the test directory"), Env.Save().GetSaveDirectory(), Env.Root);
	Env.Save().ResetSaveDirectory();
	TestTrue(TEXT("the default save root is Saved/SpnSaves"), Env.Save().GetSaveDirectory().EndsWith(TEXT("SpnSaves")));
	Env.Save().SetSaveDirectory(Env.Root);

	TestTrue(TEXT("a plain slot id is valid"), USpnSaveSubsystem::IsValidSlotId(TEXT("slot_one")));
	for (const TCHAR* Bad : {TEXT(""), TEXT("Slot"), TEXT("1slot"), TEXT("a b"), TEXT("a/b"), TEXT("a\\b"), TEXT("a.b"), TEXT(".."), TEXT("../x"), TEXT("a:b")})
	{
		TestFalse(FString::Printf(TEXT("'%s' is not a valid slot id"), Bad), USpnSaveSubsystem::IsValidSlotId(Bad));
		TestTrue(FString::Printf(TEXT("'%s' has no file path"), Bad), Env.Save().GetSlotFilePath(Bad).IsEmpty());
	}
	TestFalse(TEXT("65 characters are too long"), USpnSaveSubsystem::IsValidSlotId(FString::ChrN(65, TEXT('a'))));
	TestTrue(TEXT("64 characters are fine"), USpnSaveSubsystem::IsValidSlotId(FString::ChrN(64, TEXT('a'))));

	// a second game instance has its own subsystem and its own (default) save root
	FScopedGameInstance Other;
	TestFalse(TEXT("another game instance has its own subsystem"), Other.Save() == Env.Instance.Save());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnSaveInvalidInputTest, "SpnCore.SaveSubsystem.InvalidSlotsAndDirectories", SpnSaveTestFlags)
bool FSpnSaveInvalidInputTest::RunTest(const FString& Parameters)
{
	FTestEnv Env;
	UseDoor(Env.World(), DoorA);

	for (const TCHAR* Bad : {TEXT(""), TEXT("../escape"), TEXT("A"), TEXT("a b"), TEXT("a/b")})
	{
		TestEqual(FString::Printf(TEXT("save '%s' is refused"), Bad), Env.Save().SaveSlot(Bad).Result, ESpnSaveResult::InvalidSlotId);
		TestEqual(FString::Printf(TEXT("load '%s' is refused"), Bad), Env.Save().LoadSlot(Bad).Result, ESpnSaveResult::InvalidSlotId);
	}
	TestFalse(TEXT("nothing was created for invalid slot ids"), IFileManager::Get().DirectoryExists(*Env.Root));
	TestFalse(TEXT("no slot exists"), Env.Save().DoesSlotExist(TEXT("../escape")));

	const FString Before = Env.Save().GetSaveDirectory();
	const FString TooLong = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir()) / FString::ChrN(200, TEXT('d'));
	const TArray<FString> BadDirectories = {TEXT(""), TEXT("relative/dir"), TEXT("Saved"), Env.Root / TEXT("..") / TEXT("up"), Env.Root / TEXT("a?b"),
		Env.Root / TEXT("a*b"), Env.Root / TEXT("a|b"), Env.Root / TEXT("a<b"), TooLong};
	for (const FString& Directory : BadDirectories)
	{
		const FSpnSaveStatus Status = Env.Save().SetSaveDirectory(Directory);
		TestEqual(FString::Printf(TEXT("directory '%s' is rejected"), *Directory.Left(60)), Status.Result, ESpnSaveResult::InvalidDirectory);
		TestEqual(TEXT("and the old directory stays"), Env.Save().GetSaveDirectory(), Before);
	}

	// a directory that cannot be created: its parent is a file
	const FString Blocker = Env.Root / TEXT("blocker");
	IFileManager::Get().MakeDirectory(*Env.Root, true);
	FFileHelper::SaveStringToFile(TEXT("x"), *Blocker);
	TestTrue(TEXT("a directory below a file is formally acceptable"), Env.Save().SetSaveDirectory(Blocker / TEXT("below")).IsOk());
	AddExpectedError(TEXT("NOT saved: cannot create"), EAutomationExpectedErrorFlags::Contains, 1);
	const FSpnSaveStatus Status = Env.Save().SaveSlot(TestSlotId);
	TestEqual(TEXT("saving there fails with InvalidDirectory"), Status.Result, ESpnSaveResult::InvalidDirectory);
	TestEqual(TEXT("the live state is not touched by a failed save"), Env.World().GetRevision(), uint64{1});

	// characters that are not valid UTF-16 must not escape as exceptions
	Env.Save().SetSaveDirectory(Env.Root);
	FString Odd = Env.Root / TEXT("odd");
	Odd.AppendChar(static_cast<TCHAR>(0xD800));
	const FSpnSaveStatus OddStatus = Env.Save().SetSaveDirectory(Odd);
	if (OddStatus.IsOk())
	{
		AddExpectedError(TEXT("saving slot 'slot_one' to"), EAutomationExpectedErrorFlags::Contains, 1);
		const FSpnSaveStatus OddSave = Env.Save().SaveSlot(TestSlotId);
		TestTrue(TEXT("an unpaired surrogate in the directory gives a result, not an exception"),
			OddSave.Result == ESpnSaveResult::Ok || OddSave.Result == ESpnSaveResult::Io || OddSave.Result == ESpnSaveResult::InvalidDirectory);
	}
	return true;
}

// --- save, load, round trip ----------------------------------------------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnSaveCreateTest, "SpnCore.SaveSubsystem.SaveCreatesFile", SpnSaveTestFlags)
bool FSpnSaveCreateTest::RunTest(const FString& Parameters)
{
	FTestEnv Env;
	TestFalse(TEXT("no slot before the first save"), Env.Save().DoesSlotExist(TestSlotId));
	UseDoor(Env.World(), DoorA);
	UseDoor(Env.World(), DoorB);

	const FSpnSaveStatus Status = Env.Save().SaveSlot(TestSlotId);
	TestTrue(FString::Printf(TEXT("save succeeds (%s)"), *Status.Detail), Status.IsOk());
	TestEqual(TEXT("the status names the current generation"), Status.Source, ESpnSaveSource::Current);
	TestEqual(TEXT("and the revision that was saved"), Status.FactsRevision, uint64{2});
	TestEqual(TEXT("and the file"), Status.Path, Env.File());
	TestTrue(TEXT("the main file exists"), IFileManager::Get().FileExists(*Env.File()));
	TestFalse(TEXT("there is no previous generation yet"), IFileManager::Get().FileExists(*Env.Prev()));
	TestFalse(TEXT("and no temporary file is left"), IFileManager::Get().FileExists(*(Env.File() + TEXT(".tmp"))));
	TestTrue(TEXT("DoesSlotExist"), Env.Save().DoesSlotExist(TestSlotId));
	TestEqual(TEXT("saving does not change the live state"), Env.World().GetRevision(), uint64{2});

	const spn::core::SaveSlot Parsed = ParsedSlot(Env.File());
	TestEqual(TEXT("the file is a save slot with two sections"), static_cast<int32>(Parsed.Count()), 2);
	TestNotNull(TEXT("world.facts"), Parsed.Find("world.facts"));
	TestNotNull(TEXT("slot.meta"), Parsed.Find("slot.meta"));
	TestNull(TEXT("no inventory section is invented"), Parsed.Find("player.inventory"));
	TestNull(TEXT("no map section is invented"), Parsed.Find("world.map"));
	TestNull(TEXT("no object section is invented"), Parsed.Find("world.objects"));
	if (const spn::core::SaveSection* Meta = Parsed.Find("slot.meta"))
	{
		TestEqual(TEXT("slot.meta records the facts revision"), FString(UTF8_TO_TCHAR(Meta->payload.c_str())), FString(TEXT("facts_revision 2\n")));
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnSaveRoundTripTest, "SpnCore.SaveSubsystem.WorldFactsRoundTrip", SpnSaveTestFlags)
bool FSpnSaveRoundTripTest::RunTest(const FString& Parameters)
{
	FTestEnv Env;
	UseDoor(Env.World(), DoorA); // open
	UseDoor(Env.World(), DoorB); // open
	UseDoor(Env.World(), DoorB); // closed again
	const TArray<uint8> Saved = Fingerprint(Env);
	TestTrue(TEXT("the probe save worked"), Saved.Num() > 0);
	TestTrue(TEXT("save"), Env.Save().SaveSlot(TestSlotId).IsOk());

	// change the state, then restore it
	UseDoor(Env.World(), DoorA); // closed
	UseDoor(Env.World(), DoorB); // open
	TestEqual(TEXT("the live state changed"), DoorOpen(Env.World(), DoorA), TOptional<bool>(false));
	int32 Replaced = 0;
	Env.World().OnWorldStateReplaced.AddLambda([&Replaced]() { ++Replaced; });
	int32 Loaded = 0;
	Env.Save().OnLoaded.AddLambda([&Loaded]() { ++Loaded; });

	const FSpnSaveStatus Status = Env.Save().LoadSlot(TestSlotId);
	TestTrue(FString::Printf(TEXT("load succeeds (%s)"), *Status.Detail), Status.IsOk());
	TestEqual(TEXT("from the current generation"), Status.Source, ESpnSaveSource::Current);
	TestEqual(TEXT("revision restored"), Env.World().GetRevision(), uint64{3});
	TestEqual(TEXT("the status reports it"), Status.FactsRevision, uint64{3});
	TestEqual(TEXT("door A is open again"), DoorOpen(Env.World(), DoorA), TOptional<bool>(true));
	TestEqual(TEXT("door B is closed again"), DoorOpen(Env.World(), DoorB), TOptional<bool>(false));
	TestEqual(TEXT("listeners were told once"), Replaced, 1);
	TestEqual(TEXT("and OnLoaded fired once"), Loaded, 1);
	TestTrue(TEXT("the restored facts are byte-identical to the saved ones (journal included)"), Fingerprint(Env) == Saved);

	// the interaction registry still works on the restored store
	TestEqual(TEXT("the restored door can be used"), UseDoor(Env.World(), DoorA).Result, ESpnInteractResult::Changed);
	TestEqual(TEXT("and the journal continues"), Env.World().GetRevision(), uint64{4});

	// a different game instance can load the same file (a fresh process, as after a restart)
	FTestEnv Fresh;
	Fresh.Save().SetSaveDirectory(Env.Root);
	TestTrue(TEXT("a fresh game instance loads the slot"), Fresh.Save().LoadSlot(TestSlotId).IsOk());
	TestEqual(TEXT("with the saved revision"), Fresh.World().GetRevision(), uint64{3});
	Fresh.Save().ResetSaveDirectory();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnSaveRepeatedTest, "SpnCore.SaveSubsystem.RepeatedCycles", SpnSaveTestFlags)
bool FSpnSaveRepeatedTest::RunTest(const FString& Parameters)
{
	FTestEnv Env;
	for (int32 Cycle = 1; Cycle <= 25; ++Cycle)
	{
		UseDoor(Env.World(), DoorA);
		const TOptional<bool> Expected = DoorOpen(Env.World(), DoorA);
		const uint64 Revision = Env.World().GetRevision();
		const FSpnSaveStatus Saved = Env.Save().SaveSlot(TestSlotId);
		if (!TestTrue(FString::Printf(TEXT("cycle %d: save (%s)"), Cycle, *Saved.Detail), Saved.IsOk()))
		{
			return false;
		}
		UseDoor(Env.World(), DoorB); // diverge after the save
		const FSpnSaveStatus Loaded = Env.Save().LoadSlot(TestSlotId);
		if (!TestTrue(FString::Printf(TEXT("cycle %d: load (%s)"), Cycle, *Loaded.Detail), Loaded.IsOk()))
		{
			return false;
		}
		TestEqual(FString::Printf(TEXT("cycle %d: revision"), Cycle), Env.World().GetRevision(), Revision);
		TestEqual(FString::Printf(TEXT("cycle %d: door A"), Cycle), DoorOpen(Env.World(), DoorA), Expected);
		TestFalse(FString::Printf(TEXT("cycle %d: the divergence after the save is gone"), Cycle), DoorOpen(Env.World(), DoorB).IsSet());
	}
	TestTrue(TEXT("a previous generation exists after repeated saves"), IFileManager::Get().FileExists(*Env.Prev()));
	TestFalse(TEXT("no temporary files are left"), IFileManager::Get().FileExists(*(Env.File() + TEXT(".tmp"))) || IFileManager::Get().FileExists(*(Env.File() + TEXT(".prev.tmp"))));
	TArray<FString> Files;
	IFileManager::Get().FindFiles(Files, *(Env.File().LeftChop(FString(TEXT("game.spnsave")).Len()) / TEXT("*")), true, false);
	TestEqual(TEXT("exactly the current and the previous file are in the slot directory"), Files.Num(), 2);
	return true;
}

// --- missing, corrupt, unsupported, rejected -----------------------------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnSaveMissingTest, "SpnCore.SaveSubsystem.MissingSave", SpnSaveTestFlags)
bool FSpnSaveMissingTest::RunTest(const FString& Parameters)
{
	FTestEnv Env;
	UseDoor(Env.World(), DoorA);
	int32 Replaced = 0;
	Env.World().OnWorldStateReplaced.AddLambda([&Replaced]() { ++Replaced; });
	const FSpnSaveStatus Status = Env.Save().LoadSlot(TestSlotId);
	TestEqual(TEXT("a missing slot is NotFound"), Status.Result, ESpnSaveResult::NotFound);
	TestEqual(TEXT("nothing was loaded"), Status.Source, ESpnSaveSource::None);
	TestEqual(TEXT("the state is unchanged"), Env.World().GetRevision(), uint64{1});
	TestEqual(TEXT("and nobody was notified"), Replaced, 0);
	TestFalse(TEXT("loading does not create files"), IFileManager::Get().DirectoryExists(*Env.Root));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnSaveCorruptTest, "SpnCore.SaveSubsystem.CorruptSave", SpnSaveTestFlags)
bool FSpnSaveCorruptTest::RunTest(const FString& Parameters)
{
	FTestEnv Env;
	UseDoor(Env.World(), DoorA);
	TestTrue(TEXT("save"), Env.Save().SaveSlot(TestSlotId).IsOk());
	TArray<uint8> Good;
	ReadBytes(Env.File(), Good);
	UseDoor(Env.World(), DoorB);
	const TArray<uint8> Before = Fingerprint(Env);
	int32 Replaced = 0;
	Env.World().OnWorldStateReplaced.AddLambda([&Replaced]() { ++Replaced; });

	struct FCase { const TCHAR* Name; TArray<uint8> Bytes; };
	TArray<FCase> Cases;
	Cases.Add({TEXT("empty file"), TArray<uint8>()});
	Cases.Add({TEXT("random text"), ToBytes("this is not a save file\n")});
	Cases.Add({TEXT("truncated"), TArray<uint8>(Good.GetData(), Good.Num() / 2)});
	Cases.Add({TEXT("last byte removed"), TArray<uint8>(Good.GetData(), Good.Num() - 1)});
	{
		TArray<uint8> Flipped = Good;
		Flipped[Flipped.Num() / 2] ^= 0x01;
		Cases.Add({TEXT("one flipped bit in the middle"), Flipped});
		TArray<uint8> Trailing = Good;
		Trailing.Add('x');
		Cases.Add({TEXT("trailing byte"), Trailing});
	}
	for (const FCase& Case : Cases)
	{
		WriteBytes(Env.File(), Case.Bytes);
		const FSpnSaveStatus Status = Env.Save().LoadSlot(TestSlotId);
		TestEqual(FString::Printf(TEXT("%s is rejected as Corrupt"), Case.Name), Status.Result, ESpnSaveResult::Corrupt);
		TestFalse(FString::Printf(TEXT("%s: the reason is given"), Case.Name), Status.Detail.IsEmpty());
		TestEqual(FString::Printf(TEXT("%s: the live revision is unchanged"), Case.Name), Env.World().GetRevision(), uint64{2});
		TestEqual(FString::Printf(TEXT("%s: door B is still open in the live state"), Case.Name), DoorOpen(Env.World(), DoorB), TOptional<bool>(true));
	}
	TestEqual(TEXT("no failed load notified listeners"), Replaced, 0);
	TestTrue(TEXT("the live facts are byte-identical to before all failed loads"), Fingerprint(Env) == Before);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnSaveUnsupportedTest, "SpnCore.SaveSubsystem.UnsupportedVersions", SpnSaveTestFlags)
bool FSpnSaveUnsupportedTest::RunTest(const FString& Parameters)
{
	FTestEnv Env;
	UseDoor(Env.World(), DoorA);
	TestTrue(TEXT("save generation 1"), Env.Save().SaveSlot(TestSlotId).IsOk());
	UseDoor(Env.World(), DoorB);
	TestTrue(TEXT("save generation 2 (generation 1 becomes the previous file)"), Env.Save().SaveSlot(TestSlotId).IsOk());
	const spn::core::SaveSlot Good = ParsedSlot(Env.File());
	const TArray<uint8> Before = Fingerprint(Env);

	struct FCase { const TCHAR* Name; std::string Text; };
	TArray<FCase> Cases;
	{
		spn::core::SaveSlot NewerFacts = Good;
		(void)NewerFacts.Put(spn::core::SaveSection{"world.facts", 2, true, Good.Find("world.facts")->payload});
		Cases.Add({TEXT("world.facts written by a newer section version"), NewerFacts.Serialize()});
		spn::core::SaveSlot FutureRequired = Good;
		(void)FutureRequired.Put(spn::core::SaveSection{"future.required", 1, true, "x"});
		Cases.Add({TEXT("an unknown required section"), FutureRequired.Serialize()});
		std::string NewerContainer = Good.Serialize();
		NewerContainer.replace(0, 14, "SPN-SAVESLOT 2");
		Cases.Add({TEXT("a newer container version"), NewerContainer});
	}
	for (const FCase& Case : Cases)
	{
		WriteBytes(Env.File(), ToBytes(Case.Text));
		const FSpnSaveStatus Status = Env.Save().LoadSlot(TestSlotId);
		TestEqual(FString::Printf(TEXT("%s: UnsupportedVersion"), Case.Name), Status.Result, ESpnSaveResult::UnsupportedVersion);
		TestEqual(FString::Printf(TEXT("%s: no silent roll back to the older generation"), Case.Name), Status.Source, ESpnSaveSource::None);
		TestEqual(FString::Printf(TEXT("%s: the live revision is unchanged"), Case.Name), Env.World().GetRevision(), uint64{2});

		TArray<uint8> OnDisk;
		ReadBytes(Env.File(), OnDisk);
		const FSpnSaveStatus SaveStatus = Env.Save().SaveSlot(TestSlotId);
		TestEqual(FString::Printf(TEXT("%s: saving over it is refused"), Case.Name), SaveStatus.Result, ESpnSaveResult::UnsupportedVersion);
		TArray<uint8> After;
		ReadBytes(Env.File(), After);
		TestTrue(FString::Printf(TEXT("%s: the file is untouched"), Case.Name), OnDisk == After);
	}
	TestTrue(TEXT("the live facts never changed"), Fingerprint(Env) == Before);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnSaveRejectedTest, "SpnCore.SaveSubsystem.RejectedContent", SpnSaveTestFlags)
bool FSpnSaveRejectedTest::RunTest(const FString& Parameters)
{
	FTestEnv Env;
	UseDoor(Env.World(), DoorA);
	TestTrue(TEXT("save"), Env.Save().SaveSlot(TestSlotId).IsOk());
	const TArray<uint8> Before = Fingerprint(Env);
	spn::core::SaveSlot Good = ParsedSlot(Env.File());

	// checksums are fine (the container is rebuilt) but the facts payload is garbage: only the domain parser can notice
	spn::core::SaveSlot BadFacts = Good;
	(void)BadFacts.Put(spn::core::SaveSection{"world.facts", 1, true, "this is not a facts file\n"});
	WriteBytes(Env.File(), ToBytes(BadFacts.Serialize()));
	FSpnSaveStatus Status = Env.Save().LoadSlot(TestSlotId);
	TestEqual(TEXT("garbage facts behind a valid checksum are Rejected"), Status.Result, ESpnSaveResult::Rejected);
	TestTrue(TEXT("and the reason names the section"), Status.Detail.Contains(TEXT("world.facts")));
	TestEqual(TEXT("state unchanged"), Env.World().GetRevision(), uint64{1});

	// slot.meta that disagrees with the facts
	spn::core::SaveSlot BadMeta = Good;
	(void)BadMeta.Put(spn::core::SaveSection{"slot.meta", 1, false, "facts_revision 99\n"});
	WriteBytes(Env.File(), ToBytes(BadMeta.Serialize()));
	Status = Env.Save().LoadSlot(TestSlotId);
	TestEqual(TEXT("a slot.meta from another moment is Rejected"), Status.Result, ESpnSaveResult::Rejected);
	TestTrue(TEXT("the live facts never changed"), Fingerprint(Env) == Before);
	return true;
}

// --- previous generation -------------------------------------------------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnSavePreviousGenerationTest, "SpnCore.SaveSubsystem.PreviousGenerationRecovery", SpnSaveTestFlags)
bool FSpnSavePreviousGenerationTest::RunTest(const FString& Parameters)
{
	FTestEnv Env;
	UseDoor(Env.World(), DoorA); // revision 1
	TestTrue(TEXT("save generation 1"), Env.Save().SaveSlot(TestSlotId).IsOk());
	UseDoor(Env.World(), DoorB); // revision 2
	TestTrue(TEXT("save generation 2"), Env.Save().SaveSlot(TestSlotId).IsOk());
	UseDoor(Env.World(), DoorB); // revision 3, unsaved
	TestTrue(TEXT("the previous generation exists"), IFileManager::Get().FileExists(*Env.Prev()));

	// 1. damaged main file (checksum) -> previous
	FlipByte(Env.File(), 10);
	FSpnSaveStatus Status = Env.Save().LoadSlot(TestSlotId);
	TestTrue(FString::Printf(TEXT("a damaged main file is recovered from the previous generation (%s)"), *Status.Detail), Status.IsOk());
	TestEqual(TEXT("the status says Previous"), Status.Source, ESpnSaveSource::Previous);
	TestFalse(TEXT("and names why the main file was not used"), Status.CurrentProblem.IsEmpty());
	TestEqual(TEXT("generation 1 is live: revision 1"), Env.World().GetRevision(), uint64{1});
	TestEqual(TEXT("door A open"), DoorOpen(Env.World(), DoorA), TOptional<bool>(true));
	TestFalse(TEXT("door B not yet touched in generation 1"), DoorOpen(Env.World(), DoorB).IsSet());

	// 2. main file missing -> previous
	IFileManager::Get().Delete(*Env.File());
	UseDoor(Env.World(), DoorB);
	Status = Env.Save().LoadSlot(TestSlotId);
	TestTrue(TEXT("a missing main file is recovered too"), Status.IsOk());
	TestEqual(TEXT("from the previous generation"), Status.Source, ESpnSaveSource::Previous);
	TestEqual(TEXT("revision 1 again"), Env.World().GetRevision(), uint64{1});

	// 3. checksum-valid main file with rejected content -> previous
	spn::core::SaveSlot BadFacts = ParsedSlot(Env.Prev());
	(void)BadFacts.Put(spn::core::SaveSection{"world.facts", 1, true, "not facts\n"});
	WriteBytes(Env.File(), ToBytes(BadFacts.Serialize()));
	UseDoor(Env.World(), DoorB);
	Status = Env.Save().LoadSlot(TestSlotId);
	TestTrue(TEXT("a main file whose content is rejected falls back as well"), Status.IsOk() && Status.Source == ESpnSaveSource::Previous);
	TestEqual(TEXT("revision 1"), Env.World().GetRevision(), uint64{1});

	// 4. both damaged -> failure, no change
	WriteBytes(Env.File(), ToBytes("junk"));
	WriteBytes(Env.Prev(), ToBytes("junk"));
	UseDoor(Env.World(), DoorB);
	const uint64 Revision = Env.World().GetRevision();
	Status = Env.Save().LoadSlot(TestSlotId);
	TestEqual(TEXT("with both generations damaged the load fails"), Status.Result, ESpnSaveResult::Corrupt);
	TestEqual(TEXT("and changes nothing"), Env.World().GetRevision(), Revision);

	// 5. saving after damage works and keeps the old good generation if the main file was damaged
	TestTrue(TEXT("a new save overwrites the damaged files"), Env.Save().SaveSlot(TestSlotId).IsOk());
	TestTrue(TEXT("and loads again"), Env.Save().LoadSlot(TestSlotId).IsOk());
	return true;
}

// --- failure leaves the state alone; unknown and unintegrated sections ----------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnSaveUnknownSectionsTest, "SpnCore.SaveSubsystem.UnknownSectionsArePreserved", SpnSaveTestFlags)
bool FSpnSaveUnknownSectionsTest::RunTest(const FString& Parameters)
{
	FTestEnv Env;
	UseDoor(Env.World(), DoorA);
	TestTrue(TEXT("save"), Env.Save().SaveSlot(TestSlotId).IsOk());

	// a save of a newer build: the same facts plus an optional section this build does not know
	spn::core::SaveSlot Future = ParsedSlot(Env.File());
	const std::string Payload = "future payload \x01\x02 with\nnewlines";
	(void)Future.Put(spn::core::SaveSection{"future.cosmetics", 3, false, Payload});
	(void)Future.Put(spn::core::SaveSection{"zzz.last", 1, false, "last"});
	WriteBytes(Env.File(), ToBytes(Future.Serialize()));

	FSpnSaveStatus Status = Env.Save().LoadSlot(TestSlotId);
	TestTrue(TEXT("a slot with unknown optional sections still loads"), Status.IsOk());
	UseDoor(Env.World(), DoorB);
	Status = Env.Save().SaveSlot(TestSlotId);
	TestTrue(TEXT("and can be saved again"), Status.IsOk());
	TestTrue(TEXT("the status lists what was carried over"), Status.PreservedSections.Contains(TEXT("future.cosmetics")) && Status.PreservedSections.Contains(TEXT("zzz.last")));

	const spn::core::SaveSlot After = ParsedSlot(Env.File());
	TestNotNull(TEXT("future.cosmetics survived"), After.Find("future.cosmetics"));
	if (const spn::core::SaveSection* Kept = After.Find("future.cosmetics"))
	{
		TestTrue(TEXT("byte for byte, with its version"), Kept->payload == Payload && Kept->version == 3 && !Kept->required);
	}
	TestNotNull(TEXT("zzz.last survived"), After.Find("zzz.last"));
	TestEqual(TEXT("the facts are the new ones"), Env.World().GetRevision(), uint64{2});
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpnSaveNotIntegratedTest, "SpnCore.SaveSubsystem.SectionsWithoutOwnerAreNeverDropped", SpnSaveTestFlags)
bool FSpnSaveNotIntegratedTest::RunTest(const FString& Parameters)
{
	FTestEnv Env;
	UseDoor(Env.World(), DoorA);
	TestTrue(TEXT("save"), Env.Save().SaveSlot(TestSlotId).IsOk());
	const TArray<uint8> Before = Fingerprint(Env);

	for (const char* Section : {"player.inventory", "world.map", "world.objects"})
	{
		spn::core::SaveSlot WithSection = ParsedSlot(Env.File());
		(void)WithSection.Put(spn::core::SaveSection{Section, 1, false, "payload of a domain without an Unreal owner"});
		WriteBytes(Env.File(), ToBytes(WithSection.Serialize()));
		TArray<uint8> OnDisk;
		ReadBytes(Env.File(), OnDisk);

		FSpnSaveStatus Status = Env.Save().LoadSlot(TestSlotId);
		TestEqual(FString::Printf(TEXT("%hs: load is refused instead of restoring only part of the slot"), Section), Status.Result, ESpnSaveResult::SectionsNotIntegrated);
		TestTrue(TEXT("and names the section"), Status.Detail.Contains(UTF8_TO_TCHAR(Section)));
		Status = Env.Save().SaveSlot(TestSlotId);
		TestEqual(FString::Printf(TEXT("%hs: save is refused instead of dropping the section"), Section), Status.Result, ESpnSaveResult::SectionsNotIntegrated);
		TArray<uint8> After;
		ReadBytes(Env.File(), After);
		TestTrue(TEXT("and the file is untouched"), OnDisk == After);
		TestTrue(TEXT("the live state is untouched"), Fingerprint(Env) == Before);
		IFileManager::Get().Delete(*Env.File());
		IFileManager::Get().Delete(*Env.Prev());
		TestTrue(TEXT("a fresh save in the same slot works again"), Env.Save().SaveSlot(TestSlotId).IsOk());
	}
	return true;
}
#endif
