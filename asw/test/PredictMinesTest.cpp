#include "PlayingField.h"
#include "StringMaker.h"
#include "predict_mines.h"
#include <catch.hpp>

using namespace asw;

namespace {

TEST_CASE("predict_mines_field") {
    constexpr Array2d<Cell, 2, 2> buffer{// clang-format off
            Cell::Hidden, Cell::One,
            Cell::One,    Cell::One};  // clang-format on
    auto const prediction = Vector2d<Prediction>{
            2,
            2,  // clang-format off
            {Prediction::Unsafe, Prediction::Safe,
            Prediction::Safe,    Prediction::Safe}};  // clang-format on
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
            {Safe,   Safe, Safe,
             Unsafe, Safe, Safe}};  // clang-format on
    REQUIRE(predict_mines_field(buffer.cspan()) == expected);
}

TEST_CASE("predict_mines_field when not completely revealed") {
    using enum Cell;
    constexpr Array2d<Cell, 3, 3> buffer{// clang-format off
            One,    One,    Empty,
            Hidden, Hidden, Empty,
            One,    One,    Empty};  // clang-format on
    using enum Prediction;
    auto const expected = Vector2d<Prediction>{
            3,
            3,  // clang-format off
            {Safe,   Safe, Safe,
             Unsafe, Safe, Safe,
             Safe,   Safe, Safe}};  // clang-format on
    REQUIRE(predict_mines_field(buffer.cspan()) == expected);
}

TEST_CASE("predict_mines_field when one cell can be excluded") {
    using enum Cell;
    constexpr Array2d<Cell, 2, 3> buffer{// clang-format off
            Two,    Hidden, One,
            Hidden, Hidden, Hidden};  // clang-format on
    using enum Prediction;
    auto const expected = Vector2d<Prediction>{
            2,
            3,  // clang-format off
            {Safe,   Unsafe, Safe,
             Unsafe, Unsafe, Safe}};  // clang-format on
    REQUIRE(predict_mines_field(buffer.cspan()) == expected);
}

TEST_CASE("predict_mines_field case") {
    using enum Cell;
    constexpr Array2d<Cell, 3, 3> buffer{// clang-format off
            Two,    Hidden, One,
            Hidden, Hidden, One,
            Hidden, Two,    One};  // clang-format on
    using enum Prediction;
    auto const expected = Vector2d<Prediction>{
            3,
            3,  // clang-format off
            {Safe,   Safe,   Safe,
             Unsafe, Unsafe, Safe,
             Safe,   Safe,   Safe}};  // clang-format on
    REQUIRE(predict_mines_field(buffer.cspan()) == expected);
}

}  // namespace