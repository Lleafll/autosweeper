#pragma once

#include "Image.h"
#include <vector>
namespace asw {
struct Position;
}  // namespace asw

namespace asw {

std::vector<Position>
find_in_image(ImageConstSpan const& image, ImageConstSpan const& sub_image);

}  // namespace asw
