#include "PlayingFieldPresenter.h"

namespace aswui {

PlayingFieldPresenter::PlayingFieldPresenter(PlayingFieldView& view)
    : view_{view} {
}

void PlayingFieldPresenter::set(asw::ConstCellSpan const& cells) {
    view_.set_row_count(cells.extent(0));
    view_.set_column_count(cells.extent(1));
}

}  // namespace aswui