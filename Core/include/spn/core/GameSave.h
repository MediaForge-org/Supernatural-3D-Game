#pragma once

// Adapters between the game's persistent domains and the SaveSlot container (M2B2). Each domain keeps its own format, parser and tests; the
// payload of its section is exactly the bytes of its own Serialize(), so the existing formats (WorldFacts journal, object records,
// SPN-MAPSTATE 1, SPN-INVENTORY 1) are untouched.
//
// Sections (all proposals of M2B2, version 1):
//   world.facts       required   WorldFacts::Serialize()            the whole fact store incl. the journal (objects live in it)
//   world.objects     optional   SerializeRecords(Snapshot)          changed object state incl. regions that are NOT loaded and orphans:
//                                the part of the object state the live fact store does not hold
//   world.map         optional   WorldMapState::Serialize()          SPN-MAPSTATE 1
//   player.inventory  optional   InventoryState::Serialize()         SPN-INVENTORY 1
//   slot.meta         optional   "facts_revision N" / "inventory_revision N": which gameplay moment the sections were taken at
// "optional" means: absent = that domain was never saved (start it fresh); it is not a statement that the game can live without it.
//
// SNAPSHOT BOUNDARY: CaptureGameSave takes const references and returns a value. It runs synchronously on the calling thread and never mutates
// the live state; all sections are therefore taken at the same instant PROVIDED the caller does not modify the state concurrently (the Core
// is single-threaded; a multi-threaded host must capture on the game thread). As a guard it compares the facts/inventory revision before
// and after serialization and reports ConsistencyMismatch if something changed in between (e.g. an event listener that mutated state).
// The map has no revision counter, so the guard does not cover it.
//
// LOADING is two-phase: ParseGameSave builds NEW objects from the slot and changes nothing live; the caller swaps them in only on success.

#include <memory>
#include <optional>
#include <string_view>
#include <vector>

#include "spn/core/Inventory.h"
#include "spn/core/ObjectPersistence.h"
#include "spn/core/Result.h"
#include "spn/core/SaveSlot.h"
#include "spn/core/WorldFacts.h"
#include "spn/core/WorldMap.h"

namespace spn::core {

inline constexpr std::string_view kSectionFacts = "world.facts";
inline constexpr std::string_view kSectionObjects = "world.objects";
inline constexpr std::string_view kSectionMap = "world.map";
inline constexpr std::string_view kSectionInventory = "player.inventory";
inline constexpr std::string_view kSectionMeta = "slot.meta";

/// What this build reads: world.facts required, everything else optional, all at version 1.
[[nodiscard]] SaveSchema GameSaveSchema();

struct GameSaveSources {
    const WorldFacts* facts = nullptr; ///< required
    /// Optional pair: both or neither. With them, world.objects is written from RegionStateCache::Snapshot.
    const RegionStateCache* regions = nullptr;
    const WorldObjectCatalog* catalog = nullptr;
    const WorldMapState* map = nullptr;
    const InventoryState* inventory = nullptr;
};

/// Builds a slot from the live state. `previous` (optional): sections this build does not know or cannot read (UnreadSections) are carried over
/// unchanged, so saving with an older build does not drop newer optional data.
[[nodiscard]] Result<SaveSlot, SaveError> CaptureGameSave(const GameSaveSources& sources, const SaveSlot* previous = nullptr);

/// What the parser needs to rebuild the domains that were saved against shared definitions.
struct GameLoadContext {
    std::shared_ptr<const WorldMapDefinition> mapDefinition; ///< needed iff world.map is present
    MapStateConfig mapConfig;
    const ItemCatalog* itemCatalog = nullptr;                ///< needed iff player.inventory is present; must outlive the loaded state
};

struct LoadedGame {
    WorldFacts facts;
    std::optional<std::vector<ObjectRecord>> objects;
    std::optional<WorldMapState> map;
    std::optional<InventoryState> inventory;
    std::vector<std::string> unreadSections; ///< optional sections this build ignored (and kept in the slot)
};

/// Parses every present section into new objects. Errors: SectionRejected (a domain parser said no; detail = section + domain error),
/// MissingContext, ConsistencyMismatch (slot.meta disagrees with the revisions found in the sections), plus the schema errors.
[[nodiscard]] Result<LoadedGame, SaveError> ParseGameSave(const SaveSlot& slot, const GameLoadContext& context);

} // namespace spn::core
