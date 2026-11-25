#pragma once

// Local identifiers for investigation content (evidence, dialogue, NPC, node, option).
// Grammar: [a-z][a-z0-9_]*, at most kMaxLocalIdLength characters. No dots and no colons,
// so a local id can be embedded in a WorldFacts key without ambiguity (Core/src/WorldFacts).
// The same grammar as the pickup item ids (Pickup.h), so every id in a key is unambiguous.

#include <cstddef>
#include <string_view>

namespace spn::core {

inline constexpr std::size_t kMaxLocalIdLength = 48;

[[nodiscard]] constexpr bool IsValidLocalId(std::string_view id) noexcept {
    if (id.empty() || id.size() > kMaxLocalIdLength) return false;
    const char first = id.front();
    if (!(first >= 'a' && first <= 'z')) return false;
    for (const char c : id) {
        const bool ok = (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_';
        if (!ok) return false;
    }
    return true;
}

} // namespace spn::core
