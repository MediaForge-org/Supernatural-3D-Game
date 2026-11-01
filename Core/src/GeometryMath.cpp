#include <algorithm>
#include <limits>
#include <cmath>
#include <spn/core/GeometryMath.h>

namespace spn::core {

namespace {
double Cross(Vec2 o, Vec2 a, Vec2 b) noexcept { return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x); }
int Sign(double v) noexcept { return v > 0.0 ? 1 : (v < 0.0 ? -1 : 0); }
bool OnSegment(Vec2 a, Vec2 b, Vec2 p) noexcept {
    return std::min(a.x, b.x) <= p.x && p.x <= std::max(a.x, b.x) && std::min(a.y, b.y) <= p.y && p.y <= std::max(a.y, b.y);
}
} // namespace

double Distance(Vec2 a, Vec2 b) noexcept { return std::hypot(a.x - b.x, a.y - b.y); }

MapRect BoundsOf(const std::vector<Vec2>& points) noexcept {
    if (points.empty()) {
        return MapRect{};
    }
    MapRect r{points.front(), points.front()};
    for (const Vec2 p : points) {
        r.min.x = std::min(r.min.x, p.x);
        r.min.y = std::min(r.min.y, p.y);
        r.max.x = std::max(r.max.x, p.x);
        r.max.y = std::max(r.max.y, p.y);
    }
    return r;
}

bool RectsIntersect(const MapRect& a, const MapRect& b) noexcept { return a.min.x <= b.max.x && b.min.x <= a.max.x && a.min.y <= b.max.y && b.min.y <= a.max.y; }

SegmentProjection ProjectPointToSegment(Vec2 p, Vec2 a, Vec2 b) noexcept {
    const double dx = b.x - a.x;
    const double dy = b.y - a.y;
    const double len2 = dx * dx + dy * dy;
    double t = 0.0;
    if (len2 > 0.0) {
        t = std::clamp(((p.x - a.x) * dx + (p.y - a.y) * dy) / len2, 0.0, 1.0);
    }
    SegmentProjection r;
    r.t = t;
    r.point = t <= 0.0 ? a : (t >= 1.0 ? b : Vec2{a.x + dx * t, a.y + dy * t});
    r.distance = Distance(p, r.point);
    return r;
}

double PolylineLength(const std::vector<Vec2>& points) noexcept {
    double sum = 0.0;
    for (std::size_t i = 1; i < points.size(); ++i) {
        sum += Distance(points[i - 1], points[i]);
    }
    return sum;
}

std::vector<double> CumulativeLengths(const std::vector<Vec2>& points) {
    std::vector<double> out(points.size(), 0.0);
    for (std::size_t i = 1; i < points.size(); ++i) {
        out[i] = out[i - 1] + Distance(points[i - 1], points[i]);
    }
    return out;
}

PolylineProjection ProjectPointToPolyline(Vec2 p, const std::vector<Vec2>& points, const std::vector<double>& cumulative) noexcept {
    PolylineProjection best;
    best.distance = std::numeric_limits<double>::infinity();
    for (std::size_t i = 0; i + 1 < points.size(); ++i) {
        const SegmentProjection s = ProjectPointToSegment(p, points[i], points[i + 1]);
        if (s.distance < best.distance) {
            best.distance = s.distance;
            best.point = s.point;
            best.edge = i;
            best.offsetMetres = cumulative[i] + s.t * (cumulative[i + 1] - cumulative[i]);
        }
    }
    return best;
}

Vec2 PointAtOffset(const std::vector<Vec2>& points, const std::vector<double>& cumulative, double offsetMetres) noexcept {
    if (points.empty()) {
        return Vec2{};
    }
    if (offsetMetres <= 0.0) {
        return points.front();
    }
    if (offsetMetres >= cumulative.back()) {
        return points.back();
    }
    const auto it = std::upper_bound(cumulative.begin(), cumulative.end(), offsetMetres);
    const std::size_t i = static_cast<std::size_t>(it - cumulative.begin()) - 1;
    const double span = cumulative[i + 1] - cumulative[i];
    const double t = span > 0.0 ? (offsetMetres - cumulative[i]) / span : 0.0;
    return Vec2{points[i].x + (points[i + 1].x - points[i].x) * t, points[i].y + (points[i + 1].y - points[i].y) * t};
}

