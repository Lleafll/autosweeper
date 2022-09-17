#include "MineField.h"

namespace asw {

MineField::MineField(int const rows, int const columns)
    : rows_{rows}, columns_{columns}, cells_(rows_ * columns_) {}

MineField::Cell& MineField::operator()(int const row, int const column) {
    return cells_[row + rows_ * column];
}

MineField::Cell MineField::operator()(int const row, int const column) const {
    return cells_[row + rows_ * column];
}

}  // namespace asw