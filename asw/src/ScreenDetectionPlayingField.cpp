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

ScreenDetectionPlayingField::ScreenDetectionPlayingField(
        std::size_t const rows,
        std::size_t const columns)
    : ScreenDetectionPlayingField{
              rows,
              columns,
              di::make_owning<Tesseract>(),
              di::make_owning<MinesweeperScreen>()} {
}

ScreenDetectionPlayingField::ScreenDetectionPlayingField(
        size_t rows,
        size_t columns,
        di::ptr<ITesseract> tesseract,
        di::ptr<IScreen> screen)
    : rows_{rows},
      columns_{columns},
      tesseract_{std::move(tesseract)},
      screen_{std::move(screen)},
      detected_cells_{rows_, columns_, Cell::Hidden} {
    detect(*tesseract_, *screen_, detected_cells_.span());
}

ScreenDetectionPlayingField::~ScreenDetectionPlayingField() = default;

std::size_t ScreenDetectionPlayingField::rows() const {
    return rows_;
}

std::size_t ScreenDetectionPlayingField::columns() const {
    return columns_;
}

int ScreenDetectionPlayingField::mine_count() const {
    return 0;
}

Cell ScreenDetectionPlayingField::operator()(
        [[maybe_unused]] std::size_t row,
        [[maybe_unused]] std::size_t column) const {
    throw std::runtime_error{"NYI"};
}

void ScreenDetectionPlayingField::reveal(
        [[maybe_unused]] std::size_t row,
        [[maybe_unused]] std::size_t column) {
    throw std::runtime_error{"NYI"};
}

CellSpan ScreenDetectionPlayingField::span() {
    return stdex::mdspan{detected_cells_.data(), rows_, columns_};
}

ConstCellSpan ScreenDetectionPlayingField::cspan() const {
    return stdex::mdspan{detected_cells_.data(), rows_, columns_};
}

}  // namespace asw