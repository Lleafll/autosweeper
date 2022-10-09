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

TEST_CASE("MinePrediction::intersect() when no intersection") {
    REQUIRE(intersect(
                    MinePrediction{{{1, 1}}, 1}, MinePrediction{{{2, 3}}, 1}) ==
            std::nullopt);
}

TEST_CASE("MinePrediction::intersect() when intersection") {
    REQUIRE(intersect(
                    MinePrediction{{{1, 1}}, 1}, MinePrediction{{{2, 3}}, 1}) ==
            std::nullopt);
}

TEST_CASE("MinePrediction::subtract() when there is no intersection") {
    MinePrediction prediction{{{1, 1}, {2, 2}}, 2};
    prediction.subtract(MinePrediction{{{3, 3}}, 1});
    REQUIRE(prediction == MinePrediction{{{1, 1}, {2, 2}}, 2});
}

TEST_CASE("MinePrediction::subtract() when there is a intersection") {
    MinePrediction prediction{{{1, 1}, {2, 2}}, 2};
    prediction.subtract(MinePrediction{{{2, 2}}, 1});
    REQUIRE(prediction == MinePrediction{{{1, 1}}, 1});
}

TEST_CASE("predict_mines for easy case") {
    // clang-format off
    constexpr Array2d<Cell, 2, 2> buffer{
            Cell::Hidden, Cell::One,
            Cell::One,    Cell::One};
    // clang-format on
    auto const predictions = predict_mines(buffer.cspan());
    REQUIRE(predictions == std::list{MinePrediction{{{0, 0}}, 1}});
}

TEST_CASE("predict_mines field between 1 and Empty") {
    constexpr Array2d<Cell, 2, 3> buffer{// clang-format off
            Cell::One,    Cell::Hidden, Cell::Empty,
            Cell::Hidden, Cell::Hidden, Cell::Hidden};  // clang-format on
    auto predictions = predict_mines(buffer.cspan());
    REQUIRE(predictions ==
            std::list<MinePrediction>{
                    {{{1, 0}}, 1}, {{{1, 2}}, 0}, {{{0, 1}, {1, 1}}, 0}});
}

TEST_CASE("predict_mines_field") {
    constexpr Array2d<Cell, 2, 2> buffer{// clang-format off
            Cell::Hidden, Cell::One,
            Cell::One,    Cell::One};  // clang-format on
    auto const prediction = Vector2d<Prediction>{
            2,
            2,  // clang-format off
            {Prediction::Unsafe, Prediction::Safe,
            Prediction::Safe, Prediction::Safe}};  // clang-format on
    REQUIRE(predict_mines_field(buffer.cspan()) == prediction);
}

TEST_CASE("predict_mines_field with revealed empty field") {
    constexpr Array2d<Cell, 1, 3> buffer{
            Cell::Empty, Cell::Hidden, Cell::Hidden};
    auto const expected = Vector2d<Prediction>{
            1, 3, {Prediction::Safe, Prediction::Safe, Prediction::Unknown}};
    REQUIRE(predict_mines_field(buffer.cspan()) == expected);
}

TEST_CASE("predict_mines_field field between 1 and Empty") {
    constexpr Array2d<Cell, 2, 3> buffer{// clang-format off
            Cell::One,    Cell::Hidden, Cell::Empty,
            Cell::Hidden, Cell::Hidden, Cell::Hidden};  // clang-format on
    using enum Prediction;
    auto const expected = Vector2d<Prediction>{
            2,
            3,  // clang-format off
            {Safe, Safe, Safe,
            Unsafe, Safe, Safe}};  // clang-format on
    REQUIRE(predict_mines_field(buffer.cspan()) == expected);
}

}  // namespace