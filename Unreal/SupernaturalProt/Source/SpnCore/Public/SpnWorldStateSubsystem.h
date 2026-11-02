#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Templates/PimplPtr.h"

#include "SpnWorldStateSubsystem.generated.h"

// Unreal bridge to the persistent world state of the SPN core (Docs/Proposals/0004,
// increment 3). ENGINEERING PROPOSAL, not master canon.
//
// Core types (spn::core::WorldFacts, InteractionRegistry) stay behind a private
// implementation: they are not exported from the SpnCore DLL and their headers are a
// private include path (Docs/Decisions/0006), so this interface uses Unreal types only.
//
// The slot file is a PLACEHOLDER for the segmented, crash-safe save system of the
// master (M5): one WorldFacts v1 file per slot, Saved/SpnSaves/<slot>/worldfacts.spnfacts.

/** Result of SaveToSlot / LoadFromSlot. */
enum class ESpnSlotResult : uint8
{
	Ok,
	InvalidSlotName, // slot name breaks IsValidSlotName
	SlotNotFound,    // load: no save file in the slot
	ReadFailed,      // load: file exists but could not be read
	WriteFailed,     // save: temporary file could not be written, verified or committed
	CorruptData,     // load: WorldFacts rejected the file; see Detail and Line
};

SPNCORE_API const TCHAR* LexToString(ESpnSlotResult Result);

class USpnSaveSubsystem;

struct FSpnSlotStatus
{
	ESpnSlotResult Result = ESpnSlotResult::Ok;
	/** CorruptData: the core's error text (spn::core::ToString(FactErrorCode), e.g. "journal mismatch"); else empty. */
	FString Detail;
	/** CorruptData: 1-based line in the file, 0 if not applicable. */
	int32 Line = 0;

	bool IsOk() const { return Result == ESpnSlotResult::Ok; }
};

/** Use = E / DualSense Square. Force (NightBuild04) = deliberate, violent alternative (break a door). New values are appended. */
enum class ESpnInteractionVerb : uint8
{
	Use,
	Force,
};

/** Result of TryInteract. */
enum class ESpnInteractResult : uint8
{
	Changed,      // facts were written; see Feedback
	Unchanged,    // accepted, but nothing had to change (nothing journaled)
	Refused,      // the object declined; see Feedback (e.g. placeholder:feedback.door.locked)
	InvalidId,    // stable id breaks the D2 rule
	UnknownKind,  // kind is malformed or not registered
	StateCorrupt, // a fact or default has an unexpected type
	Failed,       // WorldFacts rejected a command; see Detail
};

SPNCORE_API const TCHAR* LexToString(ESpnInteractResult Result);

struct FSpnInteractStatus
{
	ESpnInteractResult Result = ESpnInteractResult::Failed;
	/** Feedback content id for UI/audio, empty if none. */
	FString Feedback;
	/** Offending id/kind/key or core error name; empty if none. */
	FString Detail;
};

/**
 * Authored initial values of a placed object, by name (Proposal 0007). Doors use Bools;
 * pickups also use Ints (amount) and Strings (item). Used while the matching fact is absent.
 */
struct FSpnInteractDefaults
{
	TMap<FString, bool> Bools;
	TMap<FString, int64> Ints;
	TMap<FString, FString> Strings;
};

/** One item count of a holder (Proposal 0007, placeholder inventory). */
struct FSpnInventoryEntry
{
	FString Item;
	int64 Count = 0;
};

/**
 * Owns the world's WorldFacts store and the interaction registry for one game
 * instance, and saves/loads the store to named slots in WorldFacts format v1.
 * A failed load never changes the current store.
 */
