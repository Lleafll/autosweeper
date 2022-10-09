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
    auto* const prototype = new QTableWidgetItem{};
    prototype->setTextAlignment(Qt::AlignCenter);
    setItemPrototype(prototype);
}

void CellsWidgetQt::setColumnCount(int const columns) {
    QTableWidget::setColumnCount(columns);
    for (int i = 0; i < columns; ++i) {
        setColumnWidth(i, column_width);
    }
}

void CellsWidgetQt::setCellText(
        int const row,
        int const column,
        QString const& text) {
    auto* const item = itemPrototype()->clone();
    item->setText(text);
    setItem(row, column, item);
}

}  // namespace aswui
