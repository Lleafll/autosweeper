#include "CellConstSpanPresenter.h"
#include "CellConstSpanWidgetQt.h"
#include "CellsWidgetQt.h"
#include <QVBoxLayout>
#include <gsl/narrow>
#include <gsl/pointers>

namespace aswui {

class CellConstSpanWidgetQt::Impl final : public CellConstSpanView {
  public:
    explicit Impl(CellConstSpanWidgetQt& widget)
        : presenter_{*this},
          table_{new CellsWidgetQt{&widget}} {
        auto* const layout = new QVBoxLayout{&widget};
        layout->addWidget(table_);
        QObject::connect(
                table_,
                &QTableWidget::cellClicked,
                &widget,
                &CellConstSpanWidgetQt::clicked);
    }

    ~Impl() override = default;

    void set(asw::CellConstSpan const& cells) {
        presenter_.set(cells);
    }

  private:
    CellConstSpanPresenter presenter_;
    gsl::strict_not_null<CellsWidgetQt*> table_;

    void set_row_count(int const rows) override {
        table_->setRowCount(rows);
    }

    void set_column_count(int const columns) override {
        table_->setColumnCount(columns);
    }

    void
    set_cell(int const row, int const column, QString const& text) override {
        table_->setCellText(row, column, text);
    }
};

CellConstSpanWidgetQt::CellConstSpanWidgetQt(QWidget* const parent)
    : QWidget{parent},
      impl_{std::make_unique<Impl>(*this)} {
}

CellConstSpanWidgetQt::CellConstSpanWidgetQt(
        asw::CellConstSpan const& cells,
        QWidget* const parent)
    : CellConstSpanWidgetQt{parent} {
    set(cells);
}

CellConstSpanWidgetQt::~CellConstSpanWidgetQt() = default;

void CellConstSpanWidgetQt::set(asw::CellConstSpan const& cells) {
    impl_->set(cells);
}

}  // namespace aswui