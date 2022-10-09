#pragma once

#include <experimental/mdspan>
#include <vector>

namespace asw {

template<typename T>
class Vector2d final {
  public:
    using Span = std::experimental::mdspan<T, std::experimental::dextents<2>>;
    using ConstSpan =
            std::experimental::mdspan<T const, std::experimental::dextents<2>>;

    explicit Vector2d(
            std::size_t const rows,
            std::size_t const columns,
            T const& fill = T{})
        : rows_{rows},
          columns_{columns},
          buffer_(rows * columns, fill) {
    }

    Vector2d(
            std::size_t const rows,
            std::size_t const columns,
            std::vector<T> cells)
        : rows_{rows},
          columns_{columns},
          buffer_{std::move(cells)} {
        buffer_.resize(rows_ * columns_);
    }

    [[nodiscard]] T&
    operator()(std::size_t const row, std::size_t const column) {
        return span()(row, column);
    }

    [[nodiscard]] T const&
    operator()(std::size_t const row, std::size_t const column) const {
        return cspan()(row, column);
    }

    [[nodiscard]] Span span() {
        return Span{buffer_.data(), rows_, columns_};
    }

    [[nodiscard]] ConstSpan cspan() const {
        return ConstSpan{buffer_.data(), rows_, columns_};
    }

    bool operator==(Vector2d const&) const = default;

  private:
    std::size_t rows_;
    std::size_t columns_;
    std::vector<T> buffer_ = {};
};

}  // namespace asw