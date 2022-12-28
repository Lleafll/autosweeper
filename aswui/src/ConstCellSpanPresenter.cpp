#include "CellConstSpanPresenter.h"
#include <QString>

namespace aswui::detail {

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

}  // namespace aswui::detail