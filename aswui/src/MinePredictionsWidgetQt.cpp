#include "MinePredictionsWidgetQt.h"
#include "CellsWidgetQt.h"
#include "MinePredictionsPresenter.h"
#include <QVBoxLayout>

namespace aswui {

namespace {

struct MinePredictionsViewImpl {
    CellsWidgetQt& table;

    void set_row_count(int const rows) {
        table.setRowCount(rows);
    }

    void set_column_count(int columns) {
        table.setColumnCount(columns);
    }

    void set_cell(int const row, int const column, QString const& text) {
        table.setCellText(row, column, text);
    }
};

}  // namespace

class MinePredictionsWidgetQt::Impl {
  public:
    explicit Impl(MinePredictionsWidgetQt& widget)
        : table_{new CellsWidgetQt{&widget}},
          view_impl_{*table_},
          presenter_{view_impl_} {
        auto* const layout = new QVBoxLayout{&widget};
        layout->addWidget(table_);
        QObject::connect(
                table_,
                &QTableWidget::cellClicked,
                &widget,
                &MinePredictionsWidgetQt::clicked);
    }

    void set(asw::ConstPredictionSpan const& predictions) {
        presenter_.set(predictions);
    }

  private:
    gsl::strict_not_null<CellsWidgetQt*> table_;
    MinePredictionsViewImpl view_impl_;
    MinePredictionsPresenter<MinePredictionsViewImpl> presenter_;
};

MinePredictionsWidgetQt::MinePredictionsWidgetQt(QWidget* const parent)
    : QWidget{parent},
      impl_{std::make_unique<Impl>(*this)} {
}

MinePredictionsWidgetQt::MinePredictionsWidgetQt(
        asw::ConstPredictionSpan const& predictions,
        QWidget* const parent)
    : MinePredictionsWidgetQt{parent} {
    set(predictions);
}

MinePredictionsWidgetQt::~MinePredictionsWidgetQt() = default;

void MinePredictionsWidgetQt::set(asw::ConstPredictionSpan const& predictions) {
    impl_->set(predictions);
}

}  // namespace aswui