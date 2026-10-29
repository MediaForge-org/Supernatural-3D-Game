#include "SpnSaveSubsystem.h"
#include "SpnCoreModule.h"
#include "SpnInventoryInternal.h"
#include "SpnWorldStateInternal.h"

#include "Engine/GameInstance.h"
#include "HAL/FileManager.h"
#include "HAL/PlatformFileManager.h"
#include "Misc/Paths.h"

#include <spn/core/GameSave.h>
#include <spn/core/SaveSlot.h>
#include <spn/core/SaveStore.h>

#include <algorithm>
#include <exception>
#include <filesystem>
#include <string>

namespace
{
	constexpr int32 MaxSlotIdLength = 64;
	// root + "/" + slot (64) + "/" + "game.spnsave.prev.tmp" has to stay well below the classic MAX_PATH of 260
	constexpr int32 MaxDirectoryLength = 150;
	const TCHAR* const SlotFileName = TEXT("game.spnsave");

	FString DefaultSaveDirectory()
	{
		return FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir() / TEXT("SpnSaves"));
	}

	FString ToFString(const std::string& Utf8)
	{
		return FString(FUTF8ToTCHAR(Utf8.c_str(), static_cast<int32>(Utf8.size())));
	}

	/** UTF-8 round trip through char8_t: correct for non-ASCII names on every platform (a TCHAR* would be wchar_t only on Windows). */
	std::filesystem::path ToPath(const FString& Path)
	{
		const FTCHARToUTF8 Utf8(*Path);
		return std::filesystem::path(std::u8string(reinterpret_cast<const char8_t*>(Utf8.Get()), static_cast<std::size_t>(Utf8.Length())));
	}

	/** Sections that belong to domains without an Unreal owner: world.objects and world.map always, player.inventory while the inventory has no catalog. */
	bool IsNotIntegratedSection(std::string_view Id, bool bInventoryActive)
	{
		return Id == spn::core::kSectionObjects || Id == spn::core::kSectionMap || (!bInventoryActive && Id == spn::core::kSectionInventory);
	}

	bool IsNewerThanThisBuild(spn::core::SaveErrorCode Code)
	{
		using spn::core::SaveErrorCode;
		return Code == SaveErrorCode::UnsupportedContainerVersion || Code == SaveErrorCode::UnsupportedSectionVersion
			|| Code == SaveErrorCode::UnsupportedRequiredSection;
	}

	ESpnSaveResult ToResult(spn::core::SaveErrorCode Code)
	{
		using spn::core::SaveErrorCode;
		switch (Code)
		{
		case SaveErrorCode::UnsupportedContainerVersion:
		case SaveErrorCode::UnsupportedSectionVersion:
		case SaveErrorCode::UnsupportedRequiredSection:
			return ESpnSaveResult::UnsupportedVersion;
		case SaveErrorCode::SectionRejected:
		case SaveErrorCode::ConsistencyMismatch:
		case SaveErrorCode::MissingContext:
			return ESpnSaveResult::Rejected;
		case SaveErrorCode::Io:
		case SaveErrorCode::VerifyFailed:
			return ESpnSaveResult::Io;
		default:
			return ESpnSaveResult::Corrupt;
		}
	}

	FString Describe(const spn::core::SaveError& Error)
	{
		return FString::Printf(TEXT("%s: %s"), *ToFString(std::string(spn::core::ToString(Error.code))), *ToFString(Error.detail));
	}

	FSpnSaveStatus Fail(ESpnSaveResult Result, const FString& Detail, const FString& Path = FString())
	{
		FSpnSaveStatus Status;
		Status.Result = Result;
		Status.Detail = Detail;
		Status.Path = Path;
		return Status;
	}

	/** One generation read from disk and checked down to the container and schema level. */
	struct FCandidate
	{
		bool bExists = false;
		std::optional<spn::core::SaveSlot> Slot;
		std::optional<spn::core::SaveError> Problem;
	};

	/**
	 * A known section id with a newer version than this build reads (the container tolerates that for optional sections and would hand them to
	 * "unread"): never ignored (it would look like an absent inventory) and never overwritten (a silent downgrade). Empty = none.
	 */
	FString NewerKnownSection(const spn::core::SaveSlot& Slot)
	{
		for (const auto& Entry : Slot.Sections())
		{
			const std::string_view Id = Entry.first;
			const bool bKnown = Id == spn::core::kSectionFacts || Id == spn::core::kSectionObjects || Id == spn::core::kSectionMap || Id == spn::core::kSectionInventory
				|| Id == spn::core::kSectionMeta;
			if (bKnown && Entry.second.version > 1)
			{
				return ToFString(Entry.first);
			}
		}
		return FString();
	}

	/** Marks a save/load as running for its whole duration, including the callbacks it broadcasts. */
	struct FBusyScope
	{
		bool& Flag;
		explicit FBusyScope(bool& InFlag) : Flag(InFlag) { Flag = true; }
		~FBusyScope() { Flag = false; }
	};

	FCandidate ReadCandidate(spn::core::SaveFileSystem& Fs, const std::filesystem::path& Path)
	{
		FCandidate Candidate;
		Candidate.bExists = Fs.Exists(Path);
		if (!Candidate.bExists)
		{
			return Candidate;
		}
		auto Bytes = Fs.Read(Path);
		if (!Bytes)
		{
			Candidate.Problem = Bytes.Error();
			return Candidate;
		}
		auto Parsed = spn::core::SaveSlot::Parse(Bytes.Value());
		if (!Parsed)
		{
			Candidate.Problem = Parsed.Error();
			return Candidate;
		}
		if (const auto Error = Parsed.Value().Validate(spn::core::GameSaveSchema()))
		{
			Candidate.Problem = *Error;
			return Candidate;
		}
		Candidate.Slot = std::move(Parsed).Value();
		return Candidate;
	}

	TArray<FString> NotIntegratedSections(const spn::core::SaveSlot& Slot, bool bInventoryActive)
	{
		TArray<FString> Found;
		for (const auto& Entry : Slot.Sections())
		{
			if (IsNotIntegratedSection(Entry.first, bInventoryActive))
			{
				Found.Add(ToFString(Entry.first));
			}
		}
		return Found;
	}
}

