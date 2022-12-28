#pragma once

#include "Position.h"
#include <type_traits>

namespace asw {

template<class T>
constexpr void indexed_for_each(T&& span2d, auto&& func) {
    static_assert(std::remove_cvref_t<T>::extents_type::rank() == 2);
    using size_t = typename std::remove_cvref_t<T>::size_type;
    for (size_t row = 0; row < span2d.extent(0); ++row) {
        for (size_t column = 0; column < span2d.extent(1); ++column) {
            func(Position{.row = row, .column = column}, span2d(row, column));
        }
    }
}

constexpr void indexed_transform(auto&& in, auto&& out, auto&& func) {
    using Input = std::remove_cvref_t<decltype(in)>;
    using Output = std::remove_cvref_t<decltype(out)>;
    static_assert(Input::extents_type::rank() == 2);
    static_assert(Output::extents_type::rank() == 2);
    indexed_for_each(in, [&out, &func](Position const& i, auto&& value) {
        out(i.row, i.column) = func(i, value);
    });
}

constexpr bool equals(auto&& lhs, auto&& rhs) {
    using Lhs = std::remove_cvref_t<decltype(lhs)>;
    using Rhs = std::remove_cvref_t<decltype(rhs)>;
    static_assert(Lhs::extents_type::rank() == 2);
    static_assert(Rhs::extents_type::rank() == 2);
    if (lhs.extent(0) != rhs.extent(0)) {
        return false;
    }
    if (lhs.extent(1) != rhs.extent(1)) {
        return false;
    }
    for (size_t row = 0; row < lhs.extent(0); ++row) {
        for (size_t column = 0; column < lhs.extent(1); ++column) {
            if (lhs(row, column) != rhs(row, column)) {
                return false;
            }
        }
    }
    return true;
}

}  // namespace asw