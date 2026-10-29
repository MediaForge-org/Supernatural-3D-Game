#include "TextParsing.h"

#include <algorithm>
#include <cmath>
#include <spn/core/WorldMap.h>

namespace spn::core {

namespace {

constexpr std::string_view kMagic = "SPN-MAPSTATE";

MapError Err(MapErrorCode code, std::size_t line = 0, std::string detail = {}) { return MapError{code, line, std::move(detail)}; }

bool IsValidLabel(std::string_view label) {
    if (label.size() > kMaxMapLabelLength) {
        return false;
    }
    return std::none_of(label.begin(), label.end(), [](char c) { return static_cast<unsigned char>(c) < 0x20 || c == 0x7F; });
}

bool IsValidRect(const MapRect& r) {
    return IsValidMapCoordinate(r.min) && IsValidMapCoordinate(r.max) && r.min.x <= r.max.x && r.min.y <= r.max.y;
}

bool RectInside(const MapRect& inner, const MapRect& outer) {
    return inner.min.x >= outer.min.x && inner.min.y >= outer.min.y && inner.max.x <= outer.max.x && inner.max.y <= outer.max.y;
}

double Distance(Vec2 a, Vec2 b) { return std::hypot(a.x - b.x, a.y - b.y); }

std::string_view OriginText(MarkerOrigin o) { return o == MarkerOrigin::Player ? "player" : "system"; }

} // namespace

const ContentId& PlayerWaypointCategory() {
    static const ContentId id = *ContentId::Parse("base:marker.player_waypoint");
    return id;
}

std::optional<double> PlanarDistance(const MapPosition& a, const MapPosition& b) noexcept {
    if (a.layer != b.layer) {
        return std::nullopt;
    }
    return Distance(a.xy, b.xy);
}

std::string_view ToString(MapErrorCode code) noexcept {
    switch (code) {
    case MapErrorCode::InvalidId: return "InvalidId";
    case MapErrorCode::DuplicateLayer: return "DuplicateLayer";
    case MapErrorCode::DuplicateId: return "DuplicateId";
    case MapErrorCode::InvalidBounds: return "InvalidBounds";
    case MapErrorCode::UnknownLayer: return "UnknownLayer";
    case MapErrorCode::OutsideLayerBounds: return "OutsideLayerBounds";
    case MapErrorCode::InvalidPosition: return "InvalidPosition";
    case MapErrorCode::InvalidLabel: return "InvalidLabel";
    case MapErrorCode::InvalidCell: return "InvalidCell";
    case MapErrorCode::UnknownMarker: return "UnknownMarker";
    case MapErrorCode::UnknownEntry: return "UnknownEntry";
    case MapErrorCode::InvalidMarker: return "InvalidMarker";
    case MapErrorCode::TooManyMarkers: return "TooManyMarkers";
    case MapErrorCode::TooManyPlayerMarkers: return "TooManyPlayerMarkers";
    case MapErrorCode::InvalidQuery: return "InvalidQuery";
    case MapErrorCode::InvalidHeader: return "InvalidHeader";
    case MapErrorCode::UnsupportedVersion: return "UnsupportedVersion";
    case MapErrorCode::TruncatedData: return "TruncatedData";
    case MapErrorCode::TrailingData: return "TrailingData";
    case MapErrorCode::MalformedLine: return "MalformedLine";
    case MapErrorCode::InvalidToken: return "InvalidToken";
    case MapErrorCode::CountMismatch: return "CountMismatch";
    case MapErrorCode::DuplicateMarker: return "DuplicateMarker";
    case MapErrorCode::Io: return "Io";
    }
    return "Unknown";
}

std::string_view ToString(MapLayerKind kind) noexcept {
    switch (kind) {
    case MapLayerKind::Surface: return "Surface";
    case MapLayerKind::Underground: return "Underground";
    case MapLayerKind::Interior: return "Interior";
    case MapLayerKind::Realm: return "Realm";
    }
    return "Unknown";
}

std::string_view ToString(DiscoveryState state) noexcept {
    switch (state) {
    case DiscoveryState::Undiscovered: return "Undiscovered";
    case DiscoveryState::Known: return "Known";
    case DiscoveryState::Visited: return "Visited";
    }
    return "Unknown";
}

// --- Definition ---------------------------------------------------------------------------------------------------------------

Result<WorldMapDefinition, MapError> WorldMapDefinition::Create(std::vector<MapLayerDef> layers, std::vector<MapRegionDef> regions,
                                                                std::vector<PointOfInterestDef> pois, double gridCellMetres) {
    if (!std::isfinite(gridCellMetres) || gridCellMetres <= 0.0) {
        return Err(MapErrorCode::InvalidCell, 0, "grid cell size must be finite and > 0");
    }
    std::sort(layers.begin(), layers.end(), [](const MapLayerDef& a, const MapLayerDef& b) { return a.id < b.id; });
    for (std::size_t i = 0; i < layers.size(); ++i) {
        const MapLayerDef& layer = layers[i];
        if (layer.id.IsNull() || layer.key.IsEmpty()) {
            return Err(MapErrorCode::InvalidId, 0, "layer needs a non-null id and a key");
        }
        if (i > 0 && layers[i - 1].id == layer.id) {
            return Err(MapErrorCode::DuplicateLayer, 0, "layer id " + std::to_string(layer.id.Value()));
        }
        for (std::size_t j = 0; j < i; ++j) {
            if (layers[j].key == layer.key) {
                return Err(MapErrorCode::DuplicateLayer, 0, "layer key " + layer.key.Value());
            }
        }
        if (!IsValidRect(layer.bounds)) {
            return Err(MapErrorCode::InvalidBounds, 0, "layer " + layer.key.Value());
        }
    }

    WorldMapDefinition def;
    def.gridCell_ = gridCellMetres;
    def.layers_ = std::move(layers);

    std::set<ContentId> ids;
    std::sort(regions.begin(), regions.end(), [](const MapRegionDef& a, const MapRegionDef& b) { return a.id < b.id; });
    for (const MapRegionDef& region : regions) {
        if (region.id.IsEmpty()) {
            return Err(MapErrorCode::InvalidId, 0, "region without id");
        }
        if (!ids.insert(region.id).second) {
            return Err(MapErrorCode::DuplicateId, 0, region.id.Value());
        }
        const MapLayerDef* layer = def.FindLayer(region.layer);
        if (layer == nullptr) {
            return Err(MapErrorCode::UnknownLayer, 0, region.id.Value());
        }
        if (!IsValidRect(region.bounds)) {
            return Err(MapErrorCode::InvalidBounds, 0, region.id.Value());
        }
        if (!RectInside(region.bounds, layer->bounds)) {
            return Err(MapErrorCode::OutsideLayerBounds, 0, region.id.Value());
        }
        if (!IsValidLabel(region.label)) {
            return Err(MapErrorCode::InvalidLabel, 0, region.id.Value());
        }
    }
    def.regions_ = std::move(regions);

    std::sort(pois.begin(), pois.end(), [](const PointOfInterestDef& a, const PointOfInterestDef& b) { return a.id < b.id; });
    for (const PointOfInterestDef& poi : pois) {
        if (poi.id.IsEmpty()) {
            return Err(MapErrorCode::InvalidId, 0, "POI without id");
        }
        if (!ids.insert(poi.id).second) {
            return Err(MapErrorCode::DuplicateId, 0, poi.id.Value());
        }
        const MapLayerDef* layer = def.FindLayer(poi.position.layer);
        if (layer == nullptr) {
            return Err(MapErrorCode::UnknownLayer, 0, poi.id.Value());
        }
        if (!IsValidMapCoordinate(poi.position.xy)) {
            return Err(MapErrorCode::InvalidPosition, 0, poi.id.Value());
        }
        if (!layer->bounds.Contains(poi.position.xy)) {
            return Err(MapErrorCode::OutsideLayerBounds, 0, poi.id.Value());
        }
        if (!IsValidLabel(poi.label)) {
            return Err(MapErrorCode::InvalidLabel, 0, poi.id.Value());
        }
    }
    def.pois_ = std::move(pois);
    for (std::size_t i = 0; i < def.pois_.size(); ++i) {
        const MapPosition& p = def.pois_[i].position;
        def.poiGrids_.try_emplace(p.layer.Value(), SpatialGrid(gridCellMetres)).first->second.Insert(i, p.xy);
    }
    return def;
}

const MapLayerDef* WorldMapDefinition::FindLayer(MapLayerId id) const {
    const auto it = std::lower_bound(layers_.begin(), layers_.end(), id, [](const MapLayerDef& l, MapLayerId v) { return l.id < v; });
    return it != layers_.end() && it->id == id ? &*it : nullptr;
}

const MapLayerDef* WorldMapDefinition::FindLayer(const ContentId& key) const {
    const auto it = std::find_if(layers_.begin(), layers_.end(), [&](const MapLayerDef& l) { return l.key == key; });
    return it != layers_.end() ? &*it : nullptr;
}

const MapRegionDef* WorldMapDefinition::FindRegion(const ContentId& id) const {
    const auto it = std::lower_bound(regions_.begin(), regions_.end(), id, [](const MapRegionDef& r, const ContentId& v) { return r.id < v; });
    return it != regions_.end() && it->id == id ? &*it : nullptr;
}

const PointOfInterestDef* WorldMapDefinition::FindPoi(const ContentId& id) const {
    const auto it = std::lower_bound(pois_.begin(), pois_.end(), id, [](const PointOfInterestDef& p, const ContentId& v) { return p.id < v; });
    return it != pois_.end() && it->id == id ? &*it : nullptr;
}

std::vector<const MapRegionDef*> WorldMapDefinition::RegionsAt(const MapPosition& position) const {
    std::vector<const MapRegionDef*> found;
    for (const MapRegionDef& region : regions_) {
        if (region.layer == position.layer && region.bounds.Contains(position.xy)) {
            found.push_back(&region);
        }
    }
    std::sort(found.begin(), found.end(), [](const MapRegionDef* a, const MapRegionDef* b) {
        const double aa = a->bounds.Area();
        const double ab = b->bounds.Area();
        return aa != ab ? aa < ab : a->id < b->id;
    });
    return found;
}

std::vector<std::size_t> WorldMapDefinition::PoiIndicesInRadius(MapLayerId layer, Vec2 center, double radius) const {
    std::vector<std::uint64_t> keys;
    if (const auto it = poiGrids_.find(layer.Value()); it != poiGrids_.end()) {
        it->second.QueryRadius(center, radius, keys);
    }
    return std::vector<std::size_t>(keys.begin(), keys.end());
}

std::vector<std::size_t> WorldMapDefinition::PoiIndicesInRect(MapLayerId layer, const MapRect& rect) const {
    std::vector<std::uint64_t> keys;
    if (const auto it = poiGrids_.find(layer.Value()); it != poiGrids_.end()) {
        it->second.QueryRect(rect.min, rect.max, keys);
    }
    return std::vector<std::size_t>(keys.begin(), keys.end());
}

// --- State: markers -----------------------------------------------------------------------------------------------------------

WorldMapState::WorldMapState(std::shared_ptr<const WorldMapDefinition> definition, MapStateConfig config)
    : definition_(std::move(definition)), config_(config) {
    if (!std::isfinite(config_.gridCellMetres) || config_.gridCellMetres <= 0.0) {
        config_.gridCellMetres = 256.0;
    }
}

std::optional<MapError> WorldMapState::ValidateSpec(const MarkerSpec& spec) const {
    const MapLayerDef* layer = definition_->FindLayer(spec.position.layer);
    if (layer == nullptr) {
        return Err(MapErrorCode::UnknownLayer);
    }
    if (!IsValidMapCoordinate(spec.position.xy)) {
        return Err(MapErrorCode::InvalidPosition);
    }
    if (!layer->bounds.Contains(spec.position.xy)) {
        return Err(MapErrorCode::OutsideLayerBounds);
    }
    if (spec.category.IsEmpty()) {
        return Err(MapErrorCode::InvalidMarker, 0, "category required");
    }
    if (!IsValidLabel(spec.label)) {
        return Err(MapErrorCode::InvalidLabel);
    }
    if (!spec.linkedPoi.IsEmpty() && definition_->FindPoi(spec.linkedPoi) == nullptr) {
        return Err(MapErrorCode::UnknownEntry, 0, spec.linkedPoi.Value());
    }
    if (spec.expiresAt && (spec.persistence != MarkerPersistence::Temporary || !std::isfinite(*spec.expiresAt))) {
        return Err(MapErrorCode::InvalidMarker, 0, "expiresAt needs a Temporary marker and a finite time");
    }
    return std::nullopt;
}

void WorldMapState::Index(const Marker& marker) {
    markerGrids_.try_emplace(marker.position.layer.Value(), LayerIndex{SpatialGrid(config_.gridCellMetres)})
        .first->second.grid.Insert(marker.id.Value(), marker.position.xy);
}

Result<MarkerId, MapError> WorldMapState::AddMarker(MarkerSpec spec) {
    if (auto error = ValidateSpec(spec)) {
        return *error;
    }
    if (markers_.size() >= kMaxMapMarkers) {
        return Err(MapErrorCode::TooManyMarkers);
    }
    if (spec.origin == MarkerOrigin::Player && config_.maxPlayerMarkers && playerMarkers_ >= *config_.maxPlayerMarkers) {
        return Err(MapErrorCode::TooManyPlayerMarkers);
    }
    Marker marker;
    static_cast<MarkerSpec&>(marker) = std::move(spec);
    marker.id = MarkerId(nextMarker_++);
    Index(marker);
    playerMarkers_ += marker.origin == MarkerOrigin::Player ? 1U : 0U;
    markers_.emplace(marker.id.Value(), std::move(marker));
    ++revision_;
    return MarkerId(nextMarker_ - 1);
}

Result<MarkerId, MapError> WorldMapState::AddPlayerWaypoint(MapPosition position, std::string label) {
    MarkerSpec spec;
    spec.position = position;
    spec.category = PlayerWaypointCategory();
    spec.label = std::move(label);
    spec.origin = MarkerOrigin::Player;
    return AddMarker(std::move(spec));
}

std::optional<MapError> WorldMapState::RemoveMarker(MarkerId id) {
    const auto it = markers_.find(id.Value());
    if (it == markers_.end()) {
        return Err(MapErrorCode::UnknownMarker);
    }
    markerGrids_.at(it->second.position.layer.Value()).grid.Remove(id.Value());
    playerMarkers_ -= it->second.origin == MarkerOrigin::Player ? 1U : 0U;
    markers_.erase(it);
    ++revision_;
    return std::nullopt;
}

std::optional<MapError> WorldMapState::MoveMarker(MarkerId id, MapPosition position) {
    const auto it = markers_.find(id.Value());
    if (it == markers_.end()) {
        return Err(MapErrorCode::UnknownMarker);
    }
    MarkerSpec moved = it->second;
    moved.position = position;
    if (auto error = ValidateSpec(moved)) {
        return error; // nothing changed
    }
    Marker& marker = it->second;
    markerGrids_.at(marker.position.layer.Value()).grid.Remove(id.Value());
    marker.position = position;
    Index(marker);
    ++revision_;
    return std::nullopt;
}

std::optional<MapError> WorldMapState::SetMarkerVisible(MarkerId id, bool visible) {
    const auto it = markers_.find(id.Value());
    if (it == markers_.end()) {
        return Err(MapErrorCode::UnknownMarker);
    }
    if (it->second.visible != visible) ++revision_;
    it->second.visible = visible;
    return std::nullopt;
}

const Marker* WorldMapState::FindMarker(MarkerId id) const {
    const auto it = markers_.find(id.Value());
    return it == markers_.end() ? nullptr : &it->second;
}

std::size_t WorldMapState::PruneExpired(double now) {
    if (!std::isfinite(now)) {
        return 0;
    }
    std::vector<MarkerId> expired;
    for (const auto& [key, marker] : markers_) {
        if (marker.persistence == MarkerPersistence::Temporary && marker.expiresAt && *marker.expiresAt <= now) {
            expired.push_back(marker.id);
        }
    }
    for (const MarkerId id : expired) {
        (void)RemoveMarker(id);
    }
    return expired.size();
}

// --- State: discovery ---------------------------------------------------------------------------------------------------------

DiscoveryState WorldMapState::Discovery(const ContentId& id) const {
    const auto it = discovery_.find(id);
    return it == discovery_.end() ? DiscoveryState::Undiscovered : it->second;
}

Result<bool, MapError> WorldMapState::Discover(const ContentId& id, DiscoveryState state) {
    if (!definition_->Knows(id)) {
        return Err(MapErrorCode::UnknownEntry, 0, id.Value());
    }
    if (state <= Discovery(id)) {
        return false;
    }
    discovery_[id] = state;
    ++revision_;
    return true;
}

std::size_t WorldMapState::DiscoverPoisAround(const MapPosition& position, double radius, DiscoveryState state) {
    std::size_t changed = 0;
    for (const std::size_t index : definition_->PoiIndicesInRadius(position.layer, position.xy, radius)) {
        const Result<bool, MapError> r = Discover(definition_->Pois()[index].id, state);
        changed += r.HasValue() && r.Value() ? 1U : 0U;
    }
    return changed;
}

std::size_t WorldMapState::DiscoverRegionsAt(const MapPosition& position, DiscoveryState state) {
    std::size_t changed = 0;
    for (const MapRegionDef* region : definition_->RegionsAt(position)) {
        const Result<bool, MapError> r = Discover(region->id, state);
        changed += r.HasValue() && r.Value() ? 1U : 0U;
    }
    return changed;
}

bool WorldMapState::IsVisible(const PointOfInterestDef& poi) const {
    switch (poi.visibility) {
    case MapVisibility::Hidden: return false;
    case MapVisibility::Always: return true;
    case MapVisibility::WhenKnown: return Discovery(poi.id) >= DiscoveryState::Known;
    }
    return false;
}

// --- State: queries -----------------------------------------------------------------------------------------------------------

bool WorldMapState::Passes(const Marker& marker, const MapFilter& filter) const {
    if (!filter.includeHidden && !marker.visible) {
        return false;
    }
    if (marker.persistence == MarkerPersistence::Persistent ? !filter.includePersistentMarkers : !filter.includeTemporaryMarkers) {
        return false;
    }
    if (marker.origin == MarkerOrigin::Player ? !filter.includePlayerMarkers : !filter.includeSystemMarkers) {
        return false;
    }
    return filter.categories.empty() || filter.categories.contains(marker.category);
}

bool WorldMapState::Passes(const PointOfInterestDef& poi, const MapFilter& filter) const {
    if (!filter.includePois || (!filter.includeHidden && !IsVisible(poi))) {
        return false;
    }
    return filter.categories.empty() || filter.categories.contains(poi.category);
}

std::vector<MapQueryHit> WorldMapState::Collect(MapLayerId layer, const std::vector<std::size_t>& poiIndices, const std::vector<std::uint64_t>& markerKeys,
                                                Vec2 center, const MapFilter& filter) const {
    (void)layer;
    std::vector<MapQueryHit> hits;
    for (const std::size_t index : poiIndices) {
        const PointOfInterestDef& poi = definition_->Pois()[index];
        if (Passes(poi, filter)) {
            hits.push_back(MapQueryHit{MapHitSource::PointOfInterest, &poi, nullptr, poi.position.xy, Distance(poi.position.xy, center)});
        }
    }
    for (const std::uint64_t key : markerKeys) {
        const Marker& marker = markers_.at(key);
        if (Passes(marker, filter)) {
            hits.push_back(MapQueryHit{MapHitSource::Marker, nullptr, &marker, marker.position.xy, Distance(marker.position.xy, center)});
        }
    }
    std::sort(hits.begin(), hits.end(), [](const MapQueryHit& a, const MapQueryHit& b) {
        if (a.distanceMetres != b.distanceMetres) {
            return a.distanceMetres < b.distanceMetres;
        }
        if (a.source != b.source) {
            return a.source < b.source;
        }
        return a.source == MapHitSource::PointOfInterest ? a.poi->id < b.poi->id : a.marker->id < b.marker->id;
    });
    return hits;
}

Result<std::vector<MapQueryHit>, MapError> WorldMapState::QueryRadius(const MapPosition& center, double radius, const MapFilter& filter) const {
    if (definition_->FindLayer(center.layer) == nullptr || !std::isfinite(center.xy.x) || !std::isfinite(center.xy.y) || !std::isfinite(radius) ||
        radius < 0.0) {
        return Err(MapErrorCode::InvalidQuery);
    }
    std::vector<std::uint64_t> markerKeys;
    if (const auto it = markerGrids_.find(center.layer.Value()); it != markerGrids_.end()) {
        it->second.grid.QueryRadius(center.xy, radius, markerKeys);
    }
    return Collect(center.layer, definition_->PoiIndicesInRadius(center.layer, center.xy, radius), markerKeys, center.xy, filter);
}

Result<std::vector<MapQueryHit>, MapError> WorldMapState::QueryRect(MapLayerId layer, const MapRect& rect, const MapFilter& filter) const {
    if (definition_->FindLayer(layer) == nullptr || !std::isfinite(rect.min.x) || !std::isfinite(rect.min.y) || !std::isfinite(rect.max.x) ||
        !std::isfinite(rect.max.y) || rect.min.x > rect.max.x || rect.min.y > rect.max.y) {
        return Err(MapErrorCode::InvalidQuery);
    }
    std::vector<std::uint64_t> markerKeys;
    if (const auto it = markerGrids_.find(layer.Value()); it != markerGrids_.end()) {
        it->second.grid.QueryRect(rect.min, rect.max, markerKeys);
    }
    const Vec2 center{rect.min.x + (rect.max.x - rect.min.x) / 2.0, rect.min.y + (rect.max.y - rect.min.y) / 2.0};
    return Collect(layer, definition_->PoiIndicesInRect(layer, rect), markerKeys, center, filter);
}

// --- State: persistence -------------------------------------------------------------------------------------------------------

std::string WorldMapState::Serialize() const {
    std::string out;
    out.append(kMagic).append(" 1\n");
    out.append("nextmarker ").append(std::to_string(nextMarker_)).append("\n");
    out.append("discovery ").append(std::to_string(discovery_.size())).append("\n");
    for (const auto& [id, state] : discovery_) {
        out.append("d ").append(id.Value()).append(state == DiscoveryState::Visited ? " visited\n" : " known\n");
    }
    std::size_t persistent = 0;
    for (const auto& [key, marker] : markers_) {
        persistent += marker.persistence == MarkerPersistence::Persistent ? 1U : 0U;
    }
    out.append("markers ").append(std::to_string(persistent)).append("\n");
    for (const auto& [key, m] : markers_) {
        if (m.persistence != MarkerPersistence::Persistent) {
            continue;
        }
        out.append("m ").append(std::to_string(m.id.Value())).append(" ").append(std::to_string(m.position.layer.Value())).append(" ");
        out.append(detail::FormatDouble(m.position.xy.x)).append(" ").append(detail::FormatDouble(m.position.xy.y)).append(" ");
        out.append(m.category.Value()).append(" ").append(m.linkedPoi.IsEmpty() ? "-" : m.linkedPoi.Value()).append(" ");
        out.append(OriginText(m.origin)).append(m.visible ? " 1 " : " 0 ").append(detail::PercentEncode(m.label)).append("\n");
    }
    out.append("end\n");
    return out;
}

namespace {

std::vector<std::string_view> SplitLines(std::string_view text) {
    std::vector<std::string_view> lines;
    std::size_t start = 0;
    while (start < text.size()) {
        const std::size_t end = text.find('\n', start);
        if (end == std::string_view::npos) {
            lines.push_back(text.substr(start)); // unterminated last line: caught as truncated/malformed by the caller
            break;
        }
        lines.push_back(text.substr(start, end - start));
        start = end + 1;
    }
    return lines;
}

std::vector<std::string_view> SplitSpaces(std::string_view line) {
    std::vector<std::string_view> parts;
    std::size_t start = 0;
    while (true) {
        const std::size_t end = line.find(' ', start);
        parts.push_back(line.substr(start, end == std::string_view::npos ? std::string_view::npos : end - start));
        if (end == std::string_view::npos) {
            return parts;
        }
        start = end + 1;
    }
}

} // namespace

Result<WorldMapState, MapError> WorldMapState::Deserialize(std::string_view text, std::shared_ptr<const WorldMapDefinition> definition, MapStateConfig config) {
    const std::vector<std::string_view> lines = SplitLines(text);
    if (text.empty() || text.back() != '\n') {
        return Err(lines.empty() ? MapErrorCode::InvalidHeader : MapErrorCode::TruncatedData, lines.size(), "missing final newline");
    }
    std::size_t at = 0;
    const auto line = [&]() { return at + 1; };
    const auto expectCount = [&](std::string_view keyword, std::uint64_t& count) -> std::optional<MapError> {
        if (at >= lines.size()) {
            return Err(MapErrorCode::TruncatedData, line());
        }
        const auto parts = SplitSpaces(lines[at]);
        if (parts.size() != 2 || parts[0] != keyword) {
            return Err(MapErrorCode::MalformedLine, line(), std::string(keyword));
        }
        const auto n = detail::ParseCanonicalInteger<std::uint64_t>(parts[1]);
        if (!n) {
            return Err(MapErrorCode::InvalidToken, line(), std::string(keyword));
        }
        count = *n;
        ++at;
        return std::nullopt;
    };

    // Header
    {
        const auto parts = SplitSpaces(lines.front());
        if (parts.size() != 2 || parts[0] != kMagic) {
            return Err(MapErrorCode::InvalidHeader, 1);
        }
        const auto version = detail::ParseCanonicalInteger<std::uint64_t>(parts[1]);
        if (!version) {
            return Err(MapErrorCode::InvalidHeader, 1);
        }
        if (*version != 1) {
            return Err(MapErrorCode::UnsupportedVersion, 1, parts[1] == "0" ? "0" : std::string(parts[1]));
        }
        at = 1;
    }

    WorldMapState state(std::move(definition), config);
    std::uint64_t next = 0;
    if (auto e = expectCount("nextmarker", next)) {
        return *e;
    }
    if (next == 0) {
        return Err(MapErrorCode::InvalidToken, at, "nextmarker must be >= 1");
    }

    std::uint64_t discoveryCount = 0;
    if (auto e = expectCount("discovery", discoveryCount)) {
        return *e;
    }
    std::optional<ContentId> previous;
    for (std::uint64_t i = 0; i < discoveryCount; ++i, ++at) {
        if (at >= lines.size() || lines[at] == "end") {
            return Err(MapErrorCode::CountMismatch, line(), "discovery");
        }
        const auto parts = SplitSpaces(lines[at]);
        if (parts.size() != 3 || parts[0] != "d") {
            return Err(MapErrorCode::MalformedLine, line());
        }
        const auto id = ContentId::Parse(parts[1]);
        if (!id || (parts[2] != "known" && parts[2] != "visited")) {
            return Err(MapErrorCode::InvalidToken, line());
        }
        if (previous && !(*previous < *id)) {
            return Err(MapErrorCode::MalformedLine, line(), "discovery ids must be strictly ascending");
        }
        previous = *id;
        if (!state.definition_->Knows(*id)) {
            return Err(MapErrorCode::UnknownEntry, line(), id->Value());
        }
        state.discovery_[*id] = parts[2] == "visited" ? DiscoveryState::Visited : DiscoveryState::Known;
    }

    std::uint64_t markerCount = 0;
    if (auto e = expectCount("markers", markerCount)) {
        return *e;
    }
    if (markerCount > kMaxMapMarkers) {
        return Err(MapErrorCode::TooManyMarkers, line());
    }
    std::uint64_t previousId = 0;
    for (std::uint64_t i = 0; i < markerCount; ++i, ++at) {
        if (at >= lines.size() || lines[at] == "end") {
            return Err(MapErrorCode::CountMismatch, line(), "markers");
        }
        const auto p = SplitSpaces(lines[at]);
        if (p.size() != 10 || p[0] != "m") {
            return Err(MapErrorCode::MalformedLine, line());
        }
        const auto id = detail::ParseCanonicalInteger<std::uint64_t>(p[1]);
        const auto layer = detail::ParseCanonicalInteger<std::uint32_t>(p[2]);
        const auto x = detail::ParseCanonicalDouble(p[3]);
        const auto y = detail::ParseCanonicalDouble(p[4]);
        const auto category = ContentId::Parse(p[5]);
        const auto linked = p[6] == "-" ? std::optional<ContentId>(ContentId()) : ContentId::Parse(p[6]);
        const auto label = detail::PercentDecode(p[9]);
        if (!id || !layer || !x || !y || !category || !linked || !label || (p[7] != "player" && p[7] != "system") || (p[8] != "0" && p[8] != "1")) {
            return Err(MapErrorCode::InvalidToken, line());
        }
        if (*id == 0 || *id >= next) {
            return Err(MapErrorCode::InvalidToken, line(), "marker id out of range");
        }
        if (*id <= previousId) {
            return Err(i > 0 && *id == previousId ? MapErrorCode::DuplicateMarker : MapErrorCode::MalformedLine, line(), "marker ids must be strictly ascending");
        }
        previousId = *id;
        Marker marker;
        marker.id = MarkerId(*id);
        marker.position = MapPosition{MapLayerId(*layer), Vec2{*x, *y}};
        marker.category = *category;
        marker.linkedPoi = *linked;
        marker.label = *label;
        marker.origin = p[7] == "player" ? MarkerOrigin::Player : MarkerOrigin::System;
        marker.visible = p[8] == "1";
        marker.persistence = MarkerPersistence::Persistent;
        if (auto error = state.ValidateSpec(marker)) {
            error->line = line();
            return *error;
        }
        if (marker.origin == MarkerOrigin::Player && state.config_.maxPlayerMarkers && state.playerMarkers_ >= *state.config_.maxPlayerMarkers) {
            return Err(MapErrorCode::TooManyPlayerMarkers, line());
        }
        state.Index(marker);
        state.playerMarkers_ += marker.origin == MarkerOrigin::Player ? 1U : 0U;
        state.markers_.emplace(*id, std::move(marker));
    }
    state.nextMarker_ = next;

    if (at >= lines.size()) {
        return Err(MapErrorCode::TruncatedData, line(), "missing end");
    }
    if (lines[at] != "end") {
        return Err(MapErrorCode::MalformedLine, line(), "expected end");
    }
    if (at + 1 != lines.size()) {
        return Err(MapErrorCode::TrailingData, at + 2);
    }
    return state;
}

bool WorldMapState::SameSavedState(const WorldMapState& other) const {
    if (nextMarker_ != other.nextMarker_ || discovery_ != other.discovery_) {
        return false;
    }
    const auto persistent = [](const std::map<std::uint64_t, Marker>& all) {
        std::vector<const Marker*> out;
        for (const auto& [key, marker] : all) {
            if (marker.persistence == MarkerPersistence::Persistent) {
                out.push_back(&marker);
            }
        }
        return out;
    };
    const auto a = persistent(markers_);
    const auto b = persistent(other.markers_);
    return a.size() == b.size() && std::equal(a.begin(), a.end(), b.begin(), [](const Marker* l, const Marker* r) {
               return l->id == r->id && l->position == r->position && l->category == r->category && l->label == r->label && l->origin == r->origin &&
                      l->visible == r->visible && l->linkedPoi == r->linkedPoi;
           });
}

} // namespace spn::core
