#pragma once

// Map geometry: the feature data behind a world map — road network, building footprints, areas (water, forest, districts, ...) and linear features
// (rivers, coastlines, contours). It extends WorldMapDefinition (layers, regions, POIs) with real geometry; it does not replace it. A MapGeometry is
// immutable once created and validated against the definition (known layers, inside the layer bounds, known regions).
//
// Coordinates: metres, x = east, y = north, one plane per MapLayer (see WorldMap.h). Positions are stored as doubles, so large absolute coordinates
// (up to +-10,000 km) keep sub-millimetre resolution; all queries subtract in double.
//
// Determinism: Create sorts every list by id; the input order never changes a result. Ids are stable, caller-assigned (the future WorldForge import
// assigns them) and never reused for different content.
//
// ENGINEERING FOUNDATION (NightBuild02), not master canon. Master consultation found no road-graph, routing or building-entrance specification
// (the road-graph contract lives outside this repository, SPN_WorldForge RC-01). Road classes, access rules, structure kinds and area kinds below
// are PROPOSALS. No real geography is defined here; all fixtures are synthetic.

#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <set>
#include <spn/core/ContentId.h>
#include <spn/core/Result.h>
#include <spn/core/StrongId.h>
#include <spn/core/Vec.h>
#include <spn/core/WorldMap.h>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace spn::core {

enum class TravelMode : std::uint8_t; // Navigation.h

struct RoadNodeTag;
struct RoadSegmentTag;
struct BuildingTag;
struct MapAreaTag;
struct MapLineTag;
using RoadNodeId = StrongId<RoadNodeTag, std::uint64_t>;
using RoadSegmentId = StrongId<RoadSegmentTag, std::uint64_t>;
using BuildingId = StrongId<BuildingTag, std::uint64_t>;
using MapAreaId = StrongId<MapAreaTag, std::uint64_t>;
using MapLineId = StrongId<MapLineTag, std::uint64_t>;

inline constexpr std::size_t kMaxFeaturePoints = 100'000; ///< vertices per polyline / ring (parser-style limit against absurd input)

// --- Errors -------------------------------------------------------------------------------------------------------------------

enum class GeometryErrorCode : std::uint8_t {
    InvalidId, DuplicateId, UnknownLayer, OutsideLayerBounds, InvalidCoordinate, UnknownNode, UnknownSegment, LayerMismatch, DegenerateGeometry,
    InvalidPolygon, InvalidEntrance, InvalidAccess, UnknownRegion, InvalidLabel, TooManyPoints, RestrictionNotAdjacent, MergeConflict, InvalidGridCell,
};
[[nodiscard]] std::string_view ToString(GeometryErrorCode code) noexcept;
struct GeometryError {
    GeometryErrorCode code;
    std::uint64_t id = 0; ///< raw value of the offending feature id (0 = n/a)
    std::string detail;
    friend bool operator==(const GeometryError&, const GeometryError&) = default;
};

// --- Roads ---------------------------------------------------------------------------------------------------------------------

/// Ordered from the most to the least important road; the order is the default drawing order (least important first).
enum class RoadClass : std::uint8_t {
    Motorway,     ///< Autobahn
    MotorwayLink, ///< Auffahrt / Abfahrt
    Expressway,   ///< Schnellstraße
    Primary,      ///< Bundes- / Hauptstraße
    Secondary,    ///< Landstraße
    Residential,  ///< Wohnstraße
    Service,      ///< Nebenstraße
    FieldTrack,   ///< Feldweg
    ForestTrack,  ///< Waldweg
    Footway,      ///< Fußweg
};
inline constexpr std::size_t kRoadClassCount = 10;
[[nodiscard]] std::string_view ToString(RoadClass roadClass) noexcept;

enum class RoadStructure : std::uint8_t {
    Surface,
    Bridge,    ///< crosses over water or other roads; connects to other roads only at its nodes
    Tunnel,    ///< cannot be entered mid-span from outside (a destination is never snapped into it)
    Underpass, ///< a short tunnel below another road
};
[[nodiscard]] std::string_view ToString(RoadStructure structure) noexcept;
[[nodiscard]] constexpr bool IsEnclosed(RoadStructure s) noexcept { return s == RoadStructure::Tunnel || s == RoadStructure::Underpass; }

/// Direction of travel for VEHICLES. `Forward` = from -> to only. Pedestrians ignore it (as in common road data); roads are walkable both ways.
enum class RoadDirection : std::uint8_t { Both, Forward, Backward };

