#include "Spn/UI/SpnMapView.h"

#include "Fonts/FontMeasure.h"
#include "Framework/Application/SlateApplication.h"
#include "Rendering/DrawElements.h"
#include "Spn/UI/SpnUiButton.h"
#include "Spn/UI/SpnUiText.h"
#include "Styling/CoreStyle.h"
#include "Styling/SlateBrush.h"
#include "Styling/SlateTypes.h"

namespace SpnMapStyle
{
	FSpnMapSymbol SymbolFor(const FString& Category, bool bPlayerWaypoint)
	{
		FSpnMapSymbol Symbol;
		if (bPlayerWaypoint)
		{
			Symbol.Shape = FSpnMapSymbol::EShape::Diamond;
			Symbol.Color = FLinearColor(0.95f, 0.50f, 0.22f, 1.f);
			Symbol.LegendKey = TEXT("ui.map.legend.waypoint");
		}
		else if (Category.EndsWith(TEXT("marker.landmark")))
		{
			Symbol.Shape = FSpnMapSymbol::EShape::Circle;
			Symbol.Color = SpnUi::Accent;
			Symbol.LegendKey = TEXT("ui.map.legend.landmark");
		}
		else if (Category.EndsWith(TEXT("marker.street")))
		{
			Symbol.Shape = FSpnMapSymbol::EShape::Triangle;
			Symbol.Color = FLinearColor(0.62f, 0.66f, 0.70f, 1.f);
			Symbol.LegendKey = TEXT("ui.map.legend.street");
		}
		else if (Category.EndsWith(TEXT("marker.building")))
		{
			Symbol.Shape = FSpnMapSymbol::EShape::Square;
			Symbol.Color = SpnUi::Warning;
			Symbol.LegendKey = TEXT("ui.map.legend.building");
		}
		else
		{
			Symbol.Shape = FSpnMapSymbol::EShape::Circle;
			Symbol.Color = SpnUi::TextDim;
		}
		return Symbol;
	}

	TArray<FSpnMapSymbol> LegendSymbols()
	{
		return {SymbolFor(TEXT("marker.landmark"), false), SymbolFor(TEXT("marker.street"), false), SymbolFor(TEXT("marker.building"), false),
			SymbolFor(FString(), true)};
	}
}

namespace
{
	const FSlateBrush* White() { return FCoreStyle::Get().GetBrush("WhiteBrush"); }

	FVector2f F(const FVector2D& V) { return FVector2f(static_cast<float>(V.X), static_cast<float>(V.Y)); }

	FVector2D Rotate(const FVector2D& V, double Radians)
	{
		const double C = FMath::Cos(Radians), S = FMath::Sin(Radians);
		return FVector2D(V.X * C - V.Y * S, V.X * S + V.Y * C);
	}

	void Box(FSlateWindowElementList& Out, int32 Layer, const FGeometry& G, const FVector2D& TopLeft, const FVector2D& Size, const FLinearColor& Color)
	{
		FSlateDrawElement::MakeBox(Out, Layer, G.ToPaintGeometry(F(Size), FSlateLayoutTransform(F(TopLeft))), White(), ESlateDrawEffect::None, Color);
	}

	void Dot(FSlateWindowElementList& Out, int32 Layer, const FGeometry& G, const FVector2D& Center, float Radius, const FLinearColor& Fill,
		const FLinearColor& Outline = FLinearColor::Transparent, float OutlineWidth = 0.f)
	{
		const FSlateRoundedBoxBrush Brush(FLinearColor::White, Radius, Outline, OutlineWidth);
		FSlateDrawElement::MakeBox(Out, Layer, G.ToPaintGeometry(FVector2f(Radius * 2.f, Radius * 2.f), FSlateLayoutTransform(F(Center - FVector2D(Radius, Radius)))),
			&Brush, ESlateDrawEffect::None, Fill);
	}

	void Line(FSlateWindowElementList& Out, int32 Layer, const FGeometry& G, const FVector2D& A, const FVector2D& B, const FLinearColor& Color, float Thickness = 1.f)
	{
		TArray<FVector2f> Points;
		Points.Add(F(A));
		Points.Add(F(B));
		FSlateDrawElement::MakeLines(Out, Layer, G.ToPaintGeometry(), Points, ESlateDrawEffect::None, Color, true, Thickness);
	}

	void Poly(FSlateWindowElementList& Out, int32 Layer, const FGeometry& G, const TArray<FVector2D>& Pts, const FLinearColor& Color, float Thickness)
	{
		TArray<FVector2f> Points;
		for (const FVector2D& P : Pts) { Points.Add(F(P)); }
		if (Pts.Num() > 2) { Points.Add(F(Pts[0])); }
		FSlateDrawElement::MakeLines(Out, Layer, G.ToPaintGeometry(), Points, ESlateDrawEffect::None, Color, true, Thickness);
	}

	void Text(FSlateWindowElementList& Out, int32 Layer, const FGeometry& G, const FVector2D& TopLeft, const FString& Value, int32 Size, const FLinearColor& Color, bool bBold = false)
	{
		FSlateDrawElement::MakeText(Out, Layer, G.ToPaintGeometry(FVector2f(400.f, 40.f), FSlateLayoutTransform(F(TopLeft))), Value, SpnUi::Font(Size, bBold),
			ESlateDrawEffect::None, Color);
	}

	FVector2D MeasureText(const FString& Value, int32 Size, bool bBold = false)
	{
		return FVector2D(FSlateApplication::Get().GetRenderer()->GetFontMeasureService()->Measure(Value, SpnUi::Font(Size, bBold)));
	}

