#include "ImageMatchingPlayingField.h"
#include "IScreen.h"

namespace asw {

ImageMatchingPlayingField::ImageMatchingPlayingField(
        di::ptr<IScreen> screen,
        Matcher matcher)
    : screen_{std::move(screen)},
      matcher_{std::move(matcher)} {
}

size_t ImageMatchingPlayingField::rows() const {
    return field_.cspan().extent(0);
}

size_t ImageMatchingPlayingField::columns() const {
    return field_.cspan().extent(1);
}

int ImageMatchingPlayingField::mine_count() const {
    return 0;
}

Cell ImageMatchingPlayingField::operator()(
        [[maybe_unused]] size_t row,
        [[maybe_unused]] size_t column) const {
    throw std::runtime_error{"NYI"};
}

void ImageMatchingPlayingField::reveal(
        [[maybe_unused]] Position const& position) {
    throw std::runtime_error{"NYI"};
}

CellSpan ImageMatchingPlayingField::span() {
    throw std::runtime_error{"NYI"};
}

CellConstSpan ImageMatchingPlayingField::cspan() const {
    return field_.cspan();
}

}  // namespace asw