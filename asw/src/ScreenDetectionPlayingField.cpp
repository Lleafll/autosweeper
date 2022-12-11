#include "ScreenDetectionPlayingField.h"
#include "ITesseract.h"
#include "MinesweeperScreen.h"
#include "Tesseract.h"
#include <gsl/narrow>
#include <iostream>

namespace stdex = std::experimental;

namespace asw {

namespace {

constexpr int bytes_per_pixel = 4;

void detect(
        ITesseract& detection,
        IScreen& screen,
        [[maybe_unused]] CellSpan const cells) {
    auto const image = screen.grab();
    if (!image.has_value()) {
        std::puts("Could not grab image");
        return;
    }
    auto const span = image->cspan();
    detection.set_image(
            span,
            ImageInfo{
                    bytes_per_pixel,
                    bytes_per_pixel * gsl::narrow_cast<int>(span.extent(0))});
    auto iterator = detection.get_iterator();
    if (iterator != nullptr) {
        do {
            std::puts(iterator->get_utf8_text().data());
        } while (iterator->next());
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