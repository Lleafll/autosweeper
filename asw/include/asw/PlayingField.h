#pragma once

#include "Cell.h"
#include <experimental/mdspan>
#include <gsl/pointers>
#include <memory>
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
    [[nodiscard]] PlayingField
    reveal(std::size_t row, std::size_t column) const;

    operator ConstCellSpan() const;

private:
    std::size_t rows_;
    std::size_t columns_;
    std::vector<Cell> hidden_;
    gsl::strict_not_null<std::shared_ptr<std::vector<Cell> const>> cells_;

    PlayingField(
            std::size_t rows,
            std::size_t columns,
            std::vector<Cell> hidden,
            gsl::strict_not_null<std::shared_ptr<std::vector<Cell> const>>
                    cells);

    [[nodiscard]] ConstCellSpan hidden() const;
    [[nodiscard]] ConstCellSpan cells() const;
};

}  // namespace asw