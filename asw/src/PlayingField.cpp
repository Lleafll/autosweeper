#include "PlayingField.h"
#include "Vector2d.h"
#include "algorithm2d.h"
#include <algorithm>

namespace stdex = std::experimental;

namespace asw {

namespace {

template<class T>
struct Proximity final {
    std::size_t min_row;
    std::size_t min_column;
    T subspan;
};

auto proximity(
        ConstMineCellSpan const& mines,
        std::size_t const row,
        std::size_t const column) {
    auto const min_row = row == 0 ? 0 : row - 1;
    auto const max_row = row + 1 == mines.extent(0) ? row : row + 1;
    auto const min_column = column == 0 ? 0 : column - 1;
    auto const max_column = column + 1 == mines.extent(1) ? column : column + 1;
    auto const subspan = stdex::submdspan(
            mines,
            std::pair{min_row, max_row + 1},
            std::pair{min_column, max_column + 1});
    return Proximity<decltype(subspan)>{min_row, min_column, subspan};
}

Cell calculate_proximity(
        ConstMineCellSpan const& mines,
        std::size_t const row,
        std::size_t const column) {
    if (mines(row, column) == MineCell::Mined) {
        return Cell::Mine;
    }
    std::underlying_type_t<Cell> count = 0;
    auto const proximity_result = proximity(mines, row, column);
    indexed_for_each(
            proximity_result.subspan,
            [row, column, &count, &proximity_result](
                    std::size_t const r,
                    std::size_t const c,
                    MineCell const mine) {
                if (r + proximity_result.min_row == row and
                    c + proximity_result.min_column == column) {
                    return;
                }
                if (mine == MineCell::Mined) {
                    ++count;
                }
            });
    return static_cast<Cell>(count);
}

std::vector<Cell> calculate_cells(ConstMineCellSpan const& mines) {
    auto const rows = mines.extent(0);
    auto const columns = mines.extent(1);
    std::vector<Cell> cells(rows * columns);
    indexed_for_each(
            stdex::mdspan{cells.data(), rows, columns},
            [&mines](
                    std::size_t const row,
                    std::size_t const column,
                    Cell& cell) {
                cell = calculate_proximity(mines, row, column);
            });
    return cells;
}

}  // namespace

PlayingField::PlayingField(ConstMineCellSpan const& mines)
    : rows_{mines.extent(0)},
      columns_{mines.extent(1)},
      hidden_{rows_ * columns_, Cell::Hidden},
      cells_{calculate_cells(mines)} {
}

std::size_t PlayingField::rows() const {
    return rows_;
}

std::size_t PlayingField::columns() const {
    return columns_;
}

int PlayingField::mine_count() const {
    return static_cast<int>(std::ranges::count(cells_, Cell::Mine));
}

Cell PlayingField::operator()(std::size_t const row, std::size_t const column)
        const {
    return hidden()(row, column);
}

void PlayingField::reveal(std::size_t const row, std::size_t const column) {
    stdex::mdspan{hidden_.data(), rows_, columns_}(row, column) =
            cells()(row, column);
}

CellSpan PlayingField::span() {
    return stdex::mdspan{hidden_.data(), rows_, columns_};
}

ConstCellSpan PlayingField::cspan() const {
    return hidden();
}

ConstCellSpan PlayingField::hidden() const {
    return stdex::mdspan{hidden_.data(), rows_, columns_};
}

ConstCellSpan PlayingField::cells() const {
    return stdex::mdspan{cells_.data(), rows_, columns_};
}

}  // namespace asw