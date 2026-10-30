#include "Spn/UI/SpnMenuScreen.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/ScaleBox.h"
#include "Components/ScrollBox.h"
#include "Components/SizeBox.h"
#include "Components/Spacer.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Engine/GameInstance.h"
#include "Spn/UI/SpnUiText.h"
#include "Styling/SlateBrush.h"
#include "Styling/SlateTypes.h"

namespace
{
	FSlateBrush Box(const FLinearColor& Fill, const FLinearColor& Outline = FLinearColor::Transparent, float OutlineWidth = 0.f, float Radius = 4.f)
	{
		return FSlateRoundedBoxBrush(Fill, Radius, Outline, OutlineWidth);
	}

	constexpr double StickRepeatSeconds = 0.22;
	constexpr float StickThreshold = 0.6f;
}

UTextBlock* USpnMenuScreen::NewText(int32 Size, bool bBold, const FLinearColor& Color)
{
	UTextBlock* Text = WidgetTree->ConstructWidget<UTextBlock>();
	Text->SetFont(SpnUi::Font(Size, bBold));
	Text->SetColorAndOpacity(FSlateColor(Color));
	return Text;
}

USpnUiButton* USpnMenuScreen::NewButton()
{
	if (APlayerController* Owner = GetOwningPlayer())
	{
		return CreateWidget<USpnUiButton>(Owner, USpnUiButton::StaticClass());
	}
	return CreateWidget<USpnUiButton>(GetWorld(), USpnUiButton::StaticClass());
}

TSharedRef<SWidget> USpnMenuScreen::RebuildWidget()
{
	if (WidgetTree == nullptr)
	{
		WidgetTree = NewObject<UWidgetTree>(this, TEXT("WidgetTree"), RF_Transient);
	}
	if (WidgetTree->RootWidget == nullptr)
	{
		BuildTree();
	}
	TSharedRef<SWidget> Slate = Super::RebuildWidget();
	HeldSlate = Slate;
	Sync();
	return Slate;
}

void USpnMenuScreen::ReleaseSlateResources(bool bReleaseChildren)
{
	HeldSlate.Reset();
	Super::ReleaseSlateResources(bReleaseChildren);
}

