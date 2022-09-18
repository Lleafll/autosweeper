#include "MineField.h"
#include <catch.hpp>

using namespace asw;

namespace {

TEST_CASE(
        "Rows and columns should reflect rows and columns from construction") {
    MineField mine_field{12, 34};
    REQUIRE(mine_field.rows() == 12);
    REQUIRE(mine_field.columns() == 34);
}

TEST_CASE("MineField is initially set to cleared") {
    MineField const mine_field{1, 2};
    REQUIRE((mine_field(0, 0)) == MineField::Cell::Empty);
    REQUIRE((mine_field(0, 1)) == MineField::Cell::Empty);
}

TEST_CASE("Cells in MineField can be changed") {
    MineField mine_field{1, 2};
    mine_field(0, 0) = MineField::Cell::Mine;
    REQUIRE((mine_field(0, 0)) == MineField::Cell::Mine);
    REQUIRE((mine_field(0, 1)) == MineField::Cell::Empty);
}

TEST_CASE("Can specify mine positions during construction") {
    MineField const mines{2, 1, {{1, 0}}};
    REQUIRE(mines(0, 0) == MineField::Cell::Empty);
    REQUIRE(mines(1, 0) == MineField::Cell::Mine);
}

}  // namespace