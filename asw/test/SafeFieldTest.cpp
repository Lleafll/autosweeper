#include "SafeField.h"
#include "PlayingFieldWithMines.h"
#include <catch.hpp>

using namespace asw;

namespace {

TEST_CASE("find_first_safe_field finds safe field on simple field") {
    PlayingFieldWithMines field{2, 2};
    {
        auto view = field.view();
        view(0, 1) = PlayingField::Cell::Mine;
        view(1, 0) = PlayingField::Cell::Two;
        view(1, 1) = PlayingField::Cell::Mine;
    }
    REQUIRE(find_first_safe_field(field) == SafeField{0, 0});
}

}  // namespace