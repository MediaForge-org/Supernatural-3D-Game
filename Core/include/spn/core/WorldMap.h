#pragma once

// World map domain model (portable, no engine types, no rendering).
//
//   WorldMapDefinition  immutable authored data: layers, regions, points of interest (POIs). Built once and validated by Create.
//                       The data will eventually come from SPN WorldForge; nothing here invents geography.
//   WorldMapState       mutable runtime state on top of a shared definition: discovery, player markers, system markers,
//                       temporary markers; spatial queries; versioned save format "SPN-MAPSTATE 1".
//
// Coordinates: metres in the plane of one MapLayer, x = east, y = north (right-handed, "north-up" maps have +y up). Doubles keep
// sub-micrometre resolution at the +-10,000 km limit (kMaxMapCoordinateMetres). Queries and the minimap subtract in double first
// and only then narrow, so large absolute positions do not cost precision in the result. A layer is an independent plane: the
// surface, an underground region, one floor of an interior or another realm. Additional islands are regions and/or layers of the
// same kind; nothing assumes a single connected landmass.
//
// ENGINEERING FOUNDATION (M2B1), not master canon. Master consultation found no map, minimap or waypoint specification
// (Docs/M2B1/GAME_SYSTEMS_ARCHITECTURE.md, "Master basis"). Discovery states, the visibility rules and every limit below are
// proposals for review.

#include <cstddef>
#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <spn/core/ContentId.h>
#include <spn/core/Result.h>
#include <spn/core/SpatialGrid.h>
#include <spn/core/StrongId.h>
#include <spn/core/Vec.h>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace spn::core {

struct MapLayerTag;
struct MapMarkerTag;
/// Id of a map layer; 0 is the null id. Allocated by the (authored) definition, stable across saves.
using MapLayerId = StrongId<MapLayerTag, std::uint32_t>;
/// Id of a runtime marker; 0 is the null id. Allocated by WorldMapState (monotonic, never reused, saved).
using MarkerId = StrongId<MapMarkerTag, std::uint64_t>;

inline constexpr std::size_t kMaxMapLabelLength = 128;
inline constexpr std::size_t kMaxMapMarkers = 1'000'000; ///< parser/store limit against absurd counts, not a design target

/// Well-known marker categories the Core itself needs. All other categories are data (any valid ContentId).
[[nodiscard]] const ContentId& PlayerWaypointCategory(); ///< base:marker.player_waypoint

struct MapRect {
    Vec2 min;
    Vec2 max;
    [[nodiscard]] bool Contains(Vec2 p) const noexcept { return p.x >= min.x && p.x <= max.x && p.y >= min.y && p.y <= max.y; }
    [[nodiscard]] double Area() const noexcept { return (max.x - min.x) * (max.y - min.y); }
    friend bool operator==(const MapRect&, const MapRect&) = default;
};

/// A point on one layer.
struct MapPosition {
    MapLayerId layer;
    Vec2 xy;
    friend bool operator==(const MapPosition&, const MapPosition&) = default;
};

/// Straight-line distance in metres; nullopt if the layers differ (distances across layers need a route through a transition).
[[nodiscard]] std::optional<double> PlanarDistance(const MapPosition& a, const MapPosition& b) noexcept;

// --- Errors -------------------------------------------------------------------------------------------------------------------

enum class MapErrorCode : std::uint8_t {
    // Definition
    InvalidId, DuplicateLayer, DuplicateId, InvalidBounds, UnknownLayer, OutsideLayerBounds, InvalidPosition, InvalidLabel, InvalidCell,
    // Runtime commands
    UnknownMarker, UnknownEntry, InvalidMarker, TooManyMarkers, TooManyPlayerMarkers, InvalidQuery,
    // Persistence
    InvalidHeader, UnsupportedVersion, TruncatedData, TrailingData, MalformedLine, InvalidToken, CountMismatch, DuplicateMarker, Io,
};
[[nodiscard]] std::string_view ToString(MapErrorCode code) noexcept;
struct MapError {
    MapErrorCode code;
    std::size_t line = 0; ///< 1-based line in saved data, 0 = n/a
    std::string detail;
    friend bool operator==(const MapError&, const MapError&) = default;
};

// --- Definition ---------------------------------------------------------------------------------------------------------------

enum class MapLayerKind : std::uint8_t { Surface, Underground, Interior, Realm };
[[nodiscard]] std::string_view ToString(MapLayerKind kind) noexcept;

struct MapLayerDef {
    MapLayerId id;
    ContentId key;    ///< stable text key, unique
    MapLayerKind kind = MapLayerKind::Surface;
    ContentId group;  ///< optional: layers that belong together (the floors of one building, the levels of one cave system)
    std::int32_t level = 0; ///< order inside the group (floor number, depth); no unit implied
    MapRect bounds;   ///< map bounds of the layer; every region and POI of the layer lies inside
};

