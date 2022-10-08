#include "PlayingField.h"
#include "StringMaker.h"
#include "predict_mines.h"
#include <catch.hpp>

using namespace asw;

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
    constexpr CellArray<2, 2> buffer{
            Cell::Hidden, Cell::One,
            Cell::One, Cell::One};
    // clang-format on
    auto const predictions = predict_mines(buffer.cspan());
    REQUIRE(predictions == std::list{MinePrediction{{{0, 0}}, 1}});
}

}  // namespace