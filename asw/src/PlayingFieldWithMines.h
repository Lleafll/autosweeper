#pragma once

#include "PlayingField.h"
#include <experimental/mdspan>

namespace asw {

class PlayingFieldWithMines final {
public:
    using View = std::experimental::mdspan<
            PlayingField::Cell,
            std::experimental::extents<
                    std::experimental::dynamic_extent,
                    std::experimental::dynamic_extent>>;
    using ConstView = std::experimental::mdspan<
            PlayingField::Cell const,
            std::experimental::extents<
                    std::experimental::dynamic_extent,
                    std::experimental::dynamic_extent>>;

    PlayingFieldWithMines(std::size_t rows, std::size_t columns);

    [[nodiscard]] View view();
    [[nodiscard]] ConstView view() const;

private:
    std::size_t rows_;
    std::size_t columns_;
    std::vector<PlayingField::Cell> buffer_;
};

PlayingFieldWithMines find_mines(PlayingField const& field);

}  // namespace asw