#include <spn/core/ObjectPersistence.h>

#include <algorithm>
#include <charconv>
#include <fstream>
#include <iterator>
#include <spn/core/InteractionSystem.h>
#include <system_error>
#include <utility>
#include <variant>

namespace spn::core {

namespace {

constexpr std::string_view kMagic = "SPN-OBJECTS";
constexpr std::string_view kRestoreCause = "persist.restore";

bool IsLower(char c) { return c >= 'a' && c <= 'z'; }
bool IsDigit(char c) { return c >= '0' && c <= '9'; }

PersistError Err(PersistErrorCode code, std::size_t line = 0, std::string detail = {}) { return PersistError{code, line, std::move(detail)}; }

char TypeOf(const FactValue& v) { return std::holds_alternative<bool>(v) ? 'b' : std::holds_alternative<std::int64_t>(v) ? 'i' : 's'; }

std::string Encode(std::string_view text) {
    static constexpr char kHex[] = "0123456789ABCDEF";
    std::string out;
    for (const char ch : text) {
        const auto c = static_cast<unsigned char>(ch);
        if (IsLower(ch) || (ch >= 'A' && ch <= 'Z') || IsDigit(ch) || ch == '_' || ch == '.' || ch == '-') {
            out.push_back(ch);
        } else {
            out.push_back('%');
            out.push_back(kHex[c >> 4]);
            out.push_back(kHex[c & 15]);
        }
    }
    if (out.empty()) out = "%"; // an empty string needs a token: a lone '%' means empty
    return out;
}

std::optional<std::string> Decode(std::string_view token) {
    if (token == "%") return std::string();
    std::string out;
    const auto hex = [](char c) -> int { return IsDigit(c) ? c - '0' : (c >= 'A' && c <= 'F') ? c - 'A' + 10 : -1; };
    for (std::size_t i = 0; i < token.size(); ++i) {
        if (token[i] != '%') { out.push_back(token[i]); continue; }
        if (i + 2 >= token.size()) return std::nullopt;
        const int hi = hex(token[i + 1]), lo = hex(token[i + 2]);
        if (hi < 0 || lo < 0) return std::nullopt;
        out.push_back(static_cast<char>(hi * 16 + lo));
        i += 2;
    }
    return out;
}

std::vector<std::string_view> Split(std::string_view line) {
    std::vector<std::string_view> parts;
    std::size_t start = 0;
    while (start <= line.size()) {
        const std::size_t end = line.find(' ', start);
        parts.push_back(line.substr(start, end == std::string_view::npos ? std::string_view::npos : end - start));
        if (end == std::string_view::npos) break;
        start = end + 1;
    }
    return parts;
}

bool ParseU64(std::string_view token, std::uint64_t& out) {
    if (token.empty() || (token.size() > 1 && token.front() == '0')) return false;
    const auto r = std::from_chars(token.data(), token.data() + token.size(), out);
    return r.ec == std::errc() && r.ptr == token.data() + token.size();
}

bool ParseI64(std::string_view token, std::int64_t& out) {
    if (token.empty()) return false;
    const auto r = std::from_chars(token.data(), token.data() + token.size(), out);
    return r.ec == std::errc() && r.ptr == token.data() + token.size();
}

} // namespace

bool IsValidFieldName(std::string_view name) noexcept {
    if (name.empty() || name.size() > kMaxFieldNameLength || !IsLower(name.front())) return false;
    return std::all_of(name.begin(), name.end(), [](char c) { return IsLower(c) || IsDigit(c) || c == '_'; });
}

std::string_view ToString(PersistErrorCode code) noexcept {
    switch (code) {
    case PersistErrorCode::InvalidHeader: return "invalid header";
    case PersistErrorCode::UnsupportedVersion: return "unsupported version";
    case PersistErrorCode::TruncatedData: return "truncated data";
    case PersistErrorCode::TrailingData: return "trailing data";
    case PersistErrorCode::MalformedLine: return "malformed line";
    case PersistErrorCode::InvalidToken: return "invalid token";
    case PersistErrorCode::DuplicateObject: return "duplicate object";
    case PersistErrorCode::CountMismatch: return "count mismatch";
    case PersistErrorCode::InvalidId: return "invalid id";
    case PersistErrorCode::InvalidRegion: return "invalid region";
    case PersistErrorCode::UnknownKind: return "unknown kind";
    case PersistErrorCode::DuplicateId: return "duplicate id";
    case PersistErrorCode::InvalidDefaults: return "invalid defaults";
    case PersistErrorCode::Io: return "i/o error";
    case PersistErrorCode::VerifyFailed: return "verification failed";
    case PersistErrorCode::NoValidFile: return "no valid file";
    }
    return "unknown error";
}

// --- catalog -------------------------------------------------------------------------------------------------------------------------

std::optional<PersistError> WorldObjectCatalog::Add(ObjectConfig config) {
    if (!IsValidInteractableId(config.id)) return Err(PersistErrorCode::InvalidId, 0, config.id);
    if (!IsValidInteractableId(config.region)) return Err(PersistErrorCode::InvalidRegion, 0, config.region);
    if (config.kind.IsEmpty()) return Err(PersistErrorCode::UnknownKind, 0, config.id);
    if (objects_.contains(config.id)) return Err(PersistErrorCode::DuplicateId, 0, config.id);
    for (const auto& [name, value] : config.defaults) {
        const auto* text = std::get_if<std::string>(&value);
        if (!IsValidFieldName(name) || (text != nullptr && text->size() > kMaxFactStringLength)) return Err(PersistErrorCode::InvalidDefaults, 0, config.id + "." + name);
    }
    const std::string prefix = ObjectFactPrefix(config.kind, config.id);
    if (prefix.empty() || byPrefix_.contains(prefix)) return Err(PersistErrorCode::DuplicateId, 0, config.id);
    const std::string id = config.id;
    const std::string region = config.region;
    const auto inserted = objects_.emplace(id, std::move(config)).first;
    byPrefix_.emplace(prefix, &inserted->second);
    byRegion_[region].emplace(id, &inserted->second);
    return std::nullopt;
}

const ObjectConfig* WorldObjectCatalog::Find(std::string_view id) const {
    const auto found = objects_.find(id);
    return found == objects_.end() ? nullptr : &found->second;
}

std::vector<const ObjectConfig*> WorldObjectCatalog::InRegion(std::string_view region) const {
    std::vector<const ObjectConfig*> result;
    const auto found = byRegion_.find(region);
    if (found == byRegion_.end()) return result;
    for (const auto& [id, object] : found->second) result.push_back(object);
    return result;
}

const ObjectConfig* WorldObjectCatalog::OwnerOf(std::string_view factKey, std::string* field) const {
    // Fields have no dot, so the owner prefix ends at the LAST dot of the key.
    const std::size_t dot = factKey.rfind('.');
    if (dot == std::string_view::npos || dot + 1 >= factKey.size()) return nullptr;
    const std::string_view name = factKey.substr(dot + 1);
    if (!IsValidFieldName(name)) return nullptr;
    const auto found = byPrefix_.find(factKey.substr(0, dot + 1));
    if (found == byPrefix_.end()) return nullptr;
    if (field != nullptr) field->assign(name);
    return found->second;
}

std::string WorldObjectCatalog::KeyFor(const ObjectConfig& object, std::string_view field) const {
    return ObjectFactPrefix(object.kind, object.id).append(field);
}

// --- capture -------------------------------------------------------------------------------------------------------------------------

std::vector<ObjectRecord> CaptureChanged(const WorldFacts& facts, const WorldObjectCatalog& catalog, std::optional<std::string_view> region) {
    std::map<std::string, ObjectRecord, std::less<>> byId;
    for (const auto& [key, value] : facts.Facts()) {
        std::string field;
        const ObjectConfig* owner = catalog.OwnerOf(key, &field);
        if (owner == nullptr || (region && owner->region != *region)) continue;
        const auto def = owner->defaults.find(field);
        if (def != owner->defaults.end() && def->second == value) continue;
        ObjectRecord& record = byId[owner->id];
        if (record.id.empty()) {
            record.id = owner->id;
            record.kind = owner->kind;
            record.region = owner->region;
        }
        record.fields.emplace(std::move(field), value);
    }
    if (!byId.empty()) {
        for (const ChangeRecord& change : facts.Journal()) {
            const std::string_view key = std::visit([](const auto& c) -> std::string_view { return c.key; }, change.command);
            const ObjectConfig* owner = catalog.OwnerOf(key);
            if (owner == nullptr) continue;
            if (const auto it = byId.find(owner->id); it != byId.end()) ++it->second.revision;
        }
    }
    std::vector<ObjectRecord> result;
    result.reserve(byId.size());
    for (auto& [id, record] : byId) result.push_back(std::move(record));
    return result;
}

std::vector<std::string> DirtyObjects(const WorldFacts& facts, const WorldObjectCatalog& catalog, std::uint64_t sinceRevision) {
    std::set<std::string> dirty;
    const auto& journal = facts.Journal();
    for (std::size_t i = static_cast<std::size_t>(std::min<std::uint64_t>(sinceRevision, journal.size())); i < journal.size(); ++i) {
        const std::string_view key = std::visit([](const auto& c) -> std::string_view { return c.key; }, journal[i].command);
        if (const ObjectConfig* owner = catalog.OwnerOf(key)) dirty.insert(owner->id);
    }
    return {dirty.begin(), dirty.end()};
}

// --- format --------------------------------------------------------------------------------------------------------------------------

std::string SerializeRecords(const std::vector<ObjectRecord>& records) {
    std::vector<const ObjectRecord*> sorted;
    sorted.reserve(records.size());
    for (const ObjectRecord& r : records) sorted.push_back(&r);
    std::sort(sorted.begin(), sorted.end(), [](const ObjectRecord* a, const ObjectRecord* b) { return a->id < b->id; });
    std::string out;
    out.append(kMagic).append(" ").append(std::to_string(kObjectSaveFormatVersion)).append("\n");
    out.append("objects ").append(std::to_string(sorted.size())).append("\n");
    for (const ObjectRecord* r : sorted) {
        out.append("object ").append(r->id).append(" ").append(r->kind.Value()).append(" ").append(r->region).append(" ")
            .append(std::to_string(r->revision)).append(" ").append(std::to_string(r->fields.size())).append("\n");
        for (const auto& [name, value] : r->fields) {
            out.append("field ").append(name).append(" ").push_back(TypeOf(value));
            out.push_back(' ');
            if (const bool* b = std::get_if<bool>(&value)) out.append(*b ? "1" : "0");
            else if (const auto* i = std::get_if<std::int64_t>(&value)) out.append(std::to_string(*i));
            else out.append(Encode(std::get<std::string>(value)));
            out.push_back('\n');
        }
    }
    out.append("end\n");
    return out;
}

Result<std::vector<ObjectRecord>, PersistError> ParseRecords(std::string_view text) {
    std::vector<std::string_view> lines;
    std::size_t start = 0;
    while (start < text.size()) {
        const std::size_t end = text.find('\n', start);
        if (end == std::string_view::npos) return Err(PersistErrorCode::TruncatedData, lines.size() + 1, "missing final newline");
        lines.push_back(text.substr(start, end - start));
        start = end + 1;
    }
    if (lines.empty()) return Err(PersistErrorCode::InvalidHeader, 1);
    std::size_t at = 0;
    const auto header = Split(lines[at]);
    if (header.size() != 2 || header[0] != kMagic) return Err(PersistErrorCode::InvalidHeader, 1);
    std::uint64_t version = 0;
    if (!ParseU64(header[1], version)) return Err(PersistErrorCode::InvalidHeader, 1);
    if (version != kObjectSaveFormatVersion) return Err(PersistErrorCode::UnsupportedVersion, 1, std::to_string(version));
    ++at;
    if (at >= lines.size()) return Err(PersistErrorCode::TruncatedData, at + 1);
    const auto countLine = Split(lines[at]);
    std::uint64_t count = 0;
    if (countLine.size() != 2 || countLine[0] != "objects" || !ParseU64(countLine[1], count) || count > kMaxObjectRecords) return Err(PersistErrorCode::MalformedLine, at + 1);
    ++at;
    std::vector<ObjectRecord> records;
    std::set<std::string, std::less<>> seen;
    for (std::uint64_t n = 0; n < count; ++n, ++at) {
        if (at >= lines.size()) return Err(PersistErrorCode::TruncatedData, at + 1);
        const auto p = Split(lines[at]);
        if (p.size() != 6 || p[0] != "object") return Err(PersistErrorCode::MalformedLine, at + 1);
        ObjectRecord record;
        record.id = std::string(p[1]);
        if (!IsValidInteractableId(p[1])) return Err(PersistErrorCode::InvalidId, at + 1, record.id);
        const auto kind = ContentId::Parse(p[2]);
        if (!kind) return Err(PersistErrorCode::InvalidToken, at + 1, "kind");
        record.kind = *kind;
        if (!IsValidInteractableId(p[3])) return Err(PersistErrorCode::InvalidRegion, at + 1, std::string(p[3]));
        record.region = std::string(p[3]);
        std::uint64_t fieldCount = 0;
        if (!ParseU64(p[4], record.revision) || !ParseU64(p[5], fieldCount) || fieldCount > 100'000) return Err(PersistErrorCode::InvalidToken, at + 1, "number");
        if (!seen.insert(record.id).second) return Err(PersistErrorCode::DuplicateObject, at + 1, record.id);
        for (std::uint64_t f = 0; f < fieldCount; ++f) {
            ++at;
            if (at >= lines.size()) return Err(PersistErrorCode::TruncatedData, at + 1);
            const auto q = Split(lines[at]);
            if (q.size() != 4 || q[0] != "field" || q[2].size() != 1) return Err(PersistErrorCode::MalformedLine, at + 1);
            if (!IsValidFieldName(q[1])) return Err(PersistErrorCode::InvalidToken, at + 1, "field name");
            FactValue value;
            if (q[2][0] == 'b') {
                if (q[3] != "0" && q[3] != "1") return Err(PersistErrorCode::InvalidToken, at + 1, "bool");
                value = (q[3] == "1");
            } else if (q[2][0] == 'i') {
                std::int64_t i = 0;
                if (!ParseI64(q[3], i)) return Err(PersistErrorCode::InvalidToken, at + 1, "integer");
                value = i;
            } else if (q[2][0] == 's') {
                auto decoded = Decode(q[3]);
                if (!decoded || decoded->size() > kMaxFactStringLength) return Err(PersistErrorCode::InvalidToken, at + 1, "string");
                value = std::move(*decoded);
            } else {
                return Err(PersistErrorCode::InvalidToken, at + 1, "type");
            }
            if (!record.fields.emplace(std::string(q[1]), std::move(value)).second) return Err(PersistErrorCode::DuplicateObject, at + 1, record.id + "." + std::string(q[1]));
        }
        records.push_back(std::move(record));
    }
    if (at >= lines.size() || lines[at] != "end") return Err(at >= lines.size() ? PersistErrorCode::TruncatedData : PersistErrorCode::CountMismatch, at + 1);
    if (at + 1 != lines.size()) return Err(PersistErrorCode::TrailingData, at + 2);
    return records;
}

// --- restore -------------------------------------------------------------------------------------------------------------------------

RestoreReport ApplyRecords(WorldFacts& facts, const WorldObjectCatalog& catalog, const std::vector<ObjectRecord>& records, RestoreMode mode,
                           std::optional<std::string_view> scopeRegion, GameEventBus* bus) {
    RestoreReport report;
    if (mode == RestoreMode::Replace) {
        std::vector<std::string> keys;
        for (const auto& [key, value] : facts.Facts()) {
            const ObjectConfig* owner = catalog.OwnerOf(key);
            if (owner != nullptr && (!scopeRegion || owner->region == *scopeRegion)) keys.push_back(key);
        }
        for (const std::string& key : keys) (void)facts.Apply(RemoveFact{key}, "persist.reset");
    }
    for (const ObjectRecord& record : records) {
        const ObjectConfig* object = catalog.Find(record.id);
        if (object == nullptr) {
            report.missing.push_back(record);
            continue;
        }
        if (object->kind != record.kind) { report.rejected.push_back({record.id, "kind differs from the catalog"}); continue; }
        if (object->region != record.region) { report.rejected.push_back({record.id, "region differs from the catalog"}); continue; }
        std::vector<SetFact> writes;
        std::string problem;
        for (const auto& [name, value] : record.fields) {
            const auto def = object->defaults.find(name);
            if (def == object->defaults.end()) { report.unknownFields.push_back(record.id + "." + name); continue; }
            if (def->second.index() != value.index()) { problem = "field " + name + " has another type than the authored default"; break; }
            const std::string key = catalog.KeyFor(*object, name);
            if (!IsValidFactKey(key)) { problem = "key too long: " + key; break; }
            if (const auto* text = std::get_if<std::string>(&value); text != nullptr && text->size() > kMaxFactStringLength) { problem = "string too long: " + name; break; }
            if (const FactValue* current = facts.Find(key); current != nullptr && current->index() != value.index()) { problem = "live state of " + name + " has another type"; break; }
            writes.push_back(SetFact{key, value});
        }
        if (!problem.empty()) { report.rejected.push_back({record.id, std::move(problem)}); continue; }
        bool failed = false;
        for (const SetFact& w : writes) {
            const FactValue* current = facts.Find(w.key);
            if (current != nullptr && *current == w.value) continue;
            if (!facts.Apply(w, kRestoreCause)) { failed = true; break; }
        }
        if (failed) { report.rejected.push_back({record.id, "a write was refused by the fact store"}); continue; }
        report.restored.push_back(record.id);
    }
    if (bus != nullptr && !report.restored.empty()) {
        GameEvent e;
        e.type = GameEventType::PersistentStateRestored;
        e.count = report.restored.size();
        bus->Publish(std::move(e));
    }
    return report;
}

// --- regions -------------------------------------------------------------------------------------------------------------------------

std::size_t RegionStateCache::Unload(std::string_view region, WorldFacts& facts, const WorldObjectCatalog& catalog) {
    if (unloaded_.contains(region)) return 0;
    std::vector<ObjectRecord> records = CaptureChanged(facts, catalog, region);
    std::vector<std::string> keys;
    for (const auto& [key, value] : facts.Facts()) {
        const ObjectConfig* owner = catalog.OwnerOf(key);
        if (owner != nullptr && owner->region == region) keys.push_back(key);
    }
    for (const std::string& key : keys) (void)facts.Apply(RemoveFact{key}, "region.unload");
    const std::size_t count = records.size();
    retained_[std::string(region)] = std::move(records);
    unloaded_.emplace(region);
    return count;
}

RestoreReport RegionStateCache::Load(std::string_view region, WorldFacts& facts, const WorldObjectCatalog& catalog, GameEventBus* bus) {
    RestoreReport report;
    const auto it = unloaded_.find(region);
    if (it == unloaded_.end()) return report;
    const auto kept = retained_.find(region);
    if (kept != retained_.end()) {
        report = ApplyRecords(facts, catalog, kept->second, RestoreMode::Overlay, std::nullopt, bus);
        for (const ObjectRecord& r : report.missing) orphans_.insert_or_assign(r.id, r);
        for (const RejectedObject& rej : report.rejected) {
            for (const ObjectRecord& r : kept->second) if (r.id == rej.id) orphans_.insert_or_assign(r.id, r); // never drop state
        }
        retained_.erase(kept);
    }
    unloaded_.erase(it);
    return report;
}

std::vector<ObjectRecord> RegionStateCache::Snapshot(const WorldFacts& facts, const WorldObjectCatalog& catalog) const {
    std::map<std::string, ObjectRecord, std::less<>> byId;
    for (const auto& [id, record] : orphans_) byId.insert_or_assign(id, record);
    for (const auto& [region, records] : retained_) for (const ObjectRecord& r : records) byId.insert_or_assign(r.id, r);
    for (ObjectRecord& r : CaptureChanged(facts, catalog)) byId.insert_or_assign(r.id, std::move(r)); // live state wins
    std::vector<ObjectRecord> result;
    result.reserve(byId.size());
    for (auto& [id, record] : byId) result.push_back(std::move(record));
    return result;
}

void RegionStateCache::AddOrphans(std::vector<ObjectRecord> records) {
    for (ObjectRecord& r : records) orphans_.insert_or_assign(r.id, std::move(r));
}

std::size_t RegionStateCache::RetainedCount() const noexcept {
    std::size_t n = 0;
    for (const auto& [region, records] : retained_) n += records.size();
    return n;
}

// --- files ---------------------------------------------------------------------------------------------------------------------------

namespace {

std::optional<std::string> ReadAll(const std::filesystem::path& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) return std::nullopt;
    std::string data((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    if (in.bad()) return std::nullopt;
    return data;
}

} // namespace

std::optional<PersistError> WriteFileAtomic(const std::filesystem::path& path, std::string_view text) {
    namespace fs = std::filesystem;
    const fs::path tmp = path.string() + ".tmp";
    const fs::path bak = path.string() + ".bak";
    std::error_code ec;
    {
        std::ofstream out(tmp, std::ios::binary | std::ios::trunc);
        if (!out) return Err(PersistErrorCode::Io, 0, "cannot create " + tmp.string());
        out.write(text.data(), static_cast<std::streamsize>(text.size()));
        out.flush();
        if (!out) { out.close(); fs::remove(tmp, ec); return Err(PersistErrorCode::Io, 0, "write failed"); }
    }
    const auto back = ReadAll(tmp);
    if (!back || *back != text) { fs::remove(tmp, ec); return Err(PersistErrorCode::VerifyFailed, 0, "read-back differs"); }
    if (fs::exists(path, ec)) {
        // keep the last GOOD file as .bak; a corrupt main file must not replace a good backup
        const auto current = ReadAll(path);
        if (current && ParseRecords(*current)) {
            fs::copy_file(path, bak, fs::copy_options::overwrite_existing, ec);
            if (ec) { fs::remove(tmp, ec); return Err(PersistErrorCode::Io, 0, "cannot write backup"); }
        }
    }
    fs::rename(tmp, path, ec);
    if (ec) { std::error_code ignore; fs::remove(tmp, ignore); return Err(PersistErrorCode::Io, 0, "rename failed: " + ec.message()); }
    return std::nullopt;
}

Result<LoadedRecords, PersistError> LoadRecordsFile(const std::filesystem::path& path) {
    std::string mainDetail = "file missing";
    if (const auto text = ReadAll(path)) {
        auto parsed = ParseRecords(*text);
        if (parsed) return LoadedRecords{std::move(parsed).Value(), false};
        mainDetail = std::string(ToString(parsed.Error().code));
    }
    if (const auto text = ReadAll(path.string() + ".bak")) {
        auto parsed = ParseRecords(*text);
        if (parsed) return LoadedRecords{std::move(parsed).Value(), true};
    }
    return Err(PersistErrorCode::NoValidFile, 0, mainDetail);
}

} // namespace spn::core
