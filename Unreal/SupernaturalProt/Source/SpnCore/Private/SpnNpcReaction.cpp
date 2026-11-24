#include "SpnNpcReaction.h"

#include <spn/core/GameEvents.h>
#include <spn/core/NpcReaction.h>

#include <cmath>

namespace
{
	spn::core::Vec2 ToCore(const FVector& Cm)
	{
		return spn::core::Vec2{Cm.X / 100.0, Cm.Y / 100.0};
	}

	FVector ToCm(const spn::core::Vec2& M, double Z = 0.0)
	{
		return FVector(M.x * 100.0, M.y * 100.0, Z);
	}

	spn::core::NoiseKind ToCore(ESpnNoiseKind Kind)
	{
		return static_cast<spn::core::NoiseKind>(static_cast<uint8>(Kind));
	}

	spn::core::GameplayNoise ToCore(const FSpnNoise& Noise)
	{
		spn::core::GameplayNoise N;
		N.position = ToCore(Noise.LocationCm);
		N.range = Noise.RangeCm / 100.0;
		N.strength = Noise.Strength;
		N.kind = ToCore(Noise.Kind);
		return N;
	}

	std::string ToUtf8(const FString& Text)
	{
		const FTCHARToUTF8 Converted(*Text);
		return std::string(Converted.Get(), static_cast<std::size_t>(Converted.Length()));
	}

	FString FromUtf8(const std::string& Text)
	{
		return FString(FUTF8ToTCHAR(Text.c_str(), static_cast<int32>(Text.size())));
	}
}

const TCHAR* LexToString(ESpnReactionState State)
{
	switch (State)
	{
	case ESpnReactionState::Idle: return TEXT("Idle");
	case ESpnReactionState::Investigate: return TEXT("Investigate");
	case ESpnReactionState::Alert: return TEXT("Alert");
	case ESpnReactionState::Flee: return TEXT("Flee");
	case ESpnReactionState::ReturnToRoutine: return TEXT("ReturnToRoutine");
	}
	return TEXT("Unknown");
}

const TCHAR* LexToString(ESpnNoiseKind Kind)
{
	switch (Kind)
	{
	case ESpnNoiseKind::Footstep: return TEXT("Footstep");
	case ESpnNoiseKind::DoorUse: return TEXT("DoorUse");
	case ESpnNoiseKind::DoorBreak: return TEXT("DoorBreak");
	case ESpnNoiseKind::Impact: return TEXT("Impact");
	case ESpnNoiseKind::Shout: return TEXT("Shout");
	}
	return TEXT("Unknown");
}

// ------------------------------------------------------------------ senses

bool SpnNpcSense::IsSuspicious(const FSpnNoise& Noise)
{
	return spn::core::IsSuspicious(ToCore(Noise.Kind), Noise.Strength);
}

bool SpnNpcSense::CanHear(const FSpnNoise& Noise, const FVector& ListenerCm, int32 Walls, float Sensitivity, float WallAttenuation, float& OutDistanceCm, float& OutEffectiveRangeCm)
{
	spn::core::HearingConfig Config;
	Config.sensitivity = Sensitivity;
	Config.wallAttenuation = WallAttenuation;
	const spn::core::HearingResult R = spn::core::CanHear(Config, ToCore(Noise), ToCore(ListenerCm), Walls);
	OutDistanceCm = static_cast<float>(R.distance * 100.0);
	OutEffectiveRangeCm = static_cast<float>(R.effectiveRange * 100.0);
	return R.heard;
}

bool SpnNpcSense::CanSee(const FVector& EyeCm, float FacingYawDegrees, const FVector& TargetCm, bool bLineClear, float SightRangeCm, float FieldOfViewDegrees, float CloseRangeCm)
{
	spn::core::NpcPerceptionConfig Config;
	Config.sightRange = SightRangeCm / 100.0;
	Config.fieldOfViewDegrees = FieldOfViewDegrees;
	Config.closeSenseRange = CloseRangeCm / 100.0;
	return spn::core::CanSee(Config, ToCore(EyeCm), FacingYawDegrees, ToCore(TargetCm), bLineClear);
}

// ------------------------------------------------------------------ brain

struct FSpnReactionBrain::FState
{
	spn::core::NpcReactionBrain Brain;
};

FSpnReactionBrain::FSpnReactionBrain() : State(MakePimpl<FState>()) {}
FSpnReactionBrain::~FSpnReactionBrain() = default;

