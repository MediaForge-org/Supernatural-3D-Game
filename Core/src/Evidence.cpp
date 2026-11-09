#include <spn/core/Evidence.h>

#include <set>

namespace spn::core {

namespace {

constexpr std::string_view kCauseLearn = "evidence:learn";
constexpr std::string_view kCauseFind = "evidence:find";
constexpr std::string_view kCauseCollect = "evidence:collect";

bool ReadBool(const WorldFacts& facts, const std::string& key, bool& out, bool& corrupt) {
    const FactValue* value = facts.Find(key);
    if (value == nullptr) {
        out = false;
        return true;
    }
    const bool* flag = std::get_if<bool>(value);
    if (flag == nullptr) {
        corrupt = true;
        return false;
    }
    out = *flag;
    return true;
}

// Validates the id and the catalog membership; returns the id or an error.
Result<std::string, EvidenceError> CheckId(const EvidenceCatalog& catalog, std::string_view id) {
    if (!IsValidLocalId(id)) return EvidenceError{EvidenceErrorCode::InvalidId, std::nullopt};
    if (!catalog.Contains(id)) return EvidenceError{EvidenceErrorCode::UnknownEvidence, std::nullopt};
    return std::string(id);
}

Result<bool, EvidenceError> SetBool(WorldFacts& facts, const std::string& key, std::string_view cause) {
    const FactValue* current = facts.Find(key);
    if (current != nullptr && std::get_if<bool>(current) != nullptr && *std::get_if<bool>(current)) {
        return false; // already set: nothing is written
    }
    const auto applied = facts.Apply(SetFact{key, true}, cause);
    if (!applied) return EvidenceError{EvidenceErrorCode::Store, applied.Error()};
    return true;
}

} // namespace

Result<EvidenceCatalog, EvidenceCatalog::Error> EvidenceCatalog::Create(std::vector<EvidenceDef> defs) {
    if (defs.empty()) return Error::Empty;
    std::set<std::string> seen;
    for (const auto& def : defs) {
        if (!IsValidLocalId(def.id)) return Error::InvalidId;
        if (!IsValidLocalId(def.category)) return Error::InvalidCategory;
        if (!seen.insert(def.id).second) return Error::Duplicate;
    }
    return EvidenceCatalog(std::move(defs));
}

bool EvidenceCatalog::Contains(std::string_view id) const noexcept { return Find(id) != nullptr; }

const EvidenceDef* EvidenceCatalog::Find(std::string_view id) const noexcept {
    for (const auto& def : defs_) {
        if (def.id == id) return &def;
    }
    return nullptr;
}

std::string EvidenceKnownKey(std::string_view id) { return "placeholder.evidence." + std::string(id) + ".known"; }
std::string EvidenceFoundKey(std::string_view id) { return "placeholder.evidence." + std::string(id) + ".found"; }
std::string EvidenceJournalKey(std::string_view id) { return "placeholder.journal.evidence." + std::string(id); }

Result<EvidenceStage, EvidenceError> ReadEvidenceStage(const WorldFacts& facts, const EvidenceCatalog& catalog,
                                                       std::string_view id) {
    const auto checked = CheckId(catalog, id);
    if (!checked) return checked.Error();
    bool known = false, found = false, registered = false, corrupt = false;
    if (!ReadBool(facts, EvidenceKnownKey(id), known, corrupt) || !ReadBool(facts, EvidenceFoundKey(id), found, corrupt) ||
        !ReadBool(facts, EvidenceJournalKey(id), registered, corrupt)) {
        return EvidenceError{EvidenceErrorCode::StateCorrupt, std::nullopt};
    }
    if (registered) return EvidenceStage::Registered;
    if (found) return EvidenceStage::Found;
    if (known) return EvidenceStage::Known;
    return EvidenceStage::Unknown;
}

Result<bool, EvidenceError> LearnEvidence(WorldFacts& facts, const EvidenceCatalog& catalog, std::string_view id,
                                          std::string_view cause) {
    const auto checked = CheckId(catalog, id);
    if (!checked) return checked.Error();
    return SetBool(facts, EvidenceKnownKey(id), cause.empty() ? kCauseLearn : cause);
}

Result<bool, EvidenceError> FindEvidence(WorldFacts& facts, const EvidenceCatalog& catalog, std::string_view id,
                                         std::string_view cause) {
    const auto stage = ReadEvidenceStage(facts, catalog, id);
    if (!stage) return stage.Error();
    if (stage.Value() == EvidenceStage::Registered) return EvidenceError{EvidenceErrorCode::AlreadyRegistered, std::nullopt};
    return SetBool(facts, EvidenceFoundKey(id), cause.empty() ? kCauseFind : cause);
}

Result<bool, EvidenceError> CollectEvidence(WorldFacts& facts, const EvidenceCatalog& catalog, std::string_view id,
                                            std::string_view cause) {
    const auto stage = ReadEvidenceStage(facts, catalog, id);
    if (!stage) return stage.Error();
    if (stage.Value() == EvidenceStage::Registered) return EvidenceError{EvidenceErrorCode::AlreadyRegistered, std::nullopt};
    if (stage.Value() != EvidenceStage::Found) return EvidenceError{EvidenceErrorCode::NotFound, std::nullopt};
    return SetBool(facts, EvidenceJournalKey(id), cause.empty() ? kCauseCollect : cause);
}

Result<std::vector<std::string>, EvidenceError> RegisteredEvidence(const WorldFacts& facts, const EvidenceCatalog& catalog) {
    std::vector<std::string> out;
    for (const auto& def : catalog.Defs()) {
        const auto stage = ReadEvidenceStage(facts, catalog, def.id);
        if (!stage) return stage.Error();
        if (stage.Value() == EvidenceStage::Registered) out.push_back(def.id);
    }
    return out;
}

std::string_view ToString(EvidenceStage stage) noexcept {
    switch (stage) {
        case EvidenceStage::Unknown: return "unknown";
        case EvidenceStage::Known: return "known";
        case EvidenceStage::Found: return "found";
        case EvidenceStage::Registered: return "registered";
    }
    return "unknown";
}

std::string_view ToString(EvidenceErrorCode code) noexcept {
    switch (code) {
        case EvidenceErrorCode::InvalidId: return "invalid_id";
        case EvidenceErrorCode::UnknownEvidence: return "unknown_evidence";
        case EvidenceErrorCode::NotFound: return "not_found";
        case EvidenceErrorCode::AlreadyRegistered: return "already_registered";
        case EvidenceErrorCode::StateCorrupt: return "state_corrupt";
        case EvidenceErrorCode::Store: return "store";
    }
    return "unknown";
}

} // namespace spn::core
