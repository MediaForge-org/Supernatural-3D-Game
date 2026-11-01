#include <algorithm>
#include <cmath>
#include <tuple>
#include <spn/core/GeometryMath.h>
#include <spn/core/MapGeometry.h>
#include <spn/core/Navigation.h>

namespace spn::core {

std::string_view ToString(GeometryErrorCode code) noexcept {
    switch (code) {
    case GeometryErrorCode::InvalidId: return "InvalidId";
    case GeometryErrorCode::DuplicateId: return "DuplicateId";
    case GeometryErrorCode::UnknownLayer: return "UnknownLayer";
    case GeometryErrorCode::OutsideLayerBounds: return "OutsideLayerBounds";
    case GeometryErrorCode::InvalidCoordinate: return "InvalidCoordinate";
    case GeometryErrorCode::UnknownNode: return "UnknownNode";
    case GeometryErrorCode::UnknownSegment: return "UnknownSegment";
    case GeometryErrorCode::LayerMismatch: return "LayerMismatch";
    case GeometryErrorCode::DegenerateGeometry: return "DegenerateGeometry";
    case GeometryErrorCode::InvalidPolygon: return "InvalidPolygon";
    case GeometryErrorCode::InvalidEntrance: return "InvalidEntrance";
    case GeometryErrorCode::InvalidAccess: return "InvalidAccess";
    case GeometryErrorCode::UnknownRegion: return "UnknownRegion";
    case GeometryErrorCode::InvalidLabel: return "InvalidLabel";
    case GeometryErrorCode::TooManyPoints: return "TooManyPoints";
    case GeometryErrorCode::RestrictionNotAdjacent: return "RestrictionNotAdjacent";
    case GeometryErrorCode::MergeConflict: return "MergeConflict";
    case GeometryErrorCode::InvalidGridCell: return "InvalidGridCell";
    }
    return "Unknown";
}

std::string_view ToString(RoadClass roadClass) noexcept {
    switch (roadClass) {
    case RoadClass::Motorway: return "Motorway";
    case RoadClass::MotorwayLink: return "MotorwayLink";
    case RoadClass::Expressway: return "Expressway";
    case RoadClass::Primary: return "Primary";
    case RoadClass::Secondary: return "Secondary";
    case RoadClass::Residential: return "Residential";
    case RoadClass::Service: return "Service";
    case RoadClass::FieldTrack: return "FieldTrack";
    case RoadClass::ForestTrack: return "ForestTrack";
    case RoadClass::Footway: return "Footway";
    }
    return "Unknown";
}

std::string_view ToString(RoadStructure structure) noexcept {
    switch (structure) {
    case RoadStructure::Surface: return "Surface";
    case RoadStructure::Bridge: return "Bridge";
    case RoadStructure::Tunnel: return "Tunnel";
    case RoadStructure::Underpass: return "Underpass";
    }
    return "Unknown";
}

std::string_view ToString(AreaKind kind) noexcept {
    switch (kind) {
    case AreaKind::Water: return "Water";
    case AreaKind::Island: return "Island";
    case AreaKind::Forest: return "Forest";
    case AreaKind::Park: return "Park";
    case AreaKind::Green: return "Green";
    case AreaKind::Industrial: return "Industrial";
    case AreaKind::Parking: return "Parking";
    case AreaKind::Plaza: return "Plaza";
    case AreaKind::District: return "District";
    case AreaKind::Mountain: return "Mountain";
    }
    return "Unknown";
}

std::string_view ToString(LineKind kind) noexcept {
    switch (kind) {
    case LineKind::River: return "River";
    case LineKind::Stream: return "Stream";
    case LineKind::Coastline: return "Coastline";
    case LineKind::Contour: return "Contour";
    }
    return "Unknown";
}

std::uint8_t DefaultAccess(RoadClass roadClass) noexcept {
    switch (roadClass) {
    case RoadClass::Motorway:
    case RoadClass::MotorwayLink:
    case RoadClass::Expressway: return kAccessVehicle;
    case RoadClass::Footway: return kAccessFoot;
    default: return static_cast<std::uint8_t>(kAccessFoot | kAccessVehicle);
    }
}

// --- BoxGrid ---------------------------------------------------------------------------------------------------------------------

namespace {
constexpr std::int64_t kMaxCellsPerFeature = 1024;
}

std::size_t BoxGrid::CellHash::operator()(const CellKey& c) const noexcept {
    const auto a = static_cast<std::uint64_t>(c.x);
    const auto b = static_cast<std::uint64_t>(c.y);
    return static_cast<std::size_t>((a * 0x9E3779B97F4A7C15ULL) ^ (b + 0x7F4A7C15ULL + (a << 6) + (a >> 2)));
}

BoxGrid::BoxGrid(double cellMetres) : cell_(std::isfinite(cellMetres) && cellMetres > 0.0 ? cellMetres : 256.0) {}

void BoxGrid::Insert(std::uint32_t key, const MapRect& bounds) {
    bounds_[key] = bounds;
    const auto cx0 = static_cast<std::int64_t>(std::floor(bounds.min.x / cell_));
    const auto cx1 = static_cast<std::int64_t>(std::floor(bounds.max.x / cell_));
    const auto cy0 = static_cast<std::int64_t>(std::floor(bounds.min.y / cell_));
    const auto cy1 = static_cast<std::int64_t>(std::floor(bounds.max.y / cell_));
    if ((cx1 - cx0 + 1) * (cy1 - cy0 + 1) > kMaxCellsPerFeature) {
        large_.push_back(key);
        return;
    }
    for (std::int64_t x = cx0; x <= cx1; ++x) {
        for (std::int64_t y = cy0; y <= cy1; ++y) {
            cells_[CellKey{x, y}].push_back(key);
        }
    }
}

void BoxGrid::Query(const MapRect& rect, std::vector<std::uint32_t>& out) const {
    if (!std::isfinite(rect.min.x) || !std::isfinite(rect.min.y) || !std::isfinite(rect.max.x) || !std::isfinite(rect.max.y) || rect.min.x > rect.max.x ||
        rect.min.y > rect.max.y) {
        return;
    }
    const std::size_t first = out.size();
    const auto cx0 = static_cast<std::int64_t>(std::floor(rect.min.x / cell_));
    const auto cx1 = static_cast<std::int64_t>(std::floor(rect.max.x / cell_));
    const auto cy0 = static_cast<std::int64_t>(std::floor(rect.min.y / cell_));
    const auto cy1 = static_cast<std::int64_t>(std::floor(rect.max.y / cell_));
    const bool scanAll = (cx1 - cx0 + 1) * (cy1 - cy0 + 1) > static_cast<std::int64_t>(cells_.size());
    if (scanAll) {
        for (const auto& [cellKey, keys] : cells_) {
            if (cellKey.x >= cx0 && cellKey.x <= cx1 && cellKey.y >= cy0 && cellKey.y <= cy1) {
                out.insert(out.end(), keys.begin(), keys.end());
            }
        }
    } else {
        for (std::int64_t x = cx0; x <= cx1; ++x) {
            for (std::int64_t y = cy0; y <= cy1; ++y) {
                const auto it = cells_.find(CellKey{x, y});
                if (it != cells_.end()) {
                    out.insert(out.end(), it->second.begin(), it->second.end());
                }
            }
        }
    }
    out.insert(out.end(), large_.begin(), large_.end());
    std::sort(out.begin() + static_cast<std::ptrdiff_t>(first), out.end());
    out.erase(std::unique(out.begin() + static_cast<std::ptrdiff_t>(first), out.end()), out.end());
    // exact bounding-box filter (cells are coarser than the boxes)
    out.erase(std::remove_if(out.begin() + static_cast<std::ptrdiff_t>(first), out.end(),
                             [&](std::uint32_t key) { return !RectsIntersect(bounds_.at(key), rect); }),
              out.end());
}

// --- RoadNetwork ---------------------------------------------------------------------------------------------------------------------

struct RoadNetwork::Index {
    std::unordered_map<std::uint32_t, BoxGrid> perLayer; // key = layer id, value keys = segment indices
    std::unordered_map<std::uint64_t, std::uint32_t> nodeById;
    std::unordered_map<std::uint64_t, std::uint32_t> segmentById;
};

namespace {
GeometryError Err(GeometryErrorCode code, std::uint64_t id, std::string detail = {}) { return GeometryError{code, id, std::move(detail)}; }
bool Finite(Vec2 v) { return std::isfinite(v.x) && std::isfinite(v.y); }
} // namespace

Result<RoadNetwork, GeometryError> RoadNetwork::Create(std::vector<RoadNodeDef> nodes, std::vector<RoadSegmentDef> segments,
                                                        std::vector<TurnRestriction> restrictions, double gridCellMetres) {
    if (!std::isfinite(gridCellMetres) || gridCellMetres <= 0.0) {
        return Err(GeometryErrorCode::InvalidGridCell, 0);
    }
    RoadNetwork net;
    std::sort(nodes.begin(), nodes.end(), [](const RoadNodeDef& a, const RoadNodeDef& b) { return a.id < b.id; });
    std::sort(segments.begin(), segments.end(), [](const RoadSegmentDef& a, const RoadSegmentDef& b) { return a.id < b.id; });
    auto index = std::make_shared<Index>();

    for (std::size_t i = 0; i < nodes.size(); ++i) {
        const RoadNodeDef& n = nodes[i];
        if (n.id.IsNull()) {
            return Err(GeometryErrorCode::InvalidId, 0, "node id 0");
        }
        if (i > 0 && nodes[i - 1].id == n.id) {
            return Err(GeometryErrorCode::DuplicateId, n.id.Value(), "node");
        }
        if (n.position.layer.IsNull()) {
            return Err(GeometryErrorCode::UnknownLayer, n.id.Value(), "node");
        }
        if (!IsValidMapCoordinate(n.position.xy)) {
            return Err(GeometryErrorCode::InvalidCoordinate, n.id.Value(), "node");
        }
        index->nodeById.emplace(n.id.Value(), static_cast<std::uint32_t>(i));
    }

    net.info_.reserve(segments.size());
    net.incident_.assign(nodes.size(), {});
    for (std::size_t i = 0; i < segments.size(); ++i) {
        const RoadSegmentDef& s = segments[i];
        const auto segIndex = static_cast<std::uint32_t>(i);
        if (s.id.IsNull()) {
            return Err(GeometryErrorCode::InvalidId, 0, "segment id 0");
        }
        if (i > 0 && segments[i - 1].id == s.id) {
            return Err(GeometryErrorCode::DuplicateId, s.id.Value(), "segment");
        }
        const auto from = index->nodeById.find(s.from.Value());
        const auto to = index->nodeById.find(s.to.Value());
        if (from == index->nodeById.end() || to == index->nodeById.end()) {
            return Err(GeometryErrorCode::UnknownNode, s.id.Value());
        }
        if (nodes[from->second].position.layer != nodes[to->second].position.layer) {
            return Err(GeometryErrorCode::LayerMismatch, s.id.Value());
        }
        if (s.shape.size() > kMaxFeaturePoints) {
            return Err(GeometryErrorCode::TooManyPoints, s.id.Value());
        }
        if (s.access.has_value() && (*s.access == 0 || (*s.access & ~(kAccessFoot | kAccessVehicle)) != 0)) {
            return Err(GeometryErrorCode::InvalidAccess, s.id.Value());
        }
        SegmentInfo info;
        info.fromNode = from->second;
        info.toNode = to->second;
        info.points.push_back(nodes[from->second].position.xy);
        for (const Vec2 p : s.shape) {
            if (!IsValidMapCoordinate(p)) {
                return Err(GeometryErrorCode::InvalidCoordinate, s.id.Value(), "shape");
            }
            info.points.push_back(p);
        }
        info.points.push_back(nodes[to->second].position.xy);
        info.cumulative = CumulativeLengths(info.points);
        info.length = info.cumulative.back();
        if (!(info.length > 0.0)) {
            return Err(GeometryErrorCode::DegenerateGeometry, s.id.Value(), "zero length");
        }
        info.roadClass = s.roadClass;
        info.structure = s.structure;
        info.direction = s.direction;
        info.access = s.access.value_or(DefaultAccess(s.roadClass));
        info.roundabout = s.roundabout;
        info.layer = nodes[from->second].position.layer;
        info.bounds = BoundsOf(info.points);
        net.incident_[info.fromNode].push_back(Incident{segIndex, true});
        net.incident_[info.toNode].push_back(Incident{segIndex, false});
        index->segmentById.emplace(s.id.Value(), segIndex);
        auto& grid = index->perLayer.try_emplace(info.layer.Value(), gridCellMetres).first->second;
        grid.Insert(segIndex, info.bounds);
        net.info_.push_back(std::move(info));
    }

    for (const TurnRestriction& r : restrictions) {
        const auto via = index->nodeById.find(r.via.Value());
        const auto from = index->segmentById.find(r.from.Value());
        const auto to = index->segmentById.find(r.to.Value());
        if (via == index->nodeById.end()) {
            return Err(GeometryErrorCode::UnknownNode, r.via.Value(), "restriction");
        }
        if (from == index->segmentById.end() || to == index->segmentById.end()) {
            return Err(GeometryErrorCode::UnknownSegment, r.via.Value(), "restriction");
        }
        const auto touches = [&](std::uint32_t seg) { return net.info_[seg].fromNode == via->second || net.info_[seg].toNode == via->second; };
        if (!touches(from->second) || !touches(to->second)) {
            return Err(GeometryErrorCode::RestrictionNotAdjacent, r.via.Value());
        }
        net.restrictionKeys_.push_back(RestrictionKey{via->second, from->second, to->second, r.vehiclesOnly});
    }
    std::sort(net.restrictionKeys_.begin(), net.restrictionKeys_.end(), [](const RestrictionKey& a, const RestrictionKey& b) {
        return std::tie(a.via, a.from, a.to, a.vehiclesOnly) < std::tie(b.via, b.from, b.to, b.vehiclesOnly);
    });

    std::sort(restrictions.begin(), restrictions.end(), [](const TurnRestriction& a, const TurnRestriction& b) {
        return std::tie(a.via, a.from, a.to, a.vehiclesOnly) < std::tie(b.via, b.from, b.to, b.vehiclesOnly);
    });
    net.nodes_ = std::move(nodes);
    net.segments_ = std::move(segments);
    net.restrictions_ = std::move(restrictions);
    net.index_ = std::move(index);
    return net;
}

std::optional<std::uint32_t> RoadNetwork::NodeIndex(RoadNodeId id) const {
    const auto it = index_->nodeById.find(id.Value());
    return it == index_->nodeById.end() ? std::nullopt : std::optional<std::uint32_t>(it->second);
}

std::optional<std::uint32_t> RoadNetwork::SegmentIndex(RoadSegmentId id) const {
    const auto it = index_->segmentById.find(id.Value());
    return it == index_->segmentById.end() ? std::nullopt : std::optional<std::uint32_t>(it->second);
}

bool RoadNetwork::Allows(std::uint32_t segmentIndex, TravelMode mode, bool forward) const noexcept {
    const SegmentInfo& s = info_[segmentIndex];
    if (mode == TravelMode::OnFoot) {
        return (s.access & kAccessFoot) != 0;
    }
    if ((s.access & kAccessVehicle) == 0) {
        return false;
    }
    switch (s.direction) {
    case RoadDirection::Both: return true;
    case RoadDirection::Forward: return forward;
    case RoadDirection::Backward: return !forward;
    }
    return false;
}

bool RoadNetwork::TurnForbidden(std::uint32_t via, std::uint32_t fromSegment, std::uint32_t toSegment, TravelMode mode) const {
    const auto lo = std::lower_bound(restrictionKeys_.begin(), restrictionKeys_.end(), RestrictionKey{via, fromSegment, toSegment, false},
                                     [](const RestrictionKey& a, const RestrictionKey& b) { return std::tie(a.via, a.from, a.to) < std::tie(b.via, b.from, b.to); });
    for (auto it = lo; it != restrictionKeys_.end() && it->via == via && it->from == fromSegment && it->to == toSegment; ++it) {
        if (!it->vehiclesOnly || mode == TravelMode::Driving) {
            return true;
        }
    }
    return false;
}

std::vector<RoadSnap> RoadNetwork::Snap(const MapPosition& position, double radius, const SnapRules& rules, std::size_t maxCount) const {
    std::vector<RoadSnap> out;
    if (!Finite(position.xy) || !std::isfinite(radius) || radius < 0.0 || maxCount == 0) {
        return out;
    }
    const auto grid = index_->perLayer.find(position.layer.Value());
    if (grid == index_->perLayer.end()) {
        return out;
    }
    std::vector<std::uint32_t> candidates;
    grid->second.Query(MapRect{{position.xy.x - radius, position.xy.y - radius}, {position.xy.x + radius, position.xy.y + radius}}, candidates);
    for (const std::uint32_t seg : candidates) {
        const SegmentInfo& s = info_[seg];
        if (IsEnclosed(s.structure) && !rules.allowEnclosed) {
            continue;
        }
        if (!Allows(seg, rules.mode, true) && !Allows(seg, rules.mode, false)) {
            continue;
        }
        if (rules.closed != nullptr && rules.closed->contains(segments_[seg].id)) {
            continue;
        }
        const PolylineProjection p = ProjectPointToPolyline(position.xy, s.points, s.cumulative);
        if (p.distance <= radius) {
            out.push_back(RoadSnap{seg, segments_[seg].id, p.point, p.offsetMetres, p.distance});
        }
    }
    std::sort(out.begin(), out.end(), [&](const RoadSnap& a, const RoadSnap& b) {
        if (a.distanceMetres != b.distanceMetres) {
            return a.distanceMetres < b.distanceMetres;
        }
        // exact tie (e.g. a road below a bridge, no elevation in the model): prefer the surface road, then the lower id
        const bool aSurface = info_[a.segmentIndex].structure == RoadStructure::Surface;
        const bool bSurface = info_[b.segmentIndex].structure == RoadStructure::Surface;
        if (aSurface != bSurface) {
            return aSurface;
        }
        return a.segment < b.segment;
    });
    if (out.size() > maxCount) {
        out.resize(maxCount);
    }
    return out;
}

bool RoadNetwork::HasLayer(MapLayerId layer) const { return index_->perLayer.contains(layer.Value()); }

std::vector<std::uint32_t> RoadNetwork::SegmentsInRect(MapLayerId layer, const MapRect& rect) const {
    std::vector<std::uint32_t> out;
    const auto grid = index_->perLayer.find(layer.Value());
    if (grid != index_->perLayer.end()) {
        grid->second.Query(rect, out);
    }
    return out;
}

} // namespace spn::core
