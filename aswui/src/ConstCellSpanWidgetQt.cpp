#include "ConstCellSpanWidgetQt.h"
#include "ConstCellSpanPresenter.h"
#include <QTableWidget>
#include <QVBoxLayout>
#include <gsl/narrow>
#include <gsl/pointers>

namespace aswui {

class ConstCellSpanWidgetQt::Impl final : public ConstCellSpanView {
public:
    explicit Impl(QWidget& widget)
        : presenter_{*this},
          table_{new QTableWidget{&widget}} {
        auto* const layout = new QVBoxLayout{&widget};
        layout->addWidget(table_);
    }

    ~Impl() override = default;

    void set(asw::ConstCellSpan const& cells) {
        presenter_.set(cells);
    }

private:
    void set_row_count(int const rows) override {
        table_->setRowCount(rows);
    }

    void set_column_count(int const columns) override {
        table_->setColumnCount(columns);
    }

    void
    set_cell(int const row, int const column, QString const& text) override {
        table_->setItem(row, column, new QTableWidgetItem{text});
    }

private:
    ConstCellSpanPresenter presenter_;
    gsl::strict_not_null<QTableWidget*> table_;
};

ConstCellSpanWidgetQt::ConstCellSpanWidgetQt(QWidget* const parent)
    : QWidget{parent},
      impl_{std::make_unique<Impl>(*this)} {
}

ConstCellSpanWidgetQt::ConstCellSpanWidgetQt(
        asw::ConstCellSpan const& cells,
        QWidget* const parent)
    : ConstCellSpanWidgetQt{parent} {
    set(cells);
}

ConstCellSpanWidgetQt::~ConstCellSpanWidgetQt() = default;

void ConstCellSpanWidgetQt::set(asw::ConstCellSpan const& cells) {
    impl_->set(cells);
}

}  // namespace aswui