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

/// placeholder:feedback.door.disabled / placeholder:feedback.door.blocked. Disabled: the door's `.disabled` fact (or authored default) is set;
/// Use is Refused and nothing is written. Blocked is never produced by the handler (the engine side knows about obstructions);
/// the interaction system returns it, see MakeDoorObstructionRule.
[[nodiscard]] const ContentId& TestDoorDisabledFeedback();
[[nodiscard]] const ContentId& TestDoorBlockedFeedback();
inline constexpr std::string_view kTestDoorDisabledState = "disabled";
[[nodiscard]] std::string TestDoorDisabledKey(std::string_view id);

/// Sets the lock state of a door in the facts (the path keys, mechanisms and tests use; Use never changes the lock). Same validation as
/// every fact write; returns the fact error code on failure, nullopt on success (also when nothing changes).
[[nodiscard]] std::optional<FactError> SetTestDoorLocked(WorldFacts& facts, std::string_view id, bool locked, std::string_view cause);
[[nodiscard]] std::optional<FactError> SetTestDoorDisabled(WorldFacts& facts, std::string_view id, bool disabled, std::string_view cause);

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
