#pragma once

#include "Cell.h"
#include "Position.h"

namespace asw {

class PlayingField {
  public:
    virtual ~PlayingField() = default;

    [[nodiscard]] virtual size_t rows() const = 0;
    [[nodiscard]] virtual size_t columns() const = 0;
    [[nodiscard]] virtual int mine_count() const = 0;
    [[nodiscard]] virtual Cell operator()(size_t row, size_t column) const = 0;
    virtual void reveal(Position const& position) = 0;
    [[nodiscard]] virtual CellSpan span() = 0;
    [[nodiscard]] virtual CellConstSpan cspan() const = 0;

    bool operator==(PlayingField const&) const = default;
};

}  // namespace asw
