#include "find_in_image.h"
#include "Position.h"
#include "algorithm2d.h"
#include <gsl/narrow>

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

}  // namespace asw