/// Bit set of the travel modes a segment allows.
inline constexpr std::uint8_t kAccessFoot = 1;
inline constexpr std::uint8_t kAccessVehicle = 2;
/// Default access of a class: Motorway/MotorwayLink/Expressway = vehicles only, Footway = foot only, everything else both.
[[nodiscard]] std::uint8_t DefaultAccess(RoadClass roadClass) noexcept;

struct RoadNodeDef {
    RoadNodeId id;
    MapPosition position;
    friend bool operator==(const RoadNodeDef&, const RoadNodeDef&) = default;
};

/// A road section between two nodes. Intermediate vertices (`shape`) give the geometry; the segment length is the length of the polyline
/// from -> shape -> to, never the straight distance of the end nodes.
struct RoadSegmentDef {
    RoadSegmentId id;
    RoadNodeId from;
    RoadNodeId to;
    RoadClass roadClass = RoadClass::Residential;
    RoadStructure structure = RoadStructure::Surface;
    RoadDirection direction = RoadDirection::Both;
    bool roundabout = false;            ///< part of a roundabout ring (direction is then normally Forward)
    std::optional<std::uint8_t> access; ///< unset = DefaultAccess(roadClass)
    std::vector<Vec2> shape;            ///< vertices strictly between the two nodes
    ContentId region;                   ///< optional: discovery region this feature belongs to
    std::string name;
    friend bool operator==(const RoadSegmentDef&, const RoadSegmentDef&) = default;
};

/// Forbids the move from segment `from` onto segment `to` at node `via` (a "no left turn"). Both segments must touch the node.
struct TurnRestriction {
    RoadNodeId via;
    RoadSegmentId from;
    RoadSegmentId to;
    bool vehiclesOnly = true; ///< false: applies to pedestrians as well
    friend bool operator==(const TurnRestriction&, const TurnRestriction&) = default;
};

/// One road snapped position: the nearest point of a segment's polyline.
struct RoadSnap {
    std::uint32_t segmentIndex = 0;
    RoadSegmentId segment;
    Vec2 point;
    double offsetMetres = 0.0;   ///< along the segment from its `from` node
    double distanceMetres = 0.0; ///< from the queried position to `point`
};

struct SnapRules {
    TravelMode mode;
    bool allowEnclosed = false; ///< also snap into tunnels / underpasses (an origin inside a tunnel; never for destinations)
    const std::set<RoadSegmentId>* closed = nullptr;
};

/// Immutable road graph with per-segment polylines, incident lists, turn restrictions and a spatial index. Edge cases handled by the data model:
/// roads that cross without a shared node do not connect (bridges, underpasses); segments of different layers never connect.
class RoadNetwork {
public:
    struct SegmentInfo {
        std::uint32_t fromNode = 0;
        std::uint32_t toNode = 0;
        std::vector<Vec2> points; ///< from, shape..., to
        std::vector<double> cumulative;
        double length = 0.0;
        RoadClass roadClass = RoadClass::Residential;
        RoadStructure structure = RoadStructure::Surface;
        RoadDirection direction = RoadDirection::Both;
        std::uint8_t access = 0;
        bool roundabout = false;
        MapLayerId layer;
        MapRect bounds;
    };
    struct Incident {
        std::uint32_t segment = 0;
        bool startsHere = false; ///< true: the segment's `from` node is this node (travel forward leaves here)
    };

    /// Validates ids, nodes, segments, shapes and restrictions (see GeometryErrorCode); sorts by id.
    [[nodiscard]] static Result<RoadNetwork, GeometryError> Create(std::vector<RoadNodeDef> nodes, std::vector<RoadSegmentDef> segments,
                                                                    std::vector<TurnRestriction> restrictions = {}, double gridCellMetres = 256.0);

    [[nodiscard]] const std::vector<RoadNodeDef>& Nodes() const noexcept { return nodes_; }
    [[nodiscard]] const std::vector<RoadSegmentDef>& Segments() const noexcept { return segments_; }
    [[nodiscard]] const std::vector<TurnRestriction>& Restrictions() const noexcept { return restrictions_; }
    [[nodiscard]] std::optional<std::uint32_t> NodeIndex(RoadNodeId id) const;
    [[nodiscard]] std::optional<std::uint32_t> SegmentIndex(RoadSegmentId id) const;
    [[nodiscard]] const SegmentInfo& Info(std::uint32_t segmentIndex) const { return info_[segmentIndex]; }
    [[nodiscard]] const std::vector<Incident>& IncidentAt(std::uint32_t nodeIndex) const { return incident_[nodeIndex]; }
    [[nodiscard]] Vec2 NodePosition(std::uint32_t nodeIndex) const { return nodes_[nodeIndex].position.xy; }

