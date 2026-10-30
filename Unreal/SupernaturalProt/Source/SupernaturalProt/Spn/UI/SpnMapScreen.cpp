#include "Spn/UI/SpnMapScreen.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/ScaleBox.h"
#include "Components/SizeBox.h"
#include "Components/Spacer.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Engine/GameInstance.h"
#include "GameFramework/PlayerController.h"
#include "Spn/UI/SpnMapView.h"
#include "Spn/UI/SpnUiText.h"
#include "Styling/SlateBrush.h"
#include "Styling/SlateTypes.h"

namespace
{
	FSlateBrush Box(const FLinearColor& Fill, const FLinearColor& Outline = FLinearColor::Transparent, float OutlineWidth = 0.f, float Radius = 4.f)
	{
		return FSlateRoundedBoxBrush(Fill, Radius, Outline, OutlineWidth);
	}

	FText Distance(double Metres)
	{
		return Metres >= 1000.0 ? FText::FromString(FString::Printf(TEXT("%.2f km"), Metres / 1000.0)) : FText::FromString(FString::Printf(TEXT("%d m"), FMath::RoundToInt(Metres)));
	}

	FString Wind(double BearingRadians)
	{
		static const TCHAR* Names[] = {TEXT("N"), TEXT("NE"), TEXT("E"), TEXT("SE"), TEXT("S"), TEXT("SW"), TEXT("W"), TEXT("NW")};
		const int32 Index = FMath::RoundToInt(FMath::RadiansToDegrees(BearingRadians) / 45.0) % 8;
		return Names[(Index + 8) % 8];
	}

	constexpr double PanStepPx = 70.0;
	constexpr double ZoomStep = 1.25;
	constexpr double PanSpeedPx = 520.0;
	constexpr double ZoomRate = 1.7;
}

UTextBlock* USpnMapScreen::NewText(int32 Size, bool bBold, const FLinearColor& Color)
{
	UTextBlock* Text = WidgetTree->ConstructWidget<UTextBlock>();
	Text->SetFont(SpnUi::Font(Size, bBold));
	Text->SetColorAndOpacity(FSlateColor(Color));
	Text->SetAutoWrapText(true);
	return Text;
}

USpnUiButton* USpnMapScreen::NewButton(const FText& Title, TFunction<void()> Action)
{
	APlayerController* Owner = GetOwningPlayer();
	USpnUiButton* Button = Owner != nullptr ? CreateWidget<USpnUiButton>(Owner, USpnUiButton::StaticClass()) : CreateWidget<USpnUiButton>(GetWorld(), USpnUiButton::StaticClass());
	FSpnUiButtonContent Content;
	Content.Title = Title;
	Button->SetContent(Content);
	Button->OnClicked.BindLambda([Action](USpnUiButton*) { Action(); });
	Button->SetVisualState(false, false, true);
	return Button;
}

TSharedRef<SWidget> USpnMapScreen::RebuildWidget()
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
	return Slate;
}

void USpnMapScreen::ReleaseSlateResources(bool bReleaseChildren)
{
	HeldSlate.Reset();
	Super::ReleaseSlateResources(bReleaseChildren);
}