void USpnMenuScreen::BuildTree()
{
	const bool bMain = Style == ESpnMenuStyle::MainMenu;
	UOverlay* Root = WidgetTree->ConstructWidget<UOverlay>();
	WidgetTree->RootWidget = Root;

	Backdrop = WidgetTree->ConstructWidget<UBorder>();
	Backdrop->SetBrush(Box(FLinearColor(0.f, 0.f, 0.f, BackdropOpacity), FLinearColor::Transparent, 0.f, 0.f));
	UOverlaySlot* BackdropSlot = Root->AddChildToOverlay(Backdrop);
	BackdropSlot->SetHorizontalAlignment(HAlign_Fill);
	BackdropSlot->SetVerticalAlignment(VAlign_Fill);

	UScaleBox* Scale = WidgetTree->ConstructWidget<UScaleBox>();
	Scale->SetStretch(EStretch::ScaleToFit);
	UOverlaySlot* ScaleSlot = Root->AddChildToOverlay(Scale);
	ScaleSlot->SetHorizontalAlignment(HAlign_Fill);
	ScaleSlot->SetVerticalAlignment(VAlign_Fill);
	ScaleSlot->SetPadding(FMargin(40.f));

	USizeBox* Design = WidgetTree->ConstructWidget<USizeBox>();
	Design->SetWidthOverride(1840.f);
	Design->SetHeightOverride(1000.f);
	Scale->AddChild(Design);

	UBorder* Align = WidgetTree->ConstructWidget<UBorder>();
	Align->SetBrush(Box(FLinearColor::Transparent, FLinearColor::Transparent, 0.f, 0.f));
	Align->SetHorizontalAlignment(bMain ? HAlign_Left : HAlign_Center);
	Align->SetVerticalAlignment(VAlign_Center);
	Align->SetPadding(FMargin(bMain ? 110.f : 0.f, 0.f, 0.f, 0.f));
	Design->AddChild(Align);

	USizeBox* Width = WidgetTree->ConstructWidget<USizeBox>();
	Width->SetWidthOverride(bMain ? 640.f : 820.f);
	Align->SetContent(Width);

	PanelBorder = WidgetTree->ConstructWidget<UBorder>();
	PanelBorder->SetBrush(bMain ? Box(FLinearColor::Transparent, FLinearColor::Transparent, 0.f, 0.f) : Box(SpnUi::Panel, SpnUi::PanelEdge, 1.f, 6.f));
	PanelBorder->SetPadding(bMain ? FMargin(0.f) : FMargin(26.f, 22.f));
	Width->SetContent(PanelBorder);

	UVerticalBox* Column = WidgetTree->ConstructWidget<UVerticalBox>();
	PanelBorder->SetContent(Column);

	TitleText = NewText(bMain ? 64 : 30, true, bMain ? SpnUi::Text : SpnUi::Text);
	Column->AddChildToVerticalBox(TitleText);
	SubtitleText = NewText(bMain ? 16 : 14, false, SpnUi::TextDim);
	SubtitleText->SetAutoWrapText(true);
	UVerticalBoxSlot* SubSlot = Column->AddChildToVerticalBox(SubtitleText);
	SubSlot->SetPadding(FMargin(0.f, 4.f, 0.f, bMain ? 34.f : 16.f));

	USizeBox* ListLimit = WidgetTree->ConstructWidget<USizeBox>();
	ListLimit->SetMaxDesiredHeight(bMain ? 520.f : 620.f);
	ListScroll = WidgetTree->ConstructWidget<UScrollBox>();
	ListScroll->SetScrollBarVisibility(ESlateVisibility::Collapsed);
	ListBox = WidgetTree->ConstructWidget<UVerticalBox>();
	ListScroll->AddChild(ListBox);
	ListLimit->SetContent(ListScroll);
	Column->AddChildToVerticalBox(ListLimit);

	MessageLine = NewText(14, true, SpnUi::TextDim);
	MessageLine->SetAutoWrapText(true);
	UVerticalBoxSlot* MsgSlot = Column->AddChildToVerticalBox(MessageLine);
	MsgSlot->SetPadding(FMargin(0.f, 14.f, 0.f, 0.f));

	FooterText = NewText(12, false, SpnUi::TextFaint);
	FooterText->SetAutoWrapText(true);
	UVerticalBoxSlot* FooterSlot = Column->AddChildToVerticalBox(FooterText);
	FooterSlot->SetPadding(FMargin(0.f, 10.f, 0.f, 0.f));
}

void USpnMenuScreen::SetTexts(const FText& InTitle, const FText& InSubtitle, const FText& InFooter)
{
	Title = InTitle;
	Subtitle = InSubtitle;
	Footer = InFooter;
	Sync();
}

void USpnMenuScreen::SetBackdropOpacity(float Opacity)
{
	BackdropOpacity = Opacity;
	if (Backdrop != nullptr)
	{
		Backdrop->SetBrush(Box(FLinearColor(0.f, 0.f, 0.f, BackdropOpacity), FLinearColor::Transparent, 0.f, 0.f));
	}
}

void USpnMenuScreen::SetMessage(const FText& Text, const FLinearColor& Color)
{
	MessageText = Text;
	MessageColor = Color;
	Sync();
}

void USpnMenuScreen::SetItems(const TArray<FSpnMenuItem>& InItems, FName SelectId)
{
	const FName Keep = SelectId.IsNone() ? GetSelectedId() : SelectId;
	Items = InItems;
	Selected = 0;
	const int32 Found = FindIndex(Keep);
	if (Found != INDEX_NONE)
	{
		Selected = Found;
	}
	else
	{
		for (int32 I = 0; I < Items.Num(); ++I)
		{
			if (!Items[I].bHeader && Items[I].bEnabled) { Selected = I; break; }
		}
	}
	Sync();
}

