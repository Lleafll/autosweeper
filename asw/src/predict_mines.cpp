#include "predict_mines.h"
#include "algorithm2d.h"
#include <algorithm>
#include <iterator>
#include <list>
#include <optional>
#include <unordered_set>

namespace stdex = std::experimental;

namespace asw {

namespace {

struct MinePrediction final {
    std::unordered_set<Position> cells;
    int mine_count;

    MinePrediction(std::unordered_set<Position> cells, int const mine_count)
        : cells{std::move(cells)},
          mine_count{mine_count} {
    }

    [[nodiscard]] bool is_subset_of(MinePrediction const& other) const {
        if (cells.size() > other.cells.size()) {
            return false;
        }
        // O(N²) :(
        return std::ranges::all_of(
                cells, [other](Position const& cell) -> bool {
                    return std::ranges::find(other.cells, cell) !=
                           other.cells.cend();
                });
    }

    void subtract(MinePrediction const& other) {
        auto const count = std::erase_if(
                cells, [&other](Position const& position) -> bool {
                    return other.cells.contains(position);
                });
        if (count > 0) {
            mine_count -= other.mine_count;
        }
    }

    bool operator==(MinePrediction const&) const = default;
};

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
    }
    return std::make_optional<MinePrediction>(
            std::move(positions), std::min(lhs.mine_count, rhs.mine_count));
}

MinePrediction get_prediction(
        Position const& position,
        Cell const cell,
        ConstCellSpan const& field) {
    MinePrediction prediction{{}, static_cast<int>(cell)};
    auto const row = position.row;
    auto const column = position.column;
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
                    Position const& i, Cell const cell) {
                if (cell == Cell::Hidden) {
                    prediction.cells.emplace(
                            Position{i.row + r_begin, i.column + c_begin});
                }
            });
    return prediction;
}

bool is_revealed_and_not_mine(Cell const cell) {
    return cell < Cell::Hidden;
}

void calculate_overlap(std::list<MinePrediction>& predictions) {
    for (auto& i: predictions) {
        for (auto& k: predictions) {
            if (i == k) {
                continue;
            }
            if (i.is_subset_of(k)) {
                k.subtract(i);
            } else if (auto overlap = intersect(i, k)) {
                auto remaining = i;
                remaining.subtract(*overlap);
                if (std::cmp_equal(
                            remaining.cells.size(),
                            i.mine_count - k.mine_count)) {
                    auto no_mines = k;
                    no_mines.subtract(*overlap);
                    predictions.emplace_back(std::move(no_mines.cells), 0);
                    k.cells = std::move(overlap->cells);
                }
            }
        }
    }
}

void remove_empty_cells(std::list<MinePrediction>& predictions) {
    for (auto i = predictions.begin(); i != predictions.end();) {
        if (i->cells.empty()) {
            i = predictions.erase(i);
        } else {
            ++i;
        }
    }
}

void remove_duplicates(std::list<MinePrediction>& predictions) {
    for (auto i = predictions.begin();
         i != std::prev(predictions.end()) && i != predictions.end();
         ++i) {
        for (auto k = std::next(i); k != predictions.end();) {
            if (*i == *k) {
                k = predictions.erase(k);
            } else {
                ++k;
            }
        }
    }
}

bool consolidate(std::list<MinePrediction>& predictions) {
    if (predictions.size() <= 1) {
        return false;
    }
    auto const original = predictions;
    calculate_overlap(predictions);
    remove_empty_cells(predictions);
    remove_duplicates(predictions);
    return predictions != original;
}

std::list<MinePrediction> predict_mines(ConstCellSpan const& field) {
    std::list<MinePrediction> predictions;
    indexed_for_each(
            field, [&predictions, &field](Position const& i, Cell const cell) {
                if (!is_revealed_and_not_mine(cell)) {
                    return;
                }
                predictions.push_back(get_prediction(
                        Position{.row = i.row, .column = i.column},
                        cell,
                        field));
            });
    while (consolidate(predictions)) {
    }
    return predictions;
}

Prediction safe_status(
        std::list<MinePrediction> const& predictions,
        Position const& position) {
    static constexpr int no_prediction_possible_sentinel = 10;
    auto mines = no_prediction_possible_sentinel;
    for (auto const& prediction: predictions) {
        if (prediction.cells.contains(position)) {
            mines = std::min(mines, prediction.mine_count);
        }
    }
    switch (mines) {
        case no_prediction_possible_sentinel:
            return Prediction::Unknown;
        case 0:
            return Prediction::Safe;
        default:
            return Prediction::Unsafe;
    }
}

Prediction to_prediction(Cell const cell) {
    using enum Prediction;
    switch (cell) {
        case Cell::Empty:
            return Empty;
        case Cell::One:
            return One;
        case Cell::Two:
            return Two;
        case Cell::Three:
            return Three;
        case Cell::Four:
            return Four;
        case Cell::Five:
            return Five;
        case Cell::Six:
            return Six;
        case Cell::Seven:
            return Seven;
        case Cell::Eight:
            return Eight;
        case Cell::Hidden:
            return Hidden;
        case Cell::Mine:
            return Mine;
    }
    abort();
}

}  // namespace

Vector2d<Prediction> predict_mines_field(ConstCellSpan const& field) {
    auto const predictions = predict_mines(field);
    Vector2d<Prediction> prediction_field{
            Size{.rows = field.extent(0), .columns = field.extent(1)}};
    indexed_for_each(
            field,
            [&prediction_field,
             &predictions](Position const& i, Cell const cell) {
                if (is_revealed_and_not_mine(cell) or cell == Cell::Mine) {
                    prediction_field(i.row, i.column) = to_prediction(cell);
                } else {
                    prediction_field(i.row, i.column) =
                            safe_status(predictions, {i.row, i.column});
                }
            });
    return prediction_field;
}

}  // namespace asw