void USpnMapScreen::BuildTree()
{
	UOverlay* Root = WidgetTree->ConstructWidget<UOverlay>();
	WidgetTree->RootWidget = Root;
	UBorder* Backdrop = WidgetTree->ConstructWidget<UBorder>();
	Backdrop->SetBrush(Box(FLinearColor(0.f, 0.f, 0.f, 0.93f), FLinearColor::Transparent, 0.f, 0.f));
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
	UHorizontalBox* Row = WidgetTree->ConstructWidget<UHorizontalBox>();
	Design->SetContent(Row);

	// left: the map
	UBorder* MapFrame = WidgetTree->ConstructWidget<UBorder>();
	MapFrame->SetBrush(Box(SpnUi::Header, SpnUi::PanelEdge, 1.f, 6.f));
	MapFrame->SetPadding(FMargin(2.f));
	MapView = WidgetTree->ConstructWidget<USpnMapViewWidget>();
	MapView->Configure(SSpnMapView::EMode::Full, FVector2D(1380.0, 996.0));
	MapFrame->SetContent(MapView);
	UHorizontalBoxSlot* MapSlot = Row->AddChildToHorizontalBox(MapFrame);
	MapSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));

	// right: information, actions, legend
	UBorder* Side = WidgetTree->ConstructWidget<UBorder>();
	Side->SetBrush(Box(SpnUi::Panel, SpnUi::PanelEdge, 1.f, 6.f));
	Side->SetPadding(FMargin(20.f, 18.f));
	UHorizontalBoxSlot* SideSlot = Row->AddChildToHorizontalBox(Side);
	SideSlot->SetPadding(FMargin(14.f, 0.f, 0.f, 0.f));
	USizeBox* SideWidth = WidgetTree->ConstructWidget<USizeBox>();
	SideWidth->SetWidthOverride(420.f);
	Side->SetContent(SideWidth);
	UVerticalBox* Col = WidgetTree->ConstructWidget<UVerticalBox>();
	SideWidth->SetContent(Col);

	TitleText = NewText(26, true, SpnUi::Text);
	Col->AddChildToVerticalBox(TitleText);
	UTextBlock* Placeholder = NewText(12, false, SpnUi::Warning);
	Placeholder->SetText(FSpnUiText::Get(TEXT("ui.map.placeholder")));
	Col->AddChildToVerticalBox(Placeholder)->SetPadding(FMargin(0.f, 4.f, 0.f, 14.f));

	auto Section = [&](const TCHAR* Key, int32 Size = 14) -> UTextBlock*
	{
		UTextBlock* Head = NewText(11, true, SpnUi::TextDim);
		Head->SetText(FText::FromString(FSpnUiText::Get(Key).ToString().ToUpper()));
		Col->AddChildToVerticalBox(Head)->SetPadding(FMargin(0.f, 8.f, 0.f, 2.f));
		UTextBlock* Body = NewText(Size, false, SpnUi::Text);
		Col->AddChildToVerticalBox(Body);
		return Body;
	};
	PlayerText = Section(TEXT("ui.map.you"));
	SelectionText = Section(TEXT("ui.map.selection"));
	WaypointText = Section(TEXT("ui.map.waypoint"));

	ButtonBox = WidgetTree->ConstructWidget<UVerticalBox>();
	Col->AddChildToVerticalBox(ButtonBox)->SetPadding(FMargin(0.f, 14.f, 0.f, 0.f));
	auto AddButton = [this](const TCHAR* Key, TFunction<void()> Action)
	{
		USpnUiButton* B = NewButton(FSpnUiText::Get(Key), MoveTemp(Action));
		ButtonBox->AddChildToVerticalBox(B)->SetPadding(FMargin(0.f, 0.f, 0.f, 4.f));
		Buttons.Add(B);
	};
	AddButton(TEXT("ui.map.btn.waypoint"), [this] { PlaceWaypoint(); });
	AddButton(TEXT("ui.map.btn.remove"), [this] { RemoveWaypoint(); });
	AddButton(TEXT("ui.map.btn.center"), [this] { CenterOnPlayer(); });
	AddButton(TEXT("ui.map.btn.close"), [this] { OnCloseRequested.ExecuteIfBound(); });

	UTextBlock* LegendHead = NewText(11, true, SpnUi::TextDim);
	LegendHead->SetText(FText::FromString(FSpnUiText::Get(TEXT("ui.map.legend")).ToString().ToUpper()));
	Col->AddChildToVerticalBox(LegendHead)->SetPadding(FMargin(0.f, 14.f, 0.f, 4.f));
	LegendBox = WidgetTree->ConstructWidget<UVerticalBox>();
	Col->AddChildToVerticalBox(LegendBox);
	for (const FSpnMapSymbol& Symbol : SpnMapStyle::LegendSymbols())
	{
		UHorizontalBox* Line = WidgetTree->ConstructWidget<UHorizontalBox>();
		USizeBox* Swatch = WidgetTree->ConstructWidget<USizeBox>();
		Swatch->SetWidthOverride(14.f);
		Swatch->SetHeightOverride(14.f);
		UBorder* Color = WidgetTree->ConstructWidget<UBorder>();
		const float Radius = Symbol.Shape == FSpnMapSymbol::EShape::Circle ? 7.f : Symbol.Shape == FSpnMapSymbol::EShape::Diamond ? 3.f : 1.f;
		Color->SetBrush(Box(Symbol.Color, FLinearColor::Transparent, 0.f, Radius));
		Swatch->SetContent(Color);
		Line->AddChildToHorizontalBox(Swatch)->SetVerticalAlignment(VAlign_Center);
		UTextBlock* Label = NewText(13, false, SpnUi::Text);
		Label->SetText(FSpnUiText::Get(Symbol.LegendKey));
		Line->AddChildToHorizontalBox(Label)->SetPadding(FMargin(10.f, 0.f, 0.f, 0.f));
		LegendBox->AddChildToVerticalBox(Line)->SetPadding(FMargin(0.f, 2.f));
		++LegendCount;
	}
	UTextBlock* Discovery = NewText(12, false, SpnUi::TextDim);
	Discovery->SetText(FSpnUiText::Get(TEXT("ui.map.legend.discovery")));
	LegendBox->AddChildToVerticalBox(Discovery)->SetPadding(FMargin(0.f, 6.f, 0.f, 0.f));

	UVerticalBoxSlot* Spacer = Col->AddChildToVerticalBox(WidgetTree->ConstructWidget<USpacer>());
	Spacer->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
	MessageText = NewText(14, true, SpnUi::Good);
	Col->AddChildToVerticalBox(MessageText)->SetPadding(FMargin(0.f, 6.f, 0.f, 6.f));
	HintText = NewText(12, false, SpnUi::TextFaint);
	Col->AddChildToVerticalBox(HintText);
}

