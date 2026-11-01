#include <algorithm>
#include <cmath>
#include <tuple>
#include <spn/core/GeometryMath.h>
#include <spn/core/MapRender.h>

namespace spn::core {

namespace {

struct Viewport {
    const MinimapConfig& config;
    MapRect box; // screen bounding box including the margin
    [[nodiscard]] bool Meets(const MapRect& screenBox) const { return RectsIntersect(box, screenBox); }
};

std::vector<Vec2> ToScreen(const MinimapTransform& t, const std::vector<Vec2>& world) {
    std::vector<Vec2> out;
    out.reserve(world.size());
    for (const Vec2 p : world) {
        out.push_back(t.WorldToScreen(p));
    }
    return out;
}

bool AllFinite(const std::vector<Vec2>& pts) {
    return std::all_of(pts.begin(), pts.end(), [](Vec2 p) { return std::isfinite(p.x) && std::isfinite(p.y); });
}

bool Discovered(const WorldMapState* state, const ContentId& region, bool respect) {
    if (!respect || state == nullptr || region.IsEmpty()) {
        return true;
    }
    return state->Discovery(region) != DiscoveryState::Undiscovered;
}

double Importance(RoadClass c) { return static_cast<double>(kRoadClassCount) - static_cast<double>(static_cast<std::size_t>(c)); }

// Draw order: least important first, structures: tunnel below surface below bridge.
int StructureOrder(RoadStructure s) {
    switch (s) {
    case RoadStructure::Tunnel:
    case RoadStructure::Underpass: return 0;
    case RoadStructure::Surface: return 1;
    case RoadStructure::Bridge: return 2;
    }
    return 1;
}

int AreaOrder(AreaKind k) {
    switch (k) {
    case AreaKind::District: return 0;
    case AreaKind::Mountain: return 1;
    case AreaKind::Forest: return 2;
    case AreaKind::Green: return 3;
    case AreaKind::Park: return 4;
    case AreaKind::Industrial: return 5;
    case AreaKind::Parking: return 6;
    case AreaKind::Plaza: return 7;
    case AreaKind::Water: return 8;
    case AreaKind::Island: return 9;
    }
    return 0;
}

void ClipRoute(const Route& route, std::size_t first, std::size_t last, double alongFirst, bool fromAlong, const MinimapTransform& t, const MinimapConfig& vp,
               std::vector<std::vector<Vec2>>& out) {
    // Emits the part of segments [first, last) (optionally starting `alongFirst` metres into the first segment when fromAlong) as clipped polylines.
    std::vector<Vec2> current;
    const auto flush = [&] {
        if (current.size() >= 2) { out.push_back(current); }
        current.clear();
    };
    for (std::size_t i = first; i < last && i < route.segments.size(); ++i) {
        const RouteSegment& s = route.segments[i];
        if (s.kind != SegmentKind::Travel) { flush(); continue; }
        Vec2 a = s.from.xy;
        const Vec2 b = s.to.xy;
        if (fromAlong && i == first) {
            const double len = Distance(a, b);
            const double f = len > 0.0 ? std::clamp(alongFirst / len, 0.0, 1.0) : 0.0;
            a = Vec2{a.x + (b.x - a.x) * f, a.y + (b.y - a.y) * f};
        }
        const Vec2 sa = t.WorldToScreen(a);
        const Vec2 sb = t.WorldToScreen(b);
        Vec2 ca, cb;
        const bool visible = vp.shape == MinimapShape::Circle ? ClipSegmentToCircle(sa, sb, Vec2{0, 0}, vp.halfWidthPx, ca, cb)
                                                               : ClipSegmentToRect(sa, sb, MapRect{{-vp.halfWidthPx, -vp.halfHeightPx}, {vp.halfWidthPx, vp.halfHeightPx}}, ca, cb);
        if (!visible) { flush(); continue; }
        if (!current.empty() && !(current.back() == ca)) { flush(); } // the line left the viewport and came back
        if (current.empty()) { current.push_back(ca); }
        current.push_back(cb);
        if (!(cb == sb)) { flush(); }
    }
    flush();
}

} // namespace

Result<MapRenderFrame, MinimapError> BuildMapRenderFrame(const MapGeometry& geometry, const WorldMapState* mapState, const MinimapView& view, const MinimapConfig& viewport,
                                                          const MapRenderConfig& config, const RouteOverlay& overlay) {
    if (const auto bad = ValidateConfig(viewport)) {
        return *bad;
    }
    auto transform = MinimapTransform::Create(viewport, view);
    if (!transform) {
        return transform.Error();
    }
    const MinimapTransform& t = transform.Value();
    MapRenderFrame frame;
    frame.layer = t.Layer();
    frame.metresPerPixel = t.MetresPerPixel();
    frame.rotationRadians = t.RotationRadians();
    frame.northOnScreenRadians = t.NorthOnScreenRadians();
    frame.playerHeadingOnScreenRadians = NormalizeAngle(view.headingRadians - t.RotationRadians());
    MapRenderStats& stats = frame.stats;
    const double mpp = t.MetresPerPixel();

    const double reachM = (ViewportReachPx(viewport) + config.edgeMarginPx) * mpp;
    const MapRect worldRect{{view.player.xy.x - reachM, view.player.xy.y - reachM}, {view.player.xy.x + reachM, view.player.xy.y + reachM}};
    const double margin = config.edgeMarginPx;
    const Viewport vp{viewport, MapRect{{-viewport.halfWidthPx - margin, -viewport.halfHeightPx - margin}, {viewport.halfWidthPx + margin, viewport.halfHeightPx + margin}}};
    const double tol = config.lod.simplifyTolerancePx;
    const auto distToPlayer = [&](const MapRect& b) {
        const Vec2 c{(b.min.x + b.max.x) * 0.5, (b.min.y + b.max.y) * 0.5};
        return Distance(c, view.player.xy);
    };

    // --- roads ---
    struct RoadCand { std::uint32_t index; double importance; double dist; };
    std::vector<RoadCand> roads;
    const RoadNetwork& net = geometry.Roads();
    for (const std::uint32_t i : net.SegmentsInRect(view.player.layer, worldRect)) {
        ++stats.candidates;
        const RoadNetwork::SegmentInfo& info = net.Info(i);
        if (mpp > config.lod.roadMaxMetresPerPixel[static_cast<std::size_t>(info.roadClass)]) { ++stats.culledLod; continue; }
        if (!Discovered(mapState, net.Segments()[i].region, config.respectDiscovery)) { ++stats.culledDiscovery; continue; }
        roads.push_back(RoadCand{i, Importance(info.roadClass), distToPlayer(info.bounds)});
    }
    // keep the most important / nearest `maxRoads`
    std::sort(roads.begin(), roads.end(), [](const RoadCand& a, const RoadCand& b) { return std::make_tuple(-a.importance, a.dist, a.index) < std::make_tuple(-b.importance, b.dist, b.index); });
    if (roads.size() > config.maxRoads) {
        roads.resize(config.maxRoads);
        stats.truncated = true;
    }
    for (const RoadCand& c : roads) {
        const RoadNetwork::SegmentInfo& info = net.Info(c.index);
        std::vector<Vec2> screen = ToScreen(t, info.points);
        if (!AllFinite(screen) || !vp.Meets(BoundsOf(screen))) { ++stats.culledScreen; continue; }
        stats.pointsBefore += screen.size();
        screen = SimplifyPolyline(screen, tol);
        stats.pointsAfter += screen.size();
        RenderPolyline r;
        r.id = net.Segments()[c.index].id.Value();
        r.roadClass = info.roadClass;
        r.structure = info.structure;
        r.oneWay = info.direction != RoadDirection::Both;
        r.roundabout = info.roundabout;
        r.points = std::move(screen);
        frame.roads.push_back(std::move(r));
    }
    std::stable_sort(frame.roads.begin(), frame.roads.end(), [](const RenderPolyline& a, const RenderPolyline& b) {
        return std::make_tuple(StructureOrder(a.structure), -static_cast<int>(a.roadClass), a.id) < std::make_tuple(StructureOrder(b.structure), -static_cast<int>(b.roadClass), b.id);
    });

    // --- buildings ---
    struct PolyCand { std::uint32_t index; double dist; };
    std::vector<PolyCand> buildings;
    for (const std::uint32_t i : geometry.BuildingsInRect(view.player.layer, worldRect)) {
        ++stats.candidates;
        const BuildingDef& b = geometry.Buildings()[i];
        if (mpp > config.lod.buildingMaxMetresPerPixel) { ++stats.culledLod; continue; }
        if (!Discovered(mapState, b.region, config.respectDiscovery)) { ++stats.culledDiscovery; continue; }
        buildings.push_back(PolyCand{i, distToPlayer(BoundsOf(b.footprint))});
    }
    std::sort(buildings.begin(), buildings.end(), [](const PolyCand& a, const PolyCand& b) { return std::tie(a.dist, a.index) < std::tie(b.dist, b.index); });
    if (buildings.size() > config.maxBuildings) {
        buildings.resize(config.maxBuildings);
        stats.truncated = true;
    }
    for (const PolyCand& c : buildings) {
        const BuildingDef& b = geometry.Buildings()[c.index];
        std::vector<Vec2> screen = ToScreen(t, b.footprint);
        if (!AllFinite(screen) || !vp.Meets(BoundsOf(screen))) { ++stats.culledScreen; continue; }
        stats.pointsBefore += screen.size();
        screen = SimplifyPolyline(screen, tol);
        if (screen.size() < 3) { screen = ToScreen(t, b.footprint); } // never simplify a footprint below a triangle
        stats.pointsAfter += screen.size();
        RenderPolygon p;
        p.id = b.id.Value();
        p.isBuilding = true;
        p.category = b.category;
        p.points = std::move(screen);
        p.entrances = ToScreen(t, b.entrances);
        frame.buildings.push_back(std::move(p));
    }
    std::sort(frame.buildings.begin(), frame.buildings.end(), [](const RenderPolygon& a, const RenderPolygon& b) { return a.id < b.id; });

    // --- areas ---
    std::vector<PolyCand> areas;
    for (const std::uint32_t i : geometry.AreasInRect(view.player.layer, worldRect)) {
        ++stats.candidates;
        const AreaDef& a = geometry.Areas()[i];
        if (mpp > config.lod.areaMaxMetresPerPixel[static_cast<std::size_t>(a.kind)]) { ++stats.culledLod; continue; }
        if (!Discovered(mapState, a.region, config.respectDiscovery)) { ++stats.culledDiscovery; continue; }
        areas.push_back(PolyCand{i, distToPlayer(BoundsOf(a.outline))});
    }
    std::sort(areas.begin(), areas.end(), [](const PolyCand& a, const PolyCand& b) { return std::tie(a.dist, a.index) < std::tie(b.dist, b.index); });
    if (areas.size() > config.maxAreas) {
        areas.resize(config.maxAreas);
        stats.truncated = true;
    }
    for (const PolyCand& c : areas) {
        const AreaDef& a = geometry.Areas()[c.index];
        std::vector<Vec2> screen = ToScreen(t, a.outline);
        if (!AllFinite(screen) || !vp.Meets(BoundsOf(screen))) { ++stats.culledScreen; continue; }
        stats.pointsBefore += screen.size();
        std::vector<Vec2> simplified = SimplifyPolyline(screen, tol);
        if (simplified.size() >= 3) { screen = std::move(simplified); }
        stats.pointsAfter += screen.size();
        RenderPolygon p;
        p.id = a.id.Value();
        p.areaKind = a.kind;
        p.points = std::move(screen);
        frame.areas.push_back(std::move(p));
    }
    std::sort(frame.areas.begin(), frame.areas.end(), [&](const RenderPolygon& a, const RenderPolygon& b) {
        return std::make_tuple(AreaOrder(a.areaKind), a.id) < std::make_tuple(AreaOrder(b.areaKind), b.id);
    });

    // --- lines ---
    std::vector<PolyCand> lines;
    for (const std::uint32_t i : geometry.LinesInRect(view.player.layer, worldRect)) {
        ++stats.candidates;
        const LineFeatureDef& l = geometry.Lines()[i];
        if (mpp > config.lod.lineMaxMetresPerPixel[static_cast<std::size_t>(l.kind)]) { ++stats.culledLod; continue; }
        if (!Discovered(mapState, l.region, config.respectDiscovery)) { ++stats.culledDiscovery; continue; }
        lines.push_back(PolyCand{i, distToPlayer(BoundsOf(l.points))});
    }
    std::sort(lines.begin(), lines.end(), [](const PolyCand& a, const PolyCand& b) { return std::tie(a.dist, a.index) < std::tie(b.dist, b.index); });
    if (lines.size() > config.maxLines) {
        lines.resize(config.maxLines);
        stats.truncated = true;
    }
    for (const PolyCand& c : lines) {
        const LineFeatureDef& l = geometry.Lines()[c.index];
        std::vector<Vec2> screen = ToScreen(t, l.points);
        if (!AllFinite(screen) || !vp.Meets(BoundsOf(screen))) { ++stats.culledScreen; continue; }
        stats.pointsBefore += screen.size();
        screen = SimplifyPolyline(screen, tol);
        stats.pointsAfter += screen.size();
        RenderPolyline r;
        r.id = l.id.Value();
        r.lineKind = l.kind;
        r.points = std::move(screen);
        frame.lines.push_back(std::move(r));
    }
    std::sort(frame.lines.begin(), frame.lines.end(), [](const RenderPolyline& a, const RenderPolyline& b) { return a.id < b.id; });

    stats.emitted = frame.roads.size() + frame.buildings.size() + frame.areas.size() + frame.lines.size();

    // --- route overlay ---
    if (overlay.route != nullptr && !overlay.route->segments.empty() && overlay.route->segments.front().from.layer == view.player.layer) {
        const Route& route = *overlay.route;
        const std::size_t split = std::min(overlay.segmentIndex, route.segments.size() - 1);
        // travelled = segments before the current one + the current one up to the projection point
        Route head;
        head.segments.assign(route.segments.begin(), route.segments.begin() + static_cast<std::ptrdiff_t>(split));
        RouteSegment partial = route.segments[split];
        const double len = SegmentLength(partial);
        const double f = len > 0.0 ? std::clamp(overlay.alongSegmentMetres / len, 0.0, 1.0) : 0.0;
        if (f > 0.0) {
            partial.to.xy = Vec2{partial.from.xy.x + (partial.to.xy.x - partial.from.xy.x) * f, partial.from.xy.y + (partial.to.xy.y - partial.from.xy.y) * f};
            head.segments.push_back(partial);
        }
        ClipRoute(head, 0, head.segments.size(), 0.0, false, t, viewport, frame.routeTravelled);
        ClipRoute(route, split, route.segments.size(), overlay.alongSegmentMetres, true, t, viewport, frame.routeRemaining);
        const MapPosition dest = route.destination.position;
        if (dest.layer == view.player.layer && IsValidMapCoordinate(dest.xy)) {
            const Vec2 s = t.WorldToScreen(dest.xy);
            frame.destinationScreen = s;
            frame.destinationInside = InsideViewport(viewport, s);
            if (!frame.destinationInside) {
                frame.destinationIndicator = ProjectToViewportEdge(viewport, s, viewport.indicatorInsetPx);
            }
        }
    }
    return frame;
}

} // namespace spn::core
