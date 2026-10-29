#pragma once

// Minimap calculations: turns map-domain data (WorldMapState) and the player's pose into screen-space data for a renderer.
// It never draws anything and knows no UI toolkit.
//
// Conventions
//   World:   metres, x = east, y = north (see WorldMap.h).
//   Heading: compass bearing in radians, clockwise from north: 0 = north, pi/2 = east. Any finite value is accepted and normalized.
//   Screen:  pixels, origin = viewport centre, x to the right, y DOWN (screen convention). The player is always at (0, 0).
//   NorthUp: north is screen-up. HeadingUp: the player's heading is screen-up (the map rotates).
//   Precision: world offsets are subtracted in double BEFORE rotation and scaling, so a player at x = 5,000,000 m loses nothing
//   compared to one at the origin.
//
// ENGINEERING FOUNDATION (M2B1), not master canon. Defaults of MinimapConfig are placeholders so tests have a value; they are not
// tuned and not approved UI/gameplay values.

#include <cstddef>
#include <cstdint>
#include <optional>
#include <spn/core/Result.h>
#include <spn/core/Vec.h>
#include <spn/core/WorldMap.h>
#include <string_view>
#include <vector>

namespace spn::core {

enum class MinimapRotation : std::uint8_t { NorthUp, HeadingUp };
enum class MinimapShape : std::uint8_t { Circle, Rectangle };

/// Points within this many pixels outside the viewport edge still count as inside (rotation rounding at the exact boundary).
inline constexpr double kViewportEpsilonPx = 1.0e-6;

struct MinimapConfig {
    MinimapShape shape = MinimapShape::Circle;
    double halfWidthPx = 128.0;  ///< Circle: the radius. Rectangle: half the width.
    double halfHeightPx = 128.0; ///< Rectangle only.
    double baseMetresPerPixel = 1.0; ///< at zoom 1; effective = base / zoom
    double minZoom = 0.25;
    double maxZoom = 8.0;
    double indicatorInsetPx = 0.0;          ///< off-screen indicators sit this far inside the edge
    double offscreenIndicatorRangeMetres = 0.0; ///< 0 = no off-screen indicators; otherwise markers up to this distance get one
};

struct MinimapView {
    MapPosition player;
    double headingRadians = 0.0;
    double zoom = 1.0; ///< clamped to [minZoom, maxZoom]
    MinimapRotation rotation = MinimapRotation::NorthUp;
};

enum class MinimapErrorCode : std::uint8_t { InvalidConfig, InvalidView, UnknownLayer };
[[nodiscard]] std::string_view ToString(MinimapErrorCode code) noexcept;
struct MinimapError {
    MinimapErrorCode code;
    friend bool operator==(const MinimapError&, const MinimapError&) = default;
};

/// Validates the config: positive finite sizes and scale, 0 < minZoom <= maxZoom, 0 <= inset < smallest half size, range >= 0 and finite.
[[nodiscard]] std::optional<MinimapError> ValidateConfig(const MinimapConfig& config);

/// Normalizes any finite angle to [0, 2*pi).
[[nodiscard]] double NormalizeAngle(double radians) noexcept;

/// The world <-> screen mapping for one view. Cheap to build; rejects non-finite input.
class MinimapTransform {
public:
    [[nodiscard]] static Result<MinimapTransform, MinimapError> Create(const MinimapConfig& config, const MinimapView& view);

    [[nodiscard]] Vec2 WorldToScreen(Vec2 world) const noexcept;
    /// Inverse of WorldToScreen (exact up to rounding): e.g. to turn a tap on the minimap into a waypoint position.
    [[nodiscard]] Vec2 ScreenToWorld(Vec2 screen) const noexcept;
    [[nodiscard]] double MetresPerPixel() const noexcept { return metresPerPixel_; }
    [[nodiscard]] double Zoom() const noexcept { return zoom_; }
    /// Angle by which the world is rotated on screen: 0 for NorthUp, the normalized heading for HeadingUp.
    [[nodiscard]] double RotationRadians() const noexcept { return rotation_; }
    /// Screen direction of north as an angle clockwise from screen-up, for a compass needle (0 for NorthUp).
    [[nodiscard]] double NorthOnScreenRadians() const noexcept { return NormalizeAngle(-rotation_); }
    [[nodiscard]] Vec2 Player() const noexcept { return player_; }
    [[nodiscard]] MapLayerId Layer() const noexcept { return layer_; }

private:
    MinimapTransform() = default;
    Vec2 player_;
    MapLayerId layer_;
    double metresPerPixel_ = 1.0;
    double zoom_ = 1.0;
    double rotation_ = 0.0;
    double sin_ = 0.0;
    double cos_ = 1.0;
};

/// True if the screen point lies inside the viewport (inclusive edge, kViewportEpsilonPx tolerance).
[[nodiscard]] bool InsideViewport(const MinimapConfig& config, Vec2 screen) noexcept;
/// Point on the viewport edge, `inset` pixels inside, in the direction of `screen` from the centre. For a zero vector the
/// result is straight up. Precondition: config is valid.
[[nodiscard]] Vec2 ProjectToViewportEdge(const MinimapConfig& config, Vec2 screen, double inset) noexcept;
/// Largest distance from the centre to any viewport point, in pixels (circle: radius; rectangle: half diagonal).
[[nodiscard]] double ViewportReachPx(const MinimapConfig& config) noexcept;

struct MinimapMarker {
    MapQueryHit hit; ///< pointers valid until the map state is modified
    Vec2 screen;
    double distanceMetres = 0.0;
};

struct MinimapIndicator {
    MapQueryHit hit;
    Vec2 screen;             ///< on the viewport edge (inset applied)
    double angleRadians = 0; ///< direction from the centre, clockwise from screen-up, [0, 2*pi)
    double distanceMetres = 0.0;
};

struct MinimapFrame {
    MapLayerId layer;
    double metresPerPixel = 1.0;
    double zoom = 1.0;
    double rotationRadians = 0.0;
    double northOnScreenRadians = 0.0;
    std::vector<MinimapMarker> markers;       ///< inside the viewport, nearest first
    std::vector<MinimapIndicator> indicators; ///< outside the viewport but within the indicator range, nearest first
    std::size_t rejected = 0;                 ///< entries skipped because their screen position was not finite
};

/// Builds the frame for the player's layer (`view.player.layer`): one radius query on the map state (no global scan), then
/// classification of each hit. Layer selection = the layer of the player position; build another view to show another layer.
[[nodiscard]] Result<MinimapFrame, MinimapError> BuildMinimapFrame(const WorldMapState& map, const MinimapView& view, const MinimapConfig& config,
                                                                   const MapFilter& filter = {});

} // namespace spn::core
