#include "ScreenDetectionPlayingField.h"
#include "ITesseract.h"
#include "MinesweeperScreen.h"
#include "Tesseract.h"

namespace stdex = std::experimental;

namespace asw {

namespace {

void detect(
        [[maybe_unused]] ITesseract& detection,
        IScreen& screen,
        [[maybe_unused]] CellSpan const cells) {
    auto const image = screen.grab();
    if (!image.has_value()) {
        std::puts("Could not grab image");
        return;
    }
}

}  // namespace

ScreenDetectionPlayingField::ScreenDetectionPlayingField(Size const& size)
    : ScreenDetectionPlayingField{
              size,
              di::make_owning<Tesseract>(),
              di::make_owning<MinesweeperScreen>()} {
}

ScreenDetectionPlayingField::ScreenDetectionPlayingField(
        Size const& size,
        di::ptr<ITesseract> tesseract,
        di::ptr<IScreen> screen)
    : rows_{size.rows},
      columns_{size.columns},
      tesseract_{std::move(tesseract)},
      screen_{std::move(screen)},
      detected_cells_{size, Cell::Hidden} {
    detect(*tesseract_, *screen_, detected_cells_.span());
}

ScreenDetectionPlayingField::~ScreenDetectionPlayingField() = default;

size_t ScreenDetectionPlayingField::rows() const {
    return rows_;
}

size_t ScreenDetectionPlayingField::columns() const {
    return columns_;
}

int ScreenDetectionPlayingField::mine_count() const {
    return 0;
}

Cell ScreenDetectionPlayingField::operator()(
        [[maybe_unused]] size_t row,
        [[maybe_unused]] size_t column) const {
    throw std::runtime_error{"NYI"};
}

void ScreenDetectionPlayingField::reveal(
        [[maybe_unused]] Position const& position) {
    throw std::runtime_error{"NYI"};
}

CellSpan ScreenDetectionPlayingField::span() {
    return stdex::mdspan{detected_cells_.data(), rows_, columns_};
}

ConstCellSpan ScreenDetectionPlayingField::cspan() const {
    return stdex::mdspan{detected_cells_.data(), rows_, columns_};
}

}  // namespace asw