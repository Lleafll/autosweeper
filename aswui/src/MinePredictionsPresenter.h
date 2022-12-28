#pragma once

#include <QString>
#include <asw/algorithm2d.h>
#include <asw/predict_mines.h>
#include <gsl/narrow>

namespace aswui {

template<class T>
concept MinePredictionsView = requires(T t) {
                                  { t.set_row_count(int{}) };
                                  { t.set_column_count(int{}) };
                                  { t.set_cell(int{}, int{}, QString{}) };
                              };
namespace detail {

QString to_qstring(asw::Prediction prediction);

}  // namespace detail

template<MinePredictionsView T>
class MinePredictionsPresenter final {
  public:
    explicit MinePredictionsPresenter(T& view) : view_{view} {
    }

    void set(asw::ConstPredictionSpan const& predictions) {
        view_.set_row_count(gsl::narrow_cast<int>(predictions.extent(0)));
        view_.set_column_count(gsl::narrow_cast<int>(predictions.extent(1)));
        asw::indexed_for_each(
                predictions,
                [this](asw::Position const& i,
                       asw::Prediction const prediction) {
                    view_.set_cell(
                            gsl::narrow_cast<int>(i.row),
                            gsl::narrow_cast<int>(i.column),
                            detail::to_qstring(prediction));
                });
    }

  private:
    T& view_;
};

}  // namespace aswui