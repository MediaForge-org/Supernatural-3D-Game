#pragma once

#include "CoreMinimal.h"
#include "SpnWorldFrame.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Templates/PimplPtr.h"

#include "SpnMapSubsystem.generated.h"

// Unreal integration of the portable world map, minimap and navigation (Core: WorldMap.h, Minimap.h, Navigation.h) -> Night Build 01.
// ENGINEERING PROPOSAL, not master canon.
//
// ONE SOURCE OF TRUTH: this subsystem owns the single WorldMapState of the game instance. The full map screen, the minimap and the compass
// all read it; nothing else keeps markers, discovery or the waypoint. It is saved in the "world.map" section by USpnSaveSubsystem.
//
// WORLD DATA: until SPN WorldForge delivers a definition, InstallTestDefinition() builds a small PLACEHOLDER definition that mirrors the
// authored test district (~300 m); it is not the 2,200 km2 world and not geography. A real definition arrives through SetDefinition-style
// loading later; nothing here assumes the extent of the world, and every query is a spatial-index query (no global scans, no tiles allocated).
//
// COORDINATES: the Unreal <-> canonical axis preset is NOT decided (WF-DC01 / CQ-101). Frame is a parameter (default EastSouthUp, the
// preset that keeps the authored "X = east" of the test levels); changing it requires reinstalling the definition.
//
// NAVIGATION IS A STRAIGHT LINE: bearing and distance to the active waypoint are computed in the plane of the map. There is NO pathfinding,
// no road routing and no obstacle awareness. The UI says so ("direct line").
//
// WAYPOINT POLICY (proposal): at most one player waypoint exists at a time and it is the active navigation destination; setting a new one
// replaces the old one. Because the waypoint is an ordinary persistent marker, the active destination is saved and restored for free.

UENUM()
enum class ESpnMapResult : uint8
{
	Ok,
	/** No definition installed. */
	NoDefinition,
	/** Position not finite or outside the layer bounds. */
	OutOfBounds,
	/** Unknown marker or point-of-interest id. */
	UnknownTarget,
	/** The Core rejected the request (see Detail). */
	Rejected,
	InternalError,
};
SPNCORE_API const TCHAR* LexToString(ESpnMapResult Result);

enum class ESpnDiscovery : uint8 { Undiscovered, Known, Visited };

/** Mirrors spn::core::NavigationStatus. */
enum class ESpnNavStatus : uint8 { Idle, Active, Arrived, NoRoute, Cancelled };
SPNCORE_API const TCHAR* LexToString(ESpnNavStatus Status);

/** Mirrors spn::core::TravelMode. */
enum class ESpnTravelMode : uint8 { OnFoot, Driving };

/** One polyline of a render frame (road, river/coast/contour line). Screen pixels, origin = viewport centre, x right, y DOWN. */
struct FSpnRenderPolyline
{
	uint64 Id = 0;
	/** spn::core::RoadClass index (roads), spn::core::LineKind index (lines). */
	uint8 Kind = 0;
	/** spn::core::RoadStructure index: 0 Surface, 1 Bridge, 2 Tunnel, ... (roads). */
	uint8 Structure = 0;
	bool bOneWay = false;
	bool bRoundabout = false;
	bool bClosed = false;
	TArray<FVector2D> Points;
};

/** One filled ring (area or building). */
struct FSpnRenderPolygon
{
	uint64 Id = 0;
	/** spn::core::AreaKind index (areas). */
	uint8 Kind = 0;
	bool bBuilding = false;
	TArray<FVector2D> Points;
	TArray<FVector2D> Entrances;
};

