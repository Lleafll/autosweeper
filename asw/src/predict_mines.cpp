#include "predict_mines.h"
#include "algorithm2d.h"
#include <algorithm>
#include <list>

namespace stdex = std::experimental;

namespace asw {

MinePrediction::MinePrediction(
        std::unordered_set<Position> cells,
        int const mine_count)
    : cells{std::move(cells)},
      mine_count{mine_count} {
}

bool MinePrediction::is_subset_of(MinePrediction const& other) const {
    if (cells.size() > other.cells.size()) {
        return false;
    }
    // O(N²) :(
    return std::ranges::all_of(cells, [other](Position const& cell) -> bool {
        return std::ranges::find(other.cells, cell) != other.cells.cend();
    });
}

void MinePrediction::subtract(MinePrediction const& other) {
    auto const count =
            std::erase_if(cells, [&other](Position const& position) -> bool {
                return other.cells.contains(position);
            });
    if (count > 0) {
        mine_count -= other.mine_count;
    }
}

std::optional<MinePrediction>
intersect(MinePrediction const& lhs, MinePrediction const& rhs) {
    std::unordered_set<Position> positions;
    std::ranges::copy_if(
            lhs.cells,
            std::inserter(positions, positions.begin()),
            [&rhs](Position const& cell) -> bool {
                return rhs.cells.contains(cell);
            });
    if (positions.empty()) {
        return std::nullopt;
    } else {
        return std::make_optional<MinePrediction>(
                std::move(positions), std::min(lhs.mine_count, rhs.mine_count));
    }
}

namespace {

MinePrediction get_prediction(
        std::size_t const row,
        std::size_t const column,
        Cell const cell,
        ConstCellSpan const& field) {
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
            [&prediction, r_begin, c_begin](
                    std::size_t const row,
                    std::size_t const column,
                    Cell const cell) {
                if (cell == Cell::Hidden) {
                    prediction.cells.emplace(row + r_begin, column + c_begin);
                }
            });
    return prediction;
}

bool is_revealed_and_not_mine(Cell const cell) {
    return static_cast<std::underlying_type_t<Cell>>(cell) < 9;
}

void consolidate(std::list<MinePrediction>& predictions) {
    if (predictions.size() <= 1) {
        return;
    }
    std::list<MinePrediction> intersections;
    for (auto i = predictions.begin();
         i != std::prev(predictions.end()) && i != predictions.end();
         ++i) {
        for (auto k = std::next(i); k != predictions.end();) {
            if (k->is_subset_of(*i)) {
                k = predictions.erase(k);
            } else {
                auto intersection = intersect(*k, *i);
                if (intersection.has_value()) {
                    intersections.push_back(std::move(*intersection));
                }
                ++k;
            }
        }
    }
    for (auto const& intersection : intersections) {
        for (auto& prediction : predictions) {
            prediction.subtract(intersection);
        }
    }
    std::ranges::move(intersections, std::back_inserter(predictions));
}

}  // namespace

std::list<MinePrediction> predict_mines(ConstCellSpan const& field) {
    std::list<MinePrediction> predictions;
    indexed_for_each(
            field,
            [&predictions, &field](
                    std::size_t const row,
                    std::size_t const column,
                    Cell const cell) {
                if (!is_revealed_and_not_mine(cell)) {
                    return;
                }
                predictions.push_back(get_prediction(row, column, cell, field));
            });
    consolidate(predictions);
    return predictions;
}

namespace {

Prediction safe_status(
        std::list<MinePrediction> const& predictions,
        Position const& position) {
    auto mines = 10;
    for (auto const& prediction: predictions) {
        if (prediction.cells.contains(position)) {
            mines = std::min(mines, prediction.mine_count);
        }
    }
    switch (mines) {
        case 10:
            return Prediction::Unknown;
        case 0:
            return Prediction::Safe;
        default:
            return Prediction::Unsafe;
    }
}

}  // namespace

Vector2d<Prediction> predict_mines_field(ConstCellSpan const& field) {
    auto const predictions = predict_mines(field);
    Vector2d<Prediction> prediction_field{field.extent(0), field.extent(1)};
    indexed_for_each(
            field,
            [&prediction_field, &predictions](
                    std::size_t const row,
                    std::size_t const column,
                    Cell const cell) {
                if (is_revealed_and_not_mine(cell)) {
                    prediction_field(row, column) = Prediction::Safe;
                } else if (cell == Cell::Mine) {
                    prediction_field(row, column) = Prediction::Unsafe;
                } else {
                    prediction_field(row, column) =
                            safe_status(predictions, {row, column});
                }
            });
    return prediction_field;
}

}  // namespace asw
