#pragma once

#include "PlayingField.h"

namespace asw {

class ScreenDetectionPlayingField final : public PlayingField {
  public:
    ~ScreenDetectionPlayingField() override = default;

    [[nodiscard]] std::size_t rows() const override;
    [[nodiscard]] std::size_t columns() const override;
    [[nodiscard]] int mine_count() const override;
    [[nodiscard]] Cell
    operator()(std::size_t row, std::size_t column) const override;
    void reveal(std::size_t row, std::size_t column) override;
    [[nodiscard]] CellSpan span() override;
    [[nodiscard]] ConstCellSpan cspan() const override;
};

}  // namespace asw