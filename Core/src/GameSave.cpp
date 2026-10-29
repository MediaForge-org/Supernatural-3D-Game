#include "spn/core/GameSave.h"

#include "TextParsing.h"

namespace spn::core {

namespace {

SaveError Err(SaveErrorCode code, std::string detail) { return SaveError{code, std::move(detail)}; }

std::string Meta(std::uint64_t factsRevision, std::optional<std::uint64_t> inventoryRevision) {
    std::string s = "facts_revision " + std::to_string(factsRevision) + "\n";
    if (inventoryRevision) s += "inventory_revision " + std::to_string(*inventoryRevision) + "\n";
    return s;
}

struct ParsedMeta {
    std::uint64_t factsRevision = 0;
    std::optional<std::uint64_t> inventoryRevision;
};

std::optional<ParsedMeta> ParseMeta(std::string_view text) {
    ParsedMeta meta;
    bool haveFacts = false;
    std::size_t pos = 0;
    while (pos < text.size()) {
        const std::size_t nl = text.find('\n', pos);
        if (nl == std::string_view::npos) return std::nullopt;
        const auto line = text.substr(pos, nl - pos);
        pos = nl + 1;
        const std::size_t sp = line.find(' ');
        if (sp == std::string_view::npos) return std::nullopt;
        const auto key = line.substr(0, sp);
        const auto value = detail::ParseCanonicalInteger<std::uint64_t>(line.substr(sp + 1));
        if (!value) return std::nullopt;
        if (key == "facts_revision" && !haveFacts) { meta.factsRevision = *value; haveFacts = true; }
        else if (key == "inventory_revision" && !meta.inventoryRevision) meta.inventoryRevision = *value;
        else return std::nullopt;
    }
    return haveFacts ? std::optional<ParsedMeta>(meta) : std::nullopt;
}

} // namespace

SaveSchema GameSaveSchema() {
    SaveSchema schema;
    schema.sections = {
        {std::string(kSectionFacts), 1, true},   {std::string(kSectionObjects), 1, false}, {std::string(kSectionMap), 1, false},
        {std::string(kSectionInventory), 1, false}, {std::string(kSectionMeta), 1, false},
    };
    return schema;
}

Result<SaveSlot, SaveError> CaptureGameSave(const GameSaveSources& src, const SaveSlot* previous) {
    if (src.facts == nullptr) return Err(SaveErrorCode::MissingContext, "WorldFacts is required");
    if ((src.regions == nullptr) != (src.catalog == nullptr)) return Err(SaveErrorCode::MissingContext, "regions and catalog go together");

    const std::uint64_t factsBefore = src.facts->Revision();
    const std::optional<std::uint64_t> inventoryBefore = src.inventory ? std::optional<std::uint64_t>(src.inventory->Revision()) : std::nullopt;

    SaveSlot slot;
    const auto add = [&](std::string_view id, bool required, std::string payload) -> std::optional<SaveError> {
        return slot.Add(SaveSection{std::string(id), 1, required, std::move(payload)});
    };
    if (auto e = add(kSectionFacts, true, src.facts->Serialize())) return *e;
    if (src.regions != nullptr) {
        if (auto e = add(kSectionObjects, false, SerializeRecords(src.regions->Snapshot(*src.facts, *src.catalog)))) return *e;
    }
    if (src.map != nullptr) {
        if (auto e = add(kSectionMap, false, src.map->Serialize())) return *e;
    }
    if (src.inventory != nullptr) {
        if (auto e = add(kSectionInventory, false, src.inventory->Serialize())) return *e;
    }
    if (auto e = add(kSectionMeta, false, Meta(factsBefore, inventoryBefore))) return *e;

    if (src.facts->Revision() != factsBefore || (src.inventory && src.inventory->Revision() != *inventoryBefore))
        return Err(SaveErrorCode::ConsistencyMismatch, "state changed while the snapshot was being taken");

    if (previous != nullptr) {
        for (const auto& id : previous->UnreadSections(GameSaveSchema())) {
            if (slot.Find(id) == nullptr) {
                if (auto e = slot.Add(*previous->Find(id))) return *e;
            }
        }
    }
    return slot;
}

Result<LoadedGame, SaveError> ParseGameSave(const SaveSlot& slot, const GameLoadContext& ctx) {
    const SaveSchema schema = GameSaveSchema();
    if (auto e = slot.Validate(schema)) return *e;

    const SaveSection* facts = slot.Find(kSectionFacts);
    auto parsedFacts = WorldFacts::Deserialize(facts->payload);
    if (!parsedFacts)
        return Err(SaveErrorCode::SectionRejected, std::string(kSectionFacts) + ": " + std::string(ToString(parsedFacts.Error().code)) + " line " +
                                                       std::to_string(parsedFacts.Error().line));
    LoadedGame game{std::move(parsedFacts).Value(), std::nullopt, std::nullopt, std::nullopt, slot.UnreadSections(schema)};

    const auto readable = [&](std::string_view id) {
        const SaveSection* s = slot.Find(id);
        return s != nullptr && s->version <= 1 ? s : nullptr;
    };
    if (const SaveSection* s = readable(kSectionObjects)) {
        auto records = ParseRecords(s->payload);
        if (!records)
            return Err(SaveErrorCode::SectionRejected, std::string(kSectionObjects) + ": " + std::string(ToString(records.Error().code)) + " line " +
                                                           std::to_string(records.Error().line));
        game.objects = std::move(records).Value();
    }
    if (const SaveSection* s = readable(kSectionMap)) {
        if (!ctx.mapDefinition) return Err(SaveErrorCode::MissingContext, "world.map present but no map definition given");
        auto map = WorldMapState::Deserialize(s->payload, ctx.mapDefinition, ctx.mapConfig);
        if (!map)
            return Err(SaveErrorCode::SectionRejected, std::string(kSectionMap) + ": " + std::string(ToString(map.Error().code)) + " line " +
                                                           std::to_string(map.Error().line) + " " + map.Error().detail);
        game.map = std::move(map).Value();
    }
    if (const SaveSection* s = readable(kSectionInventory)) {
        if (ctx.itemCatalog == nullptr) return Err(SaveErrorCode::MissingContext, "player.inventory present but no item catalog given");
        auto inv = InventoryState::Deserialize(s->payload, *ctx.itemCatalog);
        if (!inv)
            return Err(SaveErrorCode::SectionRejected, std::string(kSectionInventory) + ": " + std::string(ToString(inv.Error().code)) + " line " +
                                                           std::to_string(inv.Error().commandIndex) + " " + inv.Error().detail);
        game.inventory = std::move(inv).Value();
    }
    if (const SaveSection* s = readable(kSectionMeta)) {
        const auto meta = ParseMeta(s->payload);
        if (!meta) return Err(SaveErrorCode::SectionRejected, std::string(kSectionMeta) + ": malformed");
        if (meta->factsRevision != game.facts.Revision())
            return Err(SaveErrorCode::ConsistencyMismatch, "facts revision " + std::to_string(game.facts.Revision()) + " but slot.meta says " +
                                                               std::to_string(meta->factsRevision));
        if (meta->inventoryRevision) {
            if (!game.inventory) return Err(SaveErrorCode::ConsistencyMismatch, "slot.meta names an inventory revision but the section is missing");
            if (*meta->inventoryRevision != game.inventory->Revision())
                return Err(SaveErrorCode::ConsistencyMismatch, "inventory revision " + std::to_string(game.inventory->Revision()) + " but slot.meta says " +
                                                                   std::to_string(*meta->inventoryRevision));
        } else if (game.inventory) {
            return Err(SaveErrorCode::ConsistencyMismatch, "inventory section present but slot.meta has no inventory revision");
        }
    }
    return game;
}

} // namespace spn::core
