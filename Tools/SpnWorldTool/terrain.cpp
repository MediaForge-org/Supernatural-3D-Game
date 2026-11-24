// Terrain of the placeholder world: a height field shaped by noise, then fitted to the road network, building lots, a creek and ponds
// described in a plain text file written by the Unreal generator script (spn_world/layout.py). Also writes the ground layer weights.
//
// Input lines (cm, degrees):
//   grid <samples> <quad_cm> <zscale>            seed <n>            town <cx> <cy> <radius>
//   road <name> <half_width> <shoulder> <blend> <ditch 0|1> <n> x y x y ...
//   pad <name> <cx> <cy> <half_x> <half_y> <yaw> <blend> <anchor_x> <anchor_y> <offset>      (anchor: point whose final height the pad takes)
//   paved <cx> <cy> <half_x> <half_y> <yaw>      (ground covered by a drive, path or parking area: gravel layer, so no grass grows through)
//   creek <name> <half_width> <depth> <n> x y ...      pond <name> <cx> <cy> <radius> <depth>      clear <cx> <cy> <radius>
// Output in <out>: height.r16, layer_<Grass|Forest|Dirt|Gravel|Rock>.r8, forest.r8, roaddist.r8, heights.txt (road, pad, creek, pond heights).
#include "common.h"

#include <fstream>
#include <sstream>