	/** Arrow pointing along `AngleRadians` (clockwise from screen-up). */
	void Arrow(FSlateWindowElementList& Out, int32 Layer, const FGeometry& G, const FVector2D& Center, double AngleRadians, float Size, const FLinearColor& Color)
	{
		TArray<FVector2D> Pts;
		Pts.Add(Center + Rotate(FVector2D(0.0, -Size), AngleRadians));
		Pts.Add(Center + Rotate(FVector2D(Size * 0.72, Size * 0.78), AngleRadians));
		Pts.Add(Center + Rotate(FVector2D(0.0, Size * 0.38), AngleRadians));
		Pts.Add(Center + Rotate(FVector2D(-Size * 0.72, Size * 0.78), AngleRadians));
		Poly(Out, Layer, G, Pts, Color, 2.4f);
		Dot(Out, Layer, G, Center, 1.8f, Color);
	}

	void Symbol(FSlateWindowElementList& Out, int32 Layer, const FGeometry& G, const FVector2D& C, const FSpnMapSymbol& S, float Size, bool bFilled)
	{
		const FLinearColor Dark(0.02f, 0.025f, 0.03f, 1.f);
		switch (S.Shape)
		{
		case FSpnMapSymbol::EShape::Circle:
			Dot(Out, Layer, G, C, Size, S.Color, Dark, 1.2f);
			if (!bFilled) { Dot(Out, Layer + 1, G, C, Size * 0.5f, Dark); }
			break;
		case FSpnMapSymbol::EShape::Square:
			Box(Out, Layer, G, C - FVector2D(Size, Size), FVector2D(Size * 2.f, Size * 2.f), Dark);
			Box(Out, Layer + 1, G, C - FVector2D(Size - 1.2f, Size - 1.2f), FVector2D((Size - 1.2f) * 2.f, (Size - 1.2f) * 2.f), S.Color);
			if (!bFilled) { Box(Out, Layer + 2, G, C - FVector2D(Size * 0.45f, Size * 0.45f), FVector2D(Size * 0.9f, Size * 0.9f), Dark); }
			break;
		case FSpnMapSymbol::EShape::Diamond:
		{
			TArray<FVector2D> Pts{C + FVector2D(0, -Size * 1.35f), C + FVector2D(Size * 1.05f, 0), C + FVector2D(0, Size * 1.35f), C + FVector2D(-Size * 1.05f, 0)};
			Poly(Out, Layer, G, Pts, S.Color, 3.2f);
			if (bFilled) { Line(Out, Layer, G, Pts[0], Pts[2], S.Color, 3.f); Line(Out, Layer, G, Pts[1], Pts[3], S.Color, 3.f); }
			break;
		}
		case FSpnMapSymbol::EShape::Triangle:
		{
			TArray<FVector2D> Pts{C + FVector2D(0, -Size * 1.1f), C + FVector2D(Size, Size * 0.8f), C + FVector2D(-Size, Size * 0.8f)};
			Poly(Out, Layer, G, Pts, S.Color, bFilled ? 3.f : 1.6f);
			break;
		}
		}
	}

	// ----- road map drawing (NightBuild03): consumes the Core MapRenderFrame through USpnMapSubsystem::BuildMapRenderFrame -----

	/** Ear-clipping triangulation of a simple ring (screen space). False for degenerate input; the caller then draws nothing for the fill. */
	bool Triangulate(const TArray<FVector2D>& P, TArray<SlateIndex>& OutIndices)
	{
		const int32 N = P.Num();
		if (N < 3 || N > 512) { return false; }
		double Area = 0.0;
		for (int32 I = 0; I < N; ++I) { const FVector2D& A = P[I]; const FVector2D& B = P[(I + 1) % N]; Area += A.X * B.Y - B.X * A.Y; }
		if (FMath::Abs(Area) < 1e-6) { return false; }
		TArray<int32> Idx;
		for (int32 I = 0; I < N; ++I) { Idx.Add(Area > 0.0 ? I : N - 1 - I); }
		auto Cross = [&](int32 A, int32 B, int32 C) { return (P[B].X - P[A].X) * (P[C].Y - P[A].Y) - (P[B].Y - P[A].Y) * (P[C].X - P[A].X); };
		int32 Guard = N * N + 8;
		while (Idx.Num() > 3 && Guard-- > 0)
		{
			bool bClipped = false;
			for (int32 I = 0; I < Idx.Num(); ++I)
			{
				const int32 A = Idx[(I + Idx.Num() - 1) % Idx.Num()], B = Idx[I], C = Idx[(I + 1) % Idx.Num()];
				if (Cross(A, B, C) <= 1e-9) { continue; } // reflex (or flat) corner
				bool bInside = false;
				for (const int32 Q : Idx)
				{
					if (Q == A || Q == B || Q == C) { continue; }
					if (Cross(A, B, Q) >= 0.0 && Cross(B, C, Q) >= 0.0 && Cross(C, A, Q) >= 0.0) { bInside = true; break; }
				}
				if (bInside) { continue; }
				OutIndices.Add(static_cast<SlateIndex>(A)); OutIndices.Add(static_cast<SlateIndex>(B)); OutIndices.Add(static_cast<SlateIndex>(C));
				Idx.RemoveAt(I);
				bClipped = true;
				break;
			}
			if (!bClipped) { return false; }
		}
		if (Idx.Num() == 3) { OutIndices.Add(static_cast<SlateIndex>(Idx[0])); OutIndices.Add(static_cast<SlateIndex>(Idx[1])); OutIndices.Add(static_cast<SlateIndex>(Idx[2])); }
		return OutIndices.Num() >= 3;
	}

