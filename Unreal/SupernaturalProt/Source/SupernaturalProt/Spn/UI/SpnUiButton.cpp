#include "Spn/UI/SpnUiButton.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Styling/CoreStyle.h"
#include "Styling/SlateBrush.h"
#include "Styling/SlateTypes.h"

namespace SpnUi
{
	FLinearColor SwatchColor(const FString& Seed)
	{
		// stable hash -> one of a few muted, cold-leaning hues (never saturated: this is a stand-in for an icon, not decoration)
		const uint32 Hash = GetTypeHash(Seed);
		const float Hue = static_cast<float>(Hash % 360u);
		return FLinearColor::MakeFromHSV8(static_cast<uint8>(Hue / 360.f * 255.f), 70, 96).CopyWithNewOpacity(1.f);
	}

	FSlateFontInfo Font(int32 Size, bool bBold)
	{
		return FCoreStyle::GetDefaultFontStyle(bBold ? TEXT("Bold") : TEXT("Regular"), Size);
	}
}

namespace
{
	FSlateBrush BoxBrush(const FLinearColor& Fill, const FLinearColor& Outline = FLinearColor::Transparent, float OutlineWidth = 0.f, float Radius = 3.f)
	{
		return FSlateRoundedBoxBrush(Fill, Radius, Outline, OutlineWidth);
	}

	UTextBlock* MakeText(UWidgetTree& Tree, int32 Size, bool bBold, const FLinearColor& Color)
	{
		UTextBlock* Text = Tree.ConstructWidget<UTextBlock>();
		Text->SetFont(SpnUi::Font(Size, bBold));
		Text->SetColorAndOpacity(FSlateColor(Color));
		Text->SetJustification(ETextJustify::Left);
		return Text;
	}
}

TSharedRef<SWidget> USpnUiButton::RebuildWidget()
{
	if (WidgetTree == nullptr)
	{
		WidgetTree = NewObject<UWidgetTree>(this, TEXT("WidgetTree"), RF_Transient);
	}
	if (WidgetTree->RootWidget == nullptr)
	{
		RingBorder = WidgetTree->ConstructWidget<UBorder>();
		RingBorder->SetPadding(FMargin(2.f));
		WidgetTree->RootWidget = RingBorder;

		BodyBorder = WidgetTree->ConstructWidget<UBorder>();
		BodyBorder->SetPadding(FMargin(9.f, 6.f, 10.f, 6.f));
		RingBorder->SetContent(BodyBorder);

		UHorizontalBox* Line = WidgetTree->ConstructWidget<UHorizontalBox>();
		BodyBorder->SetContent(Line);

		// icon swatch (the fallback when there is no icon texture)
		USizeBox* IconSize = WidgetTree->ConstructWidget<USizeBox>();
		IconSize->SetWidthOverride(34.f);
		IconSize->SetHeightOverride(34.f);
		IconBorder = WidgetTree->ConstructWidget<UBorder>();
		IconBorder->SetHorizontalAlignment(HAlign_Center);
		IconBorder->SetVerticalAlignment(VAlign_Center);
		IconText = MakeText(*WidgetTree, 15, true, SpnUi::Text);
		IconBorder->SetContent(IconText);
		IconSize->SetContent(IconBorder);
		UHorizontalBoxSlot* IconSlot = Line->AddChildToHorizontalBox(IconSize);
		IconSlot->SetVerticalAlignment(VAlign_Center);
		IconSlot->SetPadding(FMargin(0.f, 0.f, 10.f, 0.f));

		UVerticalBox* Texts = WidgetTree->ConstructWidget<UVerticalBox>();
		TitleText = MakeText(*WidgetTree, 15, false, SpnUi::Text);
		SubtitleText = MakeText(*WidgetTree, 11, false, SpnUi::TextDim);
		SubtitleText->SetAutoWrapText(true);
		Texts->AddChildToVerticalBox(TitleText);
		UVerticalBoxSlot* SubSlot = Texts->AddChildToVerticalBox(SubtitleText);
		SubSlot->SetPadding(FMargin(0.f, 1.f, 0.f, 0.f));
		UHorizontalBoxSlot* TextSlot = Line->AddChildToHorizontalBox(Texts);
		TextSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
		TextSlot->SetVerticalAlignment(VAlign_Center);

		BadgeBorder = WidgetTree->ConstructWidget<UBorder>();
		BadgeBorder->SetPadding(FMargin(6.f, 2.f));
		BadgeText = MakeText(*WidgetTree, 10, true, SpnUi::Accent);
		BadgeBorder->SetContent(BadgeText);
		UHorizontalBoxSlot* BadgeSlot = Line->AddChildToHorizontalBox(BadgeBorder);
		BadgeSlot->SetVerticalAlignment(VAlign_Center);
		BadgeSlot->SetPadding(FMargin(8.f, 0.f, 0.f, 0.f));

		RightText = MakeText(*WidgetTree, 14, true, SpnUi::Text);
		RightText->SetJustification(ETextJustify::Right);
		UHorizontalBoxSlot* RightSlot = Line->AddChildToHorizontalBox(RightText);
		RightSlot->SetVerticalAlignment(VAlign_Center);
		RightSlot->SetPadding(FMargin(12.f, 0.f, 0.f, 0.f));
	}
	SetVisibility(ESlateVisibility::Visible);
	ApplyContent();
	ApplyVisuals();
	return Super::RebuildWidget();
}

