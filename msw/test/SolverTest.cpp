#include "Solver.h"
#include "PlayingField.h"
#include <catch.hpp>

using namespace asw;

namespace {

TEST_CASE(R"(Can get next step for simple case)") {
    PlayingField field{MineField{1, 3, {{0, 1}}}};
    field.reveal(0, 0);
    Solver solver{field};
    auto const result = solver.next();
    REQUIRE(result == Solver::Result::Success);
    REQUIRE(field(0, 2) == PlayingField::Cell::One);
}

TEST_CASE("Return Lost when mine is revealed") {
    PlayingField field{MineField{1, 1, {{0, 0}}}};
    field.reveal(0, 0);
    Solver solver{field};
    REQUIRE(solver.next() == Solver::Result::Lost);
}

}  // namespace