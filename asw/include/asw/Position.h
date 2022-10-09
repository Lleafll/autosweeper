#pragma once

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