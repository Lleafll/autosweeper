#include "Array2d.h"
#include "Position.h"
#include "StringMaker.h"
#include "find_in_image.h"
#include <catch.hpp>

using namespace asw;

namespace {

TEST_CASE("Should be empty when sub_image is not present") {
    static constexpr Array2d<unsigned char, 8, 8> image(0);
    static constexpr Array2d<unsigned char, 4, 4> sub_image{
            0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3};
    REQUIRE(find_in_image(image.cspan(), sub_image.cspan()).empty());
}

TEST_CASE("Should find all positions when sub-image is present") {
    Array2d<unsigned char, 8, 8> image(0);
    image(0, 4) = 1;
    image(0, 5) = 2;
    image(0, 6) = 3;
    image(0, 7) = 4;
    image(2, 0) = 1;
    image(2, 1) = 2;
    image(2, 2) = 3;
    image(2, 3) = 4;
    static constexpr Array2d<unsigned char, 1, 4> sub_image{1, 2, 3, 4};
    REQUIRE(find_in_image(image.cspan(), sub_image.cspan()) ==
            std::vector{Position{0, 4}, Position{2, 0}});
}

}  // namespace