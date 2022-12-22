#include "ImageMatchingPlayingField.h"
#include "IScreen.h"
#include "MinesweeperScreen.h"
#include "algorithm2d.h"
#include "find_in_image.h"
#include <algorithm>
#include <gsl/narrow>

namespace stdr = std::ranges;

namespace asw {

namespace {

struct Bounds final {
    size_t bottom;
    size_t left;
    size_t top;
    size_t right;
};

Bounds find_bounds(std::span<Match const> const matches) {
    static constexpr auto row = [](Match const& match) -> size_t {
        return match.screen_position.row;
    };
    static constexpr auto column = [](Match const& match) -> size_t {
        return match.screen_position.column;
    };
    auto const [bottom, top] = stdr::minmax_element(matches, {}, row);
    auto const [left, right] = stdr::minmax_element(matches, {}, column);
    return {row(*bottom), column(*left), row(*top), column(*right)};
}

std::tuple<Vector2d<Cell>, Position> matches_to_field(
        std::span<Match const> const matches,
        int const distance_between_cells) {
    auto const [bottom, left, top, right] = find_bounds(matches);
    auto const to_row = [bottom, distance_between_cells](size_t const index) {
        return gsl::narrow_cast<size_t>(
                (index - bottom) / distance_between_cells);
    };
    auto const to_column = [left, distance_between_cells](size_t const index) {
        return gsl::narrow_cast<size_t>(
                (index - left) / distance_between_cells);
    };
    Vector2d<Cell> field{{to_row(top) + 1, to_column(right) + 1}, Cell::Hidden};
    for (auto const& match: matches) {
        field(to_row(match.screen_position.row),
              to_column(match.screen_position.column)) = match.cell;
    }
    return std::make_tuple<Vector2d<Cell>, Position>(
            std::move(field), {bottom, left});
}

}  // namespace

ImageMatchingPlayingField::ImageMatchingPlayingField(Logger& logger)
    : ImageMatchingPlayingField{
              di::make_owning<MinesweeperScreen>(logger),
              asw::Matcher{logger},
              16} {
}

ImageMatchingPlayingField::ImageMatchingPlayingField(
        di::ptr<IScreen> screen,
        Matcher matcher,
        int const distance_between_cells)
    : screen_{std::move(screen)},
      matcher_{std::move(matcher)},
      distance_between_cells_{distance_between_cells} {
    update();
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

Cell ImageMatchingPlayingField::operator()(size_t row, size_t column) const {
    return field_(row, column);
}

void ImageMatchingPlayingField::reveal(Position const& position) {
    screen_->click(
            {bottom_left_.column + position.column * distance_between_cells_,
             bottom_left_.row + position.row * distance_between_cells_});
}

CellConstSpan ImageMatchingPlayingField::cspan() const {
    return field_.cspan();
}

void ImageMatchingPlayingField::update() {
    auto const grab = screen_->grab();
    if (not grab.has_value()) {
        return;
    }
    auto const matches = matcher_(grab->cspan());
    if (matches.empty()) {
        return;
    }
    std::tie(field_, bottom_left_) =
            matches_to_field(matches, distance_between_cells_);
}

}  // namespace asw