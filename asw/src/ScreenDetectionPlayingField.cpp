#include "ScreenDetectionPlayingField.h"
#include "ITesseract.h"

namespace asw {

ScreenDetectionPlayingField::ScreenDetectionPlayingField(
        std::unique_ptr<ITesseract> tesseract)
    : tesseract_{std::move(tesseract)} {
}

std::size_t ScreenDetectionPlayingField::rows() const {
    throw std::runtime_error{"NYI"};
}

std::size_t ScreenDetectionPlayingField::columns() const {
    throw std::runtime_error{"NYI"};
}

int ScreenDetectionPlayingField::mine_count() const {
    throw std::runtime_error{"NYI"};
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
    throw std::runtime_error{"NYI"};
}

ConstCellSpan ScreenDetectionPlayingField::cspan() const {
    throw std::runtime_error{"NYI"};
}

}  // namespace asw