#include <spn/core/CasePrototype.h>

namespace spn::core {

namespace {

constexpr std::string_view kWitness = "placeholder.case.proto.witness_interviewed";
constexpr std::string_view kEvidence = "placeholder.case.proto.evidence_found";
constexpr std::string_view kResolved = "placeholder.case.proto.resolved";
constexpr std::string_view kCauseInterview = "case-proto:interview";
constexpr std::string_view kCauseFind = "case-proto:find-evidence";
constexpr std::string_view kCauseClose = "case-proto:close";

struct Flags {
    bool witness = false;
    bool evidence = false;
    bool resolved = false;
};

Result<Flags, CaseProtoError> ReadFlags(const WorldFacts& facts) {
    Flags flags;
    const auto read = [&facts](std::string_view key, bool& out) -> bool {
        const FactValue* value = facts.Find(key);
        if (value == nullptr) return true;
        const bool* flag = std::get_if<bool>(value);
        if (flag == nullptr) return false;
        out = *flag;
        return true;
    };
    if (!read(kWitness, flags.witness) || !read(kEvidence, flags.evidence) || !read(kResolved, flags.resolved)) {
        return CaseProtoError{CaseProtoErrorCode::StateCorrupt, {}};
    }
    return flags;
}

Result<bool, CaseProtoError> SetFlag(WorldFacts& facts, std::string_view key, std::string_view cause) {
    const auto flags = ReadFlags(facts);
    if (!flags) return flags.Error();
    const FactValue* current = facts.Find(key);
    if (current != nullptr && std::get_if<bool>(current) != nullptr && *std::get_if<bool>(current)) {
        return false; // already set: writes nothing
    }
    const auto applied = facts.Apply(SetFact{std::string(key), true}, cause);
    if (!applied) return CaseProtoError{CaseProtoErrorCode::Store, applied.Error()};
    return true;
}

} // namespace

std::string_view CaseProtoWitnessKey() noexcept { return kWitness; }
std::string_view CaseProtoEvidenceKey() noexcept { return kEvidence; }
std::string_view CaseProtoResolvedKey() noexcept { return kResolved; }

Result<CaseProtoStatus, CaseProtoError> ReadCaseProtoStatus(const WorldFacts& facts) {
    const auto flags = ReadFlags(facts);
    if (!flags) return flags.Error();
    if (flags.Value().resolved) return CaseProtoStatus::Resolved;
    if (flags.Value().witness && flags.Value().evidence) return CaseProtoStatus::Ready;
    return CaseProtoStatus::Open;
}

Result<bool, CaseProtoError> InterviewCaseProtoWitness(WorldFacts& facts) {
    const auto flags = ReadFlags(facts);
    if (!flags) return flags.Error();
    if (flags.Value().resolved) return CaseProtoError{CaseProtoErrorCode::AlreadyResolved, {}};
    return SetFlag(facts, kWitness, kCauseInterview);
}

Result<bool, CaseProtoError> FindCaseProtoEvidence(WorldFacts& facts) {
    const auto flags = ReadFlags(facts);
    if (!flags) return flags.Error();
    if (flags.Value().resolved) return CaseProtoError{CaseProtoErrorCode::AlreadyResolved, {}};
    return SetFlag(facts, kEvidence, kCauseFind);
}

Result<CaseProtoStatus, CaseProtoError> CloseCaseProto(WorldFacts& facts) {
    const auto flags = ReadFlags(facts);
    if (!flags) return flags.Error();
    if (flags.Value().resolved) return CaseProtoError{CaseProtoErrorCode::AlreadyResolved, {}};
    if (!(flags.Value().witness && flags.Value().evidence)) {
        return CaseProtoError{CaseProtoErrorCode::NotReady, {}};
    }
    const auto set = SetFlag(facts, kResolved, kCauseClose);
    if (!set) return set.Error();
    return CaseProtoStatus::Resolved;
}

std::string_view ToString(CaseProtoStatus status) noexcept {
    switch (status) {
        case CaseProtoStatus::Open: return "open";
        case CaseProtoStatus::Ready: return "ready";
        case CaseProtoStatus::Resolved: return "resolved";
    }
    return "unknown";
}

std::string_view ToString(CaseProtoErrorCode code) noexcept {
    switch (code) {
        case CaseProtoErrorCode::NotReady: return "not_ready";
        case CaseProtoErrorCode::AlreadyResolved: return "already_resolved";
        case CaseProtoErrorCode::StateCorrupt: return "state_corrupt";
        case CaseProtoErrorCode::Store: return "store";
    }
    return "unknown";
}

} // namespace spn::core
