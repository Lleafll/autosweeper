#include "Array2d.h"
#include "Match.h"
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

TEST_CASE("Matcher correctly finds subimages in image") {
    Matcher const matcher{
            {{{1, 4}, {123, 123, 123, 123}},
             {{1, 4}, {1, 1, 1, 1}},
             {{1, 4}, {2, 2, 2, 2}},
             {{1, 4}, {3, 3, 3, 3}},
             {{1, 4}, {4, 4, 4, 4}},
             {{1, 4}, {5, 5, 5, 5}},
             {{1, 4}, {6, 6, 6, 6}},
             {{1, 4}, {7, 7, 7, 7}},
             {{1, 4}, {8, 8, 8, 8}},
             {{1, 4}, {66, 66, 66, 66}},
             {{1, 4}, {99, 99, 99, 99}}}};
    Image const
            image{{11, 4},  // clang-format off
        {123, 123, 123, 123,
         1, 1, 1, 1,
         2, 2, 2, 2,
         3, 3, 3, 3,
         4, 4, 4, 4,
         5, 5, 5, 5,
         6, 6, 6, 6,
         7, 7, 7, 7,
         8, 8, 8, 8,
         66, 66, 66, 66,
         99, 99, 99, 99}};  // clang-format on
    REQUIRE(matcher(image.cspan()) == std::vector<Match>{
                                              Match{{0, 0}, Cell::Empty},
                                              Match{{1, 0}, Cell::One},
                                              Match{{2, 0}, Cell::Two},
                                              Match{{3, 0}, Cell::Three},
                                              Match{{4, 0}, Cell::Four},
                                              Match{{5, 0}, Cell::Five},
                                              Match{{6, 0}, Cell::Six},
                                              Match{{7, 0}, Cell::Seven},
                                              Match{{8, 0}, Cell::Eight},
                                              Match{{9, 0}, Cell::Hidden},
                                              Match{{10, 0}, Cell::Mine}});
}

}  // namespace