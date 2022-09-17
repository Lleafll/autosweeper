#pragma once

#include <vector>

namespace asw {

class MineField final {
public:
    enum class Cell { Empty, Mine };

    MineField(int rows, int columns);

    [[nodiscard]] inline int rows() const { return rows_; }
    [[nodiscard]] inline int columns() const { return columns_; }
    [[nodiscard]] Cell& operator()(int row, int column);
    [[nodiscard]] Cell operator()(int row, int column) const;

private:
    int rows_;
    int columns_;
    std::vector<Cell> cells_;
};

}  // namespace asw