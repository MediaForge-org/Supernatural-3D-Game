#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "InputCoreTypes.h"
#include "Subsystems/LocalPlayerSubsystem.h"

#include "SpnInventoryUiSubsystem.generated.h"

class FSpnInventoryInputProcessor;
class USpnInventoryScreen;

/** Project settings of the inventory screen (M2B3.5). Keys are configurable; nothing here is a gameplay binding. */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "SPN Inventory UI"))
class SUPERNATURALPROT_API USpnInventoryUiSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	/** Keyboard key that toggles the inventory screen. */
	UPROPERTY(Config, EditAnywhere, Category = "Input")
	FKey OpenKey = EKeys::I;

	/** Gamepad key that toggles the inventory screen (DualSense: touchpad button / Options are free; Create/Share is used here). */
	UPROPERTY(Config, EditAnywhere, Category = "Input")
	FKey OpenGamepadKey = EKeys::Gamepad_Special_Left;

	/** Empty = follow the game's culture (German or English); "en" / "de" forces one. */
	UPROPERTY(Config, EditAnywhere, Category = "Localization")
	FString LanguageOverride;
};

/** Ids of containers that belong to the world (chests, boxes). Process-wide: the actors register themselves in BeginPlay, before any UI exists. */
SUPERNATURALPROT_API TSet<int64>& SpnWorldContainerRegistry();

/**
 * Owns the inventory screen of one local player: creates it, opens/closes it, switches the input mode and restores the previous one.
 * While the screen is open an input pre-processor routes keys to it, so the screen works independent of widget focus; while it is closed the
 * pre-processor only watches the open keys and never consumes anything else (gameplay input stays untouched).
 */
UCLASS()
class SUPERNATURALPROT_API USpnInventoryUiSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/** Opens the screen over the current inventory. False if there is no player controller / inventory yet. */
	bool OpenInventory();
	/** Opens the screen as a container view: tabs = the player's own container + this world container, the world container active (Night Build 01). */
	bool OpenContainer(int64 ContainerId);
	/** Containers that belong to the world (chests, boxes): hidden from the normal inventory, shown only through OpenContainer. */
	void RegisterWorldContainer(int64 ContainerId);
	void UnregisterWorldContainer(int64 ContainerId);
	bool IsWorldContainer(int64 ContainerId) const;
	/** True while a container view (not the plain inventory) is open. */
	bool IsContainerView() const { return bOpen && OpenedContainer != 0; }
	/** Another screen (pause, map, menu) owns the input: the inventory must not open. Set by USpnUiSubsystem. */
	TFunction<bool()> OpenBlocker;
	void CloseInventory();
	void ToggleInventory();
	bool IsOpen() const { return bOpen; }
	USpnInventoryScreen* GetScreen() const { return Screen; }

	/** Called by the input pre-processor. Returns true if the event was consumed. */
	bool ProcessKeyDown(const FKeyEvent& Event);
	bool ProcessAnalog(const FAnalogInputEvent& Event);

	/** Test hook: when false the pre-processor does not consume anything (tests route keys themselves). */
	bool bRoutingEnabled = true;

	/** True while the viewport console or any Slate text field (e.g. the editor Output Log command box) has keyboard focus. */
	bool IsTextEntryFocused() const;

private:
	APlayerController* GetController() const;
	bool IsConsoleActive() const;

	UPROPERTY(Transient) TObjectPtr<USpnInventoryScreen> Screen;
	TSharedPtr<FSpnInventoryInputProcessor> Processor;
	bool bOpen = false;
	int64 OpenedContainer = 0;
	bool OpenInternal(int64 FocusContainer);
	bool bHadShowMouseCursor = false;
	bool bHadGameInputMode = true;
	bool bWasPaused = false;
};
