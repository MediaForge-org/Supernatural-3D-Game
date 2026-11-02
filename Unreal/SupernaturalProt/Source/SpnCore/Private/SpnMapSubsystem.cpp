#include "SpnMapSubsystem.h"
#include "SpnMapInternal.h"
#include "SpnCoreModule.h"

#include <cmath>
#include <string>
#include <utility>

namespace
{
	using namespace spn::core;

	ContentId Cid(const char* Text)
	{
		auto Parsed = ContentId::Parse(Text);
		check(Parsed.has_value());
		return *Parsed;
	}

	FString ToF(const std::string& Utf8)
	{
		return FString(FUTF8ToTCHAR(Utf8.c_str(), static_cast<int32>(Utf8.size())));
	}

	std::string ToUtf8(const FString& Text)
	{
		const FTCHARToUTF8 Converted(*Text);
		return std::string(Converted.Get(), static_cast<size_t>(Converted.Length()));
	}

	/** Labels: no control characters, at most 100 bytes of UTF-8 on a character boundary (the Core allows 128). */
	std::string SanitizeLabel(const FString& In, const char* Fallback)
	{
		FString Clean;
		for (const TCHAR C : In)
		{
			if (C >= 32 && C != 127)
			{
				Clean.AppendChar(C);
			}
		}
		Clean.TrimStartAndEndInline();
		while (Clean.Len() > 0 && ToUtf8(Clean).size() > 100)
		{
			Clean.LeftChopInline(1);
		}
		return Clean.IsEmpty() ? std::string(Fallback) : ToUtf8(Clean);
	}

	ESpnDiscovery ToDiscovery(DiscoveryState State)
	{
		switch (State)
		{
		case DiscoveryState::Known: return ESpnDiscovery::Known;
		case DiscoveryState::Visited: return ESpnDiscovery::Visited;
		default: return ESpnDiscovery::Undiscovered;
		}
	}

	ESpnMapResult FromMapError(MapErrorCode Code)
	{
		switch (Code)
		{
		case MapErrorCode::OutsideLayerBounds:
		case MapErrorCode::InvalidPosition: return ESpnMapResult::OutOfBounds;
		case MapErrorCode::UnknownEntry:
		case MapErrorCode::UnknownMarker: return ESpnMapResult::UnknownTarget;
		default: return ESpnMapResult::Rejected;
		}
	}

	double NormalizeSigned(double Radians)
	{
		double Wrapped = std::fmod(Radians, 2.0 * PI);
		if (Wrapped > PI) { Wrapped -= 2.0 * PI; }
		if (Wrapped <= -PI) { Wrapped += 2.0 * PI; }
		return Wrapped;
	}

	spn::core::TravelMode ToCoreMode(ESpnTravelMode Mode)
	{
		return Mode == ESpnTravelMode::Driving ? spn::core::TravelMode::Driving : spn::core::TravelMode::OnFoot;
	}

	ESpnNavStatus ToNavStatus(NavigationStatus Status)
	{
		switch (Status)
		{
		case NavigationStatus::Active: return ESpnNavStatus::Active;
		case NavigationStatus::Arrived: return ESpnNavStatus::Arrived;
		case NavigationStatus::NoRoute: return ESpnNavStatus::NoRoute;
		case NavigationStatus::Cancelled: return ESpnNavStatus::Cancelled;
		default: return ESpnNavStatus::Idle;
		}
	}

	/** The vertices of a route: first segment start, then every segment end (layer transitions have none in this build). */
	std::vector<Vec2> RoutePoints(const Route& In)
	{
		std::vector<Vec2> Points;
		for (const RouteSegment& Segment : In.segments)
		{
			if (Segment.kind != SegmentKind::Travel)
			{
				continue;
			}
			if (Points.empty())
			{
				Points.push_back(Segment.from.xy);
			}
			Points.push_back(Segment.to.xy);
		}
		return Points;
	}

	FSpnMapMarkerView ToView(const Marker& M)
	{
		FSpnMapMarkerView View;
		View.Id = static_cast<int64>(M.id.Value());
		View.Category = ToF(M.category.Value());
		View.Label = ToF(M.label);
		View.Metres = FVector2D(M.position.xy.x, M.position.xy.y);
		View.bPlayerWaypoint = M.category == PlayerWaypointCategory();
		View.LinkedPoi = ToF(M.linkedPoi.Value());
		return View;
	}

	// Test district, mirrors the authored test level (Tools/spn_district_layout.py). Unreal centimetres; PLACEHOLDER names.
	struct FTestPoi { const char* Id; const char* Category; const char* Label; double XCm; double YCm; MapVisibility Visibility; };
	constexpr FTestPoi TestPois[] = {
		{"spn_test:poi.player_start", "spn_test:marker.landmark", "Start (test)", -10550.0, -4700.0, MapVisibility::Always},
		{"spn_test:poi.birkenweg", "spn_test:marker.street", "Birkenweg (test)", -10200.0, -7000.0, MapVisibility::WhenKnown},
		{"spn_test:poi.storage_road", "spn_test:marker.street", "Storage Road (test)", -3000.0, -7000.0, MapVisibility::WhenKnown},
		{"spn_test:poi.lagerstrasse", "spn_test:marker.street", "Lagerstrasse (test)", 6000.0, -10500.0, MapVisibility::WhenKnown},
		{"spn_test:poi.kurvenweg", "spn_test:marker.street", "Kurvenweg (test)", -8000.0, -12400.0, MapVisibility::WhenKnown},
		{"spn_test:poi.workshop_lot", "spn_test:marker.building", "Workshop lot (test)", 9800.0, -11900.0, MapVisibility::WhenKnown},
		{"spn_test:poi.lindenweg_end", "spn_test:marker.street", "Lindenweg end (test)", -7000.0, 11000.0, MapVisibility::WhenKnown},
		{"spn_test:poi.hofweg_end", "spn_test:marker.street", "Hofweg end (test)", 4600.0, 9600.0, MapVisibility::WhenKnown},
		{"spn_test:poi.east_lot", "spn_test:marker.building", "East lot (test)", 12700.0, 10500.0, MapVisibility::WhenKnown},
	};
}

