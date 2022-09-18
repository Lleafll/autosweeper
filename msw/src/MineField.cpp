#include "MineField.h"

namespace msw {

MineField::MineField(std::size_t const rows, std::size_t const columns)
    : rows_{rows}, columns_{columns}, cells_(rows_ * columns_) {}


MineField::MineField(
        std::size_t const rows,
        std::size_t const columns,
        std::vector<MinePosition> const& positions)
    : MineField{rows, columns} {
    for (auto const& position: positions) {
        (*this)(position.row, position.column) = Cell::Mine;
    }
}

MineField::Cell&
MineField::operator()(std::size_t const row, std::size_t const column) {
    return cells_[row + rows_ * column];
}

MineField::Cell
MineField::operator()(std::size_t const row, std::size_t const column) const {
    return cells_[row + rows_ * column];
}

}  // namespace msw