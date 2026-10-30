#include "Spn/UI/SpnInventoryScreen.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Engine/GameInstance.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/ProgressBar.h"
#include "Components/ScaleBox.h"
#include "Components/ScrollBox.h"
#include "Components/SizeBox.h"
#include "Components/Spacer.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"
#include "HAL/PlatformTime.h"
#include "SpnInventorySubsystem.h"
#include "SpnSaveSubsystem.h"
#include "Spn/UI/SpnUiText.h"
#include "Styling/SlateBrush.h"
#include "Styling/SlateTypes.h"

namespace
{
	FSlateBrush Box(const FLinearColor& Fill, const FLinearColor& Outline = FLinearColor::Transparent, float OutlineWidth = 0.f, float Radius = 4.f)
	{
		return FSlateRoundedBoxBrush(Fill, Radius, Outline, OutlineWidth);
	}

	bool IsGamepadKey(const FKey& Key) { return Key.IsGamepadKey(); }

	FString FirstLetter(const FText& Name)
	{
		const FString S = Name.ToString().TrimStartAndEnd();
		return S.IsEmpty() ? FString(TEXT("?")) : S.Left(1).ToUpper();
	}
}

// === building ====================================================================================================================================

USpnUiButton* USpnInventoryScreen::NewButton()
{
	if (APlayerController* Owner = GetOwningPlayer())
	{
		return CreateWidget<USpnUiButton>(Owner, USpnUiButton::StaticClass());
	}
	if (UGameInstance* Instance = GetTypedOuter<UGameInstance>())
	{
		return CreateWidget<USpnUiButton>(Instance, USpnUiButton::StaticClass());
	}
	return CreateWidget<USpnUiButton>(GetWorld(), USpnUiButton::StaticClass());
}

UTextBlock* USpnInventoryScreen::NewText(int32 Size, bool bBold, const FLinearColor& Color)
{
	UTextBlock* Text = WidgetTree->ConstructWidget<UTextBlock>();
	Text->SetFont(SpnUi::Font(Size, bBold));
	Text->SetColorAndOpacity(FSlateColor(Color));
	return Text;
}

UBorder* USpnInventoryScreen::NewPanel(const FText& Title, UVerticalBox*& OutBody)
{
	UBorder* Panel = WidgetTree->ConstructWidget<UBorder>();
	Panel->SetBrush(Box(SpnUi::Panel, SpnUi::PanelEdge, 1.f, 5.f));
	Panel->SetPadding(FMargin(14.f, 12.f));
	UVerticalBox* Column = WidgetTree->ConstructWidget<UVerticalBox>();
	Panel->SetContent(Column);
	OutBody = Column;
	return Panel;
}

TSharedRef<SWidget> USpnInventoryScreen::RebuildWidget()
{
	if (WidgetTree == nullptr)
	{
		WidgetTree = NewObject<UWidgetTree>(this, TEXT("WidgetTree"), RF_Transient);
	}
	if (WidgetTree->RootWidget == nullptr)
	{
		BuildTree();
	}
	return Super::RebuildWidget();
}

