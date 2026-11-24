#include "common.h"

#include <cstdlib>
#include <cstring>

namespace spn
{
bool WriteFile(const std::string& path, const void* data, size_t bytes)
{
    std::FILE* f = nullptr;
#ifdef _MSC_VER
    fopen_s(&f, path.c_str(), "wb");
#else
    f = std::fopen(path.c_str(), "wb");
#endif
    if (f == nullptr) { std::fprintf(stderr, "cannot write %s\n", path.c_str()); return false; }
    const bool ok = std::fwrite(data, 1, bytes, f) == bytes;
    std::fclose(f);
    return ok;
}

bool WriteTga(const std::string& path, int w, int h, int channels, const std::vector<uint8_t>& data)
{
    std::vector<uint8_t> out(18 + static_cast<size_t>(w) * h * channels);
    out[2] = channels == 1 ? 3 : 2;
    out[12] = static_cast<uint8_t>(w & 255); out[13] = static_cast<uint8_t>(w >> 8);
    out[14] = static_cast<uint8_t>(h & 255); out[15] = static_cast<uint8_t>(h >> 8);
    out[16] = static_cast<uint8_t>(channels * 8);
    out[17] = static_cast<uint8_t>(0x20 | (channels == 4 ? 8 : 0));   // top-left origin, 8 alpha bits
    uint8_t* dst = out.data() + 18;
    const size_t pixels = static_cast<size_t>(w) * h;
    for (size_t i = 0; i < pixels; ++i)
    {
        const uint8_t* s = &data[i * channels];
        if (channels == 1) { dst[i] = s[0]; continue; }
        dst[i * channels] = s[2]; dst[i * channels + 1] = s[1]; dst[i * channels + 2] = s[0];
        if (channels == 4) { dst[i * 4 + 3] = s[3]; }
    }
    return WriteFile(path, out.data(), out.size());
}
}   // namespace spn

int main(int argc, char** argv)
{
    if (argc >= 3 && std::strcmp(argv[1], "textures") == 0)
    {
        return spn::RunTextures(argv[2], argc >= 4 ? std::atoi(argv[3]) : 2048);
    }
    if (argc >= 4 && std::strcmp(argv[1], "terrain") == 0)
    {
        return spn::RunTerrain(argv[2], argv[3]);
    }
    std::printf("usage:\n  spn_worldtool textures <out_dir> [size]\n  spn_worldtool terrain <spec.txt> <out_dir>\n");
    return 1;
}