int32 USpnMenuScreen::FindIndex(FName Id) const
{
	if (Id.IsNone())
	{
		return INDEX_NONE;
	}
	return Items.IndexOfByPredicate([Id](const FSpnMenuItem& Item) { return Item.Id == Id; });
}

void USpnMenuScreen::Sync()
{
	if (ListBox == nullptr)
	{
		bDirty = true;
		return;
	}
	bDirty = false;
	TitleText->SetText(Title);
	SubtitleText->SetText(Subtitle);
	SubtitleText->SetVisibility(Subtitle.IsEmpty() ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
	MessageLine->SetText(MessageText);
	MessageLine->SetColorAndOpacity(FSlateColor(MessageColor));
	MessageLine->SetVisibility(MessageText.IsEmpty() ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
	FooterText->SetText(Footer);
	FooterText->SetVisibility(Footer.IsEmpty() ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);

	while (Pool.Num() < Items.Num())
	{
		USpnUiButton* Button = NewButton();
		Button->OnClicked.BindUObject(this, &USpnMenuScreen::OnButtonClicked);
		Button->OnHovered.BindUObject(this, &USpnMenuScreen::OnButtonHovered);
		Button->OnRightClicked.BindLambda([this](USpnUiButton* B) { Select(Pool.IndexOfByKey(B)); Activate(true); });
		ListBox->AddChildToVerticalBox(Button)->SetPadding(FMargin(0.f, 0.f, 0.f, 4.f));
		Pool.Add(Button);
	}
	Shown = Items.Num();
	for (int32 I = 0; I < Pool.Num(); ++I)
	{
		USpnUiButton* Button = Pool[I];
		if (I >= Items.Num())
		{
			Button->SetVisibility(ESlateVisibility::Collapsed);
			continue;
		}
		const FSpnMenuItem& Item = Items[I];
		Button->SetVisibility(ESlateVisibility::Visible);
		FSpnUiButtonContent Content;
		Content.Title = Item.bHeader ? FText::FromString(Item.Title.ToString().ToUpper()) : Item.Title;
		Content.Subtitle = Item.Subtitle;
		Content.SubtitleColor = Item.SubtitleColor;
		Content.Right = Item.bAdjustable ? FText::FromString(FString::Printf(TEXT("‹  %s  ›"), *Item.Value.ToString())) : Item.Value;
		Content.bShowIcon = false;
		Button->SetContent(Content);
		Button->Tag = I;
		Button->SetVisualState(I == Selected && !Item.bHeader, I == Selected && !Item.bHeader, Item.bEnabled || Item.bHeader);
	}
	if (Pool.IsValidIndex(Selected) && Selected < Shown)
	{
		ListScroll->ScrollWidgetIntoView(Pool[Selected], false);
	}
}

void USpnMenuScreen::Select(int32 Index)
{
	if (Items.IsValidIndex(Index) && !Items[Index].bHeader && Items[Index].bEnabled && Index != Selected)
	{
		Selected = Index;
		Sync();
	}
}

void USpnMenuScreen::Move(int32 Step)
{
	const int32 Count = Items.Num();
	if (Count == 0)
	{
		return;
	}
	int32 Index = Selected;
	for (int32 I = 0; I < Count; ++I)
	{
		Index = (Index + Step + Count) % Count;
		if (!Items[Index].bHeader && Items[Index].bEnabled)
		{
			break; // disabled entries are skipped by the keyboard and the controller (the mouse can still hover them and read why)
		}
		if (I == Count - 1)
		{
			Index = Selected; // nothing selectable: stay
		}
	}
	Selected = Index;
	Sync();
}

void USpnMenuScreen::Activate(bool bSecondary)
{
	if (Items.IsValidIndex(Selected) && !Items[Selected].bHeader && Items[Selected].bEnabled)
	{
		OnActivate.ExecuteIfBound(Items[Selected].Id, bSecondary);
	}
}

void USpnMenuScreen::OnButtonClicked(USpnUiButton* Button)
{
	const int32 Index = Pool.IndexOfByKey(Button);
	if (Items.IsValidIndex(Index) && !Items[Index].bHeader)
	{
		Selected = Index;
		Sync();
		Activate(false);
	}
}

void USpnMenuScreen::OnButtonHovered(USpnUiButton* Button)
{
	Select(Pool.IndexOfByKey(Button));
}

bool USpnMenuScreen::HandleKey(const FKey& Key, bool bRepeat)
{
	if (Key == EKeys::Up || Key == EKeys::W || Key == EKeys::Gamepad_DPad_Up)
	{
		Move(-1);
		return true;
	}
	if (Key == EKeys::Down || Key == EKeys::S || Key == EKeys::Gamepad_DPad_Down)
	{
		Move(1);
		return true;
	}
	if (Key == EKeys::Left || Key == EKeys::A || Key == EKeys::Gamepad_DPad_Left || Key == EKeys::Right || Key == EKeys::D || Key == EKeys::Gamepad_DPad_Right)
	{
		const int32 Delta = (Key == EKeys::Left || Key == EKeys::A || Key == EKeys::Gamepad_DPad_Left) ? -1 : 1;
		if (Items.IsValidIndex(Selected) && Items[Selected].bAdjustable && Items[Selected].bEnabled)
		{
			OnAdjust.ExecuteIfBound(Items[Selected].Id, Delta);
		}
		return true;
	}
	if (!bRepeat && (Key == EKeys::Enter || Key == EKeys::SpaceBar || Key == EKeys::Gamepad_FaceButton_Bottom))
	{
		Activate(false);
		return true;
	}
	if (!bRepeat && (Key == EKeys::Delete || Key == EKeys::Gamepad_FaceButton_Left))
	{
		Activate(true);
		return true;
	}
	if (!bRepeat && (Key == EKeys::Escape || Key == EKeys::BackSpace || Key == EKeys::Gamepad_FaceButton_Right))
	{
		OnBack.ExecuteIfBound();
		return true;
	}
	return false;
}

void USpnMenuScreen::HandleAnalog(const FKey& Key, float Value)
{
	if (Key == EKeys::Gamepad_LeftY) { StickDirection.Y = Value; }
	else if (Key == EKeys::Gamepad_LeftX) { StickDirection.X = Value; }
}

void USpnMenuScreen::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	Clock += InDeltaTime;
	if (bDirty)
	{
		Sync();
	}
	if (Clock >= NextStickRepeat)
	{
		if (StickDirection.Y > StickThreshold) { Move(-1); NextStickRepeat = Clock + StickRepeatSeconds; }
		else if (StickDirection.Y < -StickThreshold) { Move(1); NextStickRepeat = Clock + StickRepeatSeconds; }
		else if (FMath::Abs(StickDirection.X) > StickThreshold && Items.IsValidIndex(Selected) && Items[Selected].bAdjustable)
		{
			OnAdjust.ExecuteIfBound(Items[Selected].Id, StickDirection.X > 0 ? 1 : -1);
			NextStickRepeat = Clock + StickRepeatSeconds * 1.5;
		}
	}
}

FString USpnMenuScreen::GetItemTitle(int32 Index) const { return Items.IsValidIndex(Index) ? Items[Index].Title.ToString() : FString(); }
FString USpnMenuScreen::GetItemValue(int32 Index) const { return Items.IsValidIndex(Index) ? Items[Index].Value.ToString() : FString(); }
FString USpnMenuScreen::GetTitleString() const { return Title.ToString(); }
FString USpnMenuScreen::GetMessageString() const { return MessageText.ToString(); }
