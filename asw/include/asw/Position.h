#pragma once

#include <functional>

namespace asw {

struct Position final {
    size_t row;
    size_t column;

    constexpr bool operator==(Position const&) const = default;
};

}  // namespace asw

template<>
struct std::hash<asw::Position> {
    size_t operator()(asw::Position const& position) const {
        return (std::hash<size_t>{}(position.row)) ^
               (std::hash<size_t>{}(position.column) << 1);
    }
};