const TCHAR* LexToString(ESpnSaveResult Result)
{
	switch (Result)
	{
	case ESpnSaveResult::Ok: return TEXT("Ok");
	case ESpnSaveResult::InvalidSlotId: return TEXT("InvalidSlotId");
	case ESpnSaveResult::InvalidDirectory: return TEXT("InvalidDirectory");
	case ESpnSaveResult::WorldStateUnavailable: return TEXT("WorldStateUnavailable");
	case ESpnSaveResult::NotFound: return TEXT("NotFound");
	case ESpnSaveResult::Io: return TEXT("Io");
	case ESpnSaveResult::Corrupt: return TEXT("Corrupt");
	case ESpnSaveResult::UnsupportedVersion: return TEXT("UnsupportedVersion");
	case ESpnSaveResult::Rejected: return TEXT("Rejected");
	case ESpnSaveResult::SectionsNotIntegrated: return TEXT("SectionsNotIntegrated");
	case ESpnSaveResult::InconsistentSnapshot: return TEXT("InconsistentSnapshot");
	case ESpnSaveResult::InternalError: return TEXT("InternalError");
	}
	return TEXT("Unknown");
}

void USpnSaveSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Collection.InitializeDependency<USpnWorldStateSubsystem>();
	Collection.InitializeDependency<USpnInventorySubsystem>();
	Super::Initialize(Collection);
	SaveDirectory = DefaultSaveDirectory();
	UE_LOG(LogSpnCore, Log, TEXT("Save [%s]: subsystem initialized, save root '%s'"), *GetNameSafe(GetGameInstance()), *SaveDirectory);
}

void USpnSaveSubsystem::Deinitialize()
{
	OnSaved.Clear();
	OnLoaded.Clear();
	Super::Deinitialize();
}

bool USpnSaveSubsystem::IsValidSlotId(const FString& SlotId)
{
	return USpnWorldStateSubsystem::IsValidSlotName(SlotId);
}

FString USpnSaveSubsystem::GetSaveDirectory() const
{
	return SaveDirectory.IsEmpty() ? DefaultSaveDirectory() : SaveDirectory;
}

void USpnSaveSubsystem::ResetSaveDirectory()
{
	SaveDirectory = DefaultSaveDirectory();
}