UCLASS()
class SPNCORE_API USpnWorldStateSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	USpnWorldStateSubsystem();
	virtual ~USpnWorldStateSubsystem() override;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/**
	 * Applies Verb to the object (Kind, StableId) through the registry. BoolDefaults are
	 * the authored initial states (e.g. "open" -> false) used while a fact is absent.
	 * Cause goes to the journal as audit text. On any error the store is unchanged.
	 */
	FSpnInteractStatus TryInteract(const FString& Kind, const FString& StableId, ESpnInteractionVerb Verb,
		const TMap<FString, bool>& BoolDefaults, const FString& Cause);

	/**
	 * As above, for objects that depend on who acts or need int/string defaults (pickups).
	 * Holder is the actor's inventory id (DefaultHolder() in single player; co-op later passes
	 * the player's own id). A malformed Holder gives InvalidId and changes nothing.
	 */
	FSpnInteractStatus TryInteract(const FString& Kind, const FString& StableId, ESpnInteractionVerb Verb,
		const FSpnInteractDefaults& Defaults, const FString& Holder, const FString& Cause);

	/** Count of Item held by Holder; 0 if none. Unset for a malformed id or a wrongly typed count fact. */
	TOptional<int64> ReadInventoryCount(const FString& Holder, const FString& Item) const;

	/** All items of Holder with a count, ordered by item id. Empty for a malformed Holder or a wrongly typed count. */
	TArray<FSpnInventoryEntry> ListInventory(const FString& Holder) const;

	/** Bool fact by key; unset if the fact is absent or not a bool. */
	TOptional<bool> ReadFlag(const FString& Key) const;

	/**
	 * Sets a bool fact (Night Build 01: persistent "taken / opened" flags of world objects whose CONTENT lives in the inventory). Key rule: first
	 * [a-z], rest [a-z0-9_.], at most 128 chars. False (store unchanged) for a bad key, or if the key holds a non-bool value.
	 */
	bool SetFlag(const FString& Key, bool bValue, const FString& Cause);

	/** New game: replaces the store with an empty one and broadcasts OnWorldStateReplaced. Does not touch files. */
	void ResetToEmpty();

	/** Number of journaled changes (0 for an empty store). */
	uint64 GetRevision() const;

	/** Writes the store to the slot: temporary file, flushed, verified, then atomically renamed over the old file. */
	FSpnSlotStatus SaveToSlot(const FString& Slot) const;

	/** Replaces the store with the slot's content. On any failure the store is unchanged and OnWorldStateReplaced is not broadcast. */
	FSpnSlotStatus LoadFromSlot(const FString& Slot);

	/** Placeholder slot rule: 1..64 chars, first [a-z], rest [a-z0-9_]. No dots, separators or upper case. */
	static bool IsValidSlotName(const FString& Slot);

	/** Absolute path of the slot's save file; empty for an invalid slot name. */
	static FString GetSlotFilePath(const FString& Slot);

	/** D2 stable id rule of the core (1..96 chars, [a-z][a-z0-9_.]*), for editor-side validation. */
	static bool IsValidInteractableId(const FString& StableId);

	/** Prototype id rules of the core (Proposal 0007): holder 1..24 chars [a-z][a-z0-9_.]*, item 1..48 chars [a-z][a-z0-9_]*. */
	static bool IsValidHolderId(const FString& Holder);
	static bool IsValidItemId(const FString& Item);
	/** Holder used in single player ("player.local"). */
	static FString DefaultHolder();

	/** Kind and taken-fact key of the placeholder test pickup (spn::core Pickup). */
	static FString TestPickupKind();
	static FString TestPickupTakenKey(const FString& StableId);

	/** Kind and fact key of the placeholder test door (spn::core TestDoor). */
	static FString TestDoorKind();
	static FString TestDoorOpenKey(const FString& StableId);
	static FString TestDoorLockedKey(const FString& StableId);
	static FString TestDoorBrokenKey(const FString& StableId);
	static FString TestDoorDisabledKey(const FString& StableId);
	/** Fact `placeholder.key.<KeyId>.held`: the player owns this key (set by key objects, tests and, later, the inventory). */
	static FString TestKeyHeldKey(const FString& KeyId);

	/** Kind and fact key of the placeholder light switch (spn::core Devices: placeholder.switch.<id>.on). */
	static FString SwitchKind();
	static FString SwitchOnKey(const FString& StableId);

	/** Broadcast after a successful LoadFromSlot replaced the store. */
	FSimpleMulticastDelegate OnWorldStateReplaced;

private:
	/** USpnSaveSubsystem captures and replaces the Core state directly (Private/SpnWorldStateInternal.h). */
	friend class USpnSaveSubsystem;

	/** Owning game instance name for logs, so separate instances (PIE sessions, tests) can be told apart. */
	FString DescribeOwner() const;

	struct FState;
	TPimplPtr<FState> State;
};