void USpnMapScreen::Setup(USpnMapSubsystem* InMap)
{
	Map = InMap;
	TakeWidget(); // builds the tree
	MapView->SetMap(InMap);
	MapView->OnMapClick.BindUObject(this, &USpnMapScreen::OnMapClick);
	if (SSpnMapView* View = MapView->GetView())
	{
		View->OnMapClick = MapView->OnMapClick;
	}
}

void USpnMapScreen::SetUsingGamepad(bool bValue)
{
	bUsingGamepad = bValue;
	HintText->SetText(FSpnUiText::Get(bValue ? TEXT("ui.map.hints.pad") : TEXT("ui.map.hints.kb")));
}

bool USpnMapScreen::GetPlayerPose(FVector2D& OutMetres, double& OutBearing, FVector& OutLocation, float& OutYaw) const
{
	const APlayerController* Controller = GetOwningPlayer();
	const APawn* Pawn = Controller != nullptr ? Controller->GetPawn() : nullptr;
	const USpnMapSubsystem* M = Map.Get();
	if (Pawn == nullptr || M == nullptr)
	{
		return false;
	}
	OutLocation = Pawn->GetActorLocation();
	OutYaw = Controller->GetControlRotation().Yaw;
	return M->UnrealToMap(OutLocation, OutMetres) && SpnWorldFrame::YawDegreesToBearing(M->GetFrame().Axes, OutYaw, OutBearing);
}

void USpnMapScreen::OnShown()
{
	TitleText->SetText(FSpnUiText::Get(TEXT("ui.map.title")));
	SetUsingGamepad(bUsingGamepad);
	ClearSelection();
	MessageText->SetText(FText::GetEmpty());
	MessageUntil = 0.0;
	PanInput = PanKeys = FVector2D::ZeroVector;
	ZoomInput = ZoomKeys = 0.f;
	if (SSpnMapView* View = MapView->GetView())
	{
		FVector2D Metres;
		double Bearing;
		FVector Location;
		float Yaw;
		if (GetPlayerPose(Metres, Bearing, Location, Yaw))
		{
			View->SetPlayer(Metres, Bearing);
		}
		View->FitToBounds();
		// open around the player at a readable scale, not on the whole area
		View->CenterOnPlayer();
		View->ZoomBy(1.0);
		View->bShowCrosshair = bUsingGamepad;
	}
	LastRevision = ~0ull;
	NextTextRefresh = 0.0;
	RefreshTexts();
}

