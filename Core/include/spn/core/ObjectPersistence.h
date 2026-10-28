#pragma once

// Persistence of interactive world-object state: only CHANGED state is stored, per object, grouped by region (cell) so a streamed world can
// unload objects and keep what changed. Runtime state stays in WorldFacts (one source of truth); this layer
//   * knows which facts belong to which object (WorldObjectCatalog = authored configuration: id, kind, region, defaults),
//   * captures the difference to the authored defaults (CaptureChanged), tracks which objects changed (DirtyObjects),
//   * writes/reads a versioned, validated, deterministic text format ("SPN-OBJECTS 1"),
//   * restores records into the facts (ApplyRecords) and
//   * unloads/loads regions without losing changed state (RegionStateCache).
// Files are written with a temp file, read-back verification, a .bak of the last good file and an atomic rename.
//
// Format version 1:
//   SPN-OBJECTS 1
//   objects <N>
//   object <id> <kind> <region> <revision> <fieldCount>      (sorted by id)
//   field <name> <b|i|s> <value>                              (sorted by name; b: 0/1, i: integer, s: %XX-encoded)
//   end
// Compatibility: a file with a higher version is rejected as UnsupportedVersion (never half-read); fields a record carries that the
// catalog does not know are skipped on restore and reported, objects the catalog does not know are kept as orphans (RestoreReport::missing
// and RegionStateCache) and written back unchanged, so content that is temporarily absent (mods, older builds) loses no state.
//
// ENGINEERING FOUNDATION (M2A3), not master canon; the keys keep the placeholder. prefix of the existing test objects.

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <map>
#include <optional>
#include <set>
#include <spn/core/ContentId.h>
#include <spn/core/GameEvents.h>
#include <spn/core/Result.h>
#include <spn/core/WorldFacts.h>
#include <string>
#include <string_view>
#include <vector>