void USpnInventoryScreen::BuildTree()
{
	UOverlay* Root = WidgetTree->ConstructWidget<UOverlay>();
	WidgetTree->RootWidget = Root;

	// dimmed backdrop over the whole screen (the game stays visible behind it)
	UBorder* Backdrop = WidgetTree->ConstructWidget<UBorder>();
	Backdrop->SetBrush(Box(SpnUi::Backdrop, FLinearColor::Transparent, 0.f, 0.f));
	UOverlaySlot* BackdropSlot = Root->AddChildToOverlay(Backdrop);
	BackdropSlot->SetHorizontalAlignment(HAlign_Fill);
	BackdropSlot->SetVerticalAlignment(VAlign_Fill);

	// the design is 1920x1080; the scale box fits it into any resolution and aspect ratio
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

	UBorder* Frame = WidgetTree->ConstructWidget<UBorder>();
	Frame->SetBrush(Box(SpnUi::Header, SpnUi::PanelEdge, 1.f, 6.f));
	Frame->SetPadding(FMargin(0.f));
	Design->AddChild(Frame);

	UVerticalBox* Main = WidgetTree->ConstructWidget<UVerticalBox>();
	Frame->SetContent(Main);

	// --- header ---
	UBorder* Header = WidgetTree->ConstructWidget<UBorder>();
	Header->SetBrush(Box(SpnUi::Header, FLinearColor::Transparent, 0.f, 0.f));
	Header->SetPadding(FMargin(22.f, 14.f, 22.f, 14.f));
	Main->AddChildToVerticalBox(Header);
	UHorizontalBox* HeaderRow = WidgetTree->ConstructWidget<UHorizontalBox>();
	Header->SetContent(HeaderRow);
	TitleText = NewText(24, true, SpnUi::Text);
	UHorizontalBoxSlot* TitleSlot = HeaderRow->AddChildToHorizontalBox(TitleText);
	TitleSlot->SetVerticalAlignment(VAlign_Center);
	TitleBadge = WidgetTree->ConstructWidget<UBorder>();
	TitleBadge->SetBrush(Box(SpnUi::Warning.CopyWithNewOpacity(0.14f), SpnUi::Warning.CopyWithNewOpacity(0.55f), 1.f, 3.f));
	TitleBadge->SetPadding(FMargin(8.f, 3.f));
	UTextBlock* BadgeText = NewText(11, true, SpnUi::Warning);
	DetailsTest = nullptr;
	BadgeText->SetText(FSpnUiText::Get(TEXT("ui.details.testdata")));
	TitleBadge->SetContent(BadgeText);
	UHorizontalBoxSlot* BadgeSlot = HeaderRow->AddChildToHorizontalBox(TitleBadge);
	BadgeSlot->SetVerticalAlignment(VAlign_Center);
	BadgeSlot->SetPadding(FMargin(16.f, 0.f, 0.f, 0.f));
	TabBox = WidgetTree->ConstructWidget<UHorizontalBox>();
	UHorizontalBoxSlot* TabSlot = HeaderRow->AddChildToHorizontalBox(TabBox);
	TabSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
	TabSlot->SetHorizontalAlignment(HAlign_Right);
	TabSlot->SetVerticalAlignment(VAlign_Center);
	CloseHintText = NewText(12, false, SpnUi::TextDim);
	UHorizontalBoxSlot* CloseSlot = HeaderRow->AddChildToHorizontalBox(CloseHintText);
	CloseSlot->SetVerticalAlignment(VAlign_Center);
	CloseSlot->SetPadding(FMargin(22.f, 0.f, 0.f, 0.f));

	// --- body ---
	UHorizontalBox* Body = WidgetTree->ConstructWidget<UHorizontalBox>();
	UVerticalBoxSlot* BodySlot = Main->AddChildToVerticalBox(Body);
	BodySlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
	BodySlot->SetPadding(FMargin(14.f, 0.f, 14.f, 10.f));

	// left column: equipment + capacity
	UVerticalBox* LeftColumn = WidgetTree->ConstructWidget<UVerticalBox>();
	USizeBox* LeftSize = WidgetTree->ConstructWidget<USizeBox>();
	LeftSize->SetWidthOverride(420.f);
	LeftSize->AddChild(LeftColumn);
	UHorizontalBoxSlot* LeftSlot = Body->AddChildToHorizontalBox(LeftSize);
	LeftSlot->SetPadding(FMargin(0.f, 0.f, 12.f, 0.f));

	UVerticalBox* EquipBody = nullptr;
	UBorder* EquipPanel = NewPanel(FText::GetEmpty(), EquipBody);
	EquipmentTitle = NewText(12, true, SpnUi::TextDim);
	EquipBody->AddChildToVerticalBox(EquipmentTitle)->SetPadding(FMargin(0.f, 0.f, 0.f, 8.f));
	SlotBox = WidgetTree->ConstructWidget<UVerticalBox>();
	EquipBody->AddChildToVerticalBox(SlotBox);
	LeftColumn->AddChildToVerticalBox(EquipPanel)->SetPadding(FMargin(0.f, 0.f, 0.f, 12.f));

	UVerticalBox* CapacityPanelBody = nullptr;
	UBorder* CapacityPanel = NewPanel(FText::GetEmpty(), CapacityPanelBody);
	CapacityTitle = NewText(12, true, SpnUi::TextDim);
	CapacityPanelBody->AddChildToVerticalBox(CapacityTitle)->SetPadding(FMargin(0.f, 0.f, 0.f, 8.f));
	CapacityBox = WidgetTree->ConstructWidget<UVerticalBox>();
	CapacityPanelBody->AddChildToVerticalBox(CapacityBox);
	for (int32 Index = 0; Index < 3; ++Index)
	{
		UHorizontalBox* Line = WidgetTree->ConstructWidget<UHorizontalBox>();
		UTextBlock* Label = NewText(13, false, SpnUi::TextDim);
		UTextBlock* Value = NewText(13, true, SpnUi::Text);
		Value->SetJustification(ETextJustify::Right);
		Line->AddChildToHorizontalBox(Label)->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
		Line->AddChildToHorizontalBox(Value);
		CapacityBox->AddChildToVerticalBox(Line)->SetPadding(FMargin(0.f, Index == 0 ? 0.f : 10.f, 0.f, 4.f));
		UProgressBar* Bar = WidgetTree->ConstructWidget<UProgressBar>();
		FProgressBarStyle Style;
		Style.SetBackgroundImage(Box(SpnUi::Row, SpnUi::PanelEdge, 1.f, 3.f));
		Style.SetFillImage(Box(FLinearColor::White, FLinearColor::Transparent, 0.f, 3.f));
		Style.SetMarqueeImage(Box(FLinearColor::White, FLinearColor::Transparent, 0.f, 3.f));
		Bar->SetWidgetStyle(Style);
		Bar->SetFillColorAndOpacity(SpnUi::Accent);
		USizeBox* BarSize = WidgetTree->ConstructWidget<USizeBox>();
		BarSize->SetHeightOverride(8.f);
		BarSize->AddChild(Bar);
		CapacityBox->AddChildToVerticalBox(BarSize);
		CapacityLabels.Add(Label);
		CapacityValues.Add(Value);
		CapacityBars.Add(Bar);
	}
	LeftColumn->AddChildToVerticalBox(CapacityPanel);

	// centre: items
	UVerticalBox* ItemsBody = nullptr;
	UBorder* ItemsPanel = NewPanel(FText::GetEmpty(), ItemsBody);
	UHorizontalBoxSlot* CenterSlot = Body->AddChildToHorizontalBox(ItemsPanel);
	CenterSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
	CenterSlot->SetPadding(FMargin(0.f, 0.f, 12.f, 0.f));
	UHorizontalBox* ItemsHeader = WidgetTree->ConstructWidget<UHorizontalBox>();
	ItemsTitle = NewText(12, true, SpnUi::TextDim);
	ItemsHeader->AddChildToHorizontalBox(ItemsTitle)->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
	ItemsWindowText = NewText(11, false, SpnUi::TextFaint);
	ItemsHeader->AddChildToHorizontalBox(ItemsWindowText);
	ItemsBody->AddChildToVerticalBox(ItemsHeader)->SetPadding(FMargin(0.f, 0.f, 0.f, 8.f));
	// view options: view (list/grid), sort, category filter; clickable, and V / O / F (Y, L3, R3 on a controller) from the keyboard
	UHorizontalBox* Toolbar = WidgetTree->ConstructWidget<UHorizontalBox>();
	for (int32 Index = 0; Index < 3; ++Index)
	{
		USpnUiButton* Button = NewButton();
		Button->OnClicked.BindLambda([this, Index](USpnUiButton*)
		{
			bUsingMouseMode = true;
			if (Index == 0) { ToggleView(); } else if (Index == 1) { CycleSort(); } else { CycleFilter(); }
		});
		(Index == 0 ? ViewButton : Index == 1 ? SortButton : FilterButton) = Button;
		UHorizontalBoxSlot* ToolSlot = Toolbar->AddChildToHorizontalBox(Button);
		ToolSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
		ToolSlot->SetPadding(FMargin(Index == 0 ? 0.f : 6.f, 0.f, 0.f, 0.f));
	}
	ItemsBody->AddChildToVerticalBox(Toolbar)->SetPadding(FMargin(0.f, 0.f, 0.f, 8.f));
	RowScroll = WidgetTree->ConstructWidget<UScrollBox>();
	RowScroll->SetScrollBarVisibility(ESlateVisibility::Visible);
	RowScroll->SetAlwaysShowScrollbar(false);
	RowScroll->SetConsumeMouseWheel(EConsumeMouseWheel::WhenScrollingPossible);
	RowBox = WidgetTree->ConstructWidget<UVerticalBox>();
	RowScroll->AddChild(RowBox);
	RowWrap = WidgetTree->ConstructWidget<UWrapBox>();
	RowWrap->SetInnerSlotPadding(FVector2D(6.f, 6.f));
	RowWrap->SetWrapSize(static_cast<float>(GridColumnCount) * 130.f);
	RowWrap->SetVisibility(ESlateVisibility::Collapsed);
	RowScroll->AddChild(RowWrap);
	ItemsBody->AddChildToVerticalBox(RowScroll)->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
	EmptyText = NewText(15, false, SpnUi::TextDim);
	EmptyText->SetJustification(ETextJustify::Center);
	EmptyText->SetAutoWrapText(true);
	ItemsBody->AddChildToVerticalBox(EmptyText)->SetPadding(FMargin(0.f, 24.f));

	// right column: details + actions
	UVerticalBox* RightColumn = WidgetTree->ConstructWidget<UVerticalBox>();
	USizeBox* RightSize = WidgetTree->ConstructWidget<USizeBox>();
	RightSize->SetWidthOverride(560.f);
	RightSize->AddChild(RightColumn);
	Body->AddChildToHorizontalBox(RightSize);

	UVerticalBox* DetailsBody = nullptr;
	UBorder* DetailsPanel = NewPanel(FText::GetEmpty(), DetailsBody);
	DetailsTitleLabel = NewText(12, true, SpnUi::TextDim);
	DetailsBody->AddChildToVerticalBox(DetailsTitleLabel)->SetPadding(FMargin(0.f, 0.f, 0.f, 8.f));
	DetailsName = NewText(24, true, SpnUi::Text);
	DetailsName->SetAutoWrapText(true);
	DetailsBody->AddChildToVerticalBox(DetailsName);
	DetailsDescription = NewText(13, false, SpnUi::TextDim);
	DetailsDescription->SetAutoWrapText(true);
	DetailsBody->AddChildToVerticalBox(DetailsDescription)->SetPadding(FMargin(0.f, 6.f, 0.f, 12.f));
	DetailsStats = WidgetTree->ConstructWidget<UVerticalBox>();
	DetailsBody->AddChildToVerticalBox(DetailsStats);
	RightColumn->AddChildToVerticalBox(DetailsPanel)->SetPadding(FMargin(0.f, 0.f, 0.f, 12.f));

	UVerticalBox* ActionsBody = nullptr;
	UBorder* ActionsPanel = NewPanel(FText::GetEmpty(), ActionsBody);
	ActionsTitle = NewText(12, true, SpnUi::TextDim);
	ActionsBody->AddChildToVerticalBox(ActionsTitle)->SetPadding(FMargin(0.f, 0.f, 0.f, 8.f));
	ActionBox = WidgetTree->ConstructWidget<UVerticalBox>();
	ActionsBody->AddChildToVerticalBox(ActionBox);
	RightColumn->AddChildToVerticalBox(ActionsPanel)->SetSize(FSlateChildSize(ESlateSizeRule::Fill));

	// --- footer ---
	UBorder* Footer = WidgetTree->ConstructWidget<UBorder>();
	Footer->SetBrush(Box(SpnUi::Header, FLinearColor::Transparent, 0.f, 0.f));
	Footer->SetPadding(FMargin(22.f, 10.f, 22.f, 12.f));
	Main->AddChildToVerticalBox(Footer);
	UVerticalBox* FooterColumn = WidgetTree->ConstructWidget<UVerticalBox>();
	Footer->SetContent(FooterColumn);
	MessageLine = NewText(14, true, SpnUi::Text);
	MessageLine->SetAutoWrapText(true);
	FooterColumn->AddChildToVerticalBox(MessageLine)->SetPadding(FMargin(0.f, 0.f, 0.f, 4.f));
	HintLine = NewText(12, false, SpnUi::TextFaint);
	HintLine->SetAutoWrapText(true);
	FooterColumn->AddChildToVerticalBox(HintLine);

	// --- confirmation dialog (modal) ---
	DialogLayer = WidgetTree->ConstructWidget<UBorder>();
	DialogLayer->SetBrush(Box(FLinearColor(0.f, 0.f, 0.f, 0.55f), FLinearColor::Transparent, 0.f, 0.f));
	DialogLayer->SetHorizontalAlignment(HAlign_Center);
	DialogLayer->SetVerticalAlignment(VAlign_Center);
	UOverlaySlot* DialogSlot = Root->AddChildToOverlay(DialogLayer);
	DialogSlot->SetHorizontalAlignment(HAlign_Fill);
	DialogSlot->SetVerticalAlignment(VAlign_Fill);
	USizeBox* DialogSize = WidgetTree->ConstructWidget<USizeBox>();
	DialogSize->SetWidthOverride(640.f);
	DialogLayer->SetContent(DialogSize);
	UBorder* DialogPanel = WidgetTree->ConstructWidget<UBorder>();
	DialogPanel->SetBrush(Box(SpnUi::Panel, SpnUi::Warning.CopyWithNewOpacity(0.7f), 1.5f, 6.f));
	DialogPanel->SetPadding(FMargin(26.f, 22.f));
	DialogSize->AddChild(DialogPanel);
	UVerticalBox* DialogColumn = WidgetTree->ConstructWidget<UVerticalBox>();
	DialogPanel->SetContent(DialogColumn);
	DialogTitle = NewText(20, true, SpnUi::Warning);
	DialogColumn->AddChildToVerticalBox(DialogTitle)->SetPadding(FMargin(0.f, 0.f, 0.f, 10.f));
	DialogBody = NewText(15, false, SpnUi::Text);
	DialogBody->SetAutoWrapText(true);
	DialogColumn->AddChildToVerticalBox(DialogBody)->SetPadding(FMargin(0.f, 0.f, 0.f, 18.f));
	UHorizontalBox* DialogButtons = WidgetTree->ConstructWidget<UHorizontalBox>();
	DialogColumn->AddChildToVerticalBox(DialogButtons);
	DialogCancel = NewButton();
	DialogConfirm = NewButton();
	DialogCancel->OnClicked.BindLambda([this](USpnUiButton*) { CloseDialog(); });
	DialogConfirm->OnClicked.BindLambda([this](USpnUiButton*) { ConfirmDialog(); });
	DialogButtons->AddChildToHorizontalBox(DialogCancel)->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
	UHorizontalBoxSlot* ConfirmSlot = DialogButtons->AddChildToHorizontalBox(DialogConfirm);
	ConfirmSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
	ConfirmSlot->SetPadding(FMargin(10.f, 0.f, 0.f, 0.f));
	DialogHint = NewText(11, false, SpnUi::TextFaint);
	DialogColumn->AddChildToVerticalBox(DialogHint)->SetPadding(FMargin(0.f, 12.f, 0.f, 0.f));
	DialogLayer->SetVisibility(ESlateVisibility::Collapsed);

	SetIsFocusable(true);
	SetVisibility(ESlateVisibility::Visible);
}

void USpnInventoryScreen::NativeConstruct()
{
	Super::NativeConstruct();
	SetIsFocusable(true);
}

void USpnInventoryScreen::NativeDestruct()
{
	Super::NativeDestruct();
}

void USpnInventoryScreen::ReleaseSlateResources(bool bReleaseChildren)
{
	HeldSlate.Reset();
	Super::ReleaseSlateResources(bReleaseChildren);
}

// === binding ======================================================================================================================================

void USpnInventoryScreen::Setup(USpnInventorySubsystem* InInventory, USpnSaveSubsystem* InSave, int64 InPrimaryContainerId)
{
	Teardown();
	Inventory = InInventory;
	Save = InSave;
	PrimaryContainerId = InPrimaryContainerId;
	ViewModel = NewObject<USpnInventoryViewModel>(this);
	ViewModel->Initialize(InInventory, InSave);
	ViewModel->SetGridColumns(bGrid ? GridColumnCount : 1);
	ViewModel->OnChangedNative.AddUObject(this, &USpnInventoryScreen::OnVmChanged);
	ViewModel->OnContainerChangedNative.AddUObject(this, &USpnInventoryScreen::OnVmContainerChanged);
	ViewModel->OnSelectionChangedNative.AddUObject(this, &USpnInventoryScreen::OnVmSelectionChanged);
	ViewModel->OnMessageNative.AddUObject(this, &USpnInventoryScreen::OnVmMessage);
	if (InInventory != nullptr)
	{
		InventoryChangedHandle = InInventory->OnInventoryChangedDetailed.AddUObject(this, &USpnInventoryScreen::OnInventoryChangedDetailed);
	}
	BindContainers();
}

