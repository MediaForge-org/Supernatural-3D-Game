#pragma once

// In-memory SaveFileSystem with deterministic fault injection (tests only). Every mutating operation (WriteSynced, Rename, Remove,
// SyncDirectory) is counted; a FaultPlan breaks the Nth one in one of several ways.

#include <map>
#include <spn/core/SaveStore.h>
#include <string>

namespace spn::testdata {

enum class FaultMode {
    None,
    Error,            ///< the operation fails and has no effect
    ErrorAfterEffect, ///< the operation takes effect but reports an error (the classic "did it happen?" case)
    Crash,            ///< power loss BEFORE the operation: it is not performed and the process is gone (all later operations fail)
    TornCrash,        ///< WriteSynced persists only the first half of the bytes, then power loss; other operations behave like Crash
};

struct FaultPlan {
    FaultMode mode = FaultMode::None;
    std::size_t atOperation = 0; ///< 1-based index among mutating operations
};

class MemoryFs : public spn::core::SaveFileSystem {
public:
    std::map<std::string, std::string> files; ///< what is on "disk"
    FaultPlan plan;
    std::size_t operations = 0; ///< mutating operations seen so far
    bool dead = false;          ///< simulated power loss happened; Revive() = reboot

    void Revive() {
        dead = false;
        plan = {};
    }

    std::optional<spn::core::SaveError> WriteSynced(const std::filesystem::path& path, std::string_view bytes) override {
        if (const auto f = Enter()) {
            if (*f == FaultMode::TornCrash) {
                files[path.string()] = std::string(bytes.substr(0, bytes.size() / 2));
                dead = true;
            }
            if (*f == FaultMode::Crash) dead = true;
            return Fail("write");
        }
        files[path.string()] = std::string(bytes);
        return Finish();
    }
    spn::core::Result<std::string, spn::core::SaveError> Read(const std::filesystem::path& path) override {
        if (dead) return Fail("read after crash").value();
        const auto it = files.find(path.string());
        if (it == files.end()) return spn::core::SaveError{spn::core::SaveErrorCode::Io, "missing " + path.string()};
        return it->second;
    }
    std::optional<spn::core::SaveError> Rename(const std::filesystem::path& from, const std::filesystem::path& to) override {
        if (const auto f = Enter()) {
            if (*f == FaultMode::Crash || *f == FaultMode::TornCrash) dead = true;
            return Fail("rename");
        }
        const auto it = files.find(from.string());
        if (it == files.end()) return spn::core::SaveError{spn::core::SaveErrorCode::Io, "rename: missing source"};
        files[to.string()] = std::move(it->second);
        files.erase(from.string());
        return Finish();
    }
    std::optional<spn::core::SaveError> Remove(const std::filesystem::path& path) override {
        if (const auto f = Enter()) {
            if (*f == FaultMode::Crash || *f == FaultMode::TornCrash) dead = true;
            return Fail("remove");
        }
        files.erase(path.string());
        return Finish();
    }
    bool Exists(const std::filesystem::path& path) override { return !dead && files.contains(path.string()); }
    std::optional<spn::core::SaveError> SyncDirectory(const std::filesystem::path&) override {
        if (const auto f = Enter()) {
            if (*f == FaultMode::Crash || *f == FaultMode::TornCrash) dead = true;
            return Fail("syncdir");
        }
        return Finish();
    }

private:
    // Counts the operation. Returns the fault to apply BEFORE the effect, or nullopt to perform it normally.
    std::optional<FaultMode> Enter() {
        if (dead) return FaultMode::Error;
        ++operations;
        pendingAfterEffect_ = false;
        if (plan.mode != FaultMode::None && operations == plan.atOperation) {
            if (plan.mode == FaultMode::ErrorAfterEffect) {
                pendingAfterEffect_ = true;
                return std::nullopt;
            }
            return plan.mode;
        }
        return std::nullopt;
    }
    std::optional<spn::core::SaveError> Finish() {
        if (pendingAfterEffect_) {
            pendingAfterEffect_ = false;
            return Fail("after effect");
        }
        return std::nullopt;
    }
    static std::optional<spn::core::SaveError> Fail(const char* what) {
        return spn::core::SaveError{spn::core::SaveErrorCode::Io, std::string("injected fault: ") + what};
    }
    bool pendingAfterEffect_ = false;
};

} // namespace spn::testdata