void USpnUiButton::SetContent(const FSpnUiButtonContent& InContent)
{
	Content = InContent;
	ApplyContent();
}

void USpnUiButton::SetVisualState(bool bInSelected, bool bInFocused, bool bInEnabled)
{
	if (bSelected == bInSelected && bFocusRing == bInFocused && bEnabledState == bInEnabled)
	{
		return;
	}
	bSelected = bInSelected;
	bFocusRing = bInFocused;
	bEnabledState = bInEnabled;
	ApplyVisuals();
}

void USpnUiButton::ApplyContent()
{
	if (TitleText == nullptr)
	{
		return; // the widget tree is built on the first RebuildWidget
	}
	TitleText->SetText(Content.Title);
	SubtitleText->SetText(Content.Subtitle);
	SubtitleText->SetVisibility(Content.Subtitle.IsEmpty() ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
	SubtitleText->SetColorAndOpacity(FSlateColor(Content.SubtitleColor));
	RightText->SetText(Content.Right);
	RightText->SetVisibility(Content.Right.IsEmpty() ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
	BadgeText->SetText(Content.Badge);
	BadgeText->SetColorAndOpacity(FSlateColor(Content.BadgeColor));
	BadgeBorder->SetBrush(BoxBrush(Content.BadgeColor.CopyWithNewOpacity(0.16f), Content.BadgeColor.CopyWithNewOpacity(0.55f), 1.f, 2.f));
	BadgeBorder->SetVisibility(Content.Badge.IsEmpty() ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
	IconBorder->GetParent()->SetVisibility(Content.bShowIcon ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	IconBorder->SetBrush(BoxBrush(Content.IconColor, Content.IconColor * 1.6f, 1.f, 3.f));
	IconText->SetText(FText::FromString(Content.IconLetter));
	ApplyVisuals();
}

void USpnUiButton::ApplyVisuals()
{
	if (BodyBorder == nullptr)
	{
		return;
	}
	const FLinearColor Fill = bSelected ? SpnUi::RowSelected : (bHovered && bEnabledState ? SpnUi::RowHover : SpnUi::Row);
	// the ring: bright and thick for the focus (keyboard / controller), a thin accent line for a selection outside the active zone, nothing otherwise
	FLinearColor Ring = SpnUi::Transparent;
	if (bFocusRing)
	{
		Ring = SpnUi::Ring;
	}
	else if (bSelected)
	{
		Ring = SpnUi::AccentDim;
	}
	RingBorder->SetBrush(BoxBrush(Ring, FLinearColor::Transparent, 0.f, 4.f));
	BodyBorder->SetBrush(BoxBrush(Fill, bSelected ? SpnUi::Accent.CopyWithNewOpacity(0.55f) : SpnUi::PanelEdge, 1.f, 3.f));
	const float Dim = bEnabledState ? 1.f : 0.45f;
	TitleText->SetColorAndOpacity(FSlateColor(SpnUi::Text * Dim));
	RightText->SetColorAndOpacity(FSlateColor(SpnUi::Text * Dim));
	SetRenderOpacity(bEnabledState ? 1.f : 0.82f);
}

FString USpnUiButton::GetTitleString() const { return TitleText != nullptr ? TitleText->GetText().ToString() : FString(); }
FString USpnUiButton::GetSubtitleString() const { return SubtitleText != nullptr && SubtitleText->GetVisibility() != ESlateVisibility::Collapsed ? SubtitleText->GetText().ToString() : FString(); }
FString USpnUiButton::GetRightString() const { return RightText != nullptr && RightText->GetVisibility() != ESlateVisibility::Collapsed ? RightText->GetText().ToString() : FString(); }
FString USpnUiButton::GetBadgeString() const { return BadgeText != nullptr && BadgeBorder->GetVisibility() != ESlateVisibility::Collapsed ? BadgeText->GetText().ToString() : FString(); }
FString USpnUiButton::GetIconLetterString() const { return IconText != nullptr ? IconText->GetText().ToString() : FString(); }

FReply USpnUiButton::NativeOnMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& Event)
{
	if (Event.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		OnClicked.ExecuteIfBound(this);
		return FReply::Handled();
	}
	if (Event.GetEffectingButton() == EKeys::RightMouseButton)
	{
		OnRightClicked.ExecuteIfBound(this);
		return FReply::Handled();
	}
	return Super::NativeOnMouseButtonDown(Geometry, Event);
}

FReply USpnUiButton::NativeOnMouseButtonDoubleClick(const FGeometry& Geometry, const FPointerEvent& Event)
{
	if (Event.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		OnDoubleClicked.ExecuteIfBound(this);
		return FReply::Handled();
	}
	return Super::NativeOnMouseButtonDoubleClick(Geometry, Event);
}

void USpnUiButton::NativeOnMouseEnter(const FGeometry& Geometry, const FPointerEvent& Event)
{
	Super::NativeOnMouseEnter(Geometry, Event);
	bHovered = true;
	ApplyVisuals();
	OnHovered.ExecuteIfBound(this);
}

void USpnUiButton::NativeOnMouseLeave(const FPointerEvent& Event)
{
	Super::NativeOnMouseLeave(Event);
	bHovered = false;
	ApplyVisuals();
}
