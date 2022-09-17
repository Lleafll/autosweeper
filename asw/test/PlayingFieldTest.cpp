#include "PlayingField.h"
#include <catch.hpp>

using namespace asw;

namespace {

TEST_CASE("All fields are hidden initially") {
    PlayingField const field{MineField{2, 1, {{1, 0}}}};
    REQUIRE(field(0, 0) == PlayingField::Cell::Hidden);
    REQUIRE(field(1, 0) == PlayingField::Cell::Hidden);
}

}  // namespace