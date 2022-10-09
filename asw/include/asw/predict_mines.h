#pragma once

#include "Array2d.h"
#include "Cell.h"
#include "Position.h"
#include "Vector2d.h"
#include <list>
#include <optional>
#include <unordered_set>

namespace asw {

struct MinePrediction final {
    std::unordered_set<Position> cells;
    int mine_count;

    MinePrediction(std::unordered_set<Position> cells, int mine_count);

    [[nodiscard]] bool is_subset_of(MinePrediction const& other) const;

    bool operator==(MinePrediction const&) const = default;
};

std::optional<MinePrediction>
intersect(MinePrediction const& lhs, MinePrediction const& rhs);

std::list<MinePrediction> predict_mines(ConstCellSpan const& field);

enum class Prediction { Unknown, Safe, Unsafe };

Vector2d<Prediction> predict_mines_field(ConstCellSpan const& field);

template<std::size_t rows, std::size_t columns>
using PredictionArray = Array2d<Prediction, rows, columns>;

using ConstPredictionSpan = std::experimental::
        mdspan<Prediction const, std::experimental::dextents<2>>;

}  // namespace asw