void USpnMapScreen::CenterOnPlayer()
{
	if (SSpnMapView* View = MapView->GetView())
	{
		View->CenterOnPlayer();
	}
}

void USpnMapScreen::ClearSelection()
{
	Select(FString(), 0);
}

void USpnMapScreen::Select(const FString& PoiId, int64 MarkerId)
{
	SelectedPoi = PoiId;
	SelectedMarker = MarkerId;
	if (SSpnMapView* View = MapView != nullptr ? MapView->GetView() : nullptr)
	{
		View->SetSelection(PoiId, MarkerId);
	}
	LastRevision = ~0ull;
}

FVector2D USpnMapScreen::GetCrosshairMetres() const
{
	const SSpnMapView* View = MapView != nullptr ? MapView->GetView() : nullptr;
	return View != nullptr ? View->GetCrosshairMetres() : FVector2D::ZeroVector;
}

double USpnMapScreen::GetMetresPerPixel() const
{
	const SSpnMapView* View = MapView != nullptr ? MapView->GetView() : nullptr;
	return View != nullptr ? View->GetMetresPerPixel() : 1.0;
}

bool USpnMapScreen::IsCrosshairShown() const
{
	const SSpnMapView* View = MapView != nullptr ? MapView->GetView() : nullptr;
	return View != nullptr && View->bShowCrosshair;
}

void USpnMapScreen::ShowMessage(const FText& Text, const FLinearColor& Color)
{
	MessageText->SetText(Text);
	MessageText->SetColorAndOpacity(FSlateColor(Color));
	MessageUntil = Clock + 6.0;
}

void USpnMapScreen::PlaceWaypoint()
{
	USpnMapSubsystem* M = Map.Get();
	if (M == nullptr)
	{
		return;
	}
	int64 Marker = 0;
	ESpnMapResult Result;
	if (!SelectedPoi.IsEmpty())
	{
		Result = M->SetWaypointToPoi(SelectedPoi, Marker);
	}
	else
	{
		Result = M->SetWaypointAt(GetCrosshairMetres(), FSpnUiText::Get(TEXT("ui.map.waypoint.default")).ToString(), Marker);
	}
	if (Result == ESpnMapResult::Ok)
	{
		Select(FString(), Marker);
		FSpnMapMarkerView Waypoint;
		ShowMessage(M->GetWaypoint(Waypoint) ? FSpnUiText::Get(TEXT("ui.map.msg.set"), {FText::FromString(Waypoint.Label)}) : FText::GetEmpty(), SpnUi::Good);
	}
	else
	{
		ShowMessage(Result == ESpnMapResult::OutOfBounds ? FSpnUiText::Get(TEXT("ui.map.msg.outside")) : FSpnUiText::Get(TEXT("ui.map.msg.failed")), SpnUi::Error);
	}
	LastRevision = ~0ull;
}

void USpnMapScreen::RemoveWaypoint()
{
	USpnMapSubsystem* M = Map.Get();
	if (M == nullptr)
	{
		return;
	}
	const bool bRemoved = M->ClearWaypoint();
	if (bRemoved)
	{
		ClearSelection();
	}
	ShowMessage(bRemoved ? FSpnUiText::Get(TEXT("ui.map.msg.removed")) : FSpnUiText::Get(TEXT("ui.map.msg.none")), bRemoved ? SpnUi::Good : SpnUi::TextDim);
	LastRevision = ~0ull;
}

