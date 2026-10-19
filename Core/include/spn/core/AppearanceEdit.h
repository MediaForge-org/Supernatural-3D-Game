#pragma once

// Edit operations on an AppearanceSpec: the engine-independent backbone of a future
// character editor (player and NPC). A UI only has to translate input into edits; every
// rule lives here and is testable on Fedora. No UI is implemented in this phase.
//
// ENGINEERING PROPOSAL, not master canon.

#include <cstddef>
#include <optional>
#include <spn/core/Appearance.h>
#include <spn/core/Result.h>
#include <variant>
#include <vector>

namespace spn::core {

struct SetParameter {
    ParameterId id;
    ParamValue value = 0;
};
/// Back to the catalog default (removes the explicit value). Accepted if already absent.
struct ResetParameter {
    ParameterId id;
};
struct SetBodyProvider {
    ContentId id;
};
struct SetHairStyle {
    ContentId id; // empty = no hair
};
struct SetFacialHairStyle {
    ContentId id; // empty = none
};
struct SetClothing {
    ClothingSlot slot = ClothingSlot::Torso;
    ContentId item; // empty = take the slot's item off
};
/// Inserts at `index` (default: on top). `index` may equal the current layer count.
struct AddLayer {
    AppearanceLayer layer;
    std::optional<std::size_t> index;
};
struct RemoveLayer {
    std::size_t index = 0;
};
/// Moves a layer so that it ends up at position `to`.
struct MoveLayer {
    std::size_t from = 0;
    std::size_t to = 0;
};
struct SetLayerOpacity {
    std::size_t index = 0;
    ParamValue opacity = kMaxLayerOpacity;
};

using AppearanceEdit =
    std::variant<SetParameter, ResetParameter, SetBodyProvider, SetHairStyle, SetFacialHairStyle,
                 SetClothing, AddLayer, RemoveLayer, MoveLayer, SetLayerOpacity>;

/// Applies one edit to a valid spec and returns the new spec. Atomic: the result is fully
/// validated against the catalog, otherwise the first problem is returned and nothing
/// changed. A bad layer index gives IndexOutOfRange. Changing the body provider fails
/// while parts remain that do not fit the new one (silently dropping them would lose
/// the user's work; a "convert" policy is OPEN).
[[nodiscard]] Result<AppearanceSpec, AppearanceError>
ApplyEdit(const AppearanceSpec& spec, const AppearanceCatalog& catalog, const AppearanceEdit& edit);

/// Spec + undo/redo history. Copies of small specs are the history (simple and robust).
/// The catalog is referenced, not owned: it must outlive the editor.
class AppearanceEditor {
public:
    static constexpr std::size_t kMaxHistory = 256;

    /// Fails with the first validation issue if `initial` is not valid for the catalog.
    [[nodiscard]] static Result<AppearanceEditor, AppearanceError>
    Create(const AppearanceCatalog& catalog, AppearanceSpec initial);

    /// Applies an edit. On error nothing changes. An edit that leaves the spec unchanged is
    /// accepted but adds no history entry. A successful edit clears the redo history.
    [[nodiscard]] std::optional<AppearanceError> Apply(const AppearanceEdit& edit);

    [[nodiscard]] bool CanUndo() const noexcept { return !undo_.empty(); }
    [[nodiscard]] bool CanRedo() const noexcept { return !redo_.empty(); }
    bool Undo();
    bool Redo();

    [[nodiscard]] const AppearanceSpec& Spec() const noexcept { return current_; }

private:
    AppearanceEditor(const AppearanceCatalog& catalog, AppearanceSpec initial)
        : catalog_(&catalog), current_(std::move(initial)) {}

    const AppearanceCatalog* catalog_;
    AppearanceSpec current_;
    std::vector<AppearanceSpec> undo_;
    std::vector<AppearanceSpec> redo_;
};

} // namespace spn::core
