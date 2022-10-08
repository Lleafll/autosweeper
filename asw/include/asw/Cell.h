#pragma once

#include <array>
#include <experimental/mdspan>

namespace asw {

enum class MineCell { Empty, Mine };

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

using ConstCellSpan =
        std::experimental::mdspan<Cell const, std::experimental::dextents<2>>;

template<typename T, std::size_t rows, std::size_t columns>
class BasicCellArray final {
    static constexpr auto size = rows * columns;

public:
    using Span = std::experimental::mdspan<T, std::experimental::dextents<2>>;
    using ConstSpan =
            std::experimental::mdspan<T const, std::experimental::dextents<2>>;

    constexpr explicit BasicCellArray(T fill = T{}) {
        std::ranges::fill(buffer_, fill);
    }

    constexpr BasicCellArray(std::initializer_list<T> const& cells) {
        std::copy_n(cells.begin(), size, buffer_.begin());
    }

    constexpr T& operator()(std::size_t const row, std::size_t const column) {
        return span()(row, column);
    }

    [[nodiscard]] constexpr T
    operator()(std::size_t const row, std::size_t const column) const {
        return cspan()(row, column);
    }

    [[nodiscard]] constexpr Span span() {
        return Span{buffer_.data(), rows, columns};
    }

    [[nodiscard]] constexpr ConstSpan cspan() const {
        return ConstSpan{buffer_.data(), rows, columns};
    }

private:
    std::array<T, size> buffer_ = {};
};

template<std::size_t rows, std::size_t columns>
using CellArray = BasicCellArray<Cell, rows, columns>;

template<std::size_t rows, std::size_t columns>
using MineCellArray = BasicCellArray<MineCell, rows, columns>;

}  // namespace asw