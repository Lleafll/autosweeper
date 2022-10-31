#pragma once

#include "PlayingField.h"
namespace asw {
class ITesseract;
}  // namespace asw

namespace asw {

class ScreenDetectionPlayingField final : public PlayingField {
  public:
    explicit ScreenDetectionPlayingField(std::unique_ptr<ITesseract> tesseract);

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
    std::unique_ptr<ITesseract> tesseract_;
};

}  // namespace asw