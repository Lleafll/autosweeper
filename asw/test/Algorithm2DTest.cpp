#include "algorithm2d.h"
#include <catch.hpp>
#include <experimental/mdspan>
#include <fmt/format.h>

using namespace asw;
namespace stdex = std::experimental;

namespace {

TEST_CASE("for_each") {
    std::array const buffer{2, 4, 5, 6};
    std::vector<int> output;
    for_each(stdex::mdspan{buffer.data(), 2, 2}, [&output](int const value) {
        output.push_back(value);
    });
    REQUIRE(output == std::vector{2, 4, 5, 6});
}

TEST_CASE("indexed_for_each") {
    std::array const buffer{2, 4, 5, 6};
    std::vector<std::size_t> rows;
    std::vector<std::size_t> columns;
    std::vector<int> output;
    indexed_for_each(
            stdex::mdspan{buffer.data(), 2, 2},
            [&rows, &columns, &output](
                    std::size_t const row,
                    std::size_t const column,
                    int const value) {
                rows.push_back(row);
                columns.push_back(column);
                output.push_back(value);
            });
    REQUIRE(rows == std::vector<std::size_t>{0, 0, 1, 1});
    REQUIRE(columns == std::vector<std::size_t>{0, 1, 0, 1});
    REQUIRE(output == std::vector{2, 4, 5, 6});
}

TEST_CASE("indexed_transform") {
    std::array const input{2, 4, 5, 6};
    std::array<std::string, 4> output{};
    indexed_transform(
            stdex::mdspan{input.data(), 2, 2},
            stdex::mdspan{output.data(), 2, 2},
            [](std::size_t const row,
               std::size_t const column,
               int const value) -> std::string {
                return fmt::format("{}{}{}", row, column, value);
            });
    REQUIRE(output == std::array<std::string, 4>{"002", "014", "105", "116"});
}

}  // namespace
