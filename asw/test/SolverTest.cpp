#include "Solver.h"
#include "PlayingField.h"
#include "StringMaker.h"
#include <catch.hpp>

using namespace asw;

namespace {

TEST_CASE(R"(Can get next step for simple case)") {
    MineCellArray<1, 3> mines{};
    mines(0, 1) = MineCell::Mine;
    PlayingField field{mines.cspan()};
    field = field.reveal(0, 0);
    Solver solver{field};
    auto const result = solver.next();
    REQUIRE(result == Solver::Result::Success);
    REQUIRE(field(0, 2) == Cell::One);
}

TEST_CASE("Return Lost when mine is revealed") {
    PlayingField field{MineCellArray<1, 1>{MineCell::Mine}.cspan()};
    field = field.reveal(0, 0);
    Solver solver{field};
    REQUIRE(solver.next() == Solver::Result::Lost);
}

}  // namespace