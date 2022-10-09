#include "predict_mines.h"
#include "algorithm2d.h"
#include <algorithm>
#include <list>

namespace stdex = std::experimental;

namespace asw {

bool MinePrediction::is_subset_of(MinePrediction const& other) const {
    if (cells.size() > other.cells.size()) {
        return false;
    }
    // O(N²) :(
    return std::ranges::all_of(cells, [other](Position const& cell) -> bool {
        return std::ranges::find(other.cells, cell) != other.cells.cend();
    });
}

namespace {

MinePrediction get_prediction(
        std::size_t const row,
        std::size_t const column,
        Cell const cell,
        stdex::mdspan<Cell const, stdex::dextents<2>> const& field) {
    MinePrediction prediction{{}, static_cast<int>(cell)};
    auto const rows = field.extent(0);
    auto const columns = field.extent(1);
    auto const r_begin = row == 0 ? 0 : row - 1;
    auto const r_end = row + 1 == rows ? rows : row + 2;
    auto const c_begin = column == 0 ? 0 : column - 1;
    auto const c_end = column + 1 == columns ? columns : column + 2;
    indexed_for_each(
            stdex::submdspan(
                    field,
                    std::pair{r_begin, r_end},
                    std::pair{c_begin, c_end}),
            [&prediction](
                    std::size_t const row,
                    std::size_t const column,
                    Cell const cell) {
                if (cell == Cell::Hidden) {
                    prediction.cells.emplace_back(row, column);
                }
            });
    return prediction;
}

bool can_make_prediction(Cell const cell) {
    return static_cast<std::underlying_type_t<Cell>>(cell) < 9;
}

void consolidate(std::list<MinePrediction>& predictions) {
    if (predictions.size() <= 1) {
        return;
    }
    for (auto i = predictions.begin();
         i != std::prev(predictions.end()) && i != predictions.end();
         ++i) {
        for (auto k = std::next(i); k != predictions.end();) {
            if (k->is_subset_of(*i)) {
                k = predictions.erase(k);
            } else {
                ++k;
            }
        }
    }
}

}  // namespace

std::list<MinePrediction>
predict_mines(stdex::mdspan<Cell const, stdex::dextents<2>> const& field) {
    std::list<MinePrediction> predictions;
    indexed_for_each(
            field,
            [&predictions, &field](
                    std::size_t const row,
                    std::size_t const column,
                    Cell const cell) {
                if (!can_make_prediction(cell)) {
                    return;
                }
                predictions.push_back(get_prediction(row, column, cell, field));
            });
    consolidate(predictions);
    return predictions;
}

Vector2d<Prediction> predict_mines_field(ConstCellSpan const& field) {
    Vector2d<Prediction> prediction_field{field.extent(0), field.extent(1)};
    for (auto const& prediction: predict_mines(field)) {
        for (auto const cell: prediction.cells) {
            prediction_field(cell.row, cell.column) =
                    prediction.mine_count == 0 ? Prediction::Safe
                                               : Prediction::Unsafe;
        }
    }
    indexed_for_each(
            field,
            [&prediction_field](
                    std::size_t const row,
                    std::size_t const column,
                    Cell const cell) {
                if (can_make_prediction(cell)) {
                    prediction_field(row, column) = Prediction::Safe;
                }
            });
    return prediction_field;
}

}  // namespace asw
