#include <algorithm>
#include <cmath>
#include <doctest.h>
#include <spn/core/SpatialGrid.h>
#include <spn/core/WorldMap.h>

#include "MapTestData.h"

using namespace spn::core;
using namespace spn::testdata;

// --- SpatialGrid ----------------------------------------------------------------------------------------------------------------

TEST_CASE("SpatialGrid: insert, find, duplicate and invalid positions") {
    SpatialGrid grid(100.0);
    CHECK(grid.Insert(1, {10, 10}));
    CHECK_FALSE(grid.Insert(1, {20, 20})); // duplicate key keeps the first position
    REQUIRE(grid.Find(1) != nullptr);
    CHECK(*grid.Find(1) == Vec2{10, 10});
    CHECK_FALSE(grid.Insert(2, {std::nan(""), 0}));
    CHECK_FALSE(grid.Insert(3, {0, kInf}));
    CHECK_FALSE(grid.Insert(4, {kMaxMapCoordinateMetres * 2, 0}));
    CHECK(grid.Size() == 1);
    CHECK(SpatialGrid(-5.0).CellSize() == 256.0); // invalid cell size falls back
    CHECK(SpatialGrid(std::nan("")).CellSize() == 256.0);
}

TEST_CASE("SpatialGrid: remove and move keep the index consistent") {
    SpatialGrid grid(100.0);
    grid.Insert(1, {10, 10});
    grid.Insert(2, {20, 20}); // same cell as 1
    CHECK(grid.Remove(1));
    CHECK_FALSE(grid.Remove(1));
    std::vector<std::uint64_t> out;
    grid.QueryRadius({0, 0}, 1000, out);
    CHECK(out == std::vector<std::uint64_t>{2});
    CHECK(grid.Move(2, {5000, 5000}));
    CHECK_FALSE(grid.Move(2, {std::nan(""), 0}));
    CHECK(*grid.Find(2) == Vec2{5000, 5000}); // rejected move changed nothing
    CHECK_FALSE(grid.Move(99, {0, 0}));
    out.clear();
    grid.QueryRadius({0, 0}, 1000, out);
    CHECK(out.empty());
    CHECK(grid.OccupiedCells() == 1); // the emptied cells are gone
}

TEST_CASE("SpatialGrid: radius and rect queries agree with brute force, including negative coordinates") {
    SpatialGrid grid(64.0);
    Lcg rng(42);
    std::vector<Vec2> points;
    for (std::uint64_t k = 0; k < 2000; ++k) {
        const Vec2 p{rng.Uniform(-3000, 3000), rng.Uniform(-3000, 3000)};
        points.push_back(p);
        REQUIRE(grid.Insert(k, p));
    }
    for (int q = 0; q < 50; ++q) {
        const Vec2 c{rng.Uniform(-3500, 3500), rng.Uniform(-3500, 3500)};
        const double r = rng.Uniform(0, 900);
        std::vector<std::uint64_t> expected;
        for (std::uint64_t k = 0; k < points.size(); ++k) {
            const double dx = points[k].x - c.x;
            const double dy = points[k].y - c.y;
            if (dx * dx + dy * dy <= r * r) {
                expected.push_back(k);
            }
        }
        std::vector<std::uint64_t> got;
        grid.QueryRadius(c, r, got);
        CHECK(got == expected);

        std::vector<std::uint64_t> expectedRect;
        for (std::uint64_t k = 0; k < points.size(); ++k) {
            if (points[k].x >= c.x - r && points[k].x <= c.x + r && points[k].y >= c.y - r && points[k].y <= c.y + r) {
                expectedRect.push_back(k);
            }
        }
        std::vector<std::uint64_t> gotRect;
        grid.QueryRect({c.x - r, c.y - r}, {c.x + r, c.y + r}, gotRect);
        CHECK(gotRect == expectedRect);
    }
}

TEST_CASE("SpatialGrid: query results do not depend on insertion order") {
    std::vector<std::pair<std::uint64_t, Vec2>> items;
    Lcg rng(7);
    for (std::uint64_t k = 1; k <= 300; ++k) {
        items.push_back({k, {rng.Uniform(-500, 500), rng.Uniform(-500, 500)}});
    }
    SpatialGrid a(50.0);
    SpatialGrid b(50.0);
    for (const auto& [k, p] : items) {
        a.Insert(k, p);
    }
    for (auto it = items.rbegin(); it != items.rend(); ++it) {
        b.Insert(it->first, it->second);
    }
    std::vector<std::uint64_t> ra;
    std::vector<std::uint64_t> rb;
    a.QueryRadius({0, 0}, 300, ra);
    b.QueryRadius({0, 0}, 300, rb);
    CHECK(ra == rb);
    CHECK(std::is_sorted(ra.begin(), ra.end()));
}

