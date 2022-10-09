#pragma once

#include "Cell.h"
#include "Vector2d.h"
#include <list>
#include <vector>

namespace asw {

struct Position final {
    std::size_t row;
    std::size_t column;

    constexpr Position(std::size_t const row, std::size_t const column)
        : row{row},
          column{column} {
    }

    constexpr bool operator==(Position const&) const = default;
};

struct MinePrediction final {
    std::vector<Position> cells;
    int mine_count;

    [[nodiscard]] bool is_subset_of(MinePrediction const& other) const;

    bool operator==(MinePrediction const&) const = default;
};

std::list<MinePrediction> predict_mines(ConstCellSpan const& field);

enum class Prediction { Unknown, Safe, Unsafe };

using PredictionVector = Vector2d<Prediction>;

PredictionVector predict_mines_field(ConstCellSpan const& field);

}  // namespace asw