namespace spn::core {

inline constexpr std::uint64_t kObjectSaveFormatVersion = 1;
inline constexpr std::size_t kMaxFieldNameLength = 32;
inline constexpr std::size_t kMaxObjectRecords = 1'000'000; ///< parser limit against absurd counts, not a design target

/// [a-z][a-z0-9_]*, at most kMaxFieldNameLength (no dots: the dot separates object id and field in fact keys).
[[nodiscard]] bool IsValidFieldName(std::string_view name) noexcept;

enum class PersistErrorCode : std::uint8_t {
    InvalidHeader, UnsupportedVersion, TruncatedData, TrailingData, MalformedLine, InvalidToken, DuplicateObject, CountMismatch,
    InvalidId, InvalidRegion, UnknownKind, DuplicateId, InvalidDefaults, Io, VerifyFailed, NoValidFile,
};
[[nodiscard]] std::string_view ToString(PersistErrorCode code) noexcept;
struct PersistError {
    PersistErrorCode code;
    std::size_t line = 0;  ///< 1-based, 0 = n/a
    std::string detail;
    friend bool operator==(const PersistError&, const PersistError&) = default;
};

using FieldMap = std::map<std::string, FactValue, std::less<>>;

struct ObjectConfig {
    std::string id;       ///< IsValidInteractableId, unique in the catalog
    ContentId kind;
    std::string region;   ///< IsValidInteractableId grammar; the streaming cell the object lives in
    FieldMap defaults;    ///< authored value per field name; a field without a default counts as changed whenever a fact exists
};

class WorldObjectCatalog {
public:
    [[nodiscard]] std::optional<PersistError> Add(ObjectConfig config);
    [[nodiscard]] const ObjectConfig* Find(std::string_view id) const;
    [[nodiscard]] std::size_t Size() const noexcept { return objects_.size(); }
    [[nodiscard]] const std::map<std::string, ObjectConfig, std::less<>>& All() const noexcept { return objects_; }
    /// Objects of one region in id order.
    [[nodiscard]] std::vector<const ObjectConfig*> InRegion(std::string_view region) const;
    /// The object a fact key belongs to and the field name, or nullptr (key of no catalog object, or a field name with a dot).
    [[nodiscard]] const ObjectConfig* OwnerOf(std::string_view factKey, std::string* field = nullptr) const;
    [[nodiscard]] std::string KeyFor(const ObjectConfig& object, std::string_view field) const;

private:
    std::map<std::string, ObjectConfig, std::less<>> objects_;
    std::map<std::string, const ObjectConfig*, std::less<>> byPrefix_; // fact key prefix -> object (std::map nodes keep addresses stable)
    std::map<std::string, std::map<std::string, const ObjectConfig*, std::less<>>, std::less<>> byRegion_;
};

struct ObjectRecord {
    std::string id;
    ContentId kind;
    std::string region;
    std::uint64_t revision = 0;  ///< journal records that touched the object in the SAVING session. Informational (diagnostics, "was it touched"): a restored
                                 ///< session starts a new journal, so it is not carried over and not part of the state; compare `fields` for state equality
    FieldMap fields;             ///< only fields that differ from the authored defaults
    friend bool operator==(const ObjectRecord&, const ObjectRecord&) = default;
};

/// Records of all catalog objects (or of one region) whose facts differ from their authored defaults, ordered by id. Cost: proportional to
/// the objects in scope plus the journal length (revisions), never to unrelated facts.
[[nodiscard]] std::vector<ObjectRecord> CaptureChanged(const WorldFacts& facts, const WorldObjectCatalog& catalog, std::optional<std::string_view> region = std::nullopt);

/// Ids (sorted, unique) of catalog objects touched by journal records after `sinceRevision` (a previous facts.Revision()). The set to
/// re-save incrementally. Cost: proportional to the NEW journal records.
[[nodiscard]] std::vector<std::string> DirtyObjects(const WorldFacts& facts, const WorldObjectCatalog& catalog, std::uint64_t sinceRevision);

[[nodiscard]] std::string SerializeRecords(const std::vector<ObjectRecord>& records);
/// Validates everything (header, version, counts, grammar, duplicates). Never partially succeeds.
[[nodiscard]] Result<std::vector<ObjectRecord>, PersistError> ParseRecords(std::string_view text);

struct RejectedObject {
    std::string id;
    std::string reason;
};
struct RestoreReport {
    std::vector<std::string> restored;       ///< objects whose records were applied
    std::vector<ObjectRecord> missing;       ///< records of objects the catalog does not know (keep them: orphans)
    std::vector<RejectedObject> rejected;    ///< kind/region mismatch or invalid values; nothing of such an object was applied
    std::vector<std::string> unknownFields;  ///< "<id>.<field>" skipped: no authored default / type mismatch with the catalog
};
enum class RestoreMode : std::uint8_t {
    Overlay, ///< apply the records on top of the current facts
    Replace, ///< first remove the facts of every catalog object in scope (all, or the region of `scopeRegion`), then apply: exact restore
};
/// Applies records to the facts with cause "persist.restore". Every object is validated before any of its fields is written, so a rejected
/// object is untouched. Publishes PersistentStateRestored (count = restored objects) when a bus is given and something was restored.
RestoreReport ApplyRecords(WorldFacts& facts, const WorldObjectCatalog& catalog, const std::vector<ObjectRecord>& records, RestoreMode mode = RestoreMode::Overlay,
                           std::optional<std::string_view> scopeRegion = std::nullopt, GameEventBus* bus = nullptr);

/// Keeps changed state of unloaded regions. Unload removes the region's object facts from the live store (memory), Load restores them.
class RegionStateCache {
public:
    /// Captures the region's changed records, removes ALL facts of its catalog objects, marks the region unloaded. Unloading twice is a no-op.
    std::size_t Unload(std::string_view region, WorldFacts& facts, const WorldObjectCatalog& catalog);
    /// Applies the retained records (Overlay) and marks the region loaded. Loading a loaded region is a no-op with an empty report.
    RestoreReport Load(std::string_view region, WorldFacts& facts, const WorldObjectCatalog& catalog, GameEventBus* bus = nullptr);
    [[nodiscard]] bool IsLoaded(std::string_view region) const { return !unloaded_.contains(std::string(region)); }
    /// Records to save: changed state of loaded regions + retained state of unloaded regions + orphans. Ordered by id, deterministic.
    [[nodiscard]] std::vector<ObjectRecord> Snapshot(const WorldFacts& facts, const WorldObjectCatalog& catalog) const;
    void AddOrphans(std::vector<ObjectRecord> records);
    [[nodiscard]] std::size_t OrphanCount() const noexcept { return orphans_.size(); }
    [[nodiscard]] std::size_t RetainedCount() const noexcept;

private:
    std::set<std::string, std::less<>> unloaded_;
    std::map<std::string, std::vector<ObjectRecord>, std::less<>> retained_;
    std::map<std::string, ObjectRecord, std::less<>> orphans_;
};

/// Atomic replace: temp file + read-back check + .bak of the previous good file + rename. On failure the existing file is untouched
/// and the temp file removed. (No fsync in the standard library: durability against power loss needs a platform layer later.)
[[nodiscard]] std::optional<PersistError> WriteFileAtomic(const std::filesystem::path& path, std::string_view text);

struct LoadedRecords {
    std::vector<ObjectRecord> records;
    bool usedBackup = false; ///< the main file was missing or invalid and the .bak was used
};
/// Reads path; if it is missing or invalid, path + ".bak". Error NoValidFile (with the main file's error as detail) if neither works.
[[nodiscard]] Result<LoadedRecords, PersistError> LoadRecordsFile(const std::filesystem::path& path);

} // namespace spn::core
