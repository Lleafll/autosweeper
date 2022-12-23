#pragma once

#include "Cell.h"
#include "PlayingField.h"
#include <experimental/mdspan>
#include <vector>

namespace asw {

class InMemoryPlayingField  {
  public:
    explicit InMemoryPlayingField(MineCellConstSpan const& mines);

    [[nodiscard]] size_t rows() const;
    [[nodiscard]] size_t columns() const;
    [[nodiscard]] int mine_count() const;
    [[nodiscard]] Cell operator()(size_t row, size_t column) const;
    void reveal(Position const& position);
    [[nodiscard]] CellConstSpan cspan() const;
    void update();

    bool operator==(InMemoryPlayingField const&) const = default;

  private:
    size_t rows_;
    size_t columns_;
    std::vector<Cell> hidden_;
    std::vector<Cell> cells_;

    [[nodiscard]] CellConstSpan hidden() const;
    [[nodiscard]] CellConstSpan cells() const;
};

}  // namespace asw