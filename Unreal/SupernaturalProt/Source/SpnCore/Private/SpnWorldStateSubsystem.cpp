#include "SpnWorldStateSubsystem.h"
#include "SpnWorldStateInternal.h"
#include "SpnCoreModule.h"

#include "Engine/GameInstance.h"
#include "HAL/FileManager.h"
#include "HAL/PlatformFileManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/WindowsHWrapper.h"
#include "Windows/HideWindowsPlatformTypes.h"
#endif

#include <spn/core/ContentId.h>
#include <spn/core/Interaction.h>
#include <spn/core/Pickup.h>
#include <spn/core/TestDoor.h>
#include <spn/core/WorldFacts.h>
#include <string>
#include <string_view>
#include <utility>

namespace
{
	constexpr int32 MaxSlotNameLength = 64;
	const TCHAR* const SaveFileName = TEXT("worldfacts.spnfacts");
	const TCHAR* const TempSuffix = TEXT(".tmp");

	std::string ToUtf8(const FString& Text)
	{
		const FTCHARToUTF8 Converted(*Text);
		return std::string(Converted.Get(), Converted.Length());
	}

	FString ToFString(std::string_view Text)
	{
		return FString::ConstructFromPtrSize(reinterpret_cast<const UTF8CHAR*>(Text.data()), static_cast<int32>(Text.size()));
	}

	FString SaveRootDirectory()
	{
		return FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir() / TEXT("SpnSaves"));
	}

	ESpnInteractResult ToInteractResult(spn::core::InteractionErrorCode Code)
	{
		using spn::core::InteractionErrorCode;
		switch (Code)
		{
		case InteractionErrorCode::InvalidId: return ESpnInteractResult::InvalidId;
		case InteractionErrorCode::UnknownKind: return ESpnInteractResult::UnknownKind;
		case InteractionErrorCode::StateCorrupt: return ESpnInteractResult::StateCorrupt;
		case InteractionErrorCode::Refused: return ESpnInteractResult::Refused;
		case InteractionErrorCode::Fact:
		case InteractionErrorCode::DuplicateKind:
		case InteractionErrorCode::InvalidRegistration: return ESpnInteractResult::Failed;
		}
		return ESpnInteractResult::Failed;
	}

	/** Writes Bytes to Path and flushes them to disk before closing. */
	bool WriteFileFlushed(const FString& Path, const std::string& Bytes)
	{
		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
		IFileHandle* Handle = PlatformFile.OpenWrite(*Path);
		if (Handle == nullptr)
		{
			return false;
		}
		const bool bWritten = Handle->Write(reinterpret_cast<const uint8*>(Bytes.data()), static_cast<int64>(Bytes.size()))
			&& Handle->Flush(/*bFullFlush*/ true);
		delete Handle;
		return bWritten;
	}

	bool FileHasBytes(const FString& Path, const std::string& Bytes)
	{
		TArray64<uint8> Read;
		return FFileHelper::LoadFileToArray(Read, *Path, FILEREAD_Silent)
			&& Read.Num() == static_cast<int64>(Bytes.size())
			&& FMemory::Memcmp(Read.GetData(), Bytes.data(), Bytes.size()) == 0;
	}

	/**
	 * Renames From over To in one step, so To is either the old or the new file, never
	 * missing or partial. IFileManager::Move cannot be used: with Replace it deletes the
	 * target first. Windows needs MoveFileExW; POSIX rename() already replaces atomically.
	 */
	bool ReplaceFileAtomically(const FString& From, const FString& To)
	{
#if PLATFORM_WINDOWS
		return ::MoveFileExW(*From, *To, MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH) != 0;
#else
		return FPlatformFileManager::Get().GetPlatformFile().MoveFile(*To, *From);
#endif
	}
}

const TCHAR* LexToString(ESpnSlotResult Result)
{
	switch (Result)
	{
	case ESpnSlotResult::Ok: return TEXT("Ok");
	case ESpnSlotResult::InvalidSlotName: return TEXT("InvalidSlotName");
	case ESpnSlotResult::SlotNotFound: return TEXT("SlotNotFound");
	case ESpnSlotResult::ReadFailed: return TEXT("ReadFailed");
	case ESpnSlotResult::WriteFailed: return TEXT("WriteFailed");
	case ESpnSlotResult::CorruptData: return TEXT("CorruptData");
	}
	return TEXT("Unknown");
}

