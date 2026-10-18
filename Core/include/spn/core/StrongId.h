#pragma once

#include <compare>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <functional>

namespace spn::core {

/// Type-safe identifier. `Tag` is an empty marker type that makes ids of
/// different domains (items, places, evidence, ...) mutually incompatible at
/// compile time. The underlying `Rep` is never implicitly convertible.
///
/// Convention: the value-initialized id (value 0) is the "null id".
/// No other meaning is assigned to concrete values; id allocation is the
/// caller's responsibility.
template <typename Tag, std::unsigned_integral Rep = std::uint64_t> class StrongId {
public:
    using TagType = Tag;
    using ValueType = Rep;

    constexpr StrongId() noexcept = default;
    constexpr explicit StrongId(Rep value) noexcept : value_(value) {}

    [[nodiscard]] constexpr Rep Value() const noexcept { return value_; }
    [[nodiscard]] constexpr bool IsNull() const noexcept { return value_ == 0; }

    [[nodiscard]] friend constexpr bool operator==(StrongId, StrongId) noexcept = default;
    [[nodiscard]] friend constexpr auto operator<=>(StrongId, StrongId) noexcept = default;

private:
    Rep value_ = 0;
};

} // namespace spn::core

// Specializing std::hash for user-defined types is explicitly permitted.
template <typename Tag, typename Rep>
// NOLINTNEXTLINE(bugprone-std-namespace-modification)
struct std::hash<spn::core::StrongId<Tag, Rep>> {
    [[nodiscard]] std::size_t operator()(spn::core::StrongId<Tag, Rep> id) const noexcept {
        return std::hash<Rep>{}(id.Value());
    }
};
