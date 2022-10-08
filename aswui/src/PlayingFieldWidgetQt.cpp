#include "PlayingFieldWidgetQt.h"
#include "PlayingFieldPresenter.h"
#include <QTableWidget>
#include <QVBoxLayout>
#include <gsl/narrow>
#include <gsl/pointers>

namespace aswui {

class PlayingFieldWidgetQt::Impl final : public PlayingFieldView {
public:
    explicit Impl(QWidget& widget)
        : presenter_{*this},
          table_{new QTableWidget{&widget}} {
        auto* const layout = new QVBoxLayout{&widget};
        layout->addWidget(table_);
    }

    ~Impl() override = default;

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
    PlayingFieldPresenter presenter_;
    gsl::strict_not_null<QTableWidget*> table_;
};

PlayingFieldWidgetQt::PlayingFieldWidgetQt(QWidget* const parent)
    : QWidget{parent},
      impl_{std::make_unique<Impl>(*this)} {
}

PlayingFieldWidgetQt::~PlayingFieldWidgetQt() = default;

}  // namespace aswui