TEST_CASE("SpatialGrid: inclusive boundaries, degenerate and hostile queries") {
    SpatialGrid grid(100.0);
    grid.Insert(1, {100, 0});
    grid.Insert(2, {-100, 0});
    std::vector<std::uint64_t> out;
    grid.QueryRadius({0, 0}, 100, out);
    CHECK(out == std::vector<std::uint64_t>{1, 2}); // exactly on the radius counts
    out.clear();
    grid.QueryRadius({0, 0}, 99.999999, out);
    CHECK(out.empty());
    grid.QueryRadius({0, 0}, -1, out);
    grid.QueryRadius({0, 0}, std::nan(""), out);
    grid.QueryRadius({std::nan(""), 0}, 10, out);
    grid.QueryRect({10, 0}, {-10, 0}, out); // inverted
    CHECK(out.empty());
    grid.QueryRadius({0, 0}, 0, out); // zero radius: only an exact hit
    CHECK(out.empty());
    grid.QueryRadius({0, 0}, 1e300, out); // absurd radius clamps to the coordinate range and takes the occupied-cell path
    CHECK(out == std::vector<std::uint64_t>{1, 2});
}

TEST_CASE("SpatialGrid: points at the +-10,000 km limit") {
    SpatialGrid grid(256.0);
    CHECK(grid.Insert(1, {kMaxMapCoordinateMetres, -kMaxMapCoordinateMetres}));
    CHECK(grid.Insert(2, {kMaxMapCoordinateMetres - 3, -kMaxMapCoordinateMetres + 4}));
    std::vector<std::uint64_t> out;
    grid.QueryRadius({kMaxMapCoordinateMetres, -kMaxMapCoordinateMetres}, 5.0, out);
    CHECK(out == std::vector<std::uint64_t>{1, 2}); // distance exactly 5
    out.clear();
    grid.QueryRadius({kMaxMapCoordinateMetres, -kMaxMapCoordinateMetres}, 4.9, out);
    CHECK(out == std::vector<std::uint64_t>{1});
}

// --- Definition -----------------------------------------------------------------------------------------------------------------

TEST_CASE("WorldMapDefinition: valid definition, lookups and sorted storage") {
    const auto def = MakeTestDefinition();
    CHECK(def->Layers().size() == 5);
    CHECK(def->Regions().size() == 4);
    CHECK(def->Pois().size() == 6);
    CHECK(std::is_sorted(def->Pois().begin(), def->Pois().end(), [](const auto& a, const auto& b) { return a.id < b.id; }));
    CHECK(def->FindLayer(MapLayerId(kUnderground))->kind == MapLayerKind::Underground);
    CHECK(def->FindLayer(Cid("test:layer.house_floor1"))->level == 1);
    CHECK(def->FindLayer(MapLayerId(99)) == nullptr);
    CHECK(def->FindPoi(Cid("test:poi.shop")) != nullptr);
    CHECK(def->FindRegion(Cid("test:poi.shop")) == nullptr);
    CHECK(def->Knows(Cid("test:region.cave")));
    CHECK_FALSE(def->Knows(Cid("test:nothing.here")));
}

TEST_CASE("WorldMapDefinition: validation rejects inconsistent data") {
    const auto layer = [](std::uint32_t id, const char* key, double half) {
        MapLayerDef l;
        l.id = MapLayerId(id);
        l.key = Cid(key);
        l.bounds = MapRect{{-half, -half}, {half, half}};
        return l;
    };
    const auto create = [](std::vector<MapLayerDef> l, std::vector<MapRegionDef> r, std::vector<PointOfInterestDef> p) {
        return WorldMapDefinition::Create(std::move(l), std::move(r), std::move(p));
    };
    const auto code = [](const Result<WorldMapDefinition, MapError>& r) {
        REQUIRE_FALSE(r.HasValue());
        return r.Error().code;
    };
    CHECK(code(create({layer(0, "t:a", 10)}, {}, {})) == MapErrorCode::InvalidId);
    CHECK(code(create({layer(1, "t:a", 10), layer(1, "t:b", 10)}, {}, {})) == MapErrorCode::DuplicateLayer);
    CHECK(code(create({layer(1, "t:a", 10), layer(2, "t:a", 10)}, {}, {})) == MapErrorCode::DuplicateLayer); // same key
    CHECK(code(create({layer(1, "t:a", std::nan(""))}, {}, {})) == MapErrorCode::InvalidBounds);
    CHECK(code(create({layer(1, "t:a", kMaxMapCoordinateMetres * 2)}, {}, {})) == MapErrorCode::InvalidBounds);
    MapLayerDef inverted = layer(1, "t:a", 10);
    inverted.bounds = MapRect{{5, 5}, {-5, 5}};
    CHECK(code(create({inverted}, {}, {})) == MapErrorCode::InvalidBounds);

    MapRegionDef region;
    region.id = Cid("t:r");
    region.layer = MapLayerId(1);
    region.bounds = {{0, 0}, {5, 5}};
    MapRegionDef outside = region;
    outside.bounds = {{0, 0}, {50, 5}};
    MapRegionDef unknownLayer = region;
    unknownLayer.layer = MapLayerId(9);
    MapRegionDef noId = region;
    noId.id = ContentId();
    MapRegionDef badLabel = region;
    badLabel.label = "line\nbreak";
    CHECK(create({layer(1, "t:a", 10)}, {region}, {}).HasValue());
    CHECK(code(create({layer(1, "t:a", 10)}, {outside}, {})) == MapErrorCode::OutsideLayerBounds);
    CHECK(code(create({layer(1, "t:a", 10)}, {unknownLayer}, {})) == MapErrorCode::UnknownLayer);
    CHECK(code(create({layer(1, "t:a", 10)}, {noId}, {})) == MapErrorCode::InvalidId);
    CHECK(code(create({layer(1, "t:a", 10)}, {badLabel}, {})) == MapErrorCode::InvalidLabel);
    CHECK(code(create({layer(1, "t:a", 10)}, {region, region}, {})) == MapErrorCode::DuplicateId);

    PointOfInterestDef poi;
    poi.id = Cid("t:r"); // same id as the region: one namespace
    poi.position = At(1, 1, 1);
    CHECK(code(create({layer(1, "t:a", 10)}, {region}, {poi})) == MapErrorCode::DuplicateId);
    poi.id = Cid("t:p");
    CHECK(create({layer(1, "t:a", 10)}, {region}, {poi}).HasValue());
    poi.position = At(1, 11, 0);
    CHECK(code(create({layer(1, "t:a", 10)}, {}, {poi})) == MapErrorCode::OutsideLayerBounds);
    poi.position = At(1, std::nan(""), 0);
    CHECK(code(create({layer(1, "t:a", 10)}, {}, {poi})) == MapErrorCode::InvalidPosition);
    poi.position = At(7, 0, 0);
    CHECK(code(create({layer(1, "t:a", 10)}, {}, {poi})) == MapErrorCode::UnknownLayer);
    CHECK(code(WorldMapDefinition::Create({layer(1, "t:a", 10)}, {}, {}, 0.0)) == MapErrorCode::InvalidCell);
}

