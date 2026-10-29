// Text format "SPN-INVENTORY 1" for InventoryState. Layout (one record per line, space separated, free text percent-encoded):
//   SPN-INVENTORY 1
//   next <nextInstanceId>
//   revision <n>
//   containers <N>
//   container <id> <kind|-> <maxStacks|-> <maxWeight|-> <maxVolume|-> <categories,|-> <slots,|-> <stackCount> <equippedCount>
//   stack <id> <definition> <quantity> <fieldCount>
//   field <name> <b|i|s> <value>
//   equip <slot> <stackId>
//   end
// Containers ascend by id, stacks by id, fields by name, equip lines by slot. A file with a higher version is rejected.
#include "TextParsing.h"

#include <spn/core/Inventory.h>

namespace spn::core {

namespace {

constexpr std::string_view kMagic = "SPN-INVENTORY";

InventoryError Err(InventoryErrorCode code, std::size_t line, std::string detail = {}) { return InventoryError{code, line, std::move(detail)}; }

std::vector<std::string_view> Split(std::string_view line, char sep) {
    std::vector<std::string_view> parts;
    std::size_t start = 0;
    while (true) {
        const std::size_t end = line.find(sep, start);
        parts.push_back(line.substr(start, end == std::string_view::npos ? std::string_view::npos : end - start));
        if (end == std::string_view::npos) {
            return parts;
        }
        start = end + 1;
    }
}

std::string JoinIds(const std::set<ContentId>& ids) {
    if (ids.empty()) {
        return "-";
    }
    std::string out;
    for (const ContentId& id : ids) {
        out.append(out.empty() ? "" : ",").append(id.Value());
    }
    return out;
}

bool ParseIds(std::string_view token, std::set<ContentId>& out) {
    if (token == "-") {
        return true;
    }
    std::optional<ContentId> previous;
    for (const std::string_view part : Split(token, ',')) {
        const auto id = ContentId::Parse(part);
        if (!id || (previous && !(*previous < *id))) {
            return false; // invalid or not strictly ascending (canonical form)
        }
        previous = *id;
        out.insert(*id);
    }
    return true;
}

template <typename T> bool ParseOptionalNumber(std::string_view token, std::optional<T>& out) {
    if (token == "-") {
        return true;
    }
    const auto value = detail::ParseCanonicalInteger<T>(token);
    if (!value) {
        return false;
    }
    out = *value;
    return true;
}

template <typename T> std::string OptionalText(const std::optional<T>& v) { return v ? std::to_string(*v) : "-"; }

} // namespace

std::string InventoryState::Serialize() const {
    std::string out;
    out.append(kMagic).append(" 1\n");
    out.append("next ").append(std::to_string(nextInstance_)).append("\nrevision ").append(std::to_string(revision_)).append("\n");
    out.append("containers ").append(std::to_string(containers_.size())).append("\n");
    for (const auto& [cid, c] : containers_) {
        out.append("container ").append(std::to_string(cid.Value())).append(" ").append(c.rules.kind.IsEmpty() ? "-" : c.rules.kind.Value());
        out.append(" ").append(OptionalText(c.rules.maxStacks)).append(" ").append(OptionalText(c.rules.maxWeightGrams)).append(" ");
        out.append(OptionalText(c.rules.maxVolumeMilliliters)).append(" ").append(JoinIds(c.rules.allowedCategories)).append(" ");
        out.append(JoinIds(c.rules.equipSlots)).append(" ").append(std::to_string(c.stacks.size())).append(" ").append(std::to_string(c.equipped.size())).append("\n");
        for (const auto& [sid, s] : c.stacks) {
            out.append("stack ").append(std::to_string(sid.Value())).append(" ").append(s.definition.Value()).append(" ");
            out.append(std::to_string(s.quantity)).append(" ").append(std::to_string(s.state.size())).append("\n");
            for (const auto& [name, value] : s.state) {
                out.append("field ").append(name).append(" ");
                if (const bool* b = std::get_if<bool>(&value)) {
                    out.append("b ").append(*b ? "1" : "0");
                } else if (const auto* i = std::get_if<std::int64_t>(&value)) {
                    out.append("i ").append(std::to_string(*i));
                } else {
                    out.append("s ").append(detail::PercentEncode(std::get<std::string>(value)));
                }
                out.append("\n");
            }
        }
        for (const auto& [slot, sid] : c.equipped) {
            out.append("equip ").append(slot.Value()).append(" ").append(std::to_string(sid.Value())).append("\n");
        }
    }
    out.append("end\n");
    return out;
}

Result<InventoryState, InventoryError> InventoryState::Deserialize(std::string_view text, const ItemCatalog& catalog) {
    std::vector<std::string_view> lines;
    for (std::size_t start = 0; start < text.size();) {
        const std::size_t end = text.find('\n', start);
        lines.push_back(text.substr(start, end == std::string_view::npos ? std::string_view::npos : end - start));
        start = end == std::string_view::npos ? text.size() : end + 1;
    }
    if (lines.empty()) {
        return Err(InventoryErrorCode::InvalidHeader, 1);
    }
    if (text.back() != '\n') {
        return Err(InventoryErrorCode::TruncatedData, lines.size(), "missing final newline");
    }
    std::size_t at = 0;
    const auto line = [&]() { return at + 1; };
    const auto words = [&](std::string_view keyword, std::size_t count) -> std::optional<std::vector<std::string_view>> {
        if (at >= lines.size()) {
            return std::nullopt;
        }
        auto parts = Split(lines[at], ' ');
        if (parts.size() != count || parts[0] != keyword) {
            return std::nullopt;
        }
        return parts;
    };

    {
        const auto parts = Split(lines[0], ' ');
        const auto version = parts.size() == 2 ? detail::ParseCanonicalInteger<std::uint64_t>(parts[1]) : std::nullopt;
        if (parts[0] != kMagic || !version) {
            return Err(InventoryErrorCode::InvalidHeader, 1);
        }
        if (*version != 1) {
            return Err(InventoryErrorCode::UnsupportedVersion, 1, std::string(parts[1]));
        }
        at = 1;
    }
    const auto number = [&](std::string_view keyword, std::uint64_t& out) -> std::optional<InventoryError> {
        if (at >= lines.size()) {
            return Err(InventoryErrorCode::TruncatedData, line());
        }
        const auto parts = words(keyword, 2);
        if (!parts) {
            return Err(InventoryErrorCode::MalformedLine, line(), std::string(keyword));
        }
        const auto n = detail::ParseCanonicalInteger<std::uint64_t>((*parts)[1]);
        if (!n) {
            return Err(InventoryErrorCode::InvalidToken, line(), std::string(keyword));
        }
        out = *n;
        ++at;
        return std::nullopt;
    };

    InventoryState state(catalog);
    std::uint64_t containerCount = 0;
    if (auto e = number("next", state.nextInstance_)) {
        return *e;
    }
    if (auto e = number("revision", state.revision_)) {
        return *e;
    }
    if (state.nextInstance_ == 0) {
        return Err(InventoryErrorCode::InvalidToken, 2, "next must be >= 1");
    }
    if (auto e = number("containers", containerCount)) {
        return *e;
    }
    if (containerCount > kMaxContainers) {
        return Err(InventoryErrorCode::LimitExceeded, line());
    }

    std::uint64_t previousContainer = 0;
    for (std::uint64_t ci = 0; ci < containerCount; ++ci, ++at) {
        if (at >= lines.size()) {
            return Err(InventoryErrorCode::CountMismatch, line(), "containers");
        }
        const auto p = words("container", 10);
        if (!p) {
            return Err(InventoryErrorCode::MalformedLine, line(), "container");
        }
        Container c;
        const auto id = detail::ParseCanonicalInteger<std::uint64_t>((*p)[1]);
        const auto kind = (*p)[2] == "-" ? std::optional<ContentId>(ContentId()) : ContentId::Parse((*p)[2]);
        const auto stackCount = detail::ParseCanonicalInteger<std::uint64_t>((*p)[8]);
        const auto equipCount = detail::ParseCanonicalInteger<std::uint64_t>((*p)[9]);
        if (!id || *id == 0 || !kind || !stackCount || !equipCount || !ParseOptionalNumber((*p)[3], c.rules.maxStacks) ||
            !ParseOptionalNumber((*p)[4], c.rules.maxWeightGrams) || !ParseOptionalNumber((*p)[5], c.rules.maxVolumeMilliliters) ||
            !ParseIds((*p)[6], c.rules.allowedCategories) || !ParseIds((*p)[7], c.rules.equipSlots)) {
            return Err(InventoryErrorCode::InvalidToken, line(), "container");
        }
        if (*id <= previousContainer) {
            return Err(InventoryErrorCode::MalformedLine, line(), "container ids must be strictly ascending");
        }
        if (*stackCount > kMaxStacksPerContainer) {
            return Err(InventoryErrorCode::LimitExceeded, line());
        }
        previousContainer = *id;
        c.id = ContainerId(*id);
        c.rules.kind = *kind;

        std::uint64_t previousStack = 0;
        for (std::uint64_t si = 0; si < *stackCount; ++si) {
            ++at;
            if (at >= lines.size()) {
                return Err(InventoryErrorCode::CountMismatch, line(), "stacks");
            }
            const auto sp = words("stack", 5);
            if (!sp) {
                return Err(InventoryErrorCode::MalformedLine, line(), "stack");
            }
            const auto sid = detail::ParseCanonicalInteger<std::uint64_t>((*sp)[1]);
            const auto def = ContentId::Parse((*sp)[2]);
            const auto qty = detail::ParseCanonicalInteger<std::uint32_t>((*sp)[3]);
            const auto fields = detail::ParseCanonicalInteger<std::uint64_t>((*sp)[4]);
            if (!sid || *sid == 0 || !def || !qty || !fields || *fields > kMaxItemStateFields) {
                return Err(InventoryErrorCode::InvalidToken, line(), "stack");
            }
            if (*sid <= previousStack) {
                return Err(InventoryErrorCode::MalformedLine, line(), "stack ids must be strictly ascending");
            }
            previousStack = *sid;
            ItemStack stack;
            stack.id = ItemInstanceId(*sid);
            stack.definition = *def;
            stack.quantity = *qty;
            for (std::uint64_t fi = 0; fi < *fields; ++fi) {
                ++at;
                if (at >= lines.size()) {
                    return Err(InventoryErrorCode::CountMismatch, line(), "fields");
                }
                const auto fp = words("field", 4);
                if (!fp || !IsValidFieldName((*fp)[1]) || (*fp)[2].size() != 1) {
                    return Err(InventoryErrorCode::MalformedLine, line(), "field");
                }
                FactValue value;
                const char type = (*fp)[2][0];
                if (type == 'b' && ((*fp)[3] == "0" || (*fp)[3] == "1")) {
                    value = (*fp)[3] == "1";
                } else if (const auto n = type == 'i' ? detail::ParseCanonicalInteger<std::int64_t>((*fp)[3]) : std::nullopt; n) {
                    value = *n;
                } else if (const auto s = type == 's' ? detail::PercentDecode((*fp)[3]) : std::nullopt; s) {
                    value = *s;
                } else {
                    return Err(InventoryErrorCode::InvalidToken, line(), "field value");
                }
                if (!stack.state.emplace(std::string((*fp)[1]), std::move(value)).second) {
                    return Err(InventoryErrorCode::MalformedLine, line(), "duplicate field");
                }
            }
            c.stacks.emplace(stack.id, std::move(stack));
        }
        for (std::uint64_t ei = 0; ei < *equipCount; ++ei) {
            ++at;
            if (at >= lines.size()) {
                return Err(InventoryErrorCode::CountMismatch, line(), "equipment");
            }
            const auto ep = words("equip", 3);
            const auto slot = ep ? ContentId::Parse((*ep)[1]) : std::nullopt;
            const auto sid = ep ? detail::ParseCanonicalInteger<std::uint64_t>((*ep)[2]) : std::nullopt;
            if (!ep || !slot || !sid || *sid == 0) {
                return Err(InventoryErrorCode::MalformedLine, line(), "equip");
            }
            if (!c.equipped.emplace(*slot, ItemInstanceId(*sid)).second) {
                return Err(InventoryErrorCode::MalformedLine, line(), "duplicate slot");
            }
        }
        (void)RecomputeTotals(c, catalog); // an unknown definition or an overflow is reported by CheckInvariants below
        state.containers_.emplace(c.id, std::move(c));
    }
    if (at >= lines.size()) {
        return Err(InventoryErrorCode::TruncatedData, line(), "missing end");
    }
    if (lines[at] != "end") {
        return Err(InventoryErrorCode::MalformedLine, line(), "expected end");
    }
    if (at + 1 != lines.size()) {
        return Err(InventoryErrorCode::TrailingData, at + 2);
    }
    if (auto e = state.CheckInvariants()) {
        return *e;
    }
    return state;
}

} // namespace spn::core
