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
	/** Always true in this build: the reading is a direct line, not a route. */
	bool bStraightLine = true;
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
	double ArrivalRadiusMetres = 4.0;
	int64 ArrivalAnnouncedFor = 0;
	FString RegionLabel;
	FVector2D LastObserved = FVector2D(1e18, 1e18);

	struct FState;
	TPimplPtr<FState> State;
};
