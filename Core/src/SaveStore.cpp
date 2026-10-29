#include "spn/core/SaveStore.h"

#include <cstdio>
#include <fstream>
#include <iterator>
#include <system_error>

#if defined(_WIN32)
#include <io.h>
#else
#include <fcntl.h>
#include <unistd.h>
#endif
#if defined(__linux__)
#include <sys/vfs.h>
#endif

namespace spn::core {

namespace {

SaveError Io(std::string detail) { return SaveError{SaveErrorCode::Io, std::move(detail)}; }

bool IsNewerThanThisBuild(SaveErrorCode code) {
    return code == SaveErrorCode::UnsupportedContainerVersion || code == SaveErrorCode::UnsupportedSectionVersion ||
           code == SaveErrorCode::UnsupportedRequiredSection;
}

} // namespace

// --- StdSaveFileSystem ---

std::optional<SaveError> StdSaveFileSystem::WriteSynced(const std::filesystem::path& path, std::string_view bytes) {
    std::FILE* f = std::fopen(path.string().c_str(), "wb");
    if (f == nullptr) return Io("cannot create " + path.string());
    bool ok = bytes.empty() || std::fwrite(bytes.data(), 1, bytes.size(), f) == bytes.size();
    ok = ok && std::fflush(f) == 0;
#if defined(_WIN32)
    ok = ok && _commit(_fileno(f)) == 0;
#else
    ok = ok && ::fsync(::fileno(f)) == 0;
#endif
    const bool closed = std::fclose(f) == 0;
    if (!ok || !closed) return Io("write or flush failed: " + path.string());
    return std::nullopt;
}

Result<std::string, SaveError> StdSaveFileSystem::Read(const std::filesystem::path& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) return Io("cannot open " + path.string());
    std::string data((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    if (in.bad()) return Io("read failed: " + path.string());
    return data;
}

std::optional<SaveError> StdSaveFileSystem::Rename(const std::filesystem::path& from, const std::filesystem::path& to) {
    std::error_code ec;
    std::filesystem::rename(from, to, ec);
    if (ec) return Io("rename " + from.string() + " -> " + to.string() + ": " + ec.message());
    return std::nullopt;
}

std::optional<SaveError> StdSaveFileSystem::Remove(const std::filesystem::path& path) {
    std::error_code ec;
    std::filesystem::remove(path, ec);
    if (ec) return Io("remove " + path.string() + ": " + ec.message());
    return std::nullopt;
}

bool StdSaveFileSystem::Exists(const std::filesystem::path& path) {
    std::error_code ec;
    return std::filesystem::exists(path, ec);
}

std::optional<SaveError> StdSaveFileSystem::SyncDirectory(const std::filesystem::path& directory) {
#if defined(_WIN32)
    (void)directory; // no portable equivalent; MoveFileEx(WRITE_THROUGH) would be the platform route. Documented limitation, unverified.
    return std::nullopt;
#else
    const int fd = ::open(directory.string().c_str(), O_RDONLY | O_DIRECTORY);
    if (fd < 0) return Io("cannot open directory " + directory.string());
    const bool ok = ::fsync(fd) == 0;
    ::close(fd);
    if (!ok) return Io("directory sync failed: " + directory.string());
    return std::nullopt;
#endif
}

// --- SaveSlotStore ---

SaveSlotStore::SaveSlotStore(SaveFileSystem& fs, std::filesystem::path basePath)
    : fs_(&fs), current_(std::move(basePath)), previous_(current_.string() + ".prev"), temporary_(current_.string() + ".tmp"),
      previousTemporary_(current_.string() + ".prev.tmp") {}

std::optional<SaveError> SaveSlotStore::Save(const SaveSlot& slot, const SaveSchema& schema) {
    if (const auto e = slot.Validate(schema)) return *e;
    const std::string text = slot.Serialize();

    // 1. complete, flushed, verified temporary file
    if (const auto e = fs_->Remove(temporary_)) return *e;
    if (const auto e = fs_->WriteSynced(temporary_, text)) {
        (void)fs_->Remove(temporary_);
        return *e;
    }
    const auto back = fs_->Read(temporary_);
    if (!back || back.Value() != text) {
        (void)fs_->Remove(temporary_);
        return SaveError{SaveErrorCode::VerifyFailed, "read-back of " + temporary_.string() + " differs"};
    }

    // 2. keep the previous GOOD generation
    if (fs_->Exists(current_)) {
        const auto old = fs_->Read(current_);
        if (old && SaveSlot::Parse(old.Value()).HasValue()) {
            if (const auto e = fs_->WriteSynced(previousTemporary_, old.Value())) {
                (void)fs_->Remove(temporary_);
                (void)fs_->Remove(previousTemporary_);
                return *e;
            }
            if (const auto e = fs_->Rename(previousTemporary_, previous_)) {
                (void)fs_->Remove(temporary_);
                (void)fs_->Remove(previousTemporary_);
                return *e;
            }
        }
    }

    // 3. publish
    if (const auto e = fs_->Rename(temporary_, current_)) {
        (void)fs_->Remove(temporary_);
        return *e;
    }
    if (const auto e = fs_->SyncDirectory(current_.parent_path().empty() ? std::filesystem::path(".") : current_.parent_path())) {
        return SaveError{SaveErrorCode::Io, "not durable: saved, but the directory flush failed (" + e->detail + ")"};
    }
    return std::nullopt;
}

Result<LoadedSlot, SaveError> SaveSlotStore::Load(const SaveSchema& schema) {
    std::optional<SaveError> currentProblem;
    if (fs_->Exists(current_)) {
        const auto bytes = fs_->Read(current_);
        if (!bytes) {
            currentProblem = bytes.Error();
        } else {
            auto parsed = SaveSlot::Parse(bytes.Value());
            if (parsed) {
                if (const auto e = parsed.Value().Validate(schema)) currentProblem = *e;
                else return LoadedSlot{std::move(parsed).Value(), SaveSource::Current, std::nullopt};
            } else {
                currentProblem = parsed.Error();
            }
        }
        if (currentProblem && IsNewerThanThisBuild(currentProblem->code)) return *currentProblem; // intact but newer: never roll back silently
    } else {
        currentProblem = SaveError{SaveErrorCode::Io, "no save file"};
    }

    if (fs_->Exists(previous_)) {
        const auto bytes = fs_->Read(previous_);
        if (bytes) {
            auto parsed = SaveSlot::Parse(bytes.Value());
            if (parsed && !parsed.Value().Validate(schema)) return LoadedSlot{std::move(parsed).Value(), SaveSource::Previous, currentProblem};
        }
    }
    return SaveError{SaveErrorCode::NoValidSave, currentProblem ? std::string(ToString(currentProblem->code)) + ": " + currentProblem->detail : "unknown"};
}

// --- probes ---

std::string DescribeFilesystem(const std::filesystem::path& path) {
#if defined(__linux__)
    struct statfs s {};
    if (::statfs(path.string().c_str(), &s) != 0) return {};
    char buf[64];
    std::snprintf(buf, sizeof(buf), "statfs type 0x%lx", static_cast<unsigned long>(s.f_type));
    switch (static_cast<unsigned long>(s.f_type)) {
    case 0x65735546ul: return std::string(buf) + " (FUSE, e.g. NTFS-3G: durability depends on the user-space driver)";
    case 0x01021994ul: return std::string(buf) + " (tmpfs: memory only, no durability)";
    case 0xEF53ul: return std::string(buf) + " (ext2/3/4)";
    case 0x58465342ul: return std::string(buf) + " (xfs)";
    case 0x9123683Eul: return std::string(buf) + " (btrfs)";
    default: return buf;
    }
#else
    (void)path;
    return {};
#endif
}

PublishCapabilities ProbePublishCapabilities(SaveFileSystem& fs, const std::filesystem::path& directory) {
    PublishCapabilities caps;
    caps.filesystem = DescribeFilesystem(directory);
    const auto a = directory / "spn_probe_a.tmp";
    const auto b = directory / "spn_probe_b.dat";
    (void)fs.Remove(a);
    (void)fs.Remove(b);
    if (fs.WriteSynced(b, "old") || fs.WriteSynced(a, "new")) {
        caps.notes = "cannot create probe files";
    } else {
        caps.fileSyncSucceeded = true;
        if (!fs.Rename(a, b)) {
            const auto now = fs.Read(b);
            caps.renameReplacesExisting = now && now.Value() == "new" && !fs.Exists(a);
        }
        caps.directorySyncSucceeded = !fs.SyncDirectory(directory).has_value();
    }
    (void)fs.Remove(a);
    (void)fs.Remove(b);
    if (!caps.AllChecksPassed() && caps.notes.empty()) caps.notes = "at least one primitive behaved differently than the save protocol needs";
    if (caps.AllChecksPassed()) caps.notes = "primitives behaved as required; this cannot prove power-loss safety (a flush cannot be observed)";
    return caps;
}

} // namespace spn::core