const TCHAR* LexToString(ESpnMapResult Result)
{
	switch (Result)
	{
	case ESpnMapResult::Ok: return TEXT("Ok");
	case ESpnMapResult::NoDefinition: return TEXT("NoDefinition");
	case ESpnMapResult::OutOfBounds: return TEXT("OutOfBounds");
	case ESpnMapResult::UnknownTarget: return TEXT("UnknownTarget");
	case ESpnMapResult::Rejected: return TEXT("Rejected");
	case ESpnMapResult::InternalError: return TEXT("InternalError");
	}
	return TEXT("Unknown");
}

const TCHAR* LexToString(ESpnNavStatus Status)
{
	switch (Status)
	{
	case ESpnNavStatus::Idle: return TEXT("Idle");
	case ESpnNavStatus::Active: return TEXT("Active");
	case ESpnNavStatus::Arrived: return TEXT("Arrived");
	case ESpnNavStatus::NoRoute: return TEXT("NoRoute");
	case ESpnNavStatus::Cancelled: return TEXT("Cancelled");
	}
	return TEXT("Unknown");
}

USpnMapSubsystem::USpnMapSubsystem() = default;
USpnMapSubsystem::~USpnMapSubsystem() = default;

void USpnMapSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	State = MakePimpl<FState>();
}

void USpnMapSubsystem::Deinitialize()
{
	State.Reset();
	Super::Deinitialize();
}

ESpnMapResult USpnMapSubsystem::InstallTestDefinition(const FSpnWorldFrame& InFrame)
{
	if (!State.IsValid())
	{
		return ESpnMapResult::InternalError;
	}
	Frame = InFrame;

	MapLayerDef Layer;
	Layer.id = MapLayerId(1);
	Layer.key = Cid("spn_test:layer.surface");
	Layer.kind = MapLayerKind::Surface;
	Layer.bounds = MapRect{{-200.0, -200.0}, {200.0, 200.0}};

	MapRegionDef Region;
	Region.id = Cid("spn_test:region.district");
	Region.layer = Layer.id;
	Region.bounds = MapRect{{-160.0, -160.0}, {160.0, 160.0}};
	Region.category = Cid("spn_test:region.generic");
	Region.label = "Test district (placeholder)";

	std::vector<PointOfInterestDef> Pois;
	for (const FTestPoi& Test : TestPois)
	{
		FVector World;
		if (!SpnWorldFrame::ToWorld(Frame, FVector(Test.XCm, Test.YCm, 0.0), World))
		{
			return ESpnMapResult::InternalError;
		}
		PointOfInterestDef Poi;
		Poi.id = Cid(Test.Id);
		Poi.category = Cid(Test.Category);
		Poi.position = MapPosition{Layer.id, Vec2{World.X, World.Y}};
		Poi.label = Test.Label;
		Poi.visibility = Test.Visibility;
		Pois.push_back(std::move(Poi));
	}

	auto Definition = WorldMapDefinition::Create({Layer}, {Region}, std::move(Pois), 64.0);
	if (!Definition)
	{
		UE_LOG(LogSpnCore, Error, TEXT("Map: test definition rejected: %s"), *ToF(std::string(ToString(Definition.Error().code))));
		return ESpnMapResult::Rejected;
	}
	State->Definition = std::make_shared<const WorldMapDefinition>(std::move(Definition.Value()));
	State->SurfaceLayer = Layer.id;
	State->Map.emplace(State->Definition);
	State->Geometry.reset();
	State->Region.reset();
	State->Nav.reset();
	State->Closed.clear();
	State->LastPlayer.reset();
	State->LastUpdate = NavigationUpdate{};
	bSyntheticRegion = false;
	bTestDefinition = true;
	ResetStateInternal();
	BroadcastReplaced();
	UE_LOG(LogSpnCore, Log, TEXT("Map: installed the TEMPORARY test definition (%d POIs, placeholder, not geography)"), static_cast<int32>(State->Definition->Pois().size()));
	return ESpnMapResult::Ok;
}

ESpnMapResult USpnMapSubsystem::InstallSyntheticRegion(const FSpnWorldFrame& InFrame)
{
	if (!State.IsValid())
	{
		return ESpnMapResult::InternalError;
	}
	Frame = InFrame;
	synthetic::SyntheticMapRegion Region = synthetic::MakeSyntheticMapRegion();
	State->Definition = Region.definition;
	State->SurfaceLayer = Region.layer;
	State->Map.emplace(State->Definition);
	State->Geometry = Region.geometry;
	State->Region = std::move(Region);
	State->Closed.clear();
	State->LastPlayer.reset();
	State->LastUpdate = NavigationUpdate{};
	State->Mode = ToCoreMode(TravelMode);
	NavigationConfig Config;
	Config.options.mode = State->Mode;
	State->Nav = std::make_unique<NavigationSession>(State->Geometry, Config);
	bTestDefinition = true; // still temporary test data
	bSyntheticRegion = true;
	ResetStateInternal();
	BroadcastReplaced();
	UE_LOG(LogSpnCore, Log, TEXT("Map: installed the SYNTHETIC test region (road routing active; not geography, not WorldForge data)"));
	return ESpnMapResult::Ok;
}

double USpnMapSubsystem::NowSeconds() const
{
	return FPlatformTime::Seconds() + ClockSkew;
}

bool USpnMapSubsystem::HasRoadNetwork() const
{
	return HasDefinition() && State->Nav != nullptr && State->Geometry != nullptr;
}

