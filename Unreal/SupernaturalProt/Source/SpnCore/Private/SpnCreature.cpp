#include "SpnCreature.h"

#include <spn/core/Creature.h>
#include <spn/core/Difficulty.h>

#include <string>
#include <vector>

namespace
{
	spn::core::Vec2 CreatureToCore(const FVector& Cm)
	{
		return spn::core::Vec2{Cm.X / 100.0, Cm.Y / 100.0};
	}

	std::string CreatureUtf8(const FString& Text)
	{
		const FTCHARToUTF8 Converted(*Text);
		return std::string(Converted.Get(), static_cast<std::size_t>(Converted.Length()));
	}

	spn::core::DifficultyProfile ToCore(const FSpnDifficultyProfile& P)
	{
		spn::core::DifficultyProfile Out;
		Out.id = CreatureUtf8(P.Id);
		Out.respawnHealth = P.RespawnHealth;
		Out.creatureReactionDelay = P.CreatureReactionDelay;
		Out.creatureSearchSeconds = P.CreatureSearchSeconds;
		Out.creatureHearingScale = P.CreatureHearingScale;
		return Out;
	}
}

const TCHAR* LexToString(ESpnCreatureState State)
{
	switch (State)
	{
	case ESpnCreatureState::Patrol: return TEXT("Patrol");
	case ESpnCreatureState::Investigate: return TEXT("Investigate");
	case ESpnCreatureState::Chase: return TEXT("Chase");
	case ESpnCreatureState::Search: return TEXT("Search");
	case ESpnCreatureState::Stunned: return TEXT("Stunned");
	}
	return TEXT("Unknown");
}

struct FSpnCreatureBrain::FState
{
	spn::core::CreatureBrain Brain;
};

FSpnCreatureBrain::FSpnCreatureBrain(const FSpnCreatureTuning& T, FString* OutError) : State(MakePimpl<FState>())
{
	spn::core::CreatureConfig C;
	C.chaseStrength = T.ChaseStrength;
	C.chaseHearDistance = T.ChaseHearDistanceCm / 100.0;
	C.closeHearDistance = T.CloseHearDistanceCm / 100.0;
	C.contactRange = T.ContactRangeCm / 100.0;
	C.catchRange = T.CatchRangeCm / 100.0;
	C.reactionDelay = T.ReactionDelaySeconds;
	C.chaseLoseSeconds = T.ChaseLoseSeconds;
	C.searchSeconds = T.SearchSeconds;
	C.investigateSeconds = T.InvestigateSeconds;
	C.stunSeconds = T.StunSeconds;
	C.arrivalRadius = T.ArrivalRadiusCm / 100.0;
	if (const std::optional<std::string> Error = spn::core::Validate(C))
	{
		if (OutError != nullptr)
		{
			*OutError = UTF8_TO_TCHAR(Error->c_str());
		}
		C = spn::core::CreatureConfig{};
	}
	State->Brain = spn::core::CreatureBrain(C);
}

FSpnCreatureBrain::~FSpnCreatureBrain() = default;

FSpnCreatureOutput FSpnCreatureBrain::Update(const FSpnCreatureInput& Input)
{
	spn::core::CreatureInput In;
	In.dt = Input.DeltaSeconds;
	In.self = CreatureToCore(Input.SelfCm);
	if (Input.bHasNoise)
	{
		In.noise = spn::core::CreatureNoise{CreatureToCore(Input.NoiseCm), Input.NoiseStrength, Input.NoiseDistanceCm / 100.0};
	}
	if (Input.bHasPrey)
	{
		In.preyDistance = Input.PreyDistanceCm / 100.0;
		In.preyPosition = CreatureToCore(Input.PreyCm);
	}
	const spn::core::CreatureOutput Out = State->Brain.Update(In);
	FSpnCreatureOutput R;
	R.State = static_cast<ESpnCreatureState>(static_cast<uint8>(Out.state));
	R.Previous = static_cast<ESpnCreatureState>(static_cast<uint8>(Out.previous));
	R.bChanged = Out.changed;
	R.bListening = Out.listening;
	R.bCaught = Out.caught;
	R.bHasGoal = Out.hasGoal;
	R.GoalCm = FVector(Out.goal.x * 100.0, Out.goal.y * 100.0, Input.SelfCm.Z);
	R.SearchRadiusCm = static_cast<float>(Out.searchRadius * 100.0);
	R.Pace = static_cast<ESpnCreaturePace>(static_cast<uint8>(Out.pace));
	return R;
}

bool FSpnCreatureBrain::Stun() { return State->Brain.Stun(); }
void FSpnCreatureBrain::Reset() { State->Brain.Reset(); }
ESpnCreatureState FSpnCreatureBrain::GetState() const { return static_cast<ESpnCreatureState>(static_cast<uint8>(State->Brain.State())); }
float FSpnCreatureBrain::GetTimeInState() const { return static_cast<float>(State->Brain.TimeInState()); }
bool FSpnCreatureBrain::IsWary() const { return State->Brain.Wary(); }
float FSpnCreatureBrain::GetHearingScale() const { return static_cast<float>(State->Brain.HearingScale()); }
uint64 FSpnCreatureBrain::GetTransitionCount() const { return State->Brain.TransitionCount(); }

bool SpnCreatureRules::BellReaches(float DistanceCm, float BellRangeCm, int32 Walls)
{
	return spn::core::BellReaches(DistanceCm / 100.0, BellRangeCm / 100.0, Walls);
}

bool SpnCreatureRules::SaveAllowed(const TArray<ESpnCreatureState>& Creatures)
{
	std::vector<spn::core::CreatureState> States;
	States.reserve(Creatures.Num());
	for (ESpnCreatureState S : Creatures)
	{
		States.push_back(static_cast<spn::core::CreatureState>(static_cast<uint8>(S)));
	}
	return spn::core::SaveAllowed(States);
}

bool SpnDifficulty::ValidateList(const TArray<FSpnDifficultyProfile>& Profiles, FString& OutError)
{
	std::vector<spn::core::DifficultyProfile> List;
	List.reserve(Profiles.Num());
	for (const FSpnDifficultyProfile& P : Profiles)
	{
		List.push_back(ToCore(P));
	}
	if (const std::optional<std::string> Error = spn::core::ValidateDifficultyList(List))
	{
		OutError = UTF8_TO_TCHAR(Error->c_str());
		return false;
	}
	OutError.Reset();
	return true;
}

int32 SpnDifficulty::Find(const TArray<FSpnDifficultyProfile>& Profiles, const FString& Id)
{
	return Profiles.IndexOfByPredicate([&Id](const FSpnDifficultyProfile& P) { return P.Id == Id; });
}

float SpnDifficulty::RespawnHealth(const FSpnDifficultyProfile& Profile, float MaxHealth)
{
	return static_cast<float>(spn::core::RespawnHealth(ToCore(Profile), MaxHealth));
}
