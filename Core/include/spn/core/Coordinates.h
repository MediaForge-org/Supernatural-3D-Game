#pragma once

// Canonical world coordinates and the adapters to Unreal, map, minimap and local frames (M2B2).
//
// CANONICAL FRAME (what every Core system already uses; nothing is reinterpreted):
//   unit      metres, double precision
//   x         east-positive      (Vec3::x, Vec2::x)
//   y         north-positive     (Vec3::y, Vec2::y)
//   z         up-positive        elevation in metres (Vec3::z); the horizontal map layers (surface, underground, interior floors, realms) are
//             independent planes, so a layer is NOT an elevation: z is the height inside the layer's own plane
//   origin    none is baked in. The canonical frame has no offset; every origin is an explicit, configurable parameter of an adapter
//   bearings  compass bearing, clockwise from north, radians (as Minimap heading)
// The axis mapping to Unreal and WorldForge is a PROPOSAL (the master lists it as open: WF-DC01 / CQ-101). It is a configuration value
// (UnrealFrame), never a constant of the canonical model, so deciding differently later changes data, not code.

#include <cstdint>
#include <optional>
#include <string_view>

#include "spn/core/Minimap.h"
#include "spn/core/Result.h"
#include "spn/core/Vec.h"
#include "spn/core/WorldMap.h"

namespace spn::core {

inline constexpr double kCentimetresPerMetre = 100.0;
/// Sanity bound for |z|, in metres. Not a game rule: it only keeps arithmetic exact and rejects garbage (a corrupt save, a NaN-adjacent value).
/// Horizontal bound: kMaxMapCoordinateMetres (SpatialGrid.h).
inline constexpr double kMaxWorldElevationMetres = 100'000.0;

enum class CoordinateErrorCode : std::uint8_t {
    NonFinite,    ///< NaN or infinity
    OutOfRange,   ///< beyond kMaxMapCoordinateMetres horizontally or kMaxWorldElevationMetres vertically
    InvalidFrame, ///< a frame parameter (origin) is itself invalid
};
[[nodiscard]] std::string_view ToString(CoordinateErrorCode code) noexcept;
struct CoordinateError {
    CoordinateErrorCode code;
    friend bool operator==(const CoordinateError&, const CoordinateError&) = default;
};

/// A canonical position: x east, y north, z up, metres.
using WorldPoint = Vec3;
/// True if all components are finite and within the horizontal / vertical bounds.
[[nodiscard]] bool IsValidWorldPoint(const WorldPoint& p) noexcept;
/// The first problem of the point, if any.
[[nodiscard]] std::optional<CoordinateError> ValidateWorldPoint(const WorldPoint& p) noexcept;

// --- Unreal ------------------------------------------------------------------------------------------------------------------------------
// Unreal is left-handed, X forward, Y right, Z up, centimetres. Both presets keep that handedness and the up axis; they differ in which
// compass direction UE +X is. Neither is approved: the choice belongs to the developer together with WorldForge.

enum class UnrealAxes : std::uint8_t {
    EastSouthUp, ///< UE +X = east,  UE +Y = south (= -north), UE +Z = up.  Heading east = UE yaw 0.
    NorthEastUp, ///< UE +X = north, UE +Y = east,             UE +Z = up.  Heading north = UE yaw 0.
};

struct UnrealFrame {
    UnrealAxes axes = UnrealAxes::EastSouthUp;
    /// The canonical point (metres) that sits at UE (0, 0, 0). Zero = the canonical origin; any other value is an explicit choice.
    WorldPoint originMetres{};
    friend bool operator==(const UnrealFrame&, const UnrealFrame&) = default;
};

/// A position or offset in Unreal units (centimetres, UE axes). Plain doubles: UE5 FVector is double precision too.
struct UnrealPoint {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
    friend bool operator==(const UnrealPoint&, const UnrealPoint&) = default;
};

/// The frame is valid if its origin is a valid world point.
[[nodiscard]] std::optional<CoordinateError> ValidateFrame(const UnrealFrame& frame) noexcept;

/// Position: subtract the origin in double (metres), permute/flip the axes, multiply by 100. The input must be a valid world point.
[[nodiscard]] Result<UnrealPoint, CoordinateError> WorldToUnreal(const UnrealFrame& frame, const WorldPoint& world) noexcept;
/// Exact inverse of WorldToUnreal (up to double rounding). The result must be a valid world point.
[[nodiscard]] Result<WorldPoint, CoordinateError> UnrealToWorld(const UnrealFrame& frame, const UnrealPoint& unreal) noexcept;

/// Offsets/velocities (metres) <-> UE centimetres: axes and scale, no origin. Both must be finite.
[[nodiscard]] Result<UnrealPoint, CoordinateError> WorldDeltaToUnreal(UnrealAxes axes, const Vec3& deltaMetres) noexcept;
[[nodiscard]] Result<Vec3, CoordinateError> UnrealDeltaToWorld(UnrealAxes axes, const UnrealPoint& deltaCentimetres) noexcept;

/// Compass bearing (clockwise from north, radians, any finite value) -> UE yaw in degrees in (-180, 180]; and back to [0, 2*pi).
/// Non-finite input yields NonFinite.
[[nodiscard]] Result<double, CoordinateError> BearingToUnrealYawDegrees(UnrealAxes axes, double bearingRadians) noexcept;
[[nodiscard]] Result<double, CoordinateError> UnrealYawDegreesToBearing(UnrealAxes axes, double yawDegrees) noexcept;

// --- local (region-relative) frames -------------------------------------------------------------------------------------------------------
// A translation-only frame with canonical axes: local = world - origin, subtracted in double. Rotated frames are not provided (a rotation
// would be an arbitrary choice); a region that needs one can compose it on top.

struct LocalFrame {
    WorldPoint originMetres{};
    friend bool operator==(const LocalFrame&, const LocalFrame&) = default;
};
[[nodiscard]] Result<Vec3, CoordinateError> WorldToLocal(const LocalFrame& frame, const WorldPoint& world) noexcept;
[[nodiscard]] Result<WorldPoint, CoordinateError> LocalToWorld(const LocalFrame& frame, const Vec3& local) noexcept;

/// Spacing between neighbouring float32 values at `magnitudeMetres`: the worst-case representation step (half of it is the rounding error)
/// of a coordinate that is stored as float at that distance from its origin. Use it to decide a local frame is needed: at 10,000 km from the
/// origin the step is 1 m, within a 1 km local frame it is below 0.1 mm.
[[nodiscard]] double Float32StepMetres(double magnitudeMetres) noexcept;

// --- map and minimap ----------------------------------------------------------------------------------------------------------------------

/// Canonical point + layer -> map position (drops z). Fails if the point is invalid.
[[nodiscard]] Result<MapPosition, CoordinateError> ToMapPosition(MapLayerId layer, const WorldPoint& world) noexcept;
/// Map position + elevation inside the layer -> canonical point.
[[nodiscard]] Result<WorldPoint, CoordinateError> FromMapPosition(const MapPosition& position, double elevationMetres = 0.0) noexcept;

/// World point -> minimap pixel (screen space: y down, origin at the viewport centre; see Minimap.h). z is ignored.
[[nodiscard]] Result<Vec2, CoordinateError> WorldToMinimapScreen(const MinimapTransform& transform, const WorldPoint& world) noexcept;
/// Minimap pixel -> world point on the transform's plane (z = elevationMetres).
[[nodiscard]] Result<WorldPoint, CoordinateError> MinimapScreenToWorld(const MinimapTransform& transform, Vec2 screen, double elevationMetres = 0.0) noexcept;

} // namespace spn::core
