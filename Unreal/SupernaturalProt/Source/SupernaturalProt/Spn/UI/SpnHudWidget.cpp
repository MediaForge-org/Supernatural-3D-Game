#include "Spn/UI/SpnHudWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/ScaleBox.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Engine/GameInstance.h"
#include "GameFramework/PlayerController.h"
#include "SpnMapSubsystem.h"
#include "Spn/SpnSettingsSubsystem.h"
#include "Spn/UI/SpnMapView.h"
#include "Spn/UI/SpnUiButton.h"
#include "Spn/UI/SpnUiText.h"
#include "Styling/SlateBrush.h"
#include "Styling/SlateTypes.h"

namespace
{
	constexpr int32 MaxNotes = 5;
	constexpr double NoteSeconds = 5.0;
	constexpr double NoteFadeSeconds = 1.0;

	FSlateBrush Panel(const FLinearColor& Fill, float Radius = 5.f)
	{
		return FSlateRoundedBoxBrush(Fill, Radius, FLinearColor::Transparent, 0.f);
	}

	FLinearColor ColorFor(SpnUi::ENote Kind)
	{
		switch (Kind)
		{
		case SpnUi::ENote::Good: return SpnUi::Good;
		case SpnUi::ENote::Warning: return SpnUi::Warning;
		case SpnUi::ENote::Error: return SpnUi::Error;
		default: return SpnUi::Text;
		}
	}

	FText FormatDistance(double Metres)
	{
		return Metres >= 1000.0 ? FText::FromString(FString::Printf(TEXT("%.1f km"), Metres / 1000.0)) : FText::FromString(FString::Printf(TEXT("%d m"), FMath::RoundToInt(Metres)));
	}
}

TSharedRef<SWidget> USpnHudWidget::RebuildWidget()
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

void USpnHudWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	HeldSlate.Reset();
	Super::ReleaseSlateResources(bReleaseChildren);
}

