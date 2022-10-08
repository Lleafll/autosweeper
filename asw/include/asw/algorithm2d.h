#pragma once

#include <type_traits>

namespace asw {

template<class T>
constexpr void for_each(T&& span2d, auto&& func) {
    static_assert(std::remove_cvref_t<T>::extents_type::rank() == 2);
    using size_t = typename T::size_type;
    for (size_t row = 0; row < span2d.extent(0); ++row) {
        for (size_t column = 0; column < span2d.extent(1); ++column) {
            func(span2d(row, column));
        }
    }
}

template<class T>
constexpr void indexed_for_each(T&& span2d, auto&& func) {
    static_assert(std::remove_cvref_t<T>::extents_type::rank() == 2);
    using size_t = typename std::remove_cvref_t<T>::size_type;
    for (size_t row = 0; row < span2d.extent(0); ++row) {
        for (size_t column = 0; column < span2d.extent(1); ++column) {
            func(row, column, span2d(row, column));
        }
    }
}

constexpr void indexed_transform(auto&& in, auto&& out, auto&& func) {
    using Input = std::remove_cvref_t<decltype(in)>;
    using Output = std::remove_cvref_t<decltype(out)>;
    static_assert(Input::extents_type::rank() == 2);
    static_assert(Output::extents_type::rank() == 2);
    using size_t = typename Input::size_type;
    indexed_for_each(
            in,
            [&out, &func](size_t const row, size_t const column, auto&& value) {
                out(row, column) = func(row, column, value);
            });
}

}  // namespace asw