FSpnReactionOutput FSpnReactionBrain::Update(const FSpnReactionInput& Input)
{
	spn::core::ReactionInput In;
	In.dt = Input.DeltaSeconds;
	In.self = ToCore(Input.SelfCm);
	In.home = ToCore(Input.HomeCm);
	In.seesPlayer = Input.bSeesPlayer;
	In.playerPosition = ToCore(Input.PlayerCm);
	if (Input.bHasNoise)
	{
		In.noise = spn::core::HeardNoise{ToCore(Input.NoiseCm), Input.NoiseSeverity, Input.bNoiseSuspicious};
	}
	In.wary = Input.bWary;
	const spn::core::ReactionOutput Out = State->Brain.Update(In);

	FSpnReactionOutput R;
	R.State = static_cast<ESpnReactionState>(static_cast<uint8>(Out.state));
	R.Previous = static_cast<ESpnReactionState>(static_cast<uint8>(Out.previous));
	R.bChanged = Out.changed;
	R.bRaiseAlarm = Out.raiseAlarm;
	R.bHasGoal = Out.hasGoal;
	R.GoalCm = ToCm(Out.goal);
	R.bSprint = Out.gait == spn::core::Gait::Sprint;
	R.bFacePlayer = Out.facePlayer;
	R.bFleeAway = Out.fleeAway;
	R.ThreatCm = ToCm(Out.threat);
	return R;
}

ESpnReactionState FSpnReactionBrain::GetState() const
{
	return static_cast<ESpnReactionState>(static_cast<uint8>(State->Brain.State()));
}

float FSpnReactionBrain::GetTimeInState() const
{
	return static_cast<float>(State->Brain.TimeInState());
}

uint64 FSpnReactionBrain::GetTransitionCount() const
{
	return State->Brain.TransitionCount();
}

void FSpnReactionBrain::Restore(ESpnReactionState NewState, const FVector& TargetCm)
{
	State->Brain.Restore(static_cast<spn::core::NpcReactionState>(static_cast<uint8>(NewState)), ToCore(TargetCm));
}

// ------------------------------------------------------------------ noise channel

struct USpnNoiseSubsystem::FState
{
	spn::core::GameEventBus Bus;
	spn::core::GameEventBus::Handle Handle = spn::core::GameEventBus::kInvalidHandle;
};

USpnNoiseSubsystem::USpnNoiseSubsystem() = default;
USpnNoiseSubsystem::~USpnNoiseSubsystem() = default;

void USpnNoiseSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	State = MakePimpl<FState>();
	State->Handle = State->Bus.Subscribe(
		[this](const spn::core::GameEvent& Event)
		{
			FSpnNoise Noise;
			Noise.LocationCm = FVector(Event.noiseX * 100.0, Event.noiseY * 100.0, static_cast<double>(Event.to));
			Noise.RangeCm = static_cast<float>(Event.noiseRange * 100.0);
			Noise.Strength = static_cast<float>(Event.noiseStrength);
			Noise.Kind = static_cast<ESpnNoiseKind>(static_cast<uint8>(Event.from));
			Noise.ObjectId = FromUtf8(Event.objectId);
			Noise.SourceId = FromUtf8(Event.actor);
			Noise.Sequence = Event.sequence;
			LastNoise = Noise;
			OnNoise.Broadcast(Noise);
		},
		{spn::core::GameEventType::NoiseEmitted});
}

void USpnNoiseSubsystem::Deinitialize()
{
	if (State.IsValid())
	{
		State->Bus.Unsubscribe(State->Handle);
	}
	State.Reset();
	Super::Deinitialize();
}

uint64 USpnNoiseSubsystem::EmitNoise(FSpnNoise Noise)
{
	if (!State.IsValid() || Noise.LocationCm.ContainsNaN() || !FMath::IsFinite(Noise.RangeCm) || !FMath::IsFinite(Noise.Strength) || Noise.RangeCm <= 0.f
		|| Noise.Strength <= 0.f)
	{
		++DroppedCount;
		return 0;
	}
	Noise.Strength = FMath::Clamp(Noise.Strength, 0.f, 1.f);

	spn::core::GameEvent Event;
	Event.type = spn::core::GameEventType::NoiseEmitted;
	Event.objectId = ToUtf8(Noise.ObjectId);
	Event.actor = ToUtf8(Noise.SourceId);
	Event.noiseX = Noise.LocationCm.X / 100.0;
	Event.noiseY = Noise.LocationCm.Y / 100.0;
	Event.noiseRange = Noise.RangeCm / 100.0;
	Event.noiseStrength = Noise.Strength;
	Event.from = static_cast<int64>(Noise.Kind);
	Event.to = static_cast<int64>(Noise.LocationCm.Z);
	const uint64 Before = State->Bus.PublishedCount();
	if (!State->Bus.Publish(Event))
	{
		++DroppedCount;
		return 0;
	}
	++EmittedCount;
	return Before + 1;
}
