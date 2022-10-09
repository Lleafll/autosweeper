#pragma once

#include "Array2d.h"

namespace asw {

enum class MineCell { Empty, Mine };

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

template<std::size_t rows, std::size_t columns>
using CellArray = Array2d<Cell, rows, columns>;

template<std::size_t rows, std::size_t columns>
using MineCellArray = Array2d<MineCell, rows, columns>;

}  // namespace asw