	void FillPoly(FSlateWindowElementList& Out, int32 Layer, const FGeometry& G, const TArray<FVector2D>& Pts, const FLinearColor& Color)
	{
		TArray<SlateIndex> Indices;
		if (!Triangulate(Pts, Indices)) { return; }
		const FSlateRenderTransform Transform = G.ToPaintGeometry().GetAccumulatedRenderTransform();
		const FColor C = Color.ToFColor(true);
		TArray<FSlateVertex> Verts;
		Verts.Reserve(Pts.Num());
		for (const FVector2D& P : Pts) { Verts.Add(FSlateVertex::Make<ESlateVertexRounding::Disabled>(Transform, F(P), FVector2f(0.f, 0.f), C)); }
		const FSlateResourceHandle Handle = FSlateApplication::Get().GetRenderer()->GetResourceHandle(*White());
		FSlateDrawElement::MakeCustomVerts(Out, Layer, Handle, Verts, Indices, nullptr, 0, 0);
	}

	void Stroke(FSlateWindowElementList& Out, int32 Layer, const FGeometry& G, const TArray<FVector2D>& Pts, const FLinearColor& Color, float Thickness)
	{
		if (Pts.Num() < 2) { return; }
		TArray<FVector2f> Points;
		Points.Reserve(Pts.Num());
		for (const FVector2D& P : Pts) { Points.Add(F(P)); }
		FSlateDrawElement::MakeLines(Out, Layer, G.ToPaintGeometry(), Points, ESlateDrawEffect::None, Color, true, Thickness);
	}

	TArray<FVector2D> ToLocal(const TArray<FVector2D>& Screen, const FVector2D& Origin)
	{
		TArray<FVector2D> Out;
		Out.Reserve(Screen.Num());
		for (const FVector2D& P : Screen) { Out.Add(Origin + P); }
		return Out;
	}

	FLinearColor AreaColor(uint8 Kind)
	{
		switch (Kind)
		{
		case 0: return FLinearColor(0.07f, 0.20f, 0.36f, 1.f);   // Water
		case 1: return FLinearColor(0.10f, 0.19f, 0.10f, 1.f);   // Island
		case 2: return FLinearColor(0.05f, 0.15f, 0.07f, 1.f);   // Forest
		case 3: return FLinearColor(0.08f, 0.21f, 0.10f, 1.f);   // Park
		case 4: return FLinearColor(0.07f, 0.16f, 0.08f, 1.f);   // Green
		case 5: return FLinearColor(0.19f, 0.18f, 0.16f, 1.f);   // Industrial
		case 6: return FLinearColor(0.15f, 0.16f, 0.18f, 1.f);   // Parking
		case 7: return FLinearColor(0.21f, 0.20f, 0.18f, 1.f);   // Plaza
		case 9: return FLinearColor(0.17f, 0.15f, 0.12f, 1.f);   // Mountain
		default: return FLinearColor::Transparent;               // District: outline only
		}
	}

	/** Road width in metres and fill colour per Core RoadClass. Placeholder look (not an approved UI style). */
	void RoadStyle(uint8 Class, double& OutWidthMetres, FLinearColor& OutFill)
	{
		static const double Widths[] = {14.0, 8.0, 12.0, 10.0, 8.0, 6.0, 4.0, 3.0, 2.5, 1.6};
		static const FLinearColor Fills[] = {
			FLinearColor(0.86f, 0.55f, 0.22f, 1.f), FLinearColor(0.80f, 0.52f, 0.24f, 1.f), FLinearColor(0.88f, 0.70f, 0.28f, 1.f),
			FLinearColor(0.90f, 0.82f, 0.45f, 1.f), FLinearColor(0.78f, 0.78f, 0.74f, 1.f), FLinearColor(0.62f, 0.64f, 0.66f, 1.f),
			FLinearColor(0.52f, 0.54f, 0.57f, 1.f), FLinearColor(0.48f, 0.40f, 0.28f, 1.f), FLinearColor(0.30f, 0.38f, 0.24f, 1.f),
			FLinearColor(0.70f, 0.52f, 0.52f, 1.f)};
		const int32 I = FMath::Clamp<int32>(Class, 0, 9);
		OutWidthMetres = Widths[I];
		OutFill = Fills[I];
	}