void USpnHudWidget::BuildTree()
{
	UScaleBox* Scale = WidgetTree->ConstructWidget<UScaleBox>();
	Scale->SetStretch(EStretch::ScaleToFit);
	WidgetTree->RootWidget = Scale;
	USizeBox* Design = WidgetTree->ConstructWidget<USizeBox>();
	Design->SetWidthOverride(1920.f);
	Design->SetHeightOverride(1080.f);
	Scale->AddChild(Design);
	UOverlay* Root = WidgetTree->ConstructWidget<UOverlay>();
	Design->SetContent(Root);

	auto Text = [this](int32 Size, bool bBold, const FLinearColor& Color)
	{
		UTextBlock* T = WidgetTree->ConstructWidget<UTextBlock>();
		T->SetFont(SpnUi::Font(Size, bBold));
		T->SetColorAndOpacity(FSlateColor(Color));
		T->SetShadowOffset(FVector2D(1.0, 1.0));
		T->SetShadowColorAndOpacity(FLinearColor(0.f, 0.f, 0.f, 0.8f));
		return T;
	};

	// compass, top centre
	CompassBox = WidgetTree->ConstructWidget<UBorder>();
	CompassBox->SetBrush(Panel(FLinearColor::Transparent));
	USizeBox* CompassSize = WidgetTree->ConstructWidget<USizeBox>();
	CompassSize->SetWidthOverride(640.f);
	CompassSize->SetHeightOverride(56.f);
	Compass = WidgetTree->ConstructWidget<USpnMapViewWidget>();
	Compass->Configure(SSpnMapView::EMode::Compass, FVector2D(640.0, 56.0));
	Compass->SetVisibility(ESlateVisibility::HitTestInvisible);
	CompassSize->SetContent(Compass);
	CompassBox->SetContent(CompassSize);
	UOverlaySlot* CompassSlot = Root->AddChildToOverlay(CompassBox);
	CompassSlot->SetHorizontalAlignment(HAlign_Center);
	CompassSlot->SetVerticalAlignment(VAlign_Top);
	CompassSlot->SetPadding(FMargin(0.f, 16.f, 0.f, 0.f));
	CompassBox->SetVisibility(ESlateVisibility::Collapsed);

	// minimap, top right
	MinimapColumn = WidgetTree->ConstructWidget<UVerticalBox>();
	USizeBox* MiniSize = WidgetTree->ConstructWidget<USizeBox>();
	MiniSize->SetWidthOverride(230.f);
	MiniSize->SetHeightOverride(230.f);
	Minimap = WidgetTree->ConstructWidget<USpnMapViewWidget>();
	Minimap->Configure(SSpnMapView::EMode::Mini, FVector2D(230.0, 230.0));
	Minimap->SetVisibility(ESlateVisibility::HitTestInvisible);
	MiniSize->SetContent(Minimap);
	MinimapColumn->AddChildToVerticalBox(MiniSize);
	MinimapCaption = Text(13, true, SpnUi::Text);
	MinimapCaption->SetAutoWrapText(true);
	MinimapCaption->SetJustification(ETextJustify::Center);
	MinimapCaption->SetWrapTextAt(230.f);
	UVerticalBoxSlot* CaptionSlot = MinimapColumn->AddChildToVerticalBox(MinimapCaption);
	CaptionSlot->SetPadding(FMargin(0.f, 6.f, 0.f, 0.f));
	CaptionSlot->SetHorizontalAlignment(HAlign_Center);
	UOverlaySlot* MiniSlot = Root->AddChildToOverlay(MinimapColumn);
	MiniSlot->SetHorizontalAlignment(HAlign_Right);
	MiniSlot->SetVerticalAlignment(VAlign_Top);
	MiniSlot->SetPadding(FMargin(0.f, 24.f, 28.f, 0.f));
	MinimapColumn->SetVisibility(ESlateVisibility::Collapsed);

	// interaction prompt, lower third
	PromptBox = WidgetTree->ConstructWidget<UBorder>();
	PromptBox->SetBrush(Panel(FLinearColor(0.f, 0.f, 0.f, 0.62f), 4.f));
	PromptBox->SetPadding(FMargin(22.f, 9.f));
	PromptText = Text(30, true, FLinearColor::White);
	PromptBox->SetContent(PromptText);
	UOverlaySlot* PromptSlot = Root->AddChildToOverlay(PromptBox);
	PromptSlot->SetHorizontalAlignment(HAlign_Center);
	PromptSlot->SetVerticalAlignment(VAlign_Bottom);
	PromptSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 190.f));
	PromptBox->SetVisibility(ESlateVisibility::Collapsed);

	// notifications, lower left
	NoteBox = WidgetTree->ConstructWidget<UVerticalBox>();
	for (int32 I = 0; I < MaxNotes; ++I)
	{
		UTextBlock* Line = Text(20, true, SpnUi::Text);
		Line->SetVisibility(ESlateVisibility::Collapsed);
		NoteBox->AddChildToVerticalBox(Line)->SetPadding(FMargin(0.f, 3.f));
		NoteLines.Add(Line);
	}
	UOverlaySlot* NoteSlot = Root->AddChildToOverlay(NoteBox);
	NoteSlot->SetHorizontalAlignment(HAlign_Left);
	NoteSlot->SetVerticalAlignment(VAlign_Bottom);
	NoteSlot->SetPadding(FMargin(32.f, 0.f, 0.f, 120.f));

	// contextual hints, lower right
	HintText = Text(14, false, SpnUi::TextDim);
	UOverlaySlot* HintSlot = Root->AddChildToOverlay(HintText);
	HintSlot->SetHorizontalAlignment(HAlign_Right);
	HintSlot->SetVerticalAlignment(VAlign_Bottom);
	HintSlot->SetPadding(FMargin(0.f, 0.f, 32.f, 24.f));
	RefreshHint();
}

void USpnHudWidget::PushNotification(const FText& Text, SpnUi::ENote Kind)
{
	FNote Note;
	Note.Text = Text;
	Note.Kind = Kind;
	Note.Expires = Clock + NoteSeconds;
	Notes.Add(Note);
	while (Notes.Num() > MaxNotes)
	{
		Notes.RemoveAt(0);
	}
	RefreshNotes();
}

