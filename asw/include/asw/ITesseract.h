#pragma once

#include <experimental/mdspan>
#include <vector>

namespace asw {

struct TesseractDetection final {
    char character;
    int screen_position_x;
    int screen_position_y;
};

class ITesseract {
  public:
    virtual ~ITesseract() = default;

    [[nodiscard]] virtual std::vector<TesseractDetection>
    detect(std::experimental::mdspan<
            unsigned char const,
            std::experimental::dextents<std::size_t, 2>> image) const = 0;
};

}  // namespace asw