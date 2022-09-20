#pragma once

#include <optional>

namespace asw {
class PlayingFieldWithMines;
}  // namespace asw

namespace asw {

struct SafeField final {
    std::size_t row;
    std::size_t column;

    friend bool operator==(SafeField const&, SafeField const&) = default;
};

std::optional<SafeField>
find_first_safe_field(PlayingFieldWithMines const& field);

}  // namespace asw