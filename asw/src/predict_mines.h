#pragma once

#include "Cell.h"
#include <experimental/mdspan>
#include <vector>

namespace asw {

struct Position final {
    std::size_t row;
    std::size_t column;

    bool operator==(Position const&) const = default;
};

struct MinePrediction final {
    std::vector<Position> cells;
    int mine_count;

    [[nodiscard]] bool is_subset_of(MinePrediction const& other) const;

    bool operator==(MinePrediction const&) const = default;
};

std::vector<MinePrediction>
predict_mines(std::experimental::mdspan<
              Cell const,
              std::experimental::dextents<2>> const& field);

}  // namespace asw