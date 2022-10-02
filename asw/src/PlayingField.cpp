#include "PlayingField.h"
#include "algorithm2d.h"
#include <algorithm>

namespace stdex = std::experimental;

namespace asw {

namespace {

Cell calculate_proximity(
        PlayingField::Mines const& mines,
        std::size_t const row,
        std::size_t const column) {
    if (mines(row, column) == MineCell::Mine) {
        return Cell::Mine;
    }
    auto const min_row = row == 0 ? 0 : row - 1;
    auto const max_row = row + 1 == mines.extent(0) ? row : row + 1;
    auto const min_column = column == 0 ? 0 : column - 1;
    auto const max_column = column + 1 == mines.extent(1) ? column : column + 1;
    std::underlying_type_t<Cell> count = 0;
    for (std::size_t r = min_row; r <= max_row; ++r) {
        for (std::size_t c = min_column; c <= max_column; ++c) {
            if (r == row && c == column) {
                continue;
            }
            if (mines(r, c) == MineCell::Mine) {
                ++count;
            }
        }
    }
    return static_cast<Cell>(count);
}

gsl::strict_not_null<std::shared_ptr<std::vector<Cell>>>
calculate_cells(PlayingField::Mines const& mines) {
    auto const rows = mines.extent(0);
    auto const columns = mines.extent(1);
    auto cells = gsl::make_strict_not_null(
            std::make_shared<std::vector<Cell>>(rows * columns));
    indexed_for_each(
            stdex::mdspan{cells->data(), rows, columns},
            [&mines](
                    std::size_t const row,
                    std::size_t const column,
                    Cell& cell) {
                cell = calculate_proximity(mines, row, column);
            });
    return cells;
}

}  // namespace

PlayingField::PlayingField(Mines const& mines)
    : rows_{mines.extent(0)},
      columns_{mines.extent(1)},
      hidden_{rows_ * columns_, Cell::Hidden},
      cells_{calculate_cells(mines)} {}

std::size_t PlayingField::rows() const { return rows_; }

std::size_t PlayingField::columns() const { return columns_; }

int PlayingField::mine_count() const {
    return static_cast<int>(std::ranges::count(*cells_, Cell::Mine));
}

Cell PlayingField::operator()(std::size_t const row, std::size_t const column)
        const {
    return hidden()(row, column);
}

PlayingField
PlayingField::reveal(std::size_t const row, std::size_t const column) const {
    auto hidden = hidden_;
    stdex::mdspan{hidden.data(), rows_, columns_}(row, column) =
            cells()(row, column);
    return {rows_, columns_, std::move(hidden), cells_};
}

PlayingField::operator stdex::mdspan<Cell const, stdex::dextents<2>>() const {
    return hidden();
}

PlayingField::PlayingField(
        std::size_t const rows,
        std::size_t const columns,
        std::vector<Cell> hidden,
        gsl::strict_not_null<std::shared_ptr<std::vector<Cell> const>> cells)
    : rows_{rows},
      columns_{columns},
      hidden_{std::move(hidden)},
      cells_{std::move(cells)} {}

PlayingField::ConstCells PlayingField::hidden() const {
    return stdex::mdspan{hidden_.data(), rows_, columns_};
}

PlayingField::ConstCells PlayingField::cells() const {
    return stdex::mdspan{cells_->data(), rows_, columns_};
}

}  // namespace asw