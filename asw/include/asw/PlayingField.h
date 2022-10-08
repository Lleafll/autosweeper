#pragma once

#include "Cell.h"
#include <experimental/mdspan>
#include <gsl/pointers>
#include <vector>

namespace asw {

class PlayingField final {
public:
    using Mines = std::experimental::
            mdspan<MineCell const, std::experimental::dextents<2>>;

    explicit PlayingField(Mines const& mines);

    [[nodiscard]] std::size_t rows() const;
    [[nodiscard]] std::size_t columns() const;
    [[nodiscard]] int mine_count() const;
    [[nodiscard]] Cell operator()(std::size_t row, std::size_t column) const;
    void reveal(std::size_t row, std::size_t column);
    [[nodiscard]] ConstCellSpan cspan() const;

private:
    std::size_t rows_;
    std::size_t columns_;
    std::vector<Cell> hidden_;
    std::vector<Cell> cells_;

    [[nodiscard]] ConstCellSpan hidden() const;
    [[nodiscard]] ConstCellSpan cells() const;
};

}  // namespace asw