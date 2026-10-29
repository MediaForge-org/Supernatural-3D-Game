#pragma once

// Synthetic map fixtures for the M2B1 tests. The coordinates are arbitrary test numbers: they are NOT geography, not a map
// candidate and not related to SPN WorldForge output.

#include <doctest.h>
#include <limits>
#include <memory>
#include <spn/core/ContentId.h>
#include <spn/core/WorldMap.h>

namespace spn::testdata {

inline spn::core::ContentId Cid(std::string_view text) {
    const auto id = spn::core::ContentId::Parse(text);
    REQUIRE(id.has_value());
    return *id;
}

inline constexpr double kInf = std::numeric_limits<double>::infinity();
inline constexpr std::uint32_t kSurface = 1;
inline constexpr std::uint32_t kUnderground = 2;
inline constexpr std::uint32_t kFloor0 = 3;
inline constexpr std::uint32_t kFloor1 = 4;
inline constexpr std::uint32_t kFar = 5; // spans the full +-10,000 km coordinate range

/// Layers: surface (+-30 km, "island_a" and "island_b" regions inside it), underground, two floors of one test building and a
/// layer with the maximum coordinate range. Four POIs on the surface, one underground, one on the far layer.
inline std::shared_ptr<const spn::core::WorldMapDefinition> MakeTestDefinition(double cell = 256.0) {
    using namespace spn::core;
    const auto layer = [](std::uint32_t id, const char* key, MapLayerKind kind, const char* group, std::int32_t level, double half) {
        MapLayerDef l;
        l.id = MapLayerId(id);
        l.key = Cid(key);
        l.kind = kind;
        if (group != nullptr) {
            l.group = Cid(group);
        }
        l.level = level;
        l.bounds = MapRect{{-half, -half}, {half, half}};
        return l;
    };
    std::vector<MapLayerDef> layers{layer(kSurface, "test:layer.surface", MapLayerKind::Surface, nullptr, 0, 30000.0),
                                    layer(kUnderground, "test:layer.underground", MapLayerKind::Underground, nullptr, -1, 5000.0),
                                    layer(kFloor0, "test:layer.house_floor0", MapLayerKind::Interior, "test:group.house", 0, 50.0),
                                    layer(kFloor1, "test:layer.house_floor1", MapLayerKind::Interior, "test:group.house", 1, 50.0),
                                    layer(kFar, "test:layer.far", MapLayerKind::Realm, nullptr, 0, kMaxMapCoordinateMetres)};
    const auto region = [](const char* id, std::uint32_t layerId, MapRect bounds) {
        MapRegionDef r;
        r.id = Cid(id);
        r.layer = MapLayerId(layerId);
        r.bounds = bounds;
        r.category = Cid("test:region.generic");
        r.label = id;
        return r;
    };
    std::vector<MapRegionDef> regions{region("test:region.island_b", kSurface, {{10000, -5000}, {20000, 5000}}),
                                      region("test:region.island_a", kSurface, {{-20000, -10000}, {0, 10000}}),
                                      region("test:region.district", kSurface, {{-5000, -1000}, {-1000, 1000}}),
                                      region("test:region.cave", kUnderground, {{-100, -100}, {100, 100}})};
    const auto poi = [](const char* id, std::uint32_t layerId, double x, double y, MapVisibility v, const char* category) {
        PointOfInterestDef p;
        p.id = Cid(id);
        p.category = Cid(category);
        p.position = MapPosition{MapLayerId(layerId), Vec2{x, y}};
        p.label = id;
        p.visibility = v;
        return p;
    };
    std::vector<PointOfInterestDef> pois{poi("test:poi.shop", kSurface, 100.0, 0.0, MapVisibility::WhenKnown, "test:cat.shop"),
                                         poi("test:poi.church", kSurface, 0.0, 300.0, MapVisibility::Always, "test:cat.landmark"),
                                         poi("test:poi.secret", kSurface, -100.0, 0.0, MapVisibility::Hidden, "test:cat.landmark"),
                                         poi("test:poi.farm", kSurface, 15000.0, 0.0, MapVisibility::WhenKnown, "test:cat.shop"),
                                         poi("test:poi.vault", kUnderground, 10.0, 10.0, MapVisibility::Always, "test:cat.landmark"),
                                         poi("test:poi.edge", kFar, 9'900'000.0, -9'900'000.0, MapVisibility::Always, "test:cat.landmark")};
    auto def = WorldMapDefinition::Create(std::move(layers), std::move(regions), std::move(pois), cell);
    REQUIRE(def.HasValue());
    return std::make_shared<const WorldMapDefinition>(std::move(def).Value());
}

/// The same layers as MakeTestDefinition() but no regions and no POIs: for tests that count markers only.
inline std::shared_ptr<const spn::core::WorldMapDefinition> MakeBareDefinition() {
    using namespace spn::core;
    auto def = WorldMapDefinition::Create(MakeTestDefinition()->Layers(), {}, {});
    REQUIRE(def.HasValue());
    return std::make_shared<const WorldMapDefinition>(std::move(def).Value());
}

inline spn::core::MapPosition At(std::uint32_t layer, double x, double y) { return {spn::core::MapLayerId(layer), {x, y}}; }

/// Small deterministic generator for property-style tests (no <random>: identical on every platform).
class Lcg {
public:
    explicit Lcg(std::uint64_t seed) : s_(seed) {}
    std::uint64_t Next() {
        s_ = s_ * 6364136223846793005ULL + 1442695040888963407ULL;
        return s_ >> 11U;
    }
    double Uniform(double lo, double hi) { return lo + (hi - lo) * (static_cast<double>(Next() % 1'000'000ULL) / 999'999.0); }
    std::uint32_t Below(std::uint32_t n) { return static_cast<std::uint32_t>(Next() % n); }

private:
    std::uint64_t s_;
};

} // namespace spn::testdata
