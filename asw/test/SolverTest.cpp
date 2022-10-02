#include "Solver.h"
#include "PlayingField.h"
#include <catch.hpp>

using namespace asw;
namespace stdex = std::experimental;

namespace {

TEST_CASE(R"(Can get next step for simple case)") {
    std::array<MineCell, 3> mines{};
    auto const view = stdex::mdspan{mines.data(), 1, 3};
    view(0, 1) = MineCell::Mine;
    PlayingField field{view};
    field = field.reveal(0, 0);
    Solver solver{field};
    auto const result = solver.next();
    REQUIRE(result == Solver::Result::Success);
    REQUIRE(field(0, 2) == Cell::One);
}

TEST_CASE("Return Lost when mine is revealed") {
    PlayingField field{stdex::mdspan{
            std::array<MineCell, 1>{MineCell::Mine}.data(), 1, 1}};
    field = field.reveal(0, 0);
    Solver solver{field};
    REQUIRE(solver.next() == Solver::Result::Lost);
}

}  // namespace