void USpnMapScreen::OnMapClick(const FVector2D& Metres, bool bRight)
{
	USpnMapSubsystem* M = Map.Get();
	SSpnMapView* View = MapView->GetView();
	if (M == nullptr || View == nullptr)
	{
		return;
	}
	View->bShowCrosshair = false; // the mouse is in use
	if (bRight)
	{
		int64 Marker = 0;
		const ESpnMapResult Result = M->SetWaypointAt(Metres, FSpnUiText::Get(TEXT("ui.map.waypoint.default")).ToString(), Marker);
		if (Result == ESpnMapResult::Ok)
		{
			Select(FString(), Marker);
			ShowMessage(FSpnUiText::Get(TEXT("ui.map.msg.set"), {FSpnUiText::Get(TEXT("ui.map.waypoint.default"))}), SpnUi::Good);
		}
		else
		{
			ShowMessage(FSpnUiText::Get(Result == ESpnMapResult::OutOfBounds ? TEXT("ui.map.msg.outside") : TEXT("ui.map.msg.failed")), SpnUi::Error);
		}
		return;
	}
	FSpnMapPoiView Poi;
	FSpnMapMarkerView Marker;
	if (M->PickAt(Metres, FMath::Max(View->GetMetresPerPixel() * 16.0, 0.5), &Poi, &Marker))
	{
		Select(Poi.Id, Marker.Id);
	}
	else
	{
		ClearSelection();
	}
}

void USpnMapScreen::SelectNext(int32 Step)
{
	USpnMapSubsystem* M = Map.Get();
	SSpnMapView* View = MapView->GetView();
	if (M == nullptr || View == nullptr)
	{
		return;
	}
	struct FEntry { FString Poi; int64 Marker; FString Key; FVector2D At; };
	TArray<FEntry> Entries;
	const FSpnMapBounds Bounds = M->GetBounds();
	TArray<FSpnMapPoiView> Pois;
	TArray<FSpnMapMarkerView> Markers;
	M->QueryVisible(Bounds.Min, Bounds.Max, Pois, Markers);
	for (const FSpnMapPoiView& P : Pois) { Entries.Add({P.Id, 0, TEXT("p:") + P.Id, P.Metres}); }
	for (const FSpnMapMarkerView& K : Markers) { Entries.Add({FString(), K.Id, FString::Printf(TEXT("m:%020lld"), K.Id), K.Metres}); }
	if (Entries.IsEmpty())
	{
		ClearSelection();
		return;
	}
	Entries.Sort([](const FEntry& A, const FEntry& B) { return A.Key < B.Key; });
	int32 Index = Entries.IndexOfByPredicate([this](const FEntry& E) { return (!E.Poi.IsEmpty() && E.Poi == SelectedPoi) || (E.Marker != 0 && E.Marker == SelectedMarker); });
	Index = Index == INDEX_NONE ? (Step > 0 ? 0 : Entries.Num() - 1) : (Index + Step + Entries.Num()) % Entries.Num();
	Select(Entries[Index].Poi, Entries[Index].Marker);
	View->PanMetres(Entries[Index].At - View->GetCenterMetres()); // the crosshair moves to the entry
}

