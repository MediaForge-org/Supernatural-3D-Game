// Hand-run tool (Windows): kills a process while it is publishing saves and checks what the store can load afterwards (M2B3.1).
//
//   spn_core_save_kill_test run <directory> [iterations=300] [seed=1] [maxDelayMs=60]
//
// The parent starts a child of this same executable ("writer"), lets it save generation after generation for a random time,
// ends it with TerminateProcess and loads the slot. A child that is terminated at a random moment gets no chance to clean up, so this
// exercises the interrupted-write paths of the store with the real Windows file system. What it does NOT prove: power-loss safety.
// TerminateProcess leaves everything the process already handed to the operating system intact, so a flush that never reached the disk
// cannot be observed here.
//
// Invariants checked after every kill (P = last generation the child reported as completely saved):
//   * the store loads (source Current or Previous), never "no valid save", never a partial or damaged slot
//   * the loaded generation is never older than P or than a generation loaded in an earlier iteration (a completed save is not lost)
//   * the loaded slot is self-consistent (its payload is exactly what generation N writes)
// The directory must be empty or created for this test; the tool only writes slot files and its progress file.
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <random>
#include <spn/core/SaveStore.h>
#include <string>

#if !defined(_WIN32)
int main() {
    std::fprintf(stderr, "spn_core_save_kill_test is implemented for Windows only (CreateProcess/TerminateProcess)\n");
    return 2;
}
#else
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

using namespace spn::core;

