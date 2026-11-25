#pragma once

// Body collision profile (NightBuild06, Phase C): the gameplay collision capsule that belongs to a body of a given height. Pure rules,
// no engine code, nothing saved.
//
// Why: the visual body (USpnCharacterVisualComponent presets) can be any height, from short female to tall male profiles, while the
// capsule used to be one fixed size (35 / 90 cm = 180 cm). A 193 cm body then sticks 8-13 cm out of the capsule (Docs/Proposals/0017 §A),
// a 160 cm body is blocked where it visibly fits. This file derives a capsule from the body height, keeps it inside sane limits, and
// tells whether a body fits a passage - so every body profile gets the same rules instead of a per-body special case.
//
// Policy (deliberately conservative): mode Keep is the default and changes NOTHING. Whether the protagonist's capsule follows its body is a
// gameplay decision of the developer (0017 "Collision capsule (open point)"); this file only provides the mechanism. All numbers are
// PLACEHOLDERS (CLAUDE.md section 3), configurable per preset.

#include <cstdint>
#include <vector>

namespace spn::core {

/// Limits and defaults. The defaults describe today's capsule of the player blueprint (radius 35, half height 90, crouched 40).
struct BodyCollisionRules {
    float defaultRadiusCm = 35.f;
    float defaultHalfHeightCm = 90.f;
    float defaultCrouchedHalfHeightCm = 40.f;   ///< the engine's CharacterMovement default; crouched capsule = 80 cm
    float headClearanceCm = 2.f;                ///< capsule top above the top of the head in FromHeight mode
    float minHalfHeightCm = 60.f;               ///< 120 cm capsule: below this the movement tuning (step height, ledge bands) does not hold
    float maxHalfHeightCm = 125.f;              ///< 250 cm capsule
    float minRadiusCm = 20.f;
    float maxRadiusCm = 60.f;
};

enum class BodyCollisionMode : std::uint8_t {
    Keep,         ///< the default capsule, whatever the body height (today's behaviour)
    FromHeight,   ///< half height from the body height (+ clearance); radius stays the default; crouch scales with the half height
    Explicit,     ///< the given profile, clamped to the limits
};

struct BodyCollisionProfile {
    float radiusCm = 35.f;
    float halfHeightCm = 90.f;
    float crouchedHalfHeightCm = 40.f;
};

struct BodyCollisionRequest {
    BodyCollisionMode mode = BodyCollisionMode::Keep;
    float bodyHeightCm = 0.f;              ///< feet to the top of the head, standing straight; 0 = unknown
    BodyCollisionProfile explicitProfile;  ///< only for Explicit
};

enum class BodyCollisionIssue : std::uint8_t {
    NoBodyHeight,      ///< FromHeight without a body height: the default capsule is used
    HalfHeightClamped, ///< outside [min, max] half height
    RadiusClamped,     ///< outside [min, max] radius
    CrouchClamped,     ///< crouched half height below the radius or above the standing half height
};

struct BodyCollisionResult {
    BodyCollisionProfile profile;
    float halfHeightDeltaCm = 0.f;   ///< profile.halfHeightCm - rules.defaultHalfHeightCm (> 0: the capsule centre rises by this much)
    float headAboveCapsuleCm = 0.f;  ///< body top - capsule top (> 0: the head sticks out of the capsule); 0 when the body height is unknown
    bool differsFromDefault = false;
    std::vector<BodyCollisionIssue> issues;
};

/// Deterministic; never throws. Keep returns the default profile (and still reports headAboveCapsuleCm for the given body height).
[[nodiscard]] BodyCollisionResult ResolveBodyCollision(const BodyCollisionRequest& request, const BodyCollisionRules& rules = {});

/// Whether a capsule (and the body it carries) fits through an opening of the given clear height and width.
struct PassageFit {
    bool capsuleFitsStanding = false;   ///< 2 x half height <= clear height and 2 x radius <= clear width
    bool capsuleFitsCrouched = false;
    bool headClearsStanding = false;    ///< body height <= clear height (no visible head clipping through the frame); true if height unknown
    float headroomCm = 0.f;             ///< clear height - max(body height, capsule height)
};

[[nodiscard]] PassageFit FitPassage(const BodyCollisionProfile& profile, float bodyHeightCm, float clearHeightCm, float clearWidthCm) noexcept;

[[nodiscard]] const char* BodyCollisionIssueName(BodyCollisionIssue issue) noexcept;

} // namespace spn::core