std::vector<Vec2> SlicePolyline(const std::vector<Vec2>& points, const std::vector<double>& cumulative, double fromOffset, Vec2 startPoint, double toOffset,
                                Vec2 endPoint) {
    std::vector<Vec2> out;
    out.push_back(startPoint);
    if (fromOffset <= toOffset) {
        for (std::size_t i = 0; i < points.size(); ++i) {
            if (cumulative[i] > fromOffset && cumulative[i] < toOffset) {
                out.push_back(points[i]);
            }
        }
    } else {
        for (std::size_t i = points.size(); i-- > 0;) {
            if (cumulative[i] < fromOffset && cumulative[i] > toOffset) {
                out.push_back(points[i]);
            }
        }
    }
    out.push_back(endPoint);
    return out;
}

bool SegmentsIntersect(Vec2 a, Vec2 b, Vec2 c, Vec2 d) noexcept {
    const int d1 = Sign(Cross(a, b, c));
    const int d2 = Sign(Cross(a, b, d));
    const int d3 = Sign(Cross(c, d, a));
    const int d4 = Sign(Cross(c, d, b));
    if (d1 != d2 && d3 != d4) {
        return true;
    }
    if (d1 == 0 && OnSegment(a, b, c)) { return true; }
    if (d2 == 0 && OnSegment(a, b, d)) { return true; }
    if (d3 == 0 && OnSegment(c, d, a)) { return true; }
    if (d4 == 0 && OnSegment(c, d, b)) { return true; }
    return false;
}

bool SegmentIntersectionParameter(Vec2 a, Vec2 b, Vec2 c, Vec2 d, double& t) noexcept {
    const double rx = b.x - a.x;
    const double ry = b.y - a.y;
    const double sx = d.x - c.x;
    const double sy = d.y - c.y;
    const double denom = rx * sy - ry * sx;
    if (denom == 0.0) {
        return false;
    }
    const double tt = ((c.x - a.x) * sy - (c.y - a.y) * sx) / denom;
    const double u = ((c.x - a.x) * ry - (c.y - a.y) * rx) / denom;
    if (tt < 0.0 || tt > 1.0 || u < 0.0 || u > 1.0) {
        return false;
    }
    t = tt;
    return true;
}

bool PolygonContains(const std::vector<Vec2>& ring, Vec2 p) noexcept {
    const std::size_t n = ring.size();
    if (n < 3) {
        return false;
    }
    bool inside = false;
    for (std::size_t i = 0, j = n - 1; i < n; j = i++) {
        const Vec2 a = ring[j];
        const Vec2 b = ring[i];
        if (Cross(a, b, p) == 0.0 && OnSegment(a, b, p)) {
            return true; // on the boundary
        }
        if ((a.y > p.y) != (b.y > p.y)) {
            const double x = a.x + (p.y - a.y) / (b.y - a.y) * (b.x - a.x);
            if (p.x < x) {
                inside = !inside;
            }
        }
    }
    return inside;
}

double PolygonArea(const std::vector<Vec2>& ring) noexcept {
    double twice = 0.0;
    for (std::size_t i = 0, n = ring.size(); i < n; ++i) {
        const Vec2 a = ring[i];
        const Vec2 b = ring[(i + 1) % n];
        twice += a.x * b.y - b.x * a.y;
    }
    return std::fabs(twice) * 0.5;
}

bool SegmentCrossesRingBoundary(Vec2 a, Vec2 b, const std::vector<Vec2>& ring) noexcept {
    for (std::size_t i = 0, n = ring.size(); i < n; ++i) {
        if (SegmentsIntersect(a, b, ring[i], ring[(i + 1) % n])) {
            return true;
        }
    }
    return false;
}

bool SegmentCrossesPolyline(Vec2 a, Vec2 b, const std::vector<Vec2>& line) noexcept {
    for (std::size_t i = 0; i + 1 < line.size(); ++i) {
        if (SegmentsIntersect(a, b, line[i], line[i + 1])) {
            return true;
        }
    }
    return false;
}

