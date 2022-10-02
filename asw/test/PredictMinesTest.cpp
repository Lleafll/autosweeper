#include "PlayingField.h"
#include "predict_mines.h"
#include <catch.hpp>
#include <fmt/format.h>
#include <ranges>

using namespace asw;
namespace stdex = std::experimental;
namespace stdv = std::views;

namespace {

std::string position_to_string(Position const& position) {
    return fmt::format("{},{}", position.row, position.row);
}

}  // namespace

template<>
struct Catch::StringMaker<MinePrediction> {
    static std::string convert(MinePrediction const& cell) {
        return fmt::format(
                "([{}]; mines:{})",
                fmt::join(stdv::transform(cell.cells, position_to_string), " "),
                cell.mine_count);
    }
};

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
    REQUIRE(predictions == std::list{MinePrediction{{{0, 0}}, 1}});
}

}  // namespace