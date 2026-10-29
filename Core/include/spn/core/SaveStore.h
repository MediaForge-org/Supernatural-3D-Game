#pragma once

// Publishing a SaveSlot to storage without ever leaving a playable save half-written (M2B2).
//
// Protocol of SaveSlotStore::Save (files next to each other: <base>, <base>.prev, <base>.tmp, <base>.prev.tmp):
//   1. serialize, write <base>.tmp, flush it to stable storage, read it back and compare            (a failure leaves every other file untouched)
//   2. if <base> exists and parses: copy it to <base>.prev.tmp, flush, rename over <base>.prev       (the previous GOOD generation; a corrupt
//      <base> never replaces a good .prev)
//   3. rename <base>.tmp over <base>, then flush the directory                                       (the one publishing step)
// At every point in time at least one of <base> and <base>.prev is a complete save once any save has succeeded. Load tries <base>, then
// <base>.prev. What the protocol guarantees is relative to the filesystem: rename-over-existing must be atomic and flushes must reach stable
// storage. ProbePublishCapabilities checks the behaviour the protocol needs on the actual volume; it cannot prove power-loss safety.
// A leftover <base>.tmp is never loaded (it may be incomplete or superseded); the next save overwrites it.

#include <filesystem>
#include <optional>
#include <string>
#include <string_view>

#include "spn/core/Result.h"
#include "spn/core/SaveSlot.h"

namespace spn::core {

/// The few filesystem operations the protocol needs, behind an interface so that tests can inject faults at every step.
class SaveFileSystem {
public:
    virtual ~SaveFileSystem() = default;
    /// Creates or truncates `path`, writes all bytes and flushes them to stable storage before returning success.
    [[nodiscard]] virtual std::optional<SaveError> WriteSynced(const std::filesystem::path& path, std::string_view bytes) = 0;
    [[nodiscard]] virtual Result<std::string, SaveError> Read(const std::filesystem::path& path) = 0;
    /// Moves `from` to `to`, replacing `to` if it exists, atomically.
    [[nodiscard]] virtual std::optional<SaveError> Rename(const std::filesystem::path& from, const std::filesystem::path& to) = 0;
    /// A missing file is not an error.
    [[nodiscard]] virtual std::optional<SaveError> Remove(const std::filesystem::path& path) = 0;
    [[nodiscard]] virtual bool Exists(const std::filesystem::path& path) = 0;
    /// Flushes the directory entry changes (renames) to stable storage. May be a documented no-op where the platform has no equivalent.
    [[nodiscard]] virtual std::optional<SaveError> SyncDirectory(const std::filesystem::path& directory) = 0;
};

/// The real filesystem (std::filesystem + FILE*; fsync / _commit; directory fsync on POSIX). Windows behaviour is written but UNVERIFIED.
class StdSaveFileSystem final : public SaveFileSystem {
public:
    [[nodiscard]] std::optional<SaveError> WriteSynced(const std::filesystem::path& path, std::string_view bytes) override;
    [[nodiscard]] Result<std::string, SaveError> Read(const std::filesystem::path& path) override;
    [[nodiscard]] std::optional<SaveError> Rename(const std::filesystem::path& from, const std::filesystem::path& to) override;
    [[nodiscard]] std::optional<SaveError> Remove(const std::filesystem::path& path) override;
    [[nodiscard]] bool Exists(const std::filesystem::path& path) override;
    [[nodiscard]] std::optional<SaveError> SyncDirectory(const std::filesystem::path& directory) override;
};

enum class SaveSource : std::uint8_t { Current, Previous };

struct LoadedSlot {
    SaveSlot slot;
    SaveSource source = SaveSource::Current;
    std::optional<SaveError> currentProblem; ///< why the main file was not used when source == Previous
};

class SaveSlotStore {
public:
    SaveSlotStore(SaveFileSystem& fs, std::filesystem::path basePath);

    /// Validates the slot against `schema` (a slot without its required sections is not published), then runs the protocol above. On any error
    /// the main file still holds the last good save. Error Io with "not durable" in the detail: the new save is visible but the directory
    /// flush failed.
    [[nodiscard]] std::optional<SaveError> Save(const SaveSlot& slot, const SaveSchema& schema);

    /// Main file first, then the previous generation. A file that is intact but newer than this build understands (unsupported container
    /// or section version, unknown required section) is NOT skipped in favour of an older save: that would silently roll the player back.
    [[nodiscard]] Result<LoadedSlot, SaveError> Load(const SaveSchema& schema);

    [[nodiscard]] const std::filesystem::path& Current() const noexcept { return current_; }
    [[nodiscard]] const std::filesystem::path& Previous() const noexcept { return previous_; }
    [[nodiscard]] const std::filesystem::path& Temporary() const noexcept { return temporary_; }

private:
    SaveFileSystem* fs_;
    std::filesystem::path current_, previous_, temporary_, previousTemporary_;
};

struct PublishCapabilities {
    bool renameReplacesExisting = false; ///< rename onto an existing file replaced it and the source is gone
    bool fileSyncSucceeded = false;      ///< WriteSynced reported success (the flush itself cannot be observed)
    bool directorySyncSucceeded = false;
    std::string filesystem;              ///< best-effort description of the volume (Linux: statfs magic); empty if unknown
    std::string notes;                   ///< human-readable caveats
    /// True only if every behavioural check passed. NOT a proof of crash safety.
    [[nodiscard]] bool AllChecksPassed() const noexcept { return renameReplacesExisting && fileSyncSucceeded && directorySyncSucceeded; }
};
/// Runs the protocol's primitive operations once in `directory` (it must exist and be writable) and removes its probe files again.
[[nodiscard]] PublishCapabilities ProbePublishCapabilities(SaveFileSystem& fs, const std::filesystem::path& directory);
/// Best-effort filesystem name of the volume holding `path` (empty if the platform has no query).
[[nodiscard]] std::string DescribeFilesystem(const std::filesystem::path& path);

} // namespace spn::core
