// Hand-run tool: checks the filesystem primitives the save protocol needs in a directory.
//   spn_core_save_probe <existing directory>
// Exit code 0 only if all behavioural checks passed. A pass is NOT a proof of power-loss safety (a flush cannot be observed).
#include <cstdio>
#include <filesystem>
#include <spn/core/SaveStore.h>

int main(int argc, char** argv) {
    if (argc != 2) {
        std::fprintf(stderr, "usage: %s <directory>\n", argv[0]);
        return 2;
    }
    spn::core::StdSaveFileSystem fs;
    const auto caps = spn::core::ProbePublishCapabilities(fs, argv[1]);
    std::printf("directory:                %s\n", argv[1]);
    std::printf("filesystem:               %s\n", caps.filesystem.empty() ? "(unknown)" : caps.filesystem.c_str());
    std::printf("rename replaces existing: %s\n", caps.renameReplacesExisting ? "yes" : "NO");
    std::printf("file flush reported ok:   %s\n", caps.fileSyncSucceeded ? "yes" : "NO");
    std::printf("directory flush ok:       %s\n", caps.directorySyncSucceeded ? "yes" : "NO");
    std::printf("notes:                    %s\n", caps.notes.c_str());
    return caps.AllChecksPassed() ? 0 : 1;
}
