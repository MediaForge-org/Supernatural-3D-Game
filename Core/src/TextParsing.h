#pragma once

// Internal helpers shared by the canonical text parsers (not part of the public API).

#include <array>
#include <charconv>
#include <cstddef>
#include <optional>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>

namespace spn::core::detail {

inline std::vector<std::string_view> SplitTabs(std::string_view line) {
    std::vector<std::string_view> fields;
    std::size_t start = 0;
    while (true) {
        const std::size_t tab = line.find('\t', start);
        if (tab == std::string_view::npos) {
            fields.push_back(line.substr(start));
            return fields;
        }
        fields.push_back(line.substr(start, tab - start));
        start = tab + 1;
    }
}

/// Decimal integer in its one canonical spelling (no '+', no leading zeros, no "-0").
template <typename Int> std::optional<Int> ParseCanonicalInteger(std::string_view text) {
    Int value{};
    const char* const end = text.data() + text.size();
    // NOLINTNEXTLINE(bugprone-suspicious-stringview-data-usage): the end pointer is passed.
    const auto [ptr, ec] = std::from_chars(text.data(), end, value);
    if (ec != std::errc{} || ptr != end || std::to_string(value) != text) {
        return std::nullopt;
    }
    return value;
}

/// Finds the enumerator whose ToString (found via ADL) equals `text`.
template <typename Enum, std::size_t N>
std::optional<Enum> ParseEnum(std::string_view text, const std::array<Enum, N>& all) {
    for (const Enum value : all) {
        if (ToString(value) == text) {
            return value;
        }
    }
    return std::nullopt;
}

} // namespace spn::core::detail
