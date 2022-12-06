#include "MinePredictionsWidgetQt.h"
#include "CellsWidgetQt.h"
#include "MinePredictionsPresenter.h"
#include <QVBoxLayout>

namespace aswui {

class MinePredictionsWidgetQt::Impl final : public MinePredictionsView {
  public:
    explicit Impl(MinePredictionsWidgetQt& widget)
        : presenter_{*this},
          table_{new CellsWidgetQt{&widget}} {
        auto* const layout = new QVBoxLayout{&widget};
        layout->addWidget(table_);
        QObject::connect(
                table_,
                &QTableWidget::cellClicked,
                &widget,
                &MinePredictionsWidgetQt::clicked);
    }

    ~Impl() override = default;

    void set(asw::ConstPredictionSpan const& predictions) {
        presenter_.set(predictions);
    }

  private:
    MinePredictionsPresenter presenter_;
    gsl::strict_not_null<CellsWidgetQt*> table_;

    void set_row_count(int const rows) override {
        table_->setRowCount(rows);
    }

    void set_column_count(int columns) override {
        table_->setColumnCount(columns);
    }

    void
    set_cell(int const row, int const column, QString const& text) override {
        table_->setCellText(row, column, text);
    }
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