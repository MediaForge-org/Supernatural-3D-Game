#pragma once

#include "CoreMinimal.h"
#include "SpnSaveSubsystem.h"
#include "Containers/Ticker.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "SpnSessionSubsystem.generated.h"

class AActor;
class UWorld;

// Game flow of Night Build 01: main menu -> new game / continue / load -> gameplay -> pause -> back to the menu. ENGINEERING PROPOSAL.
//
// OPT-IN: the flow only runs when it is enabled (the main-menu level loads, StartNewGame / LoadGame is called, the command line has -SpnGameFlow,
// or the console command Spn.Game.Enable ran). Every existing level and automation test therefore behaves exactly as before.
//
// What a (re)loaded gameplay world gets: the temporary test item catalog (if none is installed), the test map definition (if none), the
// player's own container, and - for the integration level - the pickups and the container of the integration scenario (placeholder
// objects). A new game resets the world facts, the inventory, the map and the waypoint. The save format is untouched.
//
// PLAYER POSE: saved in the optional section player.pose (SpnPlayerPose.h: canonical metres through FSpnWorldFrame, bearing, pitch, gait, crouch,
// level). A load hands the pose to this subsystem AFTER the state swap; it is applied once the player pawn of the right level exists (the level
// may still be loading). A pose that cannot be used (other level, no floor below it, blocked) is reported and the player stays at the level's
// start point - never a silent default. Legacy slots without a pose load fully and are reported as such. The actors themselves are not saved;
// the scenario spawner recreates them from the world facts (pickups taken, container contents).

UENUM()
enum class ESpnSessionPending : uint8
{
	None,
	NewGame,
	Load,
};

UCLASS()
class SUPERNATURALPROT_API USpnSessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	static const TCHAR* MainMenuMapName() { return TEXT("/Game/SpnTest/Lvl_SpnMainMenu"); }
	/** The level of the integration scenario (override with -SpnGameplayMap=/Game/...). */
	static FString GameplayMapName();

	void EnableGameFlow() { bGameFlow = true; }
	bool IsGameFlowEnabled() const { return bGameFlow; }
	/** True while the loaded world is the main-menu level. */
	bool IsMenuWorld() const { return bMenuWorld; }
	const FString& GetActiveSlot() const { return ActiveSlot; }
	void SetActiveSlot(const FString& Slot) { ActiveSlot = Slot; }

	/** Like StartNewGame, but in the world that is already loaded (tests, console): resets the state and installs the scenario objects here. */
	void StartNewGameInPlace(UWorld* World);
	/** Starts a new game: travels to the gameplay level and resets the state there. */
	void StartNewGame();
	/**
	 * Loads a slot. From the main menu it validates the slot first and then travels to the gameplay level; in a gameplay level it loads in place.
	 * Returns the status of the validation / load; a refused slot (corrupt, newer build, catalog mismatch without acceptance) changes nothing.
	 */
	FSpnSaveStatus LoadGame(const FString& Slot, bool bAcceptCatalogMismatch = false);
	/** Saves to a slot and remembers it as the active one. */
	FSpnSaveStatus SaveGame(const FString& Slot);
	void ReturnToMainMenu();
	void QuitGame();
	/** Newest slot that can be loaded right now (empty if none). */
	FString FindContinueSlot() const;

	/** Brings a gameplay world into the state the flow expects (catalog, map, player container, scenario objects, pending new game / load). */
	void SetupGameplayWorld(UWorld* World);

	DECLARE_MULTICAST_DELEGATE_OneParam(FSessionWorldReady, bool /*bMenu*/);
	FSessionWorldReady OnWorldReady;
	/** Result text of the last pending load that ran after a level change (for the HUD). */
	DECLARE_MULTICAST_DELEGATE_OneParam(FSessionLoadResult, const FSpnSaveStatus&);
	FSessionLoadResult OnPendingLoadFinished;

private:
	void HandlePostLoadMap(UWorld* World);
	void HandlePoseLoaded(const FSpnPlayerPose& Pose);
	bool CaptureLivePose(FSpnPlayerPose& Out) const;
	/** Places the player. False if the pose cannot be used here (reported to the HUD); the player then stays where the level put them. */
	bool ApplyPose(UWorld* World, const FSpnPlayerPose& Pose);
	void TryApplyPendingPose();
	bool EnsureCatalog();
	void EnsureMap();
	void ResetForNewGame();
	void SpawnScenario(UWorld* World);

	FDelegateHandle PostLoadHandle;
	bool bGameFlow = false;
	bool bMenuWorld = false;
	ESpnSessionPending Pending = ESpnSessionPending::None;
	FString PendingSlot;
	bool bPendingAccept = false;
	FString ActiveSlot;
	TOptional<FSpnPlayerPose> PendingPose;
	int32 PoseAttempts = 0;
	FTSTicker::FDelegateHandle PoseTicker;
public:
	/** Test/diagnostics: the pose waiting for its pawn (unset if none). */
	const TOptional<FSpnPlayerPose>& GetPendingPose() const { return PendingPose; }
	/** Applies a pose to the live player right now (tests, console). */
	bool ApplyPoseNow(const FSpnPlayerPose& Pose);
};