bool USpnMapScreen::HandleKey(const FKey& Key, bool bRepeat, bool bShift)
{
	SSpnMapView* View = MapView->GetView();
	if (View == nullptr)
	{
		return false;
	}
	const bool bPad = Key.IsGamepadKey();
	if (bPad != bUsingGamepad)
	{
		SetUsingGamepad(bPad);
	}
	auto Keyboard = [&]() { View->bShowCrosshair = true; };
	if (Key == EKeys::Escape || Key == EKeys::M || Key == EKeys::Gamepad_FaceButton_Top || Key == EKeys::Gamepad_FaceButton_Right)
	{
		if (!bRepeat) { OnCloseRequested.ExecuteIfBound(); }
		return true;
	}
	const double Step = PanStepPx * View->GetMetresPerPixel();
	if (Key == EKeys::Left || Key == EKeys::A || Key == EKeys::Gamepad_DPad_Left) { Keyboard(); View->PanMetres(FVector2D(-Step, 0)); return true; }
	if (Key == EKeys::Right || Key == EKeys::D || Key == EKeys::Gamepad_DPad_Right) { Keyboard(); View->PanMetres(FVector2D(Step, 0)); return true; }
	if (Key == EKeys::Up || Key == EKeys::W || Key == EKeys::Gamepad_DPad_Up) { Keyboard(); View->PanMetres(FVector2D(0, Step)); return true; }
	if (Key == EKeys::Down || Key == EKeys::S || Key == EKeys::Gamepad_DPad_Down) { Keyboard(); View->PanMetres(FVector2D(0, -Step)); return true; }
	if (Key == EKeys::E || Key == EKeys::Equals || Key == EKeys::Add || Key == EKeys::PageUp) { View->ZoomBy(ZoomStep); return true; }
	if (Key == EKeys::Q || Key == EKeys::Hyphen || Key == EKeys::Subtract || Key == EKeys::PageDown) { View->ZoomBy(1.0 / ZoomStep); return true; }
	if (bRepeat)
	{
		return true;
	}
	if (Key == EKeys::Enter || Key == EKeys::SpaceBar || Key == EKeys::Gamepad_FaceButton_Bottom) { Keyboard(); PlaceWaypoint(); return true; }
	if (Key == EKeys::Delete || Key == EKeys::Gamepad_FaceButton_Left) { RemoveWaypoint(); return true; }
	if (Key == EKeys::Tab || Key == EKeys::Gamepad_RightShoulder) { Keyboard(); SelectNext(bShift ? -1 : 1); return true; }
	if (Key == EKeys::Gamepad_LeftShoulder) { Keyboard(); SelectNext(-1); return true; }
	if (Key == EKeys::C || Key == EKeys::Home || Key == EKeys::Gamepad_LeftThumbstick) { CenterOnPlayer(); return true; }
	if (Key == EKeys::F) { View->FitToBounds(); return true; }
	return true; // modal: nothing else reaches the game while the map is open
}

void USpnMapScreen::HandleAnalog(const FKey& Key, float Value)
{
	if (Key == EKeys::Gamepad_LeftX) { PanInput.X = Value; }
	else if (Key == EKeys::Gamepad_LeftY) { PanInput.Y = Value; }
	else if (Key == EKeys::Gamepad_RightTriggerAxis) { ZoomInput = FMath::Max(ZoomInput, Value) ; if (Value < 0.1f) { ZoomInput = FMath::Min(ZoomInput, 0.f); } }
	else if (Key == EKeys::Gamepad_LeftTriggerAxis) { ZoomInput = Value > 0.1f ? -Value : (ZoomInput < 0.f ? 0.f : ZoomInput); }
	if (SSpnMapView* View = MapView != nullptr ? MapView->GetView() : nullptr)
	{
		if (PanInput.SizeSquared() > 0.04) { View->bShowCrosshair = true; }
	}
}

void USpnMapScreen::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	Clock += InDeltaTime;
	SSpnMapView* View = MapView != nullptr ? MapView->GetView() : nullptr;
	if (View == nullptr)
	{
		return;
	}
	FVector2D Metres;
	double Bearing;
	FVector Location;
	float Yaw;
	if (GetPlayerPose(Metres, Bearing, Location, Yaw))
	{
		View->SetPlayer(Metres, Bearing);
		if (USpnMapSubsystem* M = Map.Get())
		{
			View->SetNavigation(M->GetNavigation(Location, Yaw));
		}
	}
	if (PanInput.SizeSquared() > 0.04)
	{
		const FVector2D Dir = PanInput.GetClampedToMaxSize(1.0);
		View->PanMetres(FVector2D(Dir.X, Dir.Y) * PanSpeedPx * View->GetMetresPerPixel() * InDeltaTime);
	}
	if (FMath::Abs(ZoomInput) > 0.1f)
	{
		View->ZoomBy(FMath::Exp(ZoomRate * ZoomInput * InDeltaTime));
	}
	if (MessageUntil > 0.0 && Clock > MessageUntil)
	{
		MessageText->SetText(FText::GetEmpty());
		MessageUntil = 0.0;
	}
	USpnMapSubsystem* M = Map.Get();
	if (Clock >= NextTextRefresh || (M != nullptr && M->GetRevision() != LastRevision))
	{
		NextTextRefresh = Clock + 0.15;
		RefreshTexts();
		LastRevision = M != nullptr ? M->GetRevision() : 0;
	}
}

