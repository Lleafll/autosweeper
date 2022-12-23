#pragma once

#include "Array2d.h"
#include "Position.h"
#include "Vector2d.h"

namespace asw {

enum class MineCell { Clear, Mined };

using MineCellConstSpan = std::experimental::
        mdspan<MineCell const, std::experimental::dextents<size_t, 2>>;

template<class T>
concept PositionGenerator = requires(T t) {
                                { t(Size{}) } -> std::convertible_to<Position>;
                            };

Vector2d<MineCell> generate_mines(
        Size const& size,
        int const count,
        PositionGenerator auto&& generator) {
    Vector2d<MineCell> mines{size};
    for (int i = 0; i < count; ++i) {
        auto const [row, column] = generator(size);
        mines(row, column) = MineCell::Mined;
    }
    return mines;
}

/**
 * Thin wrapper around generate_mines() with a random generator
 */
Vector2d<MineCell> generate_random_mines(Size const& size, int count);

enum class Cell {
    Empty,
    One,
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Hidden,
    Mine
};

using CellSpan =
        std::experimental::mdspan<Cell, std::experimental::dextents<size_t, 2>>;
using CellConstSpan = std::experimental::
        mdspan<Cell const, std::experimental::dextents<size_t, 2>>;

}  // namespace asw