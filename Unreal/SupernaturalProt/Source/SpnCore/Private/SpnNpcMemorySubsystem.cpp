#include "SpnNpcMemoryInternal.h"

#include "SpnCoreModule.h"

#include <cmath>
#include <string>

namespace
{
	std::string ToUtf8(const FString& Text)
	{
		const FTCHARToUTF8 Converted(*Text);
		return std::string(Converted.Get(), static_cast<std::size_t>(Converted.Length()));
	}

	FString FromUtf8(const std::string& Text)
	{
		return FString(FUTF8ToTCHAR(Text.c_str(), static_cast<int32>(Text.size())));
	}

	constexpr int64 MaxNpcId = (1ll << 62) - 1;

	spn::core::EntityId Npc(int64 Id)
	{
		return spn::core::EntityId(static_cast<uint64>(Id));
	}

	spn::core::EntityId Player()
	{
		return spn::core::EntityId(static_cast<uint64>(USpnNpcMemorySubsystem::PlayerEntityId));
	}
}

USpnNpcMemorySubsystem::USpnNpcMemorySubsystem() = default;
USpnNpcMemorySubsystem::~USpnNpcMemorySubsystem() = default;

void USpnNpcMemorySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	State = MakePimpl<FState>();
	WorldTime = 0.0;
}

void USpnNpcMemorySubsystem::Deinitialize()
{
	State.Reset();
	Super::Deinitialize();
}

void USpnNpcMemorySubsystem::Tick(float DeltaTime)
{
	if (State.IsValid() && DeltaTime > 0.f && FMath::IsFinite(DeltaTime))
	{
		WorldTime += DeltaTime;
	}
}

void USpnNpcMemorySubsystem::AdvanceWorldTime(double Seconds)
{
	if (State.IsValid() && Seconds > 0.0 && std::isfinite(Seconds))
	{
		WorldTime += Seconds;
	}
}

bool USpnNpcMemorySubsystem::RegisterNpc(int64 NpcId, ESpnNpcTier Tier, const FString& Label)
{
	if (!State.IsValid() || NpcId <= 0 || NpcId > MaxNpcId || NpcId == PlayerEntityId)
	{
		return false;
	}
	if (State->Store.Knows(Npc(NpcId)))
	{
		return true;
	}
	spn::core::NpcIdentity Identity;
	Identity.id = Npc(NpcId);
	Identity.tier = static_cast<spn::core::PersistenceTier>(static_cast<uint8>(Tier));
	Identity.label = ToUtf8(Label);
	if (const auto Error = State->Store.AddNpc(std::move(Identity)))
	{
		UE_LOG(LogSpnCore, Warning, TEXT("NpcMemory: registering NPC %lld failed (%s)"), NpcId, *FromUtf8(std::string(spn::core::ToString(Error->code))));
		return false;
	}
	return true;
}

bool USpnNpcMemorySubsystem::IsNpcKnown(int64 NpcId) const
{
	return State.IsValid() && NpcId > 0 && State->Store.Knows(Npc(NpcId));
}

bool USpnNpcMemorySubsystem::Remember(int64 NpcId, const FSpnIncident& Incident)
{
	if (!IsNpcKnown(NpcId))
	{
		return false;
	}
	spn::core::Incident I;
	I.kind = static_cast<spn::core::IncidentKind>(static_cast<uint8>(Incident.Kind));
	I.time = WorldTime;
	I.where = spn::core::Vec2{Incident.LocationCm.X / 100.0, Incident.LocationCm.Y / 100.0};
	I.severity = Incident.Severity;
	I.accuracy = Incident.Accuracy;
	if (!Incident.ObjectId.IsEmpty())
	{
		I.object = spn::core::ObjectEntityId(ToUtf8(Incident.ObjectId));
	}
	if (const auto Error = spn::core::RememberIncident(State->Store, Npc(NpcId), Player(), I))
	{
		UE_LOG(LogSpnCore, Warning, TEXT("NpcMemory: NPC %lld could not remember the incident (%s)"), NpcId, *FromUtf8(std::string(spn::core::ToString(Error->code))));
		return false;
	}
	return true;
}