FSpnSaveStatus USpnSaveSubsystem::SetSaveDirectory(const FString& AbsoluteDirectory)
{
	FString Reason;
	if (AbsoluteDirectory.IsEmpty())
	{
		Reason = TEXT("empty");
	}
	else if (FPaths::IsRelative(AbsoluteDirectory))
	{
		Reason = TEXT("not absolute");
	}
	else if (AbsoluteDirectory.Len() > MaxDirectoryLength)
	{
		Reason = FString::Printf(TEXT("longer than %d characters"), MaxDirectoryLength);
	}
	else
	{
		TArray<FString> Parts;
		FString Normalized = AbsoluteDirectory.Replace(TEXT("\\"), TEXT("/"));
		Normalized.ParseIntoArray(Parts, TEXT("/"), /*CullEmpty*/ true);
		for (int32 Index = 0; Index < Parts.Num() && Reason.IsEmpty(); ++Index)
		{
			const FString& Part = Parts[Index];
			if (Part == TEXT("..") || Part == TEXT("."))
			{
				Reason = TEXT("contains a '.' or '..' segment");
			}
			else
			{
				// the drive part "C:" is the only place a colon is allowed
				const bool bDrive = Index == 0 && Part.Len() == 2 && Part[1] == TEXT(':');
				for (const TCHAR Char : Part)
				{
					if (Char < 32 || Char == TEXT('<') || Char == TEXT('>') || Char == TEXT('"') || Char == TEXT('|') || Char == TEXT('?') || Char == TEXT('*')
						|| (Char == TEXT(':') && !bDrive))
					{
						Reason = FString::Printf(TEXT("segment '%s' contains a character that is not allowed in a name"), *Part);
						break;
					}
				}
			}
		}
	}
	if (!Reason.IsEmpty())
	{
		UE_LOG(LogSpnCore, Warning, TEXT("Save [%s]: save directory '%s' rejected: %s; keeping '%s'"), *GetNameSafe(GetGameInstance()), *AbsoluteDirectory, *Reason,
			*GetSaveDirectory());
		return Fail(ESpnSaveResult::InvalidDirectory, Reason);
	}
	SaveDirectory = AbsoluteDirectory;
	return FSpnSaveStatus();
}

FString USpnSaveSubsystem::GetSlotFilePath(const FString& SlotId) const
{
	return IsValidSlotId(SlotId) ? GetSaveDirectory() / SlotId / SlotFileName : FString();
}

bool USpnSaveSubsystem::DoesSlotExist(const FString& SlotId) const
{
	const FString Path = GetSlotFilePath(SlotId);
	return !Path.IsEmpty() && (FPaths::FileExists(Path) || FPaths::FileExists(Path + TEXT(".prev")));
}

