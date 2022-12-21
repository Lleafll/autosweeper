#pragma once

#include "PlayingField.h"
#include "Position.h"

namespace asw {

class ImageMatchingPlayingField final : public PlayingField {
  public:
    ~ImageMatchingPlayingField() override = default;

    [[nodiscard]] size_t rows() const override;
    [[nodiscard]] size_t columns() const override;
    [[nodiscard]] int mine_count() const override;
    Cell operator()(size_t row, size_t column) const override;
    void reveal(Position& position) override;
    CellSpan span() override;
    [[nodiscard]] CellConstSpan cspan() const override;
};

}  // namespace asw