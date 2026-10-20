#pragma once

// Test door: the first interactable kind, a PLACEHOLDER used to prove the interaction
// system and WorldFacts persistence (Docs/Proposals/0004, section 3.3). It is not a
// design for doors in the game: no keys, lockpicking, sounds or breaking.
//
// State lives only in WorldFacts:
//   placeholder.door.<id>.open    bool  absent = authored default "open"   (else false)
//   placeholder.door.<id>.locked  bool  absent = authored default "locked" (else false)
// The placeholder. prefix marks the keys as non-final; renaming them later is a
// save-compatibility event (Docs/Decisions/0004), not a silent rename.

#include <optional>
#include <spn/core/ContentId.h>
#include <spn/core/Interaction.h>
#include <string>
#include <string_view>

namespace spn::core {

/// placeholder:interactable.door
[[nodiscard]] const ContentId& TestDoorKind();
/// placeholder:feedback.door.locked, reported with Refused when Use hits a locked door.
[[nodiscard]] const ContentId& TestDoorLockedFeedback();

/// Names of the authored defaults in InteractionDefaults::values (bool each).
inline constexpr std::string_view kTestDoorOpenState = "open";
inline constexpr std::string_view kTestDoorLockedState = "locked";

[[nodiscard]] std::string TestDoorOpenKey(std::string_view id);
[[nodiscard]] std::string TestDoorLockedKey(std::string_view id);

/// Handler: Use toggles `.open` on an unlocked door; on a locked door it is Refused
/// and nothing is written. A non-bool fact or default gives StateCorrupt.
[[nodiscard]] InteractionHandler MakeTestDoorHandler();

/// Registers MakeTestDoorHandler() under TestDoorKind().
[[nodiscard]] std::optional<InteractionError> RegisterTestDoor(InteractionRegistry& registry);

} // namespace spn::core
