#include "PlayingField.h"
#include "predict_mines.h"
#include <catch.hpp>

using namespace asw;
namespace stdex = std::experimental;

namespace {

TEST_CASE("is_subset_of") {
    REQUIRE(MinePrediction{{{1, 1}}, 1}.is_subset_of(
            MinePrediction{{{1, 1}}, 1}));
    REQUIRE_FALSE(MinePrediction{{{1, 1}}, 1}.is_subset_of(
            MinePrediction{{{1, 2}}, 1}));
    REQUIRE(MinePrediction{{{1, 1}}, 1}.is_subset_of(
            MinePrediction{{{1, 1}, {1, 2}}, 1}));
}

TEST_CASE("predict_mines for easy case") {
    // clang-format off
    std::array<Cell, 4> const buffer{
            Cell::Hidden, Cell::One,
            Cell::One, Cell::One};
    // clang-format on
    auto const predictions = predict_mines(stdex::mdspan{buffer.data(), 2, 2});
    REQUIRE(predictions == std::vector{MinePrediction{{{0, 0}}, 1}});
}

}  // namespace