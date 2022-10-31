#include "PlayingField.h"
#include "StringMaker.h"
#include <catch.hpp>
#include <experimental/mdspan>

using namespace asw;
namespace stdex = std::experimental;

namespace {

TEST_CASE("All fields are hidden initially") {
    PlayingField const field{
            Array2d<MineCell, 2, 1>{MineCell::Clear, MineCell::Mined}.cspan()};
    REQUIRE(field(0, 0) == Cell::Hidden);
    REQUIRE(field(1, 0) == Cell::Hidden);
}

TEST_CASE("Can reveal field") {
    PlayingField field{
            Array2d<MineCell, 2, 1>{MineCell::Clear, MineCell::Clear}.cspan()};
    field.reveal(1, 0);
    REQUIRE(field(0, 0) == Cell::Hidden);
    REQUIRE(field(1, 0) == Cell::Empty);
}

TEST_CASE("Correctly shows mines and proximity") {
    PlayingField field{
            Array2d<MineCell, 2, 1>{MineCell::Clear, MineCell::Mined}.cspan()};
    field.reveal(0, 0);
    field.reveal(1, 0);
    REQUIRE(field(0, 0) == Cell::One);
    REQUIRE(field(1, 0) == Cell::Mine);
}

TEST_CASE("Correctly counts mines") {
    PlayingField const field{
            Array2d<MineCell, 2, 1>{MineCell::Clear, MineCell::Mined}.cspan()};
    REQUIRE(field.mine_count() == 1);
}

TEST_CASE("Correctly return rows and columns") {
    PlayingField const field{
            Array2d<MineCell, 2, 1>{MineCell::Clear, MineCell::Mined}.cspan()};
    REQUIRE(field.rows() == 2);
    REQUIRE(field.columns() == 1);
}

TEST_CASE("mdspan conversion") {
    PlayingField field{Array2d<MineCell, 2, 2>{
            MineCell::Clear, MineCell::Mined, MineCell::Clear, MineCell::Clear}
                               .cspan()};
    field.reveal(0, 0);
    field.reveal(0, 1);
    stdex::mdspan<Cell const, stdex::dextents<std::size_t, 2>> const view =
            field.cspan();
    REQUIRE(view(0, 0) == Cell::One);
    REQUIRE(view(0, 1) == Cell::Mine);
    REQUIRE(view(1, 0) == Cell::Hidden);
    REQUIRE(view(1, 1) == Cell::Hidden);
}

TEST_CASE("Correctly displays mine field") {
    using enum MineCell;
    PlayingField field{Array2d<MineCell, 5, 5>{
            // clang-format off
            Clear, Clear, Clear, Mined, Clear,
            Clear, Clear, Clear, Mined, Mined,
            Clear, Clear, Mined, Clear, Clear,
            Clear, Clear, Clear, Clear, Clear,
            Clear, Clear, Clear, Mined, Clear}.cspan()};  // clang-format on
    indexed_for_each(
            field.span(),
            [&field](
                    std::size_t const row,
                    std::size_t const column,
                    Cell const) { field.reveal(row, column); });
    using enum Cell;
    Array2d<Cell, 5, 5> const expected{
            // clang-format off
            Empty, Empty, Two,   Mine,  Three,
            Empty, One,   Three, Mine,  Mine,
            Empty, One,   Mine,  Three, Two,
            Empty, One,   Two,   Two,   One,
            Empty, Empty, One,   Mine,  One};  // clang-format on
    for (std::size_t row = 0; row < field.rows(); ++row) {
        for (std::size_t column = 0; column < field.columns(); ++column) {
            INFO(fmt::format("row: {}, column: {}", row, column));
            REQUIRE(field(row, column) == expected(row, column));
        }
    }
}

}  // namespace