#pragma once

// Unified save-slot container (M2B2): one file that carries several independent, versioned data domains ("sections").
//
// The container knows nothing about the domains. A section is an id, a format version, a required/optional flag and an opaque payload
// (for the existing domains exactly the bytes of their own Serialize(): WorldFacts journal, object records, SPN-MAPSTATE 1, SPN-INVENTORY 1).
// Existing formats are therefore embedded verbatim and keep their own parsers and tests.
//
// File format "SPN-SAVESLOT 1" (canonical; the parser accepts nothing else):
//
//   SPN-SAVESLOT 1\n
//   sections <N>\n
//   section <id> <version> <req|opt> <payload-bytes> <crc32 of payload, 8 lowercase hex digits>\n
//   <payload-bytes raw bytes>\n                 (repeated, sections in strictly ascending id order)
//   end <crc32 of every byte before the "end" line, 8 lowercase hex digits>\n
//   (end of file; any trailing byte is an error)
//
// Integrity is CRC-32 (IEEE 802.3): it detects accidental corruption (truncation, bit rot, torn writes), NOT tampering. No encryption or
// compression: nothing in the project has shown a need yet.

#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "spn/core/Result.h"

namespace spn::core {

inline constexpr std::size_t kMaxSaveSections = 1024;
inline constexpr std::size_t kMaxSectionIdLength = 64;
inline constexpr std::size_t kMaxSectionPayloadBytes = 256u * 1024u * 1024u; ///< parser limit against absurd lengths, not a design target
inline constexpr std::uint32_t kSaveSlotFormatVersion = 1;

/// CRC-32/ISO-HDLC (zlib, PNG): polynomial 0xEDB88320 reflected, init and final xor 0xFFFFFFFF. crc32("123456789") == 0xCBF43926.
[[nodiscard]] std::uint32_t Crc32(std::string_view data) noexcept;

enum class SaveErrorCode : std::uint8_t {
    // container
    BadHeader,                  ///< not an SPN-SAVESLOT file
    UnsupportedContainerVersion,
    MalformedLine,              ///< a header line does not follow the grammar (also: non-canonical numbers)
    Truncated,                  ///< the data ends before the declared structure does
    BadLength,                  ///< a declared length is absurd or does not fit
    SectionChecksumMismatch,
    TrailerChecksumMismatch,
    TrailingData,
    CountMismatch,
    InvalidSectionId,
    DuplicateSection,
    SectionOrder,               ///< sections are not in the canonical ascending order
    // schema
    MissingRequiredSection,
    UnsupportedSectionVersion,  ///< a known required section was written by a newer version than this build reads
    UnsupportedRequiredSection, ///< a required section this build does not know at all
    // domains
    SectionRejected,            ///< a domain parser rejected its payload (detail names the section and the domain error)
    MissingContext,             ///< a section is present but the data needed to read it (catalog, map definition) was not provided
    ConsistencyMismatch,        ///< sections describe different gameplay moments
    // storage
    Io,
    VerifyFailed,               ///< read-back after writing differs from what was written
    NoValidSave,
    CatalogMismatch,            ///< the inventory was saved against item definitions that differ from the current catalog (slot.catalog fingerprint)
    DefinitionMismatch,         ///< intact data that names map entries the current map definition does not have: a save of another game world
};
[[nodiscard]] std::string_view ToString(SaveErrorCode code) noexcept;

struct SaveError {
    SaveErrorCode code;
    std::string detail; ///< section id and/or reason; for humans and logs, not for logic
    friend bool operator==(const SaveError&, const SaveError&) = default;
};

/// Section ids: [a-z0-9_] segments separated by single dots, e.g. "world.facts"; no leading/trailing/double dots; 1..kMaxSectionIdLength.
[[nodiscard]] bool IsValidSectionId(std::string_view id) noexcept;

struct SaveSection {
    std::string id;
    std::uint32_t version = 1;
    bool required = false; ///< a reader that cannot interpret a required section must refuse the whole save; an optional one it keeps and ignores
    std::string payload;
    friend bool operator==(const SaveSection&, const SaveSection&) = default;
};

/// What a build knows about one section id.
struct SectionSpec {
    std::string id;
    std::uint32_t maxVersion = 1; ///< highest payload version this build reads
    bool required = false;        ///< absent => MissingRequiredSection
};

struct SaveSchema {
    std::vector<SectionSpec> sections;
};

/// An in-memory slot: a set of sections keyed by id. Value type; copying it is a snapshot.
class SaveSlot {
public:
    /// Adds a section. Fails with InvalidSectionId, DuplicateSection or BadLength (payload above kMaxSectionPayloadBytes / too many sections).
    [[nodiscard]] std::optional<SaveError> Add(SaveSection section);
    /// Replaces the section with the same id, or adds it. Same validation as Add.
    [[nodiscard]] std::optional<SaveError> Put(SaveSection section);
    bool Remove(std::string_view id);
    [[nodiscard]] const SaveSection* Find(std::string_view id) const;
    [[nodiscard]] std::size_t Count() const noexcept { return sections_.size(); }
    [[nodiscard]] const std::map<std::string, SaveSection, std::less<>>& Sections() const noexcept { return sections_; }

    /// Canonical, deterministic text. Same sections => same bytes.
    [[nodiscard]] std::string Serialize() const;
    /// Strict: validates header, counts, grammar, lengths, order, every section checksum and the trailer. Never partially succeeds.
    [[nodiscard]] static Result<SaveSlot, SaveError> Parse(std::string_view bytes);

    /// Checks the slot against what this build knows. Unknown OPTIONAL sections (and optional sections with a newer version) are fine and
    /// stay in the slot (so a rewrite keeps them); unknown required sections, newer required versions and missing required sections are errors.
    [[nodiscard]] std::optional<SaveError> Validate(const SaveSchema& schema) const;
    /// Ids of sections this build does not know or cannot read (optional only), ascending.
    [[nodiscard]] std::vector<std::string> UnreadSections(const SaveSchema& schema) const;

    friend bool operator==(const SaveSlot&, const SaveSlot&) = default;

private:
    std::map<std::string, SaveSection, std::less<>> sections_;
};

} // namespace spn::core
