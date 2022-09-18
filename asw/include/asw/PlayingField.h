#pragma once

#include "MineField.h"

namespace asw {

class PlayingField final {
public:
    enum class Cell {
        Empty,
        One,
        Two,
        Three,
        Four,
        Five,
        Six,
        Seven,
        Eight,
        Hidden,
        Mine
    };

    explicit PlayingField(MineField const& mines);

    [[nodiscard]] std::size_t rows() const;
    [[nodiscard]] std::size_t columns() const;
    [[nodiscard]] int  mine_count() const;
    [[nodiscard]] Cell operator()(std::size_t row, std::size_t column) const;
    bool reveal(std::size_t row, std::size_t column);

private:
    enum class Hidden : bool { Yes, No };

    std::size_t const rows_;
    std::size_t const columns_;
    std::vector<Hidden> hidden_;
    std::vector<Cell> cells_;

    [[nodiscard]] bool is_hidden(std::size_t row, std::size_t column) const;
    [[nodiscard]] Cell cell(std::size_t row, std::size_t column) const;
};

}  // namespace asw