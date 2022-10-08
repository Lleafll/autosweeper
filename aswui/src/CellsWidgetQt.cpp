#include "CellsWidgetQt.h"
#include <QHeaderView>

namespace aswui {

namespace {

constexpr int column_width = 10;

}  // namespace

CellsWidgetQt::CellsWidgetQt(QWidget* const parent) : QTableWidget{parent} {
    horizontalHeader()->hide();
    verticalHeader()->hide();
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setFocusPolicy(Qt::NoFocus);
    setSelectionMode(QAbstractItemView::NoSelection);
}

void CellsWidgetQt::setColumnCount(int const columns) {
    QTableWidget::setColumnCount(columns);
    for (int i = 0; i < columns; ++i) {
        setColumnWidth(i, column_width);
    }
}

}  // namespace aswui
