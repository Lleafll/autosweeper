#include "PlayingField.h"
#include "StringMaker.h"
#include <catch.hpp>
#include <experimental/mdspan>

using namespace asw;
namespace stdex = std::experimental;

namespace {

TEST_CASE("All fields are hidden initially") {
    PlayingField const field{stdex::mdspan{
            std::array{MineCell::Empty, MineCell::Mine}.data(), 2, 1}};
    REQUIRE(field(0, 0) == Cell::Hidden);
    REQUIRE(field(1, 0) == Cell::Hidden);
}

TEST_CASE("Can reveal field") {
    PlayingField field{stdex::mdspan{
            std::array{MineCell::Empty, MineCell::Empty}.data(), 2, 1}};
    field = field.reveal(1, 0);
    REQUIRE(field(0, 0) == Cell::Hidden);
    REQUIRE(field(1, 0) == Cell::Empty);
}

TEST_CASE("Correctly shows mines and proximity") {
    PlayingField field{stdex::mdspan{
            std::array{MineCell::Empty, MineCell::Mine}.data(), 2, 1}};
    field = field.reveal(0, 0).reveal(1, 0);
    REQUIRE(field(0, 0) == Cell::One);
    REQUIRE(field(1, 0) == Cell::Mine);
}

TEST_CASE("Correctly counts mines") {
    PlayingField const field{stdex::mdspan{
            std::array{MineCell::Empty, MineCell::Mine}.data(), 2, 1}};
    REQUIRE(field.mine_count() == 1);
}

TEST_CASE("Correctly return rows and columns") {
    PlayingField const field{stdex::mdspan{
            std::array{MineCell::Empty, MineCell::Mine}.data(), 2, 1}};
    REQUIRE(field.rows() == 2);
    REQUIRE(field.columns() == 1);
}

TEST_CASE("mdspan conversion") {
    PlayingField field{stdex::mdspan{
            std::array{
                    MineCell::Empty,
                    MineCell::Mine,
                    MineCell::Empty,
                    MineCell::Empty}
                    .data(),
            2,
            2}};
    field = field.reveal(0, 0).reveal(0, 1);
    stdex::mdspan<Cell const, stdex::dextents<2>> const view = field;
    REQUIRE(view(0, 0) == Cell::One);
    REQUIRE(view(0, 1) == Cell::Mine);
    REQUIRE(view(1, 0) == Cell::Hidden);
    REQUIRE(view(1, 1) == Cell::Hidden);
}

}  // namespace