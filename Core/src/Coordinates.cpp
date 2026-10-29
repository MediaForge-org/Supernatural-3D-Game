#include "spn/core/Coordinates.h"

#include <cmath>
#include <limits>

#include "spn/core/SpatialGrid.h"

namespace spn::core {

namespace {

constexpr double kPi = 3.14159265358979323846;

bool Finite(double v) noexcept { return std::isfinite(v); }
bool Finite(const Vec3& v) noexcept { return Finite(v.x) && Finite(v.y) && Finite(v.z); }

CoordinateError Err(CoordinateErrorCode code) { return CoordinateError{code}; }

// Largest UE value that still maps back into the valid world range for any valid origin: both bounds scaled to centimetres, doubled for the origin.
constexpr double kMaxUnrealHorizontalCm = 2.0 * kMaxMapCoordinateMetres * kCentimetresPerMetre;
constexpr double kMaxUnrealVerticalCm = 2.0 * kMaxWorldElevationMetres * kCentimetresPerMetre;

} // namespace

std::string_view ToString(CoordinateErrorCode code) noexcept {
    switch (code) {
    case CoordinateErrorCode::NonFinite: return "NonFinite";
    case CoordinateErrorCode::OutOfRange: return "OutOfRange";
    case CoordinateErrorCode::InvalidFrame: return "InvalidFrame";
    }
    return "Unknown";
}

std::optional<CoordinateError> ValidateWorldPoint(const WorldPoint& p) noexcept {
    if (!Finite(p)) return Err(CoordinateErrorCode::NonFinite);
    if (std::fabs(p.x) > kMaxMapCoordinateMetres || std::fabs(p.y) > kMaxMapCoordinateMetres || std::fabs(p.z) > kMaxWorldElevationMetres)
        return Err(CoordinateErrorCode::OutOfRange);
    return std::nullopt;
}

bool IsValidWorldPoint(const WorldPoint& p) noexcept { return !ValidateWorldPoint(p).has_value(); }

std::optional<CoordinateError> ValidateFrame(const UnrealFrame& frame) noexcept {
    if (ValidateWorldPoint(frame.originMetres)) return Err(CoordinateErrorCode::InvalidFrame);
    return std::nullopt;
}

// --- Unreal ---

Result<UnrealPoint, CoordinateError> WorldDeltaToUnreal(UnrealAxes axes, const Vec3& d) noexcept {
    if (!Finite(d)) return Err(CoordinateErrorCode::NonFinite);
    if (std::fabs(d.x) > 2.0 * kMaxMapCoordinateMetres || std::fabs(d.y) > 2.0 * kMaxMapCoordinateMetres || std::fabs(d.z) > 2.0 * kMaxWorldElevationMetres)
        return Err(CoordinateErrorCode::OutOfRange);
    const double e = d.x * kCentimetresPerMetre;
    const double n = d.y * kCentimetresPerMetre;
    const double u = d.z * kCentimetresPerMetre;
    switch (axes) {
    case UnrealAxes::EastSouthUp: return UnrealPoint{e, -n, u};
    case UnrealAxes::NorthEastUp: return UnrealPoint{n, e, u};
    }
    return Err(CoordinateErrorCode::InvalidFrame);
}

Result<Vec3, CoordinateError> UnrealDeltaToWorld(UnrealAxes axes, const UnrealPoint& d) noexcept {
    if (!Finite(d.x) || !Finite(d.y) || !Finite(d.z)) return Err(CoordinateErrorCode::NonFinite);
    if (std::fabs(d.x) > kMaxUnrealHorizontalCm || std::fabs(d.y) > kMaxUnrealHorizontalCm || std::fabs(d.z) > kMaxUnrealVerticalCm)
        return Err(CoordinateErrorCode::OutOfRange);
    const double x = d.x / kCentimetresPerMetre;
    const double y = d.y / kCentimetresPerMetre;
    const double z = d.z / kCentimetresPerMetre;
    switch (axes) {
    case UnrealAxes::EastSouthUp: return Vec3{x, -y, z};
    case UnrealAxes::NorthEastUp: return Vec3{y, x, z};
    }
    return Err(CoordinateErrorCode::InvalidFrame);
}

Result<UnrealPoint, CoordinateError> WorldToUnreal(const UnrealFrame& frame, const WorldPoint& world) noexcept {
    if (ValidateFrame(frame)) return Err(CoordinateErrorCode::InvalidFrame);
    if (const auto bad = ValidateWorldPoint(world)) return *bad;
    const Vec3 delta{world.x - frame.originMetres.x, world.y - frame.originMetres.y, world.z - frame.originMetres.z}; // in double, before scaling
    return WorldDeltaToUnreal(frame.axes, delta);
}

Result<WorldPoint, CoordinateError> UnrealToWorld(const UnrealFrame& frame, const UnrealPoint& unreal) noexcept {
    if (ValidateFrame(frame)) return Err(CoordinateErrorCode::InvalidFrame);
    const auto delta = UnrealDeltaToWorld(frame.axes, unreal);
    if (!delta) return delta.Error();
    const WorldPoint world{frame.originMetres.x + delta.Value().x, frame.originMetres.y + delta.Value().y, frame.originMetres.z + delta.Value().z};
    if (const auto bad = ValidateWorldPoint(world)) return *bad;
    return world;
}

// UE yaw is measured from UE +X towards UE +Y (clockwise seen from above). Bearing is clockwise from north.
//   NorthEastUp: +X = north, +Y = east        => yaw = bearing
//   EastSouthUp: +X = east,  +Y = south       => yaw = bearing - 90 degrees
namespace {
double WrapDegrees180(double deg) noexcept {
    double w = std::fmod(deg, 360.0);
    if (w > 180.0) w -= 360.0;
    if (w <= -180.0) w += 360.0;
    return w;
}
double YawOffsetDegrees(UnrealAxes axes) noexcept { return axes == UnrealAxes::EastSouthUp ? -90.0 : 0.0; }
} // namespace

Result<double, CoordinateError> BearingToUnrealYawDegrees(UnrealAxes axes, double bearingRadians) noexcept {
    if (!Finite(bearingRadians)) return Err(CoordinateErrorCode::NonFinite);
    return WrapDegrees180(bearingRadians * 180.0 / kPi + YawOffsetDegrees(axes));
}

Result<double, CoordinateError> UnrealYawDegreesToBearing(UnrealAxes axes, double yawDegrees) noexcept {
    if (!Finite(yawDegrees)) return Err(CoordinateErrorCode::NonFinite);
    return NormalizeAngle((yawDegrees - YawOffsetDegrees(axes)) * kPi / 180.0);
}

// --- local ---

Result<Vec3, CoordinateError> WorldToLocal(const LocalFrame& frame, const WorldPoint& world) noexcept {
    if (ValidateWorldPoint(frame.originMetres)) return Err(CoordinateErrorCode::InvalidFrame);
    if (const auto bad = ValidateWorldPoint(world)) return *bad;
    return Vec3{world.x - frame.originMetres.x, world.y - frame.originMetres.y, world.z - frame.originMetres.z};
}

Result<WorldPoint, CoordinateError> LocalToWorld(const LocalFrame& frame, const Vec3& local) noexcept {
    if (ValidateWorldPoint(frame.originMetres)) return Err(CoordinateErrorCode::InvalidFrame);
    if (!Finite(local)) return Err(CoordinateErrorCode::NonFinite);
    const WorldPoint world{frame.originMetres.x + local.x, frame.originMetres.y + local.y, frame.originMetres.z + local.z};
    if (const auto bad = ValidateWorldPoint(world)) return *bad;
    return world;
}

double Float32StepMetres(double magnitudeMetres) noexcept {
    if (!Finite(magnitudeMetres)) return std::numeric_limits<double>::infinity();
    const double m = std::fabs(magnitudeMetres);
    if (m > static_cast<double>(std::numeric_limits<float>::max())) return std::numeric_limits<double>::infinity();
    const float f = static_cast<float>(m);
    return static_cast<double>(std::nextafter(f, std::numeric_limits<float>::infinity())) - static_cast<double>(f);
}

// --- map / minimap ---

Result<MapPosition, CoordinateError> ToMapPosition(MapLayerId layer, const WorldPoint& world) noexcept {
    if (const auto bad = ValidateWorldPoint(world)) return *bad;
    return MapPosition{layer, Vec2{world.x, world.y}};
}

Result<WorldPoint, CoordinateError> FromMapPosition(const MapPosition& position, double elevationMetres) noexcept {
    const WorldPoint world{position.xy.x, position.xy.y, elevationMetres};
    if (const auto bad = ValidateWorldPoint(world)) return *bad;
    return world;
}

Result<Vec2, CoordinateError> WorldToMinimapScreen(const MinimapTransform& transform, const WorldPoint& world) noexcept {
    if (const auto bad = ValidateWorldPoint(world)) return *bad;
    const Vec2 s = transform.WorldToScreen(Vec2{world.x, world.y});
    if (!Finite(s.x) || !Finite(s.y)) return Err(CoordinateErrorCode::NonFinite);
    return s;
}

Result<WorldPoint, CoordinateError> MinimapScreenToWorld(const MinimapTransform& transform, Vec2 screen, double elevationMetres) noexcept {
    if (!Finite(screen.x) || !Finite(screen.y)) return Err(CoordinateErrorCode::NonFinite);
    const Vec2 w = transform.ScreenToWorld(screen);
    const WorldPoint world{w.x, w.y, elevationMetres};
    if (const auto bad = ValidateWorldPoint(world)) return *bad;
    return world;
}

} // namespace spn::core