bool IsSimpleRing(const std::vector<Vec2>& ring) {
    const std::size_t n = ring.size();
    if (n < 3 || PolygonArea(ring) <= 0.0) {
        return false;
    }
    for (std::size_t i = 0; i < n; ++i) {
        const Vec2 a = ring[i];
        const Vec2 b = ring[(i + 1) % n];
        if (a == b) {
            return false;
        }
        for (std::size_t j = i + 1; j < n; ++j) {
            const bool adjacent = (j == i + 1) || (i == 0 && j == n - 1);
            if (adjacent) {
                continue;
            }
            if (SegmentsIntersect(a, b, ring[j], ring[(j + 1) % n])) {
                return false;
            }
        }
    }
    return true;
}

std::vector<Vec2> SimplifyPolyline(const std::vector<Vec2>& points, double tolerance) {
    if (points.size() < 3 || !std::isfinite(tolerance) || tolerance <= 0.0) {
        return points;
    }
    std::vector<bool> keep(points.size(), false);
    keep.front() = true;
    keep.back() = true;
    std::vector<std::pair<std::size_t, std::size_t>> stack;
    stack.emplace_back(0, points.size() - 1);
    while (!stack.empty()) {
        const auto [first, last] = stack.back();
        stack.pop_back();
        double worst = -1.0;
        std::size_t worstIndex = first;
        for (std::size_t i = first + 1; i < last; ++i) {
            const double d = ProjectPointToSegment(points[i], points[first], points[last]).distance;
            if (d > worst) {
                worst = d;
                worstIndex = i;
            }
        }
        if (worst > tolerance) {
            keep[worstIndex] = true;
            stack.emplace_back(first, worstIndex);
            stack.emplace_back(worstIndex, last);
        }
    }
    std::vector<Vec2> out;
    for (std::size_t i = 0; i < points.size(); ++i) {
        if (keep[i]) {
            out.push_back(points[i]);
        }
    }
    return out;
}

bool ClipSegmentToRect(Vec2 a, Vec2 b, const MapRect& rect, Vec2& outA, Vec2& outB) noexcept {
    double t0 = 0.0;
    double t1 = 1.0;
    const double dx = b.x - a.x;
    const double dy = b.y - a.y;
    const std::array<double, 4> p = {-dx, dx, -dy, dy};
    const std::array<double, 4> q = {a.x - rect.min.x, rect.max.x - a.x, a.y - rect.min.y, rect.max.y - a.y};
    for (std::size_t i = 0; i < 4; ++i) {
        if (p[i] == 0.0) {
            if (q[i] < 0.0) {
                return false;
            }
        } else {
            const double r = q[i] / p[i];
            if (p[i] < 0.0) {
                if (r > t1) { return false; }
                t0 = std::max(t0, r);
            } else {
                if (r < t0) { return false; }
                t1 = std::min(t1, r);
            }
        }
    }
    outA = t0 <= 0.0 ? a : Vec2{a.x + dx * t0, a.y + dy * t0};
    outB = t1 >= 1.0 ? b : Vec2{a.x + dx * t1, a.y + dy * t1};
    return true;
}

bool ClipSegmentToCircle(Vec2 a, Vec2 b, Vec2 centre, double radius, Vec2& outA, Vec2& outB) noexcept {
    const double dx = b.x - a.x;
    const double dy = b.y - a.y;
    const double fx = a.x - centre.x;
    const double fy = a.y - centre.y;
    const double quadA = dx * dx + dy * dy;
    const double quadC = fx * fx + fy * fy - radius * radius;
    if (quadA == 0.0) {
        if (quadC > 0.0) {
            return false;
        }
        outA = a;
        outB = b;
        return true;
    }
    const double quadB = 2.0 * (fx * dx + fy * dy);
    const double disc = quadB * quadB - 4.0 * quadA * quadC;
    if (disc < 0.0) {
        return false;
    }
    const double root = std::sqrt(disc);
    const double lo = (-quadB - root) / (2.0 * quadA);
    const double hi = (-quadB + root) / (2.0 * quadA);
    const double t0 = std::max(0.0, lo);
    const double t1 = std::min(1.0, hi);
    if (t0 > t1) {
        return false;
    }
    outA = t0 <= 0.0 ? a : Vec2{a.x + dx * t0, a.y + dy * t0};
    outB = t1 >= 1.0 ? b : Vec2{a.x + dx * t1, a.y + dy * t1};
    return true;
}

} // namespace spn::core
