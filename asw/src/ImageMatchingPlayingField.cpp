#include "ImageMatchingPlayingField.h"
#include "find_in_image.h"
#include <gsl/narrow>

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
    namespace stdr = std::ranges;
    auto const [bottom, top] = stdr::minmax_element(matches, {}, row);
    auto const [left, right] = stdr::minmax_element(matches, {}, column);
    return {row(*bottom), column(*left), row(*top), column(*right)};
}

}  // namespace

namespace detail {

std::tuple<FixedArray2d<Cell>, Position> matches_to_field(
        std::span<Match const> const matches,
        int const distance_between_cells) {
    auto const bounds = find_bounds(matches);
    auto const to_row = [&bounds, distance_between_cells](size_t const index) {
        return gsl::narrow_cast<size_t>(
                (index - bounds.bottom) / distance_between_cells);
    };
    auto const to_column = [&bounds,
                            distance_between_cells](size_t const index) {
        return gsl::narrow_cast<size_t>(
                (index - bounds.left) / distance_between_cells);
    };
    FixedArray2d<Cell> field{
            {to_row(bounds.top) + 1, to_column(bounds.right) + 1},
            Cell::Hidden};
    for (auto const& match: matches) {
        field(to_row(match.screen_position.row),
              to_column(match.screen_position.column)) = match.cell;
    }
    return std::make_tuple<FixedArray2d<Cell>, Position>(
            std::move(field), {bounds.bottom, bounds.left});
}

}  // namespace detail

}  // namespace asw