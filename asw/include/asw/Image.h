#pragma once

#include "Vector2d.h"
#include <experimental/mdspan>

namespace asw {

using Image = Vector2d<unsigned char>;

using ImageConstSpan = std::experimental::
        mdspan<unsigned char const, std::experimental::dextents<size_t, 2>>;

}  // namespace asw