void USpnHudWidget::SetPrompt(const FString& Text)
{
	if (PromptBox == nullptr)
	{
		return;
	}
	if (Text.IsEmpty())
	{
		PromptBox->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	if (PromptText->GetText().ToString() != Text)
	{
		PromptText->SetText(FText::FromString(Text));
	}
	PromptBox->SetVisibility(bSuppressed ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
}

void USpnHudWidget::SetUsingGamepad(bool bValue)
{
	if (bGamepad != bValue)
	{
		bGamepad = bValue;
		bHintDirty = true;
	}
}

void USpnHudWidget::SetSuppressed(bool bValue)
{
	bSuppressed = bValue;
	SetVisibility(bValue ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
}

void USpnHudWidget::RefreshNotes()
{
	for (int32 I = 0; I < NoteLines.Num(); ++I)
	{
		UTextBlock* Line = NoteLines[I];
		if (I < Notes.Num())
		{
			const double Left = Notes[I].Expires - Clock;
			const float Alpha = static_cast<float>(FMath::Clamp(Left / NoteFadeSeconds, 0.0, 1.0));
			FLinearColor Color = ColorFor(Notes[I].Kind);
			Color.A = Alpha;
			Line->SetText(Notes[I].Text);
			Line->SetColorAndOpacity(FSlateColor(Color));
			Line->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			Line->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void USpnHudWidget::RefreshHint()
{
	HintCache = FSpnUiText::Get(bGamepad ? TEXT("ui.hud.hints.pad") : TEXT("ui.hud.hints.kb")).ToString();
	if (HintText != nullptr)
	{
		HintText->SetText(FText::FromString(HintCache));
	}
	bHintDirty = false;
}

void USpnHudWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	Clock += InDeltaTime;
	if (bHintDirty)
	{
		RefreshHint();
	}
	if (Notes.Num() > 0)
	{
		Notes.RemoveAll([this](const FNote& N) { return N.Expires <= Clock; });
		RefreshNotes();
	}

	APlayerController* Controller = GetOwningPlayer();
	const APawn* Pawn = Controller != nullptr ? Controller->GetPawn() : nullptr;
	UGameInstance* Instance = GetGameInstance();
	USpnMapSubsystem* Map = Instance != nullptr ? Instance->GetSubsystem<USpnMapSubsystem>() : nullptr;
	const USpnSettingsSubsystem* Settings = Instance != nullptr ? Instance->GetSubsystem<USpnSettingsSubsystem>() : nullptr;
	const FSpnPlayerSettings Prefs = Settings != nullptr ? Settings->Get() : FSpnPlayerSettings();

	const bool bMap = Map != nullptr && Map->HasDefinition() && Pawn != nullptr;
	CompassBox->SetVisibility(bMap ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	MinimapColumn->SetVisibility(bMap && Prefs.bMinimapVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	if (!bMap)
	{
		return;
	}

	const FVector Location = Pawn->GetActorLocation();
	const float Yaw = Controller->GetControlRotation().Yaw;
	Map->ObservePlayer(Location);
	Map->UpdateNavigation(Location, Yaw);

	FVector2D Metres;
	double Bearing = 0.0;
	if (!Map->UnrealToMap(Location, Metres) || !SpnWorldFrame::YawDegreesToBearing(Map->GetFrame().Axes, Yaw, Bearing))
	{
		return;
	}
	const FSpnNavigationReading Reading = Map->GetNavigation(Location, Yaw);
	if (SSpnMapView* View = Compass->GetView())
	{
		View->SetPlayer(Metres, Bearing);
		View->SetNavigation(Reading);
	}

	if (Prefs.bMinimapVisible)
	{
		const uint64 Revision = Map->GetRevision();
		const bool bMoved = FVector::DistSquared(Location, LastLocation) > 30.f * 30.f;
		const bool bTurned = FMath::Abs(FRotator::NormalizeAxis(Yaw - LastYaw)) > 1.f;
		const bool bChanged = Revision != LastRevision || Map->GetRouteGeneration() != LastRouteGeneration || Prefs.MinimapZoom != LastZoom || Prefs.bMinimapHeadingUp != LastHeadingUp;
		const bool bStale = Clock - LastRebuild > 0.5;
		if (bMoved || bTurned || bChanged || bStale)
		{
			FSpnMinimapRequest Request;
			Request.PlayerLocationCm = Location;
			Request.PlayerYawDegrees = Yaw;
			Request.Zoom = Prefs.MinimapZoom;
			Request.bHeadingUp = Prefs.bMinimapHeadingUp;
			Request.RadiusPx = 112.f;
			if (SSpnMapView* View = Minimap->GetView())
			{
				View->SetPlayer(Metres, Bearing);
				View->SetMinimapFrame(Map->BuildMinimapFrame(Request), Prefs.bMinimapHeadingUp);
				View->SetNavigation(Reading);
				if (Map->HasRoadNetwork())
				{
					// road layer + route of the minimap: the same Core render frame as the map screen (square viewport)
					FSpnMapRenderRequest Road;
					Road.CenterMetres = Metres;
					Road.HeadingBearingRadians = Bearing;
					Road.MetresPerPixel = 0.8 / FMath::Max(Prefs.MinimapZoom, 0.1f);
					Road.bHeadingUp = Prefs.bMinimapHeadingUp;
					Road.HalfSizePx = FVector2D(Request.RadiusPx, Request.RadiusPx);
					View->SetRoadFrame(Map->BuildMapRenderFrame(Road));
				}
				else
				{
					View->SetRoadFrame(FSpnMapRenderFrame());
				}
			}
			FText Caption;
			if (Reading.bActive && !Reading.bStraightLine)
			{
				if (Reading.Status == ESpnNavStatus::NoRoute)
				{
					Caption = FSpnUiText::Get(TEXT("ui.hud.noroute"), {FText::FromString(Reading.Label), FSpnUiText::Get(*(FString(TEXT("ui.nav.fail.")) + Reading.FailureText))});
				}
				else if (Reading.bArrived)
				{
					Caption = FSpnUiText::Get(TEXT("ui.hud.arrived"), {FText::FromString(Reading.Label)});
				}
				else if (Reading.bOffRoute)
				{
					Caption = FSpnUiText::Get(TEXT("ui.hud.offroute"), {FText::FromString(Reading.Label), FormatDistance(Reading.DistanceMetres)});
				}
				else
				{
					Caption = FSpnUiText::Get(TEXT("ui.hud.route"), {FText::FromString(Reading.Label), FormatDistance(Reading.DistanceMetres)});
				}
			}
			else if (Reading.bActive)
			{
				Caption = Reading.bArrived ? FSpnUiText::Get(TEXT("ui.hud.arrived"), {FText::FromString(Reading.Label)})
					: FSpnUiText::Get(TEXT("ui.hud.waypoint"), {FText::FromString(Reading.Label), FormatDistance(Reading.DistanceMetres)});
			}
			else
			{
				Caption = FText::FromString(Map->GetRegionLabel());
			}
			MinimapCaption->SetText(Caption);
			LastLocation = Location;
			LastYaw = Yaw;
			LastRevision = Revision;
			LastRouteGeneration = Map->GetRouteGeneration();
			LastZoom = Prefs.MinimapZoom;
			LastHeadingUp = Prefs.bMinimapHeadingUp;
			LastRebuild = Clock;
			++MinimapRebuilds;
		}
	}
}

FString USpnHudWidget::GetPromptString() const { return PromptText != nullptr ? PromptText->GetText().ToString() : FString(); }
bool USpnHudWidget::IsPromptVisible() const { return PromptBox != nullptr && PromptBox->GetVisibility() != ESlateVisibility::Collapsed; }
FString USpnHudWidget::GetNotificationString(int32 Index) const { return Notes.IsValidIndex(Index) ? Notes[Index].Text.ToString() : FString(); }
bool USpnHudWidget::IsMinimapVisible() const { return MinimapColumn != nullptr && MinimapColumn->GetVisibility() != ESlateVisibility::Collapsed; }
bool USpnHudWidget::IsCompassVisible() const { return CompassBox != nullptr && CompassBox->GetVisibility() != ESlateVisibility::Collapsed; }
FString USpnHudWidget::GetNavigationString() const { return MinimapCaption != nullptr ? MinimapCaption->GetText().ToString() : FString(); }
FString USpnHudWidget::GetHintString() const { return HintCache; }
