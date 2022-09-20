#include "Solver.h"
#include "MineFinder.h"
#include <cassert>

namespace stdex = std::experimental;

namespace asw {

Solver::Solver(PlayingField& field) : field_{field} {}

namespace {

typedef int i;
int count_surrounding_hidden_mines(
        MineFinder::ConstView const& evaluation_view,
        auto& mine_probability,
        std::size_t const row,
        std::size_t const column) {
    auto const cell = evaluation_view(row, column);
    if (cell == PlayingField::Cell::Mine) {
        return 0;
    }
    assert(cell != PlayingField::Cell::Hidden);
    auto const mine_count =
            static_cast<std::underlying_type_t<PlayingField::Cell>>(cell);
}

}  // namespace

Solver::Result Solver::next() {
    auto const rows = field_.rows();
    auto const columns = field_.columns();
    auto const evaluation = evaluate(field_);
    auto const evaluation_view = evaluation.view();
    std::vector<double> mine_probability_buffer(
            field_.rows() * field_.columns(), -1);
    auto mine_probability = stdex::mdspan<
            double,
            stdex::extents<stdex::dynamic_extent, stdex::dynamic_extent>>{
            mine_probability_buffer.data(), rows, columns};
    for (std::size_t row = 0; row < rows; ++row) {
        for (std::size_t column = 0; column < columns; ++column) {}
    }
    return Result::Success;
}

}  // namespace asw