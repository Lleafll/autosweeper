#pragma once

#include <QWidget>
#include <gsl/pointers>
#include <memory>

namespace aswui {

class MinePredictionsWidgetQt final : public QWidget {
    Q_OBJECT

  public:
    explicit MinePredictionsWidgetQt(QWidget* parent);

    ~MinePredictionsWidgetQt() override;

  private:
    class Impl;

    gsl::not_null<std::unique_ptr<Impl>> impl_;
};

}  // namespace aswui