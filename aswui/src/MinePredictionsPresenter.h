#pragma once

#include <asw/predict_mines.h>

namespace aswui {

class MinePredictionsView {
  public:
    virtual ~MinePredictionsView() = default;

  private:
    virtual void set_row_count(int rows) = 0;
    virtual void set_column_count(int columns) = 0;

    friend class MinePredictionsPresenter;
};

class MinePredictionsPresenter final {
  public:
    explicit MinePredictionsPresenter(MinePredictionsView& view);

    void
    set(std::size_t rows,
        std::size_t columns,
        std::list<asw::MinePrediction> const& predictions);

  private:
    MinePredictionsView& view_;
};

}  // namespace aswui