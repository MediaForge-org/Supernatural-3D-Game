#include <algorithm>
#include <cmath>
#include <spn/core/SpatialGrid.h>

namespace spn::core {

bool IsValidMapCoordinate(Vec2 position) noexcept {
    return std::isfinite(position.x) && std::isfinite(position.y) && std::abs(position.x) <= kMaxMapCoordinateMetres &&
           std::abs(position.y) <= kMaxMapCoordinateMetres;
}

SpatialGrid::SpatialGrid(double cellSizeMetres) : cellSize_(std::isfinite(cellSizeMetres) && cellSizeMetres > 0.0 ? cellSizeMetres : 256.0) {}

std::size_t SpatialGrid::CellHash::operator()(const Cell& cell) const noexcept {
    const auto x = static_cast<std::uint64_t>(cell.x);
    const auto y = static_cast<std::uint64_t>(cell.y);
    return static_cast<std::size_t>((x * 0x9E3779B97F4A7C15ULL) ^ (y + 0x7F4A7C15ULL + (x << 6U) + (x >> 2U)));
}

SpatialGrid::Cell SpatialGrid::CellOf(Vec2 position) const noexcept {
    return Cell{static_cast<std::int64_t>(std::floor(position.x / cellSize_)), static_cast<std::int64_t>(std::floor(position.y / cellSize_))};
}

bool SpatialGrid::Insert(std::uint64_t key, Vec2 position) {
    if (!IsValidMapCoordinate(position) || points_.contains(key)) {
        return false;
    }
    points_.emplace(key, position);
    cells_[CellOf(position)].push_back(key);
    return true;
}

bool SpatialGrid::Remove(std::uint64_t key) {
    const auto it = points_.find(key);
    if (it == points_.end()) {
        return false;
    }
    const auto cellIt = cells_.find(CellOf(it->second));
    auto& keys = cellIt->second;
    keys.erase(std::find(keys.begin(), keys.end(), key));
    if (keys.empty()) {
        cells_.erase(cellIt);
    }
    points_.erase(it);
    return true;
}

bool SpatialGrid::Move(std::uint64_t key, Vec2 position) {
    if (!IsValidMapCoordinate(position) || !points_.contains(key)) {
        return false;
    }
    Remove(key);
    return Insert(key, position);
}

const Vec2* SpatialGrid::Find(std::uint64_t key) const {
    const auto it = points_.find(key);
    return it == points_.end() ? nullptr : &it->second;
}

void SpatialGrid::CollectCandidates(Vec2 min, Vec2 max, std::vector<std::uint64_t>& out) const {
    const auto clamp = [](double v) { return std::clamp(v, -kMaxMapCoordinateMetres, kMaxMapCoordinateMetres); };
    const Cell lo = CellOf(Vec2{clamp(min.x), clamp(min.y)});
    const Cell hi = CellOf(Vec2{clamp(max.x), clamp(max.y)});
    const double span = (static_cast<double>(hi.x - lo.x) + 1.0) * (static_cast<double>(hi.y - lo.y) + 1.0);
    if (span > static_cast<double>(cells_.size())) {
        // The window covers more cells than are occupied: walk the occupied cells instead.
        for (const auto& [cell, keys] : cells_) {
            if (cell.x >= lo.x && cell.x <= hi.x && cell.y >= lo.y && cell.y <= hi.y) {
                out.insert(out.end(), keys.begin(), keys.end());
            }
        }
        return;
    }
    for (std::int64_t cx = lo.x; cx <= hi.x; ++cx) {
        for (std::int64_t cy = lo.y; cy <= hi.y; ++cy) {
            const auto it = cells_.find(Cell{cx, cy});
            if (it != cells_.end()) {
                out.insert(out.end(), it->second.begin(), it->second.end());
            }
        }
    }
}

void SpatialGrid::QueryRect(Vec2 min, Vec2 max, std::vector<std::uint64_t>& out) const {
    if (!std::isfinite(min.x) || !std::isfinite(min.y) || !std::isfinite(max.x) || !std::isfinite(max.y) || min.x > max.x || min.y > max.y) {
        return;
    }
    const std::size_t first = out.size();
    CollectCandidates(min, max, out);
    // Cells are coarser than the rectangle: filter exactly.
    const auto tail = std::remove_if(out.begin() + static_cast<std::ptrdiff_t>(first), out.end(), [&](std::uint64_t key) {
        const Vec2 p = points_.at(key);
        return p.x < min.x || p.x > max.x || p.y < min.y || p.y > max.y;
    });
    out.erase(tail, out.end());
    std::sort(out.begin() + static_cast<std::ptrdiff_t>(first), out.end());
}

void SpatialGrid::QueryRadius(Vec2 center, double radius, std::vector<std::uint64_t>& out) const {
    if (!std::isfinite(center.x) || !std::isfinite(center.y) || !std::isfinite(radius) || radius < 0.0) {
        return;
    }
    const std::size_t first = out.size();
    CollectCandidates(Vec2{center.x - radius, center.y - radius}, Vec2{center.x + radius, center.y + radius}, out);
    const double radiusSq = radius * radius;
    const auto tail = std::remove_if(out.begin() + static_cast<std::ptrdiff_t>(first), out.end(), [&](std::uint64_t key) {
        const Vec2 p = points_.at(key);
        const double dx = p.x - center.x;
        const double dy = p.y - center.y;
        return dx * dx + dy * dy > radiusSq;
    });
    out.erase(tail, out.end());
    std::sort(out.begin() + static_cast<std::ptrdiff_t>(first), out.end());
}

} // namespace spn::core
