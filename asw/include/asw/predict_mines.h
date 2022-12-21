#pragma once

#include "Array2d.h"
#include "Cell.h"
#include "Vector2d.h"

namespace asw {

enum class Prediction {
    Empty,
    One,
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Hidden,
    Mine,
    Unknown,
    Safe,
    Unsafe,
    DetectedMine
};

Vector2d<Prediction> predict_mines_field(CellConstSpan const& field);

template<size_t rows, size_t columns>
using PredictionArray = Array2d<Prediction, rows, columns>;

using ConstPredictionSpan = std::experimental::
        mdspan<Prediction const, std::experimental::dextents<size_t, 2>>;

}  // namespace asw