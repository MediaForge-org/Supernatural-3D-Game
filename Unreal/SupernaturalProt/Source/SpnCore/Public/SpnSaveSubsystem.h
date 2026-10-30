#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SpnMapSubsystem.h"

#include "SpnSaveSubsystem.generated.h"

// Unreal integration of the unified save slot of the SPN core (M2B2 -> M2B3.2). ENGINEERING PROPOSAL, not master canon.
//
// WHAT IS SAVED: one file per slot, "SPN-SAVESLOT 1" (checksummed sections), with
//   world.facts       the whole WorldFacts store of USpnWorldStateSubsystem (doors, pickups, placeholder counts, journal)  - required
//   player.inventory  the InventoryState of USpnInventorySubsystem, ONLY while that subsystem has a catalog (M2B3.3)        - optional
//   slot.meta         the facts revision (and inventory revision) the snapshot was taken at                                - optional
// Map (world.map) and object records (world.objects) have no Unreal owner yet. A slot that already contains a section without an owner
// (also player.inventory while the inventory has no catalog) is neither overwritten nor loaded (ESpnSaveResult::SectionsNotIntegrated):
// silently dropping or half-restoring it would lose or corrupt data. Unknown optional sections are carried over unchanged.
//
// SNAPSHOT GUARANTEE: the capture runs synchronously on the game thread and does not mutate the live state; the revisions of WorldFacts
// and the inventory are compared before and after serialisation. This is NOT a claim of general cross-system consistency (the map is not
// integrated; actors are not saved).
//
// LOAD IS TWO-PHASE AND MULTI-SYSTEM SAFE: phase 1 parses facts AND inventory into new objects (nothing live changes; any failure leaves
// both untouched); phase 2 swaps both in without any callback in between; only then are OnWorldStateReplaced / OnInventoryReplaced /
// OnLoaded broadcast, so a listener always sees the complete loaded state. A save or load requested from inside such a callback is
// refused with Busy.
//
// Core types stay in private headers (Decision 0006); this interface uses Unreal types only. No exception crosses it: every Core call is
// guarded and an unexpected exception becomes ESpnSaveResult::InternalError.

/** Outcome category of a save or load. Appended to, never reordered. */
UENUM()
enum class ESpnSaveResult : uint8
{
	Ok,
	/** Slot id breaks IsValidSlotId. Nothing was touched. */
	InvalidSlotId,
	/** The save directory is not usable (SetSaveDirectory rules) or cannot be created. Nothing was touched. */
	InvalidDirectory,
	/** The world-state subsystem this one depends on is missing. */
	WorldStateUnavailable,
	/** Load: neither the main file nor the previous generation exists. */
	NotFound,
	/** Reading, writing, flushing or renaming failed. A failed save leaves the previous save intact. */
	Io,
	/** Load: file damaged (truncated, bad checksum, not a save file, ...) and no valid previous generation. */
	Corrupt,
	/** The save was written by a newer build (container, section or required section). Never rolled back silently. */
	UnsupportedVersion,
	/** The checksums are fine but the world.facts content was rejected by its own parser (or by a consistency check). */
	Rejected,
	/** The slot holds sections (inventory/map/objects) that this build cannot yet restore or keep consistent. */
	SectionsNotIntegrated,
	/** Save: the facts changed while the snapshot was being taken; nothing was written. */
	InconsistentSnapshot,
	/** An unexpected exception was caught at the boundary. */
	InternalError,
	/** A save or load is already running (a callback called back in). Nothing was touched. */
	Busy,
	/** The inventory was saved against item definitions that differ from the current catalog; refused unless the caller accepts it explicitly. */
	CatalogMismatch,
};

SPNCORE_API const TCHAR* LexToString(ESpnSaveResult Result);

/** What a load did with the inventory. Explicit so that a migration is never silent. */
enum class ESpnInventoryLoadOutcome : uint8
{
	/** The inventory subsystem has no catalog (inactive); the slot's inventory section, if any, would have blocked the load. */
	NotIntegrated,
	/** player.inventory was read and replaced the inventory. */
	Restored,
	/** The slot has no player.inventory (a valid save from before inventories existed): the inventory is now EMPTY. This is a migration, reported here. The save file itself is not touched. */
	MigratedEmptyNoSection,
};