const TCHAR* LexToString(ESpnInteractResult Result)
{
	switch (Result)
	{
	case ESpnInteractResult::Changed: return TEXT("Changed");
	case ESpnInteractResult::Unchanged: return TEXT("Unchanged");
	case ESpnInteractResult::Refused: return TEXT("Refused");
	case ESpnInteractResult::InvalidId: return TEXT("InvalidId");
	case ESpnInteractResult::UnknownKind: return TEXT("UnknownKind");
	case ESpnInteractResult::StateCorrupt: return TEXT("StateCorrupt");
	case ESpnInteractResult::Failed: return TEXT("Failed");
	}
	return TEXT("Unknown");
}

USpnWorldStateSubsystem::USpnWorldStateSubsystem()
	: State(MakePimpl<FState>())
{
	if (const auto Error = spn::core::RegisterTestDoor(State->Registry))
	{
		UE_LOG(LogSpnCore, Error, TEXT("World state: registering the test door failed: %s"),
			*ToFString(spn::core::ToString(Error->code)));
	}
	if (const auto Error = spn::core::RegisterTestPickup(State->Registry))
	{
		UE_LOG(LogSpnCore, Error, TEXT("World state: registering the test pickup failed: %s"),
			*ToFString(spn::core::ToString(Error->code)));
	}
}

USpnWorldStateSubsystem::~USpnWorldStateSubsystem() = default;

void USpnWorldStateSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogSpnCore, Log, TEXT("World state [%s]: subsystem initialized (revision %llu)"),
		*DescribeOwner(), static_cast<unsigned long long>(GetRevision()));
}

void USpnWorldStateSubsystem::Deinitialize()
{
	OnWorldStateReplaced.Clear();
	Super::Deinitialize();
}

FSpnInteractStatus USpnWorldStateSubsystem::TryInteract(const FString& Kind, const FString& StableId,
	ESpnInteractionVerb Verb, const TMap<FString, bool>& BoolDefaults, const FString& Cause)
{
	FSpnInteractDefaults Defaults;
	Defaults.Bools = BoolDefaults;
	return TryInteract(Kind, StableId, Verb, Defaults, DefaultHolder(), Cause);
}

FSpnInteractStatus USpnWorldStateSubsystem::TryInteract(const FString& Kind, const FString& StableId,
	ESpnInteractionVerb Verb, const FSpnInteractDefaults& AuthoredDefaults, const FString& Holder, const FString& Cause)
{
	FSpnInteractStatus Status;

	const std::optional<spn::core::ContentId> ParsedKind = spn::core::ContentId::Parse(ToUtf8(Kind));
	if (!ParsedKind)
	{
		Status.Result = ESpnInteractResult::UnknownKind;
		Status.Detail = Kind;
		return Status;
	}

	spn::core::InteractionDefaults Defaults;
	for (const TPair<FString, bool>& Default : AuthoredDefaults.Bools)
	{
		Defaults.values.emplace(ToUtf8(Default.Key), spn::core::FactValue(static_cast<bool>(Default.Value)));
	}
	for (const TPair<FString, int64>& Default : AuthoredDefaults.Ints)
	{
		Defaults.values.emplace(ToUtf8(Default.Key), spn::core::FactValue(static_cast<std::int64_t>(Default.Value)));
	}
	for (const TPair<FString, FString>& Default : AuthoredDefaults.Strings)
	{
		Defaults.values.emplace(ToUtf8(Default.Key), spn::core::FactValue(ToUtf8(Default.Value)));
	}

	spn::core::InteractionVerb CoreVerb = spn::core::InteractionVerb::Use;
	switch (Verb)
	{
	case ESpnInteractionVerb::Use: CoreVerb = spn::core::InteractionVerb::Use; break;
	}

	const std::string HolderUtf8 = ToUtf8(Holder);
	const auto Outcome = State->Registry.Interact(State->Facts, spn::core::InteractableRef{*ParsedKind, ToUtf8(StableId)},
		CoreVerb, Defaults, ToUtf8(Cause), HolderUtf8);
	if (!Outcome)
	{
		const spn::core::InteractionError& Error = Outcome.Error();
		Status.Result = ToInteractResult(Error.code);
		Status.Feedback = ToFString(Error.feedback.Value());
		Status.Detail = Error.fact ? ToFString(spn::core::ToString(Error.fact->code)) : ToFString(Error.subject);
		return Status;
	}

	Status.Result = Outcome.Value().changes.empty() ? ESpnInteractResult::Unchanged : ESpnInteractResult::Changed;
	Status.Feedback = ToFString(Outcome.Value().feedback.Value());
	return Status;
}

TOptional<int64> USpnWorldStateSubsystem::ReadInventoryCount(const FString& Holder, const FString& Item) const
{
	const auto Count = spn::core::InventoryCount(State->Facts, ToUtf8(Holder), ToUtf8(Item));
	if (!Count)
	{
		return {};
	}
	return static_cast<int64>(Count.Value());
}

TArray<FSpnInventoryEntry> USpnWorldStateSubsystem::ListInventory(const FString& Holder) const
{
	TArray<FSpnInventoryEntry> Entries;
	const auto List = spn::core::ListInventory(State->Facts, ToUtf8(Holder));
	if (!List)
	{
		UE_LOG(LogSpnCore, Warning, TEXT("World state [%s]: inventory of '%s' unavailable: %s (%s)"), *DescribeOwner(), *Holder,
			*ToFString(spn::core::ToString(List.Error().code)), *ToFString(List.Error().subject));
		return Entries;
	}
	for (const spn::core::InventoryEntry& Entry : List.Value())
	{
		Entries.Add({ToFString(Entry.item), static_cast<int64>(Entry.count)});
	}
	return Entries;
}

TOptional<bool> USpnWorldStateSubsystem::ReadFlag(const FString& Key) const
{
	if (const bool* Value = State->Facts.FindAs<bool>(ToUtf8(Key)))
	{
		return *Value;
	}
	return {};
}

uint64 USpnWorldStateSubsystem::GetRevision() const
{
	return State->Facts.Revision();
}

FString USpnWorldStateSubsystem::DescribeOwner() const
{
	return GetNameSafe(GetGameInstance());
}

FSpnSlotStatus USpnWorldStateSubsystem::SaveToSlot(const FString& Slot) const
{
	FSpnSlotStatus Status;
	if (!IsValidSlotName(Slot))
	{
		Status.Result = ESpnSlotResult::InvalidSlotName;
		UE_LOG(LogSpnCore, Warning, TEXT("World state [%s]: NOT saved: invalid slot name '%s' (allowed: [a-z][a-z0-9_]*, at most %d chars)"),
			*DescribeOwner(), *Slot, MaxSlotNameLength);
		return Status;
	}

	const FString FinalPath = GetSlotFilePath(Slot);
	const FString TempPath = FinalPath + TempSuffix;
	const std::string Bytes = State->Facts.Serialize();

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	// The old file is touched only by the final rename: until then a failure leaves it intact.
	const bool bCommitted = PlatformFile.CreateDirectoryTree(*FPaths::GetPath(FinalPath))
		&& WriteFileFlushed(TempPath, Bytes)
		&& FileHasBytes(TempPath, Bytes)
		&& ReplaceFileAtomically(TempPath, FinalPath);
	if (!bCommitted)
	{
		PlatformFile.DeleteFile(*TempPath);
		Status.Result = ESpnSlotResult::WriteFailed;
		UE_LOG(LogSpnCore, Error, TEXT("World state [%s]: saving slot '%s' to '%s' failed; the previous save is unchanged"),
			*DescribeOwner(), *Slot, *FinalPath);
		return Status;
	}

	UE_LOG(LogSpnCore, Log, TEXT("World state [%s]: saved slot '%s' (revision %llu, %llu bytes) to '%s'"),
		*DescribeOwner(), *Slot, static_cast<unsigned long long>(GetRevision()), static_cast<unsigned long long>(Bytes.size()), *FinalPath);
	return Status;
}

FSpnSlotStatus USpnWorldStateSubsystem::LoadFromSlot(const FString& Slot)
{
	FSpnSlotStatus Status;
	if (!IsValidSlotName(Slot))
	{
		Status.Result = ESpnSlotResult::InvalidSlotName;
		UE_LOG(LogSpnCore, Warning, TEXT("World state [%s]: NOT loaded: invalid slot name '%s'; state unchanged"), *DescribeOwner(), *Slot);
		return Status;
	}

	const FString Path = GetSlotFilePath(Slot);
	if (!FPaths::FileExists(Path))
	{
		Status.Result = ESpnSlotResult::SlotNotFound;
		UE_LOG(LogSpnCore, Warning, TEXT("World state [%s]: NOT loaded: no save in slot '%s' ('%s'); state unchanged"), *DescribeOwner(), *Slot, *Path);
		return Status;
	}

	TArray64<uint8> Bytes;
	if (!FFileHelper::LoadFileToArray(Bytes, *Path, FILEREAD_Silent))
	{
		Status.Result = ESpnSlotResult::ReadFailed;
		UE_LOG(LogSpnCore, Warning, TEXT("World state [%s]: reading slot '%s' from '%s' failed; state unchanged"), *DescribeOwner(), *Slot, *Path);
		return Status;
	}

	auto Loaded = spn::core::WorldFacts::Deserialize(
		std::string_view(reinterpret_cast<const char*>(Bytes.GetData()), static_cast<std::size_t>(Bytes.Num())));
	if (!Loaded)
	{
		Status.Result = ESpnSlotResult::CorruptData;
		Status.Detail = ToFString(spn::core::ToString(Loaded.Error().code));
		Status.Line = static_cast<int32>(Loaded.Error().line);
		UE_LOG(LogSpnCore, Warning, TEXT("World state [%s]: slot '%s' rejected (%s at line %d) from '%s'; state unchanged"),
			*DescribeOwner(), *Slot, *Status.Detail, Status.Line, *Path);
		return Status;
	}

	const uint64 PreviousRevision = GetRevision();
	State->Facts = std::move(Loaded.Value());
	UE_LOG(LogSpnCore, Log, TEXT("World state [%s]: loaded slot '%s' from '%s' (revision %llu -> %llu); notifying listeners"),
		*DescribeOwner(), *Slot, *Path, static_cast<unsigned long long>(PreviousRevision), static_cast<unsigned long long>(GetRevision()));
	OnWorldStateReplaced.Broadcast();
	return Status;
}

bool USpnWorldStateSubsystem::IsValidSlotName(const FString& Slot)
{
	if (Slot.IsEmpty() || Slot.Len() > MaxSlotNameLength || !(Slot[0] >= TEXT('a') && Slot[0] <= TEXT('z')))
	{
		return false;
	}
	for (const TCHAR Char : Slot)
	{
		const bool bAllowed = (Char >= TEXT('a') && Char <= TEXT('z')) || (Char >= TEXT('0') && Char <= TEXT('9')) || Char == TEXT('_');
		if (!bAllowed)
		{
			return false;
		}
	}
	return true;
}

FString USpnWorldStateSubsystem::GetSlotFilePath(const FString& Slot)
{
	return IsValidSlotName(Slot) ? SaveRootDirectory() / Slot / SaveFileName : FString();
}

bool USpnWorldStateSubsystem::IsValidInteractableId(const FString& StableId)
{
	return spn::core::IsValidInteractableId(ToUtf8(StableId));
}

FString USpnWorldStateSubsystem::TestDoorKind()
{
	return ToFString(spn::core::TestDoorKind().Value());
}

FString USpnWorldStateSubsystem::TestDoorOpenKey(const FString& StableId)
{
	return ToFString(spn::core::TestDoorOpenKey(ToUtf8(StableId)));
}

FString USpnWorldStateSubsystem::TestDoorLockedKey(const FString& StableId)
{
	return ToFString(spn::core::TestDoorLockedKey(ToUtf8(StableId)));
}

bool USpnWorldStateSubsystem::IsValidHolderId(const FString& Holder)
{
	return spn::core::IsValidHolderId(ToUtf8(Holder));
}

bool USpnWorldStateSubsystem::IsValidItemId(const FString& Item)
{
	return spn::core::IsValidItemId(ToUtf8(Item));
}

FString USpnWorldStateSubsystem::DefaultHolder()
{
	return ToFString(spn::core::kDefaultInteractionHolder);
}

FString USpnWorldStateSubsystem::TestPickupKind()
{
	return ToFString(spn::core::TestPickupKind().Value());
}

FString USpnWorldStateSubsystem::TestPickupTakenKey(const FString& StableId)
{
	return ToFString(spn::core::TestPickupTakenKey(ToUtf8(StableId)));
}