/** Screen-space draw data from spn::core::BuildMapRenderFrame. The order of each array is the draw order. */
struct FSpnMapRenderFrame
{
	bool bValid = false;
	bool bTruncated = false;
	double MetresPerPixel = 1.0;
	double NorthOnScreenRadians = 0.0;
	double PlayerHeadingOnScreenRadians = 0.0;
	TArray<FSpnRenderPolygon> Areas;
	TArray<FSpnRenderPolyline> Lines;
	TArray<FSpnRenderPolygon> Buildings;
	TArray<FSpnRenderPolyline> Roads;
	TArray<TArray<FVector2D>> RouteTravelled;
	TArray<TArray<FVector2D>> RouteRemaining;
	bool bHasDestination = false;
	FVector2D DestinationScreen = FVector2D::ZeroVector;
	bool bDestinationInside = false;
	bool bHasDestinationIndicator = false;
	FVector2D DestinationIndicator = FVector2D::ZeroVector;
	int32 Candidates = 0;
	int32 Emitted = 0;
};

struct FSpnMapRenderRequest
{
	/** Map metres (X east, Y north) at the viewport centre. */
	FVector2D CenterMetres = FVector2D::ZeroVector;
	/** Compass bearing (radians clockwise from north) of the player; the arrow and the HeadingUp rotation use it. */
	double HeadingBearingRadians = 0.0;
	double MetresPerPixel = 1.0;
	bool bHeadingUp = false;
	bool bCircle = false;
	/** Circle: HalfSizePx.X is the radius. */
	FVector2D HalfSizePx = FVector2D(400.0, 300.0);
	bool bRespectDiscovery = false;
};

/** Raw road geometry in map metres (the same Core data the map draws; used to build the 3D demo region). */
struct FSpnGeoRoad
{
	uint64 Id = 0;
	/** spn::core::RoadClass / RoadStructure index. */
	uint8 Class = 0;
	uint8 Structure = 0;
	bool bOneWay = false;
	bool bRoundabout = false;
	TArray<FVector2D> Points;
};

struct FSpnGeoBuilding
{
	uint64 Id = 0;
	FString Category;
	FString Label;
	double HeightMetres = 0.0; // 0 = unspecified
	TArray<FVector2D> Footprint;
	TArray<FVector2D> Entrances;
};

struct FSpnGeoArea
{
	uint64 Id = 0;
	/** spn::core::AreaKind index. */
	uint8 Kind = 0;
	FString Label;
	double ElevationMetres = 0.0; // 0 = unknown
	TArray<FVector2D> Outline;
};

struct FSpnGeometryView
{
	bool bValid = false;
	TArray<FSpnGeoRoad> Roads;
	TArray<FSpnGeoBuilding> Buildings;
	TArray<FSpnGeoArea> Areas;
};

/** The active road route as map-metre vertices. */
struct FSpnRouteView
{
	bool bValid = false;
	uint64 Generation = 0;
	double LengthMetres = 0.0;
	double RoadLengthMetres = 0.0;
	double ConnectorLengthMetres = 0.0;
	double StraightLineMetres = 0.0;
	TArray<FVector2D> Points;
	/** Road segment ids the route follows (in order, without duplicates). */
	TArray<uint64> RoadSegmentIds;
};

struct FSpnMapPoiView
{
	FString Id;
	FString Category;
	FString Label;
	/** Map plane metres: X = east, Y = north. */
	FVector2D Metres = FVector2D::ZeroVector;
	ESpnDiscovery Discovery = ESpnDiscovery::Undiscovered;
	bool bVisible = false;
};

struct FSpnMapMarkerView
{
	int64 Id = 0;
	FString Category;
	FString Label;
	FVector2D Metres = FVector2D::ZeroVector;
	bool bPlayerWaypoint = false;
	/** POI the marker was resolved from, if any. */
	FString LinkedPoi;
};

struct FSpnMapBounds
{
	FVector2D Min = FVector2D::ZeroVector;
	FVector2D Max = FVector2D::ZeroVector;
};

