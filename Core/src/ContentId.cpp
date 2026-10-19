#include <spn/core/ContentId.h>
#include <utility>

namespace spn::core {

namespace {

bool IsLower(char ch) noexcept {
    return ch >= 'a' && ch <= 'z';
}

bool IsIdentifierTail(char ch) noexcept {
    return IsLower(ch) || (ch >= '0' && ch <= '9') || ch == '_';
}

/// [a-z][a-z0-9_]*
bool IsSegment(std::string_view text) noexcept {
    if (text.empty() || !IsLower(text.front())) {
        return false;
    }
    for (const char ch : text) {
        if (!IsIdentifierTail(ch)) {
            return false;
        }
    }
    return true;
}

} // namespace

bool ContentId::IsValid(std::string_view text) noexcept {
    if (text.empty() || text.size() > kMaxContentIdLength) {
        return false;
    }
    const std::size_t colon = text.find(':');
    // npos (no ':') is larger than the limit, so it is rejected here as well.
    if (colon > kMaxContentNamespaceLength || !IsSegment(text.substr(0, colon))) {
        return false;
    }
    std::string_view path = text.substr(colon + 1);
    while (true) {
        const std::size_t dot = path.find('.');
        if (!IsSegment(path.substr(0, dot))) {
            return false;
        }
        if (dot == std::string_view::npos) {
            return true;
        }
        path.remove_prefix(dot + 1);
    }
}

std::optional<ContentId> ContentId::Parse(std::string_view text) {
    if (!IsValid(text)) {
        return std::nullopt;
    }
    return ContentId{std::string(text)};
}

std::string_view ContentId::Namespace() const noexcept {
    return std::string_view(value_).substr(0, value_.find(':'));
}

} // namespace spn::core
