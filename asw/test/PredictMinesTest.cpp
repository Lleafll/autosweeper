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
    constexpr Array2d<Cell, 2, 2> buffer{
            Cell::Hidden, Cell::One,
            Cell::One, Cell::One};
    // clang-format on
    auto const predictions = predict_mines(buffer.cspan());
    REQUIRE(predictions == std::list{MinePrediction{{{0, 0}}, 1}});
}

TEST_CASE("predict_mines_field") {
    constexpr Array2d<Cell, 2, 2> buffer{// clang-format off
            Cell::Hidden, Cell::One,
            Cell::One, Cell::One};  // clang-format on
    auto const prediction = PredictionVector{
            2,
            2,  // clang-format off
            {Prediction::Unsafe, Prediction::Safe,
            Prediction::Safe, Prediction::Safe}};  // clang-format on
    REQUIRE(predict_mines_field(buffer.cspan()) == prediction);
}

TEST_CASE("predict_mines_field with revealed empty field") {
    constexpr Array2d<Cell, 1, 3> buffer{
            Cell::Empty, Cell::Hidden, Cell::Hidden};
    auto const prediction = PredictionVector{
            1, 3, {Prediction::Safe, Prediction::Safe, Prediction::Unknown}};
    REQUIRE(predict_mines_field(buffer.cspan()) == prediction);
}

}  // namespace