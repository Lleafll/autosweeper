#pragma once

#include "PlayingField.h"
#include "Image.h"
#include <di/ptr.h>
namespace asw {
class IScreen;
class ITesseract;
}  // namespace asw

namespace asw {

class TextDetectionPlayingField final : public PlayingField {
  public:
    explicit TextDetectionPlayingField(Size const& size);
    TextDetectionPlayingField(
            Size const& size,
            di::ptr<ITesseract> tesseract,
            di::ptr<IScreen> screen);

    ~TextDetectionPlayingField() override;
    TextDetectionPlayingField(TextDetectionPlayingField const&) = delete;
    TextDetectionPlayingField(TextDetectionPlayingField&&) noexcept =
            default;
    TextDetectionPlayingField&
    operator=(TextDetectionPlayingField const&) = delete;
    TextDetectionPlayingField&
    operator=(TextDetectionPlayingField&&) noexcept = default;

    [[nodiscard]] size_t rows() const override;
    [[nodiscard]] size_t columns() const override;
    [[nodiscard]] int mine_count() const override;
    [[nodiscard]] Cell operator()(size_t row, size_t column) const override;
    void reveal(Position const& position) override;
    [[nodiscard]] CellConstSpan cspan() const override;

  private:
    size_t rows_;
    size_t columns_;
    di::ptr<ITesseract> tesseract_;
    di::ptr<IScreen> screen_;
    Vector2d<Cell> detected_cells_;
};

}  // namespace asw