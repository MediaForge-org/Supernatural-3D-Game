#pragma once

#include "CoreMinimal.h"
#include "Templates/PimplPtr.h"
#include "SpnCreature.generated.h"

// Unreal wrappers of the Core creature brain and the difficulty profiles (Core/include/spn/core/Creature.h, Difficulty.h; NightBuild07, first case
// "Die Stille Mühle", Docs/Proposals/0019). Core types stay private (Decision 0006); this interface uses Unreal types in CENTIMETRES.
// ENGINEERING PROPOSAL, not master canon: every number is a placeholder gameplay parameter.

UENUM()
enum class ESpnCreatureState : uint8
{
	Patrol,
	Investigate,
	Chase,
	Search,
	Stunned,
};
SPNCORE_API const TCHAR* LexToString(ESpnCreatureState State);

UENUM()
enum class ESpnCreaturePace : uint8
{
	Still,
	Prowl,
	Hunt,
};

/** Tunable creature values in cm / s (converted to the Core's metres). Defaults = the Core defaults. */
struct FSpnCreatureTuning
{
	float ChaseStrength = 0.75f;
	float ChaseHearDistanceCm = 2500.f;
	float CloseHearDistanceCm = 800.f;
	float ContactRangeCm = 150.f;
	float CatchRangeCm = 110.f;
	float ReactionDelaySeconds = 1.f;
	float ChaseLoseSeconds = 4.f;
	float SearchSeconds = 20.f;
	float InvestigateSeconds = 10.f;
	float StunSeconds = 8.f;
	float ArrivalRadiusCm = 100.f;
};

struct FSpnCreatureInput
{
	float DeltaSeconds = 0.f;
	FVector SelfCm = FVector::ZeroVector;
	bool bHasNoise = false;
	FVector NoiseCm = FVector::ZeroVector;
	float NoiseStrength = 0.f;
	float NoiseDistanceCm = 0.f;
	bool bHasPrey = false;
	FVector PreyCm = FVector::ZeroVector;
	float PreyDistanceCm = 0.f;
};

struct FSpnCreatureOutput
{
	ESpnCreatureState State = ESpnCreatureState::Patrol;
	ESpnCreatureState Previous = ESpnCreatureState::Patrol;
	bool bChanged = false;
	bool bListening = false;
	bool bCaught = false;
	bool bHasGoal = false;
	FVector GoalCm = FVector::ZeroVector;
	float SearchRadiusCm = 0.f;
	ESpnCreaturePace Pace = ESpnCreaturePace::Prowl;
};

/** The Core CreatureBrain behind a pimpl. Not copyable. */
class SPNCORE_API FSpnCreatureBrain
{
public:
	/** Invalid tuning (Core Validate) falls back to the defaults; OutError (optional) receives the reason. */
	explicit FSpnCreatureBrain(const FSpnCreatureTuning& Tuning = FSpnCreatureTuning(), FString* OutError = nullptr);
	~FSpnCreatureBrain();
	FSpnCreatureBrain(const FSpnCreatureBrain&) = delete;
	FSpnCreatureBrain& operator=(const FSpnCreatureBrain&) = delete;

	FSpnCreatureOutput Update(const FSpnCreatureInput& Input);
	bool Stun();
	void Reset();
	ESpnCreatureState GetState() const;
	float GetTimeInState() const;
	bool IsWary() const;
	float GetHearingScale() const;
	uint64 GetTransitionCount() const;

private:
	struct FState;
	TPimplPtr<FState> State;
};

namespace SpnCreatureRules
{
	/** The bell reaches a creature at this distance; every wall between halves the range. */
	SPNCORE_API bool BellReaches(float DistanceCm, float BellRangeCm, int32 Walls);
	/** False while any creature hunts the player (TOD-03). */
	SPNCORE_API bool SaveAllowed(const TArray<ESpnCreatureState>& Creatures);
}

/**
 * One difficulty profile (DECISIONS.md DIF-01: any number of levels, names open). Authored as a list in config
 * ([/Script/SupernaturalProt.SpnCaseSettings] in DefaultGame.ini). PLACEHOLDER values.
 */
USTRUCT()
struct SPNCORE_API FSpnDifficultyProfile
{
	GENERATED_BODY()

	/** Stable key ([a-z0-9_], 1..32). The shown name is a text key / later localisation, not this id. */
	UPROPERTY(EditAnywhere, Category = "Spn|Difficulty")
	FString Id;

	/** Shown name (placeholder text until the developer names the levels). */
	UPROPERTY(EditAnywhere, Category = "Spn|Difficulty")
	FString DisplayName;

	/** Health fraction after a death and the reset to the checkpoint, (0, 1] (TOD-04). */
	UPROPERTY(EditAnywhere, Category = "Spn|Difficulty")
	float RespawnHealth = 1.f;

	/** Seconds a creature listens before it starts a pursuit (the head start after a shot). */
	UPROPERTY(EditAnywhere, Category = "Spn|Difficulty")
	float CreatureReactionDelay = 1.f;

	/** Seconds a creature searches after losing the player. */
	UPROPERTY(EditAnywhere, Category = "Spn|Difficulty")
	float CreatureSearchSeconds = 20.f;

	/** Hearing range factor of creatures, [0.25, 4]. */
	UPROPERTY(EditAnywhere, Category = "Spn|Difficulty")
	float CreatureHearingScale = 1.f;
};

namespace SpnDifficulty
{
	/** Core ValidateDifficultyList: not empty, all valid, ids unique. Returns false with the reason. */
	SPNCORE_API bool ValidateList(const TArray<FSpnDifficultyProfile>& Profiles, FString& OutError);
	/** INDEX_NONE if no profile has this id. */
	SPNCORE_API int32 Find(const TArray<FSpnDifficultyProfile>& Profiles, const FString& Id);
	/** Health after a respawn: the profile's fraction of MaxHealth, never 0. */
	SPNCORE_API float RespawnHealth(const FSpnDifficultyProfile& Profile, float MaxHealth);
}
