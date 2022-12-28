#pragma once

#include "Size.h"
#include <experimental/mdspan>
#include <vector>

namespace asw {

template<typename T>
class Vector2d final {
  public:
    using size_type = size_t;
    using extents_type = std::experimental::dextents<size_type, 2>;
    using Span = std::experimental::mdspan<T, extents_type>;
    using ConstSpan = std::experimental::mdspan<T const, extents_type>;

    Vector2d() = default;

    explicit Vector2d(Size const& size, T const& fill = T{})
        : rows_{size.rows},
          columns_{size.columns},
          buffer_(rows_ * columns_, fill) {
    }

    Vector2d(Size const& size, std::vector<T> cells)
        : rows_{size.rows},
          columns_{size.columns},
          buffer_{std::move(cells)} {
        buffer_.resize(rows_ * columns_);
    }

    [[nodiscard]] T& operator()(size_t const row, size_t const column) {
        return span()(row, column);
    }

    [[nodiscard]] T const&
    operator()(size_t const row, size_t const column) const {
        return cspan()(row, column);
    }

    [[nodiscard]] size_type extent(size_t const r) const {
        switch (r) {
            case 0:
                return rows_;
            case 1:
                return columns_;
            default:
                return 0;
        }
    }

    [[nodiscard]] Span span() {
        return Span{buffer_.data(), rows_, columns_};
    }

    [[nodiscard]] ConstSpan cspan() const {
        return ConstSpan{buffer_.data(), rows_, columns_};
    }

    [[nodiscard]] T* data() {
        return buffer_.data();
    }

    [[nodiscard]] T const* data() const {
        return buffer_.data();
    }

    bool operator==(Vector2d const&) const = default;

  private:
    size_t rows_ = 0;
    size_t columns_ = 0;
    std::vector<T> buffer_ = {};
};

}  // namespace asw