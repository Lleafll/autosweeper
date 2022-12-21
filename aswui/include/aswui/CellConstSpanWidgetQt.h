#pragma once

#include <QWidget>
#include <asw/Cell.h>
#include <gsl/pointers>
#include <memory>

namespace aswui {

class CellConstSpanWidgetQt final : public QWidget {
    Q_OBJECT

  public:
    explicit CellConstSpanWidgetQt(QWidget* parent);
    CellConstSpanWidgetQt(asw::CellConstSpan const& cells, QWidget* parent);

    ~CellConstSpanWidgetQt() override;
    CellConstSpanWidgetQt(CellConstSpanWidgetQt const&) = delete;
    CellConstSpanWidgetQt(CellConstSpanWidgetQt&&) noexcept = delete;
    CellConstSpanWidgetQt& operator=(CellConstSpanWidgetQt const&) = delete;
    CellConstSpanWidgetQt& operator=(CellConstSpanWidgetQt&&) noexcept = delete;

    void set(asw::CellConstSpan const& cells);

  signals:
    void clicked(int row, int column);

  private:
    class Impl;

    gsl::not_null<std::unique_ptr<Impl>> impl_;
};

}  // namespace aswui