bool USpnMapSubsystem::GetSyntheticPosition(const FString& Key, FVector2D& OutMetres) const
{
	if (!bSyntheticRegion || !State.IsValid() || !State->Region.has_value())
	{
		return false;
	}
	const auto& R = *State->Region;
	const MapPosition* Position = nullptr;
	if (Key == TEXT("westStreet")) { Position = &R.westStreet; }
	else if (Key == TEXT("eastStreet")) { Position = &R.eastStreet; }
	else if (Key == TEXT("westOpenField")) { Position = &R.westOpenField; }
	else if (Key == TEXT("shopInside")) { Position = &R.shopInside; }
	else if (Key == TEXT("houseInside")) { Position = &R.houseInside; }
	else if (Key == TEXT("noEntranceInside")) { Position = &R.noEntranceInside; }
	else if (Key == TEXT("warehouseInside")) { Position = &R.warehouseInside; }
	else if (Key == TEXT("islandTarget")) { Position = &R.islandTarget; }
	else if (Key == TEXT("hamletTarget")) { Position = &R.hamletTarget; }
	if (Position == nullptr)
	{
		return false;
	}
	OutMetres = FVector2D(Position->xy.x, Position->xy.y);
	return true;
}

void USpnMapSubsystem::SetTravelMode(ESpnTravelMode InMode)
{
	TravelMode = InMode;
	if (!HasRoadNetwork())
	{
		return;
	}
	State->Mode = ToCoreMode(InMode);
	// the session's configuration is fixed at construction: a new session, re-planned to the same waypoint
	NavigationConfig Config;
	Config.options.mode = State->Mode;
	Config.options.closedSegments = State->Closed;
	State->Nav = std::make_unique<NavigationSession>(State->Geometry, Config);
	State->LastUpdate = NavigationUpdate{};
	FSpnMapMarkerView Waypoint;
	if (State->LastPlayer.has_value() && GetWaypoint(Waypoint))
	{
		auto Destination = DestinationFromMarker(*State->Map, MarkerId(static_cast<uint64>(Waypoint.Id)), ArrivalRadiusMetres);
		if (Destination)
		{
			State->LastUpdate = State->Nav->SetDestination(*State->LastPlayer, Destination.Value(), NowSeconds());
		}
	}
	OnMapChanged.Broadcast();
}

bool USpnMapSubsystem::GetRoute(FSpnRouteView& Out) const
{
	Out = FSpnRouteView();
	if (!HasRoadNetwork())
	{
		return false;
	}
	const PlannedRoute* Route = State->Nav->CurrentRoute();
	if (Route == nullptr)
	{
		return false;
	}
	Out.bValid = true;
	Out.Generation = State->Nav->Generation();
	Out.LengthMetres = Route->lengthMetres;
	Out.RoadLengthMetres = Route->roadLengthMetres;
	Out.ConnectorLengthMetres = Route->connectorLengthMetres;
	Out.StraightLineMetres = Route->straightLineMetres;
	for (const Vec2& Point : RoutePoints(Route->route))
	{
		Out.Points.Add(FVector2D(Point.x, Point.y));
	}
	for (const RouteLeg& Leg : Route->legs)
	{
		if (Leg.kind == LegKind::Road)
		{
			const uint64 Id = Leg.road.Value();
			if (Out.RoadSegmentIds.Num() == 0 || Out.RoadSegmentIds.Last() != Id)
			{
				Out.RoadSegmentIds.Add(Id);
			}
		}
	}
	return true;
}

uint64 USpnMapSubsystem::GetRouteGeneration() const
{
	return HasRoadNetwork() ? State->Nav->Generation() : 0;
}

bool USpnMapSubsystem::SetRoadClosed(uint64 SegmentId, bool bClosed)
{
	if (!HasRoadNetwork() || !State->Geometry->Roads().SegmentIndex(RoadSegmentId(SegmentId)).has_value())
	{
		return false;
	}
	const RoadSegmentId Id(SegmentId);
	if (bClosed)
	{
		State->Closed.insert(Id);
	}
	else
	{
		State->Closed.erase(Id);
	}
	const MapPosition Player = State->LastPlayer.value_or(State->Region->westStreet);
	State->LastUpdate = State->Nav->SetClosedSegments(State->Closed, Player, NowSeconds());
	OnMapChanged.Broadcast();
	return true;
}

bool USpnMapSubsystem::IsRoadClosed(uint64 SegmentId) const
{
	return HasRoadNetwork() && State->Closed.count(RoadSegmentId(SegmentId)) > 0;
}

TArray<uint64> USpnMapSubsystem::GetClosedRoads() const
{
	TArray<uint64> Out;
	if (HasRoadNetwork())
	{
		for (const RoadSegmentId& Id : State->Closed)
		{
			Out.Add(Id.Value());
		}
	}
	return Out;
}

bool USpnMapSubsystem::FindRoadAt(const FVector2D& Metres, double RadiusMetres, uint64& OutSegmentId) const
{
	if (!HasRoadNetwork())
	{
		return false;
	}
	const MapPosition Position{State->SurfaceLayer, Vec2{Metres.X, Metres.Y}};
	for (const spn::core::TravelMode Mode : {spn::core::TravelMode::Driving, spn::core::TravelMode::OnFoot})
	{
		SnapRules Rules;
		Rules.mode = Mode;
		Rules.allowEnclosed = true;
		const auto Snaps = State->Geometry->Roads().Snap(Position, RadiusMetres, Rules, 1);
		if (!Snaps.empty())
		{
			OutSegmentId = Snaps.front().segment.Value();
			return true;
		}
	}
	return false;
}