    /// True if the mode may use the segment travelling in the given direction (forward = from -> to). Foot ignores one-way rules.
    [[nodiscard]] bool Allows(std::uint32_t segmentIndex, TravelMode mode, bool forward) const noexcept;
    /// True if a restriction forbids moving from segment `fromSegment` onto `toSegment` at node `via` for this mode.
    [[nodiscard]] bool TurnForbidden(std::uint32_t via, std::uint32_t fromSegment, std::uint32_t toSegment, TravelMode mode) const;
    /// Number of incident segment ends at the node (a loop counts twice).
    [[nodiscard]] std::size_t Degree(std::uint32_t nodeIndex) const { return incident_[nodeIndex].size(); }

    /// Nearest eligible segments within `radius` of `position` on its layer, nearest first (ties: lower segment id), at most `maxCount`.
    /// Eligible: the mode may use it in at least one direction, it is not closed, and (unless rules.allowEnclosed) it is not a tunnel / underpass.
    [[nodiscard]] std::vector<RoadSnap> Snap(const MapPosition& position, double radius, const SnapRules& rules, std::size_t maxCount) const;
    /// True if at least one road segment lies on `layer`. O(1).
    [[nodiscard]] bool HasLayer(MapLayerId layer) const;
    /// Indices (ascending) of the segments on `layer` whose bounding box intersects `rect`. Uses the spatial index.
    [[nodiscard]] std::vector<std::uint32_t> SegmentsInRect(MapLayerId layer, const MapRect& rect) const;

private:
    RoadNetwork() = default;
    std::vector<RoadNodeDef> nodes_;
    std::vector<RoadSegmentDef> segments_;
    std::vector<TurnRestriction> restrictions_;
    std::vector<SegmentInfo> info_;
    std::vector<std::vector<Incident>> incident_;
    struct RestrictionKey {
        std::uint32_t via, from, to;
        bool vehiclesOnly;
    };
    std::vector<RestrictionKey> restrictionKeys_; // sorted by (via, from, to)
    struct Index;
    std::shared_ptr<const Index> index_;
};

// --- Buildings, areas, lines -----------------------------------------------------------------------------------------------------

struct BuildingDef {
    BuildingId id;
    MapLayerId layer;
    ContentId category; ///< free data category (house, shop, warehouse, ...); not interpreted here
    std::vector<Vec2> footprint; ///< open ring (the closing edge is implied), simple polygon
    std::vector<Vec2> entrances; ///< points on (within 1 m of) the footprint boundary; may be empty
    ContentId region;
    std::string label;
    std::optional<double> heightMetres;
    friend bool operator==(const BuildingDef&, const BuildingDef&) = default;
};

enum class AreaKind : std::uint8_t { Water, Island, Forest, Park, Green, Industrial, Parking, Plaza, District, Mountain };
[[nodiscard]] std::string_view ToString(AreaKind kind) noexcept;
/// Water areas are not crossable by an access connector (a straight leg from a point to the road network).
[[nodiscard]] constexpr bool BlocksConnectors(AreaKind kind) noexcept { return kind == AreaKind::Water; }

struct AreaDef {
    MapAreaId id;
    MapLayerId layer;
    AreaKind kind = AreaKind::Green;
    std::vector<Vec2> outline; ///< open ring, simple polygon
    ContentId region;
    std::string label;
    std::optional<double> elevationMetres; ///< summit / plateau height where known
    friend bool operator==(const AreaDef&, const AreaDef&) = default;
};

enum class LineKind : std::uint8_t { River, Stream, Coastline, Contour };
[[nodiscard]] std::string_view ToString(LineKind kind) noexcept;
/// Rivers and coastlines cannot be crossed by an access connector; streams and contour lines can.
[[nodiscard]] constexpr bool BlocksConnectors(LineKind kind) noexcept { return kind == LineKind::River || kind == LineKind::Coastline; }

struct LineFeatureDef {
    MapLineId id;
    MapLayerId layer;
    LineKind kind = LineKind::River;
    std::vector<Vec2> points; ///< at least two
    ContentId region;
    std::string label;
    std::optional<double> elevationMetres; ///< contour lines
    friend bool operator==(const LineFeatureDef&, const LineFeatureDef&) = default;
};

