#include <algorithm>
#include <cmath>
#include <limits>
#include <spn/core/GeometryMath.h>
#include <spn/core/MapGeometry.h>

namespace spn::core {

namespace {

GeometryError Err(GeometryErrorCode code, std::uint64_t id, std::string detail = {}) { return GeometryError{code, id, std::move(detail)}; }

bool ValidLabel(const std::string& label) {
    if (label.size() > kMaxMapLabelLength) {
        return false;
    }
    return std::none_of(label.begin(), label.end(), [](char c) { return static_cast<unsigned char>(c) < 0x20 || c == 0x7f; });
}

std::optional<GeometryError> CheckCommon(const WorldMapDefinition& def, MapLayerId layer, std::uint64_t id, const ContentId& region, const std::string& label,
                                         const std::vector<Vec2>& points, const MapLayerDef** layerOut) {
    const MapLayerDef* l = def.FindLayer(layer);
    if (l == nullptr) {
        return Err(GeometryErrorCode::UnknownLayer, id);
    }
    *layerOut = l;
    if (!region.IsEmpty() && def.FindRegion(region) == nullptr) {
        return Err(GeometryErrorCode::UnknownRegion, id, region.Value());
    }
    if (!ValidLabel(label)) {
        return Err(GeometryErrorCode::InvalidLabel, id);
    }
    if (points.size() > kMaxFeaturePoints) {
        return Err(GeometryErrorCode::TooManyPoints, id);
    }
    for (const Vec2 p : points) {
        if (!IsValidMapCoordinate(p)) {
            return Err(GeometryErrorCode::InvalidCoordinate, id);
        }
        if (!l->bounds.Contains(p)) {
            return Err(GeometryErrorCode::OutsideLayerBounds, id);
        }
    }
    return std::nullopt;
}

template <typename T, typename Id> std::optional<GeometryError> SortAndCheckIds(std::vector<T>& items, Id T::*idMember, const char* what) {
    std::sort(items.begin(), items.end(), [&](const T& a, const T& b) { return a.*idMember < b.*idMember; });
    for (std::size_t i = 0; i < items.size(); ++i) {
        if ((items[i].*idMember).IsNull()) {
            return Err(GeometryErrorCode::InvalidId, 0, what);
        }
        if (i > 0 && (items[i - 1].*idMember) == (items[i].*idMember)) {
            return Err(GeometryErrorCode::DuplicateId, (items[i].*idMember).Value(), what);
        }
    }
    return std::nullopt;
}

template <typename T, typename Id> Result<std::vector<T>, GeometryError> MergeSorted(const std::vector<T>& a, const std::vector<T>& b, Id T::*idMember) {
    std::vector<T> out;
    out.reserve(a.size() + b.size());
    out.insert(out.end(), a.begin(), a.end());
    for (const T& item : b) {
        const auto same = std::find_if(a.begin(), a.end(), [&](const T& x) { return x.*idMember == item.*idMember; });
        if (same == a.end()) {
            out.push_back(item);
        } else if (!(*same == item)) {
            return Err(GeometryErrorCode::MergeConflict, (item.*idMember).Value());
        }
    }
    std::sort(out.begin(), out.end(), [&](const T& x, const T& y) { return x.*idMember < y.*idMember; });
    return out;
}

} // namespace

Result<MapGeometryData, GeometryError> MergeGeometryData(const MapGeometryData& a, const MapGeometryData& b) {
    MapGeometryData out;
    auto nodes = MergeSorted(a.nodes, b.nodes, &RoadNodeDef::id);
    if (!nodes) { return nodes.Error(); }
    auto segments = MergeSorted(a.segments, b.segments, &RoadSegmentDef::id);
    if (!segments) { return segments.Error(); }
    auto buildings = MergeSorted(a.buildings, b.buildings, &BuildingDef::id);
    if (!buildings) { return buildings.Error(); }
    auto areas = MergeSorted(a.areas, b.areas, &AreaDef::id);
    if (!areas) { return areas.Error(); }
    auto lines = MergeSorted(a.lines, b.lines, &LineFeatureDef::id);
    if (!lines) { return lines.Error(); }
    out.nodes = std::move(nodes).Value();
    out.segments = std::move(segments).Value();
    out.buildings = std::move(buildings).Value();
    out.areas = std::move(areas).Value();
    out.lines = std::move(lines).Value();
    out.restrictions = a.restrictions;
    for (const TurnRestriction& r : b.restrictions) {
        if (std::find(out.restrictions.begin(), out.restrictions.end(), r) == out.restrictions.end()) {
            out.restrictions.push_back(r);
        }
    }
    return out;
}

Result<MapGeometry, GeometryError> MapGeometry::Create(const WorldMapDefinition& def, MapGeometryData data, double gridCellMetres) {
    if (!std::isfinite(gridCellMetres) || gridCellMetres <= 0.0) {
        return Err(GeometryErrorCode::InvalidGridCell, 0);
    }
    MapGeometry g;
    const MapLayerDef* layer = nullptr;

    // roads: layer + bounds + region checks first (RoadNetwork validates the graph itself)
    std::unordered_map<std::uint64_t, MapLayerId> nodeLayer; // first definition wins; duplicate ids are rejected by RoadNetwork::Create
    nodeLayer.reserve(data.nodes.size());
    for (const RoadNodeDef& n : data.nodes) {
        if (const auto e = CheckCommon(def, n.position.layer, n.id.Value(), ContentId{}, {}, {n.position.xy}, &layer)) { return *e; }
        nodeLayer.emplace(n.id.Value(), n.position.layer);
    }
    for (const RoadSegmentDef& s : data.segments) {
        if (!s.region.IsEmpty() && def.FindRegion(s.region) == nullptr) {
            return Err(GeometryErrorCode::UnknownRegion, s.id.Value(), s.region.Value());
        }
        if (!ValidLabel(s.name)) {
            return Err(GeometryErrorCode::InvalidLabel, s.id.Value());
        }
        // shape points must lie inside the layer of the segment's nodes
        const auto from = nodeLayer.find(s.from.Value());
        if (from != nodeLayer.end()) {
            const MapLayerDef* l = def.FindLayer(from->second);
            for (const Vec2 p : s.shape) {
                if (l != nullptr && IsValidMapCoordinate(p) && !l->bounds.Contains(p)) {
                    return Err(GeometryErrorCode::OutsideLayerBounds, s.id.Value(), "shape");
                }
            }
        }
    }
    auto roads = RoadNetwork::Create(std::move(data.nodes), std::move(data.segments), std::move(data.restrictions), gridCellMetres);
    if (!roads) {
        return roads.Error();
    }
    g.roads_ = std::make_shared<const RoadNetwork>(std::move(roads).Value());

    if (const auto e = SortAndCheckIds(data.buildings, &BuildingDef::id, "building")) { return *e; }
    for (const BuildingDef& b : data.buildings) {
        if (const auto e = CheckCommon(def, b.layer, b.id.Value(), b.region, b.label, b.footprint, &layer)) { return *e; }
        if (!IsSimpleRing(b.footprint)) {
            return Err(GeometryErrorCode::InvalidPolygon, b.id.Value(), "footprint");
        }
        if (b.heightMetres.has_value() && !(std::isfinite(*b.heightMetres) && *b.heightMetres >= 0.0)) {
            return Err(GeometryErrorCode::InvalidCoordinate, b.id.Value(), "height");
        }
        for (const Vec2 e : b.entrances) {
            if (!IsValidMapCoordinate(e)) {
                return Err(GeometryErrorCode::InvalidCoordinate, b.id.Value(), "entrance");
            }
            double nearest = std::numeric_limits<double>::infinity();
            for (std::size_t i = 0; i < b.footprint.size(); ++i) {
                nearest = std::min(nearest, ProjectPointToSegment(e, b.footprint[i], b.footprint[(i + 1) % b.footprint.size()]).distance);
            }
            if (nearest > 1.0) {
                return Err(GeometryErrorCode::InvalidEntrance, b.id.Value(), "entrance is not on the footprint boundary");
            }
        }
    }
    if (const auto e = SortAndCheckIds(data.areas, &AreaDef::id, "area")) { return *e; }
    for (const AreaDef& a : data.areas) {
        if (const auto e = CheckCommon(def, a.layer, a.id.Value(), a.region, a.label, a.outline, &layer)) { return *e; }
        if (!IsSimpleRing(a.outline)) {
            return Err(GeometryErrorCode::InvalidPolygon, a.id.Value(), "outline");
        }
        if (a.elevationMetres.has_value() && !std::isfinite(*a.elevationMetres)) {
            return Err(GeometryErrorCode::InvalidCoordinate, a.id.Value(), "elevation");
        }
    }
    if (const auto e = SortAndCheckIds(data.lines, &LineFeatureDef::id, "line")) { return *e; }
    for (const LineFeatureDef& l : data.lines) {
        if (const auto e = CheckCommon(def, l.layer, l.id.Value(), l.region, l.label, l.points, &layer)) { return *e; }
        if (l.points.size() < 2 || !(PolylineLength(l.points) > 0.0)) {
            return Err(GeometryErrorCode::DegenerateGeometry, l.id.Value());
        }
        if (l.elevationMetres.has_value() && !std::isfinite(*l.elevationMetres)) {
            return Err(GeometryErrorCode::InvalidCoordinate, l.id.Value(), "elevation");
        }
    }

    auto grids = std::make_shared<Grids>();
    const auto add = [&](std::unordered_map<std::uint32_t, BoxGrid>& target, MapLayerId layerId, std::size_t i, const std::vector<Vec2>& pts) {
        target.try_emplace(layerId.Value(), gridCellMetres).first->second.Insert(static_cast<std::uint32_t>(i), BoundsOf(pts));
    };
    for (std::size_t i = 0; i < data.buildings.size(); ++i) { add(grids->buildings, data.buildings[i].layer, i, data.buildings[i].footprint); }
    for (std::size_t i = 0; i < data.areas.size(); ++i) { add(grids->areas, data.areas[i].layer, i, data.areas[i].outline); }
    for (std::size_t i = 0; i < data.lines.size(); ++i) { add(grids->lines, data.lines[i].layer, i, data.lines[i].points); }
    g.buildings_ = std::move(data.buildings);
    g.areas_ = std::move(data.areas);
    g.lines_ = std::move(data.lines);
    g.grids_ = std::move(grids);
    return g;
}

const BuildingDef* MapGeometry::FindBuilding(BuildingId id) const {
    const auto it = std::lower_bound(buildings_.begin(), buildings_.end(), id, [](const BuildingDef& b, BuildingId v) { return b.id < v; });
    return it != buildings_.end() && it->id == id ? &*it : nullptr;
}

const AreaDef* MapGeometry::FindArea(MapAreaId id) const {
    const auto it = std::lower_bound(areas_.begin(), areas_.end(), id, [](const AreaDef& a, MapAreaId v) { return a.id < v; });
    return it != areas_.end() && it->id == id ? &*it : nullptr;
}

namespace {
std::vector<std::uint32_t> QueryGrid(const std::unordered_map<std::uint32_t, BoxGrid>& grids, MapLayerId layer, const MapRect& rect) {
    std::vector<std::uint32_t> out;
    const auto it = grids.find(layer.Value());
    if (it != grids.end()) {
        it->second.Query(rect, out);
    }
    return out;
}
} // namespace

std::vector<std::uint32_t> MapGeometry::BuildingsInRect(MapLayerId layer, const MapRect& rect) const { return QueryGrid(grids_->buildings, layer, rect); }
std::vector<std::uint32_t> MapGeometry::AreasInRect(MapLayerId layer, const MapRect& rect) const { return QueryGrid(grids_->areas, layer, rect); }
std::vector<std::uint32_t> MapGeometry::LinesInRect(MapLayerId layer, const MapRect& rect) const { return QueryGrid(grids_->lines, layer, rect); }

const BuildingDef* MapGeometry::BuildingAt(const MapPosition& position) const {
    const BuildingDef* best = nullptr;
    double bestArea = 0.0;
    for (const std::uint32_t i : BuildingsInRect(position.layer, MapRect{position.xy, position.xy})) {
        const BuildingDef& b = buildings_[i];
        if (!PolygonContains(b.footprint, position.xy)) {
            continue;
        }
        const double area = PolygonArea(b.footprint);
        if (best == nullptr || area < bestArea) {
            best = &b;
            bestArea = area;
        }
    }
    return best;
}

double MapGeometry::WetLengthMetres(MapLayerId layer, Vec2 a, Vec2 b) const {
    const MapRect box{{std::min(a.x, b.x), std::min(a.y, b.y)}, {std::max(a.x, b.x), std::max(a.y, b.y)}};
    std::vector<const AreaDef*> water;
    std::vector<const AreaDef*> islands;
    std::vector<double> breaks{0.0, 1.0};
    for (const std::uint32_t i : AreasInRect(layer, box)) {
        const AreaDef& area = areas_[i];
        if (area.kind != AreaKind::Water && area.kind != AreaKind::Island) {
            continue;
        }
        (area.kind == AreaKind::Water ? water : islands).push_back(&area);
        for (std::size_t k = 0, n = area.outline.size(); k < n; ++k) {
            double t = 0.0;
            if (SegmentIntersectionParameter(a, b, area.outline[k], area.outline[(k + 1) % n], t)) {
                breaks.push_back(t);
            }
        }
    }
    if (water.empty()) {
        return 0.0;
    }
    std::sort(breaks.begin(), breaks.end());
    const double total = Distance(a, b);
    double wet = 0.0;
    for (std::size_t i = 0; i + 1 < breaks.size(); ++i) {
        const double t0 = breaks[i];
        const double t1 = breaks[i + 1];
        if (t1 <= t0) {
            continue;
        }
        const double tm = 0.5 * (t0 + t1);
        const Vec2 mid{a.x + (b.x - a.x) * tm, a.y + (b.y - a.y) * tm};
        const bool inWater = std::any_of(water.begin(), water.end(), [&](const AreaDef* w) { return PolygonContains(w->outline, mid); });
        const bool onIsland = std::any_of(islands.begin(), islands.end(), [&](const AreaDef* w) { return PolygonContains(w->outline, mid); });
        if (inWater && !onIsland) {
            wet += (t1 - t0) * total;
        }
    }
    return wet;
}

bool MapGeometry::ConnectorBlocked(MapLayerId layer, Vec2 a, Vec2 b, double maxWetMetres) const {
    const MapRect box{{std::min(a.x, b.x), std::min(a.y, b.y)}, {std::max(a.x, b.x), std::max(a.y, b.y)}};
    if (Distance(a, b) > maxWetMetres) { // a connector within the wet tolerance (a step from a bridge deck) is not a crossing
        for (const std::uint32_t i : LinesInRect(layer, box)) {
            if (BlocksConnectors(lines_[i].kind) && SegmentCrossesPolyline(a, b, lines_[i].points)) {
                return true;
            }
        }
    }
    return WetLengthMetres(layer, a, b) > maxWetMetres;
}

MapGeometryData MapGeometry::ToData() const {
    MapGeometryData d;
    d.nodes = roads_->Nodes();
    d.segments = roads_->Segments();
    d.restrictions = roads_->Restrictions();
    d.buildings = buildings_;
    d.areas = areas_;
    d.lines = lines_;
    return d;
}

} // namespace spn::core