	/** Draws a road MapRenderFrame. Uses layers Layer .. Layer + 4. Origin = viewport centre in widget pixels. */
	void DrawRoadFrame(FSlateWindowElementList& Out, int32 Layer, const FGeometry& G, const FSpnMapRenderFrame& Fr, const FVector2D& Origin, bool bDetail)
	{
		for (const FSpnRenderPolygon& Area : Fr.Areas)
		{
			const TArray<FVector2D> Pts = ToLocal(Area.Points, Origin);
			const FLinearColor Fill = AreaColor(Area.Kind);
			if (Fill.A > 0.f) { FillPoly(Out, Layer, G, Pts, Fill); }
			else { Poly(Out, Layer, G, Pts, FLinearColor(0.20f, 0.26f, 0.31f, 0.6f), 1.f); }
		}
		for (const FSpnRenderPolyline& Ln : Fr.Lines)
		{
			// LineKind: River, Stream, Coastline, Contour
			const FLinearColor C = Ln.Kind == 3 ? FLinearColor(0.30f, 0.26f, 0.20f, 0.7f) : FLinearColor(0.16f, 0.36f, 0.58f, 1.f);
			Stroke(Out, Layer + 1, G, ToLocal(Ln.Points, Origin), C, Ln.Kind == 3 ? 1.f : (Ln.Kind == 1 ? 1.5f : 2.5f));
		}
		for (const FSpnRenderPolygon& B : Fr.Buildings)
		{
			const TArray<FVector2D> Pts = ToLocal(B.Points, Origin);
			FillPoly(Out, Layer + 1, G, Pts, FLinearColor(0.30f, 0.27f, 0.25f, 1.f));
			Poly(Out, Layer + 1, G, Pts, FLinearColor(0.50f, 0.45f, 0.40f, 1.f), 1.f);
			if (bDetail)
			{
				for (const FVector2D& E : B.Entrances) { Dot(Out, Layer + 1, G, Origin + E, 2.2f, FLinearColor(0.95f, 0.85f, 0.35f, 1.f)); }
			}
		}
		const double Mpp = FMath::Max(Fr.MetresPerPixel, 1e-6);
		// casings first (bridges get a light casing, tunnels a dim one), then the fills, so junctions merge cleanly
		for (int32 Pass = 0; Pass < 2; ++Pass)
		{
			for (const FSpnRenderPolyline& Road : Fr.Roads)
			{
				double WidthM = 6.0;
				FLinearColor Fill;
				RoadStyle(Road.Kind, WidthM, Fill);
				const float Width = static_cast<float>(FMath::Clamp(WidthM / Mpp, Road.Kind <= 6 ? 3.0 : 2.0, 34.0));
				const bool bTunnel = Road.Structure == 2 || Road.Structure == 3;
				const bool bBridge = Road.Structure == 1;
				const TArray<FVector2D> Pts = ToLocal(Road.Points, Origin);
				if (Pass == 0)
				{
					const FLinearColor Casing = bBridge ? FLinearColor(0.95f, 0.95f, 0.98f, 1.f) : FLinearColor(0.01f, 0.012f, 0.015f, bTunnel ? 0.5f : 1.f);
					Stroke(Out, Layer + 2, G, Pts, Casing, Width + (bBridge ? 3.f : 2.f));
				}
				else
				{
					FLinearColor C = Fill;
					if (bTunnel) { C = C * 0.7f; C.A = 0.45f; }
					Stroke(Out, Layer + 3, G, Pts, C, Width);
					if (Road.bClosed)
					{
						Stroke(Out, Layer + 4, G, Pts, FLinearColor(0.92f, 0.16f, 0.14f, 0.95f), FMath::Max(Width * 0.55f, 2.f));
						if (Pts.Num() >= 2)
						{
							const FVector2D Mid = Pts[Pts.Num() / 2];
							Dot(Out, Layer + 4, G, Mid, 7.f, FLinearColor(0.92f, 0.16f, 0.14f, 1.f), FLinearColor::White, 1.5f);
							Line(Out, Layer + 4, G, Mid + FVector2D(-3.5, -3.5), Mid + FVector2D(3.5, 3.5), FLinearColor::White, 2.f);
							Line(Out, Layer + 4, G, Mid + FVector2D(-3.5, 3.5), Mid + FVector2D(3.5, -3.5), FLinearColor::White, 2.f);
						}
					}
				}
			}
		}
		// route: a dark casing under a bright line; the part already travelled is grey
		for (const TArray<FVector2D>& Part : Fr.RouteTravelled)
		{
			Stroke(Out, Layer + 4, G, ToLocal(Part, Origin), FLinearColor(0.45f, 0.52f, 0.58f, 0.9f), 4.f);
		}
		for (const TArray<FVector2D>& Part : Fr.RouteRemaining)
		{
			const TArray<FVector2D> Pts = ToLocal(Part, Origin);
			Stroke(Out, Layer + 4, G, Pts, FLinearColor(0.02f, 0.02f, 0.03f, 0.9f), 8.f);
			Stroke(Out, Layer + 4, G, Pts, FLinearColor(1.00f, 0.55f, 0.12f, 1.f), 5.f);
		}
		if (Fr.bHasDestinationIndicator)
		{
			Dot(Out, Layer + 4, G, Origin + Fr.DestinationIndicator, 6.f, FLinearColor(1.f, 0.55f, 0.12f, 1.f), FLinearColor::White, 1.5f);
		}
	}

	double NiceStep(double MinMetres)
	{
		static const double Steps[] = {1, 2, 5, 10, 25, 50, 100, 250, 500, 1000, 2500, 5000, 10000, 25000, 50000, 100000, 250000, 500000, 1000000};
		for (const double Step : Steps)
		{
			if (Step >= MinMetres) { return Step; }
		}
		return 1.0e6;
	}
}

void SSpnMapView::Construct(const FArguments& InArgs)
{
	Mode = InArgs._Mode;
	Map = InArgs._Map;
	if (Mode == EMode::Mini)
	{
		DesiredSizePx = FVector2D(230.0, 230.0);
	}
}

void SSpnMapView::SetPlayer(const FVector2D& Metres, double BearingRadians)
{
	PlayerMetres = Metres;
	PlayerBearing = BearingRadians;
}

void SSpnMapView::CenterOnPlayer()
{
	CenterMetres = PlayerMetres;
}

void SSpnMapView::PanMetres(const FVector2D& DeltaMetres)
{
	CenterMetres += DeltaMetres;
	if (const USpnMapSubsystem* M = Map.Get())
	{
		const FSpnMapBounds B = M->GetBounds();
		CenterMetres.X = FMath::Clamp(CenterMetres.X, B.Min.X, B.Max.X);
		CenterMetres.Y = FMath::Clamp(CenterMetres.Y, B.Min.Y, B.Max.Y);
	}
}

void SSpnMapView::PanScreen(const FVector2D& DeltaPx)
{
	PanMetres(FVector2D(-DeltaPx.X, DeltaPx.Y) * MetresPerPixel);
}

void SSpnMapView::ZoomBy(double Factor)
{
	// bounded: ~1 cm per pixel up to a continent-sized view; the map data decides what is drawn
	MetresPerPixel = FMath::Clamp(MetresPerPixel / FMath::Max(Factor, 0.01), 0.02, 2000.0);
}

