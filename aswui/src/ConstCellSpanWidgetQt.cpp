#include "CellConstSpanPresenter.h"
#include "CellConstSpanWidgetQt.h"
#include "CellsWidgetQt.h"
#include <QVBoxLayout>
#include <gsl/narrow>
#include <gsl/pointers>

namespace aswui {

class CellConstSpanViewImpl {
  public:
    explicit CellConstSpanViewImpl(CellConstSpanWidgetQt& widget)
        : table_{new CellsWidgetQt{&widget}} {
        auto* const layout = new QVBoxLayout{&widget};
        layout->addWidget(table_);
        QObject::connect(
                table_,
                &QTableWidget::cellClicked,
                &widget,
                &CellConstSpanWidgetQt::clicked);
    }

    void set_row_count(int const rows) {
        table_->setRowCount(rows);
    }

    void set_column_count(int const columns) {
        table_->setColumnCount(columns);
    }

    void set_cell(int const row, int const column, QString const& text) {
        table_->setCellText(row, column, text);
    }

  private:
    gsl::strict_not_null<CellsWidgetQt*> table_;
};

CellConstSpanWidgetQt::CellConstSpanWidgetQt(QWidget* const parent)
    : QWidget{parent},
      presenter_{std::make_unique<CellConstSpanPresenter>(
              CellConstSpanViewImpl{*this})} {
}

CellConstSpanWidgetQt::CellConstSpanWidgetQt(
        asw::CellConstSpan const& cells,
        QWidget* const parent)
    : CellConstSpanWidgetQt{parent} {
    set(cells);
}

CellConstSpanWidgetQt::~CellConstSpanWidgetQt() = default;

void CellConstSpanWidgetQt::set(asw::CellConstSpan const& cells) {
    presenter_->set(cells);
}

}  // namespace aswui