#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "SpnUiButton.generated.h"

class UBorder;
class UHorizontalBox;
class UTextBlock;
class UVerticalBox;
class USizeBox;

// Palette and metrics of the SPN interface (M2B3.5). One place, so the look stays consistent and can be restyled later.
namespace SpnUi
{
	// Dark, restrained, slightly cold. sRGB-like values used directly as linear colours with the engine's UMG gamma (looks like these values).
	inline const FLinearColor Backdrop(0.f, 0.f, 0.f, 0.72f);
	inline const FLinearColor Panel(0.040f, 0.046f, 0.052f, 0.96f);
	inline const FLinearColor PanelEdge(0.105f, 0.120f, 0.135f, 1.f);
	inline const FLinearColor Header(0.026f, 0.031f, 0.036f, 1.f);
	inline const FLinearColor Row(0.066f, 0.075f, 0.084f, 1.f);
	inline const FLinearColor RowHover(0.092f, 0.106f, 0.118f, 1.f);
	inline const FLinearColor RowSelected(0.105f, 0.158f, 0.178f, 1.f);
	inline const FLinearColor Accent(0.34f, 0.66f, 0.72f, 1.f);        // pale cold teal: selection, focus
	inline const FLinearColor AccentDim(0.18f, 0.34f, 0.38f, 1.f);
	inline const FLinearColor Ring(0.72f, 0.92f, 0.96f, 1.f);          // controller / keyboard focus ring
	inline const FLinearColor Text(0.84f, 0.87f, 0.89f, 1.f);
	inline const FLinearColor TextDim(0.50f, 0.54f, 0.57f, 1.f);
	inline const FLinearColor TextFaint(0.34f, 0.37f, 0.40f, 1.f);
	inline const FLinearColor Warning(0.88f, 0.67f, 0.30f, 1.f);       // amber
	inline const FLinearColor Error(0.86f, 0.38f, 0.34f, 1.f);
	inline const FLinearColor Good(0.46f, 0.78f, 0.60f, 1.f);
	inline const FLinearColor Disabled(0.40f, 0.43f, 0.45f, 1.f);
	inline const FLinearColor Transparent(0.f, 0.f, 0.f, 0.f);

	/** Muted swatch colour for an item without an icon, derived from its category id (stable, not meaningful). */
	SUPERNATURALPROT_API FLinearColor SwatchColor(const FString& Seed);
	SUPERNATURALPROT_API FSlateFontInfo Font(int32 Size, bool bBold = false);
}

/** What a button shows. Every field is optional; empty text collapses its element. */
struct FSpnUiButtonContent
{
	FText Title;
	FText Subtitle;
	/** Right-aligned text (quantity, weight ...). */
	FText Right;
	/** Small pill (EQUIPPED ...). */
	FText Badge;
	FLinearColor BadgeColor = SpnUi::Accent;
	/** Colour of the second line (reasons in red, hints dim). */
	FLinearColor SubtitleColor = SpnUi::TextDim;
	bool bShowIcon = false;
	/** One or two characters on the swatch when there is no icon texture. */
	FString IconLetter;
	FLinearColor IconColor = SpnUi::AccentDim;
	/** Compact vertical tile for a grid (icon on top, name below, quantity and badge under it); the subtitle is not shown. */
	bool bTile = false;
};

/**
 * One selectable line of the SPN interface: an item row, an equipment slot, a container tab, an action, a dialog button. Pure UMG (a UUserWidget
 * with a widget tree of UBorder / UHorizontalBox / UVerticalBox / UTextBlock); it owns the mouse handling (hover, click, double click, right
 * click) and reports through native delegates. Keyboard and controller input never reach it: the screen decides what the selection is.
 */
UCLASS()
class SUPERNATURALPROT_API USpnUiButton : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnButtonEvent, USpnUiButton*);

	void SetContent(const FSpnUiButtonContent& InContent);
	const FSpnUiButtonContent& GetContent() const { return Content; }

	/** Selected = the model's selection; Focused = the focus ring (the active zone of the screen); Enabled = false dims it (the Core said no). */
	void SetVisualState(bool bInSelected, bool bInFocused, bool bInEnabled);
	bool IsSelected() const { return bSelected; }
	bool IsFocusedRing() const { return bFocusRing; }
	bool IsEnabledState() const { return bEnabledState; }

	/** Payload the owner attaches (stack id, slot index, action index ...). */
	int64 Tag = 0;
	FString TagText;

	FOnButtonEvent OnClicked;
	FOnButtonEvent OnDoubleClicked;
	FOnButtonEvent OnRightClicked;
	FOnButtonEvent OnHovered;

	// --- what is on screen (tests read the real widgets, not the model) ---
	FString GetTitleString() const;
	FString GetSubtitleString() const;
	FString GetRightString() const;
	FString GetBadgeString() const;
	FString GetIconLetterString() const;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& Event) override;
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& Geometry, const FPointerEvent& Event) override;
	virtual void NativeOnMouseEnter(const FGeometry& Geometry, const FPointerEvent& Event) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& Event) override;

private:
	void ApplyContent();
	void ApplyVisuals();
	void ApplyLayout(bool bTile);
	bool bTileLayout = false;

	FSpnUiButtonContent Content;
	bool bSelected = false;
	bool bFocusRing = false;
	bool bEnabledState = true;
	bool bHovered = false;

	UPROPERTY(Transient) TObjectPtr<USizeBox> SizeRoot;
	UPROPERTY(Transient) TObjectPtr<UHorizontalBox> LineBox;
	UPROPERTY(Transient) TObjectPtr<UVerticalBox> TextsBox;
	UPROPERTY(Transient) TObjectPtr<UVerticalBox> TileBox;
	UPROPERTY(Transient) TObjectPtr<UHorizontalBox> TileFooter;
	UPROPERTY(Transient) TObjectPtr<USizeBox> IconSizeBox;
	UPROPERTY(Transient) TObjectPtr<UBorder> RingBorder;
	UPROPERTY(Transient) TObjectPtr<UBorder> BodyBorder;
	UPROPERTY(Transient) TObjectPtr<UBorder> IconBorder;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> IconText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> TitleText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> SubtitleText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> RightText;
	UPROPERTY(Transient) TObjectPtr<UBorder> BadgeBorder;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> BadgeText;
};
