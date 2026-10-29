#pragma once

// Private to the SpnCore module: the Core-typed state behind USpnInventorySubsystem. Not on any public include path, so Core types never reach
// the public headers (Decision 0006). Shared with the save subsystem (USpnSaveSubsystem is a friend).

#include "SpnInventorySubsystem.h"

#include <spn/core/Inventory.h>

#include <memory>
#include <optional>

struct USpnInventorySubsystem::FState
{
	FSpnItemCatalogData CatalogData;
	// Declaration order matters: the state points to the catalog, so it must be destroyed first.
	std::unique_ptr<spn::core::ItemCatalog> Catalog;
	std::optional<spn::core::InventoryState> Inventory;
	FSpnInventoryStatus LastFailure;
};