TEST_CASE("WorldMapDefinition: input order does not change the definition") {
    const auto a = MakeTestDefinition();
    std::vector<PointOfInterestDef> reversed(a->Pois().rbegin(), a->Pois().rend());
    std::vector<MapRegionDef> regionsReversed(a->Regions().rbegin(), a->Regions().rend());
    std::vector<MapLayerDef> layersReversed(a->Layers().rbegin(), a->Layers().rend());
    auto b = WorldMapDefinition::Create(layersReversed, regionsReversed, reversed);
    REQUIRE(b.HasValue());
    REQUIRE(b.Value().Pois().size() == a->Pois().size());
    for (std::size_t i = 0; i < a->Pois().size(); ++i) {
        CHECK(b.Value().Pois()[i].id == a->Pois()[i].id);
    }
    CHECK(b.Value().Layers().front().id == a->Layers().front().id);
}

TEST_CASE("WorldMapDefinition: regions at a position, smallest first, per layer") {
    const auto def = MakeTestDefinition();
    const auto inDistrict = def->RegionsAt(At(kSurface, -3000, 0));
    REQUIRE(inDistrict.size() == 2);
    CHECK(inDistrict[0]->id == Cid("test:region.district")); // nested: most specific first
    CHECK(inDistrict[1]->id == Cid("test:region.island_a"));
    CHECK(def->RegionsAt(At(kSurface, 15000, 0)).front()->id == Cid("test:region.island_b")); // a second island
    CHECK(def->RegionsAt(At(kSurface, 5000, 0)).empty());                                     // open sea between the islands
    CHECK(def->RegionsAt(At(kUnderground, 0, 0)).front()->id == Cid("test:region.cave"));
    CHECK(def->RegionsAt(At(kUnderground, -3000, 0)).empty()); // surface regions do not leak into other layers
    CHECK(def->RegionsAt(At(kSurface, 0, -10000)).front()->id == Cid("test:region.island_a")); // bounds are inclusive
}

// --- Markers --------------------------------------------------------------------------------------------------------------------

namespace {
MarkerSpec Spec(MapPosition p, const char* category = "test:cat.note") {
    MarkerSpec s;
    s.position = p;
    s.category = Cid(category);
    return s;
}
} // namespace

TEST_CASE("WorldMapState: add, find and remove markers; ids are stable and never reused") {
    WorldMapState map(MakeTestDefinition());
    auto a = map.AddMarker(Spec(At(kSurface, 10, 10)));
    auto b = map.AddMarker(Spec(At(kSurface, 20, 20)));
    REQUIRE(a.HasValue());
    REQUIRE(b.HasValue());
    CHECK(a.Value() == MarkerId(1));
    CHECK(b.Value() == MarkerId(2));
    CHECK(map.MarkerCount() == 2);
    CHECK(map.FindMarker(a.Value())->position.xy == Vec2{10, 10});
    CHECK_FALSE(map.RemoveMarker(a.Value()).has_value());
    CHECK(map.RemoveMarker(a.Value())->code == MapErrorCode::UnknownMarker); // second removal fails
    CHECK(map.FindMarker(a.Value()) == nullptr);
    auto c = map.AddMarker(Spec(At(kSurface, 30, 30)));
    CHECK(c.Value() == MarkerId(3)); // id 1 is not reused
    CHECK(map.QueryRadius(At(kSurface, 10, 10), 5.0).Value().empty()); // removed marker left the spatial index too
}

