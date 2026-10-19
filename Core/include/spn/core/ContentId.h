#pragma once

// ContentId: stable, namespaced, human-readable identifier for content that is
// referenced from data (appearance parts, appearance parameters, later items etc.).
//
// ENGINEERING PROPOSAL, not master canon. Text ids ("base:hair.short_01") were chosen
// over numeric ids because mods can add content under their own namespace without a
// central id allocator, and ids stay meaningful in save files and diffs.

#include <cstddef>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

namespace spn::core {

inline constexpr std::size_t kMaxContentIdLength = 96;
inline constexpr std::size_t kMaxContentNamespaceLength = 32;

/// Grammar: `<namespace>:<path>`
///  - namespace: [a-z][a-z0-9_]*, at most kMaxContentNamespaceLength chars
///  - path: dot-separated segments, each [a-z][a-z0-9_]*
///  - whole id at most kMaxContentIdLength chars
/// The default-constructed id is empty and means "none"; every non-empty id has been
/// validated by Parse.
class ContentId {
public:
    ContentId() = default;

    [[nodiscard]] static std::optional<ContentId> Parse(std::string_view text);
    [[nodiscard]] static bool IsValid(std::string_view text) noexcept;

    [[nodiscard]] const std::string& Value() const noexcept { return value_; }
    [[nodiscard]] bool IsEmpty() const noexcept { return value_.empty(); }
    /// Part before the ':'; empty for the empty id.
    [[nodiscard]] std::string_view Namespace() const noexcept;

    friend bool operator==(const ContentId&, const ContentId&) = default;
    friend auto operator<=>(const ContentId&, const ContentId&) = default;

private:
    explicit ContentId(std::string value) : value_(std::move(value)) {}
    std::string value_;
};

} // namespace spn::core