/// A named area on one layer (island, district, interior volume, ...). Rectangular bounds are an index, not the true outline:
/// exact shapes arrive with WorldForge data. Regions may overlap (a district inside a city).
struct MapRegionDef {
    ContentId id; ///< unique across regions AND POIs (one discovery namespace)
    MapLayerId layer;
    MapRect bounds;
    ContentId category;
    std::string label;
};

enum class MapVisibility : std::uint8_t {
    Hidden,    ///< never shown
    Always,    ///< shown regardless of discovery
    WhenKnown, ///< shown once discovery >= Known
};

struct PointOfInterestDef {
    ContentId id; ///< unique across regions AND POIs
    ContentId category;
    MapPosition position;
    std::string label;
    MapVisibility visibility = MapVisibility::WhenKnown;
};

class WorldMapDefinition {
public:
    /// Validates everything: non-null unique ids, finite bounds with min <= max, regions and POIs on known layers and inside
    /// the layer bounds, valid coordinates, labels within kMaxMapLabelLength without control characters. Stores layers, regions
    /// and POIs sorted by id (the input order does not matter). `gridCellMetres` only tunes the spatial index.
    [[nodiscard]] static Result<WorldMapDefinition, MapError> Create(std::vector<MapLayerDef> layers, std::vector<MapRegionDef> regions,
                                                                      std::vector<PointOfInterestDef> pois, double gridCellMetres = 256.0);

    [[nodiscard]] const std::vector<MapLayerDef>& Layers() const noexcept { return layers_; }
    [[nodiscard]] const std::vector<MapRegionDef>& Regions() const noexcept { return regions_; }
    [[nodiscard]] const std::vector<PointOfInterestDef>& Pois() const noexcept { return pois_; }
    [[nodiscard]] const MapLayerDef* FindLayer(MapLayerId id) const;
    [[nodiscard]] const MapLayerDef* FindLayer(const ContentId& key) const;
    [[nodiscard]] const MapRegionDef* FindRegion(const ContentId& id) const;
    [[nodiscard]] const PointOfInterestDef* FindPoi(const ContentId& id) const;
    [[nodiscard]] bool Knows(const ContentId& id) const { return FindRegion(id) != nullptr || FindPoi(id) != nullptr; }
    [[nodiscard]] double GridCellMetres() const noexcept { return gridCell_; }

    /// Regions containing the position (inclusive bounds), smallest area first, ties by id. Linear in the regions of the layer.
    [[nodiscard]] std::vector<const MapRegionDef*> RegionsAt(const MapPosition& position) const;
    /// POIs on `layer` within `radius` of `center`, as indices into Pois() ascending. Uses the spatial index.
    [[nodiscard]] std::vector<std::size_t> PoiIndicesInRadius(MapLayerId layer, Vec2 center, double radius) const;
    [[nodiscard]] std::vector<std::size_t> PoiIndicesInRect(MapLayerId layer, const MapRect& rect) const;

private:
    WorldMapDefinition() = default;
    double gridCell_ = 256.0;
    std::vector<MapLayerDef> layers_;
    std::vector<MapRegionDef> regions_;
    std::vector<PointOfInterestDef> pois_;
    std::unordered_map<std::uint32_t, SpatialGrid> poiGrids_; // per layer; key = index into pois_
};

// --- Runtime state ------------------------------------------------------------------------------------------------------------

/// Monotonic: states only go up (Undiscovered < Known < Visited). Proposal.
enum class DiscoveryState : std::uint8_t {
    Undiscovered, ///< not on the player's map
    Known,        ///< location known (told, read, seen from afar)
    Visited,      ///< the player has been there
};
[[nodiscard]] std::string_view ToString(DiscoveryState state) noexcept;

enum class MarkerPersistence : std::uint8_t { Persistent, Temporary };
enum class MarkerOrigin : std::uint8_t { System, Player };

/// What the caller specifies when adding a marker.
struct MarkerSpec {
    MapPosition position;
    ContentId category; ///< required
    std::string label;
    MarkerPersistence persistence = MarkerPersistence::Persistent;
    MarkerOrigin origin = MarkerOrigin::Player;
    bool visible = true;
    ContentId linkedPoi;               ///< optional: a POI of the definition this marker annotates
    std::optional<double> expiresAt;   ///< Temporary only: game-time seconds; PruneExpired(now) removes it once now >= expiresAt
};

struct Marker : MarkerSpec {
    MarkerId id;
};

struct MapStateConfig {
    double gridCellMetres = 256.0;
    std::optional<std::size_t> maxPlayerMarkers; ///< unset = unlimited; the limit is a design decision that is still open
};

/// Which entries a query returns. Default: everything visible.
struct MapFilter {
    std::set<ContentId> categories;  ///< empty = all categories
    bool includePois = true;
    bool includePersistentMarkers = true;
    bool includeTemporaryMarkers = true;
    bool includePlayerMarkers = true;
    bool includeSystemMarkers = true;
    bool includeHidden = false;      ///< also return entries whose visibility rule hides them
};

enum class MapHitSource : std::uint8_t { PointOfInterest, Marker };

