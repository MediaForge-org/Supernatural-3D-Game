#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpnInventoryViewModel.h"
#include "Spn/UI/SpnUiButton.h"

#include "SpnInventoryScreen.generated.h"

class UBorder;
class UHorizontalBox;
class UOverlay;
class UProgressBar;
class UScrollBox;
class USpnInventorySubsystem;
class USpnSaveSubsystem;
class UTextBlock;
class UVerticalBox;

// The graphical inventory (M2B3.5): a UMG screen over USpnInventoryViewModel. ENGINEERING PROPOSAL (layout, texts, key assignment), not master canon.
//
// It shows what the view-model says about the REAL inventory and sends every action back through the view-model (-> USpnInventorySubsystem ->
// Core). It contains no inventory rule: whether an action is possible, and why not, comes from the Core's dry run; a failed action changes nothing.
//
// LAYOUT (designed for 1920x1080, scaled to fit any resolution by a UScaleBox):
//   header      title | container tabs | close hint
//   left        EQUIPMENT (slots of the primary container)  +  CAPACITY (stacks, weight, volume of the active container)
//   centre      ITEMS of the active container (rows: swatch, name, category, condition, quantity, equipped badge)
//   right       DETAILS of the selected item  +  ACTIONS (with the reason when unavailable) + quantity
//   footer      message line + input hints (keyboard or controller, whichever was used last)
//   overlay     confirmation dialog (discard)
//
// INPUT (keyboard, mouse, DualSense; none of it is a gameplay binding):
//   Up/Down/D-pad/left stick  move in the active panel        Left/Right  switch panel (equipment | items | actions)
//   Tab, Shift+Tab, L1/R1     previous/next container         Enter, Space, Cross   use (equip/unequip, run the action, confirm)
//   Esc, Backspace, Circle    back (action menu -> items), then close; cancels a dialog       I, Options, touchpad   close
//   + / -, R2 / L2            quantity                         Delete, Square   discard (asks first)
//   PageUp/PageDown, Home/End move faster                      right stick  scroll the list     mouse: click, double click (use), right click (actions), wheel
// The keys are routed by USpnInventoryUiSubsystem's input pre-processor while the screen is open (so they do not depend on widget focus) and by
// NativeOnKeyDown when the widget has focus; both end in HandleKey().
UCLASS()
class SUPERNATURALPROT_API USpnInventoryScreen : public UUserWidget
{
	GENERATED_BODY()

public:
	enum class EZone : uint8 { Items, Equipment, Actions };

	/** Binds to the subsystems (the screen creates its own view-model). Safe to call again. */
	void Setup(USpnInventorySubsystem* InInventory, USpnSaveSubsystem* InSave, int64 InPrimaryContainerId = 0);
	void Teardown();
	USpnInventoryViewModel* GetViewModel() const { return ViewModel; }

	/** Called by the owner whenever the screen is shown: rebuilds everything from the current state (stale items never survive a reopening). */
	void OnShown();

	/** Raised when the user asks to close (Esc, I, Options, ...). The owner restores the input state. */
	DECLARE_DELEGATE(FOnCloseRequested);
	FOnCloseRequested OnCloseRequested;

	// --- input entry points (also used directly by tests) ---
	/** Returns true if the key was used. */
	bool HandleKey(const FKey& Key, bool bShift = false, bool bRepeat = false);
	/** Analog stick / trigger input (the owner passes the raw axis value); sticks are turned into repeated steps in NativeTick. */
	void HandleAnalog(const FKey& Key, float Value);
	/** True if the last input came from a gamepad (the hints show controller labels). */
	bool IsUsingGamepad() const { return bUsingGamepad; }
	void SetUsingGamepad(bool bValue);

