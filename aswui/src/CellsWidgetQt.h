#pragma once

#include <QTableWidget>

namespace aswui {

class CellsWidgetQt final : public QTableWidget {
    Q_OBJECT

  public:
    explicit CellsWidgetQt(QWidget* parent = nullptr);

    void setColumnCount(int columns);
    void setCellText(int const row, int const column, QString const& text);
};

}  // namespace aswui