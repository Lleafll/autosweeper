#include "ITesseract.h"
#include "MinesweeperScreen.h"
#include "Tesseract.h"
#include "TextDetectionPlayingField.h"
#include <gsl/narrow>

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
            ImageInfo{bytes_per_pixel, gsl::narrow_cast<int>(span.extent(0))});
    detection.recognize();
    auto iterator = detection.get_iterator();
    if (iterator != nullptr) {
        do {
            std::puts(iterator->get_utf8_text().c_str());
        } while (iterator->next());
    }
}

}  // namespace

TextDetectionPlayingField::TextDetectionPlayingField(Size const& size)
    : TextDetectionPlayingField{
              size,
              di::make_owning<Tesseract>(),
              di::make_owning<MinesweeperScreen>()} {
}

TextDetectionPlayingField::TextDetectionPlayingField(
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

TextDetectionPlayingField::~TextDetectionPlayingField() = default;

size_t TextDetectionPlayingField::rows() const {
    return rows_;
}

size_t TextDetectionPlayingField::columns() const {
    return columns_;
}

int TextDetectionPlayingField::mine_count() const {
    return 0;
}

Cell TextDetectionPlayingField::operator()(
        [[maybe_unused]] size_t row,
        [[maybe_unused]] size_t column) const {
    throw std::runtime_error{"NYI"};
}

void TextDetectionPlayingField::reveal(
        [[maybe_unused]] Position const& position) {
    throw std::runtime_error{"NYI"};
}

CellSpan TextDetectionPlayingField::span() {
    return stdex::mdspan{detected_cells_.data(), rows_, columns_};
}

ConstCellSpan TextDetectionPlayingField::cspan() const {
    return stdex::mdspan{detected_cells_.data(), rows_, columns_};
}

}  // namespace asw