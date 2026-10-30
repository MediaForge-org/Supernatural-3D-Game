#pragma once

// Private to the SpnCore module: the Core-typed state behind USpnMapSubsystem (Decision 0006). Shared with the save subsystem (friend).

#include "SpnMapSubsystem.h"

#include <spn/core/Minimap.h>
#include <spn/core/Navigation.h>
#include <spn/core/WorldMap.h>

#include <memory>
#include <optional>

struct USpnMapSubsystem::FState
{
	std::shared_ptr<const spn::core::WorldMapDefinition> Definition;
	std::optional<spn::core::WorldMapState> Map;
	spn::core::MapLayerId SurfaceLayer;
};
