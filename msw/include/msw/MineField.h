#pragma once

#include <vector>

namespace msw {

class MineField final {
public:
    enum class Cell { Empty, Mine };

    struct MinePosition final {
        std::size_t row;
        std::size_t column;
    };

    MineField(std::size_t rows, std::size_t columns);
    MineField(
            std::size_t rows, std::size_t columns,
            std::vector<MinePosition> const& positions
    );

    [[nodiscard]] inline std::size_t rows() const { return rows_; }
    [[nodiscard]] inline std::size_t columns() const { return columns_; }
    [[nodiscard]] Cell& operator()(std::size_t row, std::size_t column);
    [[nodiscard]] Cell operator()(std::size_t row, std::size_t column) const;

private:
    std::size_t rows_;
    std::size_t columns_;
    std::vector<Cell> cells_;
};

}  // namespace msw