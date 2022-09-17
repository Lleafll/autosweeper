#include "PlayingField.h"
#include <catch.hpp>
#include <magic_enum.hpp>

using namespace msw;

template<>
struct Catch::StringMaker<PlayingField::Cell> {
    static std::string convert(PlayingField::Cell const cell) {
        return std::string{magic_enum::enum_name(cell)};
    }
};

namespace {

TEST_CASE("All fields are hidden initially") {
    PlayingField const field{MineField{2, 1, {{1, 0}}}};
    REQUIRE(field(0, 0) == PlayingField::Cell::Hidden);
    REQUIRE(field(1, 0) == PlayingField::Cell::Hidden);
}

TEST_CASE("Can reveal field") {
    PlayingField field{MineField{2, 1}};
    field.reveal(1, 0);
    REQUIRE(field(0, 0) == PlayingField::Cell::Hidden);
    REQUIRE(field(1, 0) == PlayingField::Cell::Empty);
}

TEST_CASE("Correctly shows mines and proximity") {
    PlayingField field{MineField{2, 1, {{1, 0}}}};
    field.reveal(0, 0);
    field.reveal(1, 0);
    REQUIRE(field(0, 0) == PlayingField::Cell::One);
    REQUIRE(field(1, 0) == PlayingField::Cell::Mine);
}

TEST_CASE("Signals when mine is hit") {
    PlayingField field{MineField{2, 1, {{1, 0}}}};
    REQUIRE_FALSE(field.reveal(0, 0));
    REQUIRE(field.reveal(1, 0));
}

}  // namespace