// Procedural, tiling PBR texture sets (base colour, normal, AO/roughness/metallic + height) for the placeholder world.
// Every pattern here is computed from noise functions; no photographs or third-party textures are used.
#include "common.h"

namespace spn
{
namespace
{
struct Px { Color C; float H = 0.5f; float Rough = 0.8f; float Metal = 0.f; float Alpha = 1.f; };
using Shader = std::function<Px(float, float)>;

struct Surface
{
    int N = 0;
    std::vector<Px> P;
    Px& At(int x, int y) { return P[static_cast<size_t>(Wrap(y, N)) * N + Wrap(x, N)]; }
    const Px& At(int x, int y) const { return P[static_cast<size_t>(Wrap(y, N)) * N + Wrap(x, N)]; }
};

Surface Render(int n, const Shader& shader)
{
    Surface s; s.N = n; s.P.resize(static_cast<size_t>(n) * n);
    ParallelRows(n, [&](int y) { for (int x = 0; x < n; ++x) { s.P[static_cast<size_t>(y) * n + x] = shader((x + 0.5f) / n, (y + 0.5f) / n); } });
    return s;
}

/** Paints a soft line (tiling) into the surface: colour blended by opacity, height added. */
void Stroke(Surface& s, float u0, float v0, float u1, float v1, float widthPx, const Color& c, float opacity, float hAdd, bool wrap = true)
{
    const float len = std::sqrt((u1 - u0) * (u1 - u0) + (v1 - v0) * (v1 - v0)) * s.N;
    const int steps = std::max(1, static_cast<int>(len * 1.5f));
    const float r = std::max(0.5f, widthPx * 0.5f);
    const int ir = static_cast<int>(std::ceil(r));
    for (int i = 0; i <= steps; ++i)
    {
        const float t = static_cast<float>(i) / steps;
        const float px = Lerp(u0, u1, t) * s.N, py = Lerp(v0, v1, t) * s.N;
        for (int dy = -ir; dy <= ir; ++dy)
        {
            for (int dx = -ir; dx <= ir; ++dx)
            {
                const int x = static_cast<int>(px) + dx, y = static_cast<int>(py) + dy;
                if (!wrap && (x < 0 || y < 0 || x >= s.N || y >= s.N)) { continue; }
                const float d = std::sqrt((x + 0.5f - px) * (x + 0.5f - px) + (y + 0.5f - py) * (y + 0.5f - py));
                const float a = Clamp01(r + 0.5f - d) * opacity;
                if (a <= 0.f) { continue; }
                Px& p = s.At(x, y);
                p.C = Mix(p.C, c, a); p.H += hAdd * a; p.Alpha = std::max(p.Alpha, a);
            }
        }
    }
}

uint8_t To8(float v) { return static_cast<uint8_t>(Clamp01(v) * 255.f + 0.5f); }

void Save(const Surface& s, const std::string& outDir, const std::string& name, float normalStrength, float aoStrength, bool alpha = false)
{
    const int n = s.N;
    std::vector<uint8_t> col(static_cast<size_t>(n) * n * (alpha ? 4 : 3)), nrm(static_cast<size_t>(n) * n * 3), orm(static_cast<size_t>(n) * n * 4);
    // blurred height for the cavity term
    const int radius = std::max(2, n / 160);
    std::vector<float> tmp(static_cast<size_t>(n) * n), blur(static_cast<size_t>(n) * n);
    ParallelRows(n, [&](int y) { for (int x = 0; x < n; ++x) { float a = 0.f; for (int k = -radius; k <= radius; ++k) { a += s.At(x + k, y).H; } tmp[static_cast<size_t>(y) * n + x] = a / (2 * radius + 1); } });
    ParallelRows(n, [&](int y) { for (int x = 0; x < n; ++x) { float a = 0.f; for (int k = -radius; k <= radius; ++k) { a += tmp[static_cast<size_t>(Wrap(y + k, n)) * n + x]; } blur[static_cast<size_t>(y) * n + x] = a / (2 * radius + 1); } });
    const float slope = normalStrength * n / 1024.f;
    ParallelRows(n, [&](int y)
    {
        for (int x = 0; x < n; ++x)
        {
            const size_t i = static_cast<size_t>(y) * n + x;
            const Px& p = s.P[i];
            const float hx = (s.At(x + 1, y).H - s.At(x - 1, y).H) * 0.5f * slope, hy = (s.At(x, y + 1).H - s.At(x, y - 1).H) * 0.5f * slope;
            const float inv = 1.f / std::sqrt(hx * hx + hy * hy + 1.f);
            nrm[i * 3] = To8(0.5f - 0.5f * hx * inv); nrm[i * 3 + 1] = To8(0.5f - 0.5f * hy * inv); nrm[i * 3 + 2] = To8(0.5f + 0.5f * inv);
            const float ao = Clamp01(1.f - aoStrength * std::max(0.f, blur[i] - p.H));
            orm[i * 4] = To8(ao); orm[i * 4 + 1] = To8(p.Rough); orm[i * 4 + 2] = To8(p.Metal); orm[i * 4 + 3] = To8(p.H);
            const size_t c = i * (alpha ? 4 : 3);
            col[c] = To8(p.C.R); col[c + 1] = To8(p.C.G); col[c + 2] = To8(p.C.B);
            if (alpha) { col[c + 3] = To8(p.Alpha); }
        }
    });
    WriteTga(outDir + "/T_" + name + "_C.tga", n, n, alpha ? 4 : 3, col);
    WriteTga(outDir + "/T_" + name + "_N.tga", n, n, 3, nrm);
    WriteTga(outDir + "/T_" + name + "_ORM.tga", n, n, 4, orm);
    std::printf("  %s\n", name.c_str());
}

Color Gray(float v, float warm = 0.f) { return {v * (1.f + warm), v, v * (1.f - warm)}; }

// ---------------------------------------------------------------- recipes

Px Asphalt(float u, float v, bool old)
{
    const uint32_t seed = old ? 300u : 100u;
    const WorleyResult stones = Worley(u, v, 150, seed + 1u);
    const float stone = Smooth(0.5f, 0.12f, stones.F1);
    const float tone = static_cast<float>(stones.Id & 1023u) / 1023.f;
    const float macro = Fbm(u, v, 3, 4, seed + 2u), fine = Fbm(u, v, 256, 2, seed + 3u);
    float a = (old ? 0.27f : 0.17f) + (macro - 0.5f) * 0.07f + stone * (tone * 0.20f - 0.05f) + (fine - 0.5f) * 0.05f;
    float h = 0.45f + stone * 0.22f + (fine - 0.5f) * 0.12f;
    float rough = 0.86f - stone * tone * 0.12f;
    const float wu = u + (Fbm(u, v, 6, 3, seed + 4u) - 0.5f) * 0.10f, wv = v + (Fbm(u, v, 6, 3, seed + 5u) - 0.5f) * 0.10f;
    const WorleyResult cells = Worley(wu, wv, old ? 7 : 4, seed + 6u);
    const float region = Fbm(u, v, 2, 3, seed + 7u);
    const float crack = Smooth(0.030f, 0.004f, cells.F2 - cells.F1) * Smooth(old ? 0.46f : 0.52f, old ? 0.58f : 0.62f, region);
    if (old)
    {
        // repair patches: some cells of a coarse pattern are darker, fresher asphalt with a sealed seam
        const WorleyResult patch = Worley(u + 0.31f, v + 0.17f, 3, seed + 8u, 0.6f);
        if ((patch.Id & 7u) == 0u)
        {
            a = Lerp(a, a * 0.72f, 0.85f);
            const float seam = Smooth(0.02f, 0.0f, patch.F2 - patch.F1);
            a *= 1.f - 0.4f * seam; rough -= 0.25f * seam;
        }
        const WorleyResult fineCracks = Worley(wu * 1.f + 0.5f, wv, 19, seed + 9u);
        const float hair = Smooth(0.022f, 0.004f, fineCracks.F2 - fineCracks.F1) * Smooth(0.56f, 0.68f, Fbm(u, v, 4, 2, seed + 10u));
        a *= 1.f - 0.35f * hair; h -= 0.2f * hair;
    }
    a *= 1.f - 0.6f * crack; h -= 0.45f * crack; rough = Lerp(rough, 0.95f, crack);
    Px p; p.C = {a * 0.99f, a, a * 1.03f}; p.H = h; p.Rough = rough;
    return p;
}

Px Concrete(float u, float v, bool slabs)
{
    const float stain = Fbm(u, v, 3, 5, 410u), grain = Fbm(u, v, 300, 2, 411u), mid = Fbm(u, v, 24, 3, 412u);
    float a = 0.55f + (stain - 0.5f) * 0.14f + (grain - 0.5f) * 0.06f + (mid - 0.5f) * 0.05f;
    float h = 0.6f + (grain - 0.5f) * 0.10f + (mid - 0.5f) * 0.05f;
    const WorleyResult pore = Worley(u, v, 140, 413u);
    if ((pore.Id & 3u) == 0u) { const float d = Smooth(0.16f, 0.04f, pore.F1); a *= 1.f - 0.35f * d; h -= 0.3f * d; }
    float rough = 0.82f + (mid - 0.5f) * 0.1f;
    if (slabs)
    {
        const float du = std::fabs(Fract(u * 2.f + 0.5f) - 0.5f) * 0.5f, dv = std::fabs(Fract(v * 2.f + 0.5f) - 0.5f) * 0.5f;
        const float d = std::min(du, dv);
        const uint32_t slab = Hash(static_cast<uint32_t>(u * 2.f), static_cast<uint32_t>(v * 2.f), 414u);
        a += ((slab & 255u) / 255.f - 0.5f) * 0.07f;
        h += std::sin(v * 2.f * Pi * 220.f) * 0.025f;                       // broom finish
        a *= 1.f - 0.16f * Smooth(0.05f, 0.f, d) * Fbm(u, v, 12, 3, 415u);  // dirt gathers along the joints
        const float joint = Smooth(0.0045f, 0.0015f, d);
        a *= 1.f - 0.55f * joint; h -= 0.5f * joint; rough = Lerp(rough, 0.95f, joint);
        const WorleyResult cells = Worley(u + (stain - 0.5f) * 0.2f, v, 3, 416u);
        const float crack = Smooth(0.012f, 0.002f, cells.F2 - cells.F1) * Smooth(0.55f, 0.7f, Fbm(u, v, 2, 2, 417u));
        a *= 1.f - 0.5f * crack; h -= 0.35f * crack;
    }
    Px p; p.C = {a * 1.02f, a, a * 0.95f}; p.H = h; p.Rough = rough;
    return p;
}

Px Plaster(float u, float v)
{
    const float bump = Fbm(u, v, 40, 4, 500u), fine = Fbm(u, v, 200, 2, 501u), macro = Fbm(u, v, 3, 4, 502u);
    const WorleyResult blobs = Worley(u, v, 60, 503u);
    const float h = 0.4f + bump * 0.35f + Smooth(0.6f, 0.1f, blobs.F1) * 0.2f + (fine - 0.5f) * 0.1f;
    const float a = 0.80f + (macro - 0.5f) * 0.06f + (bump - 0.5f) * 0.05f;
    Px p; p.C = Gray(a, 0.01f); p.H = h; p.Rough = 0.9f;
    return p;
}

Px Siding(float u, float v)
{
    constexpr int rows = 16;
    const float fr = v * rows; const int row = static_cast<int>(fr); const float t = fr - row;
    const float grain = Fbm2(u + row * 0.37f, v, 6, 256, 4, 600u + static_cast<uint32_t>(row));
    float h = 0.2f + 0.62f * t + (grain - 0.5f) * 0.05f;
    h *= 1.f - Smooth(0.975f, 1.f, t) * 0.15f;
    float a = 0.80f + (Fbm(u, v, 5, 4, 601u) - 0.5f) * 0.06f + (grain - 0.5f) * 0.03f;
    Color c = Gray(a, 0.005f);
    float rough = 0.62f;
    // paint flaking near the lower edge of boards, more in some regions
    const float chips = Smooth(0.60f, 0.70f, Fbm2(u, v, 40, 80, 3, 602u) * 0.65f + 0.35f * Fbm(u, v, 4, 2, 603u)) * Smooth(0.35f, 1.f, t);
    c = Mix(c, Mix(Color{0.40f, 0.35f, 0.30f}, Color{0.52f, 0.47f, 0.41f}, grain), chips * 0.9f);
    h -= chips * 0.04f; rough = Lerp(rough, 0.88f, chips);
    // dirt under each lap
    c = Scale(c, 1.f - 0.10f * Smooth(0.25f, 0.f, t));
    // board ends
    for (int j = 0; j < 1; ++j)
    {
        if ((row % 3) == 1) { break; }
        const float uj = Rand01(static_cast<uint32_t>(row), static_cast<uint32_t>(j), 604u);
        float d = std::fabs(u - uj); d = std::min(d, 1.f - d);
        const float joint = Smooth(0.0009f, 0.0003f, d);
        c = Scale(c, 1.f - 0.35f * joint); h -= 0.12f * joint;
    }
    Px p; p.C = c; p.H = h; p.Rough = rough;
    return p;
}

Px Planks(float u, float v, int rows, bool varnished)
{
    const float fr = v * rows; const int row = static_cast<int>(fr); const float t = fr - row;
    const uint32_t seed = (varnished ? 760u : 700u) + static_cast<uint32_t>(row) * 7u;
    const float off = Rand01(static_cast<uint32_t>(row), 3u, seed);
    const float warp = Fbm2(u + off, v, 3, rows, 3, seed + 1u);
    const float grain = Fbm2(u + off, v + warp * 0.012f, 5, rows * 24, 4, seed + 2u);
    const float rings = 0.5f + 0.5f * std::sin((warp * 9.f + t * 2.2f) * 2.f * Pi);
    const float tone = Rand01(static_cast<uint32_t>(row), 9u, seed) - 0.5f;
    Color c = varnished ? Mix(Color{0.33f, 0.19f, 0.09f}, Color{0.58f, 0.38f, 0.19f}, Clamp01(grain * 0.7f + rings * 0.3f + tone * 0.5f))
                        : Mix(Color{0.27f, 0.21f, 0.15f}, Color{0.50f, 0.43f, 0.35f}, Clamp01(grain * 0.7f + rings * 0.3f + tone * 0.6f));
    float h = 0.6f + (grain - 0.5f) * (varnished ? 0.05f : 0.22f);
    float rough = varnished ? 0.38f + grain * 0.12f : 0.78f + (grain - 0.5f) * 0.1f;
    const float gapWidth = varnished ? 0.025f : 0.06f;
    const float gap = Smooth(gapWidth, gapWidth * 0.3f, std::min(t, 1.f - t));
    c = Scale(c, 1.f - 0.75f * gap); h -= 0.55f * gap;
    // board ends
    const int ends = varnished ? 3 : 1;
    for (int j = 0; j < ends; ++j)
    {
        const float uj = Rand01(static_cast<uint32_t>(row), static_cast<uint32_t>(j) + 20u, seed);
        float d = std::fabs(u - uj); d = std::min(d, 1.f - d);
        const float joint = Smooth(0.0018f, 0.0005f, d);
        c = Scale(c, 1.f - 0.7f * joint); h -= 0.4f * joint;
    }
    if (!varnished)
    {
        const WorleyResult knot = Worley(u, v, rows / 2, seed + 5u);
        if ((knot.Id & 7u) == 0u) { const float k = Smooth(0.16f, 0.04f, knot.F1); c = Mix(c, Color{0.16f, 0.11f, 0.07f}, k * 0.8f); }
        const float wear = Fbm(u, v, 5, 4, 705u);
        c = Mix(c, Gray(0.46f, 0.02f), Smooth(0.5f, 0.8f, wear) * 0.45f);   // silvering of weathered wood
    }
    Px p; p.C = c; p.H = h; p.Rough = rough;
    return p;
}

Px Brick(float u, float v)
{
    constexpr int cols = 8, rows = 24;
    const float fr = v * rows; const int row = static_cast<int>(fr); const float ly = fr - row;
    const float bx = u * cols + ((row & 1) != 0 ? 0.5f : 0.f); const int col = Wrap(static_cast<int>(std::floor(bx)), cols); const float lx = bx - std::floor(bx);
    const float mx = 0.024f, my = 0.075f;
    const float edge = std::min(std::min(lx, 1.f - lx) / mx, std::min(ly, 1.f - ly) / my);
    const float wobble = (Fbm(u, v, 64, 2, 800u) - 0.5f) * 0.6f;
    const float brick = Smooth(0.8f, 1.5f, edge + wobble);
    const uint32_t id = Hash(static_cast<uint32_t>(col), static_cast<uint32_t>(row), 801u);
    const float tone = (id & 255u) / 255.f, burn = ((id >> 8) & 15u) == 0u ? 0.62f : 1.f;
    const float surf = Fbm(u, v, 120, 3, 802u), stain = Fbm(u, v, 4, 4, 803u);
    Color bc = Scale(Mix(Color{0.42f, 0.19f, 0.14f}, Color{0.58f, 0.30f, 0.21f}, tone), burn * (0.85f + surf * 0.3f));
    bc = Mix(bc, Color{0.62f, 0.58f, 0.52f}, Smooth(0.62f, 0.8f, stain) * 0.25f);   // efflorescence
    const float mg = Fbm(u, v, 200, 2, 804u);
    const Color mc = Gray(0.50f + mg * 0.14f, 0.02f);
    Px p; p.C = Mix(mc, bc, brick); p.H = Lerp(0.25f + mg * 0.1f, 0.7f + surf * 0.15f, brick); p.Rough = Lerp(0.93f, 0.84f, brick);
    return p;
}

Px Shingles(float u, float v)
{
    constexpr int rows = 16, tabs = 8;
    const float fr = v * rows; const int row = static_cast<int>(fr); const float t = fr - row;
    const float off = ((row & 1) != 0 ? 0.5f : 0.f) + (Rand01(static_cast<uint32_t>(row), 1u, 900u) - 0.5f) * 0.12f;
    const float bx = u * tabs + off; const float lx = bx - std::floor(bx);
    const uint32_t id = Hash(static_cast<uint32_t>(Wrap(static_cast<int>(std::floor(bx)), tabs)), static_cast<uint32_t>(row), 901u);
    const float tone = 0.82f + ((id & 255u) / 255.f) * 0.32f;
    const float gran = Fbm(u, v, 400, 2, 902u), macro = Fbm(u, v, 3, 4, 903u);
    float a = (0.17f + (gran - 0.5f) * 0.10f) * tone * (0.85f + macro * 0.3f);
    float h = 0.25f + 0.5f * t + (gran - 0.5f) * 0.08f + ((id >> 8) & 255u) / 255.f * 0.06f;
    const float gap = Smooth(0.022f, 0.008f, std::min(lx, 1.f - lx)) * Smooth(0.0f, 0.1f, t);
    a *= 1.f - 0.6f * gap; h -= 0.3f * gap;
    a *= 1.f - 0.25f * Smooth(0.12f, 0.f, t);                    // shadow line under the lap above
    Color c = {a * 1.03f, a, a * 0.97f};
    const float moss = Smooth(0.62f, 0.78f, Fbm(u, v, 5, 4, 904u)) * Smooth(0.3f, 0.6f, Fbm(u, v, 60, 2, 905u));
    c = Mix(c, Color{0.16f, 0.21f, 0.09f}, moss * 0.7f); h += moss * 0.08f;
    Px p; p.C = c; p.H = h; p.Rough = 0.92f;
    return p;
}

Px Corrugated(float u, float v)
{
    const float wave = 0.5f + 0.5f * std::sin(u * 20.f * 2.f * Pi);
    const float streak = Fbm2(u, v, 48, 3, 4, 1000u), blot = Fbm(u, v, 5, 5, 1001u), fine = Fbm(u, v, 160, 2, 1002u);
    const float rust = Smooth(0.60f, 0.78f, blot * 0.6f + streak * 0.4f + (1.f - wave) * 0.08f) * 0.85f;
    const float spangle = static_cast<float>(Worley(u, v, 70, 1003u).Id & 255u) / 255.f;
    const Color zinc = Gray(0.50f + spangle * 0.10f + (fine - 0.5f) * 0.04f, -0.015f);
    const Color rc = Mix(Color{0.36f, 0.17f, 0.07f}, Color{0.24f, 0.10f, 0.05f}, fine);
    Px p; p.C = Mix(zinc, rc, rust); p.H = wave * 0.85f + rust * fine * 0.1f; p.Rough = Lerp(0.38f + spangle * 0.1f, 0.9f, rust); p.Metal = 1.f - rust;
    return p;
}

Px PaintedMetal(float u, float v)
{
    const float peel = Fbm(u, v, 180, 2, 1100u), macro = Fbm(u, v, 4, 4, 1101u);
    const WorleyResult chip = Worley(u, v, 22, 1102u);
    const float rust = Smooth(0.20f, 0.08f, chip.F1) * (((chip.Id & 15u) < 2u) ? 1.f : 0.f) * Smooth(0.52f, 0.68f, macro + (peel - 0.5f) * 0.4f);
    const float streak = Smooth(0.55f, 0.8f, Fbm2(u, v, 60, 2, 3, 1103u)) * 0.12f;
    Color c = Gray(0.72f + (macro - 0.5f) * 0.06f - streak);
    c = Mix(c, Color{0.30f, 0.14f, 0.06f}, rust);
    Px p; p.C = c; p.H = 0.5f + (peel - 0.5f) * 0.05f - rust * 0.1f; p.Rough = Lerp(0.42f + macro * 0.1f, 0.88f, rust); p.Metal = 0.f;
    return p;
}

Px Steel(float u, float v)
{
    const float brushed = Fbm2(u, v, 4, 400, 3, 1150u), macro = Fbm(u, v, 5, 4, 1151u);
    Px p; p.C = Gray(0.52f + (brushed - 0.5f) * 0.08f + (macro - 0.5f) * 0.06f); p.H = 0.5f + (brushed - 0.5f) * 0.05f; p.Rough = 0.36f + macro * 0.2f; p.Metal = 1.f;
    return p;
}

Px Gravel(float u, float v)
{
    const WorleyResult big = Worley(u, v, 46, 1200u), small = Worley(u, v, 120, 1201u);
    const float sb = Smooth(0.55f, 0.15f, big.F1), ss = Smooth(0.55f, 0.15f, small.F1);
    const float tb = (big.Id & 255u) / 255.f, ts = (small.Id & 255u) / 255.f;
    const float soil = Fbm(u, v, 80, 3, 1202u), macro = Fbm(u, v, 3, 4, 1203u);
    Color c = Mix(Color{0.25f, 0.21f, 0.17f}, Color{0.34f, 0.29f, 0.23f}, soil);
    c = Mix(c, Mix(Color{0.38f, 0.37f, 0.35f}, Color{0.60f, 0.56f, 0.50f}, ts), ss * 0.9f);
    const bool hasBig = (big.Id >> 9 & 3u) != 0u;
    if (hasBig) { c = Mix(c, Mix(Color{0.33f, 0.32f, 0.31f}, Color{0.63f, 0.60f, 0.55f}, tb), sb); }
    c = Scale(c, 0.88f + macro * 0.24f);
    Px p; p.C = c; p.H = 0.2f + soil * 0.1f + ss * 0.3f + (hasBig ? sb * 0.4f : 0.f); p.Rough = 0.9f;
    return p;
}

Px Dirt(float u, float v)
{
    const float macro = Fbm(u, v, 3, 5, 1300u), mid = Fbm(u, v, 20, 4, 1301u), fine = Fbm(u, v, 220, 2, 1302u);
    Color c = Mix(Color{0.21f, 0.16f, 0.11f}, Color{0.37f, 0.29f, 0.20f}, Clamp01(macro * 0.5f + mid * 0.5f));
    c = Scale(c, 0.9f + (fine - 0.5f) * 0.3f);
    float h = 0.4f + mid * 0.3f + (fine - 0.5f) * 0.15f;
    const WorleyResult peb = Worley(u, v, 60, 1303u);
    if ((peb.Id & 15u) == 0u)
    {
        const float s = Smooth(0.34f, 0.12f, peb.F1);
        c = Mix(c, Gray(0.27f + ((peb.Id >> 4) & 255u) / 255.f * 0.14f, 0.04f), s * 0.85f); h += s * 0.25f;
    }
    const WorleyResult clod = Worley(u + mid * 0.05f, v, 14, 1304u);
    const float crack = Smooth(0.05f, 0.01f, clod.F2 - clod.F1) * Smooth(0.5f, 0.7f, macro);
    c = Scale(c, 1.f - 0.3f * crack); h -= 0.15f * crack;
    Px p; p.C = c; p.H = h; p.Rough = 0.93f;
    return p;
}

Px Rock(float u, float v)
{
    const float warp = Fbm(u, v, 4, 4, 1400u);
    const float strata = 0.5f + 0.5f * std::sin((v * 18.f + warp * 5.f) * 2.f * Pi);
    const float ridge = Ridged(u, v, 6, 5, 1401u), fine = Fbm(u, v, 150, 3, 1402u), macro = Fbm(u, v, 2, 4, 1403u);
    const WorleyResult cells = Worley(u + warp * 0.22f, v + ridge * 0.12f, 5, 1404u);
    const float crack = Smooth(0.03f, 0.004f, cells.F2 - cells.F1) * Smooth(0.45f, 0.65f, Fbm(u, v, 3, 3, 1406u));
    Color c = Mix(Color{0.24f, 0.23f, 0.22f}, Color{0.47f, 0.44f, 0.40f}, Clamp01(ridge * 0.65f + strata * 0.12f + macro * 0.3f - 0.05f));
    c = Scale(c, (0.8f + fine * 0.4f) * (1.f - 0.5f * crack));
    const float lichen = Smooth(0.66f, 0.8f, Fbm(u, v, 9, 4, 1405u));
    c = Mix(c, Color{0.36f, 0.40f, 0.22f}, lichen * 0.45f);
    Px p; p.C = c; p.H = 0.3f + ridge * 0.45f + strata * 0.08f + (fine - 0.5f) * 0.1f - crack * 0.3f; p.Rough = 0.88f;
    return p;
}

Px ForestBase(float u, float v)
{
    const float macro = Fbm(u, v, 3, 5, 1500u), mid = Fbm(u, v, 30, 4, 1501u), fine = Fbm(u, v, 200, 2, 1502u);
    Color c = Mix(Color{0.11f, 0.08f, 0.055f}, Color{0.24f, 0.17f, 0.10f}, Clamp01(macro * 0.45f + mid * 0.55f));
    c = Scale(c, 0.85f + fine * 0.3f);
    Px p; p.C = c; p.H = 0.35f + mid * 0.2f + (fine - 0.5f) * 0.1f; p.Rough = 0.95f;
    return p;
}

Px GrassBase(float u, float v)
{
    const float macro = Fbm(u, v, 4, 5, 1600u), mid = Fbm(u, v, 40, 3, 1601u), fine = Fbm(u, v, 260, 2, 1602u);
    Color c = Mix(Color{0.13f, 0.17f, 0.06f}, Color{0.25f, 0.29f, 0.11f}, Clamp01(macro * 0.5f + mid * 0.5f));
    c = Mix(c, Color{0.26f, 0.21f, 0.13f}, Smooth(0.62f, 0.85f, Fbm(u, v, 9, 4, 1603u)) * 0.5f);   // soil showing through
    c = Scale(c, 0.85f + fine * 0.3f);
    Px p; p.C = c; p.H = 0.4f + mid * 0.2f + (fine - 0.5f) * 0.2f; p.Rough = 0.9f;
    return p;
}

Px WallPaint(float u, float v)
{
    const float roller = Fbm(u, v, 220, 3, 1700u), macro = Fbm(u, v, 3, 4, 1701u);
    Px p; p.C = Gray(0.82f + (macro - 0.5f) * 0.04f + (roller - 0.5f) * 0.02f, 0.005f); p.H = 0.5f + (roller - 0.5f) * 0.12f; p.Rough = 0.78f;
    return p;
}

Px Wallpaper(float u, float v)
{
    const float stripe = Smooth(0.20f, 0.24f, std::fabs(Fract(u * 12.f) - 0.5f));
    const float fibre = Fbm2(u, v, 300, 40, 2, 1750u), macro = Fbm(u, v, 3, 4, 1751u), stain = Smooth(0.6f, 0.85f, Fbm(u, v, 5, 4, 1752u));
    float a = Lerp(0.66f, 0.76f, stripe) + (fibre - 0.5f) * 0.04f + (macro - 0.5f) * 0.05f;
    // small diamond ornament in the light stripes
    const float ou = Fract(u * 12.f + 0.5f) - 0.5f, ov = Fract(v * 16.f) - 0.5f;
    a -= Smooth(0.10f, 0.07f, std::fabs(ou) * 2.2f + std::fabs(ov)) * 0.06f * stripe;
    Color c = Gray(a, 0.01f);
    c = Mix(c, Color{0.55f, 0.47f, 0.33f}, stain * 0.3f);
    Px p; p.C = c; p.H = 0.5f + (fibre - 0.5f) * 0.06f; p.Rough = 0.85f;
    return p;
}

Px Carpet(float u, float v)
{
    const float fibre = Fbm(u, v, 400, 2, 1800u), tuft = static_cast<float>(Worley(u, v, 260, 1801u).Id & 255u) / 255.f, macro = Fbm(u, v, 4, 4, 1802u);
    const float worn = Smooth(0.55f, 0.8f, Fbm(u, v, 3, 3, 1803u));
    float a = 0.52f + (tuft - 0.5f) * 0.16f + (fibre - 0.5f) * 0.08f + (macro - 0.5f) * 0.06f;
    a = Lerp(a, a * 0.82f, worn * 0.6f);
    Px p; p.C = Gray(a); p.H = 0.3f + tuft * 0.5f; p.Rough = 0.97f;
    return p;
}

Px Linoleum(float u, float v)
{
    constexpr int n = 8;
    const float fx = u * n, fy = v * n; const int ix = static_cast<int>(fx), iy = static_cast<int>(fy); const float lx = fx - ix, ly = fy - iy;
    const bool dark = ((ix + iy) & 1) != 0;
    const float speck = Fbm(u, v, 300, 2, 1900u), macro = Fbm(u, v, 4, 4, 1901u), scuff = Smooth(0.6f, 0.8f, Fbm2(u, v, 30, 90, 3, 1902u));
    Color c = dark ? Color{0.20f, 0.27f, 0.24f} : Color{0.70f, 0.67f, 0.58f};
    c = Scale(c, 0.9f + (speck - 0.5f) * 0.25f + (macro - 0.5f) * 0.12f);
    c = Mix(c, Gray(0.25f, 0.03f), scuff * 0.18f);
    const float grout = Smooth(0.012f, 0.004f, std::min(std::min(lx, 1.f - lx), std::min(ly, 1.f - ly)));
    c = Scale(c, 1.f - 0.6f * grout);
    Px p; p.C = c; p.H = 0.6f - grout * 0.4f + (speck - 0.5f) * 0.02f; p.Rough = Lerp(0.32f + macro * 0.15f + scuff * 0.2f, 0.9f, grout);
    return p;
}

Px Ceiling(float u, float v)
{
    const WorleyResult bumps = Worley(u, v, 110, 2000u);
    const float fine = Fbm(u, v, 240, 2, 2001u), macro = Fbm(u, v, 3, 4, 2002u);
    const float stain = Smooth(0.68f, 0.85f, Fbm(u, v, 4, 4, 2003u));
    Color c = Gray(0.84f + (macro - 0.5f) * 0.04f, 0.01f);
    c = Mix(c, Color{0.62f, 0.52f, 0.36f}, stain * 0.35f);
    Px p; p.C = c; p.H = 0.3f + Smooth(0.6f, 0.1f, bumps.F1) * 0.5f + (fine - 0.5f) * 0.15f; p.Rough = 0.95f;
    return p;
}

Px Grunge(float u, float v)
{
    // R: vertical leak streaks, G: blotchy dirt, B: fine wear (paint erosion of road markings, scratches)
    const float streak = Smooth(0.45f, 0.85f, Fbm2(u, v, 64, 3, 4, 2100u) * 0.7f + Fbm(u, v, 6, 3, 2101u) * 0.3f);
    const float blotch = Smooth(0.35f, 0.75f, Fbm(u, v, 5, 6, 2102u));
    const float wear = Smooth(0.38f, 0.62f, Fbm(u, v, 14, 5, 2103u) * 0.6f + Fbm(u, v, 120, 2, 2104u) * 0.4f);
    Px p; p.C = {streak, blotch, wear}; p.H = 0.5f; p.Rough = 1.f;
    return p;
}

Px Blotch(float u, float v)
{
    // soft-edged stain for decals: alpha fades to the border, RGB dark
    const float du = u - 0.5f, dv = v - 0.5f;
    const float r = std::sqrt(du * du + dv * dv) * 2.f;
    const float n = Fbm(u, v, 5, 5, 2200u, 0.5f, false), fine = Fbm(u, v, 40, 3, 2201u, 0.5f, false);
    const float a = Smooth(0.95f, 0.25f, r + (n - 0.5f) * 0.9f) * (0.55f + fine * 0.45f);
    Px p; p.C = Gray(0.06f + fine * 0.05f, 0.05f); p.Alpha = a; p.H = 0.5f; p.Rough = 0.35f;
    return p;
}

Px CrackDecal(float u, float v)
{
    const float du = u - 0.5f, dv = v - 0.5f;
    const float fade = Smooth(1.0f, 0.5f, std::sqrt(du * du + dv * dv) * 2.f);
    const float wu = u + (Fbm(u, v, 5, 3, 2300u, 0.5f, false) - 0.5f) * 0.25f, wv = v + (Fbm(u, v, 5, 3, 2301u, 0.5f, false) - 0.5f) * 0.25f;
    const WorleyResult cells = Worley(wu, wv, 4, 2302u);
    const float crack = Smooth(0.035f, 0.006f, cells.F2 - cells.F1);
    Px p; p.C = Gray(0.05f); p.Alpha = crack * fade; p.H = 0.5f - crack * 0.5f; p.Rough = 0.95f;
    return p;
}

void ForestFloor(const std::string& outDir, int n)
{
    Surface s = Render(n, ForestBase);
    uint32_t r = 0;
    auto rnd = [&r]() { return Rand01(r++, 17u, 1550u); };
    const int needles = n * n / 90;
    for (int i = 0; i < needles; ++i)
    {
        const float u = rnd(), v = rnd(), a = rnd() * 2.f * Pi, len = (0.012f + rnd() * 0.016f) * 1024.f / n * (n / 1024.f);
        const float t = rnd();
        const Color c = Mix(Color{0.21f, 0.15f, 0.09f}, Color{0.36f, 0.28f, 0.17f}, t);
        Stroke(s, u, v, u + std::cos(a) * len, v + std::sin(a) * len, 1.3f * n / 1024.f, Scale(c, 0.7f + rnd() * 0.45f), 0.8f, 0.10f);
    }
    for (int i = 0; i < n * n / 9000; ++i)   // twigs
    {
        float u = rnd(), v = rnd(), a = rnd() * 2.f * Pi;
        const Color c = Scale(Color{0.20f, 0.15f, 0.11f}, 0.7f + rnd() * 0.6f);
        for (int k = 0; k < 3; ++k)
        {
            const float len = 0.02f + rnd() * 0.03f, nu = u + std::cos(a) * len, nv = v + std::sin(a) * len;
            Stroke(s, u, v, nu, nv, 2.6f * n / 1024.f, c, 0.95f, 0.25f);
            u = nu; v = nv; a += (rnd() - 0.5f) * 0.9f;
        }
    }
    for (int i = 0; i < n * n / 9000; ++i)   // fallen leaves
    {
        const float u = rnd(), v = rnd(), a = rnd() * 2.f * Pi, len = 0.008f + rnd() * 0.010f;
        const float t = rnd();
        const Color c = t < 0.5f ? Mix(Color{0.26f, 0.17f, 0.08f}, Color{0.37f, 0.28f, 0.13f}, t * 2.f) : Mix(Color{0.22f, 0.14f, 0.08f}, Color{0.17f, 0.19f, 0.09f}, t * 2.f - 1.f);
        Stroke(s, u - std::cos(a) * len, v - std::sin(a) * len, u + std::cos(a) * len, v + std::sin(a) * len, 7.f * n / 1024.f, c, 0.9f, 0.12f);
    }
    for (Px& p : s.P) { p.Alpha = 1.f; }
    Save(s, outDir, "ForestFloor", 5.f, 2.2f);
}

void GrassGround(const std::string& outDir, int n)
{
    Surface s = Render(n, GrassBase);
    uint32_t r = 0;
    auto rnd = [&r]() { return Rand01(r++, 23u, 1650u); };
    const int blades = n * n / 26;
    for (int i = 0; i < blades; ++i)
    {
        const float u = rnd(), v = rnd(), a = -Pi * 0.5f + (rnd() - 0.5f) * 2.6f, len = 0.006f + rnd() * 0.012f;
        const float t = rnd(), dry = rnd();
        Color c = Mix(Color{0.13f, 0.18f, 0.06f}, Color{0.29f, 0.33f, 0.13f}, t);
        if (dry > 0.80f) { c = Mix(c, Color{0.50f, 0.44f, 0.22f}, 0.7f); }
        Stroke(s, u, v, u + std::cos(a) * len, v + std::sin(a) * len, 1.5f * n / 1024.f, c, 0.8f, 0.08f);
    }
    for (Px& p : s.P) { p.Alpha = 1.f; }
    Save(s, outDir, "GrassGround", 4.f, 2.0f);
}

/** Alpha card of a grass clump (blades fanning out from the bottom centre). variant 0 green, 1 dry/tall, 2 weeds with seed heads. */
void GrassCard(const std::string& outDir, const std::string& name, int n, int variant)
{
    const int big = n * 2;   // supersampled
    Surface s; s.N = big; s.P.resize(static_cast<size_t>(big) * big);
    const Color fill = variant == 1 ? Color{0.42f, 0.40f, 0.20f} : Color{0.20f, 0.28f, 0.09f};
    for (Px& p : s.P) { p.C = fill; p.Alpha = 0.f; p.H = 0.5f; p.Rough = 0.7f; }
    uint32_t r = 0;
    const uint32_t seed = 2400u + static_cast<uint32_t>(variant) * 37u;
    auto rnd = [&r, seed]() { return Rand01(r++, 5u, seed); };
    const int blades = variant == 2 ? 26 : 58;
    for (int i = 0; i < blades; ++i)
    {
        const float bx = 0.5f + (rnd() - 0.5f) * 0.62f;
        const float height = (variant == 1 ? 0.55f : 0.38f) + rnd() * (variant == 1 ? 0.42f : 0.5f);
        const float lean = (bx - 0.5f) * 0.9f + (rnd() - 0.5f) * 0.5f;
        const float bend = (rnd() - 0.3f) * 0.35f * (lean >= 0.f ? 1.f : -1.f);
        const float width = (0.010f + rnd() * 0.012f) * big;
        const float hue = rnd(), shade = 0.75f + rnd() * 0.45f;
        const Color base = variant == 1 ? Color{0.30f, 0.27f, 0.12f} : Color{0.07f, 0.12f, 0.035f};
        const Color tip = variant == 1 ? Mix(Color{0.62f, 0.56f, 0.30f}, Color{0.50f, 0.50f, 0.22f}, hue) : Mix(Color{0.30f, 0.42f, 0.12f}, Color{0.42f, 0.44f, 0.16f}, hue);
        constexpr int seg = 22;
        float pu = bx, pv = 0.995f;
        for (int k = 1; k <= seg; ++k)
        {
            const float t = static_cast<float>(k) / seg;
            const float u = bx + lean * height * t + bend * t * t, v = 0.995f - height * (t - 0.18f * t * t);
            const float w = std::max(1.2f, width * (1.f - t * t * 0.92f));
            Stroke(s, pu, pv, u, v, w, Scale(Mix(base, tip, Smooth(0.f, 0.9f, t)), shade), 1.f, 0.f, false);
            pu = u; pv = v;
        }
        if (variant == 2 && i % 3 == 0)   // seed head
        {
            for (int k = 0; k < 9; ++k)
            {
                const float a = rnd() * 2.f * Pi, d = rnd() * 0.02f;
                Stroke(s, pu, pv, pu + std::cos(a) * d, pv + std::sin(a) * d * 1.6f, 0.007f * big, Scale(Color{0.55f, 0.50f, 0.33f}, shade), 1.f, 0.f, false);
            }
        }
    }
    // downsample 2x
    Surface o; o.N = n; o.P.resize(static_cast<size_t>(n) * n);
    for (int y = 0; y < n; ++y)
    {
        for (int x = 0; x < n; ++x)
        {
            Px acc; acc.C = {0, 0, 0}; acc.Alpha = 0.f; float wsum = 0.f;
            for (int k = 0; k < 4; ++k)
            {
                const Px& p = s.P[static_cast<size_t>(y * 2 + k / 2) * big + (x * 2 + k % 2)];
                const float w = p.Alpha + 0.001f;
                acc.C.R += p.C.R * w; acc.C.G += p.C.G * w; acc.C.B += p.C.B * w; acc.Alpha += p.Alpha * 0.25f; wsum += w;
            }
            acc.C = Scale(acc.C, 1.f / wsum); acc.H = 0.5f; acc.Rough = 0.65f;
            o.P[static_cast<size_t>(y) * n + x] = acc;
        }
    }
    Save(o, outDir, name, 0.f, 0.f, true);
}
}   // namespace

int RunTextures(const std::string& outDir, int size)
{
    std::printf("SpnWorldTool textures -> %s (%d px)\n", outDir.c_str(), size);
    const int n = size, half = std::max(256, size / 2);
    Save(Render(n, [](float u, float v) { return Asphalt(u, v, false); }), outDir, "Asphalt", 5.f, 2.0f);
    Save(Render(n, [](float u, float v) { return Asphalt(u, v, true); }), outDir, "AsphaltOld", 5.f, 2.0f);
    Save(Render(n, [](float u, float v) { return Concrete(u, v, false); }), outDir, "Concrete", 3.f, 1.5f);
    Save(Render(n, [](float u, float v) { return Concrete(u, v, true); }), outDir, "Sidewalk", 4.f, 1.5f);
    Save(Render(n, Plaster), outDir, "Plaster", 4.f, 1.5f);
    Save(Render(n, Siding), outDir, "Siding", 7.f, 2.5f);
    Save(Render(n, [](float u, float v) { return Planks(u, v, 16, false); }), outDir, "Planks", 5.f, 2.0f);
    Save(Render(n, [](float u, float v) { return Planks(u, v, 24, true); }), outDir, "WoodFloor", 2.5f, 1.0f);
    Save(Render(n, Brick), outDir, "Brick", 6.f, 2.5f);
    Save(Render(n, Shingles), outDir, "Shingles", 7.f, 2.5f);
    Save(Render(n, Corrugated), outDir, "Corrugated", 9.f, 1.5f);
    Save(Render(half, PaintedMetal), outDir, "PaintedMetal", 2.f, 1.0f);
    Save(Render(half, Steel), outDir, "Steel", 1.5f, 0.5f);
    Save(Render(n, Gravel), outDir, "Gravel", 7.f, 2.5f);
    Save(Render(n, Dirt), outDir, "Dirt", 6.f, 2.0f);
    Save(Render(n, Rock), outDir, "Rock", 8.f, 2.5f);
    ForestFloor(outDir, n);
    GrassGround(outDir, n);
    Save(Render(half, WallPaint), outDir, "WallPaint", 1.5f, 0.5f);
    Save(Render(half, Wallpaper), outDir, "Wallpaper", 1.5f, 0.5f);
    Save(Render(half, Carpet), outDir, "Carpet", 5.f, 1.5f);
    Save(Render(half, Linoleum), outDir, "Linoleum", 2.f, 1.0f);
    Save(Render(half, Ceiling), outDir, "Ceiling", 5.f, 1.5f);
    Save(Render(half, Grunge), outDir, "Grunge", 0.f, 0.f);
    Save(Render(512, Blotch), outDir, "DecalStain", 0.f, 0.f, true);
    Save(Render(512, CrackDecal), outDir, "DecalCrack", 6.f, 0.f, true);
    GrassCard(outDir, "GrassCard", 512, 0);
    GrassCard(outDir, "GrassDryCard", 512, 1);
    GrassCard(outDir, "WeedCard", 512, 2);
    return 0;
}
}   // namespace spn
