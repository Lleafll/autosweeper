#pragma once

#include "Cell.h"
#include <cstdint>

namespace asw {

class PlayingField {
  public:
    virtual ~PlayingField() = default;

    [[nodiscard]] virtual std::size_t rows() const = 0;
    [[nodiscard]] virtual std::size_t columns() const = 0;
    [[nodiscard]] virtual int mine_count() const = 0;
    [[nodiscard]] virtual Cell
    operator()(std::size_t row, std::size_t column) const = 0;
    virtual void reveal(std::size_t row, std::size_t column) = 0;
    [[nodiscard]] virtual CellSpan span() = 0;
    [[nodiscard]] virtual ConstCellSpan cspan() const = 0;

    bool operator==(PlayingField const&) const = default;
};

}  // namespace asw
