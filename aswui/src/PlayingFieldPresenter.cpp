#include "PlayingFieldPresenter.h"
#include <gsl/narrow>

namespace aswui {

PlayingFieldPresenter::PlayingFieldPresenter(PlayingFieldView& view)
    : view_{view} {
}

void PlayingFieldPresenter::set(asw::ConstCellSpan const& cells) {
    view_.set_row_count(gsl::narrow_cast<int>(cells.extent(0)));
    view_.set_column_count(gsl::narrow_cast<int>(cells.extent(1)));
}

}  // namespace aswui