/** One entry of a minimap frame: screen pixels relative to the viewport centre, x right, y DOWN. */
struct FSpnMinimapEntry
{
	FVector2D Screen = FVector2D::ZeroVector;
	double DistanceMetres = 0.0;
	FString Label;
	FString Category;
	FString Id;      // poi id, or empty for markers
	int64 MarkerId = 0;
	bool bPlayerWaypoint = false;
	/** True for an indicator on the viewport edge (target outside the viewport). */
	bool bOffscreen = false;
};

struct FSpnMinimapFrameData
{
	bool bValid = false;
	double MetresPerPixel = 1.0;
	double Zoom = 1.0;
	/** Screen angle of north, clockwise from up (0 for north-up). */
	double NorthOnScreenRadians = 0.0;
	TArray<FSpnMinimapEntry> Entries;
};

/** Where the active waypoint is, relative to the player. A straight line only. */
struct FSpnNavigationReading
{
	bool bActive = false;
	int64 MarkerId = 0;
	FString Label;
	/** Compass bearing to the target, radians clockwise from north, [0, 2 pi). */
	double BearingRadians = 0.0;
	/** Bearing relative to the player's heading, (-pi, pi]: 0 = straight ahead, positive = to the right. */
	double RelativeRadians = 0.0;
	double DistanceMetres = 0.0;
	bool bArrived = false;
	/** True when no road route drives the reading (test district / no geometry): the reading is a direct line. */
	bool bStraightLine = true;

	// --- road navigation (synthetic region, NightBuild02 NavigationSession); meaningful when bStraightLine == false ---
	ESpnNavStatus Status = ESpnNavStatus::Idle;
	/** Distance of the player from the route. */
	double OffRouteMetres = 0.0;
	bool bOffRoute = false;
	/** Core RouteFailure name when Status == NoRoute (e.g. NoPath, DestinationNotAccessible). */
	FString FailureText;
	/** A point on the route about 40 m ahead of the player: the compass follows the road, not the air line. */
	bool bHasNextPoint = false;
	FVector2D NextPointMetres = FVector2D::ZeroVector;
};

struct FSpnMinimapRequest
{
	FVector PlayerLocationCm = FVector::ZeroVector;
	float PlayerYawDegrees = 0.f;
	float Zoom = 1.f;
	bool bHeadingUp = false;
	/** Radius of the round viewport in pixels. */
	float RadiusPx = 110.f;
	/** Metres per pixel at zoom 1. */
	float BaseMetresPerPixel = 0.5f;
	/** Targets farther than the viewport but within this range get an edge indicator (0 = none). */
	float IndicatorRangeMetres = 400.f;
};

/**
 * Owns the world map state, discovery, markers, the active waypoint and the navigation reading. Game thread only.
 * Core types stay behind a private implementation (Decision 0006).
 */