FSpnMapRenderFrame USpnMapSubsystem::BuildMapRenderFrame(const FSpnMapRenderRequest& Request) const
{
	FSpnMapRenderFrame Out;
	if (!HasRoadNetwork() || !FMath::IsFinite(Request.CenterMetres.X) || !FMath::IsFinite(Request.CenterMetres.Y))
	{
		return Out;
	}
	MinimapConfig Viewport;
	Viewport.shape = Request.bCircle ? MinimapShape::Circle : MinimapShape::Rectangle;
	Viewport.halfWidthPx = Request.HalfSizePx.X;
	Viewport.halfHeightPx = Request.bCircle ? Request.HalfSizePx.X : Request.HalfSizePx.Y;
	Viewport.baseMetresPerPixel = Request.MetresPerPixel;
	MinimapView View;
	View.player = MapPosition{State->SurfaceLayer, Vec2{Request.CenterMetres.X, Request.CenterMetres.Y}};
	View.headingRadians = Request.HeadingBearingRadians;
	View.zoom = 1.0;
	View.rotation = Request.bHeadingUp ? MinimapRotation::HeadingUp : MinimapRotation::NorthUp;
	MapRenderConfig Config;
	Config.respectDiscovery = Request.bRespectDiscovery;
	RouteOverlay Overlay;
	if (const PlannedRoute* Route = State->Nav->CurrentRoute();
		Route != nullptr && (State->Nav->Status() == NavigationStatus::Active || State->Nav->Status() == NavigationStatus::Arrived))
	{
		Overlay.route = &Route->route;
		Overlay.segmentIndex = State->LastUpdate.segmentIndex;
		Overlay.alongSegmentMetres = State->LastUpdate.alongSegmentMetres;
	}
	auto Built = spn::core::BuildMapRenderFrame(*State->Geometry, &*State->Map, View, Viewport, Config, Overlay);
	if (!Built)
	{
		return Out;
	}
	const MapRenderFrame& Core = Built.Value();
	auto ToLine = [this](const RenderPolyline& In, bool bRoad)
	{
		FSpnRenderPolyline Line;
		Line.Id = In.id;
		Line.Kind = bRoad ? static_cast<uint8>(In.roadClass) : static_cast<uint8>(In.lineKind);
		Line.Structure = static_cast<uint8>(In.structure);
		Line.bOneWay = In.oneWay;
		Line.bRoundabout = In.roundabout;
		Line.bClosed = bRoad && State->Closed.count(RoadSegmentId(In.id)) > 0;
		Line.Points.Reserve(static_cast<int32>(In.points.size()));
		for (const Vec2& P : In.points) { Line.Points.Add(FVector2D(P.x, P.y)); }
		return Line;
	};
	auto ToPoly = [](const RenderPolygon& In)
	{
		FSpnRenderPolygon Poly;
		Poly.Id = In.id;
		Poly.Kind = static_cast<uint8>(In.areaKind);
		Poly.bBuilding = In.isBuilding;
		for (const Vec2& P : In.points) { Poly.Points.Add(FVector2D(P.x, P.y)); }
		for (const Vec2& P : In.entrances) { Poly.Entrances.Add(FVector2D(P.x, P.y)); }
		return Poly;
	};
	Out.bValid = true;
	Out.bTruncated = Core.stats.truncated;
	Out.MetresPerPixel = Core.metresPerPixel;
	Out.NorthOnScreenRadians = Core.northOnScreenRadians;
	Out.PlayerHeadingOnScreenRadians = Core.playerHeadingOnScreenRadians;
	for (const RenderPolygon& A : Core.areas) { Out.Areas.Add(ToPoly(A)); }
	for (const RenderPolyline& L : Core.lines) { Out.Lines.Add(ToLine(L, false)); }
	for (const RenderPolygon& B : Core.buildings) { Out.Buildings.Add(ToPoly(B)); }
	for (const RenderPolyline& R : Core.roads) { Out.Roads.Add(ToLine(R, true)); }
	auto CopyRoute = [](const std::vector<std::vector<Vec2>>& In, TArray<TArray<FVector2D>>& OutLines)
	{
		for (const auto& Line : In)
		{
			TArray<FVector2D>& Dest = OutLines.AddDefaulted_GetRef();
			for (const Vec2& P : Line) { Dest.Add(FVector2D(P.x, P.y)); }
		}
	};
	CopyRoute(Core.routeTravelled, Out.RouteTravelled);
	CopyRoute(Core.routeRemaining, Out.RouteRemaining);
	if (Core.destinationScreen)
	{
		Out.bHasDestination = true;
		Out.DestinationScreen = FVector2D(Core.destinationScreen->x, Core.destinationScreen->y);
		Out.bDestinationInside = Core.destinationInside;
	}
	if (Core.destinationIndicator)
	{
		Out.bHasDestinationIndicator = true;
		Out.DestinationIndicator = FVector2D(Core.destinationIndicator->x, Core.destinationIndicator->y);
	}
	Out.Candidates = static_cast<int32>(Core.stats.candidates);
	Out.Emitted = static_cast<int32>(Core.stats.emitted);
	return Out;
}

FSpnGeometryView USpnMapSubsystem::GetGeometryView() const
{
	FSpnGeometryView Out;
	if (!HasRoadNetwork())
	{
		return Out;
	}
	Out.bValid = true;
	const RoadNetwork& Net = State->Geometry->Roads();
	for (std::uint32_t I = 0; I < Net.Segments().size(); ++I)
	{
		const RoadSegmentDef& Def = Net.Segments()[I];
		const auto& Info = Net.Info(I);
		FSpnGeoRoad Road;
		Road.Id = Def.id.Value();
		Road.Class = static_cast<uint8>(Info.roadClass);
		Road.Structure = static_cast<uint8>(Info.structure);
		Road.bOneWay = Info.direction != RoadDirection::Both;
		Road.bRoundabout = Info.roundabout;
		for (const Vec2& P : Info.points) { Road.Points.Add(FVector2D(P.x, P.y)); }
		Out.Roads.Add(MoveTemp(Road));
	}
	for (const BuildingDef& B : State->Geometry->Buildings())
	{
		FSpnGeoBuilding Building;
		Building.Id = B.id.Value();
		Building.Category = ToF(B.category.Value());
		Building.Label = ToF(B.label);
		Building.HeightMetres = B.heightMetres.value_or(0.0);
		for (const Vec2& P : B.footprint) { Building.Footprint.Add(FVector2D(P.x, P.y)); }
		for (const Vec2& P : B.entrances) { Building.Entrances.Add(FVector2D(P.x, P.y)); }
		Out.Buildings.Add(MoveTemp(Building));
	}
	for (const AreaDef& A : State->Geometry->Areas())
	{
		FSpnGeoArea Area;
		Area.Id = A.id.Value();
		Area.Kind = static_cast<uint8>(A.kind);
		Area.Label = ToF(A.label);
		Area.ElevationMetres = A.elevationMetres.value_or(0.0);
		for (const Vec2& P : A.outline) { Area.Outline.Add(FVector2D(P.x, P.y)); }
		Out.Areas.Add(MoveTemp(Area));
	}
	return Out;
}

