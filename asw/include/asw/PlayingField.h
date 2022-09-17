#pragma once

#include "MineField.h"

namespace asw {

class PlayingField final {
public:
    enum class Cell {
        Hidden,
        Empty,
        Flag,
        Mine,
        One,
        Two,
        Three,
        Four,
        Five,
        Six,
        Seven,
        Eight
    };

    explicit PlayingField(MineField mines);

    [[nodiscard]] Cell operator()(std::size_t row, std::size_t column) const;

    void reveal(std::size_t row, std::size_t column);

private:
    enum class Hidden : bool { Yes, No };

    std::size_t const rows_;
    std::vector<Hidden> hidden_;
    MineField mines_;

    [[nodiscard]] bool is_hidden(std::size_t row, std::size_t column) const;
};

}  // namespace asw