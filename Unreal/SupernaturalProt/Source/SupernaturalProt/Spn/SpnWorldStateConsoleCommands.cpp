#include "SupernaturalProt.h"
#include "SpnSaveSubsystem.h"
#include "SpnWorldStateSubsystem.h"

#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "HAL/IConsoleManager.h"

// PLACEHOLDER debug commands for manual save/load tests (Docs/Proposals/0004, section 5.2).
// Not a save system or save UI: they only call USpnWorldStateSubsystem on the console's world.

#if !UE_BUILD_SHIPPING

namespace
{
	const TCHAR* const DefaultDebugSlot = TEXT("debug");

	const TCHAR* WorldTypeName(EWorldType::Type Type)
	{
		switch (Type)
		{
		case EWorldType::Game: return TEXT("Game");
		case EWorldType::PIE: return TEXT("PIE");
		case EWorldType::Editor: return TEXT("Editor");
		case EWorldType::EditorPreview: return TEXT("EditorPreview");
		default: return TEXT("Other");
		}
	}

	/** "<world> (PIE|Game|Editor..., <game instance>)" so the log shows which session a command hit. */
	FString DescribeWorld(const UWorld* World)
	{
		if (World == nullptr)
		{
			return TEXT("no world");
		}
		return FString::Printf(TEXT("%s (%s, %s)"), *World->GetPathName(), WorldTypeName(World->WorldType), *GetNameSafe(World->GetGameInstance()));
	}

	USpnWorldStateSubsystem* FindWorldState(const UWorld* World, const TCHAR* Command)
	{
		const UGameInstance* GameInstance = World != nullptr ? World->GetGameInstance() : nullptr;
		USpnWorldStateSubsystem* WorldState = GameInstance != nullptr ? GameInstance->GetSubsystem<USpnWorldStateSubsystem>() : nullptr;
		if (WorldState == nullptr)
		{
			UE_LOG(LogSupernaturalProt, Warning, TEXT("%s FAILED: no world state in %s; run it while playing (PIE or game)"), Command, *DescribeWorld(World));
		}
		return WorldState;
	}

	FString SlotFromArgs(const TArray<FString>& Args)
	{
		return Args.Num() > 0 ? Args[0] : FString(DefaultDebugSlot);
	}

	void SaveWorld(const TArray<FString>& Args, UWorld* World)
	{
		if (USpnWorldStateSubsystem* WorldState = FindWorldState(World, TEXT("Spn.SaveWorld")))
		{
			const FString Slot = SlotFromArgs(Args);
			const FSpnSlotStatus Status = WorldState->SaveToSlot(Slot);
			if (Status.IsOk())
			{
				UE_LOG(LogSupernaturalProt, Display, TEXT("Spn.SaveWorld OK: slot '%s', revision %llu, in %s, file %s"), *Slot,
					static_cast<unsigned long long>(WorldState->GetRevision()), *DescribeWorld(World), *USpnWorldStateSubsystem::GetSlotFilePath(Slot));
			}
			else
			{
				UE_LOG(LogSupernaturalProt, Warning, TEXT("Spn.SaveWorld FAILED: slot '%s': %s, in %s"), *Slot, LexToString(Status.Result), *DescribeWorld(World));
			}
		}
	}

	void LoadWorld(const TArray<FString>& Args, UWorld* World)
	{
		if (USpnWorldStateSubsystem* WorldState = FindWorldState(World, TEXT("Spn.LoadWorld")))
		{
			const FString Slot = SlotFromArgs(Args);
			const FSpnSlotStatus Status = WorldState->LoadFromSlot(Slot);
			if (Status.IsOk())
			{
				UE_LOG(LogSupernaturalProt, Display, TEXT("Spn.LoadWorld OK: slot '%s', revision %llu, in %s"), *Slot,
					static_cast<unsigned long long>(WorldState->GetRevision()), *DescribeWorld(World));
			}
			else
			{
				UE_LOG(LogSupernaturalProt, Warning, TEXT("Spn.LoadWorld FAILED: slot '%s': %s%s%s, in %s; nothing changed"), *Slot,
					LexToString(Status.Result), Status.Detail.IsEmpty() ? TEXT("") : TEXT(" "), *Status.Detail, *DescribeWorld(World));
			}
		}
	}

