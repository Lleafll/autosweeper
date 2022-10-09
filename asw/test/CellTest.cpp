#include "Cell.h"
#include "StringMaker.h"
#include <catch.hpp>

using namespace asw;

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

class PredeterminedGenerator final : public PositionGenerator {
  public:
    PredeterminedGenerator(std::initializer_list<Position> const& positions)
        : positions_{positions} {
    }

    ~PredeterminedGenerator() override = default;

    Position
    operator()(std::size_t const rows, std::size_t const columns) override {
        return positions_[current_++];
    }

  private:
    std::vector<Position> positions_;
    std::size_t current_ = 0;
};

TEST_CASE("generate_mines without collision") {
    auto const mines = generate_mines(
            4, 4, 3, PredeterminedGenerator{{0, 2}, {1, 2}, {2, 3}});
    auto expected = Vector2d<MineCell>{4, 4};
    expected(0, 2) = MineCell::Mine;
    expected(1, 2) = MineCell::Mine;
    expected(2, 3) = MineCell::Mine;
    REQUIRE(mines == expected);
}

}  // namespace