	// --- state and inspection (what is really on screen) ---
	EZone GetZone() const { return Zone; }
	int64 GetActiveContainer() const { return ActiveContainer; }
	bool IsDialogOpen() const { return bDialogOpen; }
	int32 GetVisibleRowCount() const { return VisibleRows; }
	int32 GetBuiltRowWidgetCount() const { return RowPool.Num(); }
	/** The widget of a visible row (QA clicks it with a real pointer event). */
	UWidget* GetRowWidget(int32 Index) const { return RowPool.IsValidIndex(Index) && Index < VisibleRows ? static_cast<UWidget*>(RowPool[Index]) : nullptr; }
	FString GetRowTitle(int32 Index) const;
	FString GetRowRight(int32 Index) const;
	FString GetRowSubtitle(int32 Index) const;
	FString GetRowBadge(int32 Index) const;
	FString GetRowIconLetter(int32 Index) const;
	bool IsRowSelected(int32 Index) const;
	bool IsRowFocusRing(int32 Index) const;
	FString GetTabTitle(int32 Index) const;
	int32 GetTabCount() const { return TabPool.Num(); }
	int32 GetVisibleTabCount() const { return VisibleTabs; }
	bool IsTabSelected(int32 Index) const;
	int32 GetSlotCount() const { return VisibleSlots; }
	FString GetSlotTitle(int32 Index) const;
	FString GetSlotSubtitle(int32 Index) const;
	bool IsSlotFocusRing(int32 Index) const;
	int32 GetActionCount() const { return VisibleActions; }
	FString GetActionTitle(int32 Index) const;
	FString GetActionSubtitle(int32 Index) const;
	FString GetActionRight(int32 Index) const;
	bool IsActionEnabled(int32 Index) const;
	bool IsActionFocusRing(int32 Index) const;
	int32 GetFocusedActionIndex() const { return ActionIndex; }
	/** The dimmed line shown instead of actions when nothing is selected (empty when real actions are listed). */
	FString GetActionsEmptyText() const;
	FString GetDetailsTitle() const;
	FString GetDetailsText() const;
	FString GetCapacityText(ESpnCapacityKind Kind) const;
	float GetCapacityPercent(ESpnCapacityKind Kind) const;
	FString GetMessageText() const;
	FString GetHintText() const;
	FString GetEmptyText() const;
	FString GetDialogText() const;
	bool IsDialogConfirmFocused() const { return bDialogConfirmFocused; }
	FString GetHeaderTitle() const;
	int32 GetQuantity() const { return QuantityValue; }

	// --- measurements (milliseconds of the most recent operation; tests and the performance report read them) ---
	double LastRefreshMs = 0.0;
	double LastRowsBuildMs = 0.0;
	double LastSelectionMs = 0.0;
	int32 RowsRebuilt = 0;
	int32 FullRefreshes = 0;
	int32 DetailsRefreshes = 0;
	int32 EquipmentRefreshes = 0;
	/** Why the most recent full refresh was requested (diagnostics for the performance report). */
	FString LastFullReason;
	/** Which path rebuilt the rows (diagnostics): dirty flush / full refresh / container switch. */
	int32 RowsFromFlush = 0, RowsFromAll = 0, RowsFromSwitch = 0;

	static constexpr int32 MaxRowsShown = 100;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual FReply NativeOnKeyDown(const FGeometry& Geometry, const FKeyEvent& Event) override;
	virtual FReply NativeOnAnalogValueChanged(const FGeometry& Geometry, const FAnalogInputEvent& Event) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& Event) override;
	virtual FReply NativeOnMouseWheel(const FGeometry& Geometry, const FPointerEvent& Event) override;
	virtual FReply NativeOnMouseMove(const FGeometry& Geometry, const FPointerEvent& Event) override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

