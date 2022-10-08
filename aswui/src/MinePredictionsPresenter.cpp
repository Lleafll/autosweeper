#include "MinePredictionsPresenter.h"
#include <gsl/narrow>

namespace aswui {

MinePredictionsPresenter::MinePredictionsPresenter(MinePredictionsView& view)
    : view_{view} {
}

void MinePredictionsPresenter::set(
        std::size_t const rows,
        std::size_t const columns,
        std::list<asw::MinePrediction> const& predictions) {
    view_.set_row_count(gsl::narrow_cast<int>(rows));
    view_.set_column_count(gsl::narrow_cast<int>(columns));
}

}  // namespace aswui