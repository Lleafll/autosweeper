#include "Array2d.h"
#include "Match.h"
#include "Position.h"
#include "StringMaker.h"
#include "find_in_image.h"
#include <catch.hpp>

namespace {

TEST_CASE("Should be empty when sub_image is not present") {
    static constexpr asw::Array2d<asw::Color, 8, 2> image;
    static constexpr asw::Array2d<asw::Color, 4, 1> sub_image{
            {0, 0, 0}, {1, 1, 1}, {2, 2, 2}, {3, 3, 3}};
    REQUIRE(find_in_image(image.cspan(), sub_image.cspan()).empty());
}

TEST_CASE("Should find all positions when sub-image is present") {
    static constexpr size_t rows = 8;
    static constexpr size_t columns = 2;
    asw::Array2d<asw::Color, rows, columns> image;
    image(0, 1) = {1, 2, 3};
    image(2, 0) = {1, 2, 3};
    static constexpr asw::Array2d<asw::Color, 1, 1> sub_image{{1, 2, 3}};
    REQUIRE(find_in_image(image.cspan(), sub_image.cspan()) ==
            std::vector{asw::Position{0, 1}, asw::Position{2, 0}});
}

TEST_CASE("Matcher correctly finds subimages in image") {
    asw::Matcher const matcher{
            {{{1, 1}, {{123, 123, 123}}},
             {{1, 1}, {{1, 1, 1}}},
             {{1, 1}, {{2, 2, 2}}},
             {{1, 1}, {{3, 3, 3}}},
             {{1, 1}, {{4, 4, 4}}},
             {{1, 1}, {{5, 5, 5}}},
             {{1, 1}, {{6, 6, 6}}},
             {{1, 1}, {{7, 7, 7}}},
             {{1, 1}, {{8, 8, 8}}},
             {{1, 1}, {{66, 66, 66}}},
             {{1, 1}, {{99, 99, 99}}}}};
    asw::Image const image{
            {11, 1},
            {{123, 123, 123},
             {1, 1, 1},
             {2, 2, 2},
             {3, 3, 3},
             {4, 4, 4},
             {5, 5, 5},
             {6, 6, 6},
             {7, 7, 7},
             {8, 8, 8},
             {66, 66, 66},
             {99, 99, 99}}};
    REQUIRE(matcher(image.cspan()) == std::vector<asw::Match>{
                                              {{0, 0}, asw::Cell::Empty},
                                              {{1, 0}, asw::Cell::One},
                                              {{2, 0}, asw::Cell::Two},
                                              {{3, 0}, asw::Cell::Three},
                                              {{4, 0}, asw::Cell::Four},
                                              {{5, 0}, asw::Cell::Five},
                                              {{6, 0}, asw::Cell::Six},
                                              {{7, 0}, asw::Cell::Seven},
                                              {{8, 0}, asw::Cell::Eight},
                                              {{9, 0}, asw::Cell::Hidden},
                                              {{10, 0}, asw::Cell::Mine}});
}

}  // namespace