// SpnWorldTool: standalone generator of PLACEHOLDER world source data (tiling PBR textures, terrain height and layer maps) for the
// visual build of the Unreal prototype. Engine-independent C++20, no third-party code. Everything it writes is generated, original data.
#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <functional>
#include <string>
#include <thread>
#include <vector>

namespace spn
{
constexpr float Pi = 3.14159265358979f;

inline uint32_t Hash(uint32_t x, uint32_t y, uint32_t seed)
{
    uint32_t h = x * 0x9E3779B1u ^ (y * 0x85EBCA77u + 0x165667B1u) ^ (seed * 0xC2B2AE3Du + 0x27D4EB2Fu);
    h ^= h >> 15; h *= 0x2C1B3C6Du; h ^= h >> 12; h *= 0x297A2D39u; h ^= h >> 15;
    return h;
}
inline float Rand01(uint32_t x, uint32_t y, uint32_t seed) { return static_cast<float>(Hash(x, y, seed) & 0xFFFFFFu) / 16777216.0f; }
inline float Clamp01(float v) { return v < 0.f ? 0.f : (v > 1.f ? 1.f : v); }
inline float Lerp(float a, float b, float t) { return a + (b - a) * t; }
inline float Smooth(float e0, float e1, float x)
{
    if (e0 == e1) { return x < e0 ? 0.f : 1.f; }
    const float t = Clamp01((x - e0) / (e1 - e0));
    return t * t * (3.f - 2.f * t);
}
inline int Wrap(int v, int n) { v %= n; return v < 0 ? v + n : v; }
inline float Fract(float v) { return v - std::floor(v); }

/** Periodic gradient noise, result about -1..1. x, y in lattice units; periodX/Y = lattice cells per repeat (0 = not periodic). */
inline float Gradient2(float x, float y, int periodX, int periodY, uint32_t seed)
{
    const float fx = std::floor(x), fy = std::floor(y);
    const int ix = static_cast<int>(fx), iy = static_cast<int>(fy);
    const float tx = x - fx, ty = y - fy;
    const float ux = tx * tx * tx * (tx * (tx * 6.f - 15.f) + 10.f), uy = ty * ty * ty * (ty * (ty * 6.f - 15.f) + 10.f);
    auto corner = [&](int cx, int cy, float dx, float dy)
    {
        const int wx = periodX > 0 ? Wrap(cx, periodX) : cx, wy = periodY > 0 ? Wrap(cy, periodY) : cy;
        const float a = Rand01(static_cast<uint32_t>(wx), static_cast<uint32_t>(wy), seed) * 2.f * Pi;
        return std::cos(a) * dx + std::sin(a) * dy;
    };
    const float a = corner(ix, iy, tx, ty), b = corner(ix + 1, iy, tx - 1.f, ty), c = corner(ix, iy + 1, tx, ty - 1.f), d = corner(ix + 1, iy + 1, tx - 1.f, ty - 1.f);
    return Lerp(Lerp(a, b, ux), Lerp(c, d, ux), uy) * 1.41f;
}
/** Anisotropic fractal noise 0..1: fu, fv cells per tile along u and v (stretched grain, streaks). */
inline float Fbm2(float u, float v, int fu, int fv, int octaves, uint32_t seed, float gain = 0.5f)
{
    float sum = 0.f, amp = 1.f, norm = 0.f;
    for (int o = 0; o < octaves; ++o)
    {
        sum += Gradient2(u * fu, v * fv, fu, fv, seed + static_cast<uint32_t>(o) * 193u) * amp;
        norm += amp; amp *= gain; fu *= 2; fv *= 2;
    }
    return Clamp01(0.5f + 0.5f * sum / norm);
}
inline float Gradient(float x, float y, int period, uint32_t seed)
{
    const float fx = std::floor(x), fy = std::floor(y);
    const int ix = static_cast<int>(fx), iy = static_cast<int>(fy);
    const float tx = x - fx, ty = y - fy;
    const float ux = tx * tx * tx * (tx * (tx * 6.f - 15.f) + 10.f), uy = ty * ty * ty * (ty * (ty * 6.f - 15.f) + 10.f);
    auto corner = [&](int cx, int cy, float dx, float dy)
    {
        const int wx = period > 0 ? Wrap(cx, period) : cx, wy = period > 0 ? Wrap(cy, period) : cy;
        const float a = Rand01(static_cast<uint32_t>(wx), static_cast<uint32_t>(wy), seed) * 2.f * Pi;
        return std::cos(a) * dx + std::sin(a) * dy;
    };
    const float a = corner(ix, iy, tx, ty), b = corner(ix + 1, iy, tx - 1.f, ty), c = corner(ix, iy + 1, tx, ty - 1.f), d = corner(ix + 1, iy + 1, tx - 1.f, ty - 1.f);
    return Lerp(Lerp(a, b, ux), Lerp(c, d, ux), uy) * 1.41f;
}

/** Fractal noise 0..1 over u, v in 0..1 with base frequency freq (cells per tile, periodic when periodic = true). */
inline float Fbm(float u, float v, int freq, int octaves, uint32_t seed, float gain = 0.5f, bool periodic = true)
{
    float sum = 0.f, amp = 1.f, norm = 0.f;
    int f = freq;
    for (int o = 0; o < octaves; ++o)
    {
        sum += Gradient(u * f, v * f, periodic ? f : 0, seed + static_cast<uint32_t>(o) * 131u) * amp;
        norm += amp; amp *= gain; f *= 2;
    }
    return Clamp01(0.5f + 0.5f * sum / norm);
}
inline float Ridged(float u, float v, int freq, int octaves, uint32_t seed, bool periodic = true)
{
    float sum = 0.f, amp = 1.f, norm = 0.f;
    int f = freq;
    for (int o = 0; o < octaves; ++o)
    {
        const float n = 1.f - std::fabs(Gradient(u * f, v * f, periodic ? f : 0, seed + static_cast<uint32_t>(o) * 977u));
        sum += n * n * amp; norm += amp; amp *= 0.5f; f *= 2;
    }
    return Clamp01(sum / norm);
}

struct WorleyResult { float F1 = 9.f, F2 = 9.f; uint32_t Id = 0; float Cx = 0.f, Cy = 0.f; };
/** Periodic cellular noise; distances in cell units. */
inline WorleyResult Worley(float u, float v, int freq, uint32_t seed, float jitter = 1.f)
{
    const float x = u * freq, y = v * freq;
    const int ix = static_cast<int>(std::floor(x)), iy = static_cast<int>(std::floor(y));
    WorleyResult r;
    for (int dy = -1; dy <= 1; ++dy)
    {
        for (int dx = -1; dx <= 1; ++dx)
        {
            const int cx = ix + dx, cy = iy + dy;
            const uint32_t wx = static_cast<uint32_t>(Wrap(cx, freq)), wy = static_cast<uint32_t>(Wrap(cy, freq));
            const float px = cx + 0.5f + (Rand01(wx, wy, seed) - 0.5f) * jitter, py = cy + 0.5f + (Rand01(wx, wy, seed + 71u) - 0.5f) * jitter;
            const float d = std::sqrt((px - x) * (px - x) + (py - y) * (py - y));
            if (d < r.F1) { r.F2 = r.F1; r.F1 = d; r.Id = Hash(wx, wy, seed + 9u); r.Cx = px / freq; r.Cy = py / freq; }
            else if (d < r.F2) { r.F2 = d; }
        }
    }
    return r;
}

struct Color { float R = 0.f, G = 0.f, B = 0.f; };
inline Color Mix(const Color& a, const Color& b, float t) { return {Lerp(a.R, b.R, t), Lerp(a.G, b.G, t), Lerp(a.B, b.B, t)}; }
inline Color Scale(const Color& a, float s) { return {a.R * s, a.G * s, a.B * s}; }

/** Runs fn(y) for every row on all cores. */
inline void ParallelRows(int rows, const std::function<void(int)>& fn)
{
    const unsigned workers = std::max(1u, std::thread::hardware_concurrency());
    std::vector<std::thread> pool;
    for (unsigned w = 0; w < workers; ++w)
    {
        pool.emplace_back([&, w] { for (int y = static_cast<int>(w); y < rows; y += static_cast<int>(workers)) { fn(y); } });
    }
    for (std::thread& t : pool) { t.join(); }
}

bool WriteTga(const std::string& path, int w, int h, int channels, const std::vector<uint8_t>& data);   // channels 1, 3 or 4 (RGB order in data)
bool WriteFile(const std::string& path, const void* data, size_t bytes);

int RunTextures(const std::string& outDir, int size);
int RunTerrain(const std::string& specPath, const std::string& outDir);
}
