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

struct FSpnSlotStatus
{
	ESpnSlotResult Result = ESpnSlotResult::Ok;
	/** CorruptData: the core's error text (spn::core::ToString(FactErrorCode), e.g. "journal mismatch"); else empty. */
	FString Detail;
	/** CorruptData: 1-based line in the file, 0 if not applicable. */
	int32 Line = 0;

	bool IsOk() const { return Result == ESpnSlotResult::Ok; }
};

/** D1 (Proposal 0004): only Use exists today. New values are appended. */
enum class ESpnInteractionVerb : uint8
{
	Use,
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

	/** Bool fact by key; unset if the fact is absent or not a bool. */
	TOptional<bool> ReadFlag(const FString& Key) const;

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

	/** Kind and fact key of the placeholder test door (spn::core TestDoor). */
	static FString TestDoorKind();
	static FString TestDoorOpenKey(const FString& StableId);
	static FString TestDoorLockedKey(const FString& StableId);

	/** Broadcast after a successful LoadFromSlot replaced the store. */
	FSimpleMulticastDelegate OnWorldStateReplaced;

private:
	struct FState;
	TPimplPtr<FState> State;
};
