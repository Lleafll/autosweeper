#pragma once

#include "Vector2d.h"
#include <experimental/mdspan>

namespace asw {

struct Color {
    unsigned char red = 0;
    unsigned char green = 0;
    unsigned char blue = 0;

    bool operator==(Color const&) const = default;
};

using Image = Vector2d<Color>;

using ImageConstSpan = std::experimental::
        mdspan<Color const, std::experimental::dextents<size_t, 2>>;

}  // namespace asw