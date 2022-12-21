#include "ImageMatchingPlayingField.h"

namespace asw {

size_t ImageMatchingPlayingField::rows() const {
    throw std::runtime_error{"NYI"};
}

size_t ImageMatchingPlayingField::columns() const {
    throw std::runtime_error{"NYI"};
}

int ImageMatchingPlayingField::mine_count() const {
    throw std::runtime_error{"NYI"};
}

Cell ImageMatchingPlayingField::operator()(
        [[maybe_unused]] size_t row,
        [[maybe_unused]] size_t column) const {
    throw std::runtime_error{"NYI"};
}

void ImageMatchingPlayingField::reveal([[maybe_unused]] Position& position) {
    throw std::runtime_error{"NYI"};
}

CellSpan ImageMatchingPlayingField::span() {
    throw std::runtime_error{"NYI"};
}

CellConstSpan ImageMatchingPlayingField::cspan() const {
    throw std::runtime_error{"NYI"};
}

}  // namespace asw