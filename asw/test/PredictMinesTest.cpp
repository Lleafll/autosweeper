#include "StringMaker.h"
#include "predict_mines.h"
#include <catch.hpp>

using asw::Array2d;
using asw::Cell;
using asw::FixedArray2d;
using asw::Prediction;
using asw::Size;

namespace {

TEST_CASE("predict_mines_field") {
    constexpr Array2d<Cell, 2, 2> buffer{// clang-format off
            Cell::Hidden, Cell::One,
            Cell::One,    Cell::One};  // clang-format on
    auto const prediction = FixedArray2d<Prediction>{
            Size{2, 2},  // clang-format off
            {Prediction::DetectedMine, Prediction::One,
            Prediction::One,           Prediction::One}};  // clang-format on
    REQUIRE(predict_mines_field(buffer.cspan()) == prediction);
}

TEST_CASE("predict_mines_field with revealed empty field") {
    constexpr Array2d<Cell, 1, 3> buffer{
            Cell::Empty, Cell::Hidden, Cell::Hidden};
    auto const expected = FixedArray2d<Prediction>{
            Size{1, 3},
            {Prediction::Empty, Prediction::Safe, Prediction::Unknown}};
    REQUIRE(predict_mines_field(buffer.cspan()) == expected);
}

TEST_CASE("predict_mines_field field between 1 and Clear") {
    constexpr Array2d<Cell, 2, 3> buffer{// clang-format off
            Cell::One,    Cell::Hidden, Cell::Empty,
            Cell::Hidden, Cell::Hidden, Cell::Hidden};  // clang-format on
    using enum Prediction;
    auto const expected = FixedArray2d<Prediction>{
            Size{2, 3},  // clang-format off
            {One,          Safe, Empty,
             DetectedMine, Safe, Safe}};  // clang-format on
    REQUIRE(predict_mines_field(buffer.cspan()) == expected);
}

TEST_CASE("predict_mines_field when not completely revealed") {
    constexpr Array2d<Cell, 3, 3> buffer{// clang-format off
            Cell::One,    Cell::One,    Cell::Empty,
            Cell::Hidden, Cell::Hidden, Cell::Empty,
            Cell::One,    Cell::One,    Cell::Empty};  // clang-format on
    using enum Prediction;
    auto const expected = FixedArray2d<Prediction>{
            Size{3, 3},  // clang-format off
            {One,          One,  Empty,
             DetectedMine, Safe, Empty,
             One,          One,  Empty}};  // clang-format on
    REQUIRE(predict_mines_field(buffer.cspan()) == expected);
}

TEST_CASE("predict_mines_field when one cell can be excluded") {
    constexpr Array2d<Cell, 2, 3> buffer{// clang-format off
            Cell::Two,    Cell::Hidden, Cell::One,
            Cell::Hidden, Cell::Hidden, Cell::Hidden};  // clang-format on
    using enum Prediction;
    auto const expected = FixedArray2d<Prediction>{
            Size{2, 3},  // clang-format off
            {Two,          Unsafe, One,
             DetectedMine, Unsafe, Safe}};  // clang-format on
    REQUIRE(predict_mines_field(buffer.cspan()) == expected);
}

TEST_CASE("predict_mines_field case") {
    constexpr Array2d<Cell, 3, 3> buffer{// clang-format off
            Cell::Two,    Cell::Hidden, Cell::One,
            Cell::Hidden, Cell::Hidden, Cell::One,
            Cell::Hidden, Cell::Two,    Cell::One};  // clang-format on
    using enum Prediction;
    auto const expected = FixedArray2d<Prediction>{
            Size{3, 3},  // clang-format off
            {Two,          Safe,         One,
             DetectedMine, DetectedMine, One,
             Safe,         Two,          One}};  // clang-format on
    REQUIRE(predict_mines_field(buffer.cspan()) == expected);
}

}  // namespace