private:
	// building
	void BuildTree();
	USpnUiButton* NewButton();
	UTextBlock* NewText(int32 Size, bool bBold, const FLinearColor& Color);
	UBorder* NewPanel(const FText& Title, UVerticalBox*& OutBody);

	// model -> widgets
	void BindContainers();
	void SyncAll();
	void SyncTabs();
	void SyncEquipment();
	void SyncRows();
	void SyncSelectionVisuals();
	void SyncDetails();
	void SyncActions();
	void SyncCapacity();
	void SyncFooter();
	void FlushDirty();
	void EnsureValidActiveContainer();
	const FSpnContainerPanel* ActivePanel() const;
	const FSpnContainerPanel* PrimaryPanel() const;
	int32 SelectedRowIndexInActive() const;

	// events
	void OnVmChanged();
	void OnVmContainerChanged(int64 ContainerId);
	void OnVmSelectionChanged();
	void OnVmMessage(const FSpnInventoryMessage& Message);
	void OnInventoryChangedDetailed(const FSpnInventoryChange& Change);
	void ShowMessage(const FText& Text, ESpnMessageSeverity Severity);
	void ShowSpnMessage(const FSpnInventoryMessage& Message);

	// user intents
	void SetZone(EZone NewZone);
	void ChangeContainer(int32 Step);
	void SetActiveContainer(int64 ContainerId, bool bSelectFirst);
	void ActivateRow(USpnUiButton* Button, bool bUse);
	void ActivateSlot(USpnUiButton* Button);
	void ActivateAction(int32 Index);
	void RunAction(const FSpnActionOption& Option, int32 Quantity);
	void AdjustQuantity(int32 Delta);
	void ResetQuantityForFocusedAction();
	void OpenDiscardDialog(const FSpnActionOption& Option, int32 Quantity);
	void CloseDialog();
	void ConfirmDialog();
	void MoveActionFocus(int32 Delta);
	void GoBack();
	void StepStick(const FVector2D& Direction);
	FText ActionLabel(const FSpnActionOption& Option) const;
	FText ContainerName(const FSpnContainerPanel& Panel) const;
	FText ItemName(const FSpnItemRow& Row) const;
	FText ItemDescription(const FSpnItemRow& Row) const;
	FText CategoryName(const FString& CategoryId) const;
	FText SlotName(const FString& SlotId) const;
	FText ErrorText(ESpnInventoryError Error) const;

	// state
	/** UMG only holds its Slate widget weakly; the screen keeps it alive while it is shown (the viewport does the same once it is added). */
	TSharedPtr<SWidget> HeldSlate;
	UPROPERTY(Transient) TObjectPtr<USpnInventoryViewModel> ViewModel;
	TWeakObjectPtr<USpnInventorySubsystem> Inventory;
	TWeakObjectPtr<USpnSaveSubsystem> Save;
	int64 PrimaryContainerId = 0;
	int64 ActiveContainer = 0;
	EZone Zone = EZone::Items;
	TArray<FSpnActionOption> Actions;
	int32 ActionIndex = 0;
	int32 QuantityValue = 1;
	bool bUsingGamepad = false;
	bool bUsingMouseMode = false;
	bool bDialogOpen = false;
	bool bDialogConfirmFocused = false;
	FSpnActionOption PendingOption;
	int32 PendingQuantity = 1;
	bool bRebindGuard = false;
	int32 WindowStart = 0;
	int32 VisibleRows = 0, VisibleTabs = 0, VisibleSlots = 0, VisibleActions = 0;
	double MessageUntil = 0.0;
	double Clock = 0.0;
	FVector2D StickDirection = FVector2D::ZeroVector;
	double NextStickRepeat = 0.0;
	float ScrollStick = 0.f;
	FDelegateHandle InventoryChangedHandle;
	bool bDirtyRows = false, bDirtyEquipment = false, bDirtyTabs = false, bDirtyAll = false;

	// widgets
	UPROPERTY(Transient) TObjectPtr<UBorder> TitleBadge;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> TitleText;
	UPROPERTY(Transient) TObjectPtr<UHorizontalBox> TabBox;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> CloseHintText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> EquipmentTitle;
	UPROPERTY(Transient) TObjectPtr<UVerticalBox> SlotBox;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> CapacityTitle;
	UPROPERTY(Transient) TObjectPtr<UVerticalBox> CapacityBox;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> ItemsTitle;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> ItemsWindowText;
	UPROPERTY(Transient) TObjectPtr<UScrollBox> RowScroll;
	UPROPERTY(Transient) TObjectPtr<UVerticalBox> RowBox;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> EmptyText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> DetailsTitleLabel;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> DetailsName;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> DetailsTest;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> DetailsDescription;
	UPROPERTY(Transient) TObjectPtr<UVerticalBox> DetailsStats;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> ActionsTitle;
	UPROPERTY(Transient) TObjectPtr<UVerticalBox> ActionBox;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> MessageLine;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> HintLine;
	UPROPERTY(Transient) TObjectPtr<UBorder> DialogLayer;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> DialogTitle;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> DialogBody;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> DialogHint;
	UPROPERTY(Transient) TObjectPtr<USpnUiButton> DialogConfirm;
	UPROPERTY(Transient) TObjectPtr<USpnUiButton> DialogCancel;

	UPROPERTY(Transient) TArray<TObjectPtr<USpnUiButton>> RowPool;
	UPROPERTY(Transient) TArray<TObjectPtr<USpnUiButton>> TabPool;
	UPROPERTY(Transient) TArray<TObjectPtr<USpnUiButton>> SlotPool;
	UPROPERTY(Transient) TArray<TObjectPtr<USpnUiButton>> ActionPool;
	UPROPERTY(Transient) TArray<TObjectPtr<UTextBlock>> CapacityLabels;
	UPROPERTY(Transient) TArray<TObjectPtr<UTextBlock>> CapacityValues;
	UPROPERTY(Transient) TArray<TObjectPtr<UProgressBar>> CapacityBars;
	TArray<FSpnItemRow> ShownRows;
};