	void ListInventory(const TArray<FString>& Args, UWorld* World)
	{
		if (USpnWorldStateSubsystem* WorldState = FindWorldState(World, TEXT("Spn.Inventory")))
		{
			const FString Holder = Args.Num() > 0 ? Args[0] : USpnWorldStateSubsystem::DefaultHolder();
			if (!USpnWorldStateSubsystem::IsValidHolderId(Holder))
			{
				UE_LOG(LogSupernaturalProt, Warning, TEXT("Spn.Inventory FAILED: invalid holder '%s' (allowed: [a-z][a-z0-9_.]*, at most 24 chars)"), *Holder);
				return;
			}
			const TArray<FSpnInventoryEntry> Entries = WorldState->ListInventory(Holder);
			FString Summary = FString::Printf(TEXT("Inventory of %s (PLACEHOLDER):"), *Holder);
			if (Entries.IsEmpty())
			{
				Summary += TEXT(" empty");
			}
			for (const FSpnInventoryEntry& Entry : Entries)
			{
				Summary += FString::Printf(TEXT(" %s=%lld"), *Entry.Item, static_cast<long long>(Entry.Count));
			}
			UE_LOG(LogSupernaturalProt, Display, TEXT("Spn.Inventory OK: %s, revision %llu, in %s"), *Summary,
				static_cast<unsigned long long>(WorldState->GetRevision()), *DescribeWorld(World));
			if (GEngine != nullptr)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, Summary);
			}
		}
	}

	USpnSaveSubsystem* FindSave(const UWorld* World, const TCHAR* Command)
	{
		const UGameInstance* GameInstance = World != nullptr ? World->GetGameInstance() : nullptr;
		USpnSaveSubsystem* Save = GameInstance != nullptr ? GameInstance->GetSubsystem<USpnSaveSubsystem>() : nullptr;
		if (Save == nullptr)
		{
			UE_LOG(LogSupernaturalProt, Warning, TEXT("%s FAILED: no save subsystem in %s; run it while playing (PIE or game)"), Command, *DescribeWorld(World));
		}
		return Save;
	}

	void LogSaveStatus(const TCHAR* Command, const FString& Slot, const FSpnSaveStatus& Status, const UWorld* World)
	{
		if (Status.IsOk())
		{
			UE_LOG(LogSupernaturalProt, Display, TEXT("%s OK: slot '%s', facts revision %llu, %s%s, file %s, in %s"), Command, *Slot,
				static_cast<unsigned long long>(Status.FactsRevision),
				Status.Source == ESpnSaveSource::Previous ? TEXT("RECOVERED from the previous generation") : TEXT("current generation"),
				Status.Source == ESpnSaveSource::Previous ? *FString::Printf(TEXT(" (main file: %s)"), *Status.CurrentProblem) : TEXT(""), *Status.Path,
				*DescribeWorld(World));
		}
		else
		{
			UE_LOG(LogSupernaturalProt, Warning, TEXT("%s FAILED: slot '%s': %s: %s, in %s; nothing changed"), Command, *Slot, LexToString(Status.Result), *Status.Detail,
				*DescribeWorld(World));
		}
	}

	void SaveGame(const TArray<FString>& Args, UWorld* World)
	{
		if (USpnSaveSubsystem* Save = FindSave(World, TEXT("Spn.SaveGame")))
		{
			const FString Slot = SlotFromArgs(Args);
			LogSaveStatus(TEXT("Spn.SaveGame"), Slot, Save->SaveSlot(Slot), World);
		}
	}

	void LoadGame(const TArray<FString>& Args, UWorld* World)
	{
		if (USpnSaveSubsystem* Save = FindSave(World, TEXT("Spn.LoadGame")))
		{
			const FString Slot = SlotFromArgs(Args);
			LogSaveStatus(TEXT("Spn.LoadGame"), Slot, Save->LoadSlot(Slot), World);
		}
	}

	FAutoConsoleCommandWithWorldAndArgs InventoryCommand(TEXT("Spn.Inventory"),
		TEXT("PLACEHOLDER debug: print the item counts of a holder. Usage: Spn.Inventory [holder] (default: player.local)"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateStatic(&ListInventory));

	FAutoConsoleCommandWithWorldAndArgs SaveWorldCommand(TEXT("Spn.SaveWorld"),
		TEXT("PLACEHOLDER debug (legacy worldfacts.spnfacts file; kept until Spn.SaveGame is validated). Usage: Spn.SaveWorld [slot] (default slot: debug)"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateStatic(&SaveWorld));

	FAutoConsoleCommandWithWorldAndArgs LoadWorldCommand(TEXT("Spn.LoadWorld"),
		TEXT("PLACEHOLDER debug: replace the world state with a saved slot. Usage: Spn.LoadWorld [slot] (default slot: debug)"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateStatic(&LoadWorld));

	FAutoConsoleCommandWithWorldAndArgs SaveGameCommand(TEXT("Spn.SaveGame"),
		TEXT("Save the game state (WorldFacts) with the unified, checksummed save slot (USpnSaveSubsystem). Usage: Spn.SaveGame [slot] (default slot: debug). Saved/SpnSaves/<slot>/game.spnsave"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateStatic(&SaveGame));

	FAutoConsoleCommandWithWorldAndArgs LoadGameCommand(TEXT("Spn.LoadGame"),
		TEXT("Load a slot written by Spn.SaveGame; falls back to the previous generation if the main file is damaged. Usage: Spn.LoadGame [slot] (default slot: debug)"),
		FConsoleCommandWithWorldAndArgsDelegate::CreateStatic(&LoadGame));
}

#endif
