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
    explicit ScreenDetectionPlayingField(Size const& size);
    ScreenDetectionPlayingField(
            Size const& size,
            di::ptr<ITesseract> tesseract,
            di::ptr<IScreen> screen);

    ~ScreenDetectionPlayingField() override;
    ScreenDetectionPlayingField(ScreenDetectionPlayingField const&) = delete;
    ScreenDetectionPlayingField(ScreenDetectionPlayingField&&) noexcept =
            default;
    ScreenDetectionPlayingField&
    operator=(ScreenDetectionPlayingField const&) = delete;
    ScreenDetectionPlayingField&
    operator=(ScreenDetectionPlayingField&&) noexcept = default;

    [[nodiscard]] size_t rows() const override;
    [[nodiscard]] size_t columns() const override;
    [[nodiscard]] int mine_count() const override;
    [[nodiscard]] Cell operator()(size_t row, size_t column) const override;
    void reveal(Position const& position) override;
    [[nodiscard]] CellSpan span() override;
    [[nodiscard]] ConstCellSpan cspan() const override;

  private:
    size_t rows_;
    size_t columns_;
    di::ptr<ITesseract> tesseract_;
    di::ptr<IScreen> screen_;
    Vector2d<Cell> detected_cells_;
};

}  // namespace asw