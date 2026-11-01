#include <cassert>
#include <spn/core/SyntheticMapRegion.h>

namespace spn::core::synthetic {

namespace {

ContentId Cid(const char* text) {
    auto id = ContentId::Parse(text);
    assert(id.has_value());
    return *id;
}

MapLayerId Layer() { return MapLayerId(kLayerId); }

RoadNodeDef Node(std::uint64_t id, double x, double y) { return RoadNodeDef{RoadNodeId(id), MapPosition{Layer(), Vec2{x, y}}}; }

RoadSegmentDef Segment(std::uint64_t id, std::uint64_t from, std::uint64_t to, RoadClass cls, RoadStructure structure = RoadStructure::Surface,
                       RoadDirection direction = RoadDirection::Both, std::vector<Vec2> shape = {}, bool roundabout = false) {
    RoadSegmentDef s;
    s.id = RoadSegmentId(id);
    s.from = RoadNodeId(from);
    s.to = RoadNodeId(to);
    s.roadClass = cls;
    s.structure = structure;
    s.direction = direction;
    s.shape = std::move(shape);
    s.roundabout = roundabout;
    return s;
}

BuildingDef Building(std::uint64_t id, const char* category, double x0, double y0, double x1, double y1, std::vector<Vec2> entrances, const char* label) {
    BuildingDef b;
    b.id = BuildingId(id);
    b.layer = Layer();
    b.category = Cid(category);
    b.footprint = {{x0, y0}, {x1, y0}, {x1, y1}, {x0, y1}};
    b.entrances = std::move(entrances);
    b.label = label;
    return b;
}

AreaDef Area(std::uint64_t id, AreaKind kind, std::vector<Vec2> outline, const char* label, std::optional<double> elevation = std::nullopt) {
    AreaDef a;
    a.id = MapAreaId(id);
    a.layer = Layer();
    a.kind = kind;
    a.outline = std::move(outline);
    a.label = label;
    a.elevationMetres = elevation;
    return a;
}

std::vector<Vec2> Box(double x0, double y0, double x1, double y1) { return {{x0, y0}, {x1, y0}, {x1, y1}, {x0, y1}}; }

} // namespace

std::shared_ptr<const WorldMapDefinition> MakeSyntheticMapDefinition() {
    MapLayerDef layer;
    layer.id = Layer();
    layer.key = Cid("synthetic:layer.surface");
    layer.kind = MapLayerKind::Surface;
    layer.bounds = MapRect{{-1500.0, -1500.0}, {3500.0, 2500.0}};
    const auto region = [](const char* id, MapRect bounds, const char* label) {
        MapRegionDef r;
        r.id = Cid(id);
        r.layer = Layer();
        r.bounds = bounds;
        r.category = Cid("synthetic:region_kind.district");
        r.label = label;
        return r;
    };
    const auto poi = [](const char* id, double x, double y, const char* label) {
        PointOfInterestDef p;
        p.id = Cid(id);
        p.category = Cid("synthetic:poi_kind.place");
        p.position = MapPosition{Layer(), Vec2{x, y}};
        p.label = label;
        return p;
    };
    auto def = WorldMapDefinition::Create(
        {layer},
        {region("synthetic:region.west", MapRect{{-1500.0, -1500.0}, {960.0, 2500.0}}, "Synthetic West"),
         region("synthetic:region.east", MapRect{{1040.0, -1500.0}, {3500.0, 2500.0}}, "Synthetic East"),
         region("synthetic:region.hamlet", MapRect{{2500.0, 1400.0}, {3000.0, 1900.0}}, "Synthetic Hamlet")},
        {poi("synthetic:poi.west_shop", 320.0, 45.0, "Synthetic Shop"), poi("synthetic:poi.east_house", 1500.0, 75.0, "Synthetic House"),
         poi("synthetic:poi.island", 1000.0, 130.0, "Synthetic Island"), poi("synthetic:poi.hamlet", 2870.0, 1535.0, "Synthetic Hamlet House"),
         poi("synthetic:poi.warehouse", 730.0, 60.0, "Synthetic Warehouse")});
    assert(def.HasValue());
    return std::make_shared<const WorldMapDefinition>(std::move(def).Value());
}

MapGeometryData MakeSyntheticGeometryData() {
    MapGeometryData d;
    d.nodes = {Node(kWestA, 0, 0),         Node(kWestB, 450, 0),        Node(kWestC, 900, 0),        Node(kWestD, 900, 600),      Node(kWestE, 900, 1200),
               Node(kWestFootEnd, 900, -300), Node(kEastC, 1100, 0),    Node(kEastF, 1100, 300),     Node(kEastD, 1100, 600),     Node(kEastE, 1100, 1200),
               Node(kEastFootEnd, 1100, -300), Node(kEastB, 1550, 0),   Node(kEastG, 1550, 300),     Node(kEastI, 1550, 600),     Node(kEastH, 2000, 600),
               Node(kRing1, 2030, 0),      Node(kRing2, 2000, 30),      Node(kRing3, 1970, 0),       Node(kRing4, 2000, -30),     Node(31, -500, -800),
               Node(kMotorJ1, 0, -800),    Node(kMotorTunnelWest, 900, -800), Node(kMotorTunnelEast, 1100, -800), Node(kMotorJ2, 2000, -800), Node(36, 3000, -800),
               Node(kHamletA, 2600, 1500), Node(kHamletB, 2900, 1500),  Node(kHamletC, 2900, 1800),  Node(51, 0, 600),            Node(52, 300, 1000),
               Node(61, -300, 0),          Node(62, 1550, -250)};
    using C = RoadClass;
    using S = RoadStructure;
    using D = RoadDirection;
    d.segments = {
        Segment(kStreetWestAB, kWestA, kWestB, C::Residential),
        Segment(kStreetWestBC, kWestB, kWestC, C::Residential),
        Segment(kRoadWestCD, kWestC, kWestD, C::Secondary),
        Segment(kRoadWestDE, kWestD, kWestE, C::Secondary),
        Segment(kRoadWestFootApproach, kWestFootEnd, kWestC, C::Residential),
        Segment(kNorthBridge, kWestE, kEastE, C::Primary, S::Bridge),
        Segment(kFootBridge, kWestFootEnd, kEastFootEnd, C::Footway, S::Bridge),
        Segment(kFootEastApproach, kEastFootEnd, kEastC, C::Footway),
        Segment(kRoadEastCF, kEastC, kEastF, C::Residential),
        Segment(kRoadEastFD, kEastF, kEastD, C::Residential),
        Segment(kRoadEastDE, kEastD, kEastE, C::Secondary),
        Segment(kStreetEastCB, kEastC, kEastB, C::Residential),
        Segment(kOneWayEastFG, kEastF, kEastG, C::Residential, S::Surface, D::Forward),
        Segment(kStreetEastGB, kEastG, kEastB, C::Residential),
        Segment(kStreetEastGI, kEastG, kEastI, C::Residential),
        Segment(kStreetEastDI, kEastD, kEastI, C::Residential),
        Segment(kStreetEastIH, kEastI, kEastH, C::Residential),
        Segment(kStreetEastBRing, kEastB, kRing3, C::Residential),
        Segment(kRingSegment1, kRing1, kRing2, C::Residential, S::Surface, D::Forward, {{2021, 21}}, true),
        Segment(kRingSegment2, kRing2, kRing3, C::Residential, S::Surface, D::Forward, {{1979, 21}}, true),
        Segment(kRingSegment3, kRing3, kRing4, C::Residential, S::Surface, D::Forward, {{1979, -21}}, true),
        Segment(kRingSegment4, kRing4, kRing1, C::Residential, S::Surface, D::Forward, {{2021, -21}}, true),
        Segment(kRingExitNorth, kRing2, kEastH, C::Residential),
        Segment(kSpurEastB, kEastB, 62, C::Service),
        Segment(kMotorWestToJ1, 31, kMotorJ1, C::Motorway),
        Segment(kMotorJ1ToTunnel, kMotorJ1, kMotorTunnelWest, C::Motorway),
        Segment(kMotorTunnel, kMotorTunnelWest, kMotorTunnelEast, C::Motorway, S::Tunnel),
        Segment(kMotorTunnelToJ2, kMotorTunnelEast, kMotorJ2, C::Motorway),
        Segment(kMotorJ2East, kMotorJ2, 36, C::Motorway),
        Segment(kOnRampWest, kWestA, kMotorJ1, C::MotorwayLink, S::Surface, D::Forward, {{30, -400}}),
        Segment(kOffRampWest, kMotorJ1, kWestA, C::MotorwayLink, S::Surface, D::Forward, {{-30, -400}}),
        Segment(kOnRampEast, kRing4, kMotorJ2, C::MotorwayLink, S::Surface, D::Forward, {{1980, -400}}),
        Segment(kOffRampEast, kMotorJ2, kRing4, C::MotorwayLink, S::Surface, D::Forward, {{2020, -400}}),
        Segment(kFieldTrack, kWestA, 51, C::FieldTrack),
        Segment(kForestTrack, 51, 52, C::ForestTrack, S::Surface, D::Both, {{150, 800}}),
        Segment(kWestSpur, kWestA, 61, C::Residential),
        Segment(kHamletRoadAB, kHamletA, kHamletB, C::Residential),
        Segment(kHamletRoadBC, kHamletB, kHamletC, C::Residential),
    };
    // No vehicle may turn from the south road (E_F-E_D) onto the east street at E_D; pedestrians may.
    d.restrictions = {TurnRestriction{RoadNodeId(kEastD), RoadSegmentId(kRoadEastFD), RoadSegmentId(kStreetEastDI), true}};

    d.buildings = {
        Building(kShopWest, "synthetic:building.shop", 300, 30, 340, 60, {{320, 30}}, "Synthetic Shop"),
        Building(kHouseEast, "synthetic:building.house", 1480, 60, 1520, 90, {{1500, 60}}, "Synthetic House"),
        Building(kHouseNoEntrance, "synthetic:building.house", 600, 100, 640, 130, {}, "Synthetic House without entrance"),
        Building(kWarehouseTwoEntrances, "synthetic:building.warehouse", 700, 40, 760, 80, {{730, 40}, {730, 80}}, "Synthetic Warehouse"),
        Building(kHamletHouse, "synthetic:building.house", 2850, 1520, 2880, 1550, {{2865, 1520}}, "Synthetic Hamlet House"),
    };
    // A row of identical houses on the south side of the west street (a building block for rendering and culling tests).
    for (std::uint64_t i = 0; i < 8; ++i) {
        const double x = 40.0 + 45.0 * static_cast<double>(i);
        d.buildings.push_back(Building(10 + i, "synthetic:building.house", x, -60, x + 30, -35, {{x + 15, -35}}, "Synthetic Row House"));
    }
    d.areas = {
        Area(1, AreaKind::Water, Box(970, -1000, 1030, 1800), "Synthetic River"),
        Area(2, AreaKind::Island, Box(985, 100, 1015, 160), "Synthetic Island"),
        Area(3, AreaKind::Forest, Box(-400, 500, 500, 1300), "Synthetic Forest"),
        Area(4, AreaKind::Park, Box(1200, 50, 1450, 250), "Synthetic Park"),
        Area(5, AreaKind::Plaza, Box(420, 30, 480, 90), "Synthetic Plaza"),
        Area(6, AreaKind::Parking, Box(1600, -120, 1700, -40), "Synthetic Parking"),
        Area(7, AreaKind::Industrial, Box(1600, 350, 1900, 550), "Synthetic Industrial Area"),
        Area(8, AreaKind::Green, Box(100, 150, 700, 450), "Synthetic Meadow"),
        Area(9, AreaKind::Mountain, Box(2500, -300, 3300, 300), "Synthetic Mountain", 850.0),
        Area(10, AreaKind::Water, Box(2400, 700, 2700, 1000), "Synthetic Lake"),
        Area(11, AreaKind::Water, Box(-1500, -1500, 3500, -1200), "Synthetic Sea"),
        Area(12, AreaKind::District, Box(-1000, -500, 960, 1500), "Synthetic West District"),
        Area(13, AreaKind::District, Box(1040, -500, 2200, 1500), "Synthetic East District"),
    };
    d.areas[11].region = Cid("synthetic:region.west");
    d.areas[12].region = Cid("synthetic:region.east");
    const auto line = [](std::uint64_t id, LineKind kind, std::vector<Vec2> pts, const char* label, std::optional<double> elevation = std::nullopt) {
        LineFeatureDef l;
        l.id = MapLineId(id);
        l.layer = Layer();
        l.kind = kind;
        l.points = std::move(pts);
        l.label = label;
        l.elevationMetres = elevation;
        return l;
    };
    d.lines = {line(1, LineKind::River, {{1000, -1000}, {1000, 400}, {1000, 1800}}, "Synthetic River"),
               line(2, LineKind::Coastline, {{-1500, -1200}, {3500, -1200}}, "Synthetic Coast"),
               line(3, LineKind::Contour, {{2600, -200}, {3100, -200}, {3100, 200}, {2600, 200}, {2600, -200}}, "Synthetic Contour 800", 800.0)};
    return d;
}

SyntheticMapRegion MakeSyntheticMapRegion() {
    SyntheticMapRegion r;
    r.definition = MakeSyntheticMapDefinition();
    auto geometry = MapGeometry::Create(*r.definition, MakeSyntheticGeometryData());
    assert(geometry.HasValue());
    r.geometry = std::make_shared<const MapGeometry>(std::move(geometry).Value());
    r.layer = Layer();
    const auto at = [&](double x, double y) { return MapPosition{r.layer, Vec2{x, y}}; };
    r.westStreet = at(500, 0);
    r.eastStreet = at(1500, 0);
    r.westOpenField = at(250, 80);
    r.shopInside = at(320, 45);
    r.houseInside = at(1500, 75);
    r.noEntranceInside = at(620, 115);
    r.warehouseInside = at(730, 60);
    r.islandTarget = at(1000, 130);
    r.hamletTarget = at(2870, 1535);
    return r;
}

} // namespace spn::core::synthetic
