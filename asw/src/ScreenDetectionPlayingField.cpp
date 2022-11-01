#include "ScreenDetectionPlayingField.h"
#include "ITesseract.h"

namespace stdex = std::experimental;

namespace asw {

ScreenDetectionPlayingField::ScreenDetectionPlayingField(
        std::size_t const rows,
        std::size_t const columns,
        std::unique_ptr<ITesseract> tesseract)
    : rows_{rows},
      columns_{columns},
      tesseract_{std::move(tesseract)},
      detected_cells_{rows_ * columns_, Cell::Hidden} {
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