void USpnInventoryScreen::Teardown()
{
	if (USpnInventorySubsystem* Subsystem = Inventory.Get())
	{
		Subsystem->OnInventoryChangedDetailed.Remove(InventoryChangedHandle);
	}
	InventoryChangedHandle.Reset();
	if (ViewModel != nullptr)
	{
		ViewModel->OnChangedNative.RemoveAll(this);
		ViewModel->OnContainerChangedNative.RemoveAll(this);
		ViewModel->OnSelectionChangedNative.RemoveAll(this);
		ViewModel->OnMessageNative.RemoveAll(this);
		ViewModel->Shutdown();
		ViewModel = nullptr;
	}
	Inventory.Reset();
	Save.Reset();
}

void USpnInventoryScreen::BindContainers()
{
	if (ViewModel == nullptr)
	{
		return;
	}
	TArray<int64> Ids;
	if (const USpnInventorySubsystem* Subsystem = Inventory.Get())
	{
		for (const uint64 Id : Subsystem->ListContainers())
		{
			Ids.Add(static_cast<int64>(Id));
		}
	}
	if (!IncludeContainers.IsEmpty() || !ExcludeContainers.IsEmpty())
	{
		Ids.RemoveAll([this](int64 Id) { return ExcludeContainers.Contains(Id) && !IncludeContainers.Contains(Id) || (!IncludeContainers.IsEmpty() && !IncludeContainers.Contains(Id)); });
	}
	Ids.Sort();
	if (PrimaryContainerId > 0 && Ids.Remove(PrimaryContainerId) > 0)
	{
		Ids.Insert(PrimaryContainerId, 0);
	}
	if (Ids != ViewModel->GetBoundContainers())
	{
		bRebindGuard = true;
		ViewModel->SetBoundContainers(Ids);
		bRebindGuard = false;
	}
}

void USpnInventoryScreen::OnShown()
{
	if (ViewModel == nullptr)
	{
		return;
	}
	const double Start = FPlatformTime::Seconds();
	HeldSlate = TakeWidget(); // builds the Slate tree on first use
	CloseDialog();
	BindContainers();
	ActiveContainer = FocusContainerId > 0 && ViewModel->FindPanel(FocusContainerId) != nullptr ? FocusContainerId : 0;
	Zone = EZone::Items;
	ViewModel->SetNavigationZone(ESpnNavigationZone::Items);
	WindowStart = 0;
	EnsureValidActiveContainer();
	if (ViewModel->GetSelectedStackId() == 0)
	{
		ViewModel->Navigate(ESpnNavigationDirection::First);
	}
	if (FocusContainerId > 0 && ViewModel->FindPanel(FocusContainerId) != nullptr)
	{
		SetActiveContainer(FocusContainerId, true); // a container view opens on the container, not on the player's own tab
	}
	SyncAll();
	LastRefreshMs = (FPlatformTime::Seconds() - Start) * 1000.0;
}

// === model -> widgets ============================================================================================================================

const FSpnContainerPanel* USpnInventoryScreen::ActivePanel() const
{
	return ViewModel != nullptr ? ViewModel->FindPanel(ActiveContainer) : nullptr;
}

const FSpnContainerPanel* USpnInventoryScreen::PrimaryPanel() const
{
	if (ViewModel == nullptr || ViewModel->GetBoundContainers().IsEmpty())
	{
		return nullptr;
	}
	return ViewModel->FindPanel(ViewModel->GetBoundContainers()[0]);
}

void USpnInventoryScreen::EnsureValidActiveContainer()
{
	if (ViewModel == nullptr)
	{
		ActiveContainer = 0;
		return;
	}
	if (ViewModel->FindPanel(ActiveContainer) != nullptr)
	{
		return;
	}
	ActiveContainer = 0;
	for (const int64 Id : ViewModel->GetBoundContainers())
	{
		if (ViewModel->FindPanel(Id) != nullptr)
		{
			ActiveContainer = Id;
			return;
		}
	}
}

int32 USpnInventoryScreen::SelectedRowIndexInActive() const
{
	const FSpnContainerPanel* Panel = ActivePanel();
	if (Panel == nullptr || ViewModel == nullptr || ViewModel->GetSelectedContainerId() != ActiveContainer)
	{
		return INDEX_NONE;
	}
	const int64 Selected = ViewModel->GetSelectedStackId();
	return Panel->Rows.IndexOfByPredicate([Selected](const FSpnItemRow& Row) { return Row.StackId == Selected; });
}

FText USpnInventoryScreen::ContainerName(const FSpnContainerPanel& Panel) const
{
	return FSpnUiText::Get(TEXT("kind.") + Panel.Kind, {}, Panel.Title);
}

FText USpnInventoryScreen::ItemName(const FSpnItemRow& Row) const
{
	if (Row.bHasPresentation && !Row.DisplayName.IsEmpty())
	{
		return Row.DisplayName; // a presentation asset wins over the built-in test texts
	}
	return FSpnUiText::Get(TEXT("item.") + Row.DefinitionId + TEXT(".name"), {}, FText::FromString(Row.DefinitionId));
}

FText USpnInventoryScreen::ItemDescription(const FSpnItemRow& Row) const
{
	if (Row.bHasPresentation && !Row.Description.IsEmpty())
	{
		return Row.Description;
	}
	const FString Key = TEXT("item.") + Row.DefinitionId + TEXT(".desc");
	return FSpnUiText::Has(Key) ? FSpnUiText::Get(Key) : FText::GetEmpty();
}

FText USpnInventoryScreen::CategoryName(const FString& CategoryId) const
{
	return FSpnUiText::Get(TEXT("cat.") + CategoryId, {}, FText::FromString(CategoryId));
}

FText USpnInventoryScreen::SlotName(const FString& SlotId) const
{
	return FSpnUiText::Get(TEXT("slot.") + SlotId, {}, FText::FromString(SlotId));
}

FText USpnInventoryScreen::ErrorText(ESpnInventoryError Error) const
{
	const FSpnInventoryMessage Message = SpnMakeInventoryErrorMessage(Error);
	return FSpnUiText::Get(Message.Key, {}, Message.Text);
}

