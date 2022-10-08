#include "MinePredictionsWidgetQt.h"
#include "CellsWidgetQt.h"
#include "MinePredictionsPresenter.h"

namespace aswui {

class MinePredictionsWidgetQt::Impl final : public MinePredictionsView {
  public:
    explicit Impl(MinePredictionsWidgetQt& widget)
        : presenter_{*this},
          table_{new CellsWidgetQt{&widget}} {
    }

    ~Impl() override = default;

    void
    set(std::size_t const rows,
        std::size_t const columns,
        std::list<asw::MinePrediction> const& predictions) {
        presenter_.set(rows, columns, predictions);
    }

  private:
    void set_row_count(int const rows) override {
        table_->setRowCount(rows);
    }

    void set_column_count(int columns) override {
        table_->setColumnCount(columns);
    }

  private:
    MinePredictionsPresenter presenter_;
    gsl::strict_not_null<CellsWidgetQt*> table_;
};

MinePredictionsWidgetQt::MinePredictionsWidgetQt(QWidget* const parent)
    : QWidget{parent},
      impl_{std::make_unique<Impl>(*this)} {
}

MinePredictionsWidgetQt::~MinePredictionsWidgetQt() = default;

void MinePredictionsWidgetQt::set(
        std::size_t const rows,
        std::size_t const columns,
        std::list<asw::MinePrediction> const& predictions) {
    impl_->set(rows, columns, predictions);
}

}  // namespace aswui