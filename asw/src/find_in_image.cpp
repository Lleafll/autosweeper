#include "find_in_image.h"
#include "Match.h"
#include "Position.h"
#include "algorithm2d.h"
#include <gsl/narrow>

namespace stdex = std::experimental;
namespace stdr = std::ranges;

namespace asw {

std::vector<Position>
find_in_image(ImageConstSpan const& image, ImageConstSpan const& sub_image) {
    std::vector<Position> positions;
    for (auto row = 0; row < image.extent(0); ++row) {
        for (auto column = 0; column < image.extent(1); column += 4) {
            if (equals(stdex::submdspan(
                               image,
                               std::pair{row, row + sub_image.extent(0)},
                               std::pair{column, column + sub_image.extent(1)}),
                       sub_image)) {
                positions.push_back(
                        {gsl::narrow_cast<size_t>(row),
                         gsl::narrow_cast<size_t>(column)});
            }
        }
    }
    return positions;
}

Matcher::Matcher(SubImages sub_images) : sub_images_{std::move(sub_images)} {
}

namespace {

template<Cell cell>
void add_matches(
        std::vector<Match>& matches,
        ImageConstSpan const& image,
        Image const& sub_image) {
    for (auto const& position: find_in_image(image, sub_image.cspan())) {
        matches.push_back({position, cell});
    }
}

}  // namespace

std::vector<Match> Matcher::operator()(ImageConstSpan const& image) const {
    std::vector<Match> matches;
    add_matches<Cell::Empty>(matches, image, sub_images_.empty);
    add_matches<Cell::One>(matches, image, sub_images_.one);
    add_matches<Cell::Two>(matches, image, sub_images_.two);
    add_matches<Cell::Three>(matches, image, sub_images_.three);
    add_matches<Cell::Four>(matches, image, sub_images_.four);
    add_matches<Cell::Five>(matches, image, sub_images_.five);
    add_matches<Cell::Six>(matches, image, sub_images_.six);
    add_matches<Cell::Seven>(matches, image, sub_images_.seven);
    add_matches<Cell::Eight>(matches, image, sub_images_.eight);
    add_matches<Cell::Hidden>(matches, image, sub_images_.hidden);
    add_matches<Cell::Mine>(matches, image, sub_images_.mine);
    return matches;
}

}  // namespace asw