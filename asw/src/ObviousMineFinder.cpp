#include "ObviousMineFinder.h"

namespace stdex = std::experimental;

namespace asw {

ObviousMineFinder::ObviousMineFinder(const std::size_t rows, const std::size_t columns)
    : rows_{rows},
      columns_{columns},
      buffer_(rows * columns, PlayingField::Cell::Hidden) {}

ObviousMineFinder::View ObviousMineFinder::view() {
    return View{buffer_.data(), rows_, columns_};
}

ObviousMineFinder::ConstView ObviousMineFinder::view() const {
    return ConstView{buffer_.data(), rows_, columns_};
}
namespace {

void fill_in_bombs(
        ObviousMineFinder::View& view,
        std::size_t const row,
        std::size_t const column) {
    auto const cell = view(row, column);
    if (cell == PlayingField::Cell::Empty || cell == PlayingField::Cell::Mine) {
        return;
    }
    auto const rows = view.extent(0);
    auto const columns = view.extent(1);
    auto const surrounding = stdex::submdspan(
            view,
            std::pair{
                    row == 0 ? row : row - 1,
                    row + 1 == rows ? rows : rows + 1},
            std::pair{
                    column == 0 ? column : column - 1,
                    column + 1 == columns ? columns : columns + 1});
    int potential_mines = 0;
    for (std::size_t r = 0; r < surrounding.extent(0); ++r) {
        for (std::size_t c = 0; c < surrounding.extent(1); ++c) {
            if (r == row && c == column) {
                continue;
            }
            auto const neighbor = surrounding(r, c);
            if (neighbor == PlayingField::Cell::Hidden ||
                neighbor == PlayingField::Cell::Mine) {
                ++potential_mines;
            }
        }
    }
    auto const mine_count =
            static_cast<std::underlying_type_t<PlayingField::Cell>>(cell);
    if (potential_mines == mine_count) {
        for (std::size_t r = 0; r < surrounding.extent(0); ++r) {
            for (std::size_t c = 0; c < surrounding.extent(1); ++c) {
                if (r == row && c == column) {
                    continue;
                }
                surrounding(r, c) = PlayingField::Cell::Mine;
            }
        }
    }
}

}  // namespace

ObviousMineFinder evaluate(PlayingField const& field) {
    auto const rows = field.rows();
    auto const columns = field.columns();
    ObviousMineFinder evaluation{field.rows(), field.columns()};
    auto view = evaluation.view();
    for (std::size_t row = 0; row < rows; ++row) {
        for (std::size_t column = 0; column < columns; ++column) {
            auto const cell = field(row, column);
            view(row, column) = cell;
        }
    }
    for (std::size_t row = 0; row < rows; ++row) {
        for (std::size_t column = 0; column < columns; ++column) {
            fill_in_bombs(view, row, column);
        }
    }
    return evaluation;
}

}  // namespace asw