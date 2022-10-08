#include "Cell.h"
#include "StringMaker.h"
#include <catch.hpp>

using namespace asw;

namespace {

TEST_CASE("Assignment should work as expected") {
    CellArray<2, 2> cells;
    cells(1, 1) = Cell::One;
    REQUIRE(cells(0, 0) == Cell::Empty);
    REQUIRE(cells(0, 1) == Cell::Empty);
    REQUIRE(cells(1, 0) == Cell::Empty);
    REQUIRE(cells(1, 1) == Cell::One);
}

TEST_CASE("Can modify filler cell at construction") {
    constexpr CellArray<2, 1> cells(Cell::Eight);
    REQUIRE(cells(0, 0) == Cell::Eight);
    REQUIRE(cells(1, 0) == Cell::Eight);
}

TEST_CASE("Can use initializer list for filling array") {
    // clang-format off
    constexpr CellArray<2, 2> cells{
            Cell::One, Cell::Two,
            Cell::Three, Cell::Four};
    // clang-format on
    REQUIRE(cells(0, 0) == Cell::One);
    REQUIRE(cells(0, 1) == Cell::Two);
    REQUIRE(cells(1, 0) == Cell::Three);
    REQUIRE(cells(1, 1) == Cell::Four);
}

}  // namespace