FSpnSaveStatus USpnSaveSubsystem::SaveSlot(const FString& SlotId)
{
	check(IsInGameThread());
	if (bBusy)
	{
		UE_LOG(LogSpnCore, Warning, TEXT("Save [%s]: request refused: a save or load is already running (called back from a callback?)"), *GetNameSafe(GetGameInstance()));
		return Fail(ESpnSaveResult::Busy, TEXT("a save or load is already running"));
	}
	const FBusyScope BusyScope(bBusy);
	const FString Owner = GetNameSafe(GetGameInstance());
	if (!IsValidSlotId(SlotId))
	{
		UE_LOG(LogSpnCore, Warning, TEXT("Save [%s]: NOT saved: invalid slot id '%s'"), *Owner, *SlotId);
		return Fail(ESpnSaveResult::InvalidSlotId, FString::Printf(TEXT("'%s' is not a valid slot id"), *SlotId));
	}
	const USpnWorldStateSubsystem* WorldState = GetGameInstance() != nullptr ? GetGameInstance()->GetSubsystem<USpnWorldStateSubsystem>() : nullptr;
	if (WorldState == nullptr)
	{
		return Fail(ESpnSaveResult::WorldStateUnavailable, TEXT("USpnWorldStateSubsystem is not available"));
	}
	const USpnInventorySubsystem* Inventory = GetGameInstance()->GetSubsystem<USpnInventorySubsystem>();
	const bool bInventoryActive = Inventory != nullptr && Inventory->HasCatalog();
	const FString FilePath = GetSlotFilePath(SlotId);

	try
	{
		if (!FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*FPaths::GetPath(FilePath)))
		{
			UE_LOG(LogSpnCore, Error, TEXT("Save [%s]: NOT saved: cannot create '%s'"), *Owner, *FPaths::GetPath(FilePath));
			return Fail(ESpnSaveResult::InvalidDirectory, FString::Printf(TEXT("cannot create the directory '%s'"), *FPaths::GetPath(FilePath)), FilePath);
		}

		spn::core::StdSaveFileSystem Fs;
		spn::core::SaveSlotStore Store(Fs, ToPath(FilePath));

		// What is already there decides what we may overwrite.
		const FCandidate Existing = ReadCandidate(Fs, Store.Current());
		if (Existing.Problem && IsNewerThanThisBuild(Existing.Problem->code))
		{
			UE_LOG(LogSpnCore, Warning, TEXT("Save [%s]: NOT saved: '%s' was written by a newer build (%s)"), *Owner, *FilePath, *Describe(*Existing.Problem));
			return Fail(ESpnSaveResult::UnsupportedVersion, FString::Printf(TEXT("the existing save is from a newer build: %s"), *Describe(*Existing.Problem)), FilePath);
		}
		if (Existing.Slot)
		{
			const FString Newer = NewerKnownSection(*Existing.Slot);
			if (!Newer.IsEmpty())
			{
				UE_LOG(LogSpnCore, Warning, TEXT("Save [%s]: NOT saved: section %s of '%s' was written by a newer build"), *Owner, *Newer, *FilePath);
				return Fail(ESpnSaveResult::UnsupportedVersion, FString::Printf(TEXT("section %s was written by a newer build and would be downgraded"), *Newer), FilePath);
			}
			const TArray<FString> Foreign = NotIntegratedSections(*Existing.Slot, bInventoryActive);
			if (!Foreign.IsEmpty())
			{
				const FString List = FString::Join(Foreign, TEXT(", "));
				UE_LOG(LogSpnCore, Warning, TEXT("Save [%s]: NOT saved: '%s' holds sections this build cannot keep consistent (%s)"), *Owner, *FilePath, *List);
				return Fail(ESpnSaveResult::SectionsNotIntegrated, FString::Printf(TEXT("the slot holds %s, which would be lost; use another slot"), *List), FilePath);
			}
		}

		// Capture: synchronous, read-only on the live state. The previous slot supplies unknown optional sections to carry over.
		spn::core::GameSaveSources Sources;
		Sources.facts = &WorldState->State->Facts;
		if (bInventoryActive)
		{
			Sources.inventory = &*Inventory->State->Inventory;
		}
		auto Captured = spn::core::CaptureGameSave(Sources, Existing.Slot ? &*Existing.Slot : nullptr);
		if (!Captured)
		{
			const bool bInconsistent = Captured.Error().code == spn::core::SaveErrorCode::ConsistencyMismatch;
			UE_LOG(LogSpnCore, Error, TEXT("Save [%s]: NOT saved: capture failed (%s)"), *Owner, *Describe(Captured.Error()));
			return Fail(bInconsistent ? ESpnSaveResult::InconsistentSnapshot : ToResult(Captured.Error().code), Describe(Captured.Error()), FilePath);
		}

		if (const auto Error = Store.Save(Captured.Value(), spn::core::GameSaveSchema()))
		{
			UE_LOG(LogSpnCore, Error, TEXT("Save [%s]: saving slot '%s' to '%s' failed (%s); the previous save is unchanged"), *Owner, *SlotId, *FilePath,
				*Describe(*Error));
			return Fail(ToResult(Error->code), Describe(*Error), FilePath);
		}

		FSpnSaveStatus Status;
		Status.Source = ESpnSaveSource::Current;
		Status.Path = FilePath;
		Status.FactsRevision = WorldState->GetRevision();
		Status.InventoryRevision = bInventoryActive ? Inventory->GetRevision() : 0;
		if (Existing.Slot)
		{
			for (const std::string& Id : Existing.Slot->UnreadSections(spn::core::GameSaveSchema()))
			{
				Status.PreservedSections.Add(ToFString(Id));
			}
		}
		UE_LOG(LogSpnCore, Log, TEXT("Save [%s]: saved slot '%s' (facts revision %llu, %d unknown section(s) carried over) to '%s'"), *Owner, *SlotId,
			static_cast<unsigned long long>(Status.FactsRevision), Status.PreservedSections.Num(), *FilePath);
		OnSaved.Broadcast();
		return Status;
	}
	catch (const std::exception& Exception)
	{
		UE_LOG(LogSpnCore, Error, TEXT("Save [%s]: saving slot '%s' threw an exception: %s"), *Owner, *SlotId, *ToFString(Exception.what()));
		return Fail(ESpnSaveResult::InternalError, FString::Printf(TEXT("exception: %s"), *ToFString(Exception.what())), FilePath);
	}
	catch (...)
	{
		UE_LOG(LogSpnCore, Error, TEXT("Save [%s]: saving slot '%s' threw an unknown exception"), *Owner, *SlotId);
		return Fail(ESpnSaveResult::InternalError, TEXT("unknown exception"), FilePath);
	}
}

