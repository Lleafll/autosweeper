#pragma once

#include <experimental/mdspan>
#include <vector>

namespace asw {
class PlayingFieldWithMines;
}  // namespace asw

namespace asw {

class SafeField final {
public:
    enum class Cell { Safe, Other };

    using View = std::experimental::mdspan<
            Cell,
            std::experimental::extents<
                    std::experimental::dynamic_extent,
                    std::experimental::dynamic_extent>>;
    using ConstView = std::experimental::mdspan<
            Cell const,
            std::experimental::extents<
                    std::experimental::dynamic_extent,
                    std::experimental::dynamic_extent>>;

    SafeField(std::size_t rows, std::size_t columns);

    [[nodiscard]] View view();
    [[nodiscard]] ConstView view() const;

    bool operator==(SafeField const& other) const = default;

private:
    std::size_t rows_;
    std::size_t columns_;
    std::vector<Cell> buffer_;
};

SafeField find_safe_cells(PlayingFieldWithMines const& field);

}  // namespace asw