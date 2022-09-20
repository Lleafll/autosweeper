#include "PlayingFieldWithMines.h"
#include <catch.hpp>
#include <magic_enum.hpp>

using namespace asw;

template<>
struct Catch::StringMaker<PlayingField::Cell> {
    static std::string convert(PlayingField::Cell const cell) {
        return std::string{magic_enum::enum_name(cell)};
    }
};

namespace {

TEST_CASE("Evaluates hidden playing field correctly") {
    auto const evaluation = find_mines(PlayingField{MineField{1, 2, {{0, 0}}}});
    auto const view = evaluation.view();
    REQUIRE(view(0, 0) == PlayingField::Cell::Hidden);
    REQUIRE(view(0, 1) == PlayingField::Cell::Hidden);
}

TEST_CASE("find_mines finds mine") {
    auto field = PlayingField{MineField{1, 2, {{0, 1}}}};
    field.reveal(0, 0);
    auto const evaluation = find_mines(field);
    auto const view = evaluation.view();
    REQUIRE(view(0, 0) == PlayingField::Cell::One);
    REQUIRE(view(0, 1) == PlayingField::Cell::Mine);
}

}  // namespace