TEST_CASE("WorldMapState: player waypoint helper and marker validation") {
    WorldMapState map(MakeTestDefinition());
    const auto wp = map.AddPlayerWaypoint(At(kSurface, 500, 500), "meet here");
    REQUIRE(wp.HasValue());
    const Marker* m = map.FindMarker(wp.Value());
    CHECK(m->category == PlayerWaypointCategory());
    CHECK(m->origin == MarkerOrigin::Player);
    CHECK(m->persistence == MarkerPersistence::Persistent);
    CHECK(m->label == "meet here");

    const auto code = [&](MarkerSpec s) {
        const auto r = map.AddMarker(std::move(s));
        REQUIRE_FALSE(r.HasValue());
        return r.Error().code;
    };
    CHECK(code(Spec(At(77, 0, 0))) == MapErrorCode::UnknownLayer);
    CHECK(code(Spec(At(kSurface, 40000, 0))) == MapErrorCode::OutsideLayerBounds);
    CHECK(code(Spec(At(kSurface, std::nan(""), 0))) == MapErrorCode::InvalidPosition);
    CHECK(code(Spec(At(kFloor0, 51, 0))) == MapErrorCode::OutsideLayerBounds); // an interior floor is small
    MarkerSpec noCategory = Spec(At(kSurface, 0, 0));
    noCategory.category = ContentId();
    CHECK(code(noCategory) == MapErrorCode::InvalidMarker);
    MarkerSpec badLabel = Spec(At(kSurface, 0, 0));
    badLabel.label = std::string(kMaxMapLabelLength + 1, 'x');
    CHECK(code(badLabel) == MapErrorCode::InvalidLabel);
    MarkerSpec badLink = Spec(At(kSurface, 0, 0));
    badLink.linkedPoi = Cid("test:poi.nope");
    CHECK(code(badLink) == MapErrorCode::UnknownEntry);
    MarkerSpec persistentWithExpiry = Spec(At(kSurface, 0, 0));
    persistentWithExpiry.expiresAt = 10.0;
    CHECK(code(persistentWithExpiry) == MapErrorCode::InvalidMarker);
    MarkerSpec nanExpiry = Spec(At(kSurface, 0, 0));
    nanExpiry.persistence = MarkerPersistence::Temporary;
    nanExpiry.expiresAt = std::nan("");
    CHECK(code(nanExpiry) == MapErrorCode::InvalidMarker);
    CHECK(map.MarkerCount() == 1); // failed adds left nothing behind
}

TEST_CASE("WorldMapState: player marker limit counts only player markers") {
    MapStateConfig config;
    config.maxPlayerMarkers = 2;
    WorldMapState map(MakeTestDefinition(), config);
    REQUIRE(map.AddPlayerWaypoint(At(kSurface, 1, 1)).HasValue());
    const auto second = map.AddPlayerWaypoint(At(kSurface, 2, 2));
    REQUIRE(second.HasValue());
    CHECK(map.AddPlayerWaypoint(At(kSurface, 3, 3)).Error().code == MapErrorCode::TooManyPlayerMarkers);
    MarkerSpec system = Spec(At(kSurface, 4, 4));
    system.origin = MarkerOrigin::System;
    CHECK(map.AddMarker(system).HasValue());
    CHECK_FALSE(map.RemoveMarker(second.Value()).has_value());
    CHECK(map.AddPlayerWaypoint(At(kSurface, 3, 3)).HasValue()); // a slot is free again
}

TEST_CASE("WorldMapState: move marker updates the index and validates first") {
    WorldMapState map(MakeTestDefinition());
    const MarkerId id = map.AddPlayerWaypoint(At(kSurface, 0, 0)).Value();
    CHECK_FALSE(map.MoveMarker(id, At(kSurface, 5000, 5000)).has_value());
    CHECK(map.QueryRadius(At(kSurface, 0, 0), 10).Value().empty());
    CHECK(map.QueryRadius(At(kSurface, 5000, 5000), 10).Value().size() == 1);
    CHECK_FALSE(map.MoveMarker(id, At(kUnderground, 0, 0)).has_value()); // moving to another layer is allowed
    CHECK(map.QueryRadius(At(kUnderground, 0, 0), 10).Value().size() == 1);
    CHECK(map.MoveMarker(id, At(kSurface, 99999, 0))->code == MapErrorCode::OutsideLayerBounds);
    CHECK(map.FindMarker(id)->position == At(kUnderground, 0, 0)); // failed move changed nothing
    CHECK(map.MoveMarker(MarkerId(99), At(kSurface, 0, 0))->code == MapErrorCode::UnknownMarker);
}

