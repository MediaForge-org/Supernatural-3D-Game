#pragma once

#include "CoreMinimal.h"
#include "SpnSaveSubsystem.h"
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
// NOT SAVED (documented limitation): the player's position and the actors themselves. A loaded game restores facts, inventory, the map with the
// waypoint, and therefore which pickups are taken, what every container holds and where the waypoint is; the player stays where the level
// placed them.

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
};