namespace {

constexpr std::size_t kFillerBytes = 192 * 1024; // large enough that write, read-back and rename take a visible amount of time
const SaveSchema kSchema{{{"world.facts", 1, true}, {"player.inventory", 1, false}}};

std::string Filler(std::uint64_t generation, std::size_t bytes) {
    std::string s(bytes, 'a');
    for (std::size_t i = 0; i < bytes; ++i) s[i] = static_cast<char>('a' + (generation + i * 7) % 26);
    return s;
}

SaveSlot MakeGeneration(std::uint64_t n) {
    SaveSlot slot;
    (void)slot.Add(SaveSection{"world.facts", 1, true, "gen=" + std::to_string(n) + "\n" + Filler(n, kFillerBytes)});
    (void)slot.Add(SaveSection{"player.inventory", 1, false, "gen=" + std::to_string(n) + "\n" + Filler(n + 1, kFillerBytes / 2)});
    return slot;
}

// Returns the generation of a slot, or -1 if the slot is not exactly what MakeGeneration(n) writes.
long long GenerationOf(const SaveSlot& slot) {
    const SaveSection* facts = slot.Find("world.facts");
    if (facts == nullptr || facts->payload.rfind("gen=", 0) != 0) return -1;
    const std::size_t nl = facts->payload.find('\n');
    if (nl == std::string::npos) return -1;
    const long long n = std::atoll(facts->payload.substr(4, nl - 4).c_str());
    if (n < 0) return -1;
    return slot == MakeGeneration(static_cast<std::uint64_t>(n)) ? n : -1;
}

std::filesystem::path SlotPath(const std::filesystem::path& dir) { return dir / "slot1.sav"; }
std::filesystem::path ProgressPath(const std::filesystem::path& dir) { return dir / "progress.txt"; }

int RunWriter(const std::filesystem::path& dir, std::uint64_t first) {
    StdSaveFileSystem fs;
    SaveSlotStore store(fs, SlotPath(dir));
    for (std::uint64_t n = first;; ++n) {
        if (const auto e = store.Save(MakeGeneration(n), kSchema)) {
            std::fprintf(stderr, "writer: save %llu failed: %s\n", static_cast<unsigned long long>(n), e->detail.c_str());
            return 3;
        }
        std::ofstream(ProgressPath(dir), std::ios::trunc) << n << '\n'; // reported only after Save() returned
    }
}

// Last COMPLETE line of the append-only progress file (a kill can tear only the final line, which has no newline and is ignored).
long long ReadProgress(const std::filesystem::path& dir) {
    std::ifstream in(ProgressPath(dir), std::ios::binary);
    const std::string text((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    const std::size_t end = text.rfind('\n');
    if (end == std::string::npos) return -1;
    const std::size_t begin = text.rfind('\n', end == 0 ? 0 : end - 1);
    return std::atoll(text.substr(begin == std::string::npos ? 0 : begin + 1, end - (begin == std::string::npos ? 0 : begin + 1)).c_str());
}

std::wstring SelfPath() {
    std::wstring buffer(32768, L'\0');
    const DWORD n = ::GetModuleFileNameW(nullptr, buffer.data(), static_cast<DWORD>(buffer.size()));
    buffer.resize(n);
    return buffer;
}

} // namespace

int main(int argc, char** argv) {
    if (argc >= 4 && std::string(argv[1]) == "writer") {
        return RunWriter(argv[2], std::strtoull(argv[3], nullptr, 10));
    }
    if (argc < 3 || std::string(argv[1]) != "run") {
        std::fprintf(stderr, "usage: %s run <directory> [iterations] [seed] [maxDelayMs]\n", argv[0]);
        return 2;
    }
    const std::filesystem::path dir = argv[2];
    const int iterations = argc > 3 ? std::atoi(argv[3]) : 300;
    const unsigned seed = argc > 4 ? static_cast<unsigned>(std::atoi(argv[4])) : 1u;
    const int maxDelayMs = argc > 5 ? std::atoi(argv[5]) : 60;
    std::filesystem::create_directories(dir);

    StdSaveFileSystem fs;
    SaveSlotStore store(fs, SlotPath(dir));
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> delay(0, maxDelayMs);

    long long last = 0; // highest generation known to exist
    int ahead = 0, violations = 0, fromCurrent = 0, fromPrevious = 0, leftoverTemp = 0, killedBeforeFirstSave = 0, advanced = 0;
    const auto started = std::chrono::steady_clock::now();
    for (int i = 0; i < iterations; ++i) {
        const std::wstring exe = SelfPath();
        std::wstring cmd = L"\"" + exe + L"\" writer \"" + dir.wstring() + L"\" " + std::to_wstring(last + 1);
        STARTUPINFOW si{};
        si.cb = sizeof(si);
        PROCESS_INFORMATION pi{};
        if (!::CreateProcessW(nullptr, cmd.data(), nullptr, nullptr, FALSE, CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi)) {
            std::fprintf(stderr, "CreateProcess failed (%lu)\n", ::GetLastError());
            return 2;
        }
        ::Sleep(static_cast<DWORD>(delay(rng)));
        ::TerminateProcess(pi.hProcess, 99);
        ::WaitForSingleObject(pi.hProcess, 5000);
        ::CloseHandle(pi.hThread);
        ::CloseHandle(pi.hProcess);

        const long long reported = ReadProgress(dir);
        const bool temps = fs.Exists(store.Temporary()) || fs.Exists(SlotPath(dir).string() + ".prev.tmp");
        leftoverTemp += temps ? 1 : 0;
        const auto loaded = store.Load(kSchema);
        if (!loaded) {
            if (last == 0 && reported <= 0 && !fs.Exists(SlotPath(dir))) { // killed before the very first publication: nothing to load is correct
                ++killedBeforeFirstSave;
                continue;
            }
            std::printf("iteration %d: VIOLATION: nothing loadable after a kill (%s)\n", i, loaded.Error().detail.c_str());
            ++violations;
            continue;
        }
        const long long gen = GenerationOf(loaded.Value().slot);
        (loaded.Value().source == SaveSource::Current ? fromCurrent : fromPrevious) += 1;
        const long long expectedMin = std::max(reported, last); // never below a generation that was reported complete or loaded before
        if (gen < 0) {
            std::printf("iteration %d: VIOLATION: loaded slot is not a complete generation\n", i);
            ++violations;
        } else if (gen < expectedMin) {
            std::printf("iteration %d: VIOLATION: loaded generation %lld is older than %lld (reported complete or loaded earlier; source %s)\n", i,
                        gen, expectedMin, loaded.Value().source == SaveSource::Current ? "current" : "previous");
            ++violations;
        } else {
            if (gen > expectedMin + 1) ++ahead; // more than one save after the last reported one: the progress file lags (informational)
            advanced += gen > last ? 1 : 0;
            last = gen;
        }
    }
    const double seconds = std::chrono::duration<double>(std::chrono::steady_clock::now() - started).count();
    std::printf("directory:                 %s\n", dir.string().c_str());
    std::printf("volume:                    %s\n", DescribeFilesystem(dir).c_str());
    std::printf("kills:                     %d (max delay %d ms, seed %u, %.1f s)\n", iterations, maxDelayMs, seed, seconds);
    std::printf("highest generation:        %lld\n", last);
    std::printf("loaded from current/prev:  %d / %d\n", fromCurrent, fromPrevious);
    std::printf("killed before first save:  %d\n", killedBeforeFirstSave);
    std::printf("kills leaving temp files:  %d (the interrupted-write paths that were actually hit)\n", leftoverTemp);
    std::printf("violations:                %d\n", violations);
    std::printf("progress file lagging:     %d (informational: the child saved more than the progress file shows)\n", ahead);
    (void)advanced;
    return violations == 0 ? 0 : 1;
}
#endif
