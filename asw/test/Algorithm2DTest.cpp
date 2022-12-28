#include "algorithm2d.h"
#include <catch.hpp>
#include <experimental/mdspan>
#include <fmt/format.h>

namespace stdex = std::experimental;

namespace {

TEST_CASE("indexed_for_each") {
    std::array const buffer{2, 4, 5, 6};
    std::vector<size_t> rows;
    std::vector<size_t> columns;
    std::vector<int> output;
    asw::indexed_for_each(
            stdex::mdspan{buffer.data(), 2, 2},
            [&rows, &columns, &output](
                    asw::Position const& position, int const value) {
                rows.push_back(position.row);
                columns.push_back(position.column);
                output.push_back(value);
            });
    REQUIRE(rows == std::vector<size_t>{0, 0, 1, 1});
    REQUIRE(columns == std::vector<size_t>{0, 1, 0, 1});
    REQUIRE(output == std::vector{2, 4, 5, 6});
}

TEST_CASE("indexed_transform") {
    std::array const input{2, 4, 5, 6};
    std::array<std::string, 4> output{};
    asw::indexed_transform(
            stdex::mdspan{input.data(), 2, 2},
            stdex::mdspan{output.data(), 2, 2},
            [](asw::Position const& i, int const value) -> std::string {
                return fmt::format("{}{}{}", i.row, i.column, value);
            });
    REQUIRE(output == std::array<std::string, 4>{"002", "014", "105", "116"});
}

}  // namespace
