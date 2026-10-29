#pragma once

// Private to the SpnCore module: the Core-typed state behind USpnWorldStateSubsystem. Not on any public include path, so Core types
// never reach the public headers (Decision 0006). Shared by the world-state and save subsystems (USpnSaveSubsystem is a friend).

#include "SpnWorldStateSubsystem.h"

#include <spn/core/Interaction.h>
#include <spn/core/WorldFacts.h>

struct USpnWorldStateSubsystem::FState
{
	spn::core::WorldFacts Facts;
	spn::core::InteractionRegistry Registry;
};
