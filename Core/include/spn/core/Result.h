#pragma once

#include <cassert>
#include <utility>
#include <variant>

namespace spn::core {

/// Minimal value-or-error type. The Core avoids exceptions (Unreal-friendly), and
/// std::expected is C++23. Precondition for Value()/Error(): check HasValue() first.
template <typename T, typename E> class [[nodiscard]] Result {
public:
    Result(T value) : data_(std::in_place_index<0>, std::move(value)) {}
    Result(E error) : data_(std::in_place_index<1>, std::move(error)) {}

    [[nodiscard]] bool HasValue() const noexcept { return data_.index() == 0; }
    [[nodiscard]] explicit operator bool() const noexcept { return HasValue(); }

    [[nodiscard]] T& Value() & {
        assert(HasValue());
        return std::get<0>(data_);
    }
    [[nodiscard]] const T& Value() const& {
        assert(HasValue());
        return std::get<0>(data_);
    }
    [[nodiscard]] T&& Value() && {
        assert(HasValue());
        return std::get<0>(std::move(data_));
    }
    [[nodiscard]] const E& Error() const& {
        assert(!HasValue());
        return std::get<1>(data_);
    }

private:
    std::variant<T, E> data_;
};

} // namespace spn::core
