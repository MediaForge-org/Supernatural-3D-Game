#include "spn/core/SaveSlot.h"

#include <array>
#include <charconv>

#include "TextParsing.h"

namespace spn::core {

namespace {

constexpr std::array<std::uint32_t, 256> MakeCrcTable() {
    std::array<std::uint32_t, 256> table{};
    for (std::uint32_t i = 0; i < 256; ++i) {
        std::uint32_t c = i;
        for (int k = 0; k < 8; ++k) c = (c & 1u) ? (0xEDB88320u ^ (c >> 1)) : (c >> 1);
        table[i] = c;
    }
    return table;
}
constexpr auto kCrcTable = MakeCrcTable();

std::string Hex8(std::uint32_t v) {
    static constexpr char kDigits[] = "0123456789abcdef";
    std::string s(8, '0');
    for (int i = 7; i >= 0; --i) {
        s[static_cast<std::size_t>(i)] = kDigits[v & 0xFu];
        v >>= 4;
    }
    return s;
}

std::optional<std::uint32_t> ParseHex8(std::string_view text) {
    if (text.size() != 8) return std::nullopt;
    std::uint32_t v = 0;
    for (const char c : text) {
        std::uint32_t d;
        if (c >= '0' && c <= '9') d = static_cast<std::uint32_t>(c - '0');
        else if (c >= 'a' && c <= 'f') d = static_cast<std::uint32_t>(c - 'a') + 10u;
        else return std::nullopt;
        v = (v << 4) | d;
    }
    return v;
}

SaveError Err(SaveErrorCode code, std::string detail) { return SaveError{code, std::move(detail)}; }

std::vector<std::string_view> SplitSpaces(std::string_view line) {
    std::vector<std::string_view> out;
    std::size_t start = 0;
    while (true) {
        const std::size_t sp = line.find(' ', start);
        if (sp == std::string_view::npos) {
            out.push_back(line.substr(start));
            return out;
        }
        out.push_back(line.substr(start, sp - start));
        start = sp + 1;
    }
}

} // namespace

std::uint32_t Crc32(std::string_view data) noexcept {
    std::uint32_t c = 0xFFFFFFFFu;
    for (const char ch : data) c = kCrcTable[(c ^ static_cast<unsigned char>(ch)) & 0xFFu] ^ (c >> 8);
    return c ^ 0xFFFFFFFFu;
}

std::string_view ToString(SaveErrorCode code) noexcept {
    switch (code) {
    case SaveErrorCode::BadHeader: return "BadHeader";
    case SaveErrorCode::UnsupportedContainerVersion: return "UnsupportedContainerVersion";
    case SaveErrorCode::MalformedLine: return "MalformedLine";
    case SaveErrorCode::Truncated: return "Truncated";
    case SaveErrorCode::BadLength: return "BadLength";
    case SaveErrorCode::SectionChecksumMismatch: return "SectionChecksumMismatch";
    case SaveErrorCode::TrailerChecksumMismatch: return "TrailerChecksumMismatch";
    case SaveErrorCode::TrailingData: return "TrailingData";
    case SaveErrorCode::CountMismatch: return "CountMismatch";
    case SaveErrorCode::InvalidSectionId: return "InvalidSectionId";
    case SaveErrorCode::DuplicateSection: return "DuplicateSection";
    case SaveErrorCode::SectionOrder: return "SectionOrder";
    case SaveErrorCode::MissingRequiredSection: return "MissingRequiredSection";
    case SaveErrorCode::UnsupportedSectionVersion: return "UnsupportedSectionVersion";
    case SaveErrorCode::UnsupportedRequiredSection: return "UnsupportedRequiredSection";
    case SaveErrorCode::SectionRejected: return "SectionRejected";
    case SaveErrorCode::MissingContext: return "MissingContext";
    case SaveErrorCode::ConsistencyMismatch: return "ConsistencyMismatch";
    case SaveErrorCode::Io: return "Io";
    case SaveErrorCode::VerifyFailed: return "VerifyFailed";
    case SaveErrorCode::NoValidSave: return "NoValidSave";
    }
    return "Unknown";
}

bool IsValidSectionId(std::string_view id) noexcept {
    if (id.empty() || id.size() > kMaxSectionIdLength) return false;
    bool segmentStart = true;
    for (const char c : id) {
        const bool word = (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_';
        if (c == '.') {
            if (segmentStart) return false; // leading or double dot
            segmentStart = true;
        } else if (word) {
            segmentStart = false;
        } else {
            return false;
        }
    }
    return !segmentStart; // no trailing dot
}

std::optional<SaveError> SaveSlot::Put(SaveSection section) {
    if (!IsValidSectionId(section.id)) return Err(SaveErrorCode::InvalidSectionId, section.id);
    if (section.version == 0) return Err(SaveErrorCode::MalformedLine, section.id + ": version 0");
    if (section.payload.size() > kMaxSectionPayloadBytes) return Err(SaveErrorCode::BadLength, section.id);
    if (!sections_.contains(section.id) && sections_.size() >= kMaxSaveSections) return Err(SaveErrorCode::BadLength, "too many sections");
    const std::string key = section.id;
    sections_.insert_or_assign(key, std::move(section));
    return std::nullopt;
}

std::optional<SaveError> SaveSlot::Add(SaveSection section) {
    if (sections_.contains(section.id)) return Err(SaveErrorCode::DuplicateSection, section.id);
    return Put(std::move(section));
}

bool SaveSlot::Remove(std::string_view id) {
    const auto it = sections_.find(id);
    if (it == sections_.end()) return false;
    sections_.erase(it);
    return true;
}

const SaveSection* SaveSlot::Find(std::string_view id) const {
    const auto it = sections_.find(id);
    return it == sections_.end() ? nullptr : &it->second;
}

std::string SaveSlot::Serialize() const {
    std::string out = "SPN-SAVESLOT " + std::to_string(kSaveSlotFormatVersion) + "\nsections " + std::to_string(sections_.size()) + "\n";
    for (const auto& [id, s] : sections_) { // std::map: ascending id order
        out += "section " + id + " " + std::to_string(s.version) + (s.required ? " req " : " opt ") + std::to_string(s.payload.size()) + " " +
               Hex8(Crc32(s.payload)) + "\n";
        out += s.payload;
        out += '\n';
    }
    out += "end " + Hex8(Crc32(out)) + "\n";
    return out;
}

Result<SaveSlot, SaveError> SaveSlot::Parse(std::string_view bytes) {
    std::size_t pos = 0;
    // returns the line without '\n'; nullopt at end of data without a newline
    const auto line = [&]() -> std::optional<std::string_view> {
        const std::size_t nl = bytes.find('\n', pos);
        if (nl == std::string_view::npos) return std::nullopt;
        const auto l = bytes.substr(pos, nl - pos);
        pos = nl + 1;
        return l;
    };

    const auto header = line();
    if (!header) return Err(bytes.starts_with("SPN-SAVESLOT") ? SaveErrorCode::Truncated : SaveErrorCode::BadHeader, "no header line");
    if (!header->starts_with("SPN-SAVESLOT ")) return Err(SaveErrorCode::BadHeader, "not an SPN-SAVESLOT file");
    const auto version = detail::ParseCanonicalInteger<std::uint32_t>(header->substr(13));
    if (!version) return Err(SaveErrorCode::BadHeader, "container version is not a number");
    if (*version != kSaveSlotFormatVersion) return Err(SaveErrorCode::UnsupportedContainerVersion, "container version " + std::to_string(*version));

    const auto countLine = line();
    if (!countLine) return Err(SaveErrorCode::Truncated, "no section count");
    if (!countLine->starts_with("sections ")) return Err(SaveErrorCode::MalformedLine, "expected 'sections N'");
    const auto count = detail::ParseCanonicalInteger<std::size_t>(countLine->substr(9));
    if (!count) return Err(SaveErrorCode::MalformedLine, "section count is not a canonical number");
    if (*count > kMaxSaveSections) return Err(SaveErrorCode::BadLength, "section count " + std::to_string(*count));

    SaveSlot slot;
    std::string previousId;
    for (std::size_t i = 0; i < *count; ++i) {
        const auto h = line();
        if (!h) return Err(SaveErrorCode::Truncated, "missing section header " + std::to_string(i));
        if (h->starts_with("end ")) return Err(SaveErrorCode::CountMismatch, "declared " + std::to_string(*count) + " sections, found " + std::to_string(i));
        const auto f = SplitSpaces(*h);
        if (f.size() != 6 || f[0] != "section") return Err(SaveErrorCode::MalformedLine, "section header " + std::to_string(i));
        const std::string id(f[1]);
        if (!IsValidSectionId(id)) return Err(SaveErrorCode::InvalidSectionId, id);
        const auto ver = detail::ParseCanonicalInteger<std::uint32_t>(f[2]);
        if (!ver || *ver == 0) return Err(SaveErrorCode::MalformedLine, id + ": version");
        if (f[3] != "req" && f[3] != "opt") return Err(SaveErrorCode::MalformedLine, id + ": flag");
        const auto len = detail::ParseCanonicalInteger<std::size_t>(f[4]);
        if (!len || *len > kMaxSectionPayloadBytes) return Err(SaveErrorCode::BadLength, id + ": length");
        const auto crc = ParseHex8(f[5]);
        if (!crc) return Err(SaveErrorCode::MalformedLine, id + ": checksum");
        if (i > 0) {
            if (id == previousId) return Err(SaveErrorCode::DuplicateSection, id);
            if (id < previousId) return Err(SaveErrorCode::SectionOrder, id + " after " + previousId);
        }
        if (bytes.size() - pos < *len + 1) return Err(SaveErrorCode::Truncated, id + ": payload");
        const auto payload = bytes.substr(pos, *len);
        if (bytes[pos + *len] != '\n') return Err(SaveErrorCode::MalformedLine, id + ": payload not followed by newline");
        if (Crc32(payload) != *crc) return Err(SaveErrorCode::SectionChecksumMismatch, id);
        pos += *len + 1;
        previousId = id;
        SaveSection s;
        s.id = id;
        s.version = *ver;
        s.required = f[3] == "req";
        s.payload = std::string(payload);
        if (auto e = slot.Add(std::move(s))) return *e;
    }

    const std::size_t trailerStart = pos;
    const auto trailer = line();
    if (!trailer) return Err(SaveErrorCode::Truncated, "no trailer");
    if (!trailer->starts_with("end ")) return Err(SaveErrorCode::CountMismatch, "more sections than declared, or malformed trailer");
    const auto crc = ParseHex8(trailer->substr(4));
    if (!crc) return Err(SaveErrorCode::MalformedLine, "trailer checksum");
    if (Crc32(bytes.substr(0, trailerStart)) != *crc) return Err(SaveErrorCode::TrailerChecksumMismatch, "");
    if (pos != bytes.size()) return Err(SaveErrorCode::TrailingData, std::to_string(bytes.size() - pos) + " bytes");
    return slot;
}

std::optional<SaveError> SaveSlot::Validate(const SaveSchema& schema) const {
    for (const auto& spec : schema.sections) {
        if (spec.required && !sections_.contains(spec.id)) return Err(SaveErrorCode::MissingRequiredSection, spec.id);
    }
    for (const auto& [id, s] : sections_) {
        const SectionSpec* spec = nullptr;
        for (const auto& candidate : schema.sections) {
            if (candidate.id == id) { spec = &candidate; break; }
        }
        if (spec == nullptr) {
            if (s.required) return Err(SaveErrorCode::UnsupportedRequiredSection, id);
        } else if (s.version > spec->maxVersion && (s.required || spec->required)) {
            return Err(SaveErrorCode::UnsupportedSectionVersion, id + " v" + std::to_string(s.version) + " > v" + std::to_string(spec->maxVersion));
        }
    }
    return std::nullopt;
}

std::vector<std::string> SaveSlot::UnreadSections(const SaveSchema& schema) const {
    std::vector<std::string> out;
    for (const auto& [id, s] : sections_) {
        bool readable = false;
        for (const auto& spec : schema.sections) {
            if (spec.id == id && s.version <= spec.maxVersion) { readable = true; break; }
        }
        if (!readable) out.push_back(id);
    }
    return out;
}

} // namespace spn::core
