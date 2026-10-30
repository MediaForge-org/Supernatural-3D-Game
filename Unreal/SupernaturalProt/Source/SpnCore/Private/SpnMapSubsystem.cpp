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
	bTestDefinition = true;
	ResetStateInternal();
	BroadcastReplaced();
	UE_LOG(LogSpnCore, Log, TEXT("Map: installed the TEMPORARY test definition (%d POIs, placeholder, not geography)"), static_cast<int32>(State->Definition->Pois().size()));
	return ESpnMapResult::Ok;
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
	}
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
	auto Added = State->Map->AddMarker(std::move(Spec));
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