namespace spn
{
namespace
{
struct Vec2 { float X = 0.f, Y = 0.f; };
struct Road { std::string Name; float Half = 300.f, Shoulder = 100.f, Blend = 800.f; bool Ditch = false; std::vector<Vec2> P; std::vector<float> H; };
struct Pad { std::string Name; float Cx, Cy, Hx, Hy, Yaw, Blend, Ax, Ay, Offset; float Z = 0.f; };
struct Paved { float Cx, Cy, Hx, Hy, Yaw; };
struct Creek { std::string Name; float Half = 150.f, Depth = 120.f; std::vector<Vec2> P; std::vector<float> H; };
struct Pond { std::string Name; float Cx, Cy, Radius, Depth; float Z = 0.f; };
struct Clear { float Cx, Cy, Radius; };

struct Nearest { float D = 1e9f; float H = 0.f; int Index = -1; };

float SegDist(const Vec2& a, const Vec2& b, float x, float y, float& t)
{
    const float dx = b.X - a.X, dy = b.Y - a.Y, len2 = dx * dx + dy * dy;
    t = len2 > 0.f ? Clamp01(((x - a.X) * dx + (y - a.Y) * dy) / len2) : 0.f;
    const float px = a.X + dx * t - x, py = a.Y + dy * t - y;
    return std::sqrt(px * px + py * py);
}

/** Uniform buckets over polyline segments for nearest queries. */
struct SegmentGrid
{
    float Cell = 2500.f, Min = 0.f; int Count = 0;
    struct Seg { int Line, Index; };
    std::vector<std::vector<Seg>> Buckets;
    void Init(float extent) { Min = -extent; Count = static_cast<int>(2.f * extent / Cell) + 1; Buckets.assign(static_cast<size_t>(Count) * Count, {}); }
    int Coord(float v) const { return std::clamp(static_cast<int>((v - Min) / Cell), 0, Count - 1); }
    void Add(int line, int index, const Vec2& a, const Vec2& b, float reach)
    {
        const int x0 = Coord(std::min(a.X, b.X) - reach), x1 = Coord(std::max(a.X, b.X) + reach), y0 = Coord(std::min(a.Y, b.Y) - reach), y1 = Coord(std::max(a.Y, b.Y) + reach);
        for (int y = y0; y <= y1; ++y) { for (int x = x0; x <= x1; ++x) { Buckets[static_cast<size_t>(y) * Count + x].push_back({line, index}); } }
    }
    const std::vector<Seg>& At(float x, float y) const { return Buckets[static_cast<size_t>(Coord(y)) * Count + Coord(x)]; }
};
}   // namespace

int RunTerrain(const std::string& specPath, const std::string& outDir)
{
    int n = 1009; float quad = 125.f, zscale = 100.f; uint32_t seed = 7u;
    float townX = 0.f, townY = 0.f, townR = 26000.f;
    std::vector<Road> roads; std::vector<Pad> pads; std::vector<Creek> creeks; std::vector<Pond> ponds; std::vector<Clear> clears; std::vector<Paved> paved;

    std::ifstream in(specPath);
    if (!in) { std::fprintf(stderr, "cannot read %s\n", specPath.c_str()); return 2; }
    std::string line;
    while (std::getline(in, line))
    {
        std::istringstream ss(line); std::string kind; ss >> kind;
        if (kind == "grid") { ss >> n >> quad >> zscale; }
        else if (kind == "seed") { ss >> seed; }
        else if (kind == "town") { ss >> townX >> townY >> townR; }
        else if (kind == "road") { Road r; int c = 0, ditch = 0; ss >> r.Name >> r.Half >> r.Shoulder >> r.Blend >> ditch >> c; r.Ditch = ditch != 0; r.P.resize(static_cast<size_t>(c)); for (Vec2& p : r.P) { ss >> p.X >> p.Y; } roads.push_back(std::move(r)); }
        else if (kind == "pad") { Pad p; ss >> p.Name >> p.Cx >> p.Cy >> p.Hx >> p.Hy >> p.Yaw >> p.Blend >> p.Ax >> p.Ay >> p.Offset; pads.push_back(p); }
        else if (kind == "paved") { Paved p; ss >> p.Cx >> p.Cy >> p.Hx >> p.Hy >> p.Yaw; paved.push_back(p); }
        else if (kind == "creek") { Creek c; int k = 0; ss >> c.Name >> c.Half >> c.Depth >> k; c.P.resize(static_cast<size_t>(k)); for (Vec2& p : c.P) { ss >> p.X >> p.Y; } creeks.push_back(std::move(c)); }
        else if (kind == "pond") { Pond p; ss >> p.Name >> p.Cx >> p.Cy >> p.Radius >> p.Depth; ponds.push_back(p); }
        else if (kind == "clear") { Clear c; ss >> c.Cx >> c.Cy >> c.Radius; clears.push_back(c); }
    }
    const float extent = (n - 1) * quad * 0.5f;
    std::printf("SpnWorldTool terrain: %d x %d samples, %.0f m, %zu roads, %zu pads\n", n, n, extent * 2.f / 100.f, roads.size(), pads.size());
    auto wx = [&](int i) { return i * quad - extent; };

    // ---- base shape (cm)
    auto base = [&](float x, float y)
    {
        const float u = x / (2.f * extent) + 0.5f, v = y / (2.f * extent) + 0.5f;
        const float dx = (x - townX) / (extent * 1.55f), dy = (y - townY) / extent;
        const float ca = std::cos(0.22f), sa = std::sin(0.22f);
        const float rx = dx * ca + dy * sa, ry = -dx * sa + dy * ca;
        const float r = std::sqrt(rx * rx + ry * ry);
        const float warp = Fbm(u, v, 2, 3, seed + 1u, 0.5f, false);
        const float rim = Smooth(0.30f, 1.05f, r + (warp - 0.5f) * 0.35f);
        const float hills = Fbm(u + warp * 0.08f, v, 3, 5, seed + 2u, 0.5f, false), ridges = Ridged(u, v + warp * 0.1f, 4, 4, seed + 3u, false);
        const float mid = Fbm(u, v, 22, 4, seed + 4u, 0.5f, false), fine = Fbm(u, v, 110, 3, seed + 5u, 0.5f, false);
        const float town = 1.f - Smooth(0.6f, 1.25f, std::sqrt((x - townX) * (x - townX) + (y - townY) * (y - townY)) / townR);
        float h = rim * rim * 5200.f * (0.55f + 0.9f * hills) + (hills - 0.5f) * 2600.f * (0.2f + rim) + (ridges - 0.5f) * 1500.f * rim;
        h += (mid - 0.5f) * 420.f * (0.25f + rim) * (1.f - 0.7f * town) + (fine - 0.5f) * 60.f * (1.f - 0.85f * town);
        h += (x * 0.006f - y * 0.011f);                // the whole valley drains gently to one side
        return h;
    };
    std::vector<float> h0(static_cast<size_t>(n) * n), h(static_cast<size_t>(n) * n);
    ParallelRows(n, [&](int j) { for (int i = 0; i < n; ++i) { h0[static_cast<size_t>(j) * n + i] = base(wx(i), wx(j)); } });
    auto sample = [&](const std::vector<float>& f, float x, float y)
    {
        const float fx = std::clamp((x + extent) / quad, 0.f, n - 1.001f), fy = std::clamp((y + extent) / quad, 0.f, n - 1.001f);
        const int ix = static_cast<int>(fx), iy = static_cast<int>(fy); const float tx = fx - ix, ty = fy - iy;
        const float* p = &f[static_cast<size_t>(iy) * n + ix];
        return Lerp(Lerp(p[0], p[1], tx), Lerp(p[n], p[n + 1], tx), ty);
    };

    // ---- creeks: carve first, roads override them later
    h = h0;
    SegmentGrid creekGrid; creekGrid.Init(extent);
    for (size_t c = 0; c < creeks.size(); ++c)
    {
        Creek& cr = creeks[c];
        cr.H.resize(cr.P.size());
        for (size_t i = 0; i < cr.P.size(); ++i) { cr.H[i] = sample(h0, cr.P[i].X, cr.P[i].Y); }
        for (int pass = 0; pass < 60; ++pass) { for (size_t i = 1; i + 1 < cr.H.size(); ++i) { cr.H[i] = 0.5f * cr.H[i] + 0.25f * (cr.H[i - 1] + cr.H[i + 1]); } }
        for (size_t i = 1; i < cr.H.size(); ++i) { cr.H[i] = std::min(cr.H[i], cr.H[i - 1] - 0.5f); }      // water runs downhill
        for (float& v : cr.H) { v -= cr.Depth; }
        for (size_t i = 0; i + 1 < cr.P.size(); ++i) { creekGrid.Add(static_cast<int>(c), static_cast<int>(i), cr.P[i], cr.P[i + 1], cr.Half + 1500.f); }
    }
    std::vector<float> creekDist(static_cast<size_t>(n) * n, 1e9f);
    ParallelRows(n, [&](int j)
    {
        for (int i = 0; i < n; ++i)
        {
            const float x = wx(i), y = wx(j); const size_t k = static_cast<size_t>(j) * n + i;
            for (const SegmentGrid::Seg& s : creekGrid.At(x, y))
            {
                const Creek& cr = creeks[static_cast<size_t>(s.Line)];
                float t; const float d = SegDist(cr.P[static_cast<size_t>(s.Index)], cr.P[static_cast<size_t>(s.Index) + 1], x, y, t);
                if (d - cr.Half >= creekDist[k]) { continue; }
                creekDist[k] = d - cr.Half;
                const float bed = Lerp(cr.H[static_cast<size_t>(s.Index)], cr.H[static_cast<size_t>(s.Index) + 1], t);
                const float wobble = (Fbm(x / 3000.f, y / 3000.f, 1, 3, seed + 20u, 0.5f, false) - 0.5f) * 200.f;
                const float bank = Smooth(0.f, 900.f + wobble, d - cr.Half);
                h[k] = std::min(h[k], Lerp(bed, std::max(h0[k], bed + 40.f), bank));
            }
        }
    });
    for (Pond& p : ponds)
    {
        p.Z = sample(h, p.Cx, p.Cy) - 60.f;
        for (int j = 0; j < n; ++j) { for (int i = 0; i < n; ++i)
        {
            const float x = wx(i), y = wx(j); const size_t k = static_cast<size_t>(j) * n + i;
            const float wob = 1.f + (Fbm(x / 9000.f + 3.f, y / 9000.f, 1, 3, seed + 21u, 0.5f, false) - 0.5f) * 0.5f;
            const float d = std::sqrt((x - p.Cx) * (x - p.Cx) + (y - p.Cy) * (y - p.Cy)) / (p.Radius * wob);
            if (d > 1.9f) { continue; }
            const float bowl = p.Z + 25.f - p.Depth * Smooth(1.05f, 0.25f, d);
            h[k] = Lerp(bowl, h[k], Smooth(1.0f, 1.9f, d));
            creekDist[k] = std::min(creekDist[k], (d - 1.f) * p.Radius);
        } }
    }
    std::vector<float> h1 = h;   // terrain with water, before roads

    // ---- road profiles: follow the terrain, smoothed along the road, equal height where roads meet
    struct Node { int Road, Index; std::vector<int> Links; float H, Target; };
    std::vector<Node> nodes; std::vector<int> first(roads.size());
    for (size_t r = 0; r < roads.size(); ++r)
    {
        first[r] = static_cast<int>(nodes.size());
        for (size_t i = 0; i < roads[r].P.size(); ++i)
        {
            Node nd; nd.Road = static_cast<int>(r); nd.Index = static_cast<int>(i);
            float acc = 0.f;   // average over the road width so a road does not follow every bump
            for (int k = -1; k <= 1; ++k) { acc += sample(h0, roads[r].P[i].X + k * roads[r].Half, roads[r].P[i].Y) + sample(h0, roads[r].P[i].X, roads[r].P[i].Y + k * roads[r].Half); }
            nd.Target = nd.H = acc / 6.f;
            if (i > 0) { nd.Links.push_back(static_cast<int>(nodes.size()) - 1); }
            if (i + 1 < roads[r].P.size()) { nd.Links.push_back(static_cast<int>(nodes.size()) + 1); }
            nodes.push_back(std::move(nd));
        }
    }
    for (size_t a = 0; a < nodes.size(); ++a) { for (size_t b = a + 1; b < nodes.size(); ++b)
    {
        if (nodes[a].Road == nodes[b].Road) { continue; }
        const Vec2& pa = roads[static_cast<size_t>(nodes[a].Road)].P[static_cast<size_t>(nodes[a].Index)]; const Vec2& pb = roads[static_cast<size_t>(nodes[b].Road)].P[static_cast<size_t>(nodes[b].Index)];
        const float reach = std::max(roads[static_cast<size_t>(nodes[a].Road)].Half, roads[static_cast<size_t>(nodes[b].Road)].Half) + 200.f;
        if ((pa.X - pb.X) * (pa.X - pb.X) + (pa.Y - pb.Y) * (pa.Y - pb.Y) < reach * reach)
        {
            for (int rep = 0; rep < 3; ++rep) { nodes[a].Links.push_back(static_cast<int>(b)); nodes[b].Links.push_back(static_cast<int>(a)); }
        }
    } }
    std::vector<float> next(nodes.size());
    for (int pass = 0; pass < 500; ++pass)
    {
        for (size_t i = 0; i < nodes.size(); ++i)
        {
            float acc = 0.f; for (int l : nodes[i].Links) { acc += nodes[static_cast<size_t>(l)].H; }
            const float avg = nodes[i].Links.empty() ? nodes[i].H : acc / nodes[i].Links.size();
            next[i] = Lerp(nodes[i].H, avg, 0.6f) + (nodes[i].Target - nodes[i].H) * 0.012f;
        }
        for (size_t i = 0; i < nodes.size(); ++i) { nodes[i].H = next[i]; }
    }
    for (size_t r = 0; r < roads.size(); ++r) { roads[r].H.resize(roads[r].P.size()); for (size_t i = 0; i < roads[r].P.size(); ++i) { roads[r].H[i] = nodes[static_cast<size_t>(first[r]) + i].H; } }

    SegmentGrid roadGrid; roadGrid.Init(extent);
    for (size_t r = 0; r < roads.size(); ++r) { for (size_t i = 0; i + 1 < roads[r].P.size(); ++i) { roadGrid.Add(static_cast<int>(r), static_cast<int>(i), roads[r].P[i], roads[r].P[i + 1], roads[r].Half + roads[r].Shoulder + roads[r].Blend + 300.f); } }
    std::vector<float> roadEdge(static_cast<size_t>(n) * n, 1e9f), ditch(static_cast<size_t>(n) * n, 0.f);   // distance beyond the paved edge, cm
    ParallelRows(n, [&](int j)
    {
        for (int i = 0; i < n; ++i)
        {
            const float x = wx(i), y = wx(j); const size_t k = static_cast<size_t>(j) * n + i;
            float best = 1e9f, roadH = 0.f, weightSum = 0.f, blended = 0.f; const Road* owner = nullptr;
            for (const SegmentGrid::Seg& s : roadGrid.At(x, y))
            {
                const Road& rd = roads[static_cast<size_t>(s.Line)];
                float t; const float d = SegDist(rd.P[static_cast<size_t>(s.Index)], rd.P[static_cast<size_t>(s.Index) + 1], x, y, t);
                const float e = d - rd.Half, hr = Lerp(rd.H[static_cast<size_t>(s.Index)], rd.H[static_cast<size_t>(s.Index) + 1], t);
                if (e < best) { best = e; roadH = hr; owner = &rd; }
                const float w = 1.f / (std::max(e + rd.Half, 30.f) * std::max(e + rd.Half, 30.f)); weightSum += w; blended += w * hr;   // junctions: no step between two roads
            }
            if (owner == nullptr) { continue; }
            roadEdge[k] = best;
            const float inner = owner->Shoulder;
            const float target = (best < inner ? blended / weightSum : roadH) - 6.f;
            if (best <= inner) { h[k] = target; continue; }
            const float t = (best - inner) / owner->Blend;
            if (t >= 1.f) { continue; }
            float out = Lerp(target, h[k], Smooth(0.f, 1.f, t));
            if (owner->Ditch)
            {
                const float g = (t - 0.2f) / 0.13f, dip = std::exp(-g * g);
                out -= 42.f * dip; ditch[k] = dip;
            }
            h[k] = out;
        }
    });

    // ---- building lots
    std::vector<float> padMask(static_cast<size_t>(n) * n, 0.f);
    for (Pad& p : pads)
    {
        p.Z = sample(h, p.Ax, p.Ay) + p.Offset;
        const float ca = std::cos(p.Yaw * Pi / 180.f), sa = std::sin(p.Yaw * Pi / 180.f);
        const float reach = std::sqrt(p.Hx * p.Hx + p.Hy * p.Hy) + p.Blend;
        const int i0 = std::max(0, static_cast<int>((p.Cx - reach + extent) / quad)), i1 = std::min(n - 1, static_cast<int>((p.Cx + reach + extent) / quad) + 1);
        const int j0 = std::max(0, static_cast<int>((p.Cy - reach + extent) / quad)), j1 = std::min(n - 1, static_cast<int>((p.Cy + reach + extent) / quad) + 1);
        for (int j = j0; j <= j1; ++j) { for (int i = i0; i <= i1; ++i)
        {
            const size_t k = static_cast<size_t>(j) * n + i;
            const float dx = wx(i) - p.Cx, dy = wx(j) - p.Cy;
            const float lx = std::fabs(dx * ca + dy * sa) - p.Hx, ly = std::fabs(-dx * sa + dy * ca) - p.Hy;
            const float d = std::sqrt(std::max(lx, 0.f) * std::max(lx, 0.f) + std::max(ly, 0.f) * std::max(ly, 0.f));
            if (d >= p.Blend) { continue; }
            const float w = 1.f - Smooth(0.f, p.Blend, d);
            const float roadLock = Smooth(-50.f, 250.f, roadEdge[k]);         // never bend the road itself
            h[k] = Lerp(h[k], p.Z, w * roadLock);
            padMask[k] = std::max(padMask[k], 1.f - Smooth(0.f, 500.f, d));
        } }
    }

    // ---- paved ground of the lots (the meshes lie on top; the layer below must not grow grass)
    std::vector<float> paveMask(static_cast<size_t>(n) * n, 0.f);
    for (const Paved& p : paved)
    {
        const float ca = std::cos(p.Yaw * Pi / 180.f), sa = std::sin(p.Yaw * Pi / 180.f);
        const float reach = std::sqrt(p.Hx * p.Hx + p.Hy * p.Hy) + 200.f;
        const int i0 = std::max(0, static_cast<int>((p.Cx - reach + extent) / quad)), i1 = std::min(n - 1, static_cast<int>((p.Cx + reach + extent) / quad) + 1);
        const int j0 = std::max(0, static_cast<int>((p.Cy - reach + extent) / quad)), j1 = std::min(n - 1, static_cast<int>((p.Cy + reach + extent) / quad) + 1);
        for (int j = j0; j <= j1; ++j) { for (int i = i0; i <= i1; ++i)
        {
            const float dx = wx(i) - p.Cx, dy = wx(j) - p.Cy;
            const float lx = std::fabs(dx * ca + dy * sa) - p.Hx, ly = std::fabs(-dx * sa + dy * ca) - p.Hy;
            const float d = std::sqrt(std::max(lx, 0.f) * std::max(lx, 0.f) + std::max(ly, 0.f) * std::max(ly, 0.f));
            const size_t k = static_cast<size_t>(j) * n + i;
            paveMask[k] = std::max(paveMask[k], 1.f - Smooth(70.f, 190.f, d));     // wider than one grid cell, so a narrow path is not missed
        } }
    }

    // ---- layers
    const char* layerNames[5] = {"Grass", "Forest", "Dirt", "Gravel", "Rock"};
    std::vector<std::vector<uint8_t>> layers(5, std::vector<uint8_t>(static_cast<size_t>(n) * n));
    std::vector<uint8_t> forestOut(static_cast<size_t>(n) * n), roadOut(static_cast<size_t>(n) * n);
    std::vector<uint16_t> heightOut(static_cast<size_t>(n) * n);
    float minH = 1e9f, maxH = -1e9f;
    for (float v : h) { minH = std::min(minH, v); maxH = std::max(maxH, v); }
    ParallelRows(n, [&](int j)
    {
        for (int i = 0; i < n; ++i)
        {
            const size_t k = static_cast<size_t>(j) * n + i; const float x = wx(i), y = wx(j);
            const float u = x / (2.f * extent) + 0.5f, v = y / (2.f * extent) + 0.5f;
            const int il = std::max(i - 1, 0), ir = std::min(i + 1, n - 1), jl = std::max(j - 1, 0), jr = std::min(j + 1, n - 1);
            const float sx = (h[static_cast<size_t>(j) * n + ir] - h[static_cast<size_t>(j) * n + il]) / ((ir - il) * quad), sy = (h[static_cast<size_t>(jr) * n + i] - h[static_cast<size_t>(jl) * n + i]) / ((jr - jl) * quad);
            const float slope = std::sqrt(sx * sx + sy * sy);
            const float townDist = std::sqrt((x - townX) * (x - townX) + (y - townY) * (y - townY)) / townR;
            // forest: outside the town, broken up by noise, kept away from roads, lots, water and clearings
            const float patch = Fbm(u, v, 7, 4, seed + 30u, 0.5f, false), edgeNoise = Fbm(u, v, 40, 3, seed + 31u, 0.5f, false);
            float forest = Smooth(0.80f, 1.25f, townDist + (patch - 0.5f) * 0.9f + (edgeNoise - 0.5f) * 0.25f);
            forest *= Smooth(600.f, 1500.f + edgeNoise * 900.f, roadEdge[k]) * (1.f - padMask[k]) * Smooth(150.f, 900.f, creekDist[k]);
            for (const Clear& c : clears)
            {
                const float d = std::sqrt((x - c.Cx) * (x - c.Cx) + (y - c.Cy) * (y - c.Cy)) / c.Radius;
                forest *= Smooth(0.75f, 1.15f, d + (edgeNoise - 0.5f) * 0.5f);
            }
            const float rock = Smooth(0.62f, 0.85f, slope) * (roadEdge[k] > 300.f ? 1.f : 0.f);
            // road shoulders: gravel in town, worn forest floor (dirt layer) on the tracks outside it, so a forest track fades into the forest
            const float shoulder = 1.f - Smooth(40.f, 260.f, roadEdge[k]);
            const float rural = Smooth(0.85f, 1.2f, townDist);
            const float underRoad = 1.f - Smooth(0.f, 40.f, roadEdge[k]);       // under the road surface itself: no grass layer, ever
            const float gravel = std::max(std::max(shoulder * (1.f - 0.85f * rural), underRoad) + Smooth(60.f, -40.f, creekDist[k]) * 0.8f, paveMask[k]);
            const float wornNoise = Fbm(u, v, 60, 3, seed + 32u, 0.5f, false);
            float dirt = ditch[k] * 0.55f + Smooth(350.f, 0.f, creekDist[k]) * 0.7f + padMask[k] * Smooth(0.55f, 0.75f, wornNoise) * 0.6f
                + Smooth(0.66f, 0.80f, Fbm(u, v, 18, 4, seed + 33u, 0.5f, false)) * 0.5f * (1.f - forest) + Smooth(0.25f, 0.5f, slope) * 0.4f;
            // forest floor: bare, needle-covered patches between the green ones, and worn earth verges along the roads outside the town
            dirt += shoulder * rural * 0.85f;
            dirt += Smooth(0.50f, 0.66f, Fbm(u, v, 30, 4, seed + 34u, 0.5f, false)) * 0.6f * forest
                + Smooth(900.f, 250.f, roadEdge[k]) * Smooth(0.85f, 1.2f, townDist) * Smooth(0.35f, 0.6f, Fbm(u, v, 48, 3, seed + 35u, 0.5f, false)) * 0.7f;
            float w[5];
            w[4] = Clamp01(rock);
            w[3] = Clamp01(gravel) * (1.f - w[4]);
            w[2] = Clamp01(dirt) * (1.f - w[4] - w[3]);
            w[1] = Clamp01(forest) * (1.f - w[4] - w[3] - w[2]);
            w[0] = std::max(0.f, 1.f - w[1] - w[2] - w[3] - w[4]);
            int total = 0; int q[5];
            for (int l = 1; l < 5; ++l) { q[l] = static_cast<int>(w[l] * 255.f + 0.5f); total += q[l]; }
            q[0] = std::max(0, 255 - total);
            for (int l = 0; l < 5; ++l) { layers[static_cast<size_t>(l)][k] = static_cast<uint8_t>(std::min(q[l], 255)); }
            forestOut[k] = static_cast<uint8_t>(Clamp01(forest * (1.f - w[4])) * 255.f);
            roadOut[k] = static_cast<uint8_t>(std::clamp(roadEdge[k] / 25.f, 0.f, 255.f));
            heightOut[k] = static_cast<uint16_t>(std::clamp(32768.f + h[k] * 128.f / zscale, 0.f, 65535.f));
        }
    });

    WriteFile(outDir + "/height.r16", heightOut.data(), heightOut.size() * 2);
    for (int l = 0; l < 5; ++l) { WriteFile(outDir + "/layer_" + layerNames[l] + ".r8", layers[static_cast<size_t>(l)].data(), layers[static_cast<size_t>(l)].size()); }
    WriteFile(outDir + "/forest.r8", forestOut.data(), forestOut.size());
    WriteFile(outDir + "/roaddist.r8", roadOut.data(), roadOut.size());
    // preview image: height as grey with roads and forest tinted
    std::vector<uint8_t> preview(static_cast<size_t>(n) * n * 3);
    for (size_t k = 0; k < h.size(); ++k)
    {
        const float g = (h[k] - minH) / std::max(1.f, maxH - minH);
        const float road = roadEdge[k] <= 0.f ? 1.f : 0.f, f = forestOut[k] / 255.f;
        preview[k * 3] = static_cast<uint8_t>(255.f * Lerp(g * (1.f - 0.45f * f), 0.15f, road));
        preview[k * 3 + 1] = static_cast<uint8_t>(255.f * Lerp(g * (1.f - 0.15f * f), 0.15f, road));
        preview[k * 3 + 2] = static_cast<uint8_t>(255.f * Lerp(creekDist[k] < 0.f ? 1.f : g * (1.f - 0.45f * f), 0.15f, road));
    }
    WriteTga(outDir + "/preview.tga", n, n, 3, preview);

    std::ofstream out(outDir + "/heights.txt");
    out << "range " << minH << " " << maxH << "\n";
    for (const Road& r : roads) { out << "road " << r.Name << " " << r.H.size(); for (float v : r.H) { out << " " << v; } out << "\n"; }
    for (const Pad& p : pads) { out << "pad " << p.Name << " " << p.Z << "\n"; }
    for (const Creek& c : creeks) { out << "creek " << c.Name << " " << c.H.size(); for (float v : c.H) { out << " " << v; } out << "\n"; }
    for (const Pond& p : ponds) { out << "pond " << p.Name << " " << p.Z << "\n"; }
    std::printf("  height %.1f .. %.1f m\n", minH / 100.f, maxH / 100.f);
    (void)h1;
    return 0;
}
}   // namespace spn
