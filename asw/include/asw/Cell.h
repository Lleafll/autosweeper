#pragma once

#include <array>
#include <experimental/mdspan>

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
using MutCellSpan =
        std::experimental::mdspan<Cell, std::experimental::dextents<2>>;

template<std::size_t rows, std::size_t columns>
class CellArray final {
    static constexpr auto size = rows * columns;

public:
    constexpr explicit CellArray(Cell fill = Cell::Empty) : buffer_{} {
    }

    constexpr CellArray(std::initializer_list<Cell> const& cells) {
        std::copy_n(cells.begin(), size, buffer_.begin());
    }

    constexpr Cell&
    operator()(std::size_t const row, std::size_t const column) {
        return mut_span()(row, column);
    }

    [[nodiscard]] constexpr Cell
    operator()(std::size_t const row, std::size_t const column) const {
        return const_span()(row, column);
    }

    [[nodiscard]] constexpr MutCellSpan mut_span() {
        return MutCellSpan{buffer_.data(), rows, columns};
    }

    [[nodiscard]] constexpr ConstCellSpan const_span() const {
        return ConstCellSpan{buffer_.data(), rows, columns};
    }

private:
    std::array<Cell, size> buffer_ = {};
};

}  // namespace asw