bool USpnMapSubsystem::PlanRouteLength(const FVector2D& FromMetres, const FVector2D& ToMetres, double& OutLength, FString& OutFailure) const
{
	OutLength = 0.0;
	OutFailure.Reset();
	if (!HasRoadNetwork())
	{
		OutFailure = TEXT("NoRoadNetwork");
		return false;
	}
	RouteOptions Options;
	Options.mode = State->Mode;
	Options.closedSegments = State->Closed;
	auto Destination = DestinationFromPosition(*State->Map, MapPosition{State->SurfaceLayer, Vec2{ToMetres.X, ToMetres.Y}}, ArrivalRadiusMetres);
	if (!Destination)
	{
		OutFailure = ToF(std::string(ToString(Destination.Error().code)));
		return false;
	}
	auto Planned = PlanRoute(*State->Geometry, MapPosition{State->SurfaceLayer, Vec2{FromMetres.X, FromMetres.Y}}, Destination.Value(), Options);
	if (!Planned)
	{
		OutFailure = ToF(std::string(ToString(Planned.Error().code)));
		return false;
	}
	OutLength = Planned.Value().lengthMetres;
	return true;
}

bool USpnMapSubsystem::HasDefinition() const
{
	return State.IsValid() && State->Map.has_value();
}

FSpnMapBounds USpnMapSubsystem::GetBounds() const
{
	FSpnMapBounds Bounds;
	if (HasDefinition())
	{
		if (const MapLayerDef* Layer = State->Definition->FindLayer(State->SurfaceLayer))
		{
			Bounds.Min = FVector2D(Layer->bounds.min.x, Layer->bounds.min.y);
			Bounds.Max = FVector2D(Layer->bounds.max.x, Layer->bounds.max.y);
		}
	}
	return Bounds;
}

void USpnMapSubsystem::Clear()
{
	if (State.IsValid())
	{
		State->Map.reset();
		State->Definition.reset();
		State->Geometry.reset();
		State->Region.reset();
		State->Nav.reset();
		State->Closed.clear();
		State->LastPlayer.reset();
		State->LastUpdate = NavigationUpdate{};
	}
	bSyntheticRegion = false;
	bTestDefinition = false;
	ResetStateInternal();
	BroadcastReplaced();
}

void USpnMapSubsystem::ResetStateInternal()
{
	ArrivalAnnouncedFor = 0;
	RegionLabel.Reset();
	LastObserved = FVector2D(1e18, 1e18);
}

void USpnMapSubsystem::BroadcastReplaced()
{
	OnMapReplaced.Broadcast();
	OnMapChanged.Broadcast();
}

bool USpnMapSubsystem::UnrealToMap(const FVector& LocationCm, FVector2D& OutMetres) const
{
	FVector World;
	if (!SpnWorldFrame::ToWorld(Frame, LocationCm, World))
	{
		return false;
	}
	OutMetres = FVector2D(World.X, World.Y);
	return true;
}

bool USpnMapSubsystem::MapToUnreal(const FVector2D& Metres, float ZCm, FVector& OutLocationCm) const
{
	FVector Unreal;
	if (!SpnWorldFrame::ToUnreal(Frame, FVector(Metres.X, Metres.Y, 0.0), Unreal))
	{
		return false;
	}
	Unreal.Z = ZCm;
	OutLocationCm = Unreal;
	return true;
}

void USpnMapSubsystem::ObservePlayer(const FVector& PlayerLocationCm)
{
	FVector2D Metres;
	if (!HasDefinition() || !UnrealToMap(PlayerLocationCm, Metres))
	{
		return;
	}
	State->LastPlayer = MapPosition{State->SurfaceLayer, Vec2{Metres.X, Metres.Y}};
	if (FVector2D::DistSquared(Metres, LastObserved) < 4.0)
	{
		return; // moved less than 2 m
	}
	LastObserved = Metres;
	const MapPosition Position{State->SurfaceLayer, Vec2{Metres.X, Metres.Y}};
	size_t Changed = State->Map->DiscoverPoisAround(Position, 60.0, DiscoveryState::Known);
	Changed += State->Map->DiscoverPoisAround(Position, 12.0, DiscoveryState::Visited);
	Changed += State->Map->DiscoverRegionsAt(Position, DiscoveryState::Visited);

	const auto Regions = State->Definition->RegionsAt(Position);
	RegionLabel = Regions.empty() ? FString() : ToF(Regions.front()->label);
	if (Changed > 0)
	{
		OnMapChanged.Broadcast();
	}
}

TArray<FSpnMapPoiView> USpnMapSubsystem::GetPois() const
{
	TArray<FSpnMapPoiView> Out;
	if (!HasDefinition())
	{
		return Out;
	}
	for (const PointOfInterestDef& Poi : State->Definition->Pois())
	{
		FSpnMapPoiView View;
		View.Id = ToF(Poi.id.Value());
		View.Category = ToF(Poi.category.Value());
		View.Label = ToF(Poi.label);
		View.Metres = FVector2D(Poi.position.xy.x, Poi.position.xy.y);
		View.Discovery = ToDiscovery(State->Map->Discovery(Poi.id));
		View.bVisible = State->Map->IsVisible(Poi);
		Out.Add(View);
	}
	return Out;
}

