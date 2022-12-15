#include "InMemoryPlayingField.h"
#include "StringMaker.h"
#include <catch.hpp>
#include <experimental/mdspan>

namespace stdex = std::experimental;
using asw::Array2d;
using asw::Cell;
using asw::InMemoryPlayingField;
using asw::MineCell;
using asw::Position;
using asw::Size;

namespace {

TEST_CASE("All fields are hidden initially") {
    InMemoryPlayingField const field{
            Array2d<MineCell, 2, 1>{MineCell::Clear, MineCell::Mined}.cspan()};
    REQUIRE(field(0, 0) == Cell::Hidden);
    REQUIRE(field(1, 0) == Cell::Hidden);
}

TEST_CASE("Can reveal field") {
    InMemoryPlayingField field{
            Array2d<MineCell, 2, 1>{MineCell::Clear, MineCell::Clear}.cspan()};
    field.reveal({1, 0});
    REQUIRE(field(0, 0) == Cell::Hidden);
    REQUIRE(field(1, 0) == Cell::Empty);
}

TEST_CASE("Correctly shows mines and proximity") {
    InMemoryPlayingField field{
            Array2d<MineCell, 2, 1>{MineCell::Clear, MineCell::Mined}.cspan()};
    field.reveal({0, 0});
    field.reveal({1, 0});
    REQUIRE(field(0, 0) == Cell::One);
    REQUIRE(field(1, 0) == Cell::Mine);
}

TEST_CASE("Correctly counts mines") {
    InMemoryPlayingField const field{
            Array2d<MineCell, 2, 1>{MineCell::Clear, MineCell::Mined}.cspan()};
    REQUIRE(field.mine_count() == 1);
}

TEST_CASE("Correctly return rows and columns") {
    InMemoryPlayingField const field{
            Array2d<MineCell, 2, 1>{MineCell::Clear, MineCell::Mined}.cspan()};
    REQUIRE(field.rows() == 2);
    REQUIRE(field.columns() == 1);
}

TEST_CASE("mdspan conversion") {
    InMemoryPlayingField field{Array2d<MineCell, 2, 2>{
            MineCell::Clear, MineCell::Mined, MineCell::Clear, MineCell::Clear}
                                       .cspan()};
    field.reveal({0, 0});
    field.reveal({0, 1});
    stdex::mdspan<Cell const, stdex::dextents<size_t, 2>> const view =
            field.cspan();
    REQUIRE(view(0, 0) == Cell::One);
    REQUIRE(view(0, 1) == Cell::Mine);
    REQUIRE(view(1, 0) == Cell::Hidden);
    REQUIRE(view(1, 1) == Cell::Hidden);
}

TEST_CASE("Correctly displays mine field") {
    using enum MineCell;
    static constexpr Size size{5, 5};
    InMemoryPlayingField field{Array2d<MineCell, size.rows, size.columns>{
            // clang-format off
            Clear, Clear, Clear, Mined, Clear,
            Mined, Clear, Clear, Mined, Mined,
            Clear, Clear, Mined, Clear, Clear,
            Clear, Clear, Clear, Clear, Clear,
            Clear, Clear, Clear, Mined, Clear}.cspan()};  // clang-format on
    asw::indexed_for_each(
            field.span(),
            [&field](Position const& i, Cell const) { field.reveal(i); });
    using enum Cell;
    Array2d<Cell, 5, 5> const expected{
            // clang-format off
            One,    One,    Two,    Mine,   Hidden,
            Hidden, Hidden, Hidden, Hidden, Hidden,
            Hidden, Hidden, Hidden, Hidden, Hidden,
            Hidden, Hidden, Hidden, Hidden, Hidden,
            Hidden, Hidden, Hidden, Hidden, Hidden};  // clang-format on
    for (size_t row = 0; row < field.rows(); ++row) {
        for (size_t column = 0; column < field.columns(); ++column) {
            INFO(fmt::format("row: {}, column: {}", row, column));
            REQUIRE(field(row, column) == expected(row, column));
        }
    }
}

TEST_CASE("Reveal not possible when a mine is already revealed") {
    using enum MineCell;
    InMemoryPlayingField field{Array2d<MineCell, 1, 2>{Mined, Clear}.cspan()};
    field.reveal({0, 0});
    field.reveal({0, 1});
    REQUIRE(field(0, 1) == Cell::Hidden);
}

}  // namespace