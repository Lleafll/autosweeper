#pragma once

#include "Cell.h"
#include <catch.hpp>
#include <magic_enum.hpp>

template<>
struct Catch::StringMaker<asw::Cell> {
    static std::string convert(asw::Cell const cell) {
        return std::string{magic_enum::enum_name(cell)};
    }
};