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

    ~MinePredictionsWidgetQt() override;

    void
    set(std::size_t rows,
        std::size_t columns,
        std::list<asw::MinePrediction> const& predictions);

  private:
    class Impl;

    gsl::not_null<std::unique_ptr<Impl>> impl_;
};

}  // namespace aswui