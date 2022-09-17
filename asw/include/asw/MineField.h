#pragma once

#include <vector>

namespace asw {

class MineField final {
public:
    enum class Cell { Empty, Mine };

    MineField(int const rows, int const columns)
        : rows_{rows}, columns_{columns}, cells_(rows_ * columns_) {}

    [[nodiscard]] int rows() const { return rows_; }

    [[nodiscard]] int columns() const { return columns_; }

    [[nodiscard]] Cell& operator()(int const row, int const column) {
        return cells_[row + rows_ * column];
    }

    [[nodiscard]] Cell operator()(int const row, int const column) const {
        return cells_[row + rows_ * column];
    }

private:
    int rows_;
    int columns_;
    std::vector<Cell> cells_;
};

}  // namespace asw