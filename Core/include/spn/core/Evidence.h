#pragma once

// Evidence foundation (SPN-DEV-02, decisions EVD-01 and EVD-02 in Docs/Planning/DECISIONS.md, section J).
//
// Three separate stages per evidence id, all stored in WorldFacts (no second state store):
//   known       the player has learned that this evidence exists (a clue or a conversation)
//   found       the player has physically found it in the world (set by the world object)
//   registered  the player has collected it; the journal registers it only now (EVD-01)
// Collecting requires "found". A registered entry is never registered twice. Nothing here
// decides that a case is solved (EVD-04, see conflict K-35).
//
// Keys (WorldFacts, all bool):
//   placeholder.evidence.<id>.known
//   placeholder.evidence.<id>.found
//   placeholder.journal.evidence.<id>       the journal registration
// The placeholder. prefix marks the keys as non-final (renaming is a save-compatibility event).
//
// Monster-related evidence and further categories are prepared by EvidenceDef::category
// only; no category logic is implemented here (EVD-03).

#include <cstdint>
#include <optional>
#include <spn/core/InvestigationIds.h>
#include <spn/core/Result.h>
#include <spn/core/WorldFacts.h>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace spn::core {

struct EvidenceDef {
    std::string id;       ///< local id, IsValidLocalId
    std::string category; ///< placeholder category id, IsValidLocalId; "general" for the prototype
};

/// Immutable list of the evidence that exists. Ids are unique; the order is the registration order.
class EvidenceCatalog {
public:
    enum class Error : std::uint8_t { InvalidId, InvalidCategory, Duplicate, Empty };

    [[nodiscard]] static Result<EvidenceCatalog, Error> Create(std::vector<EvidenceDef> defs);
    [[nodiscard]] bool Contains(std::string_view id) const noexcept;
    [[nodiscard]] const EvidenceDef* Find(std::string_view id) const noexcept;
    [[nodiscard]] const std::vector<EvidenceDef>& Defs() const noexcept { return defs_; }

private:
    explicit EvidenceCatalog(std::vector<EvidenceDef> defs) : defs_(std::move(defs)) {}
    std::vector<EvidenceDef> defs_;
};

enum class EvidenceStage : std::uint8_t { Unknown, Known, Found, Registered };

enum class EvidenceErrorCode : std::uint8_t {
    InvalidId,         ///< the id does not follow the local-id grammar
    UnknownEvidence,   ///< the id is not in the catalog
    NotFound,          ///< collecting before the evidence was found
    AlreadyRegistered, ///< the evidence is already in the journal (no second registration)
    StateCorrupt,      ///< a stored evidence fact has the wrong type
    Store              ///< WorldFacts refused the write (see the FactError)
};

struct EvidenceError {
    EvidenceErrorCode code = EvidenceErrorCode::StateCorrupt;
    std::optional<FactError> fact; ///< only for code == Store
    friend bool operator==(const EvidenceError&, const EvidenceError&) = default;
};

[[nodiscard]] std::string EvidenceKnownKey(std::string_view id);
[[nodiscard]] std::string EvidenceFoundKey(std::string_view id);
[[nodiscard]] std::string EvidenceJournalKey(std::string_view id);

/// Reads the stage of one evidence id from the facts (checks the id against the catalog).
[[nodiscard]] Result<EvidenceStage, EvidenceError> ReadEvidenceStage(const WorldFacts& facts, const EvidenceCatalog& catalog,
                                                                     std::string_view id);

/// The player learns that the evidence exists. Returns true if this changed the facts.
[[nodiscard]] Result<bool, EvidenceError> LearnEvidence(WorldFacts& facts, const EvidenceCatalog& catalog, std::string_view id,
                                                        std::string_view cause = {});
/// The world object reports the evidence as found. Returns true if this changed the facts.
/// Refused (AlreadyRegistered) once the evidence is in the journal.
[[nodiscard]] Result<bool, EvidenceError> FindEvidence(WorldFacts& facts, const EvidenceCatalog& catalog, std::string_view id,
                                                       std::string_view cause = {});
/// The player collects the found evidence; the journal registers it. Refused (NotFound) before
/// FindEvidence, refused (AlreadyRegistered) if it is already registered. Writes exactly one fact.
[[nodiscard]] Result<bool, EvidenceError> CollectEvidence(WorldFacts& facts, const EvidenceCatalog& catalog, std::string_view id,
                                                          std::string_view cause = {});

/// Registered evidence ids in catalog order (deterministic).
[[nodiscard]] Result<std::vector<std::string>, EvidenceError> RegisteredEvidence(const WorldFacts& facts,
                                                                                const EvidenceCatalog& catalog);

[[nodiscard]] std::string_view ToString(EvidenceStage stage) noexcept;
[[nodiscard]] std::string_view ToString(EvidenceErrorCode code) noexcept;

} // namespace spn::core
