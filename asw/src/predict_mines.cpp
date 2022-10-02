#include "predict_mines.h"
#include <algorithm>

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

std::vector<MinePrediction>
predict_mines(std::experimental::mdspan<
              Cell const,
              std::experimental::dextents<2>> const& field) {
    return {};
}

}  // namespace asw
