#pragma once

// Private to the SpnCore module: the Core-typed state behind USpnNpcMemorySubsystem (Decision 0006). Shared with the save subsystem (friend).

#include "SpnNpcMemorySubsystem.h"

#include <spn/core/NpcMemory.h>
#include <spn/core/NpcReaction.h>

struct USpnNpcMemorySubsystem::FState
{
	spn::core::NpcMemoryStore Store;
};