/** Result of comparing the catalog fingerprint stored in the slot with the current catalog (see Core CatalogFingerprint.h). */
enum class ESpnCatalogCheck : uint8
{
	/** No inventory in the slot or inventory not integrated. */
	NotApplicable,
	/** Inventory without a fingerprint (saved before M2B3.4): loaded after the Core validated it against the current catalog, but NOT verified. */
	LegacyNoFingerprint,
	/** The fingerprint belongs to an older inventory revision (an older build re-saved the inventory): treated like legacy. */
	StaleFingerprint,
	/** Same gameplay definitions. */
	Match,
	/** Different definitions, accepted because FSpnLoadOptions::bAcceptCatalogMismatch was set; the inventory was validated against the current catalog. */
	MismatchAccepted,
};

SPNCORE_API const TCHAR* LexToString(ESpnInventoryLoadOutcome Outcome);
SPNCORE_API const TCHAR* LexToString(ESpnCatalogCheck Check);

/** Explicit decisions of the caller for a load. The defaults are the safe ones. */
struct FSpnLoadOptions
{
	/**
	 * Load an inventory that was saved against different item definitions. NEVER the default: a mismatch is refused (CatalogMismatch) and nothing is
	 * reset. Accepting still validates the inventory against the current catalog (unknown items, over-full stacks and so on are rejected).
	 */
	bool bAcceptCatalogMismatch = false;
};

/** Which generation a load came from. */
enum class ESpnSaveSource : uint8
{
	None,
	Current,
	/** The main file was unusable and the previous generation was loaded (see FSpnSaveStatus::CurrentProblem). */
	Previous,
};

struct FSpnSaveStatus
{
	ESpnSaveResult Result = ESpnSaveResult::Ok;
	/** Human-readable reason (for logs and UI), empty on success. Not for program logic. */
	FString Detail;
	/** Load: generation that was loaded. Save: Current. */
	ESpnSaveSource Source = ESpnSaveSource::None;
	/** Load from Previous: why the main file was not used. */
	FString CurrentProblem;
	/** Save: ids of sections from an existing slot that this build does not know and that were carried over unchanged. */
	TArray<FString> PreservedSections;
	/** Load: player.inventory was in the slot and has been restored (needs the inventory subsystem to have a catalog). */
	bool bInventoryRestored = false;
	/** Load: the slot had no player.inventory (a WorldFacts-only save) and the active inventory was reset to empty, because the slot describes the whole state. */
	bool bInventoryResetToEmpty = false;
	/** Load: world.map was in the slot and replaced the map state (markers, waypoint, discovery). Needs USpnMapSubsystem to have a definition. */
	bool bMapRestored = false;
	/** Load: the slot had no world.map (a save from before the map existed) and the markers were cleared. Reported, never silent. */
	bool bMapResetToEmpty = false;
	/** Inventory revision that was saved or restored (0 if the inventory is not part of the save). */
	uint64 InventoryRevision = 0;
	/** Load: what happened to the inventory (see ESpnInventoryLoadOutcome). */
	ESpnInventoryLoadOutcome InventoryOutcome = ESpnInventoryLoadOutcome::NotIntegrated;
	/** Load: result of the catalog fingerprint comparison. */
	ESpnCatalogCheck CatalogCheck = ESpnCatalogCheck::NotApplicable;
	/** Load: fingerprint stored in the slot (empty if none) and fingerprint of the current catalog. Save: StoredFingerprint is the one written. */
	FString StoredFingerprint;
	FString CurrentFingerprint;
	/** WorldFacts revision that was saved, or that was loaded. */
	uint64 FactsRevision = 0;
	/** Absolute path of the slot's main file (empty for InvalidSlotId/InvalidDirectory). */
	FString Path;

	bool IsOk() const { return Result == ESpnSaveResult::Ok; }
};
DECLARE_MULTICAST_DELEGATE_OneParam(FSpnSaveFinished, const FSpnSaveStatus&);


/**
 * Saves and loads the game state to named slots. Game-thread only. Depends on USpnWorldStateSubsystem (initialised first).
 * A failed load never changes the live state and does not broadcast OnWorldStateReplaced.
 */
