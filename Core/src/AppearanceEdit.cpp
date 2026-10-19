#include <algorithm>
#include <spn/core/AppearanceEdit.h>
#include <type_traits>
#include <utility>

namespace spn::core {

namespace {

AppearanceError IndexError(std::size_t index) {
    return AppearanceError{AppearanceErrorCode::IndexOutOfRange, std::to_string(index), 0};
}

/// Applies the edit to `spec` in place; returns an error for bad indices only.
std::optional<AppearanceError> Mutate(AppearanceSpec& spec, const AppearanceEdit& edit) {
    return std::visit(
        [&spec](const auto& e) -> std::optional<AppearanceError> {
            using T = std::decay_t<decltype(e)>;
            if constexpr (std::is_same_v<T, SetParameter>) {
                spec.parameters.insert_or_assign(e.id, e.value);
            } else if constexpr (std::is_same_v<T, ResetParameter>) {
                spec.parameters.erase(e.id);
            } else if constexpr (std::is_same_v<T, SetBodyProvider>) {
                spec.bodyProvider = e.id;
            } else if constexpr (std::is_same_v<T, SetHairStyle>) {
                spec.hairStyle = e.id;
            } else if constexpr (std::is_same_v<T, SetFacialHairStyle>) {
                spec.facialHairStyle = e.id;
            } else if constexpr (std::is_same_v<T, SetClothing>) {
                if (e.item.IsEmpty()) {
                    spec.clothing.erase(e.slot);
                } else {
                    spec.clothing.insert_or_assign(e.slot, e.item);
                }
            } else if constexpr (std::is_same_v<T, AddLayer>) {
                const std::size_t index = e.index.value_or(spec.layers.size());
                if (index > spec.layers.size()) {
                    return IndexError(index);
                }
                spec.layers.insert(spec.layers.begin() + static_cast<std::ptrdiff_t>(index),
                                   e.layer);
            } else if constexpr (std::is_same_v<T, RemoveLayer>) {
                if (e.index >= spec.layers.size()) {
                    return IndexError(e.index);
                }
                spec.layers.erase(spec.layers.begin() + static_cast<std::ptrdiff_t>(e.index));
            } else if constexpr (std::is_same_v<T, MoveLayer>) {
                if (e.from >= spec.layers.size()) {
                    return IndexError(e.from);
                }
                if (e.to >= spec.layers.size()) {
                    return IndexError(e.to);
                }
                const auto first = spec.layers.begin();
                if (e.from < e.to) {
                    std::rotate(first + static_cast<std::ptrdiff_t>(e.from),
                                first + static_cast<std::ptrdiff_t>(e.from) + 1,
                                first + static_cast<std::ptrdiff_t>(e.to) + 1);
                } else {
                    std::rotate(first + static_cast<std::ptrdiff_t>(e.to),
                                first + static_cast<std::ptrdiff_t>(e.from),
                                first + static_cast<std::ptrdiff_t>(e.from) + 1);
                }
            } else {
                static_assert(std::is_same_v<T, SetLayerOpacity>);
                if (e.index >= spec.layers.size()) {
                    return IndexError(e.index);
                }
                spec.layers[e.index].opacity = e.opacity;
            }
            return std::nullopt;
        },
        edit);
}

} // namespace

Result<AppearanceSpec, AppearanceError> ApplyEdit(const AppearanceSpec& spec,
                                                  const AppearanceCatalog& catalog,
                                                  const AppearanceEdit& edit) {
    AppearanceSpec updated = spec;
    if (auto error = Mutate(updated, edit)) {
        return std::move(*error);
    }
    if (auto issues = ValidateAppearance(updated, catalog); !issues.empty()) {
        return std::move(issues.front());
    }
    return updated;
}

Result<AppearanceEditor, AppearanceError> AppearanceEditor::Create(const AppearanceCatalog& catalog,
                                                                   AppearanceSpec initial) {
    if (auto issues = ValidateAppearance(initial, catalog); !issues.empty()) {
        return std::move(issues.front());
    }
    return AppearanceEditor(catalog, std::move(initial));
}

std::optional<AppearanceError> AppearanceEditor::Apply(const AppearanceEdit& edit) {
    auto updated = ApplyEdit(current_, *catalog_, edit);
    if (!updated) {
        return updated.Error();
    }
    if (updated.Value() == current_) {
        return std::nullopt;
    }
    if (undo_.size() >= kMaxHistory) {
        undo_.erase(undo_.begin());
    }
    undo_.push_back(std::move(current_));
    current_ = std::move(updated).Value();
    redo_.clear();
    return std::nullopt;
}

bool AppearanceEditor::Undo() {
    if (undo_.empty()) {
        return false;
    }
    redo_.push_back(std::move(current_));
    current_ = std::move(undo_.back());
    undo_.pop_back();
    return true;
}

bool AppearanceEditor::Redo() {
    if (redo_.empty()) {
        return false;
    }
    undo_.push_back(std::move(current_));
    current_ = std::move(redo_.back());
    redo_.pop_back();
    return true;
}

} // namespace spn::core
