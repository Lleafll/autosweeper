#pragma once

#include <QTableWidget>

namespace aswui {

class CellsWidgetQt final : public QTableWidget {
    Q_OBJECT

  public:
    explicit CellsWidgetQt(QWidget* parent = nullptr);

    void setColumnCount(int columns);
};

}  // namespace aswui