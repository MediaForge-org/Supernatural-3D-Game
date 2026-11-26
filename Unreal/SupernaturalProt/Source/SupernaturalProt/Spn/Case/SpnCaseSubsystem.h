#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Subsystems/WorldSubsystem.h"
#include "SpnCreature.h"
#include "SpnCaseSubsystem.generated.h"

class ASpnCreature;
class ASpnThrownObject;
class APawn;

// The first case's runtime frame (NightBuild07, Docs/Proposals/0019, Night build A): player health, death and the reset to the checkpoint,
// the difficulty profile, autosaves, the save block during a pursuit, the bell and the throwables. PLACEHOLDER systems for a greybox case:
//   * Death (DECISIONS.md TOD-01): the player goes back to the newest save (autosave or manual, via USpnSessionSubsystem) with the difficulty's
//     respawn health (TOD-04); without a save (or without the game flow, e.g. in tests) back to where the player started the level.
//   * Saving is refused while a creature hunts the player (TOD-03; Core SaveAllowed).
//   * Health is NOT saved yet; there is no injury model (MED-01/MED-02 are for later) - creatures kill on contact.
// All numbers are placeholder gameplay parameters, not master canon.

/** Case values in config ([/Script/SupernaturalProt.SpnCaseSettings] in DefaultGame.ini). */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "SPN Case (placeholder)"))
class SUPERNATURALPROT_API USpnCaseSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	/** Ordered list, any number of levels (DIF-01). Empty or invalid: two built-in test profiles are used and an error is logged. */
	UPROPERTY(Config, EditAnywhere, Category = "Difficulty")
	TArray<FSpnDifficultyProfile> Difficulties;

	/** Id of the profile used when nothing else is chosen (command line -SpnDifficulty=<id> overrides). */
	UPROPERTY(Config, EditAnywhere, Category = "Difficulty")
	FString DefaultDifficulty = TEXT("test_standard");

	UPROPERTY(Config, EditAnywhere, Category = "Player", meta = (ClampMin = "1"))
	float PlayerMaxHealth = 100.f;

	/** The bell's stun range (walls halve it). */
	UPROPERTY(Config, EditAnywhere, Category = "Bell", meta = (ClampMin = "0"))
	float BellRangeCm = 1500.f;

	UPROPERTY(Config, EditAnywhere, Category = "Throwables", meta = (ClampMin = "0"))
	int32 MaxThrowables = 3;

	UPROPERTY(Config, EditAnywhere, Category = "Throwables", meta = (ClampMin = "100"))
	float ThrowSpeedCm = 1400.f;

	/** Impact noise of a thrown object: range and strength (suspicious from 0.75). */
	UPROPERTY(Config, EditAnywhere, Category = "Throwables", meta = (ClampMin = "0"))
	float ThrowNoiseRangeCm = 1400.f;

	UPROPERTY(Config, EditAnywhere, Category = "Throwables", meta = (ClampMin = "0", ClampMax = "1"))
	float ThrowNoiseStrength = 0.8f;

	/** The profiles actually in use (the config list, or the built-in test profiles if it is empty or invalid). */
	const TArray<FSpnDifficultyProfile>& GetProfiles() const;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FSpnPlayerDied, const FString& /*Cause*/);
DECLARE_MULTICAST_DELEGATE(FSpnPlayerRespawned);

UCLASS()
class SUPERNATURALPROT_API USpnCaseSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// --- difficulty
	const FSpnDifficultyProfile& GetDifficulty() const;
	/** False if no profile has this id (the current one stays). */
	bool SetDifficulty(const FString& Id);

	// --- creatures
	void RegisterCreature(ASpnCreature* Creature);
	void UnregisterCreature(ASpnCreature* Creature);
	TArray<ASpnCreature*> GetCreatures() const;
	/** True while any creature hunts the player (saving is refused then). */
	bool IsSaveBlocked() const;

	// --- player health, death, respawn
	float GetHealth() const { return Health; }
	float GetMaxHealth() const;
	bool IsPlayerDead() const { return bPlayerDead; }
	int32 GetDeathCount() const { return DeathCount; }
	const FString& GetLastDeathCause() const { return LastDeathCause; }
	/** Damage to the player; at 0 the player dies (once). Ignored while dead. */
	void DamagePlayer(float Amount, const FString& Cause);
	/**
	 * Back to the checkpoint: the newest save if the game flow runs and one exists, otherwise the level start. Creatures go back to their patrol,
	 * health = the difficulty's respawn health. Returns false if the player is not dead.
	 */
	bool Respawn();

	// --- autosave
	/** Writes the autosave slot once per point id and session; refused (false) while dead, during a pursuit or without the game flow. */
	bool RequestAutosave(FName PointId);
	static FString AutosaveSlot() { return TEXT("autosave"); }
	int32 GetAutosaveCount() const { return AutosaveCount; }

	// --- bell
	/** Loud noise at the bell and a stun for every creature it reaches. Returns the number of creatures stunned. */
	int32 RingBell(const FVector& LocationCm, AActor* Ringer);

	// --- throwables
	int32 GetThrowables() const { return Throwables; }
	/** False if the hands are full. */
	bool AddThrowable();
	/** Throws one object from the character's view; false if none left or dead. */
	ASpnThrownObject* Throw(APawn* Thrower);

	/** The pawn the case treats as the player. Default: player 0's pawn. Tests (no controller) set it. */
	void SetPlayerOverride(APawn* Pawn) { PlayerOverride = Pawn; }
	APawn* GetPlayerPawn() const;
	/** Remembers where the player is as the level start (the respawn point without a save). Only the first call with a player counts. */
	void CaptureStartIfNeeded();
	bool HasLevelStart() const { return bHasStart; }
	FVector GetLevelStartCm() const { return PlayerStart.GetLocation(); }

	FSpnPlayerDied OnPlayerDied;
	FSpnPlayerRespawned OnPlayerRespawned;

private:
	void SetPlayerFrozen(bool bFrozen);

	TArray<TWeakObjectPtr<ASpnCreature>> Creatures;
	TWeakObjectPtr<APawn> PlayerOverride;
	int32 DifficultyIndex = 0;
	float Health = 100.f;
	bool bPlayerDead = false;
	int32 DeathCount = 0;
	FString LastDeathCause;
	int32 Throwables = 0;
	int32 AutosaveCount = 0;
	TSet<FName> AutosavedPoints;
	bool bHasStart = false;
	FTransform PlayerStart;
};