TEST_CASE("WorldMapState: temporary markers expire, persistent ones never do") {
    WorldMapState map(MakeTestDefinition());
    MarkerSpec t1 = Spec(At(kSurface, 1, 1));
    t1.persistence = MarkerPersistence::Temporary;
    t1.expiresAt = 10.0;
    MarkerSpec t2 = t1;
    t2.expiresAt = 20.0;
    MarkerSpec forever = t1;
    forever.expiresAt.reset(); // temporary without expiry stays until removed
    const MarkerId a = map.AddMarker(t1).Value();
    const MarkerId b = map.AddMarker(t2).Value();
    const MarkerId c = map.AddMarker(forever).Value();
    const MarkerId d = map.AddPlayerWaypoint(At(kSurface, 2, 2)).Value();
    CHECK(map.PruneExpired(9.999) == 0);
    CHECK(map.PruneExpired(std::nan("")) == 0);
    CHECK(map.PruneExpired(10.0) == 1); // expiresAt <= now
    CHECK(map.FindMarker(a) == nullptr);
    CHECK(map.PruneExpired(1e12) == 1);
    CHECK(map.FindMarker(b) == nullptr);
    CHECK(map.FindMarker(c) != nullptr);
    CHECK(map.FindMarker(d) != nullptr);
}

// --- Discovery and visibility ---------------------------------------------------------------------------------------------------

TEST_CASE("WorldMapState: discovery only moves forward and validates ids") {
    WorldMapState map(MakeTestDefinition());
    const ContentId shop = Cid("test:poi.shop");
    CHECK(map.Discovery(shop) == DiscoveryState::Undiscovered);
    CHECK(map.Discover(shop, DiscoveryState::Known).Value());
    CHECK_FALSE(map.Discover(shop, DiscoveryState::Known).Value()); // no change
    CHECK(map.Discover(shop, DiscoveryState::Visited).Value());
    CHECK_FALSE(map.Discover(shop, DiscoveryState::Known).Value()); // never lowered
    CHECK(map.Discovery(shop) == DiscoveryState::Visited);
    CHECK(map.Discover(Cid("test:poi.unknown"), DiscoveryState::Known).Error().code == MapErrorCode::UnknownEntry);
    CHECK(map.Discover(Cid("test:region.cave"), DiscoveryState::Known).Value()); // regions are discoverable too
    CHECK(map.Discovery(Cid("test:region.cave")) == DiscoveryState::Known);
}

TEST_CASE("WorldMapState: POI visibility follows its rule and discovery") {
    WorldMapState map(MakeTestDefinition());
    const auto names = [&](const MapFilter& f = {}) {
        std::vector<std::string> out;
        const auto result = map.QueryRadius(At(kSurface, 0, 0), 1000, f);
        for (const MapQueryHit& h : result.Value()) {
            out.push_back(h.poi->id.Value());
        }
        return out;
    };
    CHECK(names() == std::vector<std::string>{"test:poi.church"}); // Always; the shop is WhenKnown, the secret is Hidden
    REQUIRE(map.Discover(Cid("test:poi.shop"), DiscoveryState::Known).HasValue());
    CHECK(names() == std::vector<std::string>{"test:poi.shop", "test:poi.church"}); // nearest first (100 m, 300 m)
    REQUIRE(map.Discover(Cid("test:poi.secret"), DiscoveryState::Visited).HasValue());
    CHECK(names().size() == 2); // Hidden stays hidden even when visited
    MapFilter all;
    all.includeHidden = true;
    CHECK(names(all).size() == 3);
}

TEST_CASE("WorldMapState: discovery by proximity and by region") {
    WorldMapState map(MakeTestDefinition());
    CHECK(map.DiscoverPoisAround(At(kSurface, 0, 0), 150) == 2); // shop (100 m) and secret (100 m)
    CHECK(map.DiscoverPoisAround(At(kSurface, 0, 0), 150) == 0); // idempotent
    CHECK(map.Discovery(Cid("test:poi.farm")) == DiscoveryState::Undiscovered);
    CHECK(map.DiscoverPoisAround(At(kUnderground, 0, 0), 1e6) == 1); // only the underground POI: layers are separate
    CHECK(map.DiscoverRegionsAt(At(kSurface, -3000, 0)) == 2);       // district + island_a
    CHECK(map.Discovery(Cid("test:region.district")) == DiscoveryState::Visited);
    CHECK(map.DiscoverRegionsAt(At(kSurface, 5000, 0)) == 0);        // open sea
}

// --- Queries --------------------------------------------------------------------------------------------------------------------

TEST_CASE("WorldMapState: queries are per layer, sorted and deterministic") {
    WorldMapState map(MakeTestDefinition());
    const MarkerId far = map.AddPlayerWaypoint(At(kSurface, 0, 80)).Value();
    const MarkerId near = map.AddPlayerWaypoint(At(kSurface, 0, 20)).Value();
    const MarkerId tieA = map.AddPlayerWaypoint(At(kSurface, 50, 0)).Value();
    const MarkerId tieB = map.AddPlayerWaypoint(At(kSurface, -50, 0)).Value();
    (void)map.AddPlayerWaypoint(At(kUnderground, 0, 0));
    const auto hits = map.QueryRadius(At(kSurface, 0, 0), 100).Value();
    REQUIRE(hits.size() == 4); // nothing from the underground layer
    CHECK(hits[0].marker->id == near);
    CHECK(hits[1].marker->id == tieA); // equal distance: lower id first
    CHECK(hits[2].marker->id == tieB);
    CHECK(hits[3].marker->id == far);
    for (std::size_t i = 1; i < hits.size(); ++i) {
        CHECK(hits[i - 1].distanceMetres <= hits[i].distanceMetres);
    }
    CHECK(map.QueryRadius(At(kSurface, 0, 0), 100).Value().size() == hits.size()); // repeatable
}

