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

#include <cstdint>
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

/// Names of the authored defaults in InteractionDefaults::values (bool each, except the key).
inline constexpr std::string_view kTestDoorOpenState = "open";
inline constexpr std::string_view kTestDoorLockedState = "locked";
inline constexpr std::string_view kTestDoorBrokenState = "broken";       ///< a fact too (`.broken`)
inline constexpr std::string_view kTestDoorBreakableState = "breakable"; ///< authored only: Force may break a locked door
inline constexpr std::string_view kTestDoorKeyState = "key";             ///< authored only, string: key id that unlocks it (empty = none)

// NightBuild04 (door states): Use opens/closes; on a locked door it unlocks when the key is held (fact placeholder.key.<id>.held) and
// leaves the door shut; Force breaks a locked, breakable door (`.broken` = true, `.locked` = false, `.open` = true, permanent).
// A disabled door (barricaded / jammed) refuses both verbs.
[[nodiscard]] const ContentId& TestDoorUnlockedFeedback();    ///< placeholder:feedback.door.unlocked (Use + key: Changed)
[[nodiscard]] const ContentId& TestDoorBrokenFeedback();      ///< placeholder:feedback.door.broken (Force: Changed; Refused when already broken)
[[nodiscard]] const ContentId& TestDoorUnbreakableFeedback(); ///< placeholder:feedback.door.unbreakable
[[nodiscard]] const ContentId& TestDoorNotLockedFeedback();   ///< placeholder:feedback.door.not_locked (Force on an unlocked/open door)
[[nodiscard]] std::string TestDoorBrokenKey(std::string_view id);

/// Key possession. A fact, so it saves with the WorldFacts like everything else; a key item in the inventory can set it later.
[[nodiscard]] std::string TestKeyHeldKey(std::string_view keyId);
[[nodiscard]] std::optional<FactError> SetTestKeyHeld(WorldFacts& facts, std::string_view keyId, bool held, std::string_view cause);
[[nodiscard]] bool IsTestKeyHeld(const WorldFacts& facts, std::string_view keyId);

/// One word for the state a player sees. Priority: Blocked (disabled) > Open > Locked > Broken (closed again after being broken) > Closed.
enum class TestDoorStatus : std::uint8_t { Closed, Open, Locked, Blocked, Broken };
[[nodiscard]] std::string_view ToString(TestDoorStatus status) noexcept;

struct TestDoorInfo {
    TestDoorStatus status = TestDoorStatus::Closed;
    bool open = false;
    bool locked = false;     ///< effective: false once broken
    bool disabled = false;
    bool broken = false;
    bool breakable = false;
    std::string keyId;       ///< authored key id, empty = none
    bool keyHeld = false;
};

/// Reads the door the same way the handler does (facts, then authored defaults). StateCorrupt on a wrong-typed fact or default.
[[nodiscard]] Result<TestDoorInfo, InteractionError> ReadTestDoorInfo(const WorldFacts& facts, std::string_view id,
                                                                      const InteractionDefaults& defaults);

[[nodiscard]] std::string TestDoorOpenKey(std::string_view id);
[[nodiscard]] std::string TestDoorLockedKey(std::string_view id);

/// Handler: Use toggles `.open` on an unlocked door; on a locked door it is Refused
/// and nothing is written. A non-bool fact or default gives StateCorrupt.
[[nodiscard]] InteractionHandler MakeTestDoorHandler();

/// Registers MakeTestDoorHandler() under TestDoorKind().
[[nodiscard]] std::optional<InteractionError> RegisterTestDoor(InteractionRegistry& registry);

} // namespace spn::core