FSpnWariness USpnNpcMemorySubsystem::Assess(int64 NpcId) const
{
	FSpnWariness W;
	if (!IsNpcKnown(NpcId))
	{
		return W;
	}
	const spn::core::NpcWariness Core = spn::core::AssessWariness(State->Store, Npc(NpcId), Player(), WorldTime);
	W.Suspicion = static_cast<float>(Core.suspicion);
	W.bWary = Core.wary;
	W.bRecentAlarm = Core.recentAlarm;
	W.bBrokeInRemembered = Core.brokeInRemembered;
	W.bPlayerSeenRemembered = Core.playerSeenRemembered;
	W.bHasLastIncident = Core.lastIncident.has_value();
	if (Core.lastIncident)
	{
		W.LastIncidentCm = FVector(Core.lastIncident->x * 100.0, Core.lastIncident->y * 100.0, 0.0);
	}
	W.LastIncidentTime = Core.lastIncidentTime;
	return W;
}

void USpnNpcMemorySubsystem::GetInitialReaction(int64 NpcId, ESpnReactionState& OutState, FVector& OutTargetCm) const
{
	OutState = ESpnReactionState::Idle;
	OutTargetCm = FVector::ZeroVector;
	if (!IsNpcKnown(NpcId))
	{
		return;
	}
	const spn::core::NpcWariness Wariness = spn::core::AssessWariness(State->Store, Npc(NpcId), Player(), WorldTime);
	const spn::core::RestoredReaction R = spn::core::InitialStateFromMemory(Wariness);
	OutState = static_cast<ESpnReactionState>(static_cast<uint8>(R.state));
	OutTargetCm = FVector(R.target.x * 100.0, R.target.y * 100.0, 0.0);
}

TArray<FSpnMemoryView> USpnNpcMemorySubsystem::GetMemories(int64 NpcId) const
{
	TArray<FSpnMemoryView> Out;
	if (!IsNpcKnown(NpcId))
	{
		return Out;
	}
	for (const spn::core::MemoryRecord& M : *State->Store.Memories(Npc(NpcId)))
	{
		FSpnMemoryView V;
		V.Kind = FromUtf8(std::string(spn::core::ToString(M.kind)));
		V.Time = M.time;
		V.LocationCm = FVector(M.location.x * 100.0, M.location.y * 100.0, 0.0);
		V.Intensity = static_cast<float>(M.intensity);
		V.bObjectSubject = spn::core::IsObjectEntity(M.subject);
		Out.Add(V);
	}
	return Out;
}

int32 USpnNpcMemorySubsystem::GetMemoryCount(int64 NpcId) const
{
	return IsNpcKnown(NpcId) ? static_cast<int32>(State->Store.Memories(Npc(NpcId))->size()) : 0;
}

int32 USpnNpcMemorySubsystem::GetNpcCount() const
{
	return State.IsValid() ? static_cast<int32>(State->Store.NpcCount()) : 0;
}

int32 USpnNpcMemorySubsystem::GetTotalMemories() const
{
	return State.IsValid() ? static_cast<int32>(State->Store.TotalMemories()) : 0;
}

uint64 USpnNpcMemorySubsystem::GetRevision() const
{
	return State.IsValid() ? State->Store.Revision() : 0;
}

void USpnNpcMemorySubsystem::ResetAll()
{
	if (!State.IsValid())
	{
		return;
	}
	State->Store = spn::core::NpcMemoryStore();
	WorldTime = 0.0;
	BroadcastReplaced();
}

FString USpnNpcMemorySubsystem::SerializeState() const
{
	return State.IsValid() ? FromUtf8(State->Store.Serialize()) : FString();
}

bool USpnNpcMemorySubsystem::RestoreState(const FString& Text)
{
	if (!State.IsValid())
	{
		return false;
	}
	auto Parsed = spn::core::NpcMemoryStore::Deserialize(ToUtf8(Text));
	if (!Parsed)
	{
		return false;
	}
	State->Store = std::move(Parsed.Value());
	AdoptLoadedClock();
	BroadcastReplaced();
	return true;
}

void USpnNpcMemorySubsystem::SyncClockForSave()
{
	if (State.IsValid())
	{
		State->Store.AdvanceTo(WorldTime);
	}
}

void USpnNpcMemorySubsystem::AdoptLoadedClock()
{
	if (State.IsValid())
	{
		WorldTime = State->Store.Clock();
	}
}

void USpnNpcMemorySubsystem::BroadcastReplaced()
{
	OnReplaced.Broadcast();
}