TArray<FSpnMapMarkerView> USpnMapSubsystem::GetMarkers() const
{
	TArray<FSpnMapMarkerView> Out;
	if (!HasDefinition())
	{
		return Out;
	}
	const FSpnMapBounds Bounds = GetBounds();
	MapFilter Filter;
	Filter.includePois = false;
	Filter.includeHidden = true;
	auto Hits = State->Map->QueryRect(State->SurfaceLayer, MapRect{{Bounds.Min.X, Bounds.Min.Y}, {Bounds.Max.X, Bounds.Max.Y}}, Filter);
	if (Hits)
	{
		for (const MapQueryHit& Hit : Hits.Value())
		{
			if (Hit.marker != nullptr)
			{
				Out.Add(ToView(*Hit.marker));
			}
		}
	}
	return Out;
}

void USpnMapSubsystem::QueryVisible(const FVector2D& Min, const FVector2D& Max, TArray<FSpnMapPoiView>& OutPois, TArray<FSpnMapMarkerView>& OutMarkers) const
{
	OutPois.Reset();
	OutMarkers.Reset();
	if (!HasDefinition())
	{
		return;
	}
	auto Hits = State->Map->QueryRect(State->SurfaceLayer, MapRect{{Min.X, Min.Y}, {Max.X, Max.Y}});
	if (!Hits)
	{
		return;
	}
	for (const MapQueryHit& Hit : Hits.Value())
	{
		if (Hit.marker != nullptr)
		{
			OutMarkers.Add(ToView(*Hit.marker));
		}
		else if (Hit.poi != nullptr)
		{
			FSpnMapPoiView View;
			View.Id = ToF(Hit.poi->id.Value());
			View.Category = ToF(Hit.poi->category.Value());
			View.Label = ToF(Hit.poi->label);
			View.Metres = FVector2D(Hit.poi->position.xy.x, Hit.poi->position.xy.y);
			View.Discovery = ToDiscovery(State->Map->Discovery(Hit.poi->id));
			View.bVisible = true;
			OutPois.Add(View);
		}
	}
}

TArray<FString> USpnMapSubsystem::GetRegionLabelsAt(const FVector2D& Metres) const
{
	TArray<FString> Out;
	if (HasDefinition())
	{
		for (const MapRegionDef* Region : State->Definition->RegionsAt(MapPosition{State->SurfaceLayer, Vec2{Metres.X, Metres.Y}}))
		{
			Out.Add(ToF(Region->label));
		}
	}
	return Out;
}

ESpnMapResult USpnMapSubsystem::SetWaypointAt(const FVector2D& Metres, const FString& Label, int64& OutMarkerId, FString* OutDetail)
{
	OutMarkerId = 0;
	if (!HasDefinition())
	{
		return ESpnMapResult::NoDefinition;
	}
	if (!FMath::IsFinite(Metres.X) || !FMath::IsFinite(Metres.Y))
	{
		return ESpnMapResult::OutOfBounds;
	}
	const TArray<FSpnMapMarkerView> Old = GetMarkers();

	MarkerSpec Spec;
	Spec.position = MapPosition{State->SurfaceLayer, Vec2{Metres.X, Metres.Y}};
	Spec.category = PlayerWaypointCategory();
	Spec.label = SanitizeLabel(Label, "Waypoint");
	Result<MarkerId, MapError> Added = Result<MarkerId, MapError>(MarkerId());
	if (HasRoadNetwork() && State->LastPlayer.has_value())
	{
		// plans the road route from the player; the waypoint exists even when there is none (NoRoute is a state, not an error)
		Added = State->Nav->SetWaypoint(*State->Map, *State->LastPlayer, Spec.position, Spec.label, NowSeconds(), ArrivalRadiusMetres);
		if (Added)
		{
			State->LastUpdate = State->Nav->Update(*State->LastPlayer, NowSeconds(), &*State->Map);
		}
	}
	else
	{
		Added = State->Map->AddMarker(std::move(Spec));
	}
	if (!Added)
	{
		if (OutDetail != nullptr) { *OutDetail = ToF(Added.Error().detail); }
		return FromMapError(Added.Error().code);
	}
	OutMarkerId = static_cast<int64>(Added.Value().Value());
	for (const FSpnMapMarkerView& Previous : Old)
	{
		if (Previous.bPlayerWaypoint)
		{
			(void)State->Map->RemoveMarker(MarkerId(static_cast<uint64>(Previous.Id)));
		}
	}
	OnMapChanged.Broadcast();
	return ESpnMapResult::Ok;
}

ESpnMapResult USpnMapSubsystem::SetWaypointToPoi(const FString& PoiId, int64& OutMarkerId)
{
	OutMarkerId = 0;
	if (!HasDefinition())
	{
		return ESpnMapResult::NoDefinition;
	}
	const auto Id = ContentId::Parse(ToUtf8(PoiId));
	const PointOfInterestDef* Poi = Id ? State->Definition->FindPoi(*Id) : nullptr;
	if (Poi == nullptr)
	{
		return ESpnMapResult::UnknownTarget;
	}
	const TArray<FSpnMapMarkerView> Old = GetMarkers();
	MarkerSpec Spec;
	Spec.position = Poi->position;
	Spec.category = PlayerWaypointCategory();
	Spec.label = Poi->label;
	Spec.linkedPoi = Poi->id;
	auto Added = State->Map->AddMarker(std::move(Spec));
	if (!Added)
	{
		return FromMapError(Added.Error().code);
	}
	OutMarkerId = static_cast<int64>(Added.Value().Value());
	for (const FSpnMapMarkerView& Previous : Old)
	{
		if (Previous.bPlayerWaypoint)
		{
			(void)State->Map->RemoveMarker(MarkerId(static_cast<uint64>(Previous.Id)));
		}
	}
	OnMapChanged.Broadcast();
	return ESpnMapResult::Ok;
}

ESpnMapResult USpnMapSubsystem::RemoveMarker(int64 MarkerIdValue)
{
	if (!HasDefinition())
	{
		return ESpnMapResult::NoDefinition;
	}
	if (const auto Error = State->Map->RemoveMarker(MarkerId(static_cast<uint64>(MarkerIdValue))))
	{
		return FromMapError(Error->code);
	}
	OnMapChanged.Broadcast();
	return ESpnMapResult::Ok;
}

