#include "find_in_image.h"
#include "Match.h"
#include "Position.h"
#include "algorithm2d.h"
#include <gsl/narrow>
#include <png++/png.hpp>

namespace stdex = std::experimental;

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

namespace {

Image load_sub_image(char const* const path) {
    png::image<png::rgba_pixel> const png{path};
    Image image{{png.get_height(), png.get_width()}};
    for (size_t row = 0; row < png.get_height(); ++row) {
        for (size_t column = 0; column < png.get_width(); ++column) {
            auto const& pixel = png.get_pixel(column, row);
            image(row, column) = pixel.red;
            image(row + 1, column) = pixel.green;
            image(row + 2, column) = pixel.blue;
            image(row + 3, column) = pixel.alpha;
        }
    }
    return image;
}

SubImages load_default_sub_images() {
    static auto const sub_images = []() -> SubImages {
        return {load_sub_image("minesweeperclassic/Empty.png"),
                load_sub_image("minesweeperclassic/1.png"),
                load_sub_image("minesweeperclassic/2.png"),
                load_sub_image("minesweeperclassic/3.png"),
                load_sub_image("minesweeperclassic/4.png"),
                load_sub_image("minesweeperclassic/5.png"),
                load_sub_image("minesweeperclassic/6.png"),
                load_sub_image("minesweeperclassic/7.png"),
                load_sub_image("minesweeperclassic/8.png"),
                load_sub_image("minesweeperclassic/Hidden.png"),
                load_sub_image("minesweeperclassic/Mine.png")};
    }();
    return sub_images;
}

}  // namespace

Matcher::Matcher() : sub_images_{load_default_sub_images()} {
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