void SSpnMapView::FitToBounds()
{
	if (const USpnMapSubsystem* M = Map.Get())
	{
		const FSpnMapBounds B = M->GetBounds();
		const FVector2D Extent = B.Max - B.Min;
		CenterMetres = (B.Min + B.Max) * 0.5;
		MetresPerPixel = FMath::Max(Extent.X / FMath::Max(LastSize.X - 40.0, 100.0), Extent.Y / FMath::Max(LastSize.Y - 40.0, 100.0));
	}
}

FVector2D SSpnMapView::LocalToMap(const FVector2D& LocalPx) const
{
	const FVector2D Off = LocalPx - LastSize * 0.5;
	return CenterMetres + FVector2D(Off.X, -Off.Y) * MetresPerPixel;
}

FVector2D SSpnMapView::MapToLocal(const FVector2D& Metres) const
{
	const FVector2D D = (Metres - CenterMetres) / MetresPerPixel;
	return LastSize * 0.5 + FVector2D(D.X, -D.Y);
}

FReply SSpnMapView::OnMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& Event)
{
	if (Mode != EMode::Full)
	{
		return FReply::Unhandled();
	}
	LastSize = FVector2D(Geometry.GetLocalSize());
	const FVector2D Local = FVector2D(Geometry.AbsoluteToLocal(Event.GetScreenSpacePosition()));
	if (Event.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		bDragging = true;
		bDragMoved = false;
		DragStartLocal = LastDragLocal = Local;
		return FReply::Handled().CaptureMouse(AsShared());
	}
	if (Event.GetEffectingButton() == EKeys::RightMouseButton)
	{
		OnMapClick.ExecuteIfBound(LocalToMap(Local), true);
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

FReply SSpnMapView::OnMouseButtonUp(const FGeometry& Geometry, const FPointerEvent& Event)
{
	if (Mode == EMode::Full && Event.GetEffectingButton() == EKeys::LeftMouseButton && bDragging)
	{
		bDragging = false;
		const FVector2D Local = FVector2D(Geometry.AbsoluteToLocal(Event.GetScreenSpacePosition()));
		if (!bDragMoved)
		{
			OnMapClick.ExecuteIfBound(LocalToMap(Local), false);
		}
		return FReply::Handled().ReleaseMouseCapture();
	}
	return FReply::Unhandled();
}

FReply SSpnMapView::OnMouseMove(const FGeometry& Geometry, const FPointerEvent& Event)
{
	if (Mode == EMode::Full)
	{
		LastSize = FVector2D(Geometry.GetLocalSize());
		CursorMetres = LocalToMap(FVector2D(Geometry.AbsoluteToLocal(Event.GetScreenSpacePosition())));
	}
	if (Mode == EMode::Full && bDragging)
	{
		const FVector2D Local = FVector2D(Geometry.AbsoluteToLocal(Event.GetScreenSpacePosition()));
		if (!bDragMoved && FVector2D::Distance(Local, DragStartLocal) > 4.0)
		{
			bDragMoved = true;
		}
		if (bDragMoved)
		{
			PanScreen(Local - LastDragLocal);
		}
		LastDragLocal = Local;
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

FReply SSpnMapView::OnMouseWheel(const FGeometry& Geometry, const FPointerEvent& Event)
{
	if (Mode != EMode::Full)
	{
		return FReply::Unhandled();
	}
	LastSize = FVector2D(Geometry.GetLocalSize());
	const FVector2D Local = FVector2D(Geometry.AbsoluteToLocal(Event.GetScreenSpacePosition()));
	const FVector2D Before = LocalToMap(Local);
	ZoomBy(FMath::Pow(1.25, static_cast<double>(Event.GetWheelDelta())));
	const FVector2D After = LocalToMap(Local);
	PanMetres(Before - After); // the map point under the cursor stays under the cursor
	return FReply::Handled();
}

int32 SSpnMapView::OnPaint(const FPaintArgs& Args, const FGeometry& G, const FSlateRect& Cull, FSlateWindowElementList& Out, int32 Layer, const FWidgetStyle&, bool) const
{
	LastSize = FVector2D(G.GetLocalSize());
	Out.PushClip(FSlateClippingZone(G));
	const int32 Next = Mode == EMode::Full ? PaintFull(G, Out, Layer) : Mode == EMode::Mini ? PaintMini(G, Out, Layer) : PaintCompass(G, Out, Layer);
	Out.PopClip();
	return Next;
}

int32 SSpnMapView::PaintFull(const FGeometry& G, FSlateWindowElementList& Out, int32 L) const
{
	const USpnMapSubsystem* M = Map.Get();
	const FVector2D Size = LastSize;
	Box(Out, L, G, FVector2D::ZeroVector, Size, FLinearColor(0.020f, 0.026f, 0.031f, 1.f));
	if (M == nullptr || !M->HasDefinition())
	{
		Text(Out, L + 1, G, FVector2D(24, 24), FSpnUiText::Get(TEXT("ui.map.nodata")).ToString(), 16, SpnUi::TextDim);
		return L + 2;
	}
	const FSpnMapBounds B = M->GetBounds();

	// layer bounds (area with data) slightly lighter than the void around it
	const FVector2D BoundsA = MapToLocal(FVector2D(B.Min.X, B.Max.Y));
	const FVector2D BoundsB = MapToLocal(FVector2D(B.Max.X, B.Min.Y));
	Box(Out, L + 1, G, BoundsA, BoundsB - BoundsA, FLinearColor(0.040f, 0.050f, 0.058f, 1.f));

	// metre grid; spacing grows with the zoom-out so the line count stays small (a continent view draws the same ~20 lines)
	const double Step = NiceStep(MetresPerPixel * 64.0);
	const FVector2D ViewMin = LocalToMap(FVector2D(0, Size.Y)), ViewMax = LocalToMap(FVector2D(Size.X, 0));
	const FLinearColor GridColor(0.10f, 0.125f, 0.145f, 1.f), GridMajor(0.15f, 0.185f, 0.21f, 1.f);
	const bool bRoads = M->HasRoadNetwork();
	if (bRoads)
	{
		// roads, buildings, water and the route come from the Core (MapRenderFrame); nothing is drawn that the data does not contain
		FSpnMapRenderRequest Request;
		Request.CenterMetres = CenterMetres;
		Request.MetresPerPixel = MetresPerPixel;
		Request.HeadingBearingRadians = PlayerBearing;
		Request.HalfSizePx = Size * 0.5;
		RoadFrame = M->BuildMapRenderFrame(Request);
		if (RoadFrame.bValid) { DrawRoadFrame(Out, L + 1, G, RoadFrame, Size * 0.5, true); }
	}
	for (double X = FMath::CeilToDouble(FMath::Max(ViewMin.X, B.Min.X) / Step) * Step; X <= FMath::Min(ViewMax.X, B.Max.X) && !bRoads; X += Step)
	{
		const bool bMajor = FMath::IsNearlyZero(FMath::Fmod(X, Step * 5.0), 1e-6);
		Line(Out, L + 2, G, MapToLocal(FVector2D(X, FMath::Max(ViewMin.Y, B.Min.Y))), MapToLocal(FVector2D(X, FMath::Min(ViewMax.Y, B.Max.Y))), bMajor ? GridMajor : GridColor, 1.f);
	}
	for (double Y = FMath::CeilToDouble(FMath::Max(ViewMin.Y, B.Min.Y) / Step) * Step; Y <= FMath::Min(ViewMax.Y, B.Max.Y) && !bRoads; Y += Step)
	{
		const bool bMajor = FMath::IsNearlyZero(FMath::Fmod(Y, Step * 5.0), 1e-6);
		Line(Out, L + 2, G, MapToLocal(FVector2D(FMath::Max(ViewMin.X, B.Min.X), Y)), MapToLocal(FVector2D(FMath::Min(ViewMax.X, B.Max.X), Y)), bMajor ? GridMajor : GridColor, 1.f);
	}
	{
		TArray<FVector2D> Outline{BoundsA, FVector2D(BoundsB.X, BoundsA.Y), BoundsB, FVector2D(BoundsA.X, BoundsB.Y)};
		Poly(Out, L + 3, G, Outline, FLinearColor(0.25f, 0.33f, 0.38f, 1.f), 1.5f);
	}

	TArray<FSpnMapPoiView> Pois;
	TArray<FSpnMapMarkerView> Markers;
	M->QueryVisible(ViewMin - FVector2D(20, 20) * MetresPerPixel, ViewMax + FVector2D(20, 20) * MetresPerPixel, Pois, Markers);

	FSpnMapMarkerView Waypoint;
	const bool bHasWaypoint = M->GetWaypoint(Waypoint);
	const FVector2D PlayerLocal = MapToLocal(PlayerMetres);
	if (bHasWaypoint && !bRoads)
	{
		const FVector2D WpLocal = MapToLocal(Waypoint.Metres);
		const FVector2D Delta = WpLocal - PlayerLocal;
		const double Len = Delta.Size();
		const FLinearColor Orange(0.95f, 0.50f, 0.22f, 0.85f);
		const int32 Dashes = FMath::Clamp(static_cast<int32>(Len / 14.0), 0, 120);
		for (int32 I = 0; I < Dashes; ++I)
		{
			const double A = (I * 14.0) / Len, Bb = FMath::Min((I * 14.0 + 7.0) / Len, 1.0);
			Line(Out, L + 4, G, PlayerLocal + Delta * A, PlayerLocal + Delta * Bb, Orange, 2.f);
		}
	}

	for (const FSpnMapPoiView& Poi : Pois)
	{
		const FVector2D P = MapToLocal(Poi.Metres);
		const bool bSel = Poi.Id == SelectedPoi && !SelectedPoi.IsEmpty();
		const FSpnMapSymbol S = SpnMapStyle::SymbolFor(Poi.Category, false);
		if (bSel) { Dot(Out, L + 5, G, P, 13.f, FLinearColor::Transparent, SpnUi::Ring, 2.f); }
		Symbol(Out, L + 6, G, P, S, 6.f, Poi.Discovery == ESpnDiscovery::Visited);
		if (bSel || MetresPerPixel <= 0.7)
		{
			Text(Out, L + 7, G, P + FVector2D(10, -8), Poi.Label, 12, bSel ? SpnUi::Ring : SpnUi::TextDim, bSel);
		}
	}
	for (const FSpnMapMarkerView& Marker : Markers)
	{
		const FVector2D P = MapToLocal(Marker.Metres);
		const bool bSel = Marker.Id == SelectedMarker && SelectedMarker != 0;
		const FSpnMapSymbol S = SpnMapStyle::SymbolFor(Marker.Category, Marker.bPlayerWaypoint);
		if (bSel) { Dot(Out, L + 5, G, P, 14.f, FLinearColor::Transparent, SpnUi::Ring, 2.f); }
		Symbol(Out, L + 8, G, P, S, 7.f, true);
		Text(Out, L + 9, G, P + FVector2D(12, -9), Marker.Label, 12, S.Color, true);
	}

	Arrow(Out, L + 10, G, PlayerLocal, PlayerBearing, 11.f, FLinearColor(0.96f, 0.98f, 1.f, 1.f));

	if (bShowCrosshair)
	{
		const FVector2D C = Size * 0.5;
		const FLinearColor X(0.72f, 0.92f, 0.96f, 0.9f);
		Line(Out, L + 11, G, C + FVector2D(-14, 0), C + FVector2D(-5, 0), X, 1.5f);
		Line(Out, L + 11, G, C + FVector2D(5, 0), C + FVector2D(14, 0), X, 1.5f);
		Line(Out, L + 11, G, C + FVector2D(0, -14), C + FVector2D(0, -5), X, 1.5f);
		Line(Out, L + 11, G, C + FVector2D(0, 5), C + FVector2D(0, 14), X, 1.5f);
	}

	// scale bar and north marker
	const double Bar = NiceStep(MetresPerPixel * 80.0);
	const double BarPx = Bar / MetresPerPixel;
	const FVector2D BarA(20.0, Size.Y - 24.0);
	Line(Out, L + 12, G, BarA, BarA + FVector2D(BarPx, 0), SpnUi::Text, 2.f);
	Line(Out, L + 12, G, BarA + FVector2D(0, -4), BarA + FVector2D(0, 4), SpnUi::Text, 2.f);
	Line(Out, L + 12, G, BarA + FVector2D(BarPx, -4), BarA + FVector2D(BarPx, 4), SpnUi::Text, 2.f);
	Text(Out, L + 12, G, BarA + FVector2D(0, -22), Bar >= 1000.0 ? FString::Printf(TEXT("%g km"), Bar / 1000.0) : FString::Printf(TEXT("%g m"), Bar), 12, SpnUi::Text);
	Text(Out, L + 12, G, FVector2D(Size.X - 28.0, 12.0), TEXT("N"), 16, SpnUi::Text, true);
	Arrow(Out, L + 12, G, FVector2D(Size.X - 20.0, 42.0), 0.0, 8.f, SpnUi::TextDim);
	return L + 14;
}

int32 SSpnMapView::PaintMini(const FGeometry& G, FSlateWindowElementList& Out, int32 L) const
{
	const FVector2D Size = LastSize;
	const FVector2D C = Size * 0.5;
	const float R = static_cast<float>(FMath::Min(Size.X, Size.Y) * 0.5 - 3.0);
	if (RoadFrame.bValid)
	{
		// road minimap: a square viewport (the Core clips the route to the same rectangle); drawn instead of the disc
		Box(Out, L, G, C - FVector2D(R, R), FVector2D(R * 2.f, R * 2.f), FLinearColor(0.022f, 0.030f, 0.036f, 0.92f));
		DrawRoadFrame(Out, L + 1, G, RoadFrame, C, false);
		TArray<FVector2D> Frame4{C + FVector2D(-R, -R), C + FVector2D(R, -R), C + FVector2D(R, R), C + FVector2D(-R, R)};
		Poly(Out, L + 6, G, Frame4, FLinearColor(0.34f, 0.46f, 0.52f, 1.f), 2.f);
		if (Frame.bValid)
		{
			for (const FSpnMinimapEntry& E : Frame.Entries)
			{
				if (!E.bPlayerWaypoint) { continue; }
				const FSpnMapSymbol S = SpnMapStyle::SymbolFor(E.Category, true);
				if (E.bOffscreen)
				{
					const double Angle = FMath::Atan2(E.Screen.X, -E.Screen.Y);
					TArray<FVector2D> Pts{C + E.Screen + Rotate(FVector2D(0, -6), Angle), C + E.Screen + Rotate(FVector2D(5, 4), Angle), C + E.Screen + Rotate(FVector2D(-5, 4), Angle)};
					Poly(Out, L + 7, G, Pts, S.Color, 3.f);
				}
				else
				{
					Symbol(Out, L + 7, G, C + E.Screen, S, 5.f, true);
				}
			}
		}
		Arrow(Out, L + 8, G, C, RoadFrame.PlayerHeadingOnScreenRadians, 8.f, FLinearColor(0.96f, 0.98f, 1.f, 1.f));
		const FVector2D NPos2 = C + Rotate(FVector2D(0, -(R - 11.0)), RoadFrame.NorthOnScreenRadians);
		Text(Out, L + 9, G, NPos2 - FVector2D(4.0, 8.0), TEXT("N"), 12, FLinearColor(0.95f, 0.55f, 0.35f, 1.f), true);
		return L + 10;
	}
	Dot(Out, L, G, C, R, FLinearColor(0.022f, 0.030f, 0.036f, 0.86f), FLinearColor(0.34f, 0.46f, 0.52f, 1.f), 2.f);
	Dot(Out, L + 1, G, C, R * 0.5f, FLinearColor::Transparent, FLinearColor(0.12f, 0.16f, 0.19f, 0.8f), 1.f);
	Line(Out, L + 1, G, C + FVector2D(-R + 4, 0), C + FVector2D(R - 4, 0), FLinearColor(0.12f, 0.16f, 0.19f, 0.7f), 1.f);
	Line(Out, L + 1, G, C + FVector2D(0, -R + 4), C + FVector2D(0, R - 4), FLinearColor(0.12f, 0.16f, 0.19f, 0.7f), 1.f);

	if (Frame.bValid)
	{
		for (const FSpnMinimapEntry& E : Frame.Entries)
		{
			const FSpnMapSymbol S = SpnMapStyle::SymbolFor(E.Category, E.bPlayerWaypoint);
			if (E.bOffscreen)
			{
				// an edge indicator points outwards (towards the target)
				const double Angle = FMath::Atan2(E.Screen.X, -E.Screen.Y);
				TArray<FVector2D> Pts{C + E.Screen + Rotate(FVector2D(0, -6), Angle), C + E.Screen + Rotate(FVector2D(5, 4), Angle), C + E.Screen + Rotate(FVector2D(-5, 4), Angle)};
				Poly(Out, L + 3, G, Pts, S.Color, E.bPlayerWaypoint ? 3.f : 1.5f);
			}
			else
			{
				Symbol(Out, L + 4, G, C + E.Screen, S, E.bPlayerWaypoint ? 5.f : 3.5f, true);
			}
		}
	}
	// the player is always the centre; north-up rotates the arrow, heading-up keeps it pointing up
	Arrow(Out, L + 6, G, C, bHeadingUp ? 0.0 : PlayerBearing, 8.f, FLinearColor(0.96f, 0.98f, 1.f, 1.f));
	const double North = Frame.bValid ? Frame.NorthOnScreenRadians : 0.0;
	const FVector2D NPos = C + Rotate(FVector2D(0, -(R - 11.0)), North);
	Text(Out, L + 7, G, NPos - FVector2D(4.0, 8.0), TEXT("N"), 12, FLinearColor(0.95f, 0.55f, 0.35f, 1.f), true);
	return L + 8;
}

void USpnMapViewWidget::Configure(SSpnMapView::EMode InMode, const FVector2D& DesiredSize)
{
	Mode = InMode;
	Size = DesiredSize;
	if (View.IsValid())
	{
		View->SetDesiredSizePx(Size);
	}
}

TSharedRef<SWidget> USpnMapViewWidget::RebuildWidget()
{
	View = SNew(SSpnMapView).Mode(Mode).Map(MapRef);
	View->OnMapClick = OnMapClick;
	View->SetDesiredSizePx(Size);
	return View.ToSharedRef();
}

void USpnMapViewWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	View.Reset();
}

int32 SSpnMapView::PaintCompass(const FGeometry& G, FSlateWindowElementList& Out, int32 L) const
{
	const FVector2D Size = LastSize;
	const double Span = 180.0; // degrees shown (+-90 around the heading)
	const double PxPerDeg = Size.X / Span;
	Box(Out, L, G, FVector2D(0, 8), FVector2D(Size.X, Size.Y - 16.0), FLinearColor(0.02f, 0.028f, 0.034f, 0.62f));
	const double HeadingDeg = FMath::RadiansToDegrees(PlayerBearing);
	auto XFor = [&](double BearingDeg, bool* bInside = nullptr)
	{
		double Rel = FMath::Fmod(BearingDeg - HeadingDeg + 540.0, 360.0) - 180.0; // (-180, 180]
		if (bInside != nullptr) { *bInside = FMath::Abs(Rel) <= Span * 0.5; }
		return Size.X * 0.5 + Rel * PxPerDeg;
	};
	const double Mid = Size.Y * 0.5;
	for (int32 Deg = 0; Deg < 360; Deg += 15)
	{
		bool bInside = false;
		const double X = XFor(Deg, &bInside);
		if (!bInside) { continue; }
		const bool bCardinal = Deg % 90 == 0;
		const bool bMinor = Deg % 45 == 0;
		Line(Out, L + 1, G, FVector2D(X, Mid + 6), FVector2D(X, Mid + (bCardinal ? 17 : bMinor ? 14 : 10)), SpnUi::TextDim, 1.f);
		if (bCardinal || bMinor)
		{
			static const TCHAR* Names[] = {TEXT("N"), TEXT("NE"), TEXT("E"), TEXT("SE"), TEXT("S"), TEXT("SW"), TEXT("W"), TEXT("NW")};
			const FString Name = Names[Deg / 45];
			const FVector2D Measured = MeasureText(Name, bCardinal ? 17 : 12, bCardinal);
			Text(Out, L + 2, G, FVector2D(X - Measured.X * 0.5, 11.0), Name, bCardinal ? 17 : 12, Deg == 0 ? FLinearColor(0.95f, 0.55f, 0.35f, 1.f) : SpnUi::Text, bCardinal);
		}
	}
	// own heading caret and degree read-out
	TArray<FVector2D> Caret{FVector2D(Size.X * 0.5 - 5, 1), FVector2D(Size.X * 0.5 + 5, 1), FVector2D(Size.X * 0.5, 8)};
	Poly(Out, L + 3, G, Caret, SpnUi::Ring, 2.f);
	Text(Out, L + 3, G, FVector2D(Size.X * 0.5 + 12.0, 1.0), FString::Printf(TEXT("%03d°"), FMath::RoundToInt(HeadingDeg) % 360), 11, SpnUi::TextDim);
	if (Navigation.bActive)
	{
		const double TargetDeg = FMath::RadiansToDegrees(Navigation.BearingRadians);
		bool bInside = false;
		double X = XFor(TargetDeg, &bInside);
		const FLinearColor Orange(0.95f, 0.50f, 0.22f, 1.f);
		X = FMath::Clamp(X, 14.0, Size.X - 14.0);
		TArray<FVector2D> Diamond{FVector2D(X, Mid - 11), FVector2D(X + 8, Mid), FVector2D(X, Mid + 11), FVector2D(X - 8, Mid)};
		Poly(Out, L + 4, G, Diamond, Orange, bInside ? 3.f : 1.5f);
		const FString Dist = Navigation.bArrived ? FString(TEXT("✓")) : (Navigation.DistanceMetres >= 1000.0 ? FString::Printf(TEXT("%.1f km"), Navigation.DistanceMetres / 1000.0) : FString::Printf(TEXT("%d m"), FMath::RoundToInt(Navigation.DistanceMetres)));
		const FVector2D Measured = MeasureText(Dist, 12, true);
		Text(Out, L + 5, G, FVector2D(FMath::Clamp(X - Measured.X * 0.5, 2.0, Size.X - Measured.X - 2.0), Mid + 12.0), Dist, 12, Orange, true);
	}
	return L + 6;
}

void USpnMapViewWidget::SetMap(USpnMapSubsystem* InMap)
{
	MapRef = InMap;
	if (View.IsValid())
	{
		View->SetMap(InMap);
	}
}