/// Raw geometry input (what an importer produces). Order does not matter.
struct MapGeometryData {
    std::vector<RoadNodeDef> nodes;
    std::vector<RoadSegmentDef> segments;
    std::vector<TurnRestriction> restrictions;
    std::vector<BuildingDef> buildings;
    std::vector<AreaDef> areas;
    std::vector<LineFeatureDef> lines;
    friend bool operator==(const MapGeometryData&, const MapGeometryData&) = default;
};

/// Merges two region packages into one data set (cross-region navigation). Features with an equal id must be identical (a border node or a road
/// crossing the border may appear in both packages); equal id with different content is MergeConflict. The result is sorted by id.
[[nodiscard]] Result<MapGeometryData, GeometryError> MergeGeometryData(const MapGeometryData& a, const MapGeometryData& b);

/// Uniform grid over feature bounding boxes. Features that would cover too many cells are kept in an always-checked list. Results are sorted.
class BoxGrid {
public:
    explicit BoxGrid(double cellMetres = 256.0);
    void Insert(std::uint32_t key, const MapRect& bounds);
    void Query(const MapRect& rect, std::vector<std::uint32_t>& out) const; ///< keys whose bounds intersect `rect`, ascending, unique
    [[nodiscard]] std::size_t Size() const noexcept { return bounds_.size(); }

private:
    struct CellKey {
        std::int64_t x, y;
        friend bool operator==(const CellKey&, const CellKey&) = default;
    };
    struct CellHash {
        std::size_t operator()(const CellKey& c) const noexcept;
    };
    double cell_;
    std::unordered_map<CellKey, std::vector<std::uint32_t>, CellHash> cells_;
    std::vector<std::uint32_t> large_;
    std::unordered_map<std::uint32_t, MapRect> bounds_;
};

class MapGeometry {
public:
    /// Validates everything against `definition` (layers known, features inside layer bounds, regions known, rings simple, entrances on the footprint,
    /// labels valid) and builds the indexes. Never partially succeeds.
    [[nodiscard]] static Result<MapGeometry, GeometryError> Create(const WorldMapDefinition& definition, MapGeometryData data, double gridCellMetres = 256.0);

    [[nodiscard]] const RoadNetwork& Roads() const noexcept { return *roads_; }
    [[nodiscard]] const std::vector<BuildingDef>& Buildings() const noexcept { return buildings_; }
    [[nodiscard]] const std::vector<AreaDef>& Areas() const noexcept { return areas_; }
    [[nodiscard]] const std::vector<LineFeatureDef>& Lines() const noexcept { return lines_; }
    [[nodiscard]] const BuildingDef* FindBuilding(BuildingId id) const;
    [[nodiscard]] const AreaDef* FindArea(MapAreaId id) const;

    /// Indices (ascending) of the features of `layer` whose bounding box intersects `rect`.
    [[nodiscard]] std::vector<std::uint32_t> BuildingsInRect(MapLayerId layer, const MapRect& rect) const;
    [[nodiscard]] std::vector<std::uint32_t> AreasInRect(MapLayerId layer, const MapRect& rect) const;
    [[nodiscard]] std::vector<std::uint32_t> LinesInRect(MapLayerId layer, const MapRect& rect) const;
    /// The building whose footprint contains the position (boundary included); smallest footprint wins, ties by id.
    [[nodiscard]] const BuildingDef* BuildingAt(const MapPosition& position) const;
    /// Length of the straight segment a-b on `layer` that lies in water (Water areas minus Island areas), in metres.
    [[nodiscard]] double WetLengthMetres(MapLayerId layer, Vec2 a, Vec2 b) const;
    /// True if a straight access connector a-b may not be used: it crosses a river or coastline, or runs more than `maxWetMetres` through water.
    /// The tolerance lets positions on a bridge deck (which lies over a Water area) connect to the deck without being blocked.
    [[nodiscard]] bool ConnectorBlocked(MapLayerId layer, Vec2 a, Vec2 b, double maxWetMetres = 10.0) const;

    /// The raw data again (sorted), e.g. to merge regions or to re-save.
    [[nodiscard]] MapGeometryData ToData() const;

private:
    MapGeometry() = default;
    std::shared_ptr<const RoadNetwork> roads_;
    std::vector<BuildingDef> buildings_;
    std::vector<AreaDef> areas_;
    std::vector<LineFeatureDef> lines_;
    struct Grids {
        std::unordered_map<std::uint32_t, BoxGrid> buildings, areas, lines;
    };
    std::shared_ptr<const Grids> grids_;
};

} // namespace spn::core
