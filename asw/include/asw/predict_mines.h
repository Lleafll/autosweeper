#pragma once

#include "Array2d.h"
#include "Cell.h"
#include "Vector2d.h"
#include <list>
#include <vector>
#include "Position.h"

namespace asw {

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

template<std::size_t rows, std::size_t columns>
using PredictionArray = Array2d<Prediction, rows, columns>;

using ConstPredictionSpan = std::experimental::
        mdspan<Prediction const, std::experimental::dextents<2>>;

}  // namespace asw