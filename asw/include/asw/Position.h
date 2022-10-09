#pragma once

#include <functional>

namespace asw {

struct Position final {
    std::size_t row;
    std::size_t column;

    constexpr Position(std::size_t const row, std::size_t const column)
        : row{row},
          column{column} {
    }

    constexpr bool operator==(Position const&) const = default;
};

}  // namespace asw

template<>
struct std::hash<asw::Position> {
    std::size_t operator()(asw::Position const& position) const {
        return (std::hash<std::size_t>{}(position.row)) ^
               (std::hash<std::size_t>{}(position.column) << 1);
    }
};
