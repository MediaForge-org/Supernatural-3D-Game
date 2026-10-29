#pragma once

// Internal helpers shared by the canonical text parsers (not part of the public API).

#include <array>
#include <charconv>
#include <cmath>
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

/// Percent-encoding for free text in space-separated records: [A-Za-z0-9_.-] stay, every other byte becomes %XX
/// (uppercase hex); the empty string is the lone token "%". Canonical: PercentDecode accepts only what PercentEncode writes.
inline std::string PercentEncode(std::string_view text) {
    static constexpr char kHex[] = "0123456789ABCDEF";
    std::string out;
    for (const char ch : text) {
        const auto c = static_cast<unsigned char>(ch);
        const bool plain = (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == '_' || ch == '.' || ch == '-';
        if (plain) {
            out.push_back(ch);
        } else {
            out.push_back('%');
            out.push_back(kHex[c >> 4]);
            out.push_back(kHex[c & 15]);
        }
    }
    if (out.empty()) {
        out = "%";
    }
    return out;
}

inline std::optional<std::string> PercentDecode(std::string_view token) {
    if (token == "%") {
        return std::string();
    }
    const auto hex = [](char c) -> int { return (c >= '0' && c <= '9') ? c - '0' : (c >= 'A' && c <= 'F') ? c - 'A' + 10 : -1; };
    std::string out;
    for (std::size_t i = 0; i < token.size(); ++i) {
        if (token[i] != '%') {
            out.push_back(token[i]);
            continue;
        }
        if (i + 2 >= token.size()) {
            return std::nullopt;
        }
        const int hi = hex(token[i + 1]);
        const int lo = hex(token[i + 2]);
        if (hi < 0 || lo < 0) {
            return std::nullopt;
        }
        out.push_back(static_cast<char>(hi * 16 + lo));
        i += 2;
    }
    if (PercentEncode(out) != token) {
        return std::nullopt; // non-canonical spelling
    }
    return out;
}

/// Shortest decimal text that parses back to exactly the same double (std::to_chars). Precondition: finite.
inline std::string FormatDouble(double value) {
    std::array<char, 64> buffer{};
    const auto result = std::to_chars(buffer.data(), buffer.data() + buffer.size(), value);
    return std::string(buffer.data(), result.ptr);
}

/// Finite double in its canonical spelling only (what FormatDouble writes).
inline std::optional<double> ParseCanonicalDouble(std::string_view text) {
    double value = 0.0;
    const char* const end = text.data() + text.size();
    // NOLINTNEXTLINE(bugprone-suspicious-stringview-data-usage): the end pointer is passed.
    const auto [ptr, ec] = std::from_chars(text.data(), end, value);
    if (ec != std::errc{} || ptr != end || !std::isfinite(value) || FormatDouble(value) != text) {
        return std::nullopt;
    }
    return value;
}

} // namespace spn::core::detail