void USpnMapScreen::RefreshTexts()
{
	USpnMapSubsystem* M = Map.Get();
	if (M == nullptr || PlayerText == nullptr)
	{
		return;
	}
	FVector2D Metres;
	double Bearing;
	FVector Location;
	float Yaw;
	const bool bPose = GetPlayerPose(Metres, Bearing, Location, Yaw);
	if (bPose)
	{
		const FString Region = M->GetRegionLabel();
		PlayerText->SetText(FText::FromString(FString::Printf(TEXT("E %.1f m   N %.1f m\n%s"), Metres.X, Metres.Y, *Region)));
	}
	else
	{
		PlayerText->SetText(FSpnUiText::Get(TEXT("ui.map.nopawn")));
	}

	FText Selection = FSpnUiText::Get(TEXT("ui.map.selection.none"));
	if (!SelectedPoi.IsEmpty())
	{
		for (const FSpnMapPoiView& P : M->GetPois())
		{
			if (P.Id == SelectedPoi)
			{
				const FString Discovered = FSpnUiText::Get(P.Discovery == ESpnDiscovery::Visited ? TEXT("ui.map.discovery.visited") : TEXT("ui.map.discovery.known")).ToString();
				const double Dist = bPose ? FVector2D::Distance(Metres, P.Metres) : 0.0;
				Selection = FText::FromString(FString::Printf(TEXT("%s\n%s%s"), *P.Label, *Discovered, bPose ? *FString::Printf(TEXT(" · %s"), *Distance(Dist).ToString()) : TEXT("")));
			}
		}
	}
	else if (SelectedMarker != 0)
	{
		for (const FSpnMapMarkerView& K : M->GetMarkers())
		{
			if (K.Id == SelectedMarker)
			{
				Selection = FText::FromString(FString::Printf(TEXT("%s\n%s"), *K.Label, *FSpnUiText::Get(K.bPlayerWaypoint ? TEXT("ui.map.legend.waypoint") : TEXT("ui.map.legend.other")).ToString()));
			}
		}
	}
	SelectionText->SetText(Selection);

	FText WaypointInfo = FSpnUiText::Get(TEXT("ui.map.waypoint.none"));
	if (bPose)
	{
		const FSpnNavigationReading Reading = M->GetNavigation(Location, Yaw);
		if (Reading.bActive)
		{
			WaypointInfo = Reading.bArrived
				? FSpnUiText::Get(TEXT("ui.map.waypoint.arrived"), {FText::FromString(Reading.Label)})
				: FSpnUiText::Get(TEXT("ui.map.waypoint.info"), {FText::FromString(Reading.Label), Distance(Reading.DistanceMetres), FText::FromString(Wind(Reading.BearingRadians)),
					FText::AsNumber(FMath::RoundToInt(FMath::RadiansToDegrees(Reading.BearingRadians)) % 360)});
		}
	}
	WaypointText->SetText(WaypointInfo);
}

FString USpnMapScreen::GetPlayerString() const { return PlayerText != nullptr ? PlayerText->GetText().ToString() : FString(); }
FString USpnMapScreen::GetSelectionString() const { return SelectionText != nullptr ? SelectionText->GetText().ToString() : FString(); }
FString USpnMapScreen::GetWaypointString() const { return WaypointText != nullptr ? WaypointText->GetText().ToString() : FString(); }
FString USpnMapScreen::GetMessageString() const { return MessageText != nullptr ? MessageText->GetText().ToString() : FString(); }
