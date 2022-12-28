#pragma once

#include <array>
#include <experimental/mdspan>

namespace asw {

template<typename T, size_t rows, size_t columns>
class Array2d final {
    static constexpr auto size = rows * columns;

  public:
    using size_type = size_t;
    using extents_type = std::experimental::extents<size_type, rows, columns>;
    using Span = std::experimental::mdspan<T, extents_type>;
    using ConstSpan = std::experimental::mdspan<T const, extents_type>;

    constexpr explicit Array2d(T const& fill = T{}) {
        std::ranges::fill(buffer_, fill);
    }

    constexpr Array2d(std::initializer_list<T> const& cells) {
        std::copy_n(cells.begin(), size, buffer_.begin());
    }

    constexpr T& operator()(size_t const row, size_t const column) {
        return span()(row, column);
    }

    [[nodiscard]] constexpr T const&
    operator()(size_t const row, size_t const column) const {
        return cspan()(row, column);
    }

    [[nodiscard]] constexpr size_type extent(size_t const r) const {
        switch (r) {
            case 0:
                return rows;
            case 1:
                return columns;
            default:
                return 0;
        }
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

}  // namespace asw
