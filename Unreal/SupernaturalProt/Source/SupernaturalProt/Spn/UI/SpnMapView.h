#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "SpnMapSubsystem.h"
#include "Widgets/SLeafWidget.h"

#include "SpnMapView.generated.h"

// Drawing of the world map and the minimap (Night Build 01). ENGINEERING PROPOSAL (look, shapes, colours), not master canon.
//
// Both views read USpnMapSubsystem and nothing else: they own no markers, no discovery and no waypoint. They draw only what lies inside their own
// rectangle (USpnMapSubsystem::QueryVisible / BuildMinimapFrame), so a future tiled, streamed world costs the same as the test district.
// There is NO terrain art yet: the map shows a metre grid, the layer bounds, regions, points of interest and markers - nothing is faked.

/** How a marker category is drawn. One place, shared by the map, the minimap and the legend. */
struct FSpnMapSymbol
{
	enum class EShape : uint8 { Circle, Square, Diamond, Triangle };
	EShape Shape = EShape::Circle;
	FLinearColor Color = FLinearColor::White;
	/** Text key of the legend line (FSpnUiText). */
	const TCHAR* LegendKey = TEXT("ui.map.legend.other");
};

namespace SpnMapStyle
{
	SUPERNATURALPROT_API FSpnMapSymbol SymbolFor(const FString& Category, bool bPlayerWaypoint);
	/** Categories that appear in the legend, in display order. */
	SUPERNATURALPROT_API TArray<FSpnMapSymbol> LegendSymbols();
}

/** Full map or minimap. */
class SUPERNATURALPROT_API SSpnMapView : public SLeafWidget
{
public:
	enum class EMode : uint8 { Full, Mini, Compass };

	SLATE_BEGIN_ARGS(SSpnMapView) : _Mode(EMode::Full) {}
		SLATE_ARGUMENT(EMode, Mode)
		SLATE_ARGUMENT(TWeakObjectPtr<USpnMapSubsystem>, Map)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	// --- shared ---
	void SetMap(USpnMapSubsystem* InMap) { Map = InMap; }
	/** Player pose in map metres and as compass bearing (radians clockwise from north). */
	void SetPlayer(const FVector2D& Metres, double BearingRadians);

	// --- full map ---
	void CenterOnPlayer();
	void PanMetres(const FVector2D& DeltaMetres);
	void PanScreen(const FVector2D& DeltaPx);
	void ZoomBy(double Factor);
	/** Scripted camera (demo): sets the view centre and scale directly. */
	void SetViewMetres(const FVector2D& Centre, double InMetresPerPixel) { CenterMetres = Centre; MetresPerPixel = FMath::Clamp(InMetresPerPixel, 0.02, 2000.0); }
	void FitToBounds();
	FVector2D GetCenterMetres() const { return CenterMetres; }
	double GetMetresPerPixel() const { return MetresPerPixel; }
	/** Map position under the widget centre (the keyboard / controller crosshair). */
	FVector2D GetCrosshairMetres() const { return CenterMetres; }
	/** Map position under the mouse pointer (last known while it was over the view). */
	FVector2D GetCursorMetres() const { return CursorMetres; }
	/** Highlighted entry (selection): a POI id or a marker id; both empty/0 = none. */
	void SetSelection(const FString& InPoiId, int64 InMarkerId) { SelectedPoi = InPoiId; SelectedMarker = InMarkerId; }
	bool IsMouseOver() const { return bPointerInside; }
	FVector2D LocalToMap(const FVector2D& LocalPx) const;
	FVector2D MapToLocal(const FVector2D& Metres) const;
	/** Raised on a left click (select) and a right click (place waypoint). Metres = clicked map position. */
	DECLARE_DELEGATE_TwoParams(FOnMapClick, const FVector2D& /*Metres*/, bool /*bRightButton*/);
	FOnMapClick OnMapClick;
	bool bShowCrosshair = false;

	// --- minimap ---
	void SetMinimapFrame(const FSpnMinimapFrameData& InFrame, bool bInHeadingUp) { Frame = InFrame; bHeadingUp = bInHeadingUp; }
	void SetNavigation(const FSpnNavigationReading& InReading) { Navigation = InReading; }
	/** Road map layer of the minimap (replaces nothing when invalid). */
	void SetRoadFrame(const FSpnMapRenderFrame& InFrame) { RoadFrame = InFrame; }

	virtual FVector2D ComputeDesiredSize(float) const override { return DesiredSizePx; }
	void SetDesiredSizePx(const FVector2D& Size) { DesiredSizePx = Size; }
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
		int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual FReply OnMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& Event) override;
	virtual FReply OnMouseButtonUp(const FGeometry& Geometry, const FPointerEvent& Event) override;
	virtual FReply OnMouseMove(const FGeometry& Geometry, const FPointerEvent& Event) override;
	virtual FReply OnMouseWheel(const FGeometry& Geometry, const FPointerEvent& Event) override;
	virtual void OnMouseLeave(const FPointerEvent& Event) override { bPointerInside = false; bDragging = false; }
	virtual void OnMouseEnter(const FGeometry& Geometry, const FPointerEvent& Event) override { bPointerInside = true; }

private:
	int32 PaintFull(const FGeometry& G, FSlateWindowElementList& Out, int32 Layer) const;
	int32 PaintMini(const FGeometry& G, FSlateWindowElementList& Out, int32 Layer) const;
	int32 PaintCompass(const FGeometry& G, FSlateWindowElementList& Out, int32 Layer) const;

	EMode Mode = EMode::Full;
	TWeakObjectPtr<USpnMapSubsystem> Map;
	FVector2D DesiredSizePx = FVector2D(800.0, 600.0);
	FVector2D PlayerMetres = FVector2D::ZeroVector;
	double PlayerBearing = 0.0;
	mutable FVector2D LastSize = FVector2D(800.0, 600.0);

	// full map view
	FVector2D CenterMetres = FVector2D::ZeroVector;
	double MetresPerPixel = 0.25;
	FString SelectedPoi;
	int64 SelectedMarker = 0;
	bool bPointerInside = false;
	FVector2D CursorMetres = FVector2D::ZeroVector;
	bool bDragging = false;
	bool bDragMoved = false;
	FVector2D DragStartLocal = FVector2D::ZeroVector;
	FVector2D LastDragLocal = FVector2D::ZeroVector;

	// minimap
	FSpnMinimapFrameData Frame;
	bool bHeadingUp = false;
	FSpnNavigationReading Navigation;
	mutable FSpnMapRenderFrame RoadFrame;
};

/** UMG wrapper so the map views can sit inside a UMG widget tree. */
UCLASS()
class SUPERNATURALPROT_API USpnMapViewWidget : public UWidget
{
	GENERATED_BODY()

public:
	void Configure(SSpnMapView::EMode InMode, const FVector2D& DesiredSize);
	SSpnMapView* GetView() const { return View.Get(); }
	/** Order independent: stored here and handed to the Slate view whenever it is (re)built. */
	void SetMap(USpnMapSubsystem* InMap);
	SSpnMapView::FOnMapClick OnMapClick;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

private:
	SSpnMapView::EMode Mode = SSpnMapView::EMode::Full;
	FVector2D Size = FVector2D(800.0, 600.0);
	TSharedPtr<SSpnMapView> View;
	TWeakObjectPtr<USpnMapSubsystem> MapRef;
};
