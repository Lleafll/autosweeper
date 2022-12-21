#pragma once

#include "Array2d.h"
#include "Position.h"
#include "Vector2d.h"

namespace asw {

enum class MineCell { Clear, Mined };

using MineCellConstSpan = std::experimental::
        mdspan<MineCell const, std::experimental::dextents<size_t, 2>>;

class PositionGenerator {
  public:
    virtual ~PositionGenerator() = default;
    [[nodiscard]] virtual Position operator()(Size const& size) = 0;
};

Vector2d<MineCell>
generate_mines(Size const& size, int count, PositionGenerator&& generator);

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