bool USpnMapSubsystem::ClearWaypoint()
{
	bool bAny = false;
	if (HasRoadNetwork())
	{
		State->Nav->Cancel(nullptr);
		State->LastUpdate = NavigationUpdate{};
	}
	if (HasDefinition())
	{
		for (const FSpnMapMarkerView& Marker : GetMarkers())
		{
			if (Marker.bPlayerWaypoint && !State->Map->RemoveMarker(MarkerId(static_cast<uint64>(Marker.Id))))
			{
				bAny = true;
			}
		}
	}
	if (bAny)
	{
		OnMapChanged.Broadcast();
	}
	return bAny;
}

bool USpnMapSubsystem::GetWaypoint(FSpnMapMarkerView& Out) const
{
	// the newest player waypoint (there is at most one by policy; after a foreign save the newest wins)
	bool bFound = false;
	for (const FSpnMapMarkerView& Marker : GetMarkers())
	{
		if (Marker.bPlayerWaypoint && (!bFound || Marker.Id > Out.Id))
		{
			Out = Marker;
			bFound = true;
		}
	}
	return bFound;
}

bool USpnMapSubsystem::PickAt(const FVector2D& Metres, double RadiusMetres, FSpnMapPoiView* OutPoi, FSpnMapMarkerView* OutMarker) const
{
	if (!HasDefinition())
	{
		return false;
	}
	auto Hits = State->Map->QueryRadius(MapPosition{State->SurfaceLayer, Vec2{Metres.X, Metres.Y}}, RadiusMetres);
	if (!Hits || Hits.Value().empty())
	{
		return false;
	}
	const MapQueryHit& Hit = Hits.Value().front();
	if (Hit.marker != nullptr)
	{
		if (OutMarker != nullptr) { *OutMarker = ToView(*Hit.marker); }
		if (OutPoi != nullptr) { *OutPoi = FSpnMapPoiView(); }
		return true;
	}
	if (OutPoi != nullptr)
	{
		FSpnMapPoiView View;
		View.Id = ToF(Hit.poi->id.Value());
		View.Category = ToF(Hit.poi->category.Value());
		View.Label = ToF(Hit.poi->label);
		View.Metres = FVector2D(Hit.poi->position.xy.x, Hit.poi->position.xy.y);
		View.Discovery = ToDiscovery(State->Map->Discovery(Hit.poi->id));
		View.bVisible = true;
		*OutPoi = View;
	}
	if (OutMarker != nullptr) { *OutMarker = FSpnMapMarkerView(); }
	return true;
}

FSpnMinimapFrameData USpnMapSubsystem::BuildMinimapFrame(const FSpnMinimapRequest& Request) const
{
	FSpnMinimapFrameData Frame_;
	FVector2D Metres;
	double Bearing = 0.0;
	if (!HasDefinition() || !UnrealToMap(Request.PlayerLocationCm, Metres)
		|| !SpnWorldFrame::YawDegreesToBearing(Frame.Axes, Request.PlayerYawDegrees, Bearing))
	{
		return Frame_;
	}
	MinimapConfig Config;
	Config.shape = MinimapShape::Circle;
	Config.halfWidthPx = Request.RadiusPx;
	Config.halfHeightPx = Request.RadiusPx;
	Config.baseMetresPerPixel = Request.BaseMetresPerPixel;
	Config.indicatorInsetPx = 8.0;
	Config.offscreenIndicatorRangeMetres = Request.IndicatorRangeMetres;
	MinimapView View;
	View.player = MapPosition{State->SurfaceLayer, Vec2{Metres.X, Metres.Y}};
	View.headingRadians = Bearing;
	View.zoom = Request.Zoom;
	View.rotation = Request.bHeadingUp ? MinimapRotation::HeadingUp : MinimapRotation::NorthUp;
	auto Built = spn::core::BuildMinimapFrame(*State->Map, View, Config);
	if (!Built)
	{
		return Frame_;
	}
	const MinimapFrame& Core = Built.Value();
	Frame_.bValid = true;
	Frame_.MetresPerPixel = Core.metresPerPixel;
	Frame_.Zoom = Core.zoom;
	Frame_.NorthOnScreenRadians = Core.northOnScreenRadians;

	auto Fill = [](FSpnMinimapEntry& Entry, const MapQueryHit& Hit)
	{
		if (Hit.marker != nullptr)
		{
			Entry.MarkerId = static_cast<int64>(Hit.marker->id.Value());
			Entry.Label = ToF(Hit.marker->label);
			Entry.Category = ToF(Hit.marker->category.Value());
			Entry.bPlayerWaypoint = Hit.marker->category == PlayerWaypointCategory();
		}
		else if (Hit.poi != nullptr)
		{
			Entry.Id = ToF(Hit.poi->id.Value());
			Entry.Label = ToF(Hit.poi->label);
			Entry.Category = ToF(Hit.poi->category.Value());
		}
	};
	for (const MinimapMarker& Marker : Core.markers)
	{
		FSpnMinimapEntry Entry;
		Entry.Screen = FVector2D(Marker.screen.x, Marker.screen.y);
		Entry.DistanceMetres = Marker.distanceMetres;
		Fill(Entry, Marker.hit);
		Frame_.Entries.Add(Entry);
	}
	for (const MinimapIndicator& Indicator : Core.indicators)
	{
		FSpnMinimapEntry Entry;
		Entry.Screen = FVector2D(Indicator.screen.x, Indicator.screen.y);
		Entry.DistanceMetres = Indicator.distanceMetres;
		Entry.bOffscreen = true;
		Fill(Entry, Indicator.hit);
		Frame_.Entries.Add(Entry);
	}
	return Frame_;
}