TEST_CASE("WorldMapState: query validation") {
    WorldMapState map(MakeTestDefinition());
    const auto code = [](const Result<std::vector<MapQueryHit>, MapError>& r) {
        REQUIRE_FALSE(r.HasValue());
        return r.Error().code;
    };
    CHECK(code(map.QueryRadius(At(99, 0, 0), 10)) == MapErrorCode::InvalidQuery);
    CHECK(code(map.QueryRadius(At(kSurface, std::nan(""), 0), 10)) == MapErrorCode::InvalidQuery);
    CHECK(code(map.QueryRadius(At(kSurface, 0, 0), -1)) == MapErrorCode::InvalidQuery);
    CHECK(code(map.QueryRadius(At(kSurface, 0, 0), kInf)) == MapErrorCode::InvalidQuery);
    CHECK(code(map.QueryRect(MapLayerId(kSurface), MapRect{{1, 1}, {0, 0}})) == MapErrorCode::InvalidQuery);
    CHECK(map.QueryRadius(At(kSurface, 1e9, 1e9), 10).Value().empty()); // a center far outside the world is fine, it just finds nothing
}

TEST_CASE("WorldMapState: filters by category, persistence and origin") {
    WorldMapState map(MakeTestDefinition());
    MarkerSpec note = Spec(At(kSurface, 10, 0), "test:cat.note");
    MarkerSpec temp = Spec(At(kSurface, 20, 0), "test:cat.alert");
    temp.persistence = MarkerPersistence::Temporary;
    temp.origin = MarkerOrigin::System;
    MarkerSpec hidden = Spec(At(kSurface, 30, 0), "test:cat.note");
    hidden.visible = false;
    const MarkerId noteId = map.AddMarker(note).Value();
    const MarkerId tempId = map.AddMarker(temp).Value();
    const MarkerId hiddenId = map.AddMarker(hidden).Value();
    const auto ids = [&](const MapFilter& f) {
        std::vector<MarkerId> out;
        const auto result = map.QueryRadius(At(kSurface, 0, 0), 50, f);
        for (const MapQueryHit& h : result.Value()) {
            if (h.marker != nullptr) {
                out.push_back(h.marker->id);
            }
        }
        return out;
    };
    MapFilter f;
    CHECK(ids(f) == std::vector<MarkerId>{noteId, tempId}); // hidden marker excluded
    f.includeHidden = true;
    CHECK(ids(f) == std::vector<MarkerId>{noteId, tempId, hiddenId});
    f = MapFilter{};
    f.categories = {Cid("test:cat.alert")};
    CHECK(ids(f) == std::vector<MarkerId>{tempId});
    f = MapFilter{};
    f.includeTemporaryMarkers = false;
    CHECK(ids(f) == std::vector<MarkerId>{noteId});
    f = MapFilter{};
    f.includeSystemMarkers = false;
    CHECK(ids(f) == std::vector<MarkerId>{noteId});
    f = MapFilter{};
    f.includePlayerMarkers = false;
    CHECK(ids(f) == std::vector<MarkerId>{tempId});
    f = MapFilter{};
    f.includePersistentMarkers = false;
    CHECK(ids(f) == std::vector<MarkerId>{tempId});
    f = MapFilter{};
    f.includePois = false;
    CHECK(map.QueryRadius(At(kSurface, 0, 0), 1000, f).Value().size() == 2); // the two visible markers; the church POI is excluded, the hidden marker too
    CHECK(map.SetMarkerVisible(hiddenId, true) == std::nullopt);
    CHECK(ids(MapFilter{}).size() == 3);
}

TEST_CASE("WorldMapState: rect query matches the same entries as the radius query would") {
    WorldMapState map(MakeTestDefinition());
    (void)map.AddPlayerWaypoint(At(kSurface, 10, 10));
    (void)map.AddPlayerWaypoint(At(kSurface, 90, -90));
    (void)map.AddPlayerWaypoint(At(kSurface, 200, 0));
    const auto hits = map.QueryRect(MapLayerId(kSurface), MapRect{{0, -100}, {100, 100}}).Value();
    REQUIRE(hits.size() == 2); // the shop at (100, 0) lies inside but is WhenKnown and undiscovered; (200, 0) lies outside
    CHECK(hits[0].marker->position.xy == Vec2{10, 10});
}

