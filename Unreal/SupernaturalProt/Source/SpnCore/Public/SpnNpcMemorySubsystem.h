#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Tickable.h"
#include "SpnNpcReaction.h"

#include "SpnNpcMemorySubsystem.generated.h"

// Unreal owner of the persistent NPC memory (NightBuild05): ONE spn::core::NpcMemoryStore per game instance, written by the NPCs, saved by
// USpnSaveSubsystem as section "world.npcs" (SPN-NPCMEM 1, format unchanged), read back after a load.
//
// PERSISTENT: identities (stable ids), the bounded memories (what was heard / seen / where / how severe / when), the store clock.
// VOLATILE (never saved): the reaction state of an NPC (Investigate, Alert, ...), its timers and goals. After a load the NPC re-derives its starting
// state from the memories (Core InitialStateFromMemory).
//
// Ids: NPC ids are chosen by the level data (stable, never reused); the player is entity PlayerEntityId. NPC ids must stay below 2^62.
// ENGINEERING PROPOSAL, not master canon.

UENUM()
enum class ESpnNpcTier : uint8
{
	/** Story / close contacts: maximal memory. */
	A,
	/** Merchants, neighbours, recurring contacts. */
	B,
	/** Ordinary residents (light memory). */
	C,
};

UENUM()
enum class ESpnIncidentKind : uint8
{
	/** Heard something suspicious; the NPC does not know who caused it. */
	HeardSuspicious,
	/** Heard a door being broken; Incident.ObjectId names the door. */
	HeardDoorBroken,
	/** Saw the player. */
	SawPlayer,
	/** Raised the alarm / fled because of the player. */
	Alarmed,
};

struct FSpnIncident
{
	ESpnIncidentKind Kind = ESpnIncidentKind::HeardSuspicious;
	FVector LocationCm = FVector::ZeroVector;
	/** 0..1 */
	float Severity = 0.5f;
	/** HeardDoorBroken: the door's StableId. */
	FString ObjectId;
	/** 0..1: how well it is remembered (heard through walls / from afar is lower). */
	float Accuracy = 1.f;
};

struct FSpnWariness
{
	/** 0..1, fades with the memory. */
	float Suspicion = 0.f;
	bool bWary = false;
	/** An alarm of the player happened within the resume window (game seconds). */
	bool bRecentAlarm = false;
	bool bBrokeInRemembered = false;
	bool bPlayerSeenRemembered = false;
	bool bHasLastIncident = false;
	FVector LastIncidentCm = FVector::ZeroVector;
	double LastIncidentTime = 0.0;
};

/** Memory of ONE NPC as shown to tests and debug tools (newest last). */
struct FSpnMemoryView
{
	FString Kind;
	double Time = 0.0;
	FVector LocationCm = FVector::ZeroVector;
	float Intensity = 0.f;
	bool bObjectSubject = false;
};

UCLASS()
class SPNCORE_API USpnNpcMemorySubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:
	/** The player's entity id in the memory store (the player needs no NPC record). */
	static constexpr int64 PlayerEntityId = 1;

	USpnNpcMemorySubsystem();
	virtual ~USpnNpcMemorySubsystem() override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// FTickableGameObject: the game clock of the memory advances with real simulation time.
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(USpnNpcMemorySubsystem, STATGROUP_Tickables); }
	virtual bool IsTickable() const override { return !IsTemplate() && State.IsValid(); }
	virtual ETickableTickType GetTickableTickType() const override { return ETickableTickType::Conditional; }

	/** Registers an NPC (idempotent: an already known id is fine and returns true). False for an invalid id (0, >= 2^62, the player's id) or at the store limit. */
	bool RegisterNpc(int64 NpcId, ESpnNpcTier Tier, const FString& Label);
	bool IsNpcKnown(int64 NpcId) const;

	/** Records an incident in the memory of the NPC at the current game time. False if the NPC is unknown or the store refused it. */
	bool Remember(int64 NpcId, const FSpnIncident& Incident);

	/** What the NPC makes of its memories (neutral for unknown NPCs). */
	FSpnWariness Assess(int64 NpcId) const;

	/** Where the (volatile) reaction of the NPC starts after a load or a level start: Investigate towards the last incident while an alarm is still recent, else Idle. */
	void GetInitialReaction(int64 NpcId, ESpnReactionState& OutState, FVector& OutTargetCm) const;

	TArray<FSpnMemoryView> GetMemories(int64 NpcId) const;
	int32 GetMemoryCount(int64 NpcId) const;
	int32 GetNpcCount() const;
	int32 GetTotalMemories() const;
	uint64 GetRevision() const;

	/** Game seconds of the memory clock (restored from the save). */
	double GetWorldTime() const { return WorldTime; }
	/** Test hook: advances the clock by hand. */
	void AdvanceWorldTime(double Seconds);

	/** Empties the store (new game / test). Broadcasts OnReplaced. */
	void ResetAll();

	/** Whole store as text / replaced from text (strict; false and unchanged when the text is rejected). Used by tests and the save subsystem path. */
	FString SerializeState() const;
	bool RestoreState(const FString& Text);

	/** After the store was replaced (load, reset): NPC actors re-register and re-derive their volatile state. */
	FSimpleMulticastDelegate OnReplaced;

private:
	friend class USpnSaveSubsystem;
	struct FState;
	TPimplPtr<FState> State;
	double WorldTime = 0.0;
	/** Brings the store clock to the game time (called before a save). */
	void SyncClockForSave();
	void BroadcastReplaced();
	void AdoptLoadedClock();
};