struct MapQueryHit {
    MapHitSource source = MapHitSource::PointOfInterest;
    const PointOfInterestDef* poi = nullptr; ///< set for POI hits
    const Marker* marker = nullptr;          ///< set for marker hits
    Vec2 position;
    double distanceMetres = 0.0;
    /// Pointers stay valid until the state is next modified (or destroyed).
    [[nodiscard]] const ContentId& Category() const { return poi != nullptr ? poi->category : marker->category; }
};

class WorldMapState {
public:
    explicit WorldMapState(std::shared_ptr<const WorldMapDefinition> definition, MapStateConfig config = {});

    [[nodiscard]] const WorldMapDefinition& Definition() const noexcept { return *definition_; }

    // Markers
    /// Validates (layer known, position inside the layer bounds, category set, label rules, linkedPoi known, expiresAt only for
    /// Temporary and finite, player-marker limit) and adds. Ids are never reused.
    [[nodiscard]] Result<MarkerId, MapError> AddMarker(MarkerSpec spec);
    /// Convenience: persistent player marker of PlayerWaypointCategory().
    [[nodiscard]] Result<MarkerId, MapError> AddPlayerWaypoint(MapPosition position, std::string label = {});
    [[nodiscard]] std::optional<MapError> RemoveMarker(MarkerId id);
    [[nodiscard]] std::optional<MapError> MoveMarker(MarkerId id, MapPosition position);
    [[nodiscard]] std::optional<MapError> SetMarkerVisible(MarkerId id, bool visible);
    [[nodiscard]] const Marker* FindMarker(MarkerId id) const;
    [[nodiscard]] std::size_t MarkerCount() const noexcept { return markers_.size(); }
    /// Removes Temporary markers with expiresAt <= now; returns how many. Non-finite `now` removes nothing.
    std::size_t PruneExpired(double now);

    // Discovery
    [[nodiscard]] DiscoveryState Discovery(const ContentId& id) const;
    /// Raises the state of a region or POI (never lowers it). True if it changed; UnknownEntry if the definition lacks the id.
    [[nodiscard]] Result<bool, MapError> Discover(const ContentId& id, DiscoveryState state);
    /// Raises every POI within `radius` of the position to at least `state`; returns how many changed.
    std::size_t DiscoverPoisAround(const MapPosition& position, double radius, DiscoveryState state = DiscoveryState::Known);
    /// Raises every region containing the position to at least `state` (Visited by default); returns how many changed.
    std::size_t DiscoverRegionsAt(const MapPosition& position, DiscoveryState state = DiscoveryState::Visited);

    [[nodiscard]] bool IsVisible(const PointOfInterestDef& poi) const;

    // Queries (deterministic order: distance, then POIs before markers, then POI id / MarkerId)
    /// Entries on `center.layer` within `radius`. InvalidQuery for an unknown layer, a non-finite center or a negative/non-finite radius.
    [[nodiscard]] Result<std::vector<MapQueryHit>, MapError> QueryRadius(const MapPosition& center, double radius, const MapFilter& filter = {}) const;
    [[nodiscard]] Result<std::vector<MapQueryHit>, MapError> QueryRect(MapLayerId layer, const MapRect& rect, const MapFilter& filter = {}) const;

    // Persistence: only Persistent markers, the marker id counter and discovery are saved. Temporary markers are runtime only.
    [[nodiscard]] std::string Serialize() const;
    /// Validates everything against `definition` (layers, bounds, ids, counts, canonical spelling). Never partially succeeds.
    [[nodiscard]] static Result<WorldMapState, MapError> Deserialize(std::string_view text, std::shared_ptr<const WorldMapDefinition> definition,
                                                                      MapStateConfig config = {});

    /// Compares saveable state (persistent markers, counter, discovery); runtime-only temporary markers are ignored.
    [[nodiscard]] bool SameSavedState(const WorldMapState& other) const;

private:
    struct LayerIndex {
        SpatialGrid grid;
    };
    [[nodiscard]] std::optional<MapError> ValidateSpec(const MarkerSpec& spec) const;
    void Index(const Marker& marker);
    [[nodiscard]] bool Passes(const Marker& marker, const MapFilter& filter) const;
    [[nodiscard]] bool Passes(const PointOfInterestDef& poi, const MapFilter& filter) const;
    [[nodiscard]] std::vector<MapQueryHit> Collect(MapLayerId layer, const std::vector<std::size_t>& poiIndices, const std::vector<std::uint64_t>& markerKeys,
                                                   Vec2 center, const MapFilter& filter) const;

    std::shared_ptr<const WorldMapDefinition> definition_;
    MapStateConfig config_;
    std::uint64_t nextMarker_ = 1;
    std::size_t playerMarkers_ = 0;
    std::map<std::uint64_t, Marker> markers_;
    std::unordered_map<std::uint32_t, LayerIndex> markerGrids_;
    std::map<ContentId, DiscoveryState> discovery_; // only non-Undiscovered entries
};

} // namespace spn::core
