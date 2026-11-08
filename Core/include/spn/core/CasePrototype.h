#pragma once

// PROTOTYPE-CASE-01 ("one room, one witness, one piece of evidence, one closure"),
// a PLACEHOLDER test slice of the case logic. It is NOT the design of cases:
// the closure rule (witness interviewed AND evidence found) is the simplest possible
// rule that exercises the state machine and WorldFacts persistence. The approved
// test scope is in NAMING_PROPOSAL.md section 4; the real case rules belong to
// Docs/Game/Ermittlung and are not decided here.
//
// State lives only in WorldFacts (saves with the store):
//   placeholder.case.proto.witness_interviewed  bool  absent = false
//   placeholder.case.proto.evidence_found       bool  absent = false
//   placeholder.case.proto.resolved             bool  absent = false
// Every command is idempotent for an already set fact and writes nothing in that case.

#include <cstdint>
#include <spn/core/Result.h>
#include <spn/core/WorldFacts.h>
#include <string_view>

namespace spn::core {

enum class CaseProtoStatus : std::uint8_t { Open, Ready, Resolved };

enum class CaseProtoErrorCode : std::uint8_t {
    NotReady,         ///< closure refused: witness and evidence are not both done
    AlreadyResolved,  ///< the case is closed; no further change is accepted
    StateCorrupt,     ///< a case fact exists with the wrong type
    Store             ///< the store refused the write (see FactError)
};

struct CaseProtoError {
    CaseProtoErrorCode code = CaseProtoErrorCode::StateCorrupt;
    FactError fact{FactErrorCode::ValueTooLong}; ///< only meaningful for code == Store
    friend bool operator==(const CaseProtoError&, const CaseProtoError&) = default;
};

/// Keys of the three placeholder facts.
[[nodiscard]] std::string_view CaseProtoWitnessKey() noexcept;
[[nodiscard]] std::string_view CaseProtoEvidenceKey() noexcept;
[[nodiscard]] std::string_view CaseProtoResolvedKey() noexcept;

/// Reads the status from the facts. StateCorrupt if a case fact has the wrong type.
[[nodiscard]] Result<CaseProtoStatus, CaseProtoError> ReadCaseProtoStatus(const WorldFacts& facts);

/// The witness has been interviewed. Returns true if the fact changed, false if it was already set.
[[nodiscard]] Result<bool, CaseProtoError> InterviewCaseProtoWitness(WorldFacts& facts);

/// The evidence has been found. Returns true if the fact changed, false if it was already set.
[[nodiscard]] Result<bool, CaseProtoError> FindCaseProtoEvidence(WorldFacts& facts);

/// Closes the case. Refused (NotReady) unless witness and evidence are both done;
/// refused (AlreadyResolved) if the case is already closed.
[[nodiscard]] Result<CaseProtoStatus, CaseProtoError> CloseCaseProto(WorldFacts& facts);

[[nodiscard]] std::string_view ToString(CaseProtoStatus status) noexcept;
[[nodiscard]] std::string_view ToString(CaseProtoErrorCode code) noexcept;

} // namespace spn::core