UCLASS()
class SPNCORE_API USpnSaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/**
	 * Writes the slot: temporary file, flushed, read back and verified, previous generation kept as "<file>.prev", then published by one
	 * rename. Unknown optional sections of an existing slot are carried over. On any failure the existing save is unchanged.
	 */
	FSpnSaveStatus SaveSlot(const FString& SlotId);

	/**
	 * Replaces the WorldFacts of USpnWorldStateSubsystem with the slot's content (two-phase: parse into new objects, swap only on success).
	 * If the main file is damaged or rejected, the previous generation is tried; a save from a newer build is never rolled back.
	 */
	FSpnSaveStatus LoadSlot(const FString& SlotId, const FSpnLoadOptions& Options = FSpnLoadOptions());

	/** The result of the most recent SaveSlot / LoadSlot (success or failure); default-constructed (Ok) before the first call. */
	const FSpnSaveStatus& GetLastSaveStatus() const { return LastSaveStatus; }
	const FSpnSaveStatus& GetLastLoadStatus() const { return LastLoadStatus; }

	/** Read-only description of a slot for the slot list (Night Build 01). Never changes the live state or the files. */
	struct FSpnSlotInfo
	{
		FString SlotId;
		bool bExists = false;
		/** Ok = the slot could be loaded right now. Otherwise the reason (Corrupt, UnsupportedVersion, CatalogMismatch, SectionsNotIntegrated, ...). */
		ESpnSaveResult Result = ESpnSaveResult::NotFound;
		FString Detail;
		/** The main file was damaged and the previous generation would be used. */
		bool bFromPrevious = false;
		FDateTime Modified = FDateTime(0);
		uint64 FactsRevision = 0;
		bool bHasInventory = false;
		uint64 InventoryRevision = 0;
		bool bHasMap = false;
		int32 MapMarkers = 0;
		ESpnCatalogCheck CatalogCheck = ESpnCatalogCheck::NotApplicable;
	};

	/** Slot ids that have a main file or a previous generation, newest first (by file time). */
	TArray<FString> ListSlots() const;

	/** Dry-run of a load: reads and parses the slot exactly like LoadSlot would, but swaps nothing and broadcasts nothing. */
	FSpnSlotInfo InspectSlot(const FString& SlotId, const FSpnLoadOptions& Options = FSpnLoadOptions()) const;

	/** True if the main file or the previous generation of the slot exists (no validation). */
	bool DoesSlotExist(const FString& SlotId) const;

	/** Slot rule: 1..64 chars, first [a-z], rest [a-z0-9_] (same as the world-state placeholder slots; no separators, so no path traversal). */
	static bool IsValidSlotId(const FString& SlotId);

	/** Current save root. Default: <Project>/Saved/SpnSaves (slot files live in <root>/<slot>/game.spnsave). */
	FString GetSaveDirectory() const;

	/**
	 * Replaces the save root (tests, tools). Must be absolute, free of ".." and of characters Windows forbids in names, and short enough
	 * for a slot path. The directory is created on the first save. Returns InvalidDirectory (and keeps the old one) otherwise.
	 */
	FSpnSaveStatus SetSaveDirectory(const FString& AbsoluteDirectory);

	/** Restores the default save root. */
	void ResetSaveDirectory();

	/** Absolute path of the slot's main file; empty for an invalid slot id. */
	FString GetSlotFilePath(const FString& SlotId) const;

	/** Broadcast after a successful SaveSlot / LoadSlot (after the world state was replaced). */
	/** After every SaveSlot / LoadSlot, successful or not, with the result (a failed load changed nothing). Fires before the call returns, while busy. */
	FSpnSaveFinished OnSaveFinished;
	FSpnSaveFinished OnLoadFinished;
	FSimpleMulticastDelegate OnSaved;
	FSimpleMulticastDelegate OnLoaded;

private:
	FString SaveDirectory;
	/** Set while a save or load runs, so a callback cannot start another one in the middle of it. */
	bool bBusy = false;
	FSpnSaveStatus DoSave(const FString& SlotId);
	FSpnSaveStatus DoLoad(const FString& SlotId, const FSpnLoadOptions& Options);
	FSpnSaveStatus LastSaveStatus;
	FSpnSaveStatus LastLoadStatus;
};
