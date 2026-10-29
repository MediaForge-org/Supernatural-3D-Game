#pragma once

// SpatialGrid: uniform-grid point index in the XY plane for frequent radius / rectangle queries (map markers, POIs, later NPC or
// loot lookups). Cost of a query is proportional to the cells it touches and the points in them, never to the total number of
// points; when a query would touch more cells than exist, the occupied cells are scanned instead. Results are sorted by key, so
// they are deterministic regardless of insertion order or hash-map iteration order.
//
// ENGINEERING FOUNDATION (M2B1), not master canon. Units are metres; the cell size is a tuning parameter of the index only.

#include <cstddef>
#include <cstdint>
#include <spn/core/Vec.h>
#include <unordered_map>
#include <vector>

namespace spn::core {

/// Largest accepted |x| and |y| (10,000 km). Far above any planned world (~2,200 km^2 is ~47 km across) yet small enough that
/// cell indices and squared distances stay exactly representable. Proposal, not a world-size decision.
inline constexpr double kMaxMapCoordinateMetres = 1.0e7;

/// True if both components are finite and within +-kMaxMapCoordinateMetres.
[[nodiscard]] bool IsValidMapCoordinate(Vec2 position) noexcept;

class SpatialGrid {
public:
    /// `cellSizeMetres` must be finite and > 0; otherwise 256 is used (a value for the index only, not a gameplay value).
    explicit SpatialGrid(double cellSizeMetres = 256.0);

    /// False if the key exists already or the position is not a valid map coordinate; nothing changes then.
    bool Insert(std::uint64_t key, Vec2 position);
    /// False if the key is unknown.
    bool Remove(std::uint64_t key);
    /// False if the key is unknown or the position invalid (the old position stays).
    bool Move(std::uint64_t key, Vec2 position);

    [[nodiscard]] const Vec2* Find(std::uint64_t key) const;
    [[nodiscard]] std::size_t Size() const noexcept { return points_.size(); }
    [[nodiscard]] std::size_t OccupiedCells() const noexcept { return cells_.size(); }
    [[nodiscard]] double CellSize() const noexcept { return cellSize_; }

    /// Appends the keys of all points with min <= p <= max (inclusive) to `out`, sorted ascending. An inverted or non-finite
    /// rectangle finds nothing.
    void QueryRect(Vec2 min, Vec2 max, std::vector<std::uint64_t>& out) const;
    /// Appends the keys of all points with distance(center, p) <= radius, sorted ascending. A negative or non-finite radius,
    /// or a non-finite center, finds nothing.
    void QueryRadius(Vec2 center, double radius, std::vector<std::uint64_t>& out) const;

private:
    struct Cell {
        std::int64_t x = 0;
        std::int64_t y = 0;
        friend bool operator==(const Cell&, const Cell&) = default;
    };
    struct CellHash {
        std::size_t operator()(const Cell& cell) const noexcept;
    };

    [[nodiscard]] Cell CellOf(Vec2 position) const noexcept;
    void CollectCandidates(Vec2 min, Vec2 max, std::vector<std::uint64_t>& out) const;

    double cellSize_;
    std::unordered_map<Cell, std::vector<std::uint64_t>, CellHash> cells_;
    std::unordered_map<std::uint64_t, Vec2> points_;
};

} // namespace spn::core