UCLASS()
class SPNCORE_API USpnMapSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	USpnMapSubsystem();
	virtual ~USpnMapSubsystem() override;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// --- definition ----------------------------------------------------------------------------------------------------------------

	/** Installs the TEMPORARY test-district definition (placeholder labels, ~300 m). Replaces state with an empty one. */
	ESpnMapResult InstallTestDefinition(const FSpnWorldFrame& InFrame = FSpnWorldFrame());
	/**
	 * Installs the SYNTHETIC test region of the Core (spn::core::synthetic): definition, road/building/area geometry and a NavigationSession.
	 * Waypoints then plan real road routes (no air line). NOT geography, not a WorldForge result, not a map candidate. The Frame is the DEMO
	 * adapter between Core map metres and Unreal centimetres (Docs/NightBuild03); it is no global or RC-01 decision.
	 */
	ESpnMapResult InstallSyntheticRegion(const FSpnWorldFrame& InFrame = FSpnWorldFrame());
	bool IsSyntheticRegion() const { return bSyntheticRegion; }
	/**
	 * Installs a GENERATED placeholder region from a map data file written by the world generator (Tools/spn_world/map_data.py, format in
	 * its docstring; coordinates in Unreal cm, converted with InFrame): definition, roads, buildings, water, creek, POIs and a NavigationSession.
	 * Placeholder data of the test world SpnWorld, not geography. Replaces state with an empty one. The map save section is unchanged (it
	 * stores state only); ids of the earlier test definition are listed hidden in the file so older SpnWorld saves still load.
	 */
	ESpnMapResult InstallGeneratedRegion(const FString& FilePath, const FSpnWorldFrame& InFrame = FSpnWorldFrame(), FString* OutDetail = nullptr);
	bool IsGeneratedRegion() const { return bGeneratedRegion; }
	const FString& GetGeneratedPath() const { return GeneratedPath; }
	/** Content/SpnWorld/Data/SpnWorldMap.json */
	static FString GetSpnWorldMapPath();
	/** True when road geometry and a NavigationSession exist (routes instead of straight lines). */
	bool HasRoadNetwork() const;
	/** Named test position of the synthetic region: westStreet, eastStreet, westOpenField, shopInside, houseInside, noEntranceInside, warehouseInside, islandTarget, hamletTarget. */
	bool GetSyntheticPosition(const FString& Key, FVector2D& OutMetres) const;
	bool HasDefinition() const;
	const FSpnWorldFrame& GetFrame() const { return Frame; }
	bool IsTestDefinition() const { return bTestDefinition; }
	/** Bounds of the (single) surface layer in map metres. */
	FSpnMapBounds GetBounds() const;
	/** Drops the definition and all state. */
	void Clear();

	// --- coordinates ---------------------------------------------------------------------------------------------------------------

	/** Unreal centimetres -> map metres (X east, Y north). False for non-finite input. */
	bool UnrealToMap(const FVector& LocationCm, FVector2D& OutMetres) const;
	/** Map metres -> Unreal centimetres on the plane Z = ZCm. */
	bool MapToUnreal(const FVector2D& Metres, float ZCm, FVector& OutLocationCm) const;

	// --- discovery and queries -------------------------------------------------------------------------------------------------------

	/** Call with the player's position: marks nearby POIs Known and the region Visited. Cheap when the player barely moved. */
	void ObservePlayer(const FVector& PlayerLocationCm);
	/** All POIs; bVisible tells whether the map shows them (visibility rule + discovery). */
	TArray<FSpnMapPoiView> GetPois() const;
	TArray<FSpnMapMarkerView> GetMarkers() const;
	/**
	 * What the map shows inside a rectangle (map metres): visible POIs and all non-hidden markers, through the spatial index. A tiled / streamed
	 * world draws only the rectangle on screen; nothing here scans the whole world.
	 */
	void QueryVisible(const FVector2D& Min, const FVector2D& Max, TArray<FSpnMapPoiView>& OutPois, TArray<FSpnMapMarkerView>& OutMarkers) const;
	/** Region labels containing the position, smallest first. */
	TArray<FString> GetRegionLabelsAt(const FVector2D& Metres) const;
	FString GetRegionLabel() const { return RegionLabel; }

	// --- markers and waypoint --------------------------------------------------------------------------------------------------------

	/** Places THE player waypoint (replaces an existing one). OutMarkerId = 0 on failure. */
	ESpnMapResult SetWaypointAt(const FVector2D& Metres, const FString& Label, int64& OutMarkerId, FString* OutDetail = nullptr);
	/** Waypoint at a POI (links the marker to it). */
	ESpnMapResult SetWaypointToPoi(const FString& PoiId, int64& OutMarkerId);
	ESpnMapResult RemoveMarker(int64 MarkerId);
	/** Removes the player waypoint, if any. True if one existed. */
	bool ClearWaypoint();
	bool GetWaypoint(FSpnMapMarkerView& Out) const;
	/** Marker or POI under a map position (nearest within RadiusMetres), for mouse picking. Returns false if none. */
	bool PickAt(const FVector2D& Metres, double RadiusMetres, FSpnMapPoiView* OutPoi, FSpnMapMarkerView* OutMarker) const;
	void SetArrivalRadiusMetres(double Radius) { ArrivalRadiusMetres = FMath::Max(0.5, Radius); }
	double GetArrivalRadiusMetres() const { return ArrivalRadiusMetres; }

	// --- minimap and navigation ------------------------------------------------------------------------------------------------------

	FSpnMinimapFrameData BuildMinimapFrame(const FSpnMinimapRequest& Request) const;
	FSpnNavigationReading GetNavigation(const FVector& PlayerLocationCm, float PlayerYawDegrees) const;
	/** Evaluates arrival once per waypoint: broadcasts OnWaypointReached the first time the player is inside the radius. */
	void UpdateNavigation(const FVector& PlayerLocationCm, float PlayerYawDegrees);

	// --- road navigation (needs InstallSyntheticRegion) -------------------------------------------------------------------------------

	void SetTravelMode(ESpnTravelMode Mode);
	ESpnTravelMode GetTravelMode() const { return TravelMode; }
	/** The active road route (valid while Status is Active or Arrived). */
	bool GetRoute(FSpnRouteView& Out) const;
	/** Increases whenever a different route is installed or cleared (redraw key of the route layer). */
	uint64 GetRouteGeneration() const;
	/** Closes / opens a road segment. Re-plans at once when the active route uses it. False for an unknown segment id. */
	bool SetRoadClosed(uint64 SegmentId, bool bClosed);
	bool IsRoadClosed(uint64 SegmentId) const;
	TArray<uint64> GetClosedRoads() const;
	/** Nearest road segment within RadiusMetres of the position (any class), for picking a road on the map. */
	bool FindRoadAt(const FVector2D& Metres, double RadiusMetres, uint64& OutSegmentId) const;
	/** Draw data of the map for a viewport (bValid false without geometry). */
	FSpnMapRenderFrame BuildMapRenderFrame(const FSpnMapRenderRequest& Request) const;
	/** Copies the raw feature geometry (roads, buildings, areas) out of the Core, in map metres. For visualisation only. */
	FSpnGeometryView GetGeometryView() const;
	/** Plans a route without creating a waypoint (tests, diagnostics). False with OutFailure set when none exists. */
	bool PlanRouteLength(const FVector2D& FromMetres, const FVector2D& ToMetres, double& OutLength, FString& OutFailure) const;

	/** Moves the navigation clock forward (tests and scripted demos: re-plan cooldowns are measured on it). Real time keeps running. */
	void AdvanceNavigationClock(double Seconds) { ClockSkew += Seconds; }

	// --- change tracking -------------------------------------------------------------------------------------------------------------

	/** Increases on every successful mutation (UI refresh key). Not saved. */
	uint64 GetRevision() const;

	DECLARE_MULTICAST_DELEGATE(FSpnMapChanged);
	/** After a marker/waypoint/discovery change. */
	FSpnMapChanged OnMapChanged;
	/** After the whole map state was replaced (a load, Clear, InstallTestDefinition). */
	FSpnMapChanged OnMapReplaced;
	DECLARE_MULTICAST_DELEGATE_OneParam(FSpnWaypointReached, const FSpnMapMarkerView&);
	FSpnWaypointReached OnWaypointReached;

private:
	friend class USpnSaveSubsystem;
	void BroadcastReplaced();
	void ResetStateInternal();

	FSpnWorldFrame Frame;
	bool bTestDefinition = false;
	bool bSyntheticRegion = false;
	bool bGeneratedRegion = false;
	FString GeneratedPath;
	double ClockSkew = 0.0;
	double NowSeconds() const;
	ESpnTravelMode TravelMode = ESpnTravelMode::Driving;
	double ArrivalRadiusMetres = 4.0;
	int64 ArrivalAnnouncedFor = 0;
	FString RegionLabel;
	FVector2D LastObserved = FVector2D(1e18, 1e18);

	struct FState;
	TPimplPtr<FState> State;
};