TEST_CASE("WorldMapState: large coordinates keep metre precision") {
    WorldMapState map(MakeTestDefinition());
    const double x = 9'999'000.0;
    const double y = -9'998'500.0;
    const MarkerId id = map.AddPlayerWaypoint(At(kFar, x + 0.25, y)).Value();
    const auto hits = map.QueryRadius(At(kFar, x, y), 0.5).Value();
    REQUIRE(hits.size() == 1);
    CHECK(hits[0].marker->id == id);
    CHECK(hits[0].distanceMetres == doctest::Approx(0.25).epsilon(1e-12)); // not smeared by the 1e7 magnitude
    CHECK(map.QueryRadius(At(kFar, x, y), 0.2).Value().empty());
    const auto poiHits = map.QueryRadius(At(kFar, 9'900'000.0, -9'900'000.0), 1.0).Value();
    REQUIRE(poiHits.size() == 1);
    CHECK(poiHits[0].poi->id == Cid("test:poi.edge"));
}

TEST_CASE("WorldMapState: islands and multi-level interiors are ordinary layers") {
    WorldMapState map(MakeTestDefinition());
    (void)map.AddPlayerWaypoint(At(kFloor0, 1, 1));
    (void)map.AddPlayerWaypoint(At(kFloor1, 1, 1));
    CHECK(map.QueryRadius(At(kFloor0, 0, 0), 10).Value().size() == 1);
    CHECK(map.QueryRadius(At(kFloor1, 0, 0), 10).Value().size() == 1);
    CHECK(map.QueryRadius(At(kSurface, 0, 0), 10).Value().empty());
    const auto def = MakeTestDefinition();
    CHECK(def->FindLayer(MapLayerId(kFloor0))->group == def->FindLayer(MapLayerId(kFloor1))->group);
    CHECK_FALSE(PlanarDistance(At(kFloor0, 0, 0), At(kFloor1, 0, 0)).has_value());
    CHECK(PlanarDistance(At(kFloor0, 0, 0), At(kFloor0, 3, 4)).value() == doctest::Approx(5.0));
}

// --- Persistence ----------------------------------------------------------------------------------------------------------------

namespace {
WorldMapState PopulatedState() {
    WorldMapState map(MakeTestDefinition());
    REQUIRE(map.Discover(Cid("test:poi.shop"), DiscoveryState::Visited).HasValue());
    REQUIRE(map.Discover(Cid("test:region.cave"), DiscoveryState::Known).HasValue());
    REQUIRE(map.AddPlayerWaypoint(At(kSurface, 123.456789012345, -987.5), "a label with spaces and 100% weird/chars \xC3\xA4").HasValue());
    MarkerSpec linked = Spec(At(kSurface, 100, 0), "test:cat.note");
    linked.linkedPoi = Cid("test:poi.shop");
    linked.origin = MarkerOrigin::System;
    linked.visible = false;
    REQUIRE(map.AddMarker(linked).HasValue());
    MarkerSpec temp = Spec(At(kSurface, 7, 7));
    temp.persistence = MarkerPersistence::Temporary;
    REQUIRE(map.AddMarker(temp).HasValue());
    REQUIRE(map.AddPlayerWaypoint(At(kFar, 9'999'999.5, -0.001)).HasValue());
    return map;
}
} // namespace

TEST_CASE("WorldMapState persistence: round trip keeps saved state, drops temporary markers, text is deterministic") {
    const WorldMapState original = PopulatedState();
    const std::string text = original.Serialize();
    CHECK(text.starts_with("SPN-MAPSTATE 1\n"));
    CHECK(text == original.Serialize());
    auto loaded = WorldMapState::Deserialize(text, MakeTestDefinition());
    REQUIRE(loaded.HasValue());
    CHECK(loaded.Value().SameSavedState(original));
    CHECK(loaded.Value().Serialize() == text);
    CHECK(loaded.Value().MarkerCount() == original.MarkerCount() - 1); // the temporary marker is runtime only
    const Marker* m = loaded.Value().FindMarker(MarkerId(1));
    REQUIRE(m != nullptr);
    CHECK(m->label == "a label with spaces and 100% weird/chars \xC3\xA4");
    CHECK(m->position.xy == Vec2{123.456789012345, -987.5}); // exact double round trip
    CHECK(loaded.Value().Discovery(Cid("test:poi.shop")) == DiscoveryState::Visited);
    // The id counter survives, so a loaded game never reuses an id (the temporary marker had id 3).
    CHECK(loaded.Value().AddPlayerWaypoint(At(kSurface, 1, 1)).Value() == MarkerId(5));
    // Queries work on the loaded state (spatial index rebuilt).
    CHECK(loaded.Value().QueryRadius(At(kSurface, 123.456789012345, -987.5), 1).Value().size() == 1);
}

TEST_CASE("WorldMapState persistence: empty state round trips") {
    WorldMapState map(MakeTestDefinition());
    auto loaded = WorldMapState::Deserialize(map.Serialize(), MakeTestDefinition());
    REQUIRE(loaded.HasValue());
    CHECK(loaded.Value().SameSavedState(map));
}

