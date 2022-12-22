#pragma once

#include "Size.h"
#include <experimental/mdspan>
#include <vector>

namespace asw {

template<typename T>
class Vector2d final {
  public:
    using Span = std::experimental::
            mdspan<T, std::experimental::dextents<size_t, 2>>;
    using ConstSpan = std::experimental::
            mdspan<T const, std::experimental::dextents<size_t, 2>>;

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