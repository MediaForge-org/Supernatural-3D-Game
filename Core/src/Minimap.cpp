#include <algorithm>
#include <cmath>
#include <numbers>
#include <spn/core/Minimap.h>

namespace spn::core {

namespace {
constexpr double kTwoPi = 2.0 * std::numbers::pi;
bool Finite(Vec2 v) { return std::isfinite(v.x) && std::isfinite(v.y); }
bool PositiveFinite(double v) { return std::isfinite(v) && v > 0.0; }
} // namespace

std::string_view ToString(MinimapErrorCode code) noexcept {
    switch (code) {
    case MinimapErrorCode::InvalidConfig: return "InvalidConfig";
    case MinimapErrorCode::InvalidView: return "InvalidView";
    case MinimapErrorCode::UnknownLayer: return "UnknownLayer";
    }
    return "Unknown";
}

double NormalizeAngle(double radians) noexcept {
    double r = std::fmod(radians, kTwoPi);
    if (r < 0.0) {
        r += kTwoPi;
    }
    // fmod of a tiny negative value plus 2*pi can round to exactly 2*pi.
    return r >= kTwoPi ? 0.0 : r;
}

std::optional<MinimapError> ValidateConfig(const MinimapConfig& c) {
    const bool sizes = PositiveFinite(c.halfWidthPx) && (c.shape == MinimapShape::Circle || PositiveFinite(c.halfHeightPx));
    const double smallest = c.shape == MinimapShape::Circle ? c.halfWidthPx : std::min(c.halfWidthPx, c.halfHeightPx);
    const bool ok = sizes && PositiveFinite(c.baseMetresPerPixel) && PositiveFinite(c.minZoom) && PositiveFinite(c.maxZoom) && c.minZoom <= c.maxZoom &&
                    std::isfinite(c.indicatorInsetPx) && c.indicatorInsetPx >= 0.0 && c.indicatorInsetPx < smallest &&
                    std::isfinite(c.offscreenIndicatorRangeMetres) && c.offscreenIndicatorRangeMetres >= 0.0;
    return ok ? std::nullopt : std::optional<MinimapError>(MinimapError{MinimapErrorCode::InvalidConfig});
}

Result<MinimapTransform, MinimapError> MinimapTransform::Create(const MinimapConfig& config, const MinimapView& view) {
    if (auto error = ValidateConfig(config)) {
        return *error;
    }
    if (!Finite(view.player.xy) || !std::isfinite(view.headingRadians) || !std::isfinite(view.zoom) || view.zoom <= 0.0) {
        return MinimapError{MinimapErrorCode::InvalidView};
    }
    MinimapTransform t;
    t.player_ = view.player.xy;
    t.layer_ = view.player.layer;
    t.zoom_ = std::clamp(view.zoom, config.minZoom, config.maxZoom);
    t.metresPerPixel_ = config.baseMetresPerPixel / t.zoom_;
    t.rotation_ = view.rotation == MinimapRotation::HeadingUp ? NormalizeAngle(view.headingRadians) : 0.0;
    t.sin_ = std::sin(t.rotation_);
    t.cos_ = std::cos(t.rotation_);
    return t;
}

Vec2 MinimapTransform::WorldToScreen(Vec2 world) const noexcept {
    const double dx = world.x - player_.x; // double subtraction first: precision at large coordinates
    const double dy = world.y - player_.y;
    // Components in the frame whose "up" is the bearing `rotation_`: forward = (sin, cos), right = (cos, -sin).
    const double right = dx * cos_ - dy * sin_;
    const double forward = dx * sin_ + dy * cos_;
    return Vec2{right / metresPerPixel_, -forward / metresPerPixel_};
}

Vec2 MinimapTransform::ScreenToWorld(Vec2 screen) const noexcept {
    const double right = screen.x * metresPerPixel_;
    const double forward = -screen.y * metresPerPixel_;
    return Vec2{player_.x + right * cos_ + forward * sin_, player_.y - right * sin_ + forward * cos_};
}

bool InsideViewport(const MinimapConfig& c, Vec2 s) noexcept {
    if (c.shape == MinimapShape::Circle) {
        const double limit = c.halfWidthPx + kViewportEpsilonPx;
        return s.x * s.x + s.y * s.y <= limit * limit;
    }
    return std::abs(s.x) <= c.halfWidthPx + kViewportEpsilonPx && std::abs(s.y) <= c.halfHeightPx + kViewportEpsilonPx;
}

Vec2 ProjectToViewportEdge(const MinimapConfig& c, Vec2 s, double inset) noexcept {
    const double length = std::hypot(s.x, s.y);
    if (length == 0.0) {
        return Vec2{0.0, -(c.shape == MinimapShape::Circle ? c.halfWidthPx : c.halfHeightPx) + inset};
    }
    if (c.shape == MinimapShape::Circle) {
        const double scale = (c.halfWidthPx - inset) / length;
        return Vec2{s.x * scale, s.y * scale};
    }
    const double hx = c.halfWidthPx - inset;
    const double hy = c.halfHeightPx - inset;
    const double tx = s.x != 0.0 ? hx / std::abs(s.x) : INFINITY;
    const double ty = s.y != 0.0 ? hy / std::abs(s.y) : INFINITY;
    const double scale = std::min(tx, ty);
    return Vec2{s.x * scale, s.y * scale};
}

double ViewportReachPx(const MinimapConfig& c) noexcept {
    return c.shape == MinimapShape::Circle ? c.halfWidthPx : std::hypot(c.halfWidthPx, c.halfHeightPx);
}

Result<MinimapFrame, MinimapError> BuildMinimapFrame(const WorldMapState& map, const MinimapView& view, const MinimapConfig& config, const MapFilter& filter) {
    Result<MinimapTransform, MinimapError> made = MinimapTransform::Create(config, view);
    if (!made) {
        return made.Error();
    }
    const MinimapTransform& transform = made.Value();
    if (map.Definition().FindLayer(view.player.layer) == nullptr) {
        return MinimapError{MinimapErrorCode::UnknownLayer};
    }

    // One radius query covers the viewport (circumscribed circle, valid for any rotation) and the indicator range.
    const double viewRadiusMetres = ViewportReachPx(config) * transform.MetresPerPixel() + 1.0e-6 * transform.MetresPerPixel();
    const double queryRadius = std::max(viewRadiusMetres, config.offscreenIndicatorRangeMetres);
    Result<std::vector<MapQueryHit>, MapError> hits = map.QueryRadius(view.player, queryRadius, filter);
    if (!hits) {
        return MinimapError{MinimapErrorCode::InvalidView};
    }

    MinimapFrame frame;
    frame.layer = transform.Layer();
    frame.metresPerPixel = transform.MetresPerPixel();
    frame.zoom = transform.Zoom();
    frame.rotationRadians = transform.RotationRadians();
    frame.northOnScreenRadians = transform.NorthOnScreenRadians();
    for (const MapQueryHit& hit : hits.Value()) {
        const Vec2 screen = transform.WorldToScreen(hit.position);
        if (!Finite(screen)) {
            ++frame.rejected;
            continue;
        }
        if (InsideViewport(config, screen)) {
            frame.markers.push_back(MinimapMarker{hit, screen, hit.distanceMetres});
        } else if (config.offscreenIndicatorRangeMetres > 0.0 && hit.distanceMetres <= config.offscreenIndicatorRangeMetres) {
            const double angle = NormalizeAngle(std::atan2(screen.x, -screen.y)); // clockwise from screen-up
            frame.indicators.push_back(MinimapIndicator{hit, ProjectToViewportEdge(config, screen, config.indicatorInsetPx), angle, hit.distanceMetres});
        }
    }
    return frame;
}

} // namespace spn::core
