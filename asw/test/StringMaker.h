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

template<>
struct Catch::StringMaker<asw::Position> {
    static std::string convert(asw::Position const position) {
        return fmt::format("{},{}", position.row, position.column);
    }
};


namespace detail {

inline char to_char(asw::Prediction const prediction) {
    using enum asw::Prediction;
    switch (prediction) {
        case Hidden:
            return ' ';
        case Unknown:
            return '?';
        case Safe:
            return 's';
        case Unsafe:
            return 'X';
        default:;
            return gsl::narrow_cast<char>(
                    gsl::narrow_cast<int>('0') +
                    gsl::narrow_cast<int>(prediction));
    }
}

}  // namespace detail

template<>
struct Catch::StringMaker<asw::Vector2d<asw::Prediction>> {
    static std::string
    convert(asw::Vector2d<asw::Prediction> const& predictions) {
        auto const span = predictions.cspan();
        auto const rows = span.extent(0);
        auto const columns = span.extent(1);
        std::string formatted;
        for (size_t i = 0; i < rows; ++i) {
            auto const row_span = std::experimental::submdspan(
                    span, i, std::pair{0, columns + 1});
            fmt::format_to(
                    std::back_inserter(formatted),
                    "{}",
                    fmt::join(
                            std::views::transform(
                                    std::span{row_span.data(), row_span.size()},
                                    detail::to_char),
                            " "));
            if (i != rows - 1) {
                formatted.push_back('\n');
            }
        }
        return formatted;
    }
};