#include "SafeField.h"
#include "PlayingFieldWithMines.h"
#include <catch.hpp>

using namespace asw;

template<>
struct Catch::StringMaker<SafeField> {
    static std::string convert(SafeField const& field) {
        std::string buffer;
        auto const view = field.view();
        for (std::size_t column = 0; column < view.extent(1); ++column) {
            for (std::size_t row = 0; row < view.extent(0); ++row) {
                buffer.push_back([&view, row, column]() -> char {
                    switch (view(row, column)) {
                        case SafeField::Cell::Safe:
                            return '#';
                        case SafeField::Cell::Other:
                            return '?';
                    }
                }());
            }
            buffer.push_back('\n');
        }
        return buffer;
    }
};


namespace {

TEST_CASE("find_safe_cells calculates safe cells for a  simple field") {
    PlayingFieldWithMines field{2, 2};
    {
        auto const view = field.view();
        view(0, 1) = PlayingField::Cell::Mine;
        view(1, 0) = PlayingField::Cell::Two;
        view(1, 1) = PlayingField::Cell::Mine;
    }
    SafeField expected{2, 2};
    {
        auto const view = expected.view();
        view(0, 0) = SafeField::Cell::Safe;
    }
    REQUIRE(find_safe_cells(field) == expected);
}

}  // namespace