TEST_CASE("WorldMapState persistence: corrupt and incompatible data is rejected as a whole") {
    const auto def = MakeTestDefinition();
    const std::string good = PopulatedState().Serialize();
    const auto code = [&](const std::string& text) {
        const auto r = WorldMapState::Deserialize(text, def);
        REQUIRE_FALSE(r.HasValue());
        return r.Error().code;
    };
    const auto replace = [&](std::string text, const std::string& from, const std::string& to) {
        const auto at = text.find(from);
        REQUIRE(at != std::string::npos);
        text.replace(at, from.size(), to);
        return text;
    };
    CHECK(code("") == MapErrorCode::InvalidHeader);
    CHECK(code("NOT-A-MAP 1\n") == MapErrorCode::InvalidHeader);
    CHECK(code(replace(good, "SPN-MAPSTATE 1", "SPN-MAPSTATE 2")) == MapErrorCode::UnsupportedVersion); // newer: never half-read
    CHECK(code(good.substr(0, good.size() - 4)) == MapErrorCode::TruncatedData);                        // "end\n" cut off
    CHECK(code(good.substr(0, good.size() - 1)) == MapErrorCode::TruncatedData);                        // missing final newline
    CHECK(code(good + "m 9 1 0 0 base:x - player 1 %\n") == MapErrorCode::TrailingData);
    CHECK(code(replace(good, "markers 3", "markers 4")) == MapErrorCode::CountMismatch);
    CHECK(code(replace(good, "discovery 2", "discovery 3")) != MapErrorCode::TruncatedData); // still rejected, some code
    CHECK(code(replace(good, "d test:poi.shop visited", "d test:poi.nothing visited")) == MapErrorCode::UnknownEntry);
    CHECK(code(replace(good, "d test:poi.shop visited", "d test:poi.shop maybe")) == MapErrorCode::InvalidToken);
    CHECK(code(replace(good, "m 1 1 123.456789012345", "m 1 99 123.456789012345")) == MapErrorCode::UnknownLayer);
    CHECK(code(replace(good, "m 1 1 123.456789012345 -987.5", "m 1 1 123.456789012345 -98700.5")) == MapErrorCode::OutsideLayerBounds);
    CHECK(code(replace(good, "123.456789012345", "123.4567890123450")) == MapErrorCode::InvalidToken); // non-canonical number
    CHECK(code(replace(good, "123.456789012345", "nan")) == MapErrorCode::InvalidToken);
    CHECK(code(replace(good, "nextmarker 5", "nextmarker 2")) == MapErrorCode::InvalidToken); // a marker id >= next
    CHECK(code(replace(good, "nextmarker 5", "nextmarker 0")) == MapErrorCode::InvalidToken);
    CHECK(code(replace(good, "m 4 5", "m 2 5")) == MapErrorCode::DuplicateMarker);
}

TEST_CASE("WorldMapState persistence: player marker limit also applies when loading") {
    WorldMapState map(MakeTestDefinition());
    (void)map.AddPlayerWaypoint(At(kSurface, 1, 1));
    (void)map.AddPlayerWaypoint(At(kSurface, 2, 2));
    MapStateConfig strict;
    strict.maxPlayerMarkers = 1;
    const auto r = WorldMapState::Deserialize(map.Serialize(), MakeTestDefinition(), strict);
    REQUIRE_FALSE(r.HasValue());
    CHECK(r.Error().code == MapErrorCode::TooManyPlayerMarkers);
}

TEST_CASE("WorldMapState persistence: frozen version-1 text still loads and re-serializes byte for byte") {
    // Hand-written golden text. If this test needs to change, the on-disk format changed: that is a save-compatibility event
    // (bump the version and keep a loader for version 1), not a test update.
    const std::string golden =
        "SPN-MAPSTATE 1\n"
        "nextmarker 3\n"
        "discovery 2\n"
        "d test:poi.shop visited\n"
        "d test:region.cave known\n"
        "markers 2\n"
        "m 1 1 100.5 -20 base:marker.player_waypoint - player 1 meet%20here\n"
        "m 2 1 100 0 test:cat.note test:poi.shop system 0 %\n"
        "end\n";
    auto loaded = WorldMapState::Deserialize(golden, MakeTestDefinition());
    REQUIRE(loaded.HasValue());
    const WorldMapState& map = loaded.Value();
    CHECK(map.Discovery(Cid("test:poi.shop")) == DiscoveryState::Visited);
    CHECK(map.Discovery(Cid("test:region.cave")) == DiscoveryState::Known);
    REQUIRE(map.FindMarker(MarkerId(1)) != nullptr);
    CHECK(map.FindMarker(MarkerId(1))->label == "meet here");
    CHECK(map.FindMarker(MarkerId(1))->position == At(kSurface, 100.5, -20));
    REQUIRE(map.FindMarker(MarkerId(2)) != nullptr);
    CHECK_FALSE(map.FindMarker(MarkerId(2))->visible);
    CHECK(map.FindMarker(MarkerId(2))->linkedPoi == Cid("test:poi.shop"));
    CHECK(map.FindMarker(MarkerId(2))->label.empty());
    CHECK(map.Serialize() == golden);
}
