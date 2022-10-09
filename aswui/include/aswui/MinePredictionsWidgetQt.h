#pragma once

#include <QWidget>
#include <asw/predict_mines.h>
#include <gsl/pointers>
#include <memory>

namespace aswui {

class MinePredictionsWidgetQt final : public QWidget {
    Q_OBJECT

  public:
    explicit MinePredictionsWidgetQt(QWidget* parent);
    MinePredictionsWidgetQt(
            asw::ConstPredictionSpan const& predictions,
            QWidget* parent);

    ~MinePredictionsWidgetQt() override;

    void set(asw::ConstPredictionSpan const& predictions);

  signals:
    void clicked(int row, int column);

  private:
    class Impl;

    gsl::not_null<std::unique_ptr<Impl>> impl_;
};

}  // namespace aswui