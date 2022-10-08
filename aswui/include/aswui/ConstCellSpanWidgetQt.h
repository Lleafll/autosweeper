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

    void set(asw::ConstCellSpan const& cells);

private:
    class Impl;

    gsl::not_null<std::unique_ptr<Impl>> impl_;
};

}  // namespace aswui