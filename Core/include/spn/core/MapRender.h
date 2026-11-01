#pragma once

// MapRender: turns MapGeometry (roads, buildings, areas, lines) and an active route into screen-space draw data for the minimap / map screen.
// It draws nothing and knows no UI toolkit; a renderer (UMG, Slate, a canvas) consumes MapRenderFrame. It reuses MinimapTransform, so zoom,
// rotation (NorthUp / HeadingUp), the circle / rectangle viewport and the large-coordinate behaviour are exactly those of the minimap.
//
// Per frame it does NOT scan the world: the spatial indexes return only features whose bounding box meets the viewport, then
//   1. level of detail    a feature is dropped when the zoom is too far out for its kind (roads by class, buildings, areas, lines)
//   2. discovery          features tied to a region the player has not discovered are dropped (optional)
//   3. screen culling     features whose screen box misses the viewport are dropped
//   4. simplification     polylines / rings are reduced with Douglas-Peucker in PIXEL space (sub-pixel detail costs nothing to lose)
//   5. budget             if still above the per-kind cap, the least important / farthest features are dropped and `truncated` is set
// The route overlay is clipped to the viewport and split at the player's progress into "travelled" and "remaining".
//
// Screen space: pixels, origin = viewport centre, x right, y DOWN (Minimap.h). Output order is the draw order (first = bottom).
//
// ENGINEERING FOUNDATION (NightBuild02), not master canon. All thresholds in MapRenderConfig are placeholders (not tuned, not approved UI values).

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <spn/core/MapGeometry.h>
#include <spn/core/Minimap.h>
#include <spn/core/Navigation.h>
#include <spn/core/Routing.h>
#include <vector>

namespace spn::core {

inline constexpr std::size_t kAreaKindCount = 10;
inline constexpr std::size_t kLineKindCount = 4;

/// Per feature kind: the largest metres-per-pixel at which the kind is still drawn (larger = zoomed further out). Placeholder values.
struct MapRenderLod {
    std::array<double, kRoadClassCount> roadMaxMetresPerPixel = {400.0, 200.0, 300.0, 200.0, 120.0, 30.0, 16.0, 10.0, 10.0, 5.0};
    double buildingMaxMetresPerPixel = 8.0;
    std::array<double, kAreaKindCount> areaMaxMetresPerPixel = {2000.0, 400.0, 600.0, 300.0, 200.0, 100.0, 30.0, 40.0, 1500.0, 1000.0};
    std::array<double, kLineKindCount> lineMaxMetresPerPixel = {1000.0, 100.0, 2000.0, 60.0};
    double simplifyTolerancePx = 0.75;
};

struct MapRenderConfig {
    MapRenderLod lod;
    bool respectDiscovery = true;       ///< drop features whose region is Undiscovered (needs a WorldMapState)
    std::size_t maxRoads = 20000;
    std::size_t maxBuildings = 5000;
    std::size_t maxAreas = 2000;
    std::size_t maxLines = 2000;
    double edgeMarginPx = 2.0;          ///< features within this margin outside the viewport are still emitted (line width)
};

struct RenderPolyline {
    std::uint64_t id = 0;                 ///< road segment / line feature id
    RoadClass roadClass = RoadClass::Residential; ///< roads
    RoadStructure structure = RoadStructure::Surface;
    LineKind lineKind = LineKind::River;  ///< lines
    bool oneWay = false;                  ///< roads: vehicles only one direction (draw direction arrows)
    bool roundabout = false;
    std::vector<Vec2> points;             ///< screen space
};

struct RenderPolygon {
    std::uint64_t id = 0;
    AreaKind areaKind = AreaKind::Green;  ///< areas
    ContentId category;                   ///< buildings
    bool isBuilding = false;
    std::vector<Vec2> points;             ///< screen space ring (open)
    std::vector<Vec2> entrances;          ///< buildings: screen space
};

/// The route and the player's progress along it, as NavigationSession reports them.
struct RouteOverlay {
    const Route* route = nullptr;
    std::size_t segmentIndex = 0;
    double alongSegmentMetres = 0.0;
};

struct MapRenderStats {
    std::size_t candidates = 0;        ///< features returned by the spatial index (all kinds)
    std::size_t culledLod = 0;
    std::size_t culledDiscovery = 0;
    std::size_t culledScreen = 0;
    std::size_t emitted = 0;
    std::size_t pointsBefore = 0;      ///< vertices of the emitted features before simplification
    std::size_t pointsAfter = 0;
    bool truncated = false;            ///< a per-kind budget dropped features
};

struct MapRenderFrame {
    MapLayerId layer;
    double metresPerPixel = 1.0;
    double rotationRadians = 0.0;
    double northOnScreenRadians = 0.0;
    double playerHeadingOnScreenRadians = 0.0; ///< clockwise from screen-up: the direction arrow of the player marker (0 = up)
    std::vector<RenderPolygon> areas;
    std::vector<RenderPolyline> lines;
    std::vector<RenderPolygon> buildings;
    std::vector<RenderPolyline> roads;
    std::vector<std::vector<Vec2>> routeTravelled; ///< polylines clipped to the viewport
    std::vector<std::vector<Vec2>> routeRemaining;
    std::optional<Vec2> destinationScreen;         ///< unclipped screen position of the destination (the marker stays at the real target)
    bool destinationInside = false;
    std::optional<Vec2> destinationIndicator;      ///< on the viewport edge, pointing at an off-screen destination
    MapRenderStats stats;
};

[[nodiscard]] Result<MapRenderFrame, MinimapError> BuildMapRenderFrame(const MapGeometry& geometry, const WorldMapState* mapState, const MinimapView& view,
                                                                        const MinimapConfig& viewport, const MapRenderConfig& config, const RouteOverlay& overlay = {});

} // namespace spn::core
