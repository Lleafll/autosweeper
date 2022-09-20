#include "SafeField.h"
#include "PlayingFieldWithMines.h"
#include <experimental/mdspan>

namespace stdex = std::experimental;

namespace asw {

std::optional<SafeField>
find_first_safe_field(PlayingFieldWithMines const& field) {
    return std::make_optional<SafeField>(100, 100);
}

}  // namespace asw
