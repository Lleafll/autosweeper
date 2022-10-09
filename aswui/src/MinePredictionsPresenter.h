#pragma once

#include <asw/predict_mines.h>
class QString;

namespace aswui {

class MinePredictionsView {
  public:
    virtual ~MinePredictionsView() = default;

  private:
    virtual void set_row_count(int rows) = 0;
    virtual void set_column_count(int columns) = 0;
    virtual void set_cell(int row, int column, QString const& text) = 0;

    friend class MinePredictionsPresenter;
};

class MinePredictionsPresenter final {
  public:
    explicit MinePredictionsPresenter(MinePredictionsView& view);

    void set(asw::ConstPredictionSpan const& predictions);

  private:
    MinePredictionsView& view_;
};

}  // namespace aswui