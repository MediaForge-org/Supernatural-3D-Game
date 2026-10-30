#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Spn/UI/SpnUiButton.h"

#include "SpnMenuScreen.generated.h"

class UBorder;
class UScrollBox;
class UTextBlock;
class UVerticalBox;

// One reusable list-menu screen (Night Build 01): the main menu, the pause menu, the settings, the save-slot list and the confirmation dialogs are
// all instances of this widget with different items. It owns presentation and list navigation only; what an item does is decided by the owner
// (USpnUiSubsystem) through the delegates. ENGINEERING PROPOSAL (layout, wording), not master canon.
//
// INPUT: the owner's input pre-processor calls HandleKey(); the widget itself never consumes keys through Slate focus, so the console, text entry
// and the other screens keep their predictable priority. Mouse: hover selects, click activates.
//   Up/Down, W/S, D-pad, left stick  move      Enter, Space, Cross  activate      Left/Right, A/D, D-pad  adjust the value of an item (settings)
//   Esc, Backspace, Circle  back      PageUp/PageDown  jump

/** One line of a menu. */
struct FSpnMenuItem
{
	FName Id;
	FText Title;
	FText Subtitle;
	/** Right-aligned value (settings: the current choice, slots: the date). */
	FText Value;
	bool bEnabled = true;
	/** Settings rows are adjusted with Left/Right; the value is shown with arrows. */
	bool bAdjustable = false;
	/** Section headers cannot be selected. */
	bool bHeader = false;
	/** Colour of the subtitle (reasons / warnings). */
	FLinearColor SubtitleColor = SpnUi::TextDim;
};

UENUM()
enum class ESpnMenuStyle : uint8
{
	/** Centred panel over a dimmed backdrop (pause, settings, slots, dialogs). */
	Panel,
	/** Left-aligned title block over the 3D scene (main menu). */
	MainMenu,
};

UCLASS()
class SUPERNATURALPROT_API USpnMenuScreen : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_TwoParams(FOnActivate, FName /*ItemId*/, bool /*bSecondary*/);
	DECLARE_DELEGATE_TwoParams(FOnAdjust, FName /*ItemId*/, int32 /*Delta*/);
	DECLARE_DELEGATE(FOnBack);

	FOnActivate OnActivate;
	FOnAdjust OnAdjust;
	FOnBack OnBack;

	void SetStyle(ESpnMenuStyle InStyle) { Style = InStyle; }
	void SetTexts(const FText& InTitle, const FText& InSubtitle, const FText& InFooter);
	/** Replaces the items. Keeps the selection on the same id when it still exists. */
	void SetItems(const TArray<FSpnMenuItem>& InItems, FName SelectId = NAME_None);
	/** One-line status below the list (errors in red, confirmations in green). */
	void SetMessage(const FText& Text, const FLinearColor& Color);
	/** Opaque (no 3D scene behind) or a translucent dimmer. */
	void SetBackdropOpacity(float Opacity);

	/** Returns true if the key was used. */
	bool HandleKey(const FKey& Key, bool bRepeat);
	void HandleAnalog(const FKey& Key, float Value);

	// --- inspection (tests read what is really on screen) ---
	int32 GetItemCount() const { return Items.Num(); }
	int32 GetSelectedIndex() const { return Selected; }
	FName GetSelectedId() const { return Items.IsValidIndex(Selected) ? Items[Selected].Id : NAME_None; }
	FString GetItemTitle(int32 Index) const;
	FString GetItemValue(int32 Index) const;
	bool IsItemEnabled(int32 Index) const { return Items.IsValidIndex(Index) && Items[Index].bEnabled; }
	FString GetTitleString() const;
	FString GetMessageString() const;
	int32 FindIndex(FName Id) const;
	/** The real button widget of an item (QA clicks it with a pointer event). */
	UWidget* GetItemWidget(int32 Index) const { return Pool.IsValidIndex(Index) && Index < Shown ? static_cast<UWidget*>(Pool[Index]) : nullptr; }

	/** Moves the selection to an item without activating it. */
	void Select(int32 Index);
	void Activate(bool bSecondary);

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

private:
	void BuildTree();
	void Sync();
	void Move(int32 Step);
	void OnButtonClicked(USpnUiButton* Button);
	void OnButtonHovered(USpnUiButton* Button);
	UTextBlock* NewText(int32 Size, bool bBold, const FLinearColor& Color);
	USpnUiButton* NewButton();

	TSharedPtr<SWidget> HeldSlate;
	ESpnMenuStyle Style = ESpnMenuStyle::Panel;
	TArray<FSpnMenuItem> Items;
	int32 Selected = 0;
	int32 Shown = 0;
	FText Title, Subtitle, Footer;
	FText MessageText;
	FLinearColor MessageColor = SpnUi::TextDim;
	float BackdropOpacity = 0.72f;
	FVector2D StickDirection = FVector2D::ZeroVector;
	double NextStickRepeat = 0.0;
	double Clock = 0.0;
	bool bDirty = true;

	UPROPERTY(Transient) TObjectPtr<UBorder> Backdrop;
	UPROPERTY(Transient) TObjectPtr<UBorder> PanelBorder;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> TitleText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> SubtitleText;
	UPROPERTY(Transient) TObjectPtr<UScrollBox> ListScroll;
	UPROPERTY(Transient) TObjectPtr<UVerticalBox> ListBox;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> MessageLine;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> FooterText;
	UPROPERTY(Transient) TArray<TObjectPtr<USpnUiButton>> Pool;
};
