#pragma once

#include "Cell.h"
#include "predict_mines.h"
#include <catch.hpp>
#include <fmt/format.h>
#include <magic_enum.hpp>
#include <ranges>

template<>
struct Catch::StringMaker<asw::Cell> {
    static std::string convert(asw::Cell const cell) {
        return std::string{magic_enum::enum_name(cell)};
    }
};

namespace detail {

inline std::string position_to_string(asw::Position const& position) {
    return fmt::format("{},{}", position.row, position.row);
}

}  // namespace detail

template<>
struct Catch::StringMaker<asw::MinePrediction> {
    static std::string convert(asw::MinePrediction const& cell) {
        return fmt::format(
                "([{}]; mines:{})",
                fmt::join(
                        std::views::transform(
                                cell.cells, detail::position_to_string),
                        " "),
                cell.mine_count);
    }
};