FSpnNavigationReading USpnMapSubsystem::GetNavigation(const FVector& PlayerLocationCm, float PlayerYawDegrees) const
{
	FSpnNavigationReading Reading;
	FSpnMapMarkerView Waypoint;
	FVector2D Metres;
	if (!HasDefinition() || !GetWaypoint(Waypoint) || !UnrealToMap(PlayerLocationCm, Metres))
	{
		return Reading;
	}
	if (HasRoadNetwork())
	{
		// Road navigation: the numbers come from the NavigationSession (a real route), never from an air line.
		Reading.bActive = true;
		Reading.bStraightLine = false;
		Reading.MarkerId = Waypoint.Id;
		Reading.Label = Waypoint.Label;
		const NavigationUpdate& Last = State->LastUpdate;
		Reading.Status = ToNavStatus(State->Nav->Status());
		Reading.bArrived = Reading.Status == ESpnNavStatus::Arrived;
		Reading.bOffRoute = Last.offRoute;
		Reading.OffRouteMetres = Last.offRouteMetres;
		Reading.DistanceMetres = Reading.Status == ESpnNavStatus::Active ? Last.remainingMetres : 0.0;
		if (Reading.Status == ESpnNavStatus::NoRoute)
		{
			const auto Failure = State->Nav->LastFailure();
			Reading.FailureText = Failure ? ToF(std::string(ToString(*Failure))) : FString(TEXT("NoRoute"));
		}
		FVector2D Target(Waypoint.Metres);
		if (Reading.Status == ESpnNavStatus::Active)
		{
			if (const PlannedRoute* Route = State->Nav->CurrentRoute())
			{
				const std::vector<Vec2> Points = RoutePoints(Route->route);
				if (Points.size() >= 2)
				{
					const std::vector<double> Cumulative = CumulativeLengths(Points);
					const double Total = Cumulative.back();
					const double Offset = FMath::Clamp(Total - Last.remainingMetres + 40.0, 0.0, Total);
					const Vec2 Ahead = PointAtOffset(Points, Cumulative, Offset);
					Reading.bHasNextPoint = true;
					Reading.NextPointMetres = FVector2D(Ahead.x, Ahead.y);
					Target = Reading.NextPointMetres;
				}
			}
		}
		Reading.BearingRadians = NormalizeAngle(std::atan2(Target.X - Metres.X, Target.Y - Metres.Y));
		double Heading = 0.0;
		SpnWorldFrame::YawDegreesToBearing(Frame.Axes, PlayerYawDegrees, Heading);
		Reading.RelativeRadians = NormalizeSigned(Reading.BearingRadians - Heading);
		return Reading;
	}
	const MapPosition Origin{State->SurfaceLayer, Vec2{Metres.X, Metres.Y}};
	auto Destination = DestinationFromMarker(*State->Map, MarkerId(static_cast<uint64>(Waypoint.Id)), ArrivalRadiusMetres);
	if (!Destination)
	{
		return Reading;
	}
	auto Route = MakeStraightLineRoute(Origin, Destination.Value(), TravelMode::OnFoot);
	if (!Route)
	{
		return Reading;
	}
	Reading.bActive = true;
	Reading.MarkerId = Waypoint.Id;
	Reading.Label = Waypoint.Label;
	Reading.DistanceMetres = RouteLength(Route.Value());
	Reading.bArrived = HasArrived(Destination.Value(), Origin);
	const double Dx = Waypoint.Metres.X - Metres.X;
	const double Dy = Waypoint.Metres.Y - Metres.Y;
	Reading.BearingRadians = NormalizeAngle(std::atan2(Dx, Dy));
	double Heading = 0.0;
	SpnWorldFrame::YawDegreesToBearing(Frame.Axes, PlayerYawDegrees, Heading);
	Reading.RelativeRadians = NormalizeSigned(Reading.BearingRadians - Heading);
	return Reading;
}

void USpnMapSubsystem::UpdateNavigation(const FVector& PlayerLocationCm, float PlayerYawDegrees)
{
	if (HasRoadNetwork())
	{
		FVector2D Metres;
		if (!UnrealToMap(PlayerLocationCm, Metres))
		{
			return;
		}
		const MapPosition Player{State->SurfaceLayer, Vec2{Metres.X, Metres.Y}};
		State->LastPlayer = Player;
		const double Now = NowSeconds();
		FSpnMapMarkerView Waypoint;
		const bool bHasWaypoint = GetWaypoint(Waypoint);
		const NavigationStatus Before = State->Nav->Status();
		if (bHasWaypoint && (Before == NavigationStatus::Idle || Before == NavigationStatus::Cancelled))
		{
			// a waypoint without a session destination (loaded save, placed before the player pose was known)
			auto Destination = DestinationFromMarker(*State->Map, MarkerId(static_cast<uint64>(Waypoint.Id)), ArrivalRadiusMetres);
			if (Destination)
			{
				State->LastUpdate = State->Nav->SetDestination(Player, Destination.Value(), Now);
			}
		}
		else if (Before != NavigationStatus::Idle)
		{
			State->LastUpdate = State->Nav->Update(Player, Now, &*State->Map);
		}
		if (State->LastUpdate.routeChanged)
		{
			OnMapChanged.Broadcast();
		}
		if (bHasWaypoint && State->Nav->Status() == NavigationStatus::Arrived && ArrivalAnnouncedFor != Waypoint.Id)
		{
			ArrivalAnnouncedFor = Waypoint.Id;
			OnWaypointReached.Broadcast(Waypoint);
		}
		return;
	}
	const FSpnNavigationReading Reading = GetNavigation(PlayerLocationCm, PlayerYawDegrees);
	if (Reading.bActive && Reading.bArrived && ArrivalAnnouncedFor != Reading.MarkerId)
	{
		ArrivalAnnouncedFor = Reading.MarkerId;
		FSpnMapMarkerView Waypoint;
		if (GetWaypoint(Waypoint))
		{
			OnWaypointReached.Broadcast(Waypoint);
		}
	}
}

uint64 USpnMapSubsystem::GetRevision() const
{
	return HasDefinition() ? State->Map->Revision() : 0;
}
