#pragma once

#include "Array2d.h"
#include "Position.h"
#include "Vector2d.h"

namespace asw {

enum class MineCell { Empty, Mine };

using ConstMineCellSpan = std::experimental::
        mdspan<MineCell const, std::experimental::dextents<2>>;

class PositionGenerator {
  public:
    virtual ~PositionGenerator() = default;
    [[nodiscard]] virtual Position
    operator()(std::size_t rows, std::size_t columns) = 0;
};

Vector2d<MineCell> generate_mines(
        std::size_t rows,
        std::size_t columns,
        int count,
        PositionGenerator&& generator);

/**
 * Thin wrapper around generate_mines() with a random generator
 */
Vector2d<MineCell>
generate_random_mines(std::size_t rows, std::size_t columns, int count);

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

using ConstCellSpan =
        std::experimental::mdspan<Cell const, std::experimental::dextents<2>>;

}  // namespace asw