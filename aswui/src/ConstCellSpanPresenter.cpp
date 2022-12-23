#include "CellConstSpanPresenter.h"
#include <QString>
#include <asw/algorithm2d.h>
#include <gsl/narrow>

namespace aswui {

namespace {

QString to_qstring(asw::Cell const cell) {
    switch (cell) {
        case asw::Cell::Empty:
            return "";
        case asw::Cell::One:
            return "1";
        case asw::Cell::Two:
            return "2";
        case asw::Cell::Three:
            return "3";
        case asw::Cell::Four:
            return "4";
        case asw::Cell::Five:
            return "5";
        case asw::Cell::Six:
            return "6";
        case asw::Cell::Seven:
            return "7";
        case asw::Cell::Eight:
            return "8";
        case asw::Cell::Hidden:
            return "☐";
        case asw::Cell::Mine:
            return "💣";
    }
    abort();
}

}  // namespace

CellConstSpanPresenter::CellConstSpanPresenter(CellConstSpanView view)
    : view_{std::move(view)} {
}

void CellConstSpanPresenter::set(asw::CellConstSpan const& cells) {
    view_.set_row_count(gsl::narrow_cast<int>(cells.extent(0)));
    view_.set_column_count(gsl::narrow_cast<int>(cells.extent(1)));
    asw::indexed_for_each(
            cells, [this](asw::Position const& i, asw::Cell const cell) {
                view_.set_cell(
                        gsl::narrow_cast<int>(i.row),
                        gsl::narrow_cast<int>(i.column),
                        to_qstring(cell));
            });
}

}  // namespace aswui