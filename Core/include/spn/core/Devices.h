#pragma once

// Simple interactive devices on top of the interaction registry and WorldFacts: switch, button, lever. PLACEHOLDER kinds for the
// gameplay foundation (like the test door): no sounds, animation or power model. State lives only in WorldFacts:
//
//   placeholder.switch.<id>.on        bool   absent = authored default "on" (else false)
//   placeholder.switch.<id>.disabled  bool   absent = authored default "disabled" (else false)
//   placeholder.button.<id>.presses   int    absent = 0
//   placeholder.button.<id>.disabled  bool
//   placeholder.lever.<id>.position   int    absent = authored default "position" (else 0), always in [0, positions)
//   placeholder.lever.<id>.disabled   bool
//
// Authored configuration (InteractionDefaults, never written back): switch "on"/"disabled"; button "disabled", "max_presses"
// (int, 0 = unlimited); lever "position", "positions" (int 2..16, default 2), "disabled".
// Use: switch toggles; button counts a press (exhausted at max_presses -> Refused); lever advances to the next position and wraps.
// A disabled device is Refused with its disabled feedback and nothing is written.

#include <optional>
#include <spn/core/ContentId.h>
#include <spn/core/Interaction.h>
#include <string>
#include <string_view>

namespace spn::core {

inline constexpr std::int64_t kMinLeverPositions = 2;
inline constexpr std::int64_t kMaxLeverPositions = 16;

[[nodiscard]] const ContentId& SwitchKind();   // placeholder:interactable.switch
[[nodiscard]] const ContentId& ButtonKind();   // placeholder:interactable.button
[[nodiscard]] const ContentId& LeverKind();    // placeholder:interactable.lever
[[nodiscard]] const ContentId& DeviceDisabledFeedback(); // placeholder:feedback.device.disabled
[[nodiscard]] const ContentId& ButtonExhaustedFeedback(); // placeholder:feedback.button.exhausted

[[nodiscard]] std::string SwitchOnKey(std::string_view id);
[[nodiscard]] std::string SwitchDisabledKey(std::string_view id);
[[nodiscard]] std::string ButtonPressesKey(std::string_view id);
[[nodiscard]] std::string ButtonDisabledKey(std::string_view id);
[[nodiscard]] std::string LeverPositionKey(std::string_view id);
[[nodiscard]] std::string LeverDisabledKey(std::string_view id);

[[nodiscard]] InteractionHandler MakeSwitchHandler();
[[nodiscard]] InteractionHandler MakeButtonHandler();
[[nodiscard]] InteractionHandler MakeLeverHandler();

/// Registers the three device kinds; the first error is returned (kinds already registered are errors, as everywhere).
[[nodiscard]] std::optional<InteractionError> RegisterDevices(InteractionRegistry& registry);

} // namespace spn::core
