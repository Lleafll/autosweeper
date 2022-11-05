#pragma once

#include "PlayingField.h"
#include "Vector2d.h"
#include <di/ptr.h>
namespace asw {
class IScreen;
class ITesseract;
}  // namespace asw

namespace asw {

class ScreenDetectionPlayingField final : public PlayingField {
  public:
    ScreenDetectionPlayingField(std::size_t rows, std::size_t columns);
    ScreenDetectionPlayingField(
            std::size_t rows,
            std::size_t columns,
            di::ptr<ITesseract> tesseract,
            di::ptr<IScreen> screen);

    ~ScreenDetectionPlayingField() override;

    [[nodiscard]] std::size_t rows() const override;
    [[nodiscard]] std::size_t columns() const override;
    [[nodiscard]] int mine_count() const override;
    [[nodiscard]] Cell
    operator()(std::size_t row, std::size_t column) const override;
    void reveal(std::size_t row, std::size_t column) override;
    [[nodiscard]] CellSpan span() override;
    [[nodiscard]] ConstCellSpan cspan() const override;

  private:
    std::size_t rows_;
    std::size_t columns_;
    di::ptr<ITesseract> tesseract_;
    di::ptr<IScreen> screen_;
    Vector2d<Cell> detected_cells_;
};

}  // namespace asw