#include "InMemoryPlayingField.h"
#include "Vector2d.h"
#include "algorithm2d.h"
#include <algorithm>

namespace stdex = std::experimental;

namespace asw {

namespace {

template<class T>
struct Proximity final {
    size_t min_row;
    size_t min_column;
    T subspan;
};

auto proximity(ConstMineCellSpan const& mines, Position const& position) {
    auto const row = position.row;
    auto const column = position.column;
    auto const min_row = row == 0 ? 0 : row - 1;
    auto const max_row = row + 1 == mines.extent(0) ? row : row + 1;
    auto const min_column = column == 0 ? 0 : column - 1;
    auto const max_column = column + 1 == mines.extent(1) ? column : column + 1;
    auto const subspan = stdex::submdspan(
            mines,
            std::pair{min_row, max_row + 1},
            std::pair{min_column, max_column + 1});
    return Proximity<std::remove_cvref_t<decltype(subspan)>>{
            min_row, min_column, subspan};
}

Cell calculate_proximity(
        ConstMineCellSpan const& mines,
        Position const& position) {
    if (mines(position.row, position.column) == MineCell::Mined) {
        return Cell::Mine;
    }
    std::underlying_type_t<Cell> count = 0;
    auto const proximity_result = proximity(mines, position);
    indexed_for_each(
            proximity_result.subspan,
            [&position, &count, &proximity_result](
                    Position const& i, MineCell const mine) {
                if (i.row + proximity_result.min_row == position.row and
                    i.column + proximity_result.min_column == position.column) {
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
            [&mines](Position const& i, Cell& cell) {
                cell = calculate_proximity(mines, i);
            });
    return cells;
}

}  // namespace

InMemoryPlayingField::InMemoryPlayingField(ConstMineCellSpan const& mines)
    : rows_{mines.extent(0)},
      columns_{mines.extent(1)},
      hidden_{rows_ * columns_, Cell::Hidden},
      cells_{calculate_cells(mines)} {
}

size_t InMemoryPlayingField::rows() const {
    return rows_;
}

size_t InMemoryPlayingField::columns() const {
    return columns_;
}

int InMemoryPlayingField::mine_count() const {
    return static_cast<int>(std::ranges::count(cells_, Cell::Mine));
}

Cell InMemoryPlayingField::operator()(size_t const row, size_t const column)
        const {
    return hidden()(row, column);
}

void InMemoryPlayingField::reveal(Position const& position) {
    auto const mine_revealed =
            std::ranges::find(hidden_, Cell::Mine) != hidden_.cend();
    if (not mine_revealed) {
        stdex::mdspan{hidden_.data(), rows_, columns_}(
                position.row, position.column) =
                cells()(position.row, position.column);
    }
}

CellSpan InMemoryPlayingField::span() {
    return stdex::mdspan{hidden_.data(), rows_, columns_};
}

ConstCellSpan InMemoryPlayingField::cspan() const {
    return hidden();
}

ConstCellSpan InMemoryPlayingField::hidden() const {
    return stdex::mdspan{hidden_.data(), rows_, columns_};
}

ConstCellSpan InMemoryPlayingField::cells() const {
    return stdex::mdspan{cells_.data(), rows_, columns_};
}

}  // namespace asw