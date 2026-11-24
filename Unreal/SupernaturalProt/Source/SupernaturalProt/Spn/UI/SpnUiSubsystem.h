#pragma once

#include "CoreMinimal.h"
#include "Containers/Ticker.h"
#include "SpnSaveSubsystem.h"
#include "Spn/SpnSettingsSubsystem.h"
#include "Spn/UI/SpnMenuScreen.h"
#include "Spn/UI/SpnUiNotify.h"
#include "Subsystems/LocalPlayerSubsystem.h"

#include "SpnUiSubsystem.generated.h"

class FSpnUiInputProcessor;
class USpnHudWidget;
class USpnMapScreen;
class USpnSessionSubsystem;
class USpnInventoryUiSubsystem;
class USpnMapSubsystem;
class USpnSaveSubsystem;

// Screen router of the Night Build 01 interface: main menu, pause menu, settings, save slots, confirmations, world map and the HUD. ENGINEERING
// PROPOSAL (flow, keys, wording), not master canon.
//
// ONE OWNER OF INPUT. A single input pre-processor decides, key by key, who gets the event, in a fixed order:
//   1. the viewport console or any text field has focus, or the key is a console key  -> nobody (the key is never touched)
//   2. the inventory screen is open (it has its own pre-processor)                    -> not this router
//   3. a modal screen of this router is open (menu page, slot list, dialog, map)      -> the top screen, always consumed (modal)
//   4. nothing is open (gameplay)                                                      -> Esc / Options open the pause menu, M / Triangle the map;
//                                                                                         every other key goes to the game
// The inventory in turn refuses to open while a modal screen is up (USpnInventoryUiSubsystem::OpenBlocker). The pause menu and the map pause
// the game (single player; co-op will need a different rule); the inventory does not (as before).
//
// Only active when the game flow is enabled (USpnSessionSubsystem), so existing levels and tests are unaffected.

UENUM()
enum class ESpnPage : uint8
{
	None,
	MainMenu,
	Pause,
	Settings,
	Slots,
	Confirm,
	Map,
};

UENUM()
enum class ESpnSlotMode : uint8 { Save, Load };

UCLASS()
class SUPERNATURALPROT_API USpnUiSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// --- state ---
	bool IsModalOpen() const { return Stack.Num() > 0; }
	ESpnPage GetTopPage() const { return Stack.Num() > 0 ? Stack.Last() : ESpnPage::None; }
	int32 GetStackDepth() const { return Stack.Num(); }
	bool IsPaused() const { return bPausedByUi; }
	bool IsUsingGamepad() const { return bUsingGamepad; }
	USpnHudWidget* GetHud() const { return Hud; }
	USpnMapScreen* GetMapScreen() const { return MapScreen; }
	USpnMenuScreen* GetMenuScreen(ESpnPage Page) const;

	// --- commands ---
	void ShowMainMenu();
	bool OpenPause();
	bool OpenMap();
	void OpenSettings();
	void OpenSlots(ESpnSlotMode Mode);
	void ShowConfirm(const FText& Title, const FText& Body, const FText& YesLabel, TFunction<void()> OnYes);
	/** Closes the top page (Back). The main menu itself cannot be closed. */
	void Back();
	/** Closes every page, restores the game input. */
	void CloseAll();
	void EnsureHud();

	// --- messages for the HUD ---
	void Notify(const FText& Text, SpnUi::ENote Kind);
	void SetInteractionPrompt(const FString& Text);

	// --- input (the pre-processor calls these; tests call them directly) ---
	bool ProcessKeyDown(const FKeyEvent& Event);
	bool ProcessAnalog(const FAnalogInputEvent& Event);
	/** Test hook: when false the pre-processor consumes nothing. */
	bool bRoutingEnabled = true;

	/** QA only (command line -SpnQaKeys): F5 = next scripted station, F9 = state dump to the log. Returns true if the key was one of them. */
	bool HandleQaKey(const FKey& Key);
	/** The state line F9 writes (also used by tests). */
	FString BuildStateDump() const;

	bool IsConsoleActive() const;
	bool IsTextEntryFocused() const;

	/** Test hook: slot id the next "New save slot" creates (empty = the next free save_NN). */
	FString NextSlotId() const;

private:
	// building
	USpnMenuScreen* GetOrCreateMenu(ESpnPage Page);
	void Push(ESpnPage Page);
	void Pop();
	void ShowTop();
	void RefreshPage(ESpnPage Page);
	void ApplyInputMode();
	void RestoreGameInput();
	void SetPaused(bool bPaused);

	// pages
	void FillMainMenu();
	void FillPause();
	void FillSettings();
	void FillSlots();
	void FillConfirm();
	void OnActivate(ESpnPage Page, FName Id, bool bSecondary);
	void OnAdjust(ESpnPage Page, FName Id, int32 Delta);

	// helpers
	APlayerController* GetController() const;
public:
	/** True while the player's interaction target offers Force (then the gamepad Triangle forces instead of opening the map). */
	bool IsForceTargetInFocus() const;
private:
	UGameInstance* GetInstance() const;
	USpnSessionSubsystem* GetSession() const;
	void HandleWorldReady(bool bMenu);
	void HandlePendingLoad(const FSpnSaveStatus& Status);
	FText DescribeSaveResult(const FSpnSaveStatus& Status) const;
	FText SlotTitle(const FString& SlotId) const;
	void DoSaveToSlot(const FString& SlotId);
	void DoLoadSlot(const FString& SlotId, bool bAcceptMismatch);

	UPROPERTY(Transient) TObjectPtr<USpnHudWidget> Hud;
	UPROPERTY(Transient) TObjectPtr<USpnMapScreen> MapScreen;
	UPROPERTY(Transient) TMap<ESpnPage, TObjectPtr<USpnMenuScreen>> Menus;

	TSharedPtr<FSpnUiInputProcessor> Processor;
	TArray<ESpnPage> Stack;
	bool bUsingGamepad = false;
	bool bPausedByUi = false;
	bool bHadShowMouseCursor = false;
	bool bInputModeApplied = false;
	ESpnSlotMode SlotMode = ESpnSlotMode::Load;
	FText ConfirmTitle, ConfirmBody, ConfirmYes;
	TFunction<void()> ConfirmAction;
	/** Slot list entries by item id ("slot:<id>") -> slot id; rebuilt by FillSlots. */
	TMap<FName, FString> SlotItems;
	FString PendingMismatchSlot;
	FDelegateHandle WorldReadyHandle;
	FDelegateHandle PendingLoadHandle;
	FDelegateHandle PostLoadHandle;
	TArray<FIntPoint> Resolutions;
	FText PendingNoteText;
	SpnUi::ENote PendingNoteKind = SpnUi::ENote::Info;
	bool bHasPendingNote = false;
	int32 QaStation = 0;
	/** QA only (-SpnQaKeys): writes the player pose to a file about 30 times per second so an external driver can steer with real input in a closed loop. */
	FTSTicker::FDelegateHandle QaPoseFeed;
	void StartQaPoseFeed();
	void ApplyGameplayInput(int32 Attempt);
};
