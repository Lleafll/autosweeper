#include "PlayingField.h"
#include <ranges>

namespace stdv = std::views;

namespace asw {

namespace {

PlayingField::Cell calculate_proximity(
        MineField const& mines,
        std::size_t const row,
        std::size_t const column) {
    if (mines(row, column) == MineField::Cell::Mine) {
        return PlayingField::Cell::Mine;
    }
    auto const min_row = row == 0 ? 0 : row - 1;
    auto const max_row = row + 1 == mines.rows() ? row : row + 1;
    auto const min_column = column == 0 ? 0 : column - 1;
    auto const max_column = column + 1 == mines.columns() ? column : column + 1;
    std::underlying_type_t<PlayingField::Cell> count = 0;
    for (std::size_t r = min_row; r <= max_row; ++r) {
        for (std::size_t c = min_column; c <= max_column; ++c) {
            if (r == row && c == column) {
                continue;
            }
            if(mines(r, c) == MineField::Cell::Mine) {
                ++count;
            }
        }
    }
    return static_cast<PlayingField::Cell>(count);
}

std::vector<PlayingField::Cell> calculate_cells(MineField const& mines) {
    auto const rows = mines.rows();
    auto const columns = mines.columns();
    std::vector<PlayingField::Cell> cells(rows * columns);
    for (std::size_t row = 0; row < rows; ++row) {
        for (std::size_t column = 0; column < columns; ++column) {
            cells[row + rows * column] =
                    calculate_proximity(mines, row, column);
        }
    }
    return cells;
}

}  // namespace

PlayingField::PlayingField(MineField const& mines)
    : rows_{mines.rows()},
      hidden_{mines.rows() * mines.columns(), Hidden::Yes},
      cells_{calculate_cells(mines)} {}

PlayingField::Cell PlayingField::operator()(
        std::size_t const row,
        std::size_t const column) const {
    if (is_hidden(row, column)) {
        return Cell::Hidden;
    }
    return cells_[row + rows_ * column];
}

void PlayingField::reveal(std::size_t const row, std::size_t const column) {
    hidden_[row + rows_ * column] = Hidden::No;
}

bool PlayingField::is_hidden(std::size_t const row, std::size_t const column)
        const {
    return hidden_[row + rows_ * column] == Hidden::Yes;
}

}  // namespace asw