#pragma once

#include "Cell.h"
#include "PlayingField.h"
#include <experimental/mdspan>
#include <vector>

namespace asw {

class InMemoryPlayingField final : public PlayingField {
  public:
    explicit InMemoryPlayingField(MineCellConstSpan const& mines);

    ~InMemoryPlayingField() override = default;

    [[nodiscard]] size_t rows() const override;
    [[nodiscard]] size_t columns() const override;
    [[nodiscard]] int mine_count() const override;
    [[nodiscard]] Cell operator()(size_t row, size_t column) const override;
    void reveal(Position const& position) override;
    [[nodiscard]] CellConstSpan cspan() const override;

    bool operator==(InMemoryPlayingField const&) const = default;
    void update() override;

  private:
    size_t rows_;
    size_t columns_;
    std::vector<Cell> hidden_;
    std::vector<Cell> cells_;

    [[nodiscard]] CellConstSpan hidden() const;
    [[nodiscard]] CellConstSpan cells() const;
};

}  // namespace asw