FSpnSaveStatus USpnSaveSubsystem::LoadSlot(const FString& SlotId)
{
	check(IsInGameThread());
	if (bBusy)
	{
		UE_LOG(LogSpnCore, Warning, TEXT("Save [%s]: request refused: a save or load is already running (called back from a callback?)"), *GetNameSafe(GetGameInstance()));
		return Fail(ESpnSaveResult::Busy, TEXT("a save or load is already running"));
	}
	const FBusyScope BusyScope(bBusy);
	const FString Owner = GetNameSafe(GetGameInstance());
	if (!IsValidSlotId(SlotId))
	{
		UE_LOG(LogSpnCore, Warning, TEXT("Save [%s]: NOT loaded: invalid slot id '%s'; state unchanged"), *Owner, *SlotId);
		return Fail(ESpnSaveResult::InvalidSlotId, FString::Printf(TEXT("'%s' is not a valid slot id"), *SlotId));
	}
	USpnWorldStateSubsystem* WorldState = GetGameInstance() != nullptr ? GetGameInstance()->GetSubsystem<USpnWorldStateSubsystem>() : nullptr;
	if (WorldState == nullptr)
	{
		return Fail(ESpnSaveResult::WorldStateUnavailable, TEXT("USpnWorldStateSubsystem is not available"));
	}
	USpnInventorySubsystem* Inventory = GetGameInstance()->GetSubsystem<USpnInventorySubsystem>();
	const bool bInventoryActive = Inventory != nullptr && Inventory->HasCatalog();
	const FString FilePath = GetSlotFilePath(SlotId);

	try
	{
		spn::core::StdSaveFileSystem Fs;
		spn::core::SaveSlotStore Store(Fs, ToPath(FilePath));
		const std::filesystem::path Paths[2] = {Store.Current(), Store.Previous()};

		FString CurrentProblem;
		FString LastProblem;
		ESpnSaveResult LastResult = ESpnSaveResult::NotFound;
		bool bAnyFile = false;

		for (int32 Generation = 0; Generation < 2; ++Generation)
		{
			const bool bPrevious = Generation == 1;
			FCandidate Candidate = ReadCandidate(Fs, Paths[Generation]);
			if (!Candidate.bExists)
			{
				if (!bPrevious)
				{
					CurrentProblem = TEXT("no main file");
				}
				continue;
			}
			bAnyFile = true;

			if (!Candidate.Slot)
			{
				const spn::core::SaveError& Problem = *Candidate.Problem;
				if (IsNewerThanThisBuild(Problem.code))
				{
					// intact but written by a newer build: falling back to an older generation would silently roll the player back
					UE_LOG(LogSpnCore, Warning, TEXT("Save [%s]: NOT loaded: %s was written by a newer build (%s); state unchanged"), *Owner, bPrevious ? TEXT("the previous generation") : TEXT("the save"), *Describe(Problem));
					FSpnSaveStatus Status = Fail(ESpnSaveResult::UnsupportedVersion, Describe(Problem), FilePath);
					Status.CurrentProblem = CurrentProblem;
					return Status;
				}
				LastProblem = Describe(Problem);
				LastResult = ToResult(Problem.code);
				if (!bPrevious)
				{
					CurrentProblem = LastProblem;
				}
				continue;
			}

			const FString Newer = NewerKnownSection(*Candidate.Slot);
			if (!Newer.IsEmpty())
			{
				UE_LOG(LogSpnCore, Warning, TEXT("Save [%s]: NOT loaded: section %s of slot '%s' was written by a newer build; state unchanged"), *Owner, *Newer, *SlotId);
				FSpnSaveStatus Status = Fail(ESpnSaveResult::UnsupportedVersion, FString::Printf(TEXT("section %s was written by a newer build"), *Newer), FilePath);
				Status.CurrentProblem = CurrentProblem;
				return Status;
			}
			const TArray<FString> Foreign = NotIntegratedSections(*Candidate.Slot, bInventoryActive);
			if (!Foreign.IsEmpty())
			{
				// not a problem of this generation: older ones would be just as unrestorable, and loading only part of the slot is not allowed
				const FString List = FString::Join(Foreign, TEXT(", "));
				UE_LOG(LogSpnCore, Warning, TEXT("Save [%s]: NOT loaded: slot '%s' holds sections this build cannot restore (%s); state unchanged"), *Owner, *SlotId, *List);
				return Fail(ESpnSaveResult::SectionsNotIntegrated, FString::Printf(TEXT("the slot holds %s"), *List), FilePath);
			}

			// phase 1: build NEW objects, change nothing live
			spn::core::GameLoadContext Context;
			Context.itemCatalog = bInventoryActive ? Inventory->State->Catalog.get() : nullptr;
			auto Game = spn::core::ParseGameSave(*Candidate.Slot, Context);
			if (!Game)
			{
				LastProblem = Describe(Game.Error());
				LastResult = ToResult(Game.Error().code);
				if (!bPrevious)
				{
					CurrentProblem = LastProblem;
				}
				UE_LOG(LogSpnCore, Warning, TEXT("Save [%s]: %s of slot '%s' rejected (%s)"), *Owner, bPrevious ? TEXT("previous generation") : TEXT("main file"), *SlotId, *LastProblem);
				continue;
			}

			// phase 2: swap EVERYTHING first (moves of ordered maps do not throw and call nobody), then notify. Nothing between the two swaps can
			// observe or modify a half-loaded state, and every listener sees the complete loaded state.
			const uint64 PreviousRevision = WorldState->GetRevision();
			FSpnSaveStatus Status;
			Status.Source = bPrevious ? ESpnSaveSource::Previous : ESpnSaveSource::Current;
			Status.CurrentProblem = bPrevious ? CurrentProblem : FString();
			Status.Path = FilePath;
			WorldState->State->Facts = std::move(Game.Value().facts);
			if (bInventoryActive)
			{
				if (Game.Value().inventory)
				{
					Inventory->State->Inventory = std::move(*Game.Value().inventory);
					Status.bInventoryRestored = true;
				}
				else
				{
					// a WorldFacts-only save describes the whole state: the inventory starts fresh (reported, never silent)
					Status.bInventoryResetToEmpty = Inventory->GetRevision() > 0 || !Inventory->ListContainers().IsEmpty();
					Inventory->ResetInventory();
				}
				Status.InventoryRevision = Inventory->GetRevision();
			}
			Status.FactsRevision = WorldState->GetRevision();
			UE_LOG(LogSpnCore, Log, TEXT("Save [%s]: loaded slot '%s' from the %s (facts revision %llu -> %llu%s)%s"), *Owner, *SlotId,
				bPrevious ? TEXT("PREVIOUS generation") : TEXT("main file"), static_cast<unsigned long long>(PreviousRevision),
				static_cast<unsigned long long>(Status.FactsRevision),
				Status.bInventoryRestored ? *FString::Printf(TEXT("; inventory restored, revision %llu"), static_cast<unsigned long long>(Status.InventoryRevision))
					: Status.bInventoryResetToEmpty ? TEXT("; inventory reset to empty: the slot has none") : TEXT(""),
				bPrevious ? *FString::Printf(TEXT("; main file: %s"), *CurrentProblem) : TEXT(""));
			WorldState->OnWorldStateReplaced.Broadcast();
			if (bInventoryActive)
			{
				Inventory->OnInventoryReplaced.Broadcast();
			}
			OnLoaded.Broadcast();
			return Status;
			return Status;
		}

		if (!bAnyFile)
		{
			UE_LOG(LogSpnCore, Warning, TEXT("Save [%s]: NOT loaded: no save in slot '%s' ('%s'); state unchanged"), *Owner, *SlotId, *FilePath);
			return Fail(ESpnSaveResult::NotFound, TEXT("no save in this slot"), FilePath);
		}
		UE_LOG(LogSpnCore, Warning, TEXT("Save [%s]: NOT loaded: slot '%s' has no usable generation (%s); state unchanged"), *Owner, *SlotId, *LastProblem);
		FSpnSaveStatus Status = Fail(LastResult == ESpnSaveResult::NotFound ? ESpnSaveResult::Corrupt : LastResult, LastProblem, FilePath);
		Status.CurrentProblem = CurrentProblem;
		return Status;
	}
	catch (const std::exception& Exception)
	{
		UE_LOG(LogSpnCore, Error, TEXT("Save [%s]: loading slot '%s' threw an exception: %s; state unchanged"), *Owner, *SlotId, *ToFString(Exception.what()));
		return Fail(ESpnSaveResult::InternalError, FString::Printf(TEXT("exception: %s"), *ToFString(Exception.what())), FilePath);
	}
	catch (...)
	{
		UE_LOG(LogSpnCore, Error, TEXT("Save [%s]: loading slot '%s' threw an unknown exception; state unchanged"), *Owner, *SlotId);
		return Fail(ESpnSaveResult::InternalError, TEXT("unknown exception"), FilePath);
	}
}
