#pragma once

#include "Image.h"
#include <vector>
namespace asw {
struct Match;
struct Position;
}  // namespace asw

namespace asw {

std::vector<Position>
find_in_image(ImageConstSpan const& image, ImageConstSpan const& sub_image);

struct SubImages {
    Image empty;
    Image one;
    Image two;
    Image three;
    Image four;
    Image five;
    Image six;
    Image seven;
    Image eight;
    Image hidden;
    Image mine;
};

class Matcher {
  public:
    /**
     * @brief Matches Minesweeper Classic with default subimages
     */
    Matcher();

    explicit Matcher(SubImages sub_images);

    std::vector<Match> operator()(ImageConstSpan const& image) const;

  private:
    SubImages sub_images_;
};

}  // namespace asw
