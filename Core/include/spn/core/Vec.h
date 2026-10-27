#pragma once

// Plain 2D / 3D vectors for the engine-independent core. Units are metres unless a function says otherwise; the Unreal adapters
// convert from/to centimetres and FVector. One definition for steering, NPC and interaction code (they used three copies).

namespace spn::core {

struct Vec2 {
    double x = 0.0;
    double y = 0.0;
    friend bool operator==(const Vec2&, const Vec2&) = default;
};

struct Vec3 {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
    friend bool operator==(const Vec3&, const Vec3&) = default;
};

} // namespace spn::core
