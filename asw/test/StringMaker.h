#pragma once

#include "Cell.h"
#include "algorithm2d.h"
#include "predict_mines.h"
#include <catch.hpp>
#include <fmt/format.h>
#include <gsl/narrow>
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
    return fmt::format("{},{}", position.row, position.column);
}

}  // namespace detail

namespace detail {

inline char to_char(asw::Prediction const prediction) {
    switch (prediction) {
        case asw::Prediction::Unknown:
            return '?';
        case asw::Prediction::Safe:
            return 's';
        case asw::Prediction::Unsafe:
            return 'X';
        default:
            break;
    }
    abort();
}

}  // namespace detail

template<>
struct Catch::StringMaker<asw::Vector2d<asw::Prediction>> {
    static std::string
    convert(asw::Vector2d<asw::Prediction> const& predictions) {
        auto const span = predictions.cspan();
        auto const rows = span.extent(0);
        auto const columns = span.extent(1);
        auto const* const data = span.data();
        std::string formatted;
        for (auto i = 0; i < rows; ++i) {
            auto const begin = data + i * columns;
            auto const end = begin + columns;
            fmt::format_to(
                    std::back_inserter(formatted),
                    "{}",
                    fmt::join(
                            std::views::transform(
                                    std::ranges::subrange{begin, end},
                                    detail::to_char),
                            " "));
            if (i != rows - 1) {
                formatted.push_back('\n');
            }
        }
        return formatted;
    }
};