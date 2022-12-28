#include "Cell.h"
#include "StringMaker.h"
#include <catch.hpp>

using asw::Array2d;
using asw::Cell;
using asw::FixedArray2d;
using asw::MineCell;
using asw::Position;
using asw::Size;

namespace {

TEST_CASE("Assignment should work as expected") {
    Array2d<Cell, 2, 2> cells;
    cells(1, 1) = Cell::One;
    REQUIRE(cells(0, 0) == Cell::Empty);
    REQUIRE(cells(0, 1) == Cell::Empty);
    REQUIRE(cells(1, 0) == Cell::Empty);
    REQUIRE(cells(1, 1) == Cell::One);
}

TEST_CASE("Can modify filler cell at construction") {
    constexpr Array2d<Cell, 2, 1> cells(Cell::Eight);
    REQUIRE(cells(0, 0) == Cell::Eight);
    REQUIRE(cells(1, 0) == Cell::Eight);
}

TEST_CASE("Can use initializer list for filling array") {
    // clang-format off
    constexpr Array2d<Cell, 2, 2> cells{
            Cell::One, Cell::Two,
            Cell::Three, Cell::Four};
    // clang-format on
    REQUIRE(cells(0, 0) == Cell::One);
    REQUIRE(cells(0, 1) == Cell::Two);
    REQUIRE(cells(1, 0) == Cell::Three);
    REQUIRE(cells(1, 1) == Cell::Four);
}

class PredeterminedGenerator {
  public:
    PredeterminedGenerator(std::initializer_list<Position> const& positions)
        : positions_{positions} {
    }

    Position operator()(Size const&) {
        return positions_[current_++];
    }

  private:
    std::vector<Position> positions_;
    size_t current_ = 0;
};

TEST_CASE("generate_mines without collision") {
    constexpr Size size{4, 4};
    auto const mines = generate_mines(
            size, 3, PredeterminedGenerator{{0, 2}, {1, 2}, {2, 3}});
    auto expected = FixedArray2d<MineCell>{size};
    expected(0, 2) = MineCell::Mined;
    expected(1, 2) = MineCell::Mined;
    expected(2, 3) = MineCell::Mined;
    REQUIRE(mines == expected);
}

}  // namespace