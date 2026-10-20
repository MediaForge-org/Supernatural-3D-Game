#include "SpnWorldStateSubsystem.h"
#include "SpnCoreModule.h"

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

struct USpnWorldStateSubsystem::FState
{
	spn::core::WorldFacts Facts;
	spn::core::InteractionRegistry Registry;
};

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
}

USpnWorldStateSubsystem::~USpnWorldStateSubsystem() = default;

void USpnWorldStateSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogSpnCore, Log, TEXT("World state subsystem initialized (revision %llu)"),
		static_cast<unsigned long long>(GetRevision()));
}

void USpnWorldStateSubsystem::Deinitialize()
{
	OnWorldStateReplaced.Clear();
	Super::Deinitialize();
}

FSpnInteractStatus USpnWorldStateSubsystem::TryInteract(const FString& Kind, const FString& StableId,
	ESpnInteractionVerb Verb, const TMap<FString, bool>& BoolDefaults, const FString& Cause)
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
	for (const TPair<FString, bool>& Default : BoolDefaults)
	{
		Defaults.values.emplace(ToUtf8(Default.Key), Default.Value);
	}

	spn::core::InteractionVerb CoreVerb = spn::core::InteractionVerb::Use;
	switch (Verb)
	{
	case ESpnInteractionVerb::Use: CoreVerb = spn::core::InteractionVerb::Use; break;
	}

	const auto Outcome = State->Registry.Interact(State->Facts, spn::core::InteractableRef{*ParsedKind, ToUtf8(StableId)},
		CoreVerb, Defaults, ToUtf8(Cause));
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

FSpnSlotStatus USpnWorldStateSubsystem::SaveToSlot(const FString& Slot) const
{
	FSpnSlotStatus Status;
	if (!IsValidSlotName(Slot))
	{
		Status.Result = ESpnSlotResult::InvalidSlotName;
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
		UE_LOG(LogSpnCore, Error, TEXT("World state: saving slot '%s' to '%s' failed; the previous save is unchanged"),
			*Slot, *FinalPath);
		return Status;
	}

	UE_LOG(LogSpnCore, Log, TEXT("World state: saved slot '%s' (revision %llu, %llu bytes)"),
		*Slot, static_cast<unsigned long long>(GetRevision()), static_cast<unsigned long long>(Bytes.size()));
	return Status;
}

FSpnSlotStatus USpnWorldStateSubsystem::LoadFromSlot(const FString& Slot)
{
	FSpnSlotStatus Status;
	if (!IsValidSlotName(Slot))
	{
		Status.Result = ESpnSlotResult::InvalidSlotName;
		return Status;
	}

	const FString Path = GetSlotFilePath(Slot);
	if (!FPaths::FileExists(Path))
	{
		Status.Result = ESpnSlotResult::SlotNotFound;
		return Status;
	}

	TArray64<uint8> Bytes;
	if (!FFileHelper::LoadFileToArray(Bytes, *Path, FILEREAD_Silent))
	{
		Status.Result = ESpnSlotResult::ReadFailed;
		UE_LOG(LogSpnCore, Warning, TEXT("World state: reading slot '%s' from '%s' failed; state unchanged"), *Slot, *Path);
		return Status;
	}

	auto Loaded = spn::core::WorldFacts::Deserialize(
		std::string_view(reinterpret_cast<const char*>(Bytes.GetData()), static_cast<std::size_t>(Bytes.Num())));
	if (!Loaded)
	{
		Status.Result = ESpnSlotResult::CorruptData;
		Status.Detail = ToFString(spn::core::ToString(Loaded.Error().code));
		Status.Line = static_cast<int32>(Loaded.Error().line);
		UE_LOG(LogSpnCore, Warning, TEXT("World state: slot '%s' rejected (%s at line %d); state unchanged"),
			*Slot, *Status.Detail, Status.Line);
		return Status;
	}

	State->Facts = std::move(Loaded.Value());
	UE_LOG(LogSpnCore, Log, TEXT("World state: loaded slot '%s' (revision %llu)"),
		*Slot, static_cast<unsigned long long>(GetRevision()));
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
