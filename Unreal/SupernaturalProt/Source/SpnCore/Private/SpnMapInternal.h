#pragma once

// Private to the SpnCore module: the Core-typed state behind USpnMapSubsystem (Decision 0006). Shared with the save subsystem (friend).

#include "SpnMapSubsystem.h"

#include <spn/core/GeometryMath.h>
#include <spn/core/MapGeometry.h>
#include <spn/core/MapRender.h>
#include <spn/core/Minimap.h>
#include <spn/core/Navigation.h>
#include <spn/core/NavigationSession.h>
#include <spn/core/SyntheticMapRegion.h>
#include <spn/core/WorldMap.h>

#include <memory>
#include <optional>
#include <set>

struct USpnMapSubsystem::FState
{
	std::shared_ptr<const spn::core::WorldMapDefinition> Definition;
	std::optional<spn::core::WorldMapState> Map;
	spn::core::MapLayerId SurfaceLayer;

	// Road navigation (only with InstallSyntheticRegion). The routing itself is the Core's; nothing is re-implemented here.
	std::shared_ptr<const spn::core::MapGeometry> Geometry;
	std::optional<spn::core::synthetic::SyntheticMapRegion> Region;
	std::unique_ptr<spn::core::NavigationSession> Nav;
	spn::core::NavigationUpdate LastUpdate;
	std::set<spn::core::RoadSegmentId> Closed;
	std::optional<spn::core::MapPosition> LastPlayer;
	spn::core::TravelMode Mode = spn::core::TravelMode::Driving;
};
