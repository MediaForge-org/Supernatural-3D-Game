#pragma once

// Planar geometry helpers for map features (roads, buildings, areas, routes). Portable, no engine types. Metres, x east, y north.
//
// ENGINEERING FOUNDATION (NightBuild02), not master canon. All functions are pure and deterministic. Inputs must be finite; callers
// validate coordinates (IsValidMapCoordinate) before they get here.

#include <cstddef>
#include <spn/core/Vec.h>
#include <spn/core/WorldMap.h>
#include <vector>

namespace spn::core {

[[nodiscard]] double Distance(Vec2 a, Vec2 b) noexcept;

/// Smallest rect containing all points (an empty list yields a degenerate rect at the origin).
[[nodiscard]] MapRect BoundsOf(const std::vector<Vec2>& points) noexcept;
[[nodiscard]] bool RectsIntersect(const MapRect& a, const MapRect& b) noexcept;

struct SegmentProjection {
    Vec2 point;            ///< nearest point on the segment
    double t = 0.0;        ///< parameter along a -> b in [0, 1]
    double distance = 0.0; ///< |p - point|
};
/// Nearest point of segment a-b to p. A degenerate segment (a == b) projects to a.
[[nodiscard]] SegmentProjection ProjectPointToSegment(Vec2 p, Vec2 a, Vec2 b) noexcept;

[[nodiscard]] double PolylineLength(const std::vector<Vec2>& points) noexcept;
/// Cumulative length at each vertex (first entry 0); same size as `points`.
[[nodiscard]] std::vector<double> CumulativeLengths(const std::vector<Vec2>& points);

struct PolylineProjection {
    Vec2 point;
    double offsetMetres = 0.0; ///< distance along the polyline from its first vertex
    double distance = 0.0;     ///< |p - point|
    std::size_t edge = 0;      ///< index of the edge (points[edge] -> points[edge + 1])
};
/// Nearest point of the polyline to p; ties go to the earlier edge. Precondition: at least two points; `cumulative` from CumulativeLengths.
[[nodiscard]] PolylineProjection ProjectPointToPolyline(Vec2 p, const std::vector<Vec2>& points, const std::vector<double>& cumulative) noexcept;

/// Point at `offsetMetres` along the polyline (clamped to [0, length]).
[[nodiscard]] Vec2 PointAtOffset(const std::vector<Vec2>& points, const std::vector<double>& cumulative, double offsetMetres) noexcept;

/// The part of the polyline between two offsets, oriented from `fromOffset` to `toOffset` (reversed if from > to). The first and last point are
/// exactly `startPoint` / `endPoint` (so callers can chain pieces without rounding gaps); interior vertices are the polyline vertices strictly
/// between the offsets.
[[nodiscard]] std::vector<Vec2> SlicePolyline(const std::vector<Vec2>& points, const std::vector<double>& cumulative, double fromOffset, Vec2 startPoint,
                                              double toOffset, Vec2 endPoint);

/// True if segments a-b and c-d share at least one point (touching and collinear overlap count).
[[nodiscard]] bool SegmentsIntersect(Vec2 a, Vec2 b, Vec2 c, Vec2 d) noexcept;

/// Even-odd point-in-polygon, boundary counts as inside. `ring` is an open ring (the closing edge last->first is implied).
[[nodiscard]] bool PolygonContains(const std::vector<Vec2>& ring, Vec2 p) noexcept;
/// Absolute area of the ring.
[[nodiscard]] double PolygonArea(const std::vector<Vec2>& ring) noexcept;
/// True if segment a-b touches or crosses any edge of the ring (a segment lying completely inside does NOT).
[[nodiscard]] bool SegmentCrossesRingBoundary(Vec2 a, Vec2 b, const std::vector<Vec2>& ring) noexcept;
/// True if segment a-b touches or crosses any edge of the polyline.
[[nodiscard]] bool SegmentCrossesPolyline(Vec2 a, Vec2 b, const std::vector<Vec2>& line) noexcept;
/// Parameter t in [0, 1] along a -> b of the single crossing point with segment c-d; false for parallel / collinear segments or no crossing.
[[nodiscard]] bool SegmentIntersectionParameter(Vec2 a, Vec2 b, Vec2 c, Vec2 d, double& t) noexcept;
/// True if the ring has non-zero area and no two non-adjacent edges intersect (simple polygon).
[[nodiscard]] bool IsSimpleRing(const std::vector<Vec2>& ring);

/// Douglas-Peucker with a tolerance in the same unit as the points; keeps the first and last point, preserves order. Iterative (no recursion
/// depth limit). Tolerance <= 0 or non-finite returns the input.
[[nodiscard]] std::vector<Vec2> SimplifyPolyline(const std::vector<Vec2>& points, double tolerance);

/// Clips segment a-b to the axis-aligned rect (Liang-Barsky). nullopt-style: returns false when nothing remains; otherwise writes the clipped end points.
[[nodiscard]] bool ClipSegmentToRect(Vec2 a, Vec2 b, const MapRect& rect, Vec2& outA, Vec2& outB) noexcept;
/// Clips segment a-b to the circle (centre, radius). Same contract as ClipSegmentToRect.
[[nodiscard]] bool ClipSegmentToCircle(Vec2 a, Vec2 b, Vec2 centre, double radius, Vec2& outA, Vec2& outB) noexcept;

} // namespace spn::core