void USpnInventoryScreen::SyncAll()
{
	const double Start = FPlatformTime::Seconds();
	EnsureValidActiveContainer();
	if (TitleText == nullptr)
	{
		return; // not built yet
	}
	TitleText->SetText(FSpnUiText::Get(TEXT("ui.inventory.title")));
	EquipmentTitle->SetText(FSpnUiText::Get(TEXT("ui.equipment.title")));
	ItemsTitle->SetText(FSpnUiText::Get(TEXT("ui.items.title")));
	DetailsTitleLabel->SetText(FSpnUiText::Get(TEXT("ui.details.title")));
	ActionsTitle->SetText(FSpnUiText::Get(TEXT("ui.actions.title")));
	CapacityTitle->SetText(FSpnUiText::Get(TEXT("ui.capacity.title")));
	CloseHintText->SetText(FText::FromString(FString(TEXT("Esc")) + TEXT("  ")));
	if (UTextBlock* BadgeLabel = Cast<UTextBlock>(TitleBadge->GetContent()))
	{
		BadgeLabel->SetText(FSpnUiText::Get(TEXT("ui.details.testdata")));
	}
	TitleBadge->SetVisibility(ViewModel != nullptr && ViewModel->GetPanels().Num() > 0 && Inventory.IsValid() && Inventory->GetCatalogData().bIsTestData ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	SyncTabs();
	SyncEquipment();
	SyncToolbar();
	++RowsFromAll;
	SyncRows();
	SyncCapacity();
	SyncDetails();
	SyncActions();
	SyncFooter();
	++FullRefreshes;
	LastRefreshMs = (FPlatformTime::Seconds() - Start) * 1000.0;
}

void USpnInventoryScreen::SyncTabs()
{
	VisibleTabs = 0;
	if (ViewModel == nullptr)
	{
		return;
	}
	for (const int64 Id : ViewModel->GetBoundContainers())
	{
		const FSpnContainerPanel* Panel = ViewModel->FindPanel(Id);
		if (Panel == nullptr)
		{
			continue;
		}
		if (!TabPool.IsValidIndex(VisibleTabs))
		{
			USpnUiButton* Button = NewButton();
			Button->OnClicked.BindLambda([this](USpnUiButton* Clicked) { SetActiveContainer(Clicked->Tag, true); });
			TabPool.Add(Button);
			UHorizontalBoxSlot* TabBoxSlot = TabBox->AddChildToHorizontalBox(Button);
			TabBoxSlot->SetPadding(FMargin(6.f, 0.f, 0.f, 0.f));
		}
		USpnUiButton* Tab = TabPool[VisibleTabs];
		FSpnUiButtonContent Content;
		Content.Title = ContainerName(*Panel);
		Content.Right = FText::AsNumber(Panel->TotalStackCount);
		Tab->Tag = Id;
		Tab->SetContent(Content);
		Tab->SetVisibility(ESlateVisibility::Visible);
		Tab->SetVisualState(Id == ActiveContainer, false, true);
		++VisibleTabs;
	}
	for (int32 Index = VisibleTabs; Index < TabPool.Num(); ++Index)
	{
		TabPool[Index]->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USpnInventoryScreen::SyncEquipment()
{
	VisibleSlots = 0;
	++EquipmentRefreshes;
	const FSpnContainerPanel* Primary = PrimaryPanel();
	if (Primary == nullptr || ViewModel == nullptr)
	{
		for (USpnUiButton* Button : SlotPool) { Button->SetVisibility(ESlateVisibility::Collapsed); }
		return;
	}
	for (const FSpnEquipmentSlotView& EquipSlot : Primary->Slots)
	{
		if (!SlotPool.IsValidIndex(VisibleSlots))
		{
			USpnUiButton* Button = NewButton();
			Button->OnClicked.BindLambda([this](USpnUiButton* Clicked) { ActivateSlot(Clicked); });
			Button->OnDoubleClicked.BindLambda([this](USpnUiButton* Clicked) { ActivateSlot(Clicked); if (ViewModel != nullptr) { ViewModel->ExecuteDefaultAction(); } });
			SlotPool.Add(Button);
			SlotBox->AddChildToVerticalBox(Button)->SetPadding(FMargin(0.f, 0.f, 0.f, 6.f));
		}
		USpnUiButton* Button = SlotPool[VisibleSlots];
		FSpnUiButtonContent Content;
		Content.Title = SlotName(EquipSlot.SlotId);
		Content.bShowIcon = true;
		if (EquipSlot.bOccupied)
		{
			Content.Subtitle = FSpnUiText::Get(TEXT("item.") + EquipSlot.EquippedDefinitionId + TEXT(".name"), {}, EquipSlot.EquippedName);
			Content.SubtitleColor = SpnUi::Text;
			Content.IconLetter = FirstLetter(Content.Subtitle);
			Content.IconColor = SpnUi::SwatchColor(EquipSlot.EquippedDefinitionId);
		}
		else
		{
			Content.Subtitle = FSpnUiText::Get(TEXT("ui.slot.free"));
			Content.SubtitleColor = SpnUi::TextFaint;
			Content.IconLetter = TEXT("·");
			Content.IconColor = SpnUi::Row;
		}
		Button->Tag = static_cast<int64>(VisibleSlots);
		Button->TagText = EquipSlot.SlotId;
		Button->SetContent(Content);
		Button->SetVisibility(ESlateVisibility::Visible);
		const bool bSelected = EquipSlot.bOccupied && ViewModel->GetSelectedStackId() == EquipSlot.EquippedStackId;
		const bool bFocus = Zone == EZone::Equipment && ViewModel->GetFocusedSlot() == EquipSlot.SlotId && !bUsingMouseMode;
		Button->SetVisualState(bSelected, bFocus, true);
		++VisibleSlots;
	}
	for (int32 Index = VisibleSlots; Index < SlotPool.Num(); ++Index)
	{
		SlotPool[Index]->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USpnInventoryScreen::ResetRowPool()
{
	RowBox->ClearChildren();
	RowWrap->ClearChildren();
	RowPool.Reset();
	RowBox->SetVisibility(bGrid ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	RowWrap->SetVisibility(bGrid ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void USpnInventoryScreen::ToggleView()
{
	bGrid = !bGrid;
	if (ViewModel != nullptr) { ViewModel->SetGridColumns(bGrid ? GridColumnCount : 1); }
	ResetRowPool();
	SyncRows();
	SyncToolbar();
	ShowMessage(FSpnUiText::Get(bGrid ? TEXT("ui.inv.view.grid.set") : TEXT("ui.inv.view.list.set")), ESpnMessageSeverity::Info);
}

void USpnInventoryScreen::CycleSort(bool bToggleDirection)
{
	if (ViewModel == nullptr) { return; }
	static const ESpnInventorySort Order[] = {ESpnInventorySort::Default, ESpnInventorySort::Name, ESpnInventorySort::Category, ESpnInventorySort::TotalWeight, ESpnInventorySort::TotalVolume, ESpnInventorySort::Quantity};
	int32 Index = 0;
	for (int32 I = 0; I < UE_ARRAY_COUNT(Order); ++I) { if (Order[I] == ViewModel->GetSortMode()) { Index = I; } }
	bool bDescending = ViewModel->IsSortDescending();
	if (bToggleDirection) { bDescending = !bDescending; }
	else { Index = (Index + 1) % UE_ARRAY_COUNT(Order); bDescending = false; }
	WindowStart = 0;
	ViewModel->SetSortMode(Order[Index], bDescending); // a view change only: the Core order and every stack id stay as they are
	SyncViewChange();
}

void USpnInventoryScreen::CycleFilter()
{
	if (ViewModel == nullptr) { return; }
	const FSpnContainerPanel* Panel = ActivePanel();
	TArray<FString> Categories = Panel != nullptr ? Panel->Categories : TArray<FString>();
	const FString Current = ViewModel->GetCategoryFilter();
	if (!Current.IsEmpty() && !Categories.Contains(Current)) { Categories.Add(Current); Categories.Sort(); }
	FString Next;
	if (Current.IsEmpty()) { Next = Categories.IsEmpty() ? FString() : Categories[0]; }
	else
	{
		const int32 At = Categories.IndexOfByKey(Current);
		Next = (At == INDEX_NONE || At + 1 >= Categories.Num()) ? FString() : Categories[At + 1];
	}
	WindowStart = 0;
	ViewModel->SetCategoryFilter(Next);
	SyncViewChange();
}

void USpnInventoryScreen::SyncViewChange()
{
	// the view-model reports a sort / filter change as one generic change; the rows, the details and the actions of the (possibly new) selection are redrawn here
	EnsureValidActiveContainer();
	SyncRows();
	SyncDetails();
	SyncActions();
	SyncToolbar();
}

FString USpnInventoryScreen::GetToolbarText(int32 Index) const
{
	USpnUiButton* Button = Index == 0 ? ViewButton.Get() : Index == 1 ? SortButton.Get() : FilterButton.Get();
	return Button != nullptr ? Button->GetTitleString() : FString();
}

void USpnInventoryScreen::SyncToolbar()
{
	if (ViewModel == nullptr || ViewButton == nullptr) { return; }
	auto Set = [](USpnUiButton* Button, const FText& Text, bool bActive)
	{
		FSpnUiButtonContent Content;
		Content.Title = Text;
		Button->SetContent(Content);
		Button->SetVisualState(bActive, false, true);
	};
	Set(ViewButton, FSpnUiText::Get(bGrid ? TEXT("ui.inv.view.grid") : TEXT("ui.inv.view.list")), bGrid);
	static const TCHAR* const SortKeys[] = {TEXT("ui.inv.sort.default"), TEXT("ui.inv.sort.name"), TEXT("ui.inv.sort.category"), TEXT("ui.inv.sort.weight"), TEXT("ui.inv.sort.volume"), TEXT("ui.inv.sort.quantity")};
	const int32 SortIndex = static_cast<int32>(ViewModel->GetSortMode()) < UE_ARRAY_COUNT(SortKeys) ? static_cast<int32>(ViewModel->GetSortMode()) : 0;
	const bool bSorted = ViewModel->GetSortMode() != ESpnInventorySort::Default || ViewModel->IsSortDescending();
	Set(SortButton, FText::Format(FSpnUiText::Get(TEXT("ui.inv.sort.label")), FSpnUiText::Get(SortKeys[SortIndex]), FText::FromString(ViewModel->IsSortDescending() ? TEXT("v") : TEXT("^"))), bSorted);
	const FString Filter = ViewModel->GetCategoryFilter();
	Set(FilterButton, FText::Format(FSpnUiText::Get(TEXT("ui.inv.filter.label")), Filter.IsEmpty() ? FSpnUiText::Get(TEXT("ui.inv.filter.all")) : CategoryName(Filter)), !Filter.IsEmpty());
}

void USpnInventoryScreen::SyncRows()
{
	const double Start = FPlatformTime::Seconds();
	const FSpnContainerPanel* Panel = ActivePanel();
	ShownRows.Reset();
	VisibleRows = 0;
	if (Panel == nullptr)
	{
		for (USpnUiButton* Button : RowPool) { Button->SetVisibility(ESlateVisibility::Collapsed); }
		EmptyText->SetText(FSpnUiText::Get(IsValidLowLevel() && Inventory.IsValid() && Inventory->HasCatalog() ? TEXT("ui.empty.nocontainer") : TEXT("ui.empty.inactive")));
		EmptyText->SetVisibility(ESlateVisibility::HitTestInvisible);
		ItemsWindowText->SetText(FText::GetEmpty());
		return;
	}
	const int32 Total = Panel->Rows.Num();
	// a window of at most MaxRowsShown rows around the selection (a body inventory never needs it; a depot with thousands of stacks does)
	const int32 Selected = SelectedRowIndexInActive();
	WindowStart = FMath::Clamp(WindowStart, 0, FMath::Max(0, Total - MaxRowsShown));
	if (Selected != INDEX_NONE)
	{
		if (Selected < WindowStart) { WindowStart = Selected; }
		else if (Selected >= WindowStart + MaxRowsShown) { WindowStart = Selected - MaxRowsShown + 1; }
	}
	const int32 End = FMath::Min(Total, WindowStart + MaxRowsShown);
	ItemsWindowText->SetText(Total > MaxRowsShown ? FSpnUiText::Get(TEXT("ui.list.window"), {FText::AsNumber(WindowStart + 1), FText::AsNumber(End), FText::AsNumber(Total)}) : FText::GetEmpty());
	for (int32 Index = WindowStart; Index < End; ++Index)
	{
		const FSpnItemRow& Row = Panel->Rows[Index];
		ShownRows.Add(Row);
		if (!RowPool.IsValidIndex(VisibleRows))
		{
			USpnUiButton* Button = NewButton();
			Button->OnClicked.BindLambda([this](USpnUiButton* Clicked) { ActivateRow(Clicked, false); });
			Button->OnDoubleClicked.BindLambda([this](USpnUiButton* Clicked) { ActivateRow(Clicked, true); });
			Button->OnRightClicked.BindLambda([this](USpnUiButton* Clicked) { ActivateRow(Clicked, false); SetZone(EZone::Actions); });
			RowPool.Add(Button);
			if (bGrid) { RowWrap->AddChildToWrapBox(Button); }
			else { RowBox->AddChildToVerticalBox(Button)->SetPadding(FMargin(0.f, 0.f, 0.f, 5.f)); }
		}
		USpnUiButton* Button = RowPool[VisibleRows];
		FSpnUiButtonContent Content;
		const FText Name = ItemName(Row);
		Content.Title = Name;
		FString Sub = CategoryName(Row.Category).ToString();
		if (Row.TotalWeightGrams > 0) { Sub += FString::Printf(TEXT("  -  %s"), *FSpnUiText::FormatWeight(Row.TotalWeightGrams).ToString()); }
		if (Row.TotalVolumeMilliliters > 0) { Sub += FString::Printf(TEXT("  -  %s"), *FSpnUiText::FormatVolume(Row.TotalVolumeMilliliters).ToString()); }
		for (const FSpnItemStateField& Field : Row.State)
		{
			const FString Value = Field.Type == ESpnStateValueType::Int ? FString::Printf(TEXT("%lld"), Field.IntValue) : (Field.Type == ESpnStateValueType::Bool ? (Field.BoolValue ? TEXT("✓") : TEXT("–")) : Field.StringValue);
			Sub += FString::Printf(TEXT("  ·  %s %s"), *Field.Name, *Value);
		}
		Content.Subtitle = FText::FromString(Sub);
		Content.Right = Row.bStackable || Row.Quantity > 1 ? FText::FromString(FString::Printf(TEXT("×%d"), Row.Quantity)) : FText::GetEmpty();
		Content.Badge = Row.bEquipped ? FSpnUiText::Get(TEXT("ui.badge.equipped")) : FText::GetEmpty();
		Content.bShowIcon = true;
		Content.bTile = bGrid;
		Content.IconLetter = FirstLetter(Name);
		Content.IconColor = SpnUi::SwatchColor(Row.Category);
		Button->Tag = Row.StackId;
		Button->SetContent(Content);
		Button->SetVisibility(ESlateVisibility::Visible);
		++VisibleRows;
	}
	for (int32 Index = VisibleRows; Index < RowPool.Num(); ++Index)
	{
		RowPool[Index]->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (VisibleRows == 0)
	{
		EmptyText->SetText(FSpnUiText::Get(TEXT("ui.empty.items")));
		EmptyText->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		EmptyText->SetVisibility(ESlateVisibility::Collapsed);
	}
	SyncSelectionVisuals();
	++RowsRebuilt;
	LastRowsBuildMs = (FPlatformTime::Seconds() - Start) * 1000.0;
}

void USpnInventoryScreen::SyncSelectionVisuals()
{
	if (ViewModel == nullptr)
	{
		return;
	}
	const int64 Selected = ViewModel->GetSelectedStackId();
	const bool bSelectionHere = ViewModel->GetSelectedContainerId() == ActiveContainer;
	for (int32 Index = 0; Index < VisibleRows; ++Index)
	{
		USpnUiButton* Button = RowPool[Index];
		const bool bSel = bSelectionHere && Selected != 0 && Button->Tag == Selected;
		Button->SetVisualState(bSel, bSel && Zone == EZone::Items && !bUsingMouseMode, true);
		if (bSel && !bUsingMouseMode)
		{
			RowScroll->ScrollWidgetIntoView(Button, false, EDescendantScrollDestination::IntoView, 24.f);
		}
	}
}

void USpnInventoryScreen::SyncDetails()
{
	++DetailsRefreshes;
	DetailsStats->ClearChildren();
	FSpnItemRow Row;
	if (ViewModel == nullptr || !ViewModel->GetSelectedRow(Row))
	{
		DetailsName->SetText(FSpnUiText::Get(TEXT("ui.details.none")));
		DetailsName->SetFont(SpnUi::Font(15, false));
		DetailsName->SetColorAndOpacity(FSlateColor(SpnUi::TextDim));
		DetailsDescription->SetText(FText::GetEmpty());
		return;
	}
	DetailsName->SetFont(SpnUi::Font(24, true));
	DetailsName->SetColorAndOpacity(FSlateColor(SpnUi::Text));
	DetailsName->SetText(ItemName(Row));
	DetailsDescription->SetText(ItemDescription(Row));
	const auto AddRow = [this](const FText& Label, const FText& Value, const FLinearColor& ValueColor = SpnUi::Text)
	{
		UHorizontalBox* Line = WidgetTree->ConstructWidget<UHorizontalBox>();
		UTextBlock* L = NewText(13, false, SpnUi::TextDim);
		L->SetText(Label);
		USizeBox* LabelSize = WidgetTree->ConstructWidget<USizeBox>();
		LabelSize->SetWidthOverride(150.f);
		LabelSize->AddChild(L);
		Line->AddChildToHorizontalBox(LabelSize);
		UTextBlock* V = NewText(13, true, ValueColor);
		V->SetText(Value);
		V->SetAutoWrapText(true);
		Line->AddChildToHorizontalBox(V)->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
		DetailsStats->AddChildToVerticalBox(Line)->SetPadding(FMargin(0.f, 0.f, 0.f, 6.f));
	};
	AddRow(FSpnUiText::Get(TEXT("ui.details.category")), CategoryName(Row.Category));
	AddRow(FSpnUiText::Get(TEXT("ui.details.stack")), Row.bStackable ? FSpnUiText::Get(TEXT("ui.details.stack.value"), {FText::AsNumber(Row.Quantity), FText::AsNumber(Row.MaxStack)}) : FSpnUiText::Get(TEXT("ui.details.stack.unique")));
	AddRow(FSpnUiText::Get(TEXT("ui.details.weight")), FSpnUiText::Get(TEXT("ui.details.each"), {FSpnUiText::FormatWeight(Row.UnitWeightGrams), FSpnUiText::FormatWeight(Row.TotalWeightGrams)}));
	AddRow(FSpnUiText::Get(TEXT("ui.details.volume")), FSpnUiText::Get(TEXT("ui.details.each"), {FSpnUiText::FormatVolume(Row.UnitVolumeMilliliters), FSpnUiText::FormatVolume(Row.TotalVolumeMilliliters)}));
	FSpnItemDefinitionData Definition;
	FString Fits;
	if (Inventory.IsValid() && Inventory->FindDefinition(Row.DefinitionId, Definition))
	{
		for (const FString& SlotId : Definition.EquipSlots)
		{
			Fits += (Fits.IsEmpty() ? TEXT("") : TEXT(", ")) + SlotName(SlotId).ToString();
		}
	}
	AddRow(FSpnUiText::Get(TEXT("ui.details.fits")), Fits.IsEmpty() ? FSpnUiText::Get(TEXT("ui.details.fits.none")) : FText::FromString(Fits), Fits.IsEmpty() ? SpnUi::TextDim : SpnUi::Text);
	if (Row.bEquipped)
	{
		AddRow(FText::GetEmpty(), FSpnUiText::Get(TEXT("ui.details.equipped"), {SlotName(Row.EquippedSlot)}), SpnUi::Accent);
	}
	for (const FSpnItemStateField& Field : Row.State)
	{
		const FString Value = Field.Type == ESpnStateValueType::Int ? FString::Printf(TEXT("%lld"), Field.IntValue) : (Field.Type == ESpnStateValueType::Bool ? (Field.BoolValue ? TEXT("✓") : TEXT("–")) : Field.StringValue);
		AddRow(FSpnUiText::Get(TEXT("ui.details.condition")), FText::FromString(FString::Printf(TEXT("%s: %s"), *Field.Name, *Value)));
	}
}

void USpnInventoryScreen::SyncActions()
{
	Actions = ViewModel != nullptr ? ViewModel->GetAvailableActions() : TArray<FSpnActionOption>();
	VisibleActions = 0;
	ActionIndex = Actions.IsEmpty() ? 0 : FMath::Clamp(ActionIndex, 0, Actions.Num() - 1);
	ResetQuantityForFocusedAction();
	for (int32 Index = 0; Index < Actions.Num(); ++Index)
	{
		const FSpnActionOption& Option = Actions[Index];
		if (!ActionPool.IsValidIndex(Index))
		{
			USpnUiButton* Button = NewButton();
			Button->OnClicked.BindLambda([this](USpnUiButton* Clicked) { ActivateAction(static_cast<int32>(Clicked->Tag)); });
			ActionPool.Add(Button);
			ActionBox->AddChildToVerticalBox(Button)->SetPadding(FMargin(0.f, 0.f, 0.f, 6.f));
		}
		USpnUiButton* Button = ActionPool[Index];
		FSpnUiButtonContent Content;
		Content.Title = ActionLabel(Option);
		if (!Option.bAvailable)
		{
			Content.Subtitle = FSpnUiText::Get(TEXT("ui.actions.unavailable"), {ErrorText(Option.UnavailableReason)});
			Content.SubtitleColor = SpnUi::Warning;
		}
		const bool bTakesQuantity = Option.MaxQuantity > 1 && (Option.Action == ESpnInventoryAction::Transfer || Option.Action == ESpnInventoryAction::Split || Option.Action == ESpnInventoryAction::Discard);
		if (bTakesQuantity)
		{
			const int32 Shown = Index == ActionIndex ? QuantityValue : Option.DefaultQuantity;
			Content.Right = FSpnUiText::Get(TEXT("ui.quantity.value"), {FText::AsNumber(Shown), FText::AsNumber(Option.MaxQuantity)});
		}
		Button->Tag = Index;
		Button->SetContent(Content);
		Button->SetVisibility(ESlateVisibility::Visible);
		Button->SetVisualState(Zone == EZone::Actions && Index == ActionIndex, Zone == EZone::Actions && Index == ActionIndex && !bUsingMouseMode, Option.bAvailable);
		++VisibleActions;
	}
	for (int32 Index = VisibleActions; Index < ActionPool.Num(); ++Index)
	{
		ActionPool[Index]->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (VisibleActions == 0 && ViewModel != nullptr)
	{
		// nothing selected: one dimmed explanatory line instead of an empty box
		if (!ActionPool.IsValidIndex(0))
		{
			USpnUiButton* Button = NewButton();
			ActionPool.Add(Button);
			ActionBox->AddChildToVerticalBox(Button)->SetPadding(FMargin(0.f, 0.f, 0.f, 6.f));
		}
		FSpnUiButtonContent Content;
		Content.Title = FSpnUiText::Get(TEXT("ui.actions.none"));
		ActionPool[0]->Tag = -1;
		ActionPool[0]->SetContent(Content);
		ActionPool[0]->SetVisibility(ESlateVisibility::HitTestInvisible);
		ActionPool[0]->SetVisualState(false, false, false);
	}
}

void USpnInventoryScreen::SyncCapacity()
{
	const FSpnContainerPanel* Panel = ActivePanel();
	const ESpnCapacityKind Kinds[3] = {ESpnCapacityKind::Weight, ESpnCapacityKind::Volume, ESpnCapacityKind::Stacks};
	const TCHAR* Keys[3] = {TEXT("ui.capacity.weight"), TEXT("ui.capacity.volume"), TEXT("ui.capacity.stacks")};
	for (int32 Index = 0; Index < 3; ++Index)
	{
		CapacityLabels[Index]->SetText(FSpnUiText::Get(Keys[Index]));
		if (Panel == nullptr)
		{
			CapacityValues[Index]->SetText(FText::GetEmpty());
			CapacityBars[Index]->SetPercent(0.f);
			continue;
		}
		const FSpnCapacityMeter& Meter = Kinds[Index] == ESpnCapacityKind::Weight ? Panel->WeightMeter : (Kinds[Index] == ESpnCapacityKind::Volume ? Panel->VolumeMeter : Panel->StackMeter);
		CapacityValues[Index]->SetText(FSpnUiText::FormatMeter(Meter.Used, Meter.Max, Kinds[Index] == ESpnCapacityKind::Weight, Kinds[Index] == ESpnCapacityKind::Volume));
		CapacityBars[Index]->SetPercent(Meter.bUnlimited ? 0.f : FMath::Clamp(Meter.Fraction, 0.f, 1.f));
		const FLinearColor Color = Meter.bUnlimited ? SpnUi::TextFaint : (Meter.Fraction >= 0.95f ? SpnUi::Error : (Meter.Fraction >= 0.75f ? SpnUi::Warning : SpnUi::Accent));
		CapacityBars[Index]->SetFillColorAndOpacity(Color);
	}
}

void USpnInventoryScreen::SyncFooter()
{
	HintLine->SetText(FSpnUiText::Get(bDialogOpen ? TEXT("ui.hint.dialog") : (bUsingGamepad ? TEXT("ui.hint.gamepad") : TEXT("ui.hint.keyboard"))));
	if (Clock >= MessageUntil)
	{
		MessageLine->SetText(FText::GetEmpty());
	}
}

void USpnInventoryScreen::ShowMessage(const FText& Text, ESpnMessageSeverity Severity)
{
	MessageLine->SetText(Text);
	MessageLine->SetColorAndOpacity(FSlateColor(Severity == ESpnMessageSeverity::Error ? SpnUi::Error : (Severity == ESpnMessageSeverity::Warning ? SpnUi::Warning : SpnUi::Good)));
	MessageUntil = Clock + 6.0;
}

void USpnInventoryScreen::ShowSpnMessage(const FSpnInventoryMessage& Message)
{
	ShowMessage(FSpnUiText::Get(Message.Key, {}, Message.Text), Message.Severity);
}

void USpnInventoryScreen::FlushDirty()
{
	if (TitleText == nullptr)
	{
		bDirtyAll = bDirtyRows = bDirtyEquipment = bDirtyTabs = false;
		return;
	}
	if (bDirtyAll)
	{
		bDirtyAll = bDirtyRows = bDirtyEquipment = bDirtyTabs = false;
		SyncAll();
		return;
	}
	EnsureValidActiveContainer();
	if (bDirtyTabs) { SyncTabs(); }
	if (bDirtyEquipment) { SyncEquipment(); }
	if (bDirtyRows) { ++RowsFromFlush; SyncRows(); SyncCapacity(); }
	bDirtyTabs = bDirtyEquipment = bDirtyRows = false;
	SyncDetails();
	SyncActions();
}

// === events from the view-model and the subsystem ===================================================================================================

void USpnInventoryScreen::OnInventoryChangedDetailed(const FSpnInventoryChange& Change)
{
	// the set of containers can change without any bound container being touched (a new container, a destroyed one)
	if (!bRebindGuard && ViewModel != nullptr)
	{
		BindContainers();
	}
	if (Change.bReplacedAll)
	{
		bDirtyAll = true;
		LastFullReason = TEXT("replaced");
	}
}

void USpnInventoryScreen::OnVmContainerChanged(int64 ContainerId)
{
	bDirtyTabs = true;
	if (ContainerId == ActiveContainer) { bDirtyRows = true; }
	if (PrimaryPanel() != nullptr && ContainerId == PrimaryPanel()->ContainerId) { bDirtyEquipment = true; }
}

void USpnInventoryScreen::OnVmChanged()
{
	if (bRebindGuard)
	{
		bDirtyAll = true;
		LastFullReason = TEXT("rebind");
	}
	FlushDirty();
}

void USpnInventoryScreen::OnVmSelectionChanged()
{
	const double Start = FPlatformTime::Seconds();
	if (ViewModel != nullptr && Zone != EZone::Equipment && ViewModel->GetSelectedStackId() != 0 && ViewModel->GetSelectedContainerId() != ActiveContainer)
	{
		ActiveContainer = ViewModel->GetSelectedContainerId(); // the selection moved to another container (section change, followed stack)
		bDirtyAll = true;
		LastFullReason = TEXT("selection moved container");
	}
	if (bDirtyAll)
	{
		FlushDirty(); // a selection-only notification is not always followed by OnChanged: never leave the flag armed
		return;
	}
	if (TitleText == nullptr)
	{
		return;
	}
	// a long list is windowed: when the selection leaves the window, the window moves (the only case where a selection change rebuilds rows)
	const FSpnContainerPanel* Panel = ActivePanel();
	const int32 SelectedIndex = SelectedRowIndexInActive();
	if (Panel != nullptr && Panel->Rows.Num() > MaxRowsShown && SelectedIndex != INDEX_NONE && (SelectedIndex < WindowStart || SelectedIndex >= WindowStart + MaxRowsShown))
	{
		SyncRows(); // also refreshes the selection visuals
	}
	else
	{
		SyncSelectionVisuals();
	}
	SyncEquipment();
	SyncDetails();
	SyncActions();
	LastSelectionMs = (FPlatformTime::Seconds() - Start) * 1000.0;
}

void USpnInventoryScreen::OnVmMessage(const FSpnInventoryMessage& Message)
{
	if (TitleText != nullptr)
	{
		ShowSpnMessage(Message);
	}
}

// === user intents ======================================================================================================================================

void USpnInventoryScreen::SetUsingGamepad(bool bValue)
{
	if (bUsingGamepad != bValue)
	{
		bUsingGamepad = bValue;
		if (TitleText != nullptr) { SyncFooter(); }
	}
}

void USpnInventoryScreen::SetZone(EZone NewZone)
{
	if (ViewModel == nullptr)
	{
		return;
	}
	if (NewZone == EZone::Actions && ViewModel->GetSelectedStackId() == 0)
	{
		return; // nothing selected, no actions
	}
	Zone = NewZone;
	ViewModel->SetNavigationZone(NewZone == EZone::Equipment ? ESpnNavigationZone::Equipment : ESpnNavigationZone::Items);
	if (NewZone == EZone::Items && ViewModel->GetSelectedStackId() == 0)
	{
		ViewModel->Navigate(ESpnNavigationDirection::First);
	}
	if (TitleText != nullptr)
	{
		SyncSelectionVisuals();
		SyncEquipment();
		SyncActions();
	}
}

void USpnInventoryScreen::SetActiveContainer(int64 ContainerId, bool bSelectFirst)
{
	if (ViewModel == nullptr || ViewModel->FindPanel(ContainerId) == nullptr)
	{
		return;
	}
	const double Start = FPlatformTime::Seconds();
	ActiveContainer = ContainerId;
	WindowStart = 0;
	Zone = EZone::Items;
	ViewModel->SetNavigationZone(ESpnNavigationZone::Items);
	const FSpnContainerPanel* Panel = ActivePanel();
	if (bSelectFirst)
	{
		if (Panel != nullptr && !Panel->Rows.IsEmpty())
		{
			ViewModel->SelectStack(ContainerId, Panel->Rows[0].StackId);
		}
		else
		{
			ViewModel->ClearSelection();
		}
	}
	if (TitleText != nullptr)
	{
		SyncTabs();
		++RowsFromSwitch;
		SyncRows();
		SyncCapacity();
		SyncDetails();
		SyncActions();
		SyncEquipment();
	}
	LastSelectionMs = (FPlatformTime::Seconds() - Start) * 1000.0;
}

void USpnInventoryScreen::ChangeContainer(int32 Step)
{
	if (ViewModel == nullptr)
	{
		return;
	}
	TArray<int64> Existing;
	for (const int64 Id : ViewModel->GetBoundContainers())
	{
		if (ViewModel->FindPanel(Id) != nullptr) { Existing.Add(Id); }
	}
	if (Existing.Num() < 2)
	{
		return;
	}
	const int32 Current = Existing.IndexOfByKey(ActiveContainer);
	const int32 Next = ((Current < 0 ? 0 : Current) + Step + Existing.Num()) % Existing.Num();
	SetActiveContainer(Existing[Next], true);
}

void USpnInventoryScreen::ActivateRow(USpnUiButton* Button, bool bUse)
{
	if (ViewModel == nullptr || Button == nullptr)
	{
		return;
	}
	bUsingMouseMode = true;
	Zone = EZone::Items;
	ViewModel->SetNavigationZone(ESpnNavigationZone::Items);
	ViewModel->SelectStack(ActiveContainer, Button->Tag);
	if (bUse)
	{
		ViewModel->ExecuteDefaultAction();
	}
	if (TitleText != nullptr) { SyncSelectionVisuals(); SyncActions(); }
}

void USpnInventoryScreen::ActivateSlot(USpnUiButton* Button)
{
	const FSpnContainerPanel* Primary = PrimaryPanel();
	if (ViewModel == nullptr || Primary == nullptr || Button == nullptr)
	{
		return;
	}
	bUsingMouseMode = true;
	Zone = EZone::Equipment;
	ViewModel->SetNavigationZone(ESpnNavigationZone::Equipment);
	for (const FSpnEquipmentSlotView& EquipSlot : Primary->Slots)
	{
		if (EquipSlot.SlotId == Button->TagText)
		{
			if (EquipSlot.bOccupied)
			{
				ViewModel->SelectStack(Primary->ContainerId, EquipSlot.EquippedStackId);
			}
			else
			{
				ViewModel->ClearSelection();
			}
		}
	}
	SyncEquipment();
}

FText USpnInventoryScreen::ActionLabel(const FSpnActionOption& Option) const
{
	switch (Option.Action)
	{
	case ESpnInventoryAction::Equip: return FSpnUiText::Get(TEXT("ui.action.equip"), {SlotName(Option.Slot)});
	case ESpnInventoryAction::Unequip: return FSpnUiText::Get(TEXT("ui.action.unequip"), {SlotName(Option.Slot)});
	case ESpnInventoryAction::Transfer:
	{
		const FSpnContainerPanel* Target = ViewModel != nullptr ? ViewModel->FindPanel(Option.TargetContainerId) : nullptr;
		return FSpnUiText::Get(TEXT("ui.action.transfer"), {Target != nullptr ? ContainerName(*Target) : FText::AsNumber(Option.TargetContainerId)});
	}
	case ESpnInventoryAction::Split: return FSpnUiText::Get(TEXT("ui.action.split"), {FText::AsNumber(Option.DefaultQuantity)});
	case ESpnInventoryAction::Merge: return FSpnUiText::Get(TEXT("ui.action.merge"));
	case ESpnInventoryAction::Discard: return FSpnUiText::Get(TEXT("ui.action.discard"));
	default: return FText::GetEmpty();
	}
}

void USpnInventoryScreen::ResetQuantityForFocusedAction()
{
	QuantityValue = Actions.IsValidIndex(ActionIndex) ? FMath::Max(1, Actions[ActionIndex].DefaultQuantity) : 1;
}

void USpnInventoryScreen::AdjustQuantity(int32 Delta)
{
	if (!Actions.IsValidIndex(ActionIndex))
	{
		return;
	}
	const FSpnActionOption& Option = Actions[ActionIndex];
	if (Option.MaxQuantity <= 1 || !(Option.Action == ESpnInventoryAction::Transfer || Option.Action == ESpnInventoryAction::Split || Option.Action == ESpnInventoryAction::Discard))
	{
		return;
	}
	QuantityValue = FMath::Clamp(QuantityValue + Delta, 1, Option.MaxQuantity);
	SyncActions();
}

void USpnInventoryScreen::MoveActionFocus(int32 Delta)
{
	if (Actions.IsEmpty())
	{
		return;
	}
	ActionIndex = FMath::Clamp(ActionIndex + Delta, 0, Actions.Num() - 1);
	ResetQuantityForFocusedAction();
	SyncActions();
}

void USpnInventoryScreen::ActivateAction(int32 Index)
{
	if (!Actions.IsValidIndex(Index) || ViewModel == nullptr)
	{
		return;
	}
	if (Index != ActionIndex)
	{
		ActionIndex = Index;
		ResetQuantityForFocusedAction();
	}
	RunAction(Actions[Index], QuantityValue);
}

void USpnInventoryScreen::RunAction(const FSpnActionOption& Option, int32 Quantity)
{
	if (!Option.bAvailable)
	{
		// the Core said no: say why, change nothing
		ShowMessage(FSpnUiText::Get(TEXT("ui.actions.unavailable"), {ErrorText(Option.UnavailableReason)}), ESpnMessageSeverity::Warning);
		return;
	}
	if (Option.Action == ESpnInventoryAction::Discard)
	{
		OpenDiscardDialog(Option, Quantity);
		return;
	}
	FSpnItemRow Row;
	if (!ViewModel->GetSelectedRow(Row))
	{
		return;
	}
	FSpnActionRequest Request;
	Request.Action = Option.Action;
	Request.ContainerId = Row.ContainerId;
	Request.StackId = Row.StackId;
	Request.TargetContainerId = Option.TargetContainerId;
	Request.TargetStackId = Option.TargetStackId;
	Request.Slot = Option.Slot;
	Request.Quantity = Option.Action == ESpnInventoryAction::Merge ? 0 : Quantity;
	ViewModel->ExecuteAction(Request);
	if (Zone == EZone::Actions && ViewModel->GetSelectedStackId() == 0)
	{
		SetZone(EZone::Items);
	}
}

void USpnInventoryScreen::OpenDiscardDialog(const FSpnActionOption& Option, int32 Quantity)
{
	FSpnItemRow Row;
	if (ViewModel == nullptr || !ViewModel->GetSelectedRow(Row))
	{
		return;
	}
	PendingOption = Option;
	PendingQuantity = FMath::Clamp(Quantity, 1, FMath::Max(1, Row.Quantity));
	bDialogOpen = true;
	bDialogConfirmFocused = false; // a destructive confirmation starts on Cancel
	DialogTitle->SetText(FSpnUiText::Get(TEXT("ui.dialog.discard.title")));
	DialogBody->SetText(FSpnUiText::Get(TEXT("ui.dialog.discard.body"), {FText::AsNumber(PendingQuantity), ItemName(Row)}));
	FSpnUiButtonContent Cancel;
	Cancel.Title = FSpnUiText::Get(TEXT("ui.dialog.cancel"));
	DialogCancel->SetContent(Cancel);
	FSpnUiButtonContent Confirm;
	Confirm.Title = FSpnUiText::Get(TEXT("ui.dialog.confirm"));
	DialogConfirm->SetContent(Confirm);
	DialogCancel->SetVisualState(true, !bUsingMouseMode, true);
	DialogConfirm->SetVisualState(false, false, true);
	DialogHint->SetText(FSpnUiText::Get(TEXT("ui.hint.dialog")));
	DialogLayer->SetVisibility(ESlateVisibility::Visible);
	SyncFooter();
}

void USpnInventoryScreen::CloseDialog()
{
	bDialogOpen = false;
	if (DialogLayer != nullptr)
	{
		DialogLayer->SetVisibility(ESlateVisibility::Collapsed);
		SyncFooter();
	}
}

void USpnInventoryScreen::ConfirmDialog()
{
	if (!bDialogOpen || ViewModel == nullptr)
	{
		return;
	}
	FSpnItemRow Row;
	const bool bHave = ViewModel->GetSelectedRow(Row);
	CloseDialog();
	if (!bHave)
	{
		return;
	}
	FSpnActionRequest Request;
	Request.Action = ESpnInventoryAction::Discard;
	Request.ContainerId = Row.ContainerId;
	Request.StackId = Row.StackId;
	Request.Quantity = PendingQuantity;
	ViewModel->ExecuteAction(Request);
}

void USpnInventoryScreen::GoBack()
{
	if (bDialogOpen)
	{
		CloseDialog();
		return;
	}
	if (Zone != EZone::Items)
	{
		SetZone(EZone::Items);
		return;
	}
	OnCloseRequested.ExecuteIfBound();
}

// === keys ===================================================================================================================================================

bool USpnInventoryScreen::HandleKey(const FKey& Key, bool bShift, bool bRepeat)
{
	if (ViewModel == nullptr)
	{
		return false;
	}
	if (Key.IsGamepadKey()) { bUsingMouseMode = false; SetUsingGamepad(true); }
	else if (!Key.IsMouseButton()) { bUsingMouseMode = false; SetUsingGamepad(false); }
	const double Start = FPlatformTime::Seconds();

	const bool bUp = Key == EKeys::Up || Key == EKeys::Gamepad_DPad_Up;
	const bool bDown = Key == EKeys::Down || Key == EKeys::Gamepad_DPad_Down;
	const bool bLeft = Key == EKeys::Left || Key == EKeys::Gamepad_DPad_Left;
	const bool bRight = Key == EKeys::Right || Key == EKeys::Gamepad_DPad_Right;
	const bool bConfirm = Key == EKeys::Enter || Key == EKeys::SpaceBar || Key == EKeys::Virtual_Accept || Key == EKeys::Gamepad_FaceButton_Bottom;
	const bool bBack = Key == EKeys::Escape || Key == EKeys::BackSpace || Key == EKeys::Gamepad_FaceButton_Right;

	if (bDialogOpen)
	{
		if (bLeft || bRight || bUp || bDown || Key == EKeys::Tab)
		{
			bDialogConfirmFocused = !bDialogConfirmFocused;
			DialogCancel->SetVisualState(!bDialogConfirmFocused, !bDialogConfirmFocused && !bUsingMouseMode, true);
			DialogConfirm->SetVisualState(bDialogConfirmFocused, bDialogConfirmFocused && !bUsingMouseMode, true);
			return true;
		}
		if (bConfirm)
		{
			if (bDialogConfirmFocused) { ConfirmDialog(); } else { CloseDialog(); }
			return true;
		}
		if (bBack) { CloseDialog(); return true; }
		return true; // modal: swallow everything else
	}

	bool bHandled = true;
	if (Key == EKeys::I || Key == EKeys::Gamepad_Special_Right || Key == EKeys::Gamepad_Special_Left)
	{
		OnCloseRequested.ExecuteIfBound();
	}
	else if (bBack)
	{
		GoBack();
	}
	else if (bUp || bDown)
	{
		const int32 Direction = bDown ? 1 : -1;
		if (Zone == EZone::Actions) { MoveActionFocus(Direction); }
		else { ViewModel->Navigate(bDown ? ESpnNavigationDirection::Down : ESpnNavigationDirection::Up); }
	}
	else if (bLeft || bRight)
	{
		const bool bForward = bRight;
		if (Zone == EZone::Items && bGrid && ViewModel->Navigate(bForward ? ESpnNavigationDirection::Right : ESpnNavigationDirection::Left))
		{
			// moved inside the grid row; at the edge of the row the arrows switch the panel as in the list
		}
		else if (Zone == EZone::Items)
		{
			if (bForward) { SetZone(EZone::Actions); }
			else if (PrimaryPanel() != nullptr && !PrimaryPanel()->Slots.IsEmpty()) { SetZone(EZone::Equipment); }
		}
		else if (Zone == EZone::Equipment)
		{
			if (bForward) { SetZone(EZone::Items); }
		}
		else
		{
			if (!bForward) { SetZone(EZone::Items); }
		}
	}
	else if (Key == EKeys::Tab || Key == EKeys::Gamepad_LeftShoulder || Key == EKeys::Gamepad_RightShoulder)
	{
		ChangeContainer(Key == EKeys::Gamepad_LeftShoulder || (Key == EKeys::Tab && bShift) ? -1 : 1);
	}
	else if (bConfirm)
	{
		if (bRepeat) { return true; } // holding Enter must not run an action repeatedly
		if (Zone == EZone::Actions) { ActivateAction(ActionIndex); }
		else if (Zone == EZone::Equipment)
		{
			const FSpnContainerPanel* Primary = PrimaryPanel();
			if (Primary != nullptr)
			{
				for (const FSpnEquipmentSlotView& EquipSlot : Primary->Slots)
				{
					if (EquipSlot.SlotId == ViewModel->GetFocusedSlot())
					{
						if (EquipSlot.bOccupied)
						{
							FSpnActionRequest Request;
							Request.Action = ESpnInventoryAction::Unequip;
							Request.ContainerId = Primary->ContainerId;
							Request.Slot = EquipSlot.SlotId;
							ViewModel->ExecuteAction(Request);
						}
						else
						{
							ShowMessage(FSpnUiText::Get(TEXT("spn.inventory.error.slot_empty")), ESpnMessageSeverity::Info);
						}
					}
				}
			}
		}
		else
		{
			ViewModel->ExecuteDefaultAction();
		}
	}
	else if (Key == EKeys::V || Key == EKeys::Gamepad_LeftThumbstick)
	{
		if (!bRepeat) { ToggleView(); }
	}
	else if (Key == EKeys::O || Key == EKeys::Gamepad_FaceButton_Top)
	{
		if (!bRepeat) { CycleSort(bShift); }
	}
	else if (Key == EKeys::F || Key == EKeys::Gamepad_RightThumbstick)
	{
		if (!bRepeat) { CycleFilter(); }
	}
	else if (Key == EKeys::Delete || Key == EKeys::Gamepad_FaceButton_Left)
	{
		if (bRepeat) { return true; }
		for (const FSpnActionOption& Option : ViewModel->GetAvailableActions())
		{
			if (Option.Action == ESpnInventoryAction::Discard)
			{
				RunAction(Option, Option.DefaultQuantity);
				break;
			}
		}
	}
	else if (Key == EKeys::PageUp) { ViewModel->Navigate(ESpnNavigationDirection::PageUp); }
	else if (Key == EKeys::PageDown) { ViewModel->Navigate(ESpnNavigationDirection::PageDown); }
	else if (Key == EKeys::Home) { ViewModel->Navigate(ESpnNavigationDirection::First); }
	else if (Key == EKeys::End) { ViewModel->Navigate(ESpnNavigationDirection::Last); }
	else if (Key == EKeys::Add || Key == EKeys::Equals || Key == EKeys::Gamepad_RightTrigger) { AdjustQuantity(+1); }
	else if (Key == EKeys::Subtract || Key == EKeys::Hyphen || Key == EKeys::Gamepad_LeftTrigger) { AdjustQuantity(-1); }
	else
	{
		bHandled = false;
	}
	LastSelectionMs = FMath::Max(LastSelectionMs, 0.0);
	(void)Start;
	return bHandled;
}

void USpnInventoryScreen::StepStick(const FVector2D& Direction)
{
	if (FMath::Abs(Direction.Y) >= FMath::Abs(Direction.X))
	{
		HandleKey(Direction.Y > 0.f ? EKeys::Gamepad_DPad_Up : EKeys::Gamepad_DPad_Down, false, true);
	}
	else
	{
		HandleKey(Direction.X > 0.f ? EKeys::Gamepad_DPad_Right : EKeys::Gamepad_DPad_Left, false, true);
	}
}

void USpnInventoryScreen::HandleAnalog(const FKey& Key, float Value)
{
	SetUsingGamepad(true);
	if (Key == EKeys::Gamepad_LeftX) { StickDirection.X = FMath::Abs(Value) > 0.55f ? Value : 0.f; }
	else if (Key == EKeys::Gamepad_LeftY) { StickDirection.Y = FMath::Abs(Value) > 0.55f ? Value : 0.f; }
	else if (Key == EKeys::Gamepad_RightY) { ScrollStick = FMath::Abs(Value) > 0.2f ? Value : 0.f; }
	if (StickDirection.IsNearlyZero())
	{
		NextStickRepeat = 0.0;
	}
	else if (NextStickRepeat == 0.0)
	{
		bUsingMouseMode = false;
		NextStickRepeat = Clock + 0.40; // first step right away, then the repeat
		StepStick(StickDirection);
	}
}

void USpnInventoryScreen::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	Clock += InDeltaTime;
	if (MessageUntil > 0.0 && Clock >= MessageUntil && MessageLine != nullptr)
	{
		MessageLine->SetText(FText::GetEmpty());
		MessageUntil = 0.0;
	}
	if (!StickDirection.IsNearlyZero() && NextStickRepeat > 0.0 && Clock >= NextStickRepeat)
	{
		NextStickRepeat = Clock + 0.13;
		StepStick(StickDirection);
	}
	if (RowScroll != nullptr && FMath::Abs(ScrollStick) > 0.f)
	{
		RowScroll->SetScrollOffset(RowScroll->GetScrollOffset() - ScrollStick * 900.f * InDeltaTime);
	}
}

FReply USpnInventoryScreen::NativeOnKeyDown(const FGeometry& Geometry, const FKeyEvent& Event)
{
	return HandleKey(Event.GetKey(), Event.IsShiftDown(), Event.IsRepeat()) ? FReply::Handled() : Super::NativeOnKeyDown(Geometry, Event);
}

FReply USpnInventoryScreen::NativeOnAnalogValueChanged(const FGeometry& Geometry, const FAnalogInputEvent& Event)
{
	HandleAnalog(Event.GetKey(), Event.GetAnalogValue());
	return Super::NativeOnAnalogValueChanged(Geometry, Event);
}

FReply USpnInventoryScreen::NativeOnMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& Event)
{
	bUsingMouseMode = true;
	SetUsingGamepad(false);
	SetKeyboardFocus();
	return FReply::Handled(); // the screen is modal: a click on empty space does nothing
}

FReply USpnInventoryScreen::NativeOnMouseMove(const FGeometry& Geometry, const FPointerEvent& Event)
{
	if (!Event.GetCursorDelta().IsNearlyZero() && !bUsingMouseMode)
	{
		bUsingMouseMode = true;
		SetUsingGamepad(false);
		if (TitleText != nullptr) { SyncSelectionVisuals(); SyncEquipment(); SyncActions(); }
	}
	return Super::NativeOnMouseMove(Geometry, Event);
}

FReply USpnInventoryScreen::NativeOnMouseWheel(const FGeometry& Geometry, const FPointerEvent& Event)
{
	const FSpnContainerPanel* Panel = ActivePanel();
	if (Panel != nullptr && Panel->Rows.Num() > MaxRowsShown && ViewModel != nullptr)
	{
		// a windowed list cannot scroll natively: the wheel moves the selection (and with it the window)
		for (int32 Step = 0; Step < 3; ++Step)
		{
			ViewModel->Navigate(Event.GetWheelDelta() > 0.f ? ESpnNavigationDirection::Previous : ESpnNavigationDirection::Next);
		}
		return FReply::Handled();
	}
	return Super::NativeOnMouseWheel(Geometry, Event);
}

// === inspection (what is really on screen) =============================================================================================================

FString USpnInventoryScreen::GetRowTitle(int32 Index) const { return RowPool.IsValidIndex(Index) && Index < VisibleRows ? RowPool[Index]->GetTitleString() : FString(); }
FString USpnInventoryScreen::GetRowRight(int32 Index) const { return RowPool.IsValidIndex(Index) && Index < VisibleRows ? RowPool[Index]->GetRightString() : FString(); }
FString USpnInventoryScreen::GetRowSubtitle(int32 Index) const { return RowPool.IsValidIndex(Index) && Index < VisibleRows ? RowPool[Index]->GetSubtitleString() : FString(); }
FString USpnInventoryScreen::GetRowBadge(int32 Index) const { return RowPool.IsValidIndex(Index) && Index < VisibleRows ? RowPool[Index]->GetBadgeString() : FString(); }
FString USpnInventoryScreen::GetRowIconLetter(int32 Index) const { return RowPool.IsValidIndex(Index) && Index < VisibleRows ? RowPool[Index]->GetIconLetterString() : FString(); }
bool USpnInventoryScreen::IsRowSelected(int32 Index) const { return RowPool.IsValidIndex(Index) && Index < VisibleRows && RowPool[Index]->IsSelected(); }
bool USpnInventoryScreen::IsRowFocusRing(int32 Index) const { return RowPool.IsValidIndex(Index) && Index < VisibleRows && RowPool[Index]->IsFocusedRing(); }
FString USpnInventoryScreen::GetTabTitle(int32 Index) const { return TabPool.IsValidIndex(Index) && Index < VisibleTabs ? TabPool[Index]->GetTitleString() : FString(); }
bool USpnInventoryScreen::IsTabSelected(int32 Index) const { return TabPool.IsValidIndex(Index) && Index < VisibleTabs && TabPool[Index]->IsSelected(); }
FString USpnInventoryScreen::GetSlotTitle(int32 Index) const { return SlotPool.IsValidIndex(Index) && Index < VisibleSlots ? SlotPool[Index]->GetTitleString() : FString(); }
FString USpnInventoryScreen::GetSlotSubtitle(int32 Index) const { return SlotPool.IsValidIndex(Index) && Index < VisibleSlots ? SlotPool[Index]->GetSubtitleString() : FString(); }
bool USpnInventoryScreen::IsSlotFocusRing(int32 Index) const { return SlotPool.IsValidIndex(Index) && Index < VisibleSlots && SlotPool[Index]->IsFocusedRing(); }
FString USpnInventoryScreen::GetActionTitle(int32 Index) const { return ActionPool.IsValidIndex(Index) && Index < VisibleActions ? ActionPool[Index]->GetTitleString() : FString(); }
FString USpnInventoryScreen::GetActionSubtitle(int32 Index) const { return ActionPool.IsValidIndex(Index) && Index < VisibleActions ? ActionPool[Index]->GetSubtitleString() : FString(); }
FString USpnInventoryScreen::GetActionRight(int32 Index) const { return ActionPool.IsValidIndex(Index) && Index < VisibleActions ? ActionPool[Index]->GetRightString() : FString(); }
bool USpnInventoryScreen::IsActionEnabled(int32 Index) const { return ActionPool.IsValidIndex(Index) && Index < VisibleActions && ActionPool[Index]->IsEnabledState(); }
bool USpnInventoryScreen::IsActionFocusRing(int32 Index) const { return ActionPool.IsValidIndex(Index) && Index < VisibleActions && ActionPool[Index]->IsFocusedRing(); }
FString USpnInventoryScreen::GetActionsEmptyText() const
{
	return VisibleActions == 0 && ActionPool.Num() > 0 && ActionPool[0]->Tag == -1 && ActionPool[0]->GetVisibility() != ESlateVisibility::Collapsed ? ActionPool[0]->GetTitleString() : FString();
}

FString USpnInventoryScreen::GetDetailsTitle() const { return DetailsName != nullptr ? DetailsName->GetText().ToString() : FString(); }

FString USpnInventoryScreen::GetDetailsText() const
{
	FString Text = DetailsDescription != nullptr ? DetailsDescription->GetText().ToString() : FString();
	if (DetailsStats != nullptr)
	{
		for (UWidget* Child : DetailsStats->GetAllChildren())
		{
			if (const UHorizontalBox* Line = Cast<UHorizontalBox>(Child))
			{
				FString LineText;
				for (UWidget* Part : Line->GetAllChildren())
				{
					if (const UTextBlock* Label = Cast<UTextBlock>(Part)) { LineText += Label->GetText().ToString() + TEXT(" "); }
					else if (const USizeBox* Sized = Cast<USizeBox>(Part)) { if (const UTextBlock* Inner = Cast<UTextBlock>(Sized->GetContent())) { LineText += Inner->GetText().ToString() + TEXT(" "); } }
				}
				Text += TEXT("\n") + LineText.TrimEnd();
			}
		}
	}
	return Text;
}

FString USpnInventoryScreen::GetCapacityText(ESpnCapacityKind Kind) const
{
	const int32 Index = Kind == ESpnCapacityKind::Weight ? 0 : (Kind == ESpnCapacityKind::Volume ? 1 : 2);
	return CapacityValues.IsValidIndex(Index) ? CapacityValues[Index]->GetText().ToString() : FString();
}

float USpnInventoryScreen::GetCapacityPercent(ESpnCapacityKind Kind) const
{
	const int32 Index = Kind == ESpnCapacityKind::Weight ? 0 : (Kind == ESpnCapacityKind::Volume ? 1 : 2);
	return CapacityBars.IsValidIndex(Index) ? CapacityBars[Index]->GetPercent() : 0.f;
}

FString USpnInventoryScreen::GetMessageText() const { return MessageLine != nullptr ? MessageLine->GetText().ToString() : FString(); }
FString USpnInventoryScreen::GetHintText() const { return HintLine != nullptr ? HintLine->GetText().ToString() : FString(); }
FString USpnInventoryScreen::GetEmptyText() const { return EmptyText != nullptr && EmptyText->GetVisibility() != ESlateVisibility::Collapsed ? EmptyText->GetText().ToString() : FString(); }
FString USpnInventoryScreen::GetDialogText() const { return bDialogOpen && DialogBody != nullptr ? DialogTitle->GetText().ToString() + TEXT(" | ") + DialogBody->GetText().ToString() : FString(); }
FString USpnInventoryScreen::GetHeaderTitle() const { return TitleText != nullptr ? TitleText->GetText().ToString() : FString(); }

void USpnInventoryScreen::SetContainerVisibility(const TSet<int64>& Include, const TSet<int64>& Exclude, int64 FocusContainer)
{
	IncludeContainers = Include;
	ExcludeContainers = Exclude;
	FocusContainerId = FocusContainer;
}
