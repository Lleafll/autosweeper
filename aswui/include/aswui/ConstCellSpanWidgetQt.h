#pragma once

#include <QWidget>
#include <asw/Cell.h>
#include <gsl/pointers>
#include <memory>

namespace aswui {

class ConstCellSpanWidgetQt final : public QWidget {
    Q_OBJECT

  public:
    explicit ConstCellSpanWidgetQt(QWidget* parent);
    ConstCellSpanWidgetQt(asw::ConstCellSpan const& cells, QWidget* parent);

    ~ConstCellSpanWidgetQt() override;
    ConstCellSpanWidgetQt(ConstCellSpanWidgetQt const&) = delete;
    ConstCellSpanWidgetQt(ConstCellSpanWidgetQt&&) noexcept = delete;
    ConstCellSpanWidgetQt& operator=(ConstCellSpanWidgetQt const&) = delete;
    ConstCellSpanWidgetQt& operator=(ConstCellSpanWidgetQt&&) noexcept = delete;

    void set(asw::ConstCellSpan const& cells);

  signals:
    void clicked(int row, int column);

  private:
    class Impl;

    gsl::not_null<std::unique_ptr<Impl>> impl_;
};

}  // namespace aswui