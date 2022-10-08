#pragma once

#include "MdArray.h"

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
using CellArray = MdArray<Cell, rows, columns>;

template<std::size_t rows, std::size_t columns>
using MineCellArray = MdArray<